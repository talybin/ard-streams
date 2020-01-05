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
#include <ios.hpp>
#include <bits/ostream_insert.hpp>

namespace ard
{
    // Forward declarations
    template <class CharT, class Traits>
    struct basic_iostream;

    // Template class basic_ostream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_ostream : virtual basic_ios<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using ios_type = basic_ios<char_type, traits_type>;
        using ostream_type = basic_ostream<char_type, traits_type>;

        // Base constructor
        explicit basic_ostream(streambuf_type* sb)
        { this->init(sb); }

        // Base destructor
        virtual ~basic_ostream()
        { }

        // Safe prefix/suffix operations
        struct sentry;
        friend struct sentry;

        // Interface for manipulators
        ostream_type& operator<<(ostream_type& (*pf)(ostream_type&))
        { return pf(*this); }

        ostream_type& operator<<(ios_type& (*pf)(ios_type&))
        {
	        pf(*this);
	        return *this;
        }

        ostream_type& operator<<(ios_base& (*pf)(ios_base&))
        {
	        pf(*this);
	        return *this;
        }

        //
        // Inserters
        //

        // Integer arithmetic inserters
        ostream_type& operator<<(long n)
        { return insert_(n); }

        ostream_type& operator<<(unsigned long n)
        { return insert_(n); }

        ostream_type& operator<<(bool n)
        { return insert_(n); }

        ostream_type& operator<<(short n);

        ostream_type& operator<<(unsigned short n)
        { return insert_(static_cast<unsigned long>(n)); }

        ostream_type& operator<<(int n);

        ostream_type& operator<<(unsigned int n)
        { return insert_(static_cast<unsigned long>(n)); }

        ostream_type& operator<<(long long n)
        { return insert_(n); }

        ostream_type& operator<<(unsigned long long n)
        { return insert_(n); }

        // Floating point arithmetic inserters
        ostream_type& operator<<(double f)
        { return insert_(f); }

        ostream_type& operator<<(float f)
        { return insert_(static_cast<double>(f)); }

        ostream_type& operator<<(long double f)
        { return insert_(f); }

        // Pointer arithmetic inserters
        ostream_type& operator<<(const void* p)
        { return insert_(p); }

        // Extracting from another streambuf
        ostream_type& operator<<(streambuf_type* sb);

        //
        // Unformatted Output Functions
        //

        // Simple insertion
        ostream_type& put(char_type c);

        // Core write functionality, without sentry
        void write_(const char_type* s, std::streamsize n)
        {
	        const std::streamsize put = this->rdbuf()->sputn(s, n);
	        if (put != n)
	            this->setstate(ios_base::badbit);
        }

        // Character string insertion
        ostream_type& write(const char_type* s, std::streamsize n);

        // Synchronizing the stream buffer
        ostream_type& flush();

        // Getting the current write position
        pos_type tellp();

        // Changing the current write position
        ostream_type& seekp(pos_type);

        // Changing the current write position
        ostream_type& seekp(off_type, ios_base::seekdir);

    protected:
        basic_ostream()
        { this->init(nullptr); }

        // Non-standard constructor that does not call init()
        basic_ostream(basic_iostream<char_type, traits_type>&) { }

        basic_ostream(const basic_ostream&) = delete;

        basic_ostream(basic_ostream&& rhs)
        : ios_type()
        { ios_type::move(rhs); }

        // 27.7.3.3 Assign/swap

        basic_ostream& operator=(const basic_ostream&) = delete;

        basic_ostream& operator=(basic_ostream&& rhs)
        {
	        swap(rhs);
	        return *this;
        }

        void swap(basic_ostream& rhs)
        { ios_type::swap(rhs); }

