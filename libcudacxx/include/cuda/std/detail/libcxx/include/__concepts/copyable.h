//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___CONCEPTS_COPYABLE_H
#define _LIBCUDACXX___CONCEPTS_COPYABLE_H

#ifndef __cuda_std__
#include <__config>
#endif //__cuda_std__

#include "../__concepts/__concept_macros.h"
#include "../__concepts/assignable.h"
#include "../__concepts/constructible.h"
#include "../__concepts/movable.h"

#if defined(_CCCL_IMPLICIT_SYSTEM_HEADER_GCC)
#  pragma GCC system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_CLANG)
#  pragma clang system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_MSVC)
#  pragma system_header
#endif // no system header

_LIBCUDACXX_BEGIN_NAMESPACE_STD

#if _CCCL_STD_VER > 2017

// [concepts.object]

template<class _Tp>
concept copyable =
  copy_constructible<_Tp> &&
  movable<_Tp> &&
  assignable_from<_Tp&, _Tp&> &&
  assignable_from<_Tp&, const _Tp&> &&
  assignable_from<_Tp&, const _Tp>;

#elif _CCCL_STD_VER > 2011

template<class _Tp>
_LIBCUDACXX_CONCEPT_FRAGMENT(
  __copyable_,
  requires()(
    requires(copy_constructible<_Tp>),
    requires(movable<_Tp>),
    requires(assignable_from<_Tp&, _Tp&>),
    requires(assignable_from<_Tp&, const _Tp&>),
    requires(assignable_from<_Tp&, const _Tp>)
  ));

template<class _Tp>
_LIBCUDACXX_CONCEPT copyable = _LIBCUDACXX_FRAGMENT(__copyable_,_Tp);

#endif // _CCCL_STD_VER > 2011

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___CONCEPTS_COPYABLE_H
