/**
 * Copyright (C) 2022-2024, by Wu Jianhua (toqsxw@outlook.com)
 *
 * This library is distributed under the Apache-2.0 license.
 */

#include "VideoPlayerComponent.h"
#include <shared_mutex>

namespace Immortal
{

struct VideoPlayerContext
{
public:
	VideoPlayerContext(Ref<Demuxer> demuxer, Ref<VideoCodec> decoder, Ref<VideoCodec> audioDecoder = nullptr, int cacheSize = 3);

    ~VideoPlayerContext();

    VideoPlayerContext(const VideoPlayerContext &&other) = delete;

    VideoPlayerContext &operator=(const VideoPlayerContext &&other) = delete;

    void Seek(double seconds, int64_t min, int64_t max);

    Picture GetPicture();

    Picture GetAudioFrame();

    void PopPicture();

    void PopAudioFrame();

public:
    const Vision::DisplayOrientation *GetDisplayOrientation() const
    {
		return decoder->GetProperty<Vision::DisplayOrientation>();
    }

    const String &GetSource() const
    {
        return demuxer->GetSource();
    }

public:
    URef<Thread> demuxerThread;

    std::unique_ptr<ThreadPool> videoThreadPool;

    std::unique_ptr<ThreadPool> audioThreadPool;

    struct
    {
        std::mutex demux;

        std::shared_mutex video;

        std::shared_mutex audio;
    } mutex;

    std::condition_variable condition;

    Ref<VideoCodec> decoder;

    Ref<VideoCodec> audioDecoder;

    Ref<Demuxer> demuxer;

    std::queue<Picture> pictures;

    std::queue<Picture> audioFrames;

    const int kCacheSize;

    struct State
    {
        bool playing = false;
        bool exited = false;
        bool flush = false;
    } state;
};

Vision::Picture AsyncDecode(const Vision::CodedFrame &codedFrame, Vision::Interface::Codec *decoder)
{
    if (codedFrame && decoder->Decode(codedFrame) == CodecError::Success)
    {
        return decoder->GetPicture();
    }

    return Vision::Picture{};
}

VideoPlayerContext::VideoPlayerContext(Ref<Demuxer> demuxer, Ref<VideoCodec> decoder, Ref<VideoCodec> audioDecoder, int cacheSize) :
    demuxerThread{},
    videoThreadPool{ new ThreadPool{1} },
    audioThreadPool{ new ThreadPool{1} },
    decoder{decoder},
    audioDecoder{ audioDecoder },
    demuxer{demuxer},
    state{},
    kCacheSize{cacheSize}
{
    demuxerThread = new Thread{[=, this]() {
        while (true)
        {
            std::unique_lock lock{ mutex.demux };
            condition.wait(lock, [this] {
				return state.exited || ((pictures.size() + videoThreadPool->TaskSize()) < kCacheSize);
            });

            if (state.exited)
            {
                break;
            }

            Vision::CodedFrame codedFrame;
            if (demuxer->Read(&codedFrame) != CodecError::Success)
            {
                continue;
            }

            if (codedFrame.GetType() == MediaType::Subtitle)
            {
                continue;
            }

            if (audioDecoder && codedFrame.GetType() == MediaType::Audio)
            {
                audioThreadPool->Enqueue([=, this] () -> void {
                    Vision::Picture picture = AsyncDecode(codedFrame, audioDecoder);
                    if (picture)
                    {
                        std::unique_lock lock{ mutex.audio };
                        audioFrames.push(picture);
                    }
                });
            }

            if (codedFrame.GetType() == MediaType::Video)
            {
                videoThreadPool->Enqueue([=, this] () -> void {
                    Vision::Picture picture = AsyncDecode(codedFrame, decoder);
                    if (picture)
                    {
                        std::unique_lock lock{ mutex.video };
                        pictures.push(picture);
                    }
                });
            }
        }
    }};

    demuxerThread->Start();
    demuxerThread->SetDescription("VideoDemux");
}

void VideoPlayerContext::Seek(double seconds, int64_t min, int64_t max)
{
    std::unique_lock lock{ mutex.demux };
    videoThreadPool->RemoveTasks();
    audioThreadPool->RemoveTasks();
    videoThreadPool->Join();
    audioThreadPool->Join();

    {
        std::unique_lock lock{ mutex.video };
        pictures = std::queue<Vision::Picture>{};
    }

    {
        std::unique_lock lock{ mutex.audio };
        audioFrames = std::queue<Vision::Picture>{};
    }

    demuxer->Seek(MediaType::Video, seconds, min, max);
    condition.notify_all();
}

VideoPlayerContext::~VideoPlayerContext()
{
    state.exited = true;
    condition.notify_all();

    videoThreadPool->RemoveTasks();
    audioThreadPool->RemoveTasks();
    videoThreadPool->Join();
    audioThreadPool->Join();

    demuxerThread.Reset();
}

Picture VideoPlayerContext::GetPicture()
{
	Picture picture{}; 
    {
		std::shared_lock lock{mutex.video};
		if (!pictures.empty())
        {
			picture = pictures.front();
        }
	}
    if (!picture)
    {
		condition.notify_one();
    }

	return picture;
}

Picture VideoPlayerContext::GetAudioFrame()
{
    std::shared_lock lock{ mutex.audio };
    return audioFrames.empty() ? Vision::Picture{} : audioFrames.front();
}

void VideoPlayerContext::PopPicture()
{
    {
		std::unique_lock lock{mutex.video};
		pictures.pop();
    }
    condition.notify_one();
}

void VideoPlayerContext::PopAudioFrame()
{
    std::unique_lock lock{ mutex.audio };
    if (!audioFrames.empty())
    {
        audioFrames.pop();
    }
}

VideoPlayerComponent::VideoPlayerComponent() :
    player{}
{

}

VideoPlayerComponent::VideoPlayerComponent(const String &path) :
    player{}
{
	Ref<Demuxer>    demuxer       = new Vision::FFDemuxer;
	Ref<VideoCodec> decoder       = new Vision::FFCodec;
	Ref<VideoCodec> audiodDecoder = new Vision::FFCodec;
	if (demuxer->Open(path, decoder, audiodDecoder) != CodecError::Success)
    {
		return;
    }
}

VideoPlayerComponent::VideoPlayerComponent(Ref<Demuxer> demuxer, Ref<VideoCodec> decoder, Ref<VideoCodec> audioDecoder) :
    player{new VideoPlayerContext{ demuxer, decoder, audioDecoder }}
{

}

VideoPlayerComponent::~VideoPlayerComponent()
{
    player.Reset();
}

Picture VideoPlayerComponent::GetPicture()
{
    return player->GetPicture();
}

Picture VideoPlayerComponent::GetAudioFrame()
{
    return player->GetAudioFrame();
}

void VideoPlayerComponent::PopPicture()
{
    player->PopPicture();
}

void VideoPlayerComponent::PopAudioFrame()
{
    player->PopAudioFrame();
}

void VideoPlayerComponent::Seek(double seconds, int64_t min, int64_t max)
{
    player->Seek(seconds, min, max);
}

void VideoPlayerComponent::Swap(VideoPlayerComponent &other)
{
    player.Swap(other.player);
}

Animator *VideoPlayerComponent::GetAnimator() const
{
    return player->decoder->GetAddress<Animator>();
}

const String &VideoPlayerComponent::GetSource() const
{
    return player->GetSource();
}

const Vision::DisplayOrientation *VideoPlayerComponent::GetDisplayOrientation() const
{
	return player->GetDisplayOrientation();
}

}