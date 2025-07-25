#pragma once

namespace Proton
{
inline auto getCharLength(const unsigned char firstByte) -> int
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
} // namespace Proton