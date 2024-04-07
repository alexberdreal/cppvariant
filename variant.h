#pragma once

#include <array>

/*
    The following class tries to imitate the functionality of std::variant.

    These are the main things we need to remember about in order to create a similar class:
    1. It must work with multiple user types simultaneously.
    2. It must create and destroy each type correctly.
    3. It must store the values the most effectively.
    4. It must be compile time friendly.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace cppvariant {

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename... Types>
class Variant {
private:
    template <size_t MaxSize, typename CurType, typename... NextTypes>
    inline constexpr static size_t GetMaxSize();

    template <size_t CurIdx, typename Type, typename CurType, typename... NextTypes>
    inline constexpr static size_t GetIndex();

    template <size_t Idx, typename CurType, typename Type, typename... NextTypes>
    inline constexpr bool HasType();

    template <size_t CurIdx, size_t Idx, typename Type, typename... NextTypes>
    inline void Destroy();

    // Here is
    std::array<std::byte, GetMaxSize<0, Types...>()> _data;
    size_t _idx;

    public:

    template<typename T>
    inline constexpr explicit Variant(T&&);

    template <typename T>
    inline constexpr Variant& operator=(T&&);

    template <typename T>
    inline constexpr T* Get();

    inline ~Variant();
};

/////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace cppvariant

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "variant_inline.h"

/////////////////////////////////////////////////////////////////////////////////////////////////