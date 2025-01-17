//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// REQUIRES: long_tests

// <algorithm>

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          Predicate<auto, InIter2::value_type, InIter1::value_type> Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && CopyConstructible<Compare>
//   constexpr OutIter       // constexpr after C++17
//   merge(InIter1 first1, InIter1 last1,
//         InIter2 first2, InIter2 last2, OutIter result, Compare comp);

#include <algorithm>
#include <functional>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "counting_predicates.h"

// #if TEST_STD_VER > 2017
// TEST_CONSTEXPR bool test_constexpr() {
//           int ia[]       = {0, 1, 2, 3, 4};
//           int ib[]       = {2, 4, 6, 8};
//           int ic[]       = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//     const int expected[] = {0, 1, 2, 2, 3, 4, 4, 6, 8};
//
//     auto it = std::merge(std::begin(ia), std::end(ia),
//                          std::begin(ib), std::end(ib),
//                          std::begin(ic), [](int a, int b) {return a == b; });
//     return std::distance(std::begin(ic), it) == (std::size(ia) + std::size(ib))
//         && *it == 0
//         && std::equal(std::begin(ic), it, std::begin(expected), std::end(expected))
//         ;
//     }
// #endif

std::mt19937 randomness;

template <class InIter1, class InIter2, class OutIter>
void
test()
{
    {
    unsigned N = 100000;
    int* ia = new int[N];
    int* ib = new int[N];
    int* ic = new int[2*N];
    for (unsigned i = 0; i < N; ++i)
        ia[i] = 2*i;
    for (unsigned i = 0; i < N; ++i)
        ib[i] = 2*i+1;
    std::reverse(ia, ia+N);
    std::reverse(ib, ib+N);
    binary_counting_predicate<std::greater<int>, int, int> pred((std::greater<int>()));
    OutIter r = std::merge(InIter1(ia), InIter1(ia+N),
                           InIter2(ib), InIter2(ib+N), OutIter(ic), pred);
    assert(base(r) == ic+2*N);
    assert(ic[0] == static_cast<int>(2*N-1));
    assert(ic[2*N-1] == 0);
    assert(std::is_sorted(ic, ic+2*N, std::greater<int>()));
    assert(pred.count() <= (N + N - 1));
    delete [] ic;
    delete [] ib;
    delete [] ia;
    }
    {
    unsigned N = 100;
    int* ia = new int[N];
    int* ib = new int[N];
    int* ic = new int[2*N];
    for (unsigned i = 0; i < 2*N; ++i)
        ic[i] = i;
    std::shuffle(ic, ic+2*N, randomness);
    std::copy(ic, ic+N, ia);
    std::copy(ic+N, ic+2*N, ib);
    std::sort(ia, ia+N, std::greater<int>());
    std::sort(ib, ib+N, std::greater<int>());
    binary_counting_predicate<std::greater<int>, int, int> pred((std::greater<int>()));
    OutIter r = std::merge(InIter1(ia), InIter1(ia+N),
                           InIter2(ib), InIter2(ib+N), OutIter(ic), pred);
    assert(base(r) == ic+2*N);
    assert(ic[0] == static_cast<int>(2*N-1));
    assert(ic[2*N-1] == 0);
    assert(std::is_sorted(ic, ic+2*N, std::greater<int>()));
    assert(pred.count() <= (N + N - 1));
    delete [] ic;
    delete [] ib;
    delete [] ia;
    }
}

int main(int, char**)
{
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<const int*>, int*>();

    test<cpp17_input_iterator<const int*>, forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<const int*>, int*>();

    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<const int*>, int*>();

    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<const int*>, int*>();

    test<cpp17_input_iterator<const int*>, const int*, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, const int*, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, const int*, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, const int*, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, const int*, int*>();

    test<forward_iterator<const int*>, cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, cpp17_input_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, const int*, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, const int*, forward_iterator<int*> >();
    test<forward_iterator<const int*>, const int*, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, const int*, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, const int*, int*>();

    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, cpp17_input_iterator<const int*>, int*>();

    test<bidirectional_iterator<const int*>, forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<const int*>, int*>();

    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<const int*>, int*>();

    test<bidirectional_iterator<const int*>, random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<const int*>, int*>();

    test<bidirectional_iterator<const int*>, const int*, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, const int*, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, const int*, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, const int*, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, const int*, int*>();

    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, cpp17_input_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*>, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*>, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, const int*, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, const int*, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, const int*, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, const int*, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, const int*, int*>();

    test<const int*, cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<const int*, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<const int*, cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<const int*, cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<const int*, cpp17_input_iterator<const int*>, int*>();

    test<const int*, forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<const int*, forward_iterator<const int*>, forward_iterator<int*> >();
    test<const int*, forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<const int*, forward_iterator<const int*>, random_access_iterator<int*> >();
    test<const int*, forward_iterator<const int*>, int*>();

    test<const int*, bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<const int*, bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<const int*, bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<const int*, bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<const int*, bidirectional_iterator<const int*>, int*>();

    test<const int*, random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<const int*, random_access_iterator<const int*>, forward_iterator<int*> >();
    test<const int*, random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<const int*, random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<const int*, random_access_iterator<const int*>, int*>();

    test<const int*, const int*, cpp17_output_iterator<int*> >();
    test<const int*, const int*, forward_iterator<int*> >();
    test<const int*, const int*, bidirectional_iterator<int*> >();
    test<const int*, const int*, random_access_iterator<int*> >();
    test<const int*, const int*, int*>();

#if TEST_STD_VER > 2017
//  Not yet - waiting on std::copy
//     static_assert(test_constexpr());
#endif

  return 0;
}
