#pragma once

#include "Graphics/GPUEvent.h"

namespace Immortal
{
namespace OpenGL
{

class IMMORTAL_API GPUEvent : public SuperGPUEvent
{
public:
	GPUEvent();

	virtual ~GPUEvent() override;

	virtual void Signal(uint64_t value) override;

	virtual void Wait(uint64_t value, uint64_t timeout) override;

	virtual void Wait(uint64_t timeout) override;

	virtual uint64_t GetCompletionValue() override;

	virtual uint64_t GetSyncPoint() override;

protected:
	uint64_t value;
};

}
}
