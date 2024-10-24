#pragma once

#include "BMP.h"

#ifdef _DEBUG
namespace Immortal
{
namespace Vision
{

template <class T>
requires std::is_same_v<T, uint16_t> || std::is_same_v<T, uint8_t>
void CopyPictureMono2RGBA8(const Picture &srcPicture, Picture &dstPicture)
{
	T *src = (T *)srcPicture.GetData();
	uint8_t  *dst = (uint8_t *)dstPicture.GetData();
	for (int i = 0; i < dstPicture.GetHeight(); i++)
	{
		for (int j = 0; j < dstPicture.GetWidth(); j++, dst += 4, src += 1)
		{
			if constexpr (std::is_same_v<T, uint16_t>)
			{
				dst[0] = (float)src[0] / std::numeric_limits<T>::max() * 255;
			}
			else
			{
				dst[0] = (float)src[0];
			}
			dst[1] = dst[0];
			dst[2] = dst[0];

			dst[3] = 0xff;
		}
	}
}

static inline void SimpleWritePicture(const std::string &path, const Picture &picture)
{
	auto width  = picture.GetWidth();
	auto height = picture.GetHeight();
	Picture dstPicture{ width, height, Format::RGBA8};
	if ((picture.GetFormat() & Format::_10Bits) != Format::None)
	{
		CopyPictureMono2RGBA8<uint16_t>(picture, dstPicture);
	}
	else
	{
		CopyPictureMono2RGBA8<uint8_t>(picture, dstPicture);
	}

	Vision::BMPCodec bmp;
	bmp.Write(path, width, height, 4, dstPicture.GetData());
}

}
}
#endif