        template <class ValueT>
	    ostream_type& insert_(ValueT v);
    };

    // Performs setup work for output streams
    template <typename CharT, typename Traits>
    struct basic_ostream<CharT, Traits>::sentry
    {
        // The constructor performs preparatory work
        explicit sentry(basic_ostream<CharT, Traits>& os)
        : ok_(false)
        , os_(os)
        {
            if (os.tie() && os.good())
                os.tie()->flush();

            if (os.good())
                ok_ = true;
            else
                os.setstate(ios_base::failbit);
        }

        // Possibly flushes the stream
        ~sentry()
        {
	        if (os_.flags() & ios_base::unitbuf) {
	            // Can't call flush directly or else will get into recursive lock
	            if (os_.rdbuf() && os_.rdbuf()->pubsync() == -1)
	                os_.setstate(ios_base::badbit);
	        }
        }

        // Quick status checking
        explicit operator bool() const
        { return ok_; }

    private:
        bool ok_;
        basic_ostream<CharT, Traits>& os_;
    };

    // Character inserters
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& out, CharT c)
    { return ostream_insert(out, &c, 1); }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& out, char c)
    { return (out << out.widen(c)); }

    // Specialization
    inline basic_ostream<char>&
    operator<<(basic_ostream<char>& out, char c)
    { return ostream_insert(out, &c, 1); }

    // Signed and unsigned
    inline basic_ostream<char>&
    operator<<(basic_ostream<char>& out, signed char c)
    { return (out << static_cast<char>(c)); }

    inline basic_ostream<char>&
    operator<<(basic_ostream<char>& out, unsigned char c)
    { return (out << static_cast<char>(c)); }

    // String inserters
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& out, const CharT* s)
    {
        if (!s)
	        out.setstate(ios_base::badbit);
        else
	        ostream_insert(out, s, static_cast<std::streamsize>(Traits::length(s)));
        return out;
    }

    template <class CharT, class Traits>
    basic_ostream<CharT, Traits> &
    operator<<(basic_ostream<CharT, Traits>& out, const char* s);

    // Partial specializations
    inline basic_ostream<char>&
    operator<<(basic_ostream<char>& out, const char* s)
    {
        using traits_type = typename basic_ostream<char>::traits_type;
        if (!s)
	        out.setstate(ios_base::badbit);
        else
	        ostream_insert(out, s,
                static_cast<std::streamsize>(traits_type::length(s)));
        return out;
    }

    // Signed and unsigned
    inline basic_ostream<char>&
    operator<<(basic_ostream<char>& out, const signed char* s)
    { return (out << reinterpret_cast<const char*>(s)); }

    inline basic_ostream<char> &
    operator<<(basic_ostream<char>& out, const unsigned char* s)
    { return (out << reinterpret_cast<const char*>(s)); }

    //
    // Standard basic_ostream manipulators
    //

    // Write a newline and flush the stream
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    endl(basic_ostream<CharT, Traits>& os)
    { return flush(os.put(os.widen('\n'))); }

    // Write a null character into the output sequence
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    ends(basic_ostream<CharT, Traits>& os)
    { return os.put(CharT()); }

    // Flushes the output stream
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    flush(basic_ostream<CharT, Traits>& os)
    { return os.flush(); }


    template <class CharT, class Traits>
    template <class ValueT>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    insert_(ValueT v)
    {
        sentry cerb(*this);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            if (this->num_put_.put(*this, *this, this->fill(), v).failed()) {
                err |= ios_base::badbit;
            }
            if (err)
                this->setstate(err);
        }
        return *this;
    }
    
    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    operator<<(short n)
    {
        const ios_base::fmtflags fmt = this->flags() & ios_base::basefield;
        if (fmt == ios_base::oct || fmt == ios_base::hex)
            return insert_(static_cast<long>(static_cast<unsigned short>(n)));
        else
            return insert_(static_cast<long>(n));
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    operator<<(int n)
    {
        const ios_base::fmtflags fmt = this->flags() & ios_base::basefield;
        if (fmt == ios_base::oct || fmt == ios_base::hex)
            return insert_(static_cast<long>(static_cast<unsigned int>(n)));
        else
            return insert_(static_cast<long>(n));
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    operator<<(streambuf_type* sbin)
    {
        ios_base::iostate err = ios_base::goodbit;
        sentry cerb(*this);
        if (cerb && sbin) {
            if (!copy_streambufs(sbin, this->rdbuf()))
                err |= ios_base::failbit;
        }
        else if (!sbin)
            err |= ios_base::badbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    put(char_type c)
    {
        sentry cerb(*this);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type p = this->rdbuf()->sputc(c);
            if (traits_type::eq_int_type(p, traits_type::eof()))
                err |= ios_base::badbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    write(const CharT* s, std::streamsize n)
    {
        sentry cerb(*this);
        if (cerb)
            write_(s, n);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    flush()
    {
        ios_base::iostate err = ios_base::goodbit;
        if (this->rdbuf() && this->rdbuf()->pubsync() == -1)
            err |= ios_base::badbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline typename basic_ostream<CharT, Traits>::pos_type
    basic_ostream<CharT, Traits>::
    tellp()
    {
        pos_type ret = pos_type(-1);
        if (!this->fail())
            ret = this->rdbuf()->pubseekoff(0, ios_base::cur, ios_base::out);
        return ret;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    seekp(pos_type pos)
    {
        ios_base::iostate err = ios_base::goodbit;
        if (!this->fail()) {
            const pos_type p = this->rdbuf()->pubseekpos(pos, ios_base::out);

            // 129. Need error indication from seekp() and seekg()
            if (p == pos_type(off_type(-1)))
                err |= ios_base::failbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>& basic_ostream<CharT, Traits>::
    seekp(off_type off, ios_base::seekdir dir)
    {
        ios_base::iostate err = ios_base::goodbit;
        if (!this->fail()) {
            const pos_type p =
                this->rdbuf()->pubseekoff(off, dir, ios_base::out);

            // 129. Need error indication from seekp() and seekg()
            if (p == pos_type(off_type(-1)))
                err |= ios_base::failbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& out, const char* s)
    {
        if (!s)
            out.setstate(ios_base::badbit);
        else {
            const size_t clen = char_traits<char>::length(s);
            CharT ws[clen];

            for (size_t  i = 0; i < clen; ++i)
                ws[i] = out.widen(s[i]);
            ostream_insert(out, ws, clen);
        }
        return out;
    }

    //
    // Types from std
    //

    template <class CharT, class Traits, class Alloc>
    inline basic_ostream<CharT, Traits>&
    operator<<(basic_ostream<CharT, Traits>& out,
               const std::basic_string<CharT, Traits, Alloc>& str)
    {
        return ostream_insert(out, str.data(), str.size());
    }

    //
    // Alias
    //

    using ostream = basic_ostream<char>;

} // namespace ard

