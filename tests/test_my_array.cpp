// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <gtest/gtest.h>
#include "my_array.h"
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <string>
#include <complex>
#include <vector>


TEST(MyArray, DefaultConstructor) {
    MyArray<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], {});
    }
}

TEST(MyArray, InitializerListConstructor) {
    MyArray<int, 3> arr{1, 2, 3};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
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

TEST(MyArray, FrontBackConst) {
    const MyArray<int, 3> arr{10, 20, 30};
    EXPECT_EQ(arr.front(), 10);
    EXPECT_EQ(arr.back(), 30);
}

TEST(MyArray, SizeAndEmpty) {
    MyArray<int, 3> arr{1, 2, 3};
    EXPECT_EQ(arr.size(), 3);
    EXPECT_FALSE(arr.is_empty());
}

TEST(MyArray, Empty) {
    MyArray<int, 5> arr;
    EXPECT_FALSE(arr.is_empty());
}


TEST(MyArray, Iterators) {
    MyArray<int, 3> arr{1, 2, 3};
    auto it = std::find(arr.begin(), arr.end(), 2);
    EXPECT_NE(it, arr.end());
    EXPECT_EQ(*it, 2);
}

TEST(MyArray, ConstIterators) {
    const MyArray<int, 3> arr{1, 2, 3};
    int sum = 0;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
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

TEST(MyArray, ConstReverseIteratorTraversal) {
    const MyArray<int, 3> arr{4, 5, 6};
    std::vector<int> reversed;
    for (auto it = arr.rcbegin(); it != arr.rcend(); ++it) {
        reversed.push_back(*it);
    }
    EXPECT_EQ(reversed[0], 6);
    EXPECT_EQ(reversed[1], 5);
    EXPECT_EQ(reversed[2], 4);
}

TEST(MyArray, ConstReverseIterators) {
    const MyArray<int, 3> arr{4, 5, 6};
    auto rit = arr.rcbegin();
    EXPECT_EQ(*rit, 6);
    ++rit;
    EXPECT_EQ(*rit, 5);
}


TEST(MyArray, CopyAlgorithm) {
    MyArray<std::string, 2> arr{"hello", "world"};
    MyArray<std::string, 2> copy;
    std::copy(arr.begin(), arr.end(), copy.begin());
    EXPECT_EQ(copy, arr);
}

TEST(MyArray, TransformAlgorithm) {
    MyArray<int, 3> arr{1, 2, 3};
    MyArray<int, 3> result;

    std::transform(arr.begin(), arr.end(), result.begin(),
                   [](int x) { return x * 2; });

    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 4);
    EXPECT_EQ(result[2], 6);
}

TEST(MyArray, ReverseCopyWithSTL) {
    MyArray<int, 3> arr{7, 8, 9};
    std::vector<int> out(3);
    std::reverse_copy(arr.begin(), arr.end(), out.begin());
    EXPECT_EQ(out[0], 9);
    EXPECT_EQ(out[1], 8);
    EXPECT_EQ(out[2], 7);
}

// ==== Corner cases ====

TEST(MyArray, InitializerListShorterThanSize) {
    MyArray<int, 5> arr{1, 2};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    for (size_t i = 2; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 0);
    }
}

TEST(MyArray, InitializerListLongerThanSize) {
    MyArray<int, 3> arr{1, 2, 3, 4, 5};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(MyArray, ZeroSizedArray) {
    MyArray<int, 0> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_TRUE(arr.is_empty());
    EXPECT_EQ(arr.begin(), arr.end());
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


TEST(MyArray, StringTypeTest) {
    MyArray<std::string, 2> arr{"hello", "world"};
    EXPECT_EQ(arr[1], "world");
    arr[0] += "!";
    EXPECT_EQ(arr[0], "hello!");
}

TEST(MyArray, VectorTypeTest) {
    MyArray<std::vector<int>, 2> arr{{1, 2}, {3, 4}};
    EXPECT_EQ(arr[0][1], 2);
    EXPECT_EQ(arr[1].size(), 2);
}

TEST(MyArray, ComplexTypeTest) {
    MyArray<std::complex<double>, 2> arr{{1.0, 2.0}, {3.0, 4.0}};
    EXPECT_EQ(arr[0].real(), 1.0);
    EXPECT_EQ(arr[0].imag(), 2.0);
    EXPECT_EQ(arr[1], std::complex<double>(3.0, 4.0));
}


TEST(MyArray, ComparisonOperators) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2{1, 2, 3};
    MyArray<int, 3> arr3{1, 2, 4};

    EXPECT_TRUE(arr1 == arr2);
    EXPECT_FALSE(arr1 == arr3);
    EXPECT_TRUE(arr1 < arr3);
}

TEST(MyArray, SwapWithEmptyInitializedArray) {
    MyArray<int, 3> arr1{1, 2, 3};
    MyArray<int, 3> arr2;
    arr1.swap(arr2);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
}

MyArray<int, 2> makeArray() {
    return {9, 8};
}

TEST(MyArray, ReturnFromFunction) {
    MyArray<int, 2> arr = makeArray();
    EXPECT_EQ(arr[0], 9);
    EXPECT_EQ(arr[1], 8);
}
