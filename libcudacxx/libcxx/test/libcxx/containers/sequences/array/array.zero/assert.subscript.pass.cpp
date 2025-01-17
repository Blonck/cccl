//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

// REQUIRES: has-unix-headers
// UNSUPPORTED: c++03
// XFAIL: use_system_cxx_lib && target={{.+}}-apple-macosx{{10.9|10.10|10.11|10.12|10.13|10.14|10.15|11.0|12.0}}
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCUDACXX_ENABLE_ASSERTIONS=1
#define _LIBCUDACXX_ENABLE_ASSERTIONS 1

// test that array<T, 0>::operator[] triggers an assertion

#include <array>

#include "check_assertion.h"

int main(int, char**) {
  {
    typedef std::array<int, 0> C;
    C c = {};
    C const& cc = c;
    TEST_LIBCUDACXX_ASSERT_FAILURE(c[0], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(c[1], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(cc[0], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(cc[1], "cannot call array<T, 0>::operator[] on a zero-sized array");
  }
  {
    typedef std::array<const int, 0> C;
    C c = {{}};
    C const& cc = c;
    TEST_LIBCUDACXX_ASSERT_FAILURE(c[0], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(c[1], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(cc[0], "cannot call array<T, 0>::operator[] on a zero-sized array");
    TEST_LIBCUDACXX_ASSERT_FAILURE(cc[1], "cannot call array<T, 0>::operator[] on a zero-sized array");
  }

  return 0;
}
