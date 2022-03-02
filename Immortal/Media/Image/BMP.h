#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <string>
#include <memory>

#include "Media/Interface/Codec.h"

namespace Immortal
{
namespace Vision
{

class BMPCodec : public Interface::Codec
{
public:
    using Super = Interface::Codec;

public:
    BMPCodec() :
        Super{ Type::BMP, Format::BGRA8 }
    {
        memset(&identifer, 0, HeaderSize());
    }

    virtual ~BMPCodec()
    {

    }

    virtual uint8_t *Data() const override
    {
        return data.get();
    }

    void Swap(std::unique_ptr<uint8_t> &right)
    {
        data.swap(right);
    }

    size_t HeaderSize()
    {
        return reinterpret_cast<uint8_t *>(&importantColours) - reinterpret_cast<uint8_t *>(&identifer);
    }

    bool Read(const std::string &filename, bool alpha = true);

    bool Write(const std::string &filepath, int width, int height, int depth, uint8_t *data, int align = 0);

private:
    #pragma pack(push, 1)
    uint16_t identifer;
    uint32_t fileSize;
    uint16_t RESERVER_ST;
    uint16_t RESERVER_ND;
    uint32_t offset;
    uint32_t informationSize;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compressionType;
    uint32_t imageSize;
    int32_t  horizontalResolution;
    int32_t  verticalResolution;
    uint32_t coloursNum;
    uint32_t importantColours;
    uint16_t empty;
    #pragma pack(pop)

    std::unique_ptr<uint8_t> data;
};

}
}