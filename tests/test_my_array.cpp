// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <gtest/gtest.h>
#include "my_array.h"
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <string>

TEST(MyArray, DefaultConstructor) {
    MyArray<int, 5> arr;
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], int{});
    }
}

TEST(MyArray, InitializerListConstructor) {
    MyArray<int, 3> arr{1, 2, 3};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(MyArray, CopyAssignment) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2, arr1);
}

TEST(MyArray, MoveAssignment) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2;
    arr2 = std::move(arr1);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
}

TEST(MyArray, IndexOperator) {
    MyArray<char, 2> arr{'a', 'b'};
    EXPECT_EQ(arr[1], 'b');
}

TEST(MyArray, AtThrowsOutOfBounds) {
    MyArray<int, 3> arr{1, 2, 3};
    EXPECT_THROW(arr.at(10), std::out_of_range);
}

TEST(MyArray, FrontBack) {
    MyArray<int, 3> arr{10, 20, 30};
    EXPECT_EQ(arr.front(), 10);
    EXPECT_EQ(arr.back(), 30);
}

TEST(MyArray, Iterators) {
    MyArray<int, 3> arr{1, 2, 3};
    auto it = std::find(arr.begin(), arr.end(), 2);
    EXPECT_NE(it, arr.end());
    EXPECT_EQ(*it, 2);
}

TEST(MyArray, Empty) {
    MyArray<int, 5> arr;
    EXPECT_FALSE(arr.is_empty());
}

TEST(MyArray, Swap) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2{4, 5, 6};
    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 4);
    EXPECT_EQ(arr2[0], 1);
}

TEST(MyArray, ComparisonOperators) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2{1, 2, 3};
    MyArray<int, 3> arr3{1, 2, 4};

    EXPECT_TRUE(arr1 == arr2);
    EXPECT_FALSE(arr1 == arr3);
    EXPECT_TRUE(arr1 < arr3);  // Lexicographical comparison
}

TEST(MyArray, ReverseIterators) {
    MyArray<int, 3> arr{1, 2, 3};
    auto rit = arr.rbegin();
    EXPECT_EQ(*rit, 3);
    ++rit;
    EXPECT_EQ(*rit, 2);
    ++rit;
    EXPECT_EQ(*rit, 1);
}

TEST(MyArray, CopyConstructor) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2 = arr1;
    EXPECT_EQ(arr2, arr1);
}

TEST(MyArray, MoveConstructor) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2 = std::move(arr1);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
}

TEST(MyArray, FrontBackConst) {
    const MyArray<int, 3> arr{10, 20, 30};
    EXPECT_EQ(arr.front(), 10);
    EXPECT_EQ(arr.back(), 30);
}

TEST(MyArray, SizeAndEmpty) {
    MyArray<int, 3> arr{1, 2, 3};
    EXPECT_FALSE(arr.is_empty());
}

TEST(MyArray, SwapWithEmptyArray) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2;
    arr1.swap(arr2);
    EXPECT_EQ(arr2[0], 1);
}

TEST(MyArray, NestedArray) {
    MyArray<MyArray<int, 2>, 2> arr;
    arr[0] = {1, 2};
    arr[1] = {3, 4};
    EXPECT_EQ(arr[0][0], 1);
    EXPECT_EQ(arr[1][1], 4);
}

TEST(MyArray, NestedArrayEquality) {
    MyArray<MyArray<int, 2>, 2> arr1;
    MyArray<MyArray<int, 2>, 2> arr2;
    arr1[0] = {1, 2};
    arr1[1] = {3, 4};
    arr2[0] = {1, 2};
    arr2[1] = {3, 4};
    EXPECT_EQ(arr1, arr2);
}

TEST(MyArray, NestedArrayInequality) {
    MyArray<MyArray<int, 2>, 2> arr1;
    MyArray<MyArray<int, 2>, 2> arr2;
    arr1[0] = {1, 2};
    arr1[1] = {3, 4};
    arr2[0] = {1, 2};
    arr2[1] = {5, 6};
    EXPECT_NE(arr1, arr2);
}