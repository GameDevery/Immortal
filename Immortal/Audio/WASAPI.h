/**
 * Copyright (C) 2022, by Wu Jianhua (toqsxw@outlook.com)
 *
 * This library is distributed under the Apache-2.0 license.
 */

#pragma once

#ifndef WASAPI_CONTEXT_H_
#define WASAPI_CONTEXT_H_

#include "AudioRenderContext.h"
#include "Platform/D3D/Interface.h"
#include <audioclient.h>
#include <mmdeviceapi.h>

namespace Immortal
{

class WASAPIContext : public AudioRenderContext
{
public:
    using Super = AudioRenderContext;

public:
    WASAPIContext();

    virtual ~WASAPIContext();

    virtual void OpenDevice() override;

    virtual void Begin() override;

    virtual void End() override;

    virtual void Reset() override;

    virtual void PlaySamples(uint32_t numberSamples, const uint8_t *pData) override;

    void Release();

protected:
    ComPtr<IMMDeviceEnumerator> enumerator;

    ComPtr<IMMDevice> handle;

    ComPtr<IAudioClient> audioClient;

    ComPtr<IAudioRenderClient> renderClient;

    ComPtr<IAudioClock> clock;

    WAVEFORMATEX *waveFormat;
};

}

#endif