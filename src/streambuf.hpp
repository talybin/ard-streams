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
#include <utility>
#include <algorithm>
#include <bits/ios_base.hpp>

namespace ard
{
    // The actual work of input and output (interface)
    template <class CharT, class Traits = std::char_traits<CharT>>
    struct basic_streambuf
    {
        using char_type = CharT;
        using traits_type = Traits;
        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using streambuf_type = basic_streambuf<char_type, traits_type>;

    protected:
        char_type* in_beg_  = nullptr;  // Start of get area
        char_type* in_cur_  = nullptr;  // Current read area
        char_type* in_end_  = nullptr;  // End of get area
        char_type* out_beg_ = nullptr;  // Start of put area
        char_type* out_cur_ = nullptr;  // Current put area
        char_type* out_end_ = nullptr;  // End of put area

    public:
        // Destructor deallocates no buffer space
        virtual ~basic_streambuf()
        { }

        //
        // Buffer management and positioning
        //

        // Entry points for derived buffer functions
        basic_streambuf* pubsetbuf(char_type* s, std::streamsize n)
        { return this->setbuf(s, n); }

        // Alters the stream position
        pos_type pubseekoff(off_type offs, ios_base::seekdir way,
                            ios_base::openmode mode = ios_base::in | ios_base::out)
        { return this->seekoff(offs, way, mode); }

        // Alters the stream position
        pos_type pubseekpos(pos_type sp,
                            ios_base::openmode mode = ios_base::in | ios_base::out)
        { return this->seekpos(sp, mode); }

        // Calls virtual sync function
        int pubsync()
        { return this->sync(); }

        //
        // Get area
        //

        // Looking ahead into the stream
        std::streamsize in_avail()
        {
            const std::streamsize ret = this->egptr() - this->gptr();
            return ret ? ret : this->showmanyc();
        }

        // Getting the next character
        int_type snextc()
        {
            int_type ret = traits_type::eof();
            if (!traits_type::eq_int_type(this->sbumpc(), ret))
                ret = this->sgetc();
            return ret;
        }

        // Getting the next character
        int_type sbumpc()
        {
            int_type ret;
            if (this->gptr() < this->egptr()) {
                ret = traits_type::to_int_type(*this->gptr());
                this->gbump(1);
            }
            else
                ret = this->uflow();
            return ret;
        }

        // Getting the next character
        int_type sgetc()
        {
            return this->gptr() < this->egptr() ?
                traits_type::to_int_type(*this->gptr()) : this->underflow();
        }

        // Entry point for xsgetn
        std::streamsize sgetn(char_type* s, std::streamsize n)
        { return this->xsgetn(s, n); }

        //
        // Putback
        //

        // Pushing characters back into the input stream
        int_type sputbackc(char_type c)
        {
            const bool testpos = this->eback() < this->gptr();
            if (!testpos || !traits_type::eq(c, this->gptr()[-1])) {
                return this->pbackfail(traits_type::to_int_type(c));
            }
            this->gbump(-1);
            return traits_type::to_int_type(*this->gptr());
        }

        // Moving backwards in the input stream
        int_type sungetc()
        {
            if (this->eback() < this->gptr()) {
                this->gbump(-1);
                return traits_type::to_int_type(*this->gptr());
            }
            return this->pbackfail();
        }

        //
        // Put area
        //

        // Entry point for all single-character output functions
        int_type sputc(char_type c)
        {
            if (this->pptr() < this->epptr()) {
                *this->pptr() = c;
                this->pbump(1);
                return traits_type::to_int_type(c);
            }
            return this->overflow(traits_type::to_int_type(c));
        }

        // Entry point for all single-character output functions
        std::streamsize sputn(const char_type* s, std::streamsize n)
        { return this->xsputn(s, n); }

    protected:
        // Base constructor
        basic_streambuf() = default;

    public:
        //
        // Get area access
        //

        // Access to the get area
        // Returns the beginning pointer for the input sequence
        char_type* eback() const
        { return in_beg_; }

        // Returns the next pointer for the input sequence
        char_type* gptr() const
        { return in_cur_; }

        // Returns the end pointer for the input sequence
        char_type* egptr() const
        { return in_end_; }

        // Moving the read position
        void gbump(int n)
        { in_cur_ += n; }

        // Setting the three read area pointers
        void setg(char_type* gbeg, char_type* gnext, char_type* gend)
        {
            in_beg_ = gbeg;
            in_cur_ = gnext;
            in_end_ = gend;
        }

        //
        // Put area access
        //

        // Access to the put area
        // Returns the beginning pointer for the output sequence
        char_type* pbase() const
        { return out_beg_; }

        // Returns the next pointer for the output sequence
        char_type* pptr() const
        { return out_cur_; }

        // Returns the end pointer for the output sequence
        char_type* epptr() const
        { return out_end_; }

        // Moving the write position
        void pbump(int n)
        { out_cur_ += n; }

        // Setting the three write area pointers
        void setp(char_type* pbeg, char_type* pend)
        {
            out_beg_ = out_cur_ = pbeg;
            out_end_ = pend;
        }

        //
        // Buffer management and positioning
        //

        // Manipulates the buffer
        virtual streambuf_type* setbuf(char_type*, std::streamsize)
        { return this; }

        // Alters the stream positions
        virtual pos_type seekoff(off_type, ios_base::seekdir,
                                 ios_base::openmode = ios_base::in | ios_base::out)
        { return pos_type(off_type(-1)); }

        // Alters the stream positions
        virtual pos_type seekpos(pos_type,
                                 ios_base::openmode = ios_base::in | ios_base::out)
        { return pos_type(off_type(-1)); }

