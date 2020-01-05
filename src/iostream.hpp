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
#include <istream.hpp>
#include <ostream.hpp>

namespace ard
{
    // Template class basic_iostream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_iostream
    : basic_istream<CharT, Traits>
    , basic_ostream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using istream_type = basic_istream<char_type, traits_type>;
        using ostream_type = basic_ostream<char_type, traits_type>;
        using streambuf_type = basic_streambuf<char_type, traits_type>;

        // Constructor does nothing
        explicit basic_iostream(streambuf_type* sb)
        : istream_type(sb)
        , ostream_type(sb)
        { }

        // Destructor does nothing
        virtual ~basic_iostream()
        { }

    protected:
        basic_iostream()
        : istream_type()
        , ostream_type()
        { }

        basic_iostream(const basic_iostream&) = delete;

        basic_iostream(basic_iostream&& rhs)
        : istream_type(std::move(rhs))
        , ostream_type(std::move(rhs))
        { }

        // Assign/swap

        basic_iostream& operator=(const basic_iostream&) = delete;

        basic_iostream& operator=(basic_iostream&& rhs)
        {
            this->swap(rhs);
            return *this;
        }

        void swap(basic_iostream& rhs)
        { istream_type::swap(rhs); }
    };

    //
    // Alias
    //

    using iostream = basic_iostream<char>;

} // namespace ard

