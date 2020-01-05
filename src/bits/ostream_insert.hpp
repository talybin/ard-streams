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
    template<typename CharT, typename Traits>
    inline void
    ostream_write(basic_ostream<CharT, Traits>& out,
		          const CharT* s, std::streamsize n)
    {
        using ostream_type = basic_ostream<CharT, Traits>;
        using ios_base = typename ostream_type::ios_base;

        const std::streamsize put = out.rdbuf()->sputn(s, n);
        if (put != n)
	        out.setstate(ios_base::badbit);
    }

    template<typename CharT, typename Traits>
    inline void
    ostream_fill(basic_ostream<CharT, Traits>& out, std::streamsize n)
    {
        using ostream_type = basic_ostream<CharT, Traits>;
        using ios_base = typename ostream_type::ios_base;

        const CharT c = out.fill();
        for (; n > 0; --n) {
	        const typename Traits::int_type put = out.rdbuf()->sputc(c);
	        if (Traits::eq_int_type(put, Traits::eof())) {
	            out.setstate(ios_base::badbit);
	            break;
	        }
	    }
    }

    template<typename CharT, typename Traits>
    inline basic_ostream<CharT, Traits>&
    ostream_insert(basic_ostream<CharT, Traits>& out,
		           const CharT* s, std::streamsize n)
    {
        using ostream_type = basic_ostream<CharT, Traits>;
        using ios_base = typename ostream_type::ios_base;

        typename ostream_type::sentry cerb(out);
        if (cerb) {
	        const std::streamsize w = out.width();
	        if (w > n) {
		        const bool left =
                    ((out.flags() & ios_base::adjustfield) == ios_base::left);
		        if (!left)
		            ostream_fill(out, w - n);
		        if (out.good())
		            ostream_write(out, s, n);
		        if (left && out.good())
		            ostream_fill(out, w - n);
		    }
	        else
		        ostream_write(out, s, n);
	        out.width(0);
	    }
        return out;
    }

} // namespace ard

