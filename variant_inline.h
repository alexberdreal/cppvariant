#pragma once

namespace cppvariant {

template <typename... Types>
template <size_t MaxSize, typename CurType, typename... NextTypes>
inline constexpr size_t Variant<Types...>::GetMaxSize() {
    if constexpr (sizeof(CurType) > MaxSize) {
        if constexpr (sizeof...(NextTypes) == 0) {
            return sizeof(CurType);
        } else {
            return GetMaxSize<sizeof(CurType), NextTypes...>();
        }
    } else {
        if constexpr (sizeof...(NextTypes) == 0) {
            return MaxSize;
        } else {
            return GetMaxSize<MaxSize, NextTypes...>();
        }
    }
}

template <typename... Types>
template <size_t CurIdx, typename Type, typename CurType, typename... NextTypes>
inline constexpr size_t Variant<Types...>::GetIndex() {
    if constexpr (std::is_same_v<Type, CurType>) {
        return CurIdx;
    } else if constexpr (sizeof...(NextTypes) == 0) {
        return -1;
    } else {
        return GetIndex<CurIdx + 1, NextTypes...>();
    }
}

template <typename... Types>
template <size_t Idx, typename Type, typename CurType, typename... NextTypes>
inline constexpr bool Variant<Types...>::HasType() {
    if constexpr (Idx == _idx) {
        if (std::is_same_v<Type, CurType>) {
            return true;
        } else {
            // TODO: throw exception
            return false;
        }
    } else if (sizeof...(NextTypes) == 0) {
        return false;
    } else {
        return HasType<Idx + 1, Type, NextTypes...>();
    }
}

template <typename... Types>
template <size_t CurIdx, size_t Idx, typename Type, typename... NextTypes>
inline void Variant<Types...>::Destroy() {
    if constexpr (CurIdx == Idx) {
        Type* ptr = reinterpret_cast<Type*>(_data.data());
        ptr->~Type();
    } else if constexpr (sizeof...(NextTypes) > 0) {
        Destroy<CurIdx + 1, Idx, NextTypes...>();
    }
}

template<typename... Types>
template<typename T>
inline constexpr Variant<Types...>::Variant(T&& arg) : _idx(GetIndex<0, T, Types...>()) {
    new(_data.data()) T(std::forward<T>(arg));
}

template<typename... Types>
template <typename T>
inline constexpr Variant<Types...>& Variant<Types...>::operator=(T&& arg) {
    _idx = GetIndex<0, T, Types...>();
    new (_data.data()) T(std::forward<T>(arg));
}

template<typename... Types>
inline Variant<Types...>::~Variant() {
    if (_idx != -1) {
        Destroy<0, _idx, Types...>();
    }
}

template<typename... Types>
template <typename T>
inline constexpr T* Variant<Types...>::Get() {
    if (!HasType<0, T, Types...>()) {
        return nullptr;
    }
    return reinterpret_cast<T*>(_data.data());
}

} // namespace cppvariant