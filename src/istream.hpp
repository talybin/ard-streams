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

namespace ard
{
    // Template class basic_istream
    template <class CharT, class Traits = char_traits<CharT>>
    struct basic_istream : virtual basic_ios<CharT, Traits>
    {
        using char_type = CharT;
        using traits_type = Traits;

        using istream_type = basic_istream<char_type, traits_type>;
        using ios_type = basic_ios<char_type, traits_type>;

        using int_type = typename ios_type::int_type;
        using pos_type = typename ios_type::pos_type;
        using off_type = typename ios_type::off_type;

        using streambuf_type = typename ios_type::streambuf_type;
        using num_get_type = typename ios_type::num_get_type;

    protected:
        // The number of characters extracted in the previous unformatted
        // function; see gcount()
        std::streamsize gcount_ = 0;

    public:
        // Base constructor
        explicit basic_istream(streambuf_type* sb)
        { this->init(sb); }

        // Base destructor
        virtual ~basic_istream()
        { }

        // Safe prefix/suffix operations
        struct sentry;
        friend struct sentry;

        //
        // Interface for manipulators
        //

        istream_type& operator>>(istream_type& (*pf)(istream_type&))
        { return pf(*this); }

        istream_type& operator>>(ios_type& (*pf)(ios_type&))
        {
            pf(*this);
            return *this;
        }

        istream_type& operator>>(ios_base& (*pf)(ios_base&))
        {
            pf(*this);
            return *this;
        }

        //
        // Integer arithmetic extractors
        //

        istream_type& operator>>(bool& n)
        { return extract_(n); }

        istream_type& operator>>(short& n)
        { return extract_(n); }

        istream_type& operator>>(unsigned short& n)
        { return extract_(n); }

        istream_type& operator>>(int& n);

        istream_type& operator>>(unsigned int& n)
        { return extract_(n); }

        istream_type& operator>>(long& n)
        { return extract_(n); }

        istream_type& operator>>(unsigned long& n)
        { return extract_(n); }

        istream_type& operator>>(long long& n)
        { return extract_(n); }

        istream_type& operator>>(unsigned long long& n)
        { return extract_(n); }

        //
        // Floating point arithmetic extractors
        //

        istream_type& operator>>(float& f)
        { return extract_(f); }

        istream_type& operator>>(double& f)
        { return extract_(f); }

        istream_type& operator>>(long double& f)
        { return extract_(f); }

        //
        // Basic arithmetic extractors
        //

        istream_type& operator>>(void*& p)
        { return extract_(p); }

        // Extracting into another streambuf
        istream_type& operator>>(streambuf_type* sb);

        //
        // Unformatted Input Functions
        //

        // Character counting
        std::streamsize gcount() const
        { return gcount_; }

        // Simple extraction
        int_type get();

        // Simple extraction
        istream_type& get(char_type& c);

        // Simple multiple-character extraction
        istream_type& get(char_type* s, std::streamsize n, char_type delim);

        // Simple multiple-character extraction
        istream_type& get(char_type* s, std::streamsize n)
        { return this->get(s, n, traits_type::widen('\n')); }

        // Extraction into another streambuf
        istream_type& get(streambuf_type& sb, char_type delim);

        // Extraction into another streambuf
        istream_type& get(streambuf_type& sb)
        { return this->get(sb, traits_type::widen('\n')); }

        // String extraction
        istream_type& getline(char_type* s, std::streamsize n, char_type delim);

        // String extraction
        istream_type& getline(char_type* s, std::streamsize n)
        { return this->getline(s, n, traits_type::widen('\n')); }

        // Discarding characters
        istream_type& ignore(std::streamsize n, int_type delim);
        istream_type& ignore(std::streamsize n);
        istream_type& ignore();

        // Looking ahead in the stream
        int_type peek();

        // Extraction without delimiters
        istream_type& read(char_type* s, std::streamsize n);

        // Extraction until the buffer is exhausted, but no more
        std::streamsize readsome(char_type* s, std::streamsize n);

        // Unextracting a single character
        istream_type& putback(char_type c);

        // Unextracting the previous character
        istream_type& unget();

        // Synchronizing the stream buffer
        int sync();

        // Getting the current read position
        pos_type tellg();

        // Changing the current read position
        istream_type& seekg(pos_type);

        // Changing the current read position
        istream_type& seekg(off_type, ios_base::seekdir);

    protected:
        basic_istream()
        { this->init(nullptr); }

        basic_istream(const basic_istream&) = delete;

        basic_istream(basic_istream&& rhs)
        : ios_type()
        , gcount_(rhs.gcount_)
        {
            ios_type::move(rhs);
            rhs.gcount_ = 0;
        }

        // Assign
        basic_istream& operator=(basic_istream&& rhs)
        {
            swap(rhs);
            return *this;
        }

        // Swap
        void swap(basic_istream& rhs)
        {
            ios_type::swap(rhs);
            std::swap(gcount_, rhs.gcount_);
        }

        template <class ValueT>
        istream_type& extract_(ValueT& v);
    };


    // Performs setup work for input streams
    template <class CharT, class Traits>
    struct basic_istream<CharT, Traits>::sentry
    {
        using char_type = CharT;
        using traits_type = Traits;

        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using istream_type = basic_istream<char_type, traits_type>;
        using int_type = typename traits_type::int_type;
        using ct = ctype<char_type>;

        // The constructor performs all the work
        explicit sentry(istream_type& is, bool noskipws = false)
        {
            ios_base::iostate err = ios_base::goodbit;
            if (is.good()) {
                if (is.tie())
                    is.tie()->flush();
                if (!noskipws && (is.flags() & ios_base::skipws)) {
                    const int_type eof = traits_type::eof();
                    streambuf_type* sb = is.rdbuf();
                    int_type c = sb->sgetc();

                    while (ct::is_wsp(c))
                        c = sb->snextc();

                    if (traits_type::eq_int_type(c, eof))
                        err |= ios_base::eofbit;
                }
            }

            if (is.good() && err == ios_base::goodbit)
                ok_ = true;
            else
                is.setstate(err | ios_base::failbit);
        }

        // Quick status checking
        explicit operator bool() const
        { return ok_; }

    private:
        bool ok_ = false;
    };


