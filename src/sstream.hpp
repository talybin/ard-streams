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
#include <string>
#include <istream.hpp>
#include <ostream.hpp>

namespace ard
{
    // Template class basic_stringbuf
    template <class CharT, class Traits, class Alloc>
    struct basic_stringbuf : basic_streambuf<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using allocator_type = Alloc;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using string_type =
            std::basic_string<char_type, traits_type, allocator_type>;
        using size_type = typename string_type::size_type;

    private:
        struct xfer_bufptrs;

    protected:
        // Place to stash in || out || in | out settings for current stringbuf
        ios_base::openmode mode_;
        string_type string_;

    public:
        // Starts with an empty string buffer
        explicit basic_stringbuf(ios_base::openmode mode = ios_base::in | ios_base::out)
        : streambuf_type()
        , mode_(mode)
        , string_()
        { }

        //Starts with an existing string buffer
        explicit basic_stringbuf(const string_type& str,
		    ios_base::openmode mode = ios_base::in | ios_base::out)
        : streambuf_type()
        , mode_()
        , string_(str.data(), str.size())
        { string_buf_init(mode); }

        basic_stringbuf(const basic_stringbuf&) = delete;

        basic_stringbuf(basic_stringbuf&& rhs)
        : basic_stringbuf(std::move(rhs), xfer_bufptrs(rhs, this))
        { rhs.sync_(const_cast<char_type*>(rhs.string_.data()), 0, 0); }

        // 27.8.2.2 Assign and swap:

        basic_stringbuf& operator=(const basic_stringbuf&) = delete;

        basic_stringbuf& operator=(basic_stringbuf&& rhs)
        {
	        xfer_bufptrs st{rhs, this};
	        const streambuf_type& base = rhs;
	        streambuf_type::operator=(base);
	        mode_ = rhs.mode_;
	        string_ = std::move(rhs.string_);
	        rhs.sync_(const_cast<char_type*>(rhs.string_.data()), 0, 0);
	        return *this;
        }

        void swap(basic_stringbuf& rhs)
        {
	        xfer_bufptrs l_st{*this, std::addressof(rhs)};
	        xfer_bufptrs r_st{rhs, this};
	        streambuf_type& base = rhs;
	        streambuf_type::swap(base);
	        std::swap(mode_, rhs.mode_);
	        std::swap(string_, rhs.string_);
        }

        //
        // Get and set
        //

        // Copying out the string buffer
        string_type str() const
        {
	        string_type ret;
	        if (this->pptr()) {
	            // The current egptr() may not be the actual string end
	            if (this->pptr() > this->egptr())
	                ret = string_type(this->pbase(), this->pptr());
	            else
 	                ret = string_type(this->pbase(), this->egptr());
	        }
	        else
	            ret = string_;
	        return ret;
        }

        // Setting a new buffer
        void str(const string_type& s)
        {
	        // Cannot use string_ = s, since v3 strings are COW
	        // (not always true now but assign() always works)
	        string_.assign(s.data(), s.size());
	        string_buf_init(mode_);
        }

    protected:
        // Common initialization code goes here
        void string_buf_init(ios_base::openmode mode)
        {
	        mode_ = mode;
	        size_type len = 0;
	        if (mode_ & (ios_base::ate | ios_base::app))
	            len = string_.size();
	        sync_(const_cast<char_type*>(string_.data()), 0, len);
        }

        virtual std::streamsize showmanyc()
        {
	        std::streamsize ret = -1;
	        if (mode_ & ios_base::in) {
	            update_egptr_();
	            ret = this->egptr() - this->gptr();
	        }
	        return ret;
        }

        virtual int_type underflow();

        virtual int_type pbackfail(int_type c = traits_type::eof());

        virtual int_type overflow(int_type c = traits_type::eof());

        // Manipulates the buffer
        virtual streambuf_type* setbuf(char_type* s, std::streamsize n)
        {
	        if (s && n >= 0) {
	            // This is implementation-defined behavior, and assumes
	            // that an external char_type array of length n exists
	            // and has been pre-allocated. If this is not the case,
	            // things will quickly blow up.

	            // Step 1: Destroy the current internal array.
	            string_.clear();

	            // Step 2: Use the external array.
	            sync_(s, n, 0);
	        }
	        return this;
        }

        virtual pos_type seekoff(off_type off, ios_base::seekdir way,
	        ios_base::openmode mode = ios_base::in | ios_base::out);

        virtual pos_type seekpos(pos_type sp,
	        ios_base::openmode mode = ios_base::in | ios_base::out);

