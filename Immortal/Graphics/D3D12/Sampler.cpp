#include "Sampler.h"

namespace Immortal
{
namespace D3D12
{

static inline D3D12_FILTER CAST(Filter filter)
{
	switch (filter)
	{
		case Immortal::Filter::Linear:
		case Immortal::Filter::Bilinear:
			return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		case Immortal::Filter::Anisotropic:
			return D3D12_FILTER_MINIMUM_ANISOTROPIC;
	    case Immortal::Filter::None:
		case Immortal::Filter::Nearest:
		default:
			return D3D12_FILTER_MIN_MAG_MIP_POINT;
	}
}

static inline D3D12_TEXTURE_ADDRESS_MODE CAST(AddressMode addressMode)
{
	return D3D12_TEXTURE_ADDRESS_MODE(addressMode);
}

static inline D3D12_COMPARISON_FUNC CAST(CompareOperation compareOperation)
{
	return D3D12_COMPARISON_FUNC(compareOperation);
}

Sampler::Sampler(Device *device, Filter filter, AddressMode _addressMode, CompareOperation compareOperation, float minLod, float maxLod) :
    NonDispatchableHandle{ device }
{
	D3D12_TEXTURE_ADDRESS_MODE addressMode = CAST(_addressMode);
	D3D12_SAMPLER_DESC desc{
		.Filter         = CAST(filter),
		.AddressU       = addressMode,
		.AddressV       = addressMode,
		.AddressW       = addressMode,
		.MipLODBias     = 0,
		.MaxAnisotropy  = 1,
		.ComparisonFunc = CAST(compareOperation),
		.BorderColor    = { 0, 0, 0, 0 },
		.MinLOD         = minLod,
		.MaxLOD         = maxLod,
	};

	device->CreateSampler(&desc, &handle);
}

Sampler::~Sampler()
{

}

}
}
