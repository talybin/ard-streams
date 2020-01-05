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
#include <iterator>
#include <streambuf.hpp>

namespace ard
{
    // Forward declarations
    template <class CharT, class Traits>
    struct basic_istream;

    template <class CharT, class Traits>
    struct basic_ostream;


    template <class CharT, class Traits>
    struct istreambuf_iterator
    : std::iterator<std::input_iterator_tag, CharT,
                    typename Traits::off_type, CharT*, CharT>
    {
        using char_type = CharT;
        using traits_type = Traits;

        using int_type = typename traits_type::int_type;
        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using istream_type = basic_istream<char_type, traits_type>;

    private:
        mutable streambuf_type* sbuf_;
        mutable int_type c_;

    public:
        constexpr istreambuf_iterator() noexcept
        : sbuf_(nullptr)
        , c_(traits_type::eof())
        { }

        istreambuf_iterator(const istreambuf_iterator&) noexcept = default;

        ~istreambuf_iterator() = default;

        // Construct start of input stream iterator
        istreambuf_iterator(istream_type& s) noexcept
        : sbuf_(s.rdbuf())
        , c_(traits_type::eof())
        { }

        // Construct start of streambuf iterator
        istreambuf_iterator(streambuf_type* s) noexcept
        : sbuf_(s)
        , c_(traits_type::eof())
        { }

        // Return the current character pointed to by iterator
        char_type operator*() const
        { return traits_type::to_char_type(get_()); }

        // Advance the iterator. Calls streambuf.sbumpc().
        istreambuf_iterator& operator++()
        {
            if (sbuf_ && !at_eof_()) {
                sbuf_->sbumpc();
                c_ = traits_type::eof();
            }
            return *this;
        }

        // Advance the iterator. Calls streambuf.sbumpc()
        istreambuf_iterator operator++(int)
        {
            istreambuf_iterator old = *this;
            if (sbuf_ && !at_eof_()) {
                old.c_ = sbuf_->sbumpc();
                c_ = traits_type::eof();
            }
            return old;
        }

        // Return true both iterators are end or both are not end
        bool equal(const istreambuf_iterator& b) const
        { return at_eof_() == b.at_eof_(); }

    private:
        int_type get_() const
        {
            const int_type eof = traits_type::eof();
            int_type ret = eof;
            if (sbuf_) {
                if (!traits_type::eq_int_type(c_, eof))
                    ret = c_;
                else if (!traits_type::eq_int_type((ret = sbuf_->sgetc()), eof))
                    c_ = ret;
                else
                    sbuf_ = nullptr;
            }
            return ret;
        }

        bool at_eof_() const
        { return traits_type::eq_int_type(get_(), traits_type::eof()); }
    };


    template <class CharT, class Traits>
    inline bool operator==(const istreambuf_iterator<CharT, Traits>& a,
                           const istreambuf_iterator<CharT, Traits>& b)
    { return a.equal(b); }

    template <class CharT, class Traits>
    inline bool operator!=(const istreambuf_iterator<CharT, Traits>& a,
                           const istreambuf_iterator<CharT, Traits>& b)
    { return !a.equal(b); }


    // Provides output iterator semantics for streambufs
    template <class CharT, class Traits>
    struct ostreambuf_iterator
    : std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        using char_type = CharT;
        using traits_type = Traits;

        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using ostream_type = basic_ostream<char_type, traits_type>;

    private:
        streambuf_type* sbuf_;
        bool failed_;

    public:
        // Construct output iterator from ostream
        ostreambuf_iterator(ostream_type& s) noexcept
        : sbuf_(s.rdbuf())
        , failed_(!sbuf_)
        { }

        // Construct output iterator from streambuf
        ostreambuf_iterator(streambuf_type* s) noexcept
        : sbuf_(s)
        , failed_(!sbuf_)
        { }

        // Write character to streambuf. Calls streambuf.sputc()
        ostreambuf_iterator& operator=(char_type c)
        {
            if (!failed_ &&
                traits_type::eq_int_type(sbuf_->sputc(c), traits_type::eof()))
            {
                failed_ = true;
            }
            return *this;
        }

        // Return *this
        ostreambuf_iterator& operator*()
        { return *this; }

        // Return *this
        ostreambuf_iterator& operator++(int)
        { return *this; }

        // Return *this
        ostreambuf_iterator& operator++()
        { return *this; }

        // Return true if previous operator=() failed
        bool failed() const noexcept
        { return failed_; }

        ostreambuf_iterator& put_(const char_type* ws, std::streamsize len)
        {
            if (!failed_ && sbuf_->sputn(ws, len) != len)
                failed_ = true;
            return *this;
        }
    };

} // namespace ard

