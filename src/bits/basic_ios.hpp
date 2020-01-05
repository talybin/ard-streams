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
#include <bits/streambuf_iterator.hpp>
#include <bits/locale_facets.hpp>

namespace ard
{
    // Template class basic_ios, virtual base class for all
    // stream classes.
    //
    // Param CharT  - Type of character stream.
    // Param Traits - Traits for character type.
    //
    // Most of the member functions called dispatched on stream objects
    // (e.g., std::cout.foo(bar);) are consolidated in this class.
    //
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_ios : ios_base
    {
        // These are standard types. They permit a standardized way of
        // referring to names of (or names dependent on) the template
        // parameters, which are specific to the implementation.
        using char_type = CharT;
        using traits_type = Traits;
        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        // These are non-standard types.
        using ctype_type = ctype<char_type>;
        using num_put_type =
            num_put<char_type, ostreambuf_iterator<char_type, traits_type>>;
        using num_get_type =
            num_get<char_type, istreambuf_iterator<char_type, traits_type>>;

        using ostream_type = basic_ostream<char_type, traits_type>;
        using streambuf_type = basic_streambuf<char_type, traits_type>;

    protected:
        // Data members
        ostream_type* tie_ = nullptr;
        char_type fill_ = { };
        streambuf_type* streambuf_ = nullptr;

        // For ostream
        num_put_type num_put_;
        // For istream
        num_get_type num_get_;

    public:
        // The quick-and-easy status check.
        //
        // This allows you to write constructs such as
        // if (!a_stream) ...
        // and
        // while (a_stream) ...
        //
        explicit operator bool() const
        { return !this->fail(); }

        bool operator!() const
        { return this->fail(); }

        // Returns the error state of the stream buffer.
        // Return a bit pattern (well, isn't everything?)
        //
        // See std::ios_base::iostate for the possible bit values. Most
        // users will call one of the interpreting wrappers, e.g., good().
        //
        iostate rdstate() const
        { return streambuf_state_; }

        // [Re]sets the error state.
        // Param state - The additional state flag(s) to set.
        // See std::ios_base::iostate for the possible bit values.
        void clear(iostate state = goodbit)
        { streambuf_state_ = this->rdbuf() ? state : (state | badbit); }

        // Sets additional flags in the error state.
        // Param state - The additional state flag(s) to set.
        //
        // See std::ios_base::iostate for the possible bit values.
        //
        void setstate(iostate state)
        { this->clear(this->rdstate() | state); }

        // Fast error checking.
        // Return true if no error flags are set.
        //
        // A wrapper around rdstate.
        //
        bool good() const
        { return this->rdstate() == goodbit; }

        // Fast error checking.
        // Return true if the eofbit is set.
        //
        // Note that other iostate flags may also be set.
        //
        bool eof() const
        { return this->rdstate() & eofbit; }

        // Fast error checking.
        // Return true if either the badbit or the failbit is set.
        //
        // Checking the badbit in fail() is historical practice.
        // Note that other iostate flags may also be set.
        //
        bool fail() const
        { return this->rdstate() & (badbit | failbit); }

        // Fast error checking.
        // Return true if the badbit is set.
        //
        // Note that other iostate flags may also be set.
        //
        bool bad() const
        { return this->rdstate() & badbit; }

        // Constructor performs initialization.
        //
        // The parameter is passed by derived streams.
        //
        explicit basic_ios(streambuf_type* sb)
        : ios_base()
        { this->init(sb); }

        // Empty.
        //
        // The destructor does nothing. More specifically, it does not
        // destroy the streambuf held by rdbuf().
        //
        virtual ~basic_ios()
        { }

        // Fetches the current tied stream.
        // Return a pointer to the tied stream, or nullptr if the stream
        // is not tied.
        //
        // A stream may be tied (or synchronized) to a second output
        // stream. When this stream performs any I/O, the tied stream is
        // first flushed. For example, std::cin is tied to std::cout.
        //
        ostream_type* tie() const
        { return tie_; }

        // Ties this stream to an output stream.
        // Param tiestr - The output stream.
        // Return the previously tied output stream, or nullptr if the
        // stream was not tied.
        //
        // This sets up a new tie; see tie() for more.
        //
        ostream_type* tie(ostream_type* tiestr)
        {
            ostream_type* old = tie_;
            tie_ = tiestr;
            return old;
        }

        // Accessing the underlying buffer.
        // Return the current stream buffer.
        //
        // This does not change the state of the stream.
        //
        streambuf_type* rdbuf() const
        { return streambuf_; }

        // Changing the underlying buffer.
        // Param sb - The new stream buffer.
        // Return the previous stream buffer.
        //
        // Associates a new buffer with the current stream, and clears the
        // error state.
        //
        // Due to historical accidents which the LWG refuses to correct, the
        // I/O library suffers from a design error: this function is hidden
        // in derived classes by overrides of the zero-argument rdbuf(),
        // which is non-virtual for hysterical raisins. As a result, you
        // must use explicit qualifications to access this function via any
        // derived class. For example:
        //
        // std::fstream foo;         // or some other derived type
        // std::streambuf* p = .....;
        //
        // foo.ios::rdbuf(p);        // ios == basic_ios<char>
        //
        streambuf_type* rdbuf(streambuf_type* sb)
        {
            streambuf_type* old = streambuf_;
            streambuf_ = sb;
            this->clear();
            return old;
        }

        // Copies fields of rhs into this.
        // Param rhs - The source values for the copies.
        // Return reference to this object.
        //
        // All fields of rhs are copied into this object except that rdbuf()
        // and rdstate() remain unchanged. All values in the pword and iword
        // arrays are copied. Before copying, each callback is invoked with
        // erase_event. After copying, each (new) callback is invoked with
        // copyfmt_event.
        //
        basic_ios& copyfmt(const basic_ios& rhs)
        {
            if (this != &rhs) {
                this->flags(rhs.flags());
                this->width(rhs.width());
                this->precision(rhs.precision());
                this->tie(rhs.tie());
                this->fill(rhs.fill());
            }
            return *this;
        }

        // Retrieves the empty character.
        // Return the current fill character.
        //
        // It defaults to a space (' ') in the current locale.
        //
        char_type fill() const
        { return fill_; }

        // Sets a new empty character.
        // Param ch - The new character.
        // Return the previous fill character.
        //
        // The fill character is used to fill out space when P+ characters
        // have been requested (e.g., via setw), Q characters are actually
        // used, and Q < P. It defaults to a space (' ') in the current locale.
        //
        char_type fill(char_type ch)
        {
            char_type old = fill_;
            fill_ = ch;
            return old;
        }

        // Widens characters.
        // Param c - The character to widen.
        // Return the widened character.
        //
        // Maps a character of char to a character of char_type.
        // Returns the result of
        //
        // ard::ctype<char_type>::widen(c)
        //
        // Additional l10n notes are at
        // http://gcc.gnu.org/onlinedocs/libstdc++/manual/localization.html
        //
        char_type widen(char c) const
        { return ctype_type::widen(c); }

    protected:
        // 27.4.5.1 basic_ios constructors

        // Empty.
        //
        // The default constructor does nothing and is not normally
        // accessible to users.
        //
        basic_ios() = default;

        basic_ios(const basic_ios&) = delete;
        basic_ios& operator=(const basic_ios&) = delete;

        // All setup is performed here.
        //
        // This is called from the public constructor. It is not virtual and
        // cannot be redefined.
        //
        void init(streambuf_type* sb)
        {
            fill_ = this->widen(' ');
            tie_ = nullptr;
            streambuf_ = sb;
            streambuf_state_ = sb ? goodbit : badbit;
        }

        void move(basic_ios& rhs)
        {
            ios_base::move_(rhs);
            this->tie(rhs.tie(nullptr));
            fill_ = rhs.fill_;
            streambuf_ = nullptr;
        }

        void move(basic_ios&& rhs)
        { this->move(rhs); }

        void swap(basic_ios& rhs) noexcept
        {
            ios_base::swap_(rhs);
            std::swap(tie_, rhs.tie_);
            std::swap(fill_, rhs.fill_);
        }
    };

} // namespace ard

