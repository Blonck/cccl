//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// NetBSD does not support LC_MONETARY at the moment
// XFAIL: netbsd

// REQUIRES: locale.zh_CN.UTF-8

// <locale>

// class money_put<charT, OutputIterator>

// iter_type put(iter_type s, bool intl, ios_base& f, char_type fill,
//               long double units) const;

// TODO For zh_CN GLIBC puts the negative sign after the currency symbol.
// XFAIL: linux-gnu

#include <locale>
#include <ios>
#include <streambuf>
#include <cassert>
#include "test_macros.h"
#include "test_iterators.h"

#include "platform_support.h" // locale name macros

typedef std::money_put<char, cpp17_output_iterator<char*> > Fn;

class my_facet
    : public Fn
{
public:
    explicit my_facet(std::size_t refs = 0)
        : Fn(refs) {}
};

typedef std::money_put<wchar_t, cpp17_output_iterator<wchar_t*> > Fw;

class my_facetw
    : public Fw
{
public:
    explicit my_facetw(std::size_t refs = 0)
        : Fw(refs) {}
};

int main(int, char**)
{
    std::ios ios(0);
    std::string loc_name(LOCALE_zh_CN_UTF_8);
    ios.imbue(std::locale(ios.getloc(),
                          new std::moneypunct_byname<char, false>(loc_name)));
    ios.imbue(std::locale(ios.getloc(),
                          new std::moneypunct_byname<char, true>(loc_name)));
    ios.imbue(std::locale(ios.getloc(),
                          new std::moneypunct_byname<wchar_t, false>(loc_name)));
    ios.imbue(std::locale(ios.getloc(),
                          new std::moneypunct_byname<wchar_t, true>(loc_name)));
{
    const my_facet f(1);
    // char, national
    {   // zero
        long double v = 0;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "0.00");
    }
    {   // negative one
        long double v = -1;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "-0.01");
    }
    {   // positive
        long double v = 123456789;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "1,234,567.89");
    }
    {   // negative
        long double v = -123456789;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "-1,234,567.89");
    }
    {   // zero, showbase
        long double v = 0;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""0.00");
    }
    {   // negative one, showbase
        long double v = -1;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""-0.01");
    }
    {   // positive, showbase
        long double v = 123456789;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""1,234,567.89");
    }
    {   // negative, showbase
        long double v = -123456789;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""-1,234,567.89");
    }
    {   // negative, showbase, left
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        left(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""-1,234,567.89    ");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, internal
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        internal(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "\xEF\xBF\xA5""-    1,234,567.89");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, right
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        right(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            false, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "    \xEF\xBF\xA5""-1,234,567.89");
        assert(ios.width() == 0);
    }

    // char, international
    noshowbase(ios);
    ios.unsetf(std::ios_base::adjustfield);
    {   // zero
        long double v = 0;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "0.00");
    }
    {   // negative one
        long double v = -1;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "-0.01");
    }
    {   // positive
        long double v = 123456789;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "1,234,567.89");
    }
    {   // negative
        long double v = -123456789;
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "-1,234,567.89");
    }
    {   // zero, showbase
        long double v = 0;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY 0.00");
    }
    {   // negative one, showbase
        long double v = -1;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY -0.01");
    }
    {   // positive, showbase
        long double v = 123456789;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY 1,234,567.89");
    }
    {   // negative, showbase
        long double v = -123456789;
        showbase(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, '*', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY -1,234,567.89");
    }
    {   // negative, showbase, left
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        left(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY -1,234,567.89   ");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, internal
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        internal(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "CNY -   1,234,567.89");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, right
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        right(ios);
        char str[100];
        cpp17_output_iterator<char*> iter = f.put(cpp17_output_iterator<char*>(str),
                                            true, ios, ' ', v);
        std::string ex(str, base(iter));
        assert(ex == "   CNY -1,234,567.89");
        assert(ios.width() == 0);
    }
}
{
    const my_facetw f(1);
    // wchar_t, national
    noshowbase(ios);
    ios.unsetf(std::ios_base::adjustfield);
    {   // zero
        long double v = 0;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"0.00");
    }
    {   // negative one
        long double v = -1;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"-0.01");
    }
    {   // positive
        long double v = 123456789;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"1,234,567.89");
    }
    {   // negative
        long double v = -123456789;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"-1,234,567.89");
    }
    {   // zero, showbase
        long double v = 0;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""0.00");
    }
    {   // negative one, showbase
        long double v = -1;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""-0.01");
    }
    {   // positive, showbase
        long double v = 123456789;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""1,234,567.89");
    }
    {   // negative, showbase
        long double v = -123456789;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""-1,234,567.89");
    }
    {   // negative, showbase, left
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        left(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""-1,234,567.89      ");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, internal
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        internal(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"\xFFE5""-      1,234,567.89");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, right
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        right(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            false, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"      \xFFE5""-1,234,567.89");
        assert(ios.width() == 0);
    }

    // wchar_t, international
    noshowbase(ios);
    ios.unsetf(std::ios_base::adjustfield);
    {   // zero
        long double v = 0;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"0.00");
    }
    {   // negative one
        long double v = -1;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"-0.01");
    }
    {   // positive
        long double v = 123456789;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"1,234,567.89");
    }
    {   // negative
        long double v = -123456789;
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"-1,234,567.89");
    }
    {   // zero, showbase
        long double v = 0;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY 0.00");
    }
    {   // negative one, showbase
        long double v = -1;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY -0.01");
    }
    {   // positive, showbase
        long double v = 123456789;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY 1,234,567.89");
    }
    {   // negative, showbase
        long double v = -123456789;
        showbase(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, '*', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY -1,234,567.89");
    }
    {   // negative, showbase, left
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        left(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY -1,234,567.89   ");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, internal
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        internal(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"CNY -   1,234,567.89");
        assert(ios.width() == 0);
    }
    {   // negative, showbase, right
        long double v = -123456789;
        showbase(ios);
        ios.width(20);
        right(ios);
        wchar_t str[100];
        cpp17_output_iterator<wchar_t*> iter = f.put(cpp17_output_iterator<wchar_t*>(str),
                                            true, ios, ' ', v);
        std::wstring ex(str, base(iter));
        assert(ex == L"   CNY -1,234,567.89");
        assert(ios.width() == 0);
    }
}

  return 0;
}
