#pragma once

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ienum.h"
#include "ring_buffer.h"

template <class T>
using Chunk = RingBuffer<T>;

template <class T>
using Map = RingBuffer<Chunk<T>*>;

template <class T>
class DequeIterator : public IEnumerator<T> {
public:
    DequeIterator(IEnumerator<Chunk<T>*>* start, int size) : bufferIt(start), size(size) {
        if (size != 0) {
            chunkIt = bufferIt->Dereference()->GetEnumerator();
        }
    }

    bool IsEnd() const override {
        return index == size;
    }

    void MoveNext() override {
        assert(!IsEnd());

        chunkIt->MoveNext();
        ++index;
        if (chunkIt->IsEnd() && !IsEnd()) {
            bufferIt->MoveNext();
            chunkIt = bufferIt->Dereference()->GetEnumerator();
        }
    }

    T& Dereference() override {
        return chunkIt->Dereference();
    }

    int Index() const override {
        return index;
    }

private:
    IEnumerator<Chunk<T>*>* bufferIt;
    IEnumerator<T>* chunkIt = nullptr;

    const int size;

    int index = 0;
};

template <class T>
class DequeConstIterator : public IConstEnumerator<T> {
public:
    DequeConstIterator(IConstEnumerator<Chunk<T>*>* start, int size) : bufferIt(start), size(size) {
        if (size != 0) {
            chunkIt = bufferIt->ConstDereference()->GetConstEnumerator();
        }
    }

    bool IsEnd() const override {
        return index == size;
    }

    void MoveNext() override {
        assert(!IsEnd());

        chunkIt->MoveNext();
        ++index;
        if (chunkIt->IsEnd() && !IsEnd()) {
            bufferIt->MoveNext();
            chunkIt = bufferIt->ConstDereference()->GetConstEnumerator();
        }
    }

    const T& ConstDereference() const override {
        return chunkIt->ConstDereference();
    }

    int Index() const override {
        return index;
    }

private:
    IConstEnumerator<Chunk<T>*>* bufferIt;
    IConstEnumerator<T>* chunkIt = nullptr;

    const int size;

    int index = 0;
};

template <class T>
class Deque : public IEnumerable<T> {
public:
    Deque(int sz = 0, int bufferSize = 2, int chunkSize = 1024)
        : size(sz), bufferSize(bufferSize), chunkSize(chunkSize) {
        if (bufferSize <= 0) {
            throw std::out_of_range("Invalid bufferSize");
        }
        if (chunkSize <= 0) {
            throw std::out_of_range("Invalid chunkSize");
        }

        buffer = new Map<T>(bufferSize);
        if (size > 0) {
            int chunks = (size + chunkSize - 1) / chunkSize;
            int lastChunkSize = size % chunkSize;
            if (lastChunkSize == 0) {
                lastChunkSize = chunkSize;
            }

            for (int i = 0; i < chunks; ++i) {
                Chunk<T>* chunk = AllocateChunk();
                buffer->PushBack(chunk);
                if (i + 1 < chunks) {
                    chunk->Reserve(chunkSize);
                } else {
                    chunk->Reserve(lastChunkSize);
                }
            }
        }
    }

    int GetSize() const {
        return size;
    }

    void PushBack(const T& x) {
        if (buffer->GetSize() == 0 || buffer->GetLast()->GetSize() == chunkSize) {
            if (buffer->GetSize() == bufferSize) {
                bufferSize *= 2;
                buffer->Resize(bufferSize);
            }
            Chunk<T>* lastChunk = buffer->GetUnsafeAfterLast();
            if (lastChunk == nullptr) {
                lastChunk = AllocateChunk();
                buffer->PushBack(lastChunk);
            }
            lastChunk->PushBack(x);
            ++size;
            return;
        }
        Chunk<T>* lastChunk = buffer->GetLast();
        lastChunk->PushBack(x);
        ++size;
    }

    void PopBack() {
        if (size == 0) {
            throw std::out_of_range("Deque is empty");
        }
        Chunk<T>* lastChunk = buffer->GetLast();
        lastChunk->PopBack();
        if (lastChunk->GetSize() == 0) {
            buffer->PopBack();  // keeping that chunk really there
        }
        --size;
    }

    void PushFront(const T& x) {
        if (buffer->GetSize() == 0 || buffer->GetFirst()->GetSize() == chunkSize) {
            if (buffer->GetSize() == bufferSize) {
                bufferSize *= 2;
                buffer->Resize(bufferSize);
            }
            Chunk<T>* firstChunk = buffer->GetUnsafeBeforeFirst();
            if (firstChunk == nullptr) {
                firstChunk = AllocateChunk();
                buffer->PushFront(firstChunk);
            }
            firstChunk->PushFront(x);
            ++size;
            return;
        }
        Chunk<T>* firstChunk = buffer->GetFirst();
        firstChunk->PushFront(x);
        ++size;
    }

    void PopFront() {
        if (size == 0) {
            throw std::out_of_range("Deque is empty");
        }
        Chunk<T>* firstChunk = buffer->GetFirst();
        firstChunk->PopFront();
        if (firstChunk->GetSize() == 0) {
            buffer->PopFront();  // keeping that chunk really there
        }
        --size;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range: " + std::to_string(index) + " " + std::to_string(size));
        }
        int firstChunkSize = buffer->GetFirst()->GetSize();
        if (index < firstChunkSize) {
            return buffer->GetFirst()->GetRef(index);
        }
        index -= firstChunkSize;
        int chunkIndex = 1 + index / chunkSize;
        index -= (chunkIndex - 1) * chunkSize;
        return buffer->Get(chunkIndex)->GetRef(index);
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range: " + std::to_string(index) + " " + std::to_string(size));
        }
        int firstChunkSize = buffer->GetFirst()->GetSize();
        if (index < firstChunkSize) {
            return buffer->GetFirst()->Get(index);
        }
        index -= firstChunkSize;
        int chunkIndex = 1 + index / chunkSize;
        index -= (chunkIndex - 1) * chunkSize;
        return buffer->Get(chunkIndex)->Get(index);
    }

    IEnumerator<T>* GetEnumerator() override {
        return new DequeIterator<T>(buffer->GetEnumerator(), size);
    }

    IConstEnumerator<T>* GetConstEnumerator() const override {
        return new DequeConstIterator<T>(buffer->GetConstEnumerator(), size);
    }

    int CountAllocatedChunks() const {
        return allocatedChunks;
    }

    void Visualize() const {
        std::cout << "map:\n";
        buffer->VisualizeMap();
        std::cout << std::endl;

        for (int i = 0; i < buffer->GetSize(); ++i) {
            std::cout << "Chunk " << i << std::endl;
            buffer->Get(i)->VisualizeChunks();
        }
    }

    Map<T>* GetMap() {
        return buffer;
    }

private:
    Map<T>* buffer;
    int size = 0;
    int bufferSize = 4096, chunkSize = 4096;
    int allocatedChunks = 0;

    Chunk<T>* AllocateChunk() {
        ++allocatedChunks;
        return new Chunk<T>(chunkSize);
    }
};
