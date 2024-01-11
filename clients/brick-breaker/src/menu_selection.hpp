#pragma once

#include <type_traits>

template<typename T>
class MenuSelection {
public:
    MenuSelection() {
        static_assert(std::is_signed_v<T>);
    }

    MenuSelection(T size)
        : size(size) {
        static_assert(std::is_signed_v<T>);
    }

    void up() {
        selection = selection - static_cast<T>(1) < static_cast<T>(0) ? size - static_cast<T>(1) : selection - static_cast<T>(1);
    }

    void down() {
        selection = (selection + static_cast<T>(1)) % static_cast<int>(size);
    }

    T get() const { return selection; }
private:
    T selection {};
    T size {};
};