        // Internal function for correctly updating the internal buffer
        // for a particular string_, due to initialization or re-sizing
        // of an existing string_.
        void sync_(char_type* base, size_type i, size_type o);

        // Internal function for correctly updating egptr() to the actual
        // string end.
        void update_egptr_()
        {
	        const bool testin = mode_ & ios_base::in;
	        if (this->pptr() && this->pptr() > this->egptr()) {
	            if (testin)
	                this->setg(this->eback(), this->gptr(), this->pptr());
	            else
	                this->setg(this->pptr(), this->pptr(), this->pptr());
	        }
        }

        // Works around the issue with pbump, part of the protected
        // interface of basic_streambuf, taking just an int.
        void pbump_(char_type* pbeg, char_type* pend, off_type off);

    private:
        // This type captures the state of the gptr / pptr pointers as offsets
        // so they can be restored in another object after moving the string.
        struct xfer_bufptrs
        {
	        xfer_bufptrs(const basic_stringbuf& from, basic_stringbuf* to)
	        : to_{to}, goff_{-1, -1, -1}, poff_{-1, -1, -1}
	        {
	            const char_type* const str = from.string_.data();
	            const char_type* end = nullptr;
	            if (from.eback()) {
	                goff_[0] = from.eback() - str;
	                goff_[1] = from.gptr() - str;
	                goff_[2] = from.egptr() - str;
	                end = from.egptr();
	            }
	            if (from.pbase()) {
	                poff_[0] = from.pbase() - str;
	                poff_[1] = from.pptr() - from.pbase();
	                poff_[2] = from.epptr() - str;
	                if (from.pptr() > end)
		                end = from.pptr();
	            }

	            // Set string_ length to the greater of the get and put areas
	            if (end) {
	                // The const_cast avoids changing this constructor's signature,
	                // because it is exported from the dynamic library.
	                auto& mut_from = const_cast<basic_stringbuf&>(from);
	                mut_from.string_.length_(end - str);
	            }
	        }

	        ~xfer_bufptrs()
            {
	            char_type* str = const_cast<char_type*>(to_->string_.data());
	            if (goff_[0] != -1)
	                to_->setg(str + goff_[0], str + goff_[1], str + goff_[2]);
	            if (poff_[0] != -1)
	                to_->pbump_(str + poff_[0], str + poff_[2], poff_[1]);
	        }

	        basic_stringbuf* to_;
	        off_type goff_[3];
	        off_type poff_[3];
        };

