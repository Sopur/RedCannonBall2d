#pragma once
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#define VectorPreallocMultiplier 2
#define VecPreallocSize          8
#define packed                   __attribute__((packed))

template <typename T>
class packed IALVector {
private:
    size_t trueSize;
    void reallign() {
        trueSize = length * VectorPreallocMultiplier;
        if (trueSize < VecPreallocSize) trueSize = VecPreallocSize;
        realloc(trueSize);
    }
    void realloc(size_t amount) {
        T* tempbuffer = (T*) malloc(amount * sizeof(T));
        if (tempbuffer == nullptr) return dealloc();
        memcpy(tempbuffer, buffer, length * sizeof(T));
        free(buffer);
        buffer = tempbuffer;
    }

public:
    T* buffer;
    size_t length;

    IALVector(const T* fromBuffer = nullptr, size_t bufferLength = 0):
        trueSize(bufferLength + VecPreallocSize),
        length(bufferLength) {
        buffer = (T*) malloc(trueSize * sizeof(T));
        if (fromBuffer != nullptr && buffer != nullptr) {
            memcpy(buffer, fromBuffer, bufferLength * sizeof(T));
        }
    }
    ~IALVector() {
        free(buffer);
    }

    T& at(size_t index) {
        return buffer[index];
    }

    T& operator[](size_t index) {
        return buffer[index];
    }

    void push_back(T value) {
        if (length == trueSize) reallign();
        buffer[length] = value;
        length++;
    }

    void dealloc() {
        free(buffer);
        buffer = nullptr;
    }

    size_t size() {
        return length;
    }
};