        // Synchronizes the buffer arrays with the controlled sequences
        virtual int sync()
        { return 0; }

        //
        // Get area
        //

        // Investigating the data available
        virtual std::streamsize showmanyc()
        { return 0; }

        // Multiple character extraction
        virtual std::streamsize xsgetn(char_type* s, std::streamsize n);

        // Fetches more data from the controlled sequence
        virtual int_type underflow()
        { return traits_type::eof(); }

        // Fetches more data from the controlled sequence
        virtual int_type uflow()
        {
            int_type ret = traits_type::eof();
            const bool testeof = traits_type::eq_int_type(this->underflow(), ret);
            if (!testeof) {
                ret = traits_type::to_int_type(*this->gptr());
                this->gbump(1);
            }
            return ret;
        }

        //
        // Putback
        //

        // Tries to back up the input sequence
        virtual int_type pbackfail(int_type c  = traits_type::eof())
        { return traits_type::eof(); }

        //
        // Put area
        //

        // Multiple character insertion
        virtual std::streamsize xsputn(const char_type* s, std::streamsize n);

        // Consumes data from the buffer; writes to the controlled sequence
        virtual int_type overflow(int_type c  = traits_type::eof())
        { return traits_type::eof(); }


        basic_streambuf(const basic_streambuf&) = default;

        basic_streambuf& operator=(const basic_streambuf&) = default;

        void swap(basic_streambuf& sb)
        {
            std::swap(in_beg_, sb.in_beg_);
            std::swap(in_cur_, sb.in_cur_);
            std::swap(in_end_, sb.in_end_);
            std::swap(out_beg_, sb.out_beg_);
            std::swap(out_cur_, sb.out_cur_);
            std::swap(out_end_, sb.out_end_);
        }
    };


    template <class CharT, class Traits>
    inline std::streamsize basic_streambuf<CharT, Traits>::
    xsgetn(char_type* s, std::streamsize n)
    {
        std::streamsize ret = 0;
        while (ret < n) {
            const std::streamsize buf_len = this->egptr() - this->gptr();
            if (buf_len) {
                const std::streamsize remaining = n - ret;
                const std::streamsize len = std::min(buf_len, remaining);
                traits_type::copy(s, this->gptr(), len);
                ret += len;
                s += len;
                this->gbump(len);
            }
            if (ret < n) {
                const int_type c = this->uflow();
                if (!traits_type::eq_int_type(c, traits_type::eof())) {
                    traits_type::assign(*s++, traits_type::to_char_type(c));
                    ++ret;
                }
                else break;
            }
        }
        return ret;
    }

    template <class CharT, class Traits>
    inline std::streamsize basic_streambuf<CharT, Traits>::
    xsputn(const char_type* s, std::streamsize n)
    {
        std::streamsize ret = 0;
        while (ret < n) {
            const std::streamsize buf_len = this->epptr() - this->pptr();
             if (buf_len) {
                const std::streamsize remaining = n - ret;
                const std::streamsize len = std::min(buf_len, remaining);
                traits_type::copy(this->pptr(), s, len);
                ret += len;
                s += len;
                this->pbump(len);
            }
            if (ret < n) {
                int_type c = this->overflow(traits_type::to_int_type(*s));
                if (!traits_type::eq_int_type(c, traits_type::eof())) {
                    ++ret;
                    ++s;
                }
                else break;
            }
        }
        return ret;
    }

    // Conceivably, this could be used to implement buffer-to-buffer
    // copies, if this was ever desired in an un-ambiguous way by the
    // standard.
    template <class CharT, class Traits>
    inline std::streamsize
    copy_streambufs_eof(basic_streambuf<CharT, Traits>* sbin,
                        basic_streambuf<CharT, Traits>* sbout,
                        bool& ineof)
    {
        using traits_type = Traits;
        using int_type = typename traits_type::int_type;

        std::streamsize ret = 0;
        ineof = true;

        int_type c = sbin->sgetc();
        while (!traits_type::eq_int_type(c, traits_type::eof())) {
            c = sbout->sputc(traits_type::to_char_type(c));
            if (traits_type::eq_int_type(c, traits_type::eof())) {
                ineof = false;
                break;
            }
            ++ret;
            c = sbin->snextc();
        }
        return ret;
    }

    template <class CharT, class Traits>
    inline std::streamsize
    copy_streambufs(basic_streambuf<CharT, Traits>* sbin,
                    basic_streambuf<CharT, Traits>* sbout)
    {
        bool ineof;
        return copy_streambufs_eof(sbin, sbout, ineof);
    }


    template <>
    inline std::streamsize
    copy_streambufs_eof(basic_streambuf<char>* sbin,
                        basic_streambuf<char>* sbout, bool& ineof)
    {
        using traits_type = basic_streambuf<char>::traits_type;

        std::streamsize ret = 0;
        ineof = true;
        traits_type::int_type c = sbin->sgetc();
        while (!traits_type::eq_int_type(c, traits_type::eof())) {
            const std::streamsize n = sbin->egptr() - sbin->gptr();
            if (n > 1) {
                const std::streamsize wrote = sbout->sputn(sbin->gptr(), n);
                sbin->gbump(wrote);
                ret += wrote;
                if (wrote < n) {
                    ineof = false;
                    break;
                }
                c = sbin->underflow();
            }
            else {
                c = sbout->sputc(traits_type::to_char_type(c));
                if (traits_type::eq_int_type(c, traits_type::eof())) {
                    ineof = false;
                    break;
                }
                ++ret;
                c = sbin->snextc();
            }
        }
        return ret;
    }

} // namespace ard

