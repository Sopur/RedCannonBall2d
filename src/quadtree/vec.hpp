#pragma once
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#define IAL_VEC_PREALLOC_MUL  2
#define IAL_VEC_PREALLOC_SIZE 8

template <typename T>
class IALVector {
private:
    size_t trueSize;
    void reallign() {
        trueSize = length * IAL_VEC_PREALLOC_MUL;
        grow(trueSize);
    }

    void grow(size_t amount) {
        T* tempbuffer = (T*) std::malloc(amount * sizeof(T));
        std::memcpy(tempbuffer, buffer, length * sizeof(T));
        std::free(buffer);
        buffer = tempbuffer;
    }

    void shrink(size_t amount) {
        T* tempbuffer = (T*) std::malloc(amount * sizeof(T));
        std::memcpy(tempbuffer, buffer, amount * sizeof(T));
        std::free(buffer);
        buffer = tempbuffer;
    }

public:
    T* buffer;
    size_t length;

    IALVector():
        trueSize(IAL_VEC_PREALLOC_SIZE),
        length(0) {
        buffer = (T*) std::malloc(trueSize * sizeof(T));
    }

    ~IALVector() {}

    constexpr T& at(size_t index) {
        return buffer[index];
    }

    constexpr T& operator[](size_t index) {
        return buffer[index];
    }

    constexpr size_t size(void) const {
        return length;
    }

    constexpr T* begin() {
        return &buffer[0];
    }

    constexpr T* end() {
        return &buffer[length];
    }

    inline void push_back(T value) {
        if (length == trueSize) reallign();
        buffer[length] = value;
        length++;
    }

    void slice(T* pos) {
        length = pos - buffer; // Index diff
    }

    void clear(void) {
        length = 0;
    }

    void dealloc(void) {
        std::free(buffer);
    }
};