        // The move constructor initializes an xfer_bufptrs temporary then
        // delegates to this constructor to performs moves during its lifetime
        basic_stringbuf(basic_stringbuf&& rhs, xfer_bufptrs&&)
        : streambuf_type(static_cast<const streambuf_type&>(rhs))
        , mode_(rhs.mode_)
        , string_(std::move(rhs.string_))
        { }
    };


    // [27.7.2] Template class basic_istringstream
    template <class CharT, class Traits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT>>
    struct basic_istringstream : basic_istream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using allocator_type = Alloc;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using string_type =
            std::basic_string<char_type, traits_type, allocator_type>;
        using stringbuf_type =
            basic_stringbuf<char_type, traits_type, allocator_type>;
        using istream_type = basic_istream<char_type, traits_type>;

    private:
        stringbuf_type string_buf_;

    public:
        // Default constructor starts with an empty string buffer
        explicit basic_istringstream(ios_base::openmode mode = ios_base::in)
        : istream_type()
        , string_buf_(mode | ios_base::in)
        { this->init(&string_buf_); }

        // Starts with an existing string buffer.
        explicit basic_istringstream(const string_type& str,
            ios_base::openmode mode = ios_base::in)
        : istream_type()
        , string_buf_(str, mode | ios_base::in)
        { this->init(&string_buf_); }

        // The destructor does nothing.
        ~basic_istringstream()
        { }

        basic_istringstream(const basic_istringstream&) = delete;

        basic_istringstream(basic_istringstream&& rhs)
        : istream_type(std::move(rhs))
        , string_buf_(std::move(rhs.string_buf_))
        { istream_type::set_rdbuf(&string_buf_); }

        // 27.8.3.2 Assign and swap:

        basic_istringstream& operator=(const basic_istringstream&) = delete;

        basic_istringstream& operator=(basic_istringstream&& rhs)
        {
	        istream_type::operator=(std::move(rhs));
	        string_buf_ = std::move(rhs.string_buf_);
	        return *this;
        }

        void swap(basic_istringstream& rhs)
        {
	        istream_type::swap(rhs);
	        string_buf_.swap(rhs.string_buf_);
        }

        // Accessing the underlying buffer
        stringbuf_type* rdbuf() const
        { return const_cast<stringbuf_type*>(&string_buf_); }

        // Copying out the string buffer
        string_type str() const
        { return string_buf_.str(); }

        // Setting a new buffer
        void str(const string_type& s)
        { string_buf_.str(s); }
    };


    // [27.7.3] Template class basic_ostringstream
    template <class CharT, class Traits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT>>
    struct basic_ostringstream : basic_ostream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using allocator_type = Alloc;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using string_type =
            std::basic_string<char_type, traits_type, allocator_type>;
        using stringbuf_type =
            basic_stringbuf<char_type, traits_type, allocator_type>;
        using ostream_type = basic_ostream<char_type, traits_type>;

    private:
        stringbuf_type string_buf_;

    public:
        // Default constructor starts with an empty string buffer
        explicit basic_ostringstream(ios_base::openmode mode = ios_base::out)
        : ostream_type()
        , string_buf_(mode | ios_base::out)
        { this->init(&string_buf_); }

        // Starts with an existing string buffer
        explicit basic_ostringstream(const string_type& str,
	        ios_base::openmode mode = ios_base::out)
        : ostream_type()
        , string_buf_(str, mode | ios_base::out)
        { this->init(&string_buf_); }

        // The destructor does nothing
        ~basic_ostringstream()
        { }

        basic_ostringstream(const basic_ostringstream&) = delete;

        basic_ostringstream(basic_ostringstream&& rhs)
        : ostream_type(std::move(rhs))
        , string_buf_(std::move(rhs.string_buf_))
        { ostream_type::set_rdbuf(&string_buf_); }

        // 27.8.3.2 Assign and swap:

        basic_ostringstream& operator=(const basic_ostringstream&) = delete;

        basic_ostringstream& operator=(basic_ostringstream&& rhs)
        {
	        ostream_type::operator=(std::move(rhs));
	        string_buf_ = std::move(rhs.string_buf_);
	        return *this;
        }

        void swap(basic_ostringstream& rhs)
        {
	        ostream_type::swap(rhs);
	        string_buf_.swap(rhs.string_buf_);
        }

        // Accessing the underlying buffer
        stringbuf_type* rdbuf() const
        { return const_cast<stringbuf_type*>(&string_buf_); }

        // Copying out the string buffer
        string_type str() const
        { return string_buf_.str(); }

        // Setting a new buffer
        void str(const string_type& s)
        { string_buf_.str(s); }
    };


    // [27.7.4] Template class basic_stringstream
    template <class CharT, class Traits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT>>
    struct basic_stringstream : basic_iostream<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;
        using allocator_type = Alloc;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using string_type =
            std::basic_string<char_type, traits_type, allocator_type>;
        using stringbuf_type =
            basic_stringbuf<char_type, traits_type, allocator_type>;
        using iostream_type = basic_iostream<char_type, traits_type>;

    private:
        stringbuf_type string_buf_;

    public:
        // Default constructor starts with an empty string buffer
        explicit basic_stringstream(
            ios_base::openmode m = ios_base::out | ios_base::in)
        : iostream_type()
        , string_buf_(m)
        { this->init(&string_buf_); }

        // Starts with an existing string buffer
        explicit basic_stringstream(const string_type& str,
	        ios_base::openmode m = ios_base::out | ios_base::in)
        : iostream_type()
        , string_buf_(str, m)
        { this->init(&string_buf_); }

        // The destructor does nothing
        ~basic_stringstream()
        { }

        basic_stringstream(const basic_stringstream&) = delete;

        basic_stringstream(basic_stringstream&& rhs)
        : iostream_type(std::move(rhs))
        , string_buf_(std::move(rhs.string_buf_))
        { iostream_type::set_rdbuf(&string_buf_); }

        // 27.8.3.2 Assign and swap:

        basic_stringstream& operator=(const basic_stringstream&) = delete;

        basic_stringstream& operator=(basic_stringstream&& rhs)
        {
	        iostream_type::operator=(std::move(rhs));
	        string_buf_ = std::move(rhs.string_buf_);
	        return *this;
        }

        void swap(basic_stringstream& rhs)
        {
	        iostream_type::swap(rhs);
	        string_buf_.swap(rhs.string_buf_);
        }

        // Accessing the underlying buffer
        stringbuf_type* rdbuf() const
        { return const_cast<stringbuf_type*>(&string_buf_); }

        // Copying out the string buffer
        string_type str() const
        { return string_buf_.str(); }

        // Setting a new buffer
        void str(const string_type& s)
        { string_buf_.str(s); }
    };

    // Swap specialization for stringbufs
    template <class CharT, class Traits, class Allocator>
    inline void swap(basic_stringbuf<CharT, Traits, Allocator>& x,
	                 basic_stringbuf<CharT, Traits, Allocator>& y)
    { x.swap(y); }

    // Swap specialization for istringstreams
    template <class CharT, class Traits, class Allocator>
    inline void swap(basic_istringstream<CharT, Traits, Allocator>& x,
	                 basic_istringstream<CharT, Traits, Allocator>& y)
    { x.swap(y); }

    // Swap specialization for ostringstreams
    template <class CharT, class Traits, class Allocator>
    inline void swap(basic_ostringstream<CharT, Traits, Allocator>& x,
	                 basic_ostringstream<CharT, Traits, Allocator>& y)
    { x.swap(y); }

    // Swap specialization for stringstreams
    template <class CharT, class Traits, class Allocator>
    inline void swap(basic_stringstream<CharT, Traits, Allocator>& x,
	                 basic_stringstream<CharT, Traits, Allocator>& y)
    { x.swap(y); }

    //
    // Methods
    //

    template <class CharT, class Traits, class Alloc>
    inline typename basic_stringbuf<CharT, Traits, Alloc>::int_type
    basic_stringbuf<CharT, Traits, Alloc>::
    pbackfail(int_type c)
    {
        int_type ret = traits_type::eof();
        if (this->eback() < this->gptr()) {
            // Try to put back c into input sequence in one of three ways.
            // Order these tests done in is unspecified by the standard.
            const bool testeof = traits_type::eq_int_type(c, ret);
            if (!testeof) {
                const bool testeq = traits_type::eq(
                    traits_type::to_char_type(c), this->gptr()[-1]);
                const bool testout = this->mode_ & ios_base::out;
                if (testeq || testout) {
                    this->gbump(-1);
                    if (!testeq)
                        *this->gptr() = traits_type::to_char_type(c);
                    ret = c;
                }
            }
            else {
                this->gbump(-1);
                ret = traits_type::not_eof(c);
            }
        }
        return ret;
    }

    template <class CharT, class Traits, class Alloc>
    inline typename basic_stringbuf<CharT, Traits, Alloc>::int_type
    basic_stringbuf<CharT, Traits, Alloc>::
    overflow(int_type c)
    {
        const bool testout = this->mode_ & ios_base::out;
        if (!testout)
            return traits_type::eof();

        const bool testeof = traits_type::eq_int_type(c, traits_type::eof());
        if (testeof)
            return traits_type::not_eof(c);

        const size_type capacity = string_.capacity();

        if (size_type(this->epptr() - this->pbase()) < capacity) {
            // There is additional capacity in string_ that can be used
            char_type* base = const_cast<char_type*>(string_.data());
            pbump_(base, base + capacity, this->pptr() - this->pbase());
            if (mode_ & ios_base::in) {
                const size_type nget = this->gptr() - this->eback();
                const size_type eget = this->egptr() - this->eback();
                this->setg(base, base + nget, base + eget + 1);
            }
            *this->pptr() = traits_type::to_char_type(c);
            this->pbump(1);
            return c;
        }

        const size_type max_size = string_.max_size();
        const bool testput = this->pptr() < this->epptr();
        if (!testput && capacity == max_size)
            return traits_type::eof();

        // Try to append c into output sequence in one of two ways.
        // Order these tests done in is unspecified by the standard.
        const char_type conv = traits_type::to_char_type(c);
        if (!testput) {
            // NB: Start ostringstream buffers at 512 chars. This is an
            // experimental value (pronounced "arbitrary" in some of the
            // hipper English-speaking countries), and can be changed to
            // suit particular needs.
            const size_type opt_len =
                std::max(size_type(2 * capacity), size_type(512));
            const size_type len = std::min(opt_len, max_size);
            string_type tmp;
            tmp.reserve(len);
            if (this->pbase())
                tmp.assign(this->pbase(), this->epptr() - this->pbase());
            tmp.push_back(conv);
            string_.swap(tmp);
            sync_(const_cast<char_type*>(string_.data()),
                this->gptr() - this->eback(), this->pptr() - this->pbase());
        }
        else
            *this->pptr() = conv;
        this->pbump(1);
        return c;
    }

    template <class CharT, class Traits, class Alloc>
    inline typename basic_stringbuf<CharT, Traits, Alloc>::int_type
    basic_stringbuf<CharT, Traits, Alloc>::
    underflow()
    {
        int_type ret = traits_type::eof();
        const bool testin = this->mode_ & ios_base::in;
        if (testin) {
            // Update egptr() to match the actual string end.
            update_egptr_();

            if (this->gptr() < this->egptr())
                ret = traits_type::to_int_type(*this->gptr());
        }
        return ret;
    }

    template <class CharT, class Traits, class Alloc>
    inline typename basic_stringbuf<CharT, Traits, Alloc>::pos_type
    basic_stringbuf<CharT, Traits, Alloc>::
    seekoff(off_type off, ios_base::seekdir way, ios_base::openmode mode)
    {
        pos_type ret = pos_type(off_type(-1));
        bool testin = ios_base::in & this->mode_ & mode;
        bool testout = ios_base::out & this->mode_ & mode;
        const bool testboth = testin && testout && way != ios_base::cur;
        testin &= !(mode & ios_base::out);
        testout &= !(mode & ios_base::in);

        const char_type* beg = testin ? this->eback() : this->pbase();
        if ((beg || !off) && (testin || testout || testboth)) {
            update_egptr_();

            off_type newoffi = off;
            off_type newoffo = newoffi;
            if (way == ios_base::cur) {
                newoffi += this->gptr() - beg;
                newoffo += this->pptr() - beg;
            }
            else if (way == ios_base::end)
                newoffo = newoffi += this->egptr() - beg;

            if ((testin || testboth) && newoffi >= 0
                && this->egptr() - beg >= newoffi)
            {
                this->setg(this->eback(), this->eback() + newoffi, this->egptr());
                ret = pos_type(newoffi);
            }
            if ((testout || testboth) && newoffo >= 0
                && this->egptr() - beg >= newoffo)
            {
                pbump_(this->pbase(), this->epptr(), newoffo);
                ret = pos_type(newoffo);
            }
        }
        return ret;
    }

    template <class CharT, class Traits, class Alloc>
    inline typename basic_stringbuf<CharT, Traits, Alloc>::pos_type
    basic_stringbuf<CharT, Traits, Alloc>::
    seekpos(pos_type sp, ios_base::openmode mode)
    {
        pos_type ret =  pos_type(off_type(-1));
        const bool testin = ios_base::in & this->mode_ & mode;
        const bool testout = ios_base::out & this->mode_ & mode;

        const char_type* beg = testin ? this->eback() : this->pbase();
        if ((beg || !off_type(sp)) && (testin || testout)) {
            update_egptr_();

            const off_type pos(sp);
            const bool testpos = (0 <= pos && pos <= this->egptr() - beg);
            if (testpos) {
                if (testin)
                    this->setg(this->eback(), this->eback() + pos, this->egptr());
                if (testout)
                    pbump_(this->pbase(), this->epptr(), pos);
                ret = sp;
            }
        }
        return ret;
    }

    template <class CharT, class Traits, class Alloc>
    inline void basic_stringbuf<CharT, Traits, Alloc>::
    sync_(char_type* base, size_type i, size_type o)
    {
        const bool testin = mode_ & ios_base::in;
        const bool testout = mode_ & ios_base::out;
        char_type* endg = base + string_.size();
        char_type* endp = base + string_.capacity();

        if (base != string_.data()) {
            // setbuf: i == size of buffer area (string_.size() == 0)
            endg += i;
            i = 0;
            endp = endg;
        }

        if (testin)
            this->setg(base, base + i, endg);
        if (testout) {
            pbump_(base, endp, o);
            // egptr() always tracks the string end. When !testin,
            // for the correct functioning of the streambuf inlines
            // the other get area pointers are identical.
            if (!testin)
                this->setg(endg, endg, endg);
        }
    }

    template <class CharT, class Traits, class Alloc>
    inline void basic_stringbuf<CharT, Traits, Alloc>::
    pbump_(char_type* pbeg, char_type* pend, off_type off)
    {
        this->setp(pbeg, pend);
        while (off > std::numeric_limits<int>::max()) {
            this->pbump(std::numeric_limits<int>::max());
            off -= std::numeric_limits<int>::max();
        }
        this->pbump(off);
    }


    // Alias
    using istringstream = basic_istringstream<char>;
    using ostringstream = basic_ostringstream<char>;
    using stringstream = basic_stringstream<char>;

} // namespace ard

