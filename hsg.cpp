#ifndef _SHG_HPP
#define _SHG_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace shg {
    inline uint64_t hash(uint64_t x, uint64_t y) {
        return (x << 32) | y;
    }

    template <typename T, size_t N>
    class StaticVector {
    protected:
        size_t cursor = 0;

    public:
        T data[N];

        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T* iterator;
        typedef const T* const_iterator;

        const_iterator begin() const {
            return data;
        }

        iterator begin() {
            return data;
        }

        const_iterator end() const {
            return data + cursor;
        }

        iterator end() {
            return data + cursor;
        }

        const_iterator cbegin() const {
            return data;
        }

        const_iterator cend() const {
            return data + cursor;
        }

        size_t size() const {
            return cursor;
        }

        size_t max_size() const {
            return N;
        }

        bool empty() const {
            return cursor == 0;
        }

        const_reference operator[](size_t index) const {
            return data[index];
        }

        reference operator[](size_t index) {
            return data[index];
        }

        void push_back(const T& value) {
            data[cursor++] = value;
        }

        void push_back(T&& value) {
            data[cursor++] = value;
        }

        iterator erase(iterator pos) {
            if (pos != this->end()) {
                for (iterator it = pos; it + 1 <= this->end(); ++it) {
                    if (it + 1 == this->end()) {
                        it->~T();
                    } else {
                        *it = std::move(*(it + 1));
                    }
                }

                --cursor;

                return pos;
            } else {
                if (cursor) {
                    --cursor;
                }
                return this->end();
            }
        }

        bool contains(const T& value) const {
            for (size_t i = 0; i < cursor; ++i) {
                if (data[i] == value) {
                    return true;
                }
            }
            return false;
        }

        void clear() {
            for (size_t i = 0; i < cursor; ++i) {
                data[i].~T();
            }
            cursor = 0;
        }
    };

    template <typename Key, typename T>
    class HashMap {
    protected:
        std::vector<T> data;

    public:
        typedef Key key_type;
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef typename decltype(data)::iterator iterator;
        typedef typename decltype(data)::const_iterator const_iterator;

        HashMap(size_t max_size):
            data(max_size) {}

        const_iterator begin() const {
            return data.begin();
        }

        iterator begin() {
            return data.begin();
        }

        const_iterator end() const {
            return data.end();
        }

        iterator end() {
            return data.end();
        }

        const_iterator cbegin() const {
            return data.cbegin();
        }

        const_iterator cend() const {
            return data.cend();
        }

        size_t max_size() const {
            return data.size();
        }

        reference operator[](const Key& key) {
            return data[std::hash<Key>()(key) % this->max_size()];
        }

        const_iterator find(const Key& key) const {
            return data.cbegin() + (std::hash<Key>()(key) % this->max_size());
        }

        iterator find(const Key& key) {
            return data.begin() + (std::hash<Key>()(key) % this->max_size());
        }

        // WARNING: This clears all data in the hash map
        void reserve(size_t size) {
            data.resize(size);
            std::fill(data.begin(), data.end(), T());
        }
    };

    template <typename Entry, size_t CellMaxSize = 10>
    class SpatialHashGrid {
    protected:
        uint8_t magic_number;
        HashMap<uint64_t, StaticVector<Entry, CellMaxSize>> cells;

    public:
        typedef Entry entry_type;

        SpatialHashGrid(uint32_t width, uint32_t height, uint16_t cell_size):
            magic_number(std::round(std::log2(cell_size))),
            cells((width >> magic_number) * (height >> magic_number)) {}

        SpatialHashGrid(uint16_t cell_size, size_t memory_usage):
            magic_number(std::round(std::log2(cell_size))),
            cells(memory_usage / sizeof(typename decltype(cells)::value_type)) {}

        size_t memory_usage() const {
            return cells.max_size() * sizeof(typename decltype(cells)::value_type);
        }

        void insert(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Entry& entry) {
            uint32_t sx = x >> magic_number;
            uint32_t sy = y >> magic_number;
            uint32_t ex = (x + width) >> magic_number;
            uint32_t ey = (y + height) >> magic_number;
            for (uint32_t row = sy; row <= ey; ++row) {
                for (uint32_t column = sx; column <= ex; ++column) {
                    cells[hash(column, row)].push_back(entry);
                }
            }
        }

        void erase(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Entry& entry) {
            uint32_t sx = x >> magic_number;
            uint32_t sy = y >> magic_number;
            uint32_t ex = (x + width) >> magic_number;
            uint32_t ey = (y + height) >> magic_number;
            for (uint32_t row = sy; row <= ey; ++row) {
                for (uint32_t column = sx; column <= ex; ++column) {
                    typename decltype(cells)::reference cell = cells[hash(column, row)];
                    cell.erase(std::find(cell.begin(), cell.end(), entry));
                }
            }
        }

        void erase(const Entry& entry) {
            for (auto& cell : cells) {
                cell.erase(std::find(cell.begin(), cell.end(), entry));
            }
        }

        std::vector<Entry> find(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const {
            uint32_t sx = x >> magic_number;
            uint32_t sy = y >> magic_number;
            uint32_t ex = (x + width) >> magic_number;
            uint32_t ey = (y + height) >> magic_number;
            std::vector<Entry> ret;
            for (uint32_t row = sy; row <= ey; ++row) {
                for (uint32_t column = sx; column <= ex; ++column) {
                    typename decltype(cells)::const_iterator cell = cells.find(hash(column, row));
                    ret.insert(ret.end(), cell->begin(), cell->end());
                }
            }
            std::sort(ret.begin(), ret.end());
            ret.erase(std::unique(ret.begin(), ret.end()), ret.end());
            return ret;
        }

        size_t max_size() const {
            return cells.max_size();
        }

        // WARNING: This clears all data in the spatial hash grid
        void resize(uint32_t width, uint32_t height) {
            cells.resize((width >> magic_number) * (height >> magic_number));
        }

        void clear() {
            for (auto& cell : cells) {
                cell.clear();
            }
        }
    };
} // namespace shg

#endif