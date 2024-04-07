#pragma once

#include <array>
#include <functional>

/*
    The following class tries to imitate the functionality of std::variant.

    These are the main things we need to keep in mind in order to create a similar class:
    1. It must work with multiple user types.
    2. It must create and destroy each type correctly.
    3. It must store the values the most effectively.
    4. A part without internal index must be compile-time friendly.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace cppvariant {

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename... Types>
class Variant {
private:
    // Helper function for a visitor
    template <typename Callable, int CurIdx, typename CurType, typename... NextTypes>
    void TryVisitNext(Callable&& callable);

    // Getter of the biggest size among the available Types
    template <size_t MaxSize, typename CurType, typename... NextTypes>
    inline constexpr static size_t GetMaxSize();

    // Getter of the index of Type among the available Types
    // Return value is -1 if the type is unavailable
    template <int CurIdx, typename Type, typename CurType, typename... NextTypes>
    inline constexpr static int GetIndex();

    // Checker if the Type exists in the list of available Types
    // Return value is false if the type is unavailable
    template <int Idx, typename CurType, typename Type, typename... NextTypes>
    inline constexpr bool HasType();

    // Destroyer of a current object
    template <int CurIdx, typename Type, typename... NextTypes>
    inline void Destroy();

    std::array<std::byte, GetMaxSize<0, Types...>()> _data;
    int _idx = -1;

public:

    // Default constructor
    constexpr Variant() = default;

    // Constructor from a given type
    template<typename T>
    inline constexpr explicit Variant(T&&);

    // Assignment operator from a given type
    template <typename T>
    inline constexpr Variant& operator=(T&&);

    // Getter of a pointer to the underlying value
    template <typename T>
    inline constexpr T* Get();

    // A visitor function, analogue of std::visit
    template <typename Callable>
    void Visit(Callable&&);

    // User-defined destructor
    inline ~Variant();
};

/////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace cppvariant

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "variant_inline.h"

/////////////////////////////////////////////////////////////////////////////////////////////////