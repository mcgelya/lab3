#pragma once

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ienum.h"

template <class T>
class RingBufferIterator : public IEnumerator<T> {
public:
    RingBufferIterator(T* buf, int pos, int size, int bufferSize)
        : buf(buf), pos(pos), size(size), bufferSize(bufferSize) {
    }

    bool IsEnd() const override {
        return index == size;
    }

    void MoveNext() override {
        pos = (pos + 1) % bufferSize;
        ++index;
    }

    T& Dereference() override {
        return *(buf + pos);
    }

    int Index() const override {
        return index;
    }

private:
    T* buf;
    const int bufferSize;
    const int size;
    int pos = 0;

    int index = 0;
};

template <class T>
class RingBufferConstIterator : public IConstEnumerator<T> {
public:
    RingBufferConstIterator(const T* buf, int pos, int size, int bufferSize)
        : buf(buf), pos(pos), size(size), bufferSize(bufferSize) {
    }

    bool IsEnd() const override {
        return index == size;
    }

    void MoveNext() override {
        pos = (pos + 1) % bufferSize;
        ++index;
    }

    const T& ConstDereference() const override {
        return *(buf + pos);
    }

    int Index() const override {
        return index;
    }

private:
    const T* buf;
    const int bufferSize;
    const int size;
    int pos = 0;

    int index = 0;
};

template <class T>
class RingBuffer : public IEnumerable<T> {
public:
    RingBuffer(int size) : bufferSize(size) {
        if (size == 0) {
            throw std::out_of_range("size cannot be 0");
        }

        buffer = new T[bufferSize]();
    }

    void Reserve(int sz) {
        if (sz > bufferSize) {
            throw std::out_of_range("Cannot reserve more than bufferSize");
        }
        start = 0;
        end = sz;
        size = sz;
    }

    void Resize(int sz) {
        if (sz < bufferSize) {
            throw std::out_of_range("Cannot decrese size");
        }
        T* newBuffer = new T[sz]();
        if (size != 0) {
            int i = start, ind = 0;
            do {
                newBuffer[ind] = buffer[i];
                i = Next(i);
                ++ind;
            } while (i != end);
        }
        start = 0;
        end = size;
        delete[] buffer;
        buffer = newBuffer;
        bufferSize = sz;
    }

    int GetSize() const {
        return size;
    }

    void PushBack(const T& x) {
        if (size == bufferSize) {
            throw std::out_of_range("Buffer is full");
        }
        buffer[end] = x;
        end = Next(end);
        ++size;
    }

    void PopBack() {
        if (size == 0) {
            throw std::out_of_range("Buffer is empty");
        }
        end = Prev(end);
        --size;
    }

    void PushFront(const T& x) {
        if (size == bufferSize) {
            throw std::out_of_range("Buffer is full");
        }
        if (size != 0) {  // else we can just write to start
            start = Prev(start);
        }
        buffer[start] = x;
        ++size;
    }

    void PopFront() {
        if (size == 0) {
            throw std::out_of_range("Buffer is empty");
        }
        start = Next(start);
        --size;
    }

    const T& GetUnsafeBeforeFirst() const {
        return buffer[Prev(start)];
    }

    const T& GetFirst() const {
        if (size == 0) {
            throw std::out_of_range("Buffer is empty");
        }
        return buffer[start];
    }

    const T& GetUnsafeAfterLast() const {
        return buffer[end];
    }

    const T& GetLast() const {
        if (size == 0) {
            throw std::out_of_range("Buffer is empty");
        }
        return buffer[Prev(end)];
    }

    T& GetRef(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range: " + std::to_string(index) + " " + std::to_string(size));
        }
        return buffer[Next(start, index)];
    }

    const T& Get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range: " + std::to_string(index) + " " + std::to_string(size));
        }
        return buffer[Next(start, index)];
    }

    IEnumerator<T>* GetEnumerator() override {
        return new RingBufferIterator<T>(buffer, start, size, bufferSize);
    }

    IConstEnumerator<T>* GetConstEnumerator() const override {
        return new RingBufferConstIterator<T>(buffer, start, size, bufferSize);
    };

    void VisualizeChunks() const {
        std::vector<std::string> v(bufferSize, "--");
        int i = start, ind = 0;
        do {
            v[i] = std::to_string(Get(ind));
            i = Next(i);
            ++ind;
        } while (i != end);

        std::cout << "[ ";
        for (const std::string& s : v) {
            std::cout << s << " ";
        }
        std::cout << "]";
        std::cout << std::endl;
    }

    void VisualizeMap() const {
        std::vector<std::string> v(bufferSize, "--");
        int i = start, ind = 0;
        do {
            v[i] = "Chunk " + std::to_string(ind);
            i = Next(i);
            ++ind;
        } while (i != end);

        std::cout << "[ ";
        for (const std::string& s : v) {
            std::cout << s << " ";
        }
        std::cout << "]";
        std::cout << std::endl;
    }

private:
    T* buffer;
    int bufferSize;

    int start = 0, end = 0;
    int size = 0;

    int Next(int pos, int n = 1) const {
        return (pos + n) % bufferSize;
    }

    int Prev(int pos, int n = 1) const {
        n %= bufferSize;
        return (pos - n + bufferSize) % bufferSize;
    }
};