    template <class CharT, class Traits>
    template <class ValueT>
    inline basic_istream<CharT, Traits>& basic_istream<CharT, Traits>::
    extract_(ValueT& v)
    {
        sentry cerb(*this, false);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            this->num_get_.get(*this, 0, *this, err, v);
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::operator>>(int& n)
    {
        sentry cerb(*this, false);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            long l;
            this->num_get_.get(*this, 0, *this, err, l);

            if (l < std::numeric_limits<int>::min()) {
                err |= ios_base::failbit;
                n = std::numeric_limits<int>::min();
            }
            else if (l > std::numeric_limits<int>::max()) {
                err |= ios_base::failbit;
                n = std::numeric_limits<int>::max();
            }
            else
                n = int(l);

            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::operator>>(streambuf_type* sbout)
    {
        ios_base::iostate err = ios_base::goodbit;
        sentry cerb(*this, false);
        if (cerb && sbout) {
            bool ineof;
            if (!copy_streambufs_eof(this->rdbuf(), sbout, ineof))
                err |= ios_base::failbit;
            if (ineof)
                err |= ios_base::eofbit;
        }
        else if (!sbout)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline typename basic_istream<CharT, Traits>::int_type
    basic_istream<CharT, Traits>::get()
    {
        const int_type eof = traits_type::eof();
        int_type c = eof;
        gcount_ = 0;
        ios_base::iostate err = ios_base::goodbit;

        sentry cerb(*this, true);
        if (cerb) {
            c = this->rdbuf()->sbumpc();
            // 27.6.1.1 paragraph 3
            if (!traits_type::eq_int_type(c, eof))
                gcount_ = 1;
            else
                err |= ios_base::eofbit;
        }
        if (!gcount_)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return c;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::get(char_type& c)
    {
        gcount_ = 0;
        ios_base::iostate err = ios_base::goodbit;

        sentry cerb(*this, true);
        if (cerb) {
            const int_type cb = this->rdbuf()->sbumpc();
            // 27.6.1.1 paragraph 3
            if (!traits_type::eq_int_type(cb, traits_type::eof())) {
                gcount_ = 1;
                c = traits_type::to_char_type(cb);
            }
            else
                err |= ios_base::eofbit;
        }
        if (!gcount_)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::get(char_type* s, std::streamsize n, char_type delim)
    {
        gcount_ = 0;
        ios_base::iostate err = ios_base::goodbit;

        sentry cerb(*this, true);
        if (cerb) {
            const int_type idelim = traits_type::to_int_type(delim);
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();
            int_type c = sb->sgetc();

            while (gcount_ + 1 < n
                    && !traits_type::eq_int_type(c, eof)
                    && !traits_type::eq_int_type(c, idelim))
            {
                *s++ = traits_type::to_char_type(c);
                ++gcount_;
                c = sb->snextc();
            }
            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
        }
        if (n > 0)
            *s = char_type();
        if (!gcount_)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::get(streambuf_type& sb, char_type delim)
    {
        gcount_ = 0;
        ios_base::iostate err = ios_base::goodbit;

        sentry cerb(*this, true);
        if (cerb) {
            const int_type idelim = traits_type::to_int_type(delim);
            const int_type eof = traits_type::eof();
            streambuf_type* this_sb = this->rdbuf();
            int_type c = this_sb->sgetc();
            char_type c2 = traits_type::to_char_type(c);

            while (!traits_type::eq_int_type(c, eof)
                    && !traits_type::eq_int_type(c, idelim)
                    && !traits_type::eq_int_type(sb.sputc(c2), eof))
            {
                ++gcount_;
                c = this_sb->snextc();
                c2 = traits_type::to_char_type(c);
            }
            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
        }
        if (!gcount_)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>& basic_istream<CharT, Traits>::
    getline(char_type* s, std::streamsize n, char_type delim)
    {
        gcount_ = 0;
        ios_base::iostate err = ios_base::goodbit;
        sentry cerb(*this, true);
        if (cerb) {
            const int_type idelim = traits_type::to_int_type(delim);
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();
            int_type c = sb->sgetc();

            while (gcount_ + 1 < n &&
                   !traits_type::eq_int_type(c, eof) &&
                   !traits_type::eq_int_type(c, idelim))
            {
                *s++ = traits_type::to_char_type(c);
                c = sb->snextc();
                ++gcount_;
            }

            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
            else if (traits_type::eq_int_type(c, idelim)) {
                sb->sbumpc();
                ++gcount_;
            }
            else
                err |= ios_base::failbit;
        }
        // get and getline when sentry reports failure.
        if (n > 0)
            *s = char_type();
        if (!gcount_)
            err |= ios_base::failbit;
        if (err)
            this->setstate(err);
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>& basic_istream<CharT, Traits>::
    ignore()
    {
        gcount_ = 0;
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();

            if (traits_type::eq_int_type(sb->sbumpc(), eof))
                err |= ios_base::eofbit;
            else
                gcount_ = 1;

            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>& basic_istream<CharT, Traits>::
    ignore(std::streamsize n)
    {
        gcount_ = 0;
        sentry cerb(*this, true);
        if (n > 0 && cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();
            int_type c = sb->sgetc();

            bool large_ignore = false;
            while (true) {
                while (gcount_ < n && !traits_type::eq_int_type(c, eof)) {
                    ++gcount_;
                    c = sb->snextc();
                }
                if (n == std::numeric_limits<std::streamsize>::max() &&
                    !traits_type::eq_int_type(c, eof))
                {
                    gcount_ = std::numeric_limits<std::streamsize>::min();
                    large_ignore = true;
                }
                else break;
            }

            if (large_ignore)
                gcount_ = std::numeric_limits<std::streamsize>::max();

            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>& basic_istream<CharT, Traits>::
    ignore(std::streamsize n, int_type delim)
    {
        if (traits_type::eq_int_type(delim, traits_type::eof()))
            return ignore(n);

        gcount_ = 0;
        sentry cerb(*this, true);
        if (n > 0 && cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();
            int_type c = sb->sgetc();

            bool large_ignore = false;
            while (true) {
                while (gcount_ < n &&
                       !traits_type::eq_int_type(c, eof) &&
                       !traits_type::eq_int_type(c, delim))
                {
                    ++gcount_;
                    c = sb->snextc();
                }
                if (n == std::numeric_limits<std::streamsize>::max() &&
                    !traits_type::eq_int_type(c, eof) &&
                    !traits_type::eq_int_type(c, delim))
                {
                    gcount_ = std::numeric_limits<std::streamsize>::min();
                    large_ignore = true;
                }
                else break;
            }

            if (large_ignore)
                gcount_ = std::numeric_limits<std::streamsize>::max();

            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
            else if (traits_type::eq_int_type(c, delim)) {
                if (gcount_ < std::numeric_limits<std::streamsize>::max())
                    ++gcount_;
                sb->sbumpc();
            }
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline typename basic_istream<CharT, Traits>::int_type
    basic_istream<CharT, Traits>::peek()
    {
        int_type c = traits_type::eof();
        gcount_ = 0;
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            c = this->rdbuf()->sgetc();
            if (traits_type::eq_int_type(c, traits_type::eof()))
                err |= ios_base::eofbit;
            if (err)
                this->setstate(err);
        }
        return c;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::read(char_type* s, std::streamsize n)
    {
        gcount_ = 0;
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            gcount_ = this->rdbuf()->sgetn(s, n);
            if (gcount_ != n)
                err |= (ios_base::eofbit | ios_base::failbit);
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline std::streamsize
    basic_istream<CharT, Traits>::readsome(char_type* s, std::streamsize n)
    {
        gcount_ = 0;
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const std::streamsize num = this->rdbuf()->in_avail();
            if (num == traits_type::eof())
                err |= ios_base::eofbit;
            else if (num > 0)
                gcount_ = this->rdbuf()->sgetn(s, std::min(num, n));
            if (err)
                this->setstate(err);
        }
        return gcount_;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::putback(char_type c)
    {
        gcount_ = 0;
        // Clear eofbit per N3168
        this->clear(this->rdstate() & ~ios_base::eofbit);
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();

            if (!sb || traits_type::eq_int_type(sb->sputbackc(c), eof))
                err |= ios_base::badbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::unget()
    {
        gcount_ = 0;
        // Clear eofbit per N3168
        this->clear(this->rdstate() & ~ios_base::eofbit);
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type eof = traits_type::eof();
            streambuf_type* sb = this->rdbuf();

            if (!sb || traits_type::eq_int_type(sb->sungetc(), eof))
                err |= ios_base::badbit;
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline int
    basic_istream<CharT, Traits>::sync()
    {
        // DR60. Do not change gcount_.
        int ret = -1;
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            streambuf_type* sb = this->rdbuf();

            if (sb) {
                if (sb->pubsync() == -1)
                    err |= ios_base::badbit;
                else
                    ret = 0;
            }
            if (err)
                this->setstate(err);
        }
        return ret;
    }

    template <class CharT, class Traits>
    inline typename basic_istream<CharT, Traits>::pos_type
    basic_istream<CharT, Traits>::tellg()
    {
        // DR60. Do not change gcount_.
        pos_type ret = pos_type(-1);
        sentry cerb(*this, true);
        if (cerb) {
            if (!this->fail())
                ret = this->rdbuf()->pubseekoff(0, ios_base::cur, ios_base::in);
        }
        return ret;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::seekg(pos_type pos)
    {
        // DR60. Do not change gcount_.
        // Clear eofbit per N3168.
        this->clear(this->rdstate() & ~ios_base::eofbit);
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            if (!this->fail()) {
                // 136. seekp, seekg setting wrong streams?
                const pos_type p = this->rdbuf()->pubseekpos(pos, ios_base::in);
                // 129. Need error indication from seekp() and seekg()
                if (p == pos_type(off_type(-1)))
                    err |= ios_base::failbit;
            }
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    basic_istream<CharT, Traits>::seekg(off_type off, ios_base::seekdir dir)
    {
        // DR60. Do not change gcount_.
        // Clear eofbit per N3168.
        this->clear(this->rdstate() & ~ios_base::eofbit);
        sentry cerb(*this, true);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            if (!this->fail()) {
                // 136. seekp, seekg setting wrong streams?
                const pos_type p = this->rdbuf()->pubseekoff(off, dir, ios_base::in);
                // 129. Need error indication from seekp() and seekg()
                if (p == pos_type(off_type(-1)))
                    err |= ios_base::failbit;
            }
            if (err)
                this->setstate(err);
        }
        return *this;
    }

    //
    // Character extractors
    //

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    operator>>(basic_istream<CharT, Traits>& in, CharT& c)
    {
        using istream_type = basic_istream<CharT, Traits>;
        using int_type = typename istream_type::int_type;

        typename istream_type::sentry cerb(in, false);
        if (cerb) {
            ios_base::iostate err = ios_base::goodbit;
            const int_type cb = in.rdbuf()->sbumpc();

            if (!Traits::eq_int_type(cb, Traits::eof()))
                c = Traits::to_char_type(cb);
            else
                err |= (ios_base::eofbit | ios_base::failbit);
            if (err)
                in.setstate(err);
        }
        return in;
    }

    inline basic_istream<char>&
    operator>>(basic_istream<char>& in, unsigned char& c)
    { return (in >> reinterpret_cast<char&>(c)); }

    inline basic_istream<char>&
    operator>>(basic_istream<char>& in, signed char& c)
    { return (in >> reinterpret_cast<char&>(c)); }

    //
    // Character string extractors
    //

    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    operator>>(basic_istream<CharT, Traits>& in, CharT* s)
    {
        using char_type = CharT;
        using traits_type = Traits;

        using istream_type = basic_istream<char_type, traits_type>;
        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using int_type = typename traits_type::int_type;
        using ct = ctype<char_type>;

        std::streamsize extracted = 0;
        ios_base::iostate err = ios_base::goodbit;
        typename istream_type::sentry cerb(in, false);
        if (cerb) {
            // Figure out how many characters to extract.
            std::streamsize num = in.width();
            if (num <= 0)
                num = std::numeric_limits<std::streamsize>::max();

            const int_type eof = traits_type::eof();
            streambuf_type* sb = in.rdbuf();
            int_type c = sb->sgetc();

            while (extracted < num - 1 &&
                   !traits_type::eq_int_type(c, eof) &&
                   !ct::is_wsp(traits_type::to_char_type(c)))
            {
                *s++ = traits_type::to_char_type(c);
                ++extracted;
                c = sb->snextc();
            }
            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;

            // 68. Extractors for char* should store null at end
            *s = char_type();
            in.width(0);
        }
        if (!extracted)
            err |= ios_base::failbit;
        if (err)
            in.setstate(err);
        return in;
    }

    // Quick and easy way to eat whitespace
    template <class CharT, class Traits>
    inline basic_istream<CharT, Traits>&
    ws(basic_istream<CharT, Traits>& in)
    {
        using char_type = CharT;
        using traits_type = Traits;

        using streambuf_type = basic_streambuf<char_type, traits_type>;
        using int_type = typename traits_type::int_type;
        using ct = ctype<char_type>;

        const int_type eof = traits_type::eof();
        streambuf_type* sb = in.rdbuf();
        int_type c = sb->sgetc();

        while (ct::is_wsp(c))
            c = sb->snextc();

        if (traits_type::eq_int_type(c, eof))
            in.setstate(ios_base::eofbit);
        return in;
    }

    // Explicit specialization declarations
    inline basic_istream<char>&
    operator>>(basic_istream<char>& in, unsigned char* s)
    { return (in >> reinterpret_cast<char*>(s)); }

    inline basic_istream<char>&
    operator>>(basic_istream<char>& in, signed char* s)
    { return (in >> reinterpret_cast<char*>(s)); }

    //
    // Types from std
    //

    inline basic_istream<char>&
    operator>>(basic_istream<char>& in, std::basic_string<char>& str)
    {
        using istream_type = basic_istream<char>;
        using string_type = std::basic_string<char>;
        using ct = ctype<char>;

        using traits_type = typename istream_type::traits_type;
        using int_type = typename istream_type::int_type;
        using streambuf_type = typename istream_type::streambuf_type;
        using size_type = typename string_type::size_type;

        size_type extracted = 0;
        ios_base::iostate err = ios_base::goodbit;

        typename istream_type::sentry cerb(in, false);
        if (cerb) {
            str.erase();
            const std::streamsize w = in.width();
            const size_type n = w > 0 ? static_cast<size_type>(w)
                                      : str.max_size();
            const int_type eof = traits_type::eof();
            streambuf_type* sb = in.rdbuf();
            int_type c = sb->sgetc();

            while (extracted < n
                    && !traits_type::eq_int_type(c, eof)
                    && !ct::is_wsp(traits_type::to_char_type(c)))
            {
                std::streamsize size = std::min(
                    std::streamsize(sb->egptr()- sb->gptr()),
                    std::streamsize(n - extracted));
                if (size > 1) {
                    size = std::find_if(
                            sb->gptr() + 1, sb->gptr() + size, ct::is_wsp)
                        - sb->gptr();
                    str.append(sb->gptr(), size);
                    sb->gbump(size);
                    extracted += size;
                    c = sb->sgetc();
                }
                else {
                    str += traits_type::to_char_type(c);
                    ++extracted;
                    c = sb->snextc();
                }
            }

            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
            in.width(0);
        }
        if (!extracted)
            err |= ios_base::failbit;
        if (err)
            in.setstate(err);
        return in;
    }

    inline basic_istream<char>&
    getline(basic_istream<char>& in, std::basic_string<char>& str, char delim = '\n')
    {
        using istream_type = basic_istream<char>;
        using string_type = std::basic_string<char>;

        using traits_type = typename istream_type::traits_type;
        using int_type = typename istream_type::int_type;
        using streambuf_type = typename istream_type::streambuf_type;
        using size_type = typename string_type::size_type;

        size_type extracted = 0;
        const size_type n = str.max_size();
        ios_base::iostate err = ios_base::goodbit;

        typename istream_type::sentry cerb(in, true);
        if (cerb) {
            str.erase();
            const int_type idelim = traits_type::to_int_type(delim);
            const int_type eof = traits_type::eof();
            streambuf_type* sb = in.rdbuf();
            int_type c = sb->sgetc();

            while (extracted < n
                    && !traits_type::eq_int_type(c, eof)
                    && !traits_type::eq_int_type(c, idelim))
            {
                std::streamsize size = std::min(
                    std::streamsize(sb->egptr() - sb->gptr()),
                    std::streamsize(n - extracted));
                if (size > 1) {
                    const char* p = traits_type::find(
                        sb->gptr(), size, delim);
                    if (p)
                        size = p - sb->gptr();
                    str.append(sb->gptr(), size);
                    sb->gbump(size);
                    extracted += size;
                    c = sb->sgetc();
                }
                else {
                    str += traits_type::to_char_type(c);
                    ++extracted;
                    c = sb->snextc();
                }
            }

            if (traits_type::eq_int_type(c, eof))
                err |= ios_base::eofbit;
            else if (traits_type::eq_int_type(c, idelim)) {
                ++extracted;
                sb->sbumpc();
            }
            else
                err |= ios_base::failbit;
        }
        if (!extracted)
            err |= ios_base::failbit;
        if (err)
            in.setstate(err);
        return in;
    }

    //
    // Alias
    //

    using istream = basic_istream<char>;

} // namespace ard

