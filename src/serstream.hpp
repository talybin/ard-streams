// Copyright (C) 2019 Vladimir Talybin.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <ios.hpp>

namespace ard
{
    // Implements an unbuffered basic_streambuf
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_serialbuf : basic_streambuf<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using serial_type = ::Stream;

        using int_type = typename traits_type::int_type;

    protected:
        ios_base::openmode mode_;
        serial_type& serial_;

    public:
        explicit basic_serialbuf(serial_type& ser,
            ios_base::openmode mode = ios_base::in | ios_base::out)
        : mode_(mode)
        , serial_(ser)
        { }

        virtual ~basic_serialbuf()
        { }

        // Get a reference to the wrapped object
        serial_type& serial()
        { return serial_; }

    protected:
        // Get how many bytes available
        virtual std::streamsize showmanyc()
        { return serial_.available(); }

        // Write a single char
        virtual int_type overflow(int_type c = traits_type::eof());

        // Peek a char
        virtual int_type underflow()
        { return serial_.peek(); }

        // Read a char
        virtual int_type uflow()
        { return serial_.read(); }

        // Multiple character insertion
        virtual std::streamsize xsputn(const char_type* s, std::streamsize n)
        { return serial_.write((const uint8_t*)s, n); }
    };

    // Input stream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_iserialstream : basic_istream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using serial_type = ::Stream;

        explicit basic_iserialstream(serial_type& ser)
        : basic_istream<char_type, traits_type>(&sb_)
        , sb_(ser, ios_base::in)
        { }

        virtual ~basic_iserialstream()
        { }

    protected:
        basic_serialbuf<char_type, traits_type> sb_;
    };

    // Output stream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_oserialstream : basic_ostream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using serial_type = ::Stream;

        explicit basic_oserialstream(serial_type& ser)
        : basic_ostream<char_type, traits_type>(&sb_)
        , sb_(ser, ios_base::out)
        { }

        virtual ~basic_oserialstream()
        { }

    protected:
        basic_serialbuf<char_type, traits_type> sb_;
    };

    // Input/output stream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_serialstream : basic_iostream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using serial_type = ::Stream;

        explicit basic_serialstream(serial_type& ser)
        : basic_iostream<char_type, traits_type>(&sb_)
        , sb_(ser, ios_base::in | ios_base::out)
        { }

        virtual ~basic_serialstream()
        { }

    protected:
        basic_serialbuf<char_type, traits_type> sb_;
    };

    //
    // Methods
    //

    template <class CharT, class Traits>
    inline typename basic_serialbuf<CharT, Traits>::int_type
    basic_serialbuf<CharT, Traits>::
    overflow(int_type c)
    {
        const bool testout = this->mode_ & ios_base::out;
        if (!testout)
            return traits_type::eof();

        const bool testeof = traits_type::eq_int_type(c, traits_type::eof());
        if (testeof)
            return traits_type::not_eof(c);

        serial_.write(c);
        return c;
    }

    //
    // Alias
    //

    using iserialstream = basic_iserialstream<char>;
    using oserialstream = basic_oserialstream<char>;
    using serialstream = basic_serialstream<char>;

} // namespace ard

