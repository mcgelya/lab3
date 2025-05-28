#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <stdexcept>

#include "../src/deque.h"
#include "../src/ring_buffer.h"

TEST_CASE("Test RingBuffer") {
    RingBuffer<int>* buf = new RingBuffer<int>(5);
    buf->PushBack(3);
    buf->PushFront(2);
    buf->PushFront(1);

    REQUIRE(buf->Get(0) == 1);
    REQUIRE(buf->Get(1) == 2);
    REQUIRE(buf->Get(2) == 3);

    buf->PopFront();

    REQUIRE(buf->Get(0) == 2);
    REQUIRE(buf->Get(1) == 3);

    for (IConstEnumerator<int>* it = buf->GetConstEnumerator(); !it->IsEnd(); it->MoveNext()) {
        std::cout << it->ConstDereference() << " ";
    }
    std::cout << std::endl;

    buf->PushBack(1);
    buf->PushBack(1);
    buf->PushBack(1);

    REQUIRE_THROWS_AS(buf->PushBack(1), std::out_of_range);
}

TEST_CASE("Test Deque") {
    Deque<int, 5, 2> d;

    d.PushBack(1);
    d.PushFront(2);

    for (IConstEnumerator<int>* it = d.GetConstEnumerator(); !it->IsEnd(); it->MoveNext()) {
        std::cout << it->ConstDereference() << " ";
    }
    std::cout << std::endl;

    REQUIRE((d[0] == 2 && d[1] == 1));

    d.PushBack(3);
    REQUIRE((d[0] == 2 && d[1] == 1 && d[2] == 3));

    d.PushFront(1);
    REQUIRE((d[0] == 1 && d[1] == 2 && d[2] == 1 && d[3] == 3));
}

TEST_CASE("Test Deque with start size") {
    Deque<int, 5, 2> d(3);
    REQUIRE((d[0] == 0 && d[1] == 0 && d[2] == 0));

    d.PushBack(1);
    d.PushFront(2);

    for (IConstEnumerator<int>* it = d.GetConstEnumerator(); !it->IsEnd(); it->MoveNext()) {
        std::cout << it->ConstDereference() << " ";
    }
    std::cout << std::endl;

    REQUIRE((d[0] == 2 && d[1] == 0 && d[2] == 0 && d[3] == 0 && d[4] == 1));

    d.PushBack(3);
    REQUIRE((d[0] == 2 && d[1] == 0 && d[2] == 0 && d[3] == 0 && d[4] == 1 && d[5] == 3));

    d.PushFront(1);
    REQUIRE((d[0] == 1 && d[1] == 2 && d[2] == 0 && d[3] == 0 && d[4] == 0 && d[5] == 1 && d[6] == 3));
}

TEST_CASE("Stress test deque") {
    Deque<int> d(4096);
    REQUIRE(d.CountAllocatedChunks() == 1);
    for (int i = 0; i < 100000; ++i) {
        d.PushBack(i);
        d.PopBack();
    }
    REQUIRE(d.CountAllocatedChunks() == 2);
}
