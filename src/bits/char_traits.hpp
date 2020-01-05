// Copyright (C) 1997-2017 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library. This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.
//
// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively. If not, see
// <http://www.gnu.org/licenses/>.

#pragma once

namespace ard
{
    /**
     *  @brief  Mapping from character type to associated types.
     *
     *  @note This is an implementation class for the generic version
     *  of char_traits. It defines int_type, off_type, pos_type, and
     *  state_type. By default these are unsigned long, streamoff,
     *  streampos, and mbstate_t. Users who need a different set of
     *  types, but who don't need to change the definitions of any function
     *  defined in char_traits, can specialize gnu_cxx::_Char_types
     *  while leaving gnu_cxx::char_traits alone.
     */
    template <class CharT>
    struct char_types
    {
        using int_type = unsigned long;
        using pos_type = std::streampos;
        using off_type = std::streamoff;
    };


    /**
     *  @brief  Base class used to implement std::char_traits.
     *
     *  @note For any given actual character type, this definition is
     *  probably wrong.  (Most of the member functions are likely to be
     *  right, but the int_type and state_type typedefs, and the eof()
     *  member function, are likely to be wrong.) The reason this class
     *  exists is so users can specialize it.  Classes in namespace std
     *  may not be specialized for fundamental types, but classes in
     *  namespace gnu_cxx may be.
     *
     *  See https://gcc.gnu.org/onlinedocs/libstdc++/manual/strings.html#strings.string.character_types
     *  for advice on how to make use of this class for @a unusual character
     *  types. Also, check out include/ext/pod_char_traits.h.  
     */
    template <class CharT>
    struct char_traits
    {
        using char_type = CharT;
        using int_type  = typename char_types<CharT>::int_type;
        using pos_type  = typename char_types<CharT>::pos_type;
        using off_type  = typename char_types<CharT>::off_type;

        static constexpr void
        assign(char_type& c1, const char_type& c2)
        { c1 = c2; }

        static constexpr bool
        eq(const char_type& c1, const char_type& c2)
        { return c1 == c2; }

        static constexpr bool
        lt(const char_type& c1, const char_type& c2)
        { return c1 < c2; }

        static constexpr int
        compare(const char_type* s1, const char_type* s2, std::size_t n);

        static constexpr std::size_t
        length(const char_type* s);

        static constexpr const char_type*
        find(const char_type* s, std::size_t n, const char_type& a);

        static char_type*
        move(char_type* s1, const char_type* s2, std::size_t n);

        static char_type*
        copy(char_type* s1, const char_type* s2, std::size_t n);

        static char_type*
        assign(char_type* s, std::size_t n, char_type a);

        static constexpr char_type
        to_char_type(const int_type& c)
        { return static_cast<char_type>(c); }

        static constexpr int_type
        to_int_type(const char_type& c)
        { return static_cast<int_type>(c); }

        static constexpr bool
        eq_int_type(const int_type& c1, const int_type& c2)
        { return c1 == c2; }

        static constexpr int_type
        eof()
        { return static_cast<int_type>(-1); }

        static constexpr int_type
        not_eof(const int_type& c)
        { return !eq_int_type(c, eof()) ? c : to_int_type(char_type()); }
    };

    template <class CharT>
    constexpr int
    char_traits<CharT>::
    compare(const char_type* s1, const char_type* s2, std::size_t n)
    {
        for (std::size_t i = 0; i < n; ++i)
            if (lt(s1[i], s2[i]))
                return -1;
            else if (lt(s2[i], s1[i]))
                return 1;
        return 0;
    }

    template <class CharT>
    constexpr std::size_t
    char_traits<CharT>::
    length(const char_type* p)
    {
        std::size_t i = 0;
        while (!eq(p[i], char_type()))
            ++i;
        return i;
    }

    template <class CharT>
    constexpr const typename char_traits<CharT>::char_type*
    char_traits<CharT>::
    find(const char_type* s, std::size_t n, const char_type& a)
    {
        for (std::size_t i = 0; i < n; ++i)
            if (eq(s[i], a))
                return s + i;
        return 0;
    }

    template <class CharT>
    typename char_traits<CharT>::char_type*
    char_traits<CharT>::
    move(char_type* s1, const char_type* s2, std::size_t n)
    {
        return static_cast<CharT*>(
            __builtin_memmove(s1, s2, n * sizeof(char_type)));
    }

    template <class CharT>
    typename char_traits<CharT>::char_type*
    char_traits<CharT>::
    copy(char_type* s1, const char_type* s2, std::size_t n)
    {
        // NB: Inline std::copy so no recursive dependencies.
        std::copy(s2, s2 + n, s1);
        return s1;
    }

    template <class CharT>
    typename char_traits<CharT>::char_type*
    char_traits<CharT>::
    assign(char_type* s, std::size_t n, char_type a)
    {
        // NB: Inline std::fill_n so no recursive dependencies.
        std::fill_n(s, n, a);
        return s;
    }


    /// 21.1.3.1  char_traits specializations
    template <>
    struct char_traits<char>
    {
        using char_type = char;
        using int_type  = int;
        using pos_type  = std::streampos;
        using off_type  = std::streamoff;
    
        static void
        assign(char_type& c1, const char_type& c2) noexcept
        { c1 = c2; }
    
        static constexpr bool
        eq(const char_type& c1, const char_type& c2) noexcept
        { return c1 == c2; }

        static constexpr bool
        lt(const char_type& c1, const char_type& c2) noexcept
        {
            // LWG 467.
            return (static_cast<unsigned char>(c1)
                < static_cast<unsigned char>(c2));
        }
    
        static int
        compare(const char_type* s1, const char_type* s2, size_t n)
        {
            if (n == 0)
                return 0;
            return __builtin_memcmp(s1, s2, n);
        }

        static constexpr size_t
        length(const char_type* s)
        {
            return __builtin_strlen(s);
        }

        static const char_type*
        find(const char_type* s, size_t n, const char_type& a)
        {
            if (n == 0)
                return 0;
            return static_cast<const char_type*>(__builtin_memchr(s, a, n));
        }

        static char_type*
        move(char_type* s1, const char_type* s2, size_t n)
        {
            if (n == 0)
                return s1;
            return static_cast<char_type*>(__builtin_memmove(s1, s2, n));
        }

        static char_type*
        copy(char_type* s1, const char_type* s2, size_t n)
        {
            if (n == 0)
                return s1;
            return static_cast<char_type*>(__builtin_memcpy(s1, s2, n));
        }

        static char_type*
        assign(char_type* s, size_t n, char_type a)
        {
            if (n == 0)
                return s;
            return static_cast<char_type*>(__builtin_memset(s, a, n));
        }

        static constexpr char_type
        to_char_type(const int_type& c) noexcept
        { return static_cast<char_type>(c); }

        // To keep both the byte 0xff and the eof symbol 0xffffffff
        // from ending up as 0xffffffff.
        static constexpr int_type
        to_int_type(const char_type& c) noexcept
        { return static_cast<int_type>(static_cast<unsigned char>(c)); }

        static constexpr bool
        eq_int_type(const int_type& c1, const int_type& c2) noexcept
        { return c1 == c2; }

        static constexpr int_type
        eof() noexcept
        { return static_cast<int_type>(-1); }

        static constexpr int_type
        not_eof(const int_type& c) noexcept
        { return (c == eof()) ? 0 : c; }
    };

    /// 21.1.3.2 wchar_t char_traits specializations.
    /// Not implemented in current version.

} // namespace ard

