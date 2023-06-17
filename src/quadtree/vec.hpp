#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

template <typename T, size_t preallocSize = 10, size_t preallocMul = 2>
class IALVector {
private:
    size_t trueSize;
    void reallign() {
        trueSize = length * preallocMul;
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
        trueSize(preallocSize),
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

    void removeDups() {
        std::sort(&buffer[0], &buffer[length]);
        slice(std::unique(&buffer[0], &buffer[length]));
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

template <typename T, size_t fixedLength>
class IALStaticVector {
public:
    T buffer[fixedLength];
    size_t length;

    IALStaticVector():
        length(0) {}

    ~IALStaticVector() {}

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
        buffer[length] = value;
        length++;
    }

    void slice(T* pos) {
        length = pos - buffer; // Index diff
    }

    void clear(void) {
        length = 0;
    }
};
