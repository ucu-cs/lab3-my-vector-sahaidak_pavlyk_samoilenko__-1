// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <gtest/gtest.h>
#include "my_vector.h"
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <string>

TEST(MyVector, DefaultConstructor) {
    MyVector<int> v;
    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(MyVector, FillConstructor) {
    MyVector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(v[i], 42);
}

TEST(MyVector, IteratorConstructor) {
    std::vector<int> source = {1, 2, 3, 4};
    MyVector<int> v(source.begin(), source.end());
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[2], 3);
}

TEST(MyVector, InitializerListConstructor) {
    MyVector<std::string> v{"a", "b", "c"};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "c");
}

TEST(MyVector, CopyConstructor) {
    MyVector<int> v1{1, 2, 3};
    MyVector<int> v2 = v1;
    EXPECT_EQ(v2, v1);
}

TEST(MyVector, MoveConstructor) {
    MyVector<int> temp{5, 6};
    MyVector<int> v = std::move(temp);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);
}

TEST(MyVector, CopyAssignment) {
    MyVector<int> a{1, 2, 3};
    MyVector<int> b;
    b = a;
    EXPECT_EQ(a, b);
}

TEST(MyVector, MoveAssignment) {
    MyVector<int> a{1, 2};
    MyVector<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
}

TEST(MyVector, IndexOperator) {
    MyVector<char> v{'a', 'b'};
    EXPECT_EQ(v[1], 'b');
}

TEST(MyVector, AtThrowsOutOfBounds) {
    MyVector<int> v{1, 2, 3};
    EXPECT_THROW(v.at(10), std::out_of_range);
}

TEST(MyVector, FrontBack) {
    MyVector<int> v{10, 20, 30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
}

TEST(MyVector, Iterators) {
    MyVector<int> v{1, 2, 3};
    auto it = std::find(v.begin(), v.end(), 2);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 2);
}

// ?
TEST(MyVector, BackInserterCompatibility) {
    MyVector<int> v;
    std::vector<int> source = {1, 2, 3};
    std::copy(source.begin(), source.end(), std::back_inserter(v));
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back(), 3);
}

TEST(MyVector, CapacityManagement) {
    MyVector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    v.shrink_to_fit();
    EXPECT_LE(v.capacity(), 10);
}

TEST(MyVector, ResizeAndClear) {
    MyVector<int> v{1, 2, 3};
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    v.clear();
    EXPECT_TRUE(v.is_empty());
}

TEST(MyVector, ResizeShrink) {
    MyVector<int> v{1, 2, 3, 4, 5};
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

TEST(MyVector, ResizeGrowDefault) {
    MyVector<int> v{1, 2};
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[2], {});  // Default initialized values
}

TEST(MyVector, InsertSingle) {
    MyVector<int> v{1, 3};
    auto it = v.insert(v.begin() + 1, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(v[1], 2);
}

TEST(MyVector, InsertAtBeginEmpty) {
    MyVector<int> v;
    v.insert(v.begin(), 42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(MyVector, InsertAtEnd) {
    MyVector<int> v{1, 2, 3};
    v.insert(v.end(), 4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[3], 4);
}

TEST(MyVector, InsertRange) {
    MyVector<int> v{1, 5};
    std::vector<int> to_insert{2, 3, 4};
    v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());
    EXPECT_EQ(v[2], 3);
}

TEST(MyVector, EraseSingle) {
    MyVector<int> v{1, 2, 3};
    auto it = v.erase(v.begin() + 1);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v.size(), 2);
}

TEST(MyVector, EraseRange) {
    MyVector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 4);
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(v.size(), 2);
}

TEST(MyVector, EraseFromEmpty) {
    MyVector<int> v;
    EXPECT_NO_THROW(v.erase(v.begin()));  // No-op for an empty vector.
}

TEST(MyVector, PushPopEmplaceBack) {
    MyVector<std::string> v;
    v.push_back("hi");
    v.emplace_back("there");
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), "there");
    v.pop_back();
    EXPECT_EQ(v.size(), 1);
}

TEST(MyVector, ComparisonOperators) {
    MyVector<int> a{1, 2, 3};
    MyVector<int> b{1, 2, 4};
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(MyVector, SwapMethod) {
    MyVector<int> a{1, 2}, b{3, 4};
    a.swap(b);
    EXPECT_EQ(a[0], 3);
    EXPECT_EQ(b[0], 1);
}

TEST(MyVector, SwapWithEmptyVector) {
    MyVector<int> v1{1, 2, 3};
    MyVector<int> v2;
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
}

TEST(MyVector, NestedVectorsBasic) {
    MyVector<MyVector<int>> vv;
    MyVector<int> inner1 = {1, 2, 3};
    MyVector<int> inner2 = {4, 5};

    vv.push_back(inner1);
    vv.push_back(inner2);

    ASSERT_EQ(vv.size(), 2);
    EXPECT_EQ(vv[0].size(), 3);
    EXPECT_EQ(vv[1][1], 5);
}

TEST(MyVector, NestedVectorsEmplaceBack) {
    MyVector<MyVector<int>> vv;
    vv.emplace_back(MyVector<int>{1, 2});
    vv.emplace_back(MyVector<int>{3, 4});

    EXPECT_EQ(vv.size(), 2);
    EXPECT_EQ(vv[0].front(), 1);
    EXPECT_EQ(vv[1].back(), 4);
}

TEST(MyVector, PushBackSelfBack) {
    MyVector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    int last = v.back();
    v.push_back(last);

    EXPECT_EQ(v.size(), 11);
    EXPECT_EQ(v.back(), 9);
}

TEST(MyVector, NestedPushBackSelfBack) {
    MyVector<MyVector<int>> v;
    v.push_back({1, 2});
    v.push_back(v.back()); // Copy of the last vector

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], v[0]); // Must be deep copy
}

