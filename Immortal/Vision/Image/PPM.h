#pragma once

#include "Vision/Codec.h"

namespace Immortal
{
namespace Vision
{

class PPMCodec : public Interface::Codec
{
public:
    using Super = Interface::Codec;

public:
    PPMCodec();

    virtual ~PPMCodec();

    virtual CodecError Decode(const CodedFrame &codedFrame) override;

    virtual CodecError Encode(const Picture &picture, CodedFrame &codedFrame) override;
};

}
}
