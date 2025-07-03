#pragma once

namespace Proton
{
    inline unsigned int getCharLength(unsigned char firstByte)
    {
        if (firstByte < 0x80)
            return 1;
        if ((firstByte & 0xE0) == 0xC0)
            return 2;
        if ((firstByte & 0xF0) == 0xE0)
            return 3;
        if ((firstByte & 0xF8) == 0xF0)
            return 4;
        return 1;
    }
}