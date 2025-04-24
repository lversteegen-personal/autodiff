#ifndef BROADCAST_TYPE_H
#define BROADCAST_TYPE_H
#include <type_traits>

namespace ArrayLibrary
{
    /// @brief NONE: No broadcast possible, ANY: Some broadcast possible, LEFTMIX: Left is supershape of right, RIGHTMIX: Right is supershape of left, MATCH: Shapes match exactly
    enum class BroadcastType : uint8_t
    {
        NONE = 0,
        MIX = 1,
        LEFT = 2,
        LEFTMIX = 3,
        RIGHT = 4,
        RIGHTMIX = 5,
        MATCH = 7
    };

    using INT = std::underlying_type_t<BroadcastType>;

    inline BroadcastType operator|(BroadcastType lhs, BroadcastType rhs)
    {
        return static_cast<BroadcastType>(static_cast<INT>(lhs) | static_cast<INT>(rhs));
    }

    inline BroadcastType operator&(BroadcastType lhs, BroadcastType rhs)
    {
        return static_cast<BroadcastType>(static_cast<INT>(lhs) & static_cast<INT>(rhs));
    }

    inline BroadcastType &operator&=(BroadcastType &lhs, BroadcastType rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    inline BroadcastType &operator|=(BroadcastType &lhs, BroadcastType rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    inline bool operator==(BroadcastType lhs, INT rhs)
    {
        return static_cast<INT>(lhs) == rhs;
    }

    inline bool operator!=(BroadcastType lhs, INT rhs)
    {
        return static_cast<INT>(lhs) != rhs;
    }

    inline bool operator==(INT lhs, BroadcastType rhs)
    {
        return lhs == static_cast<INT>(rhs);
    }

    inline bool operator!=(INT lhs, BroadcastType rhs)
    {
        return lhs != static_cast<INT>(rhs);
    }
}

#endif