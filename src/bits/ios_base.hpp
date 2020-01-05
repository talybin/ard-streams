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
#include <climits>
#include <cstdio>

//
// ISO C++ 14882: 27.4 Iostreams base classes
//

namespace ard
{
    // The following definitions of bitmask types are enums, not ints,
    // as permitted (but not required) in the standard, in order to provide
    // better type safety in iostream calls. A side effect is that in C++98
    // expressions involving them are not compile-time constants.
    enum _ios_fmtflags
    {
        _e_boolalpha    = 1L << 0,
        _e_dec          = 1L << 1,
        _e_fixed        = 1L << 2,
        _e_hex          = 1L << 3,
        _e_internal     = 1L << 4,
        _e_left         = 1L << 5,
        _e_oct          = 1L << 6,
        _e_right        = 1L << 7,
        _e_scientific   = 1L << 8,
        _e_showbase     = 1L << 9,
        _e_showpoint    = 1L << 10,
        _e_showpos      = 1L << 11,
        _e_skipws       = 1L << 12,
        _e_unitbuf      = 1L << 13,
        _e_uppercase    = 1L << 14,
        _e_adjustfield  = _e_left | _e_right | _e_internal,
        _e_basefield    = _e_dec | _e_oct | _e_hex,
        _e_floatfield   = _e_scientific | _e_fixed,
        _e_ios_fmtflags_end = 1L << 16,
        _e_ios_fmtflags_max = INT_MAX,
        _e_ios_fmtflags_min = ~INT_MAX
    };

    inline constexpr _ios_fmtflags
    operator&(_ios_fmtflags a, _ios_fmtflags b)
    { return _ios_fmtflags(static_cast<int>(a) & static_cast<int>(b)); }

    inline constexpr _ios_fmtflags
    operator|(_ios_fmtflags a, _ios_fmtflags b)
    { return _ios_fmtflags(static_cast<int>(a) | static_cast<int>(b)); }

    inline constexpr _ios_fmtflags
    operator^(_ios_fmtflags a, _ios_fmtflags b)
    { return _ios_fmtflags(static_cast<int>(a) ^ static_cast<int>(b)); }

    inline constexpr _ios_fmtflags
    operator~(_ios_fmtflags a)
    { return _ios_fmtflags(~static_cast<int>(a)); }

    inline const _ios_fmtflags&
    operator|=(_ios_fmtflags& a, _ios_fmtflags b)
    { return a = a | b; }

    inline const _ios_fmtflags&
    operator&=(_ios_fmtflags& a, _ios_fmtflags b)
    { return a = a & b; }

    inline const _ios_fmtflags&
    operator^=(_ios_fmtflags& a, _ios_fmtflags b)
    { return a = a ^ b; }


    enum _ios_openmode
    {
        _e_app          = 1L << 0,
        _e_ate          = 1L << 1,
        _e_bin          = 1L << 2,
        _e_in           = 1L << 3,
        _e_out          = 1L << 4,
        _e_trunc        = 1L << 5,
        _e_ios_openmode_end = 1L << 16,
        _e_ios_openmode_max = INT_MAX,
        _e_ios_openmode_min = ~INT_MAX
    };

    inline constexpr _ios_openmode
    operator&(_ios_openmode a, _ios_openmode b)
    { return _ios_openmode(static_cast<int>(a) & static_cast<int>(b)); }

    inline constexpr _ios_openmode
    operator|(_ios_openmode a, _ios_openmode b)
    { return _ios_openmode(static_cast<int>(a) | static_cast<int>(b)); }

    inline constexpr _ios_openmode
    operator^(_ios_openmode a, _ios_openmode b)
    { return _ios_openmode(static_cast<int>(a) ^ static_cast<int>(b)); }

    inline constexpr _ios_openmode
    operator~(_ios_openmode a)
    { return _ios_openmode(~static_cast<int>(a)); }

    inline const _ios_openmode&
    operator|=(_ios_openmode& a, _ios_openmode b)
    { return a = a | b; }

    inline const _ios_openmode&
    operator&=(_ios_openmode& a, _ios_openmode b)
    { return a = a & b; }

    inline const _ios_openmode&
    operator^=(_ios_openmode& a, _ios_openmode b)
    { return a = a ^ b; }


    enum _ios_iostate
    {
        _e_goodbit          = 0,
        _e_badbit           = 1L << 0,
        _e_eofbit           = 1L << 1,
        _e_failbit          = 1L << 2,
        _e_ios_iostate_end  = 1L << 16,
        _e_ios_iostate_max  = INT_MAX,
        _e_ios_iostate_min  = ~INT_MAX
    };

    inline constexpr _ios_iostate
    operator&(_ios_iostate a, _ios_iostate b)
    { return _ios_iostate(static_cast<int>(a) & static_cast<int>(b)); }

    inline constexpr _ios_iostate
    operator|(_ios_iostate a, _ios_iostate b)
    { return _ios_iostate(static_cast<int>(a) | static_cast<int>(b)); }

    inline constexpr _ios_iostate
    operator^(_ios_iostate a, _ios_iostate b)
    { return _ios_iostate(static_cast<int>(a) ^ static_cast<int>(b)); }

    inline constexpr _ios_iostate
    operator~(_ios_iostate a)
    { return _ios_iostate(~static_cast<int>(a)); }

    inline const _ios_iostate&
    operator|=(_ios_iostate& a, _ios_iostate b)
    { return a = a | b; }

    inline const _ios_iostate&
    operator&=(_ios_iostate& a, _ios_iostate b)
    { return a = a & b; }

    inline const _ios_iostate&
    operator^=(_ios_iostate& a, _ios_iostate b)
    { return a = a ^ b; }


    enum _ios_seekdir
    {
        _e_beg  = 0,
        _e_cur  = SEEK_CUR,
        _e_end  = SEEK_END,
        _e_ios_seekdir_end = 1L << 16
    };


    // 27.4.2 Class ios_base
    // The base of the I/O class hierarchy.
    //
    // This class defines everything that can be defined about I/O that does
    // not depend on the type of characters being input or output. Most
    // people will only see ios_base when they need to specify the full name
    // of the various I/O flags (e.g., the openmodes).
    //
    struct ios_base
    {
        // 27.4.2.1.2 Type ios_base::fmtflags
        // This is a bitmask type.
        //
        // _ios_fmtflags is implementation-defined, but it is valid to
        // perform bitwise operations on these values and expect the Right
        // Thing to happen. Defined objects of type fmtflags are:
        // - boolalpha
        // - dec
        // - fixed
        // - hex
        // - internal
        // - left
        // - oct
        // - right
        // - scientific
        // - showbase
        // - showpoint
        // - showpos
        // - skipws
        // - unitbuf
        // - uppercase
        // - adjustfield
        // - basefield
        // - floatfield
        //
        using fmtflags = _ios_fmtflags;

        // Insert/extract bool in alphabetic rather than numeric format.
        static const fmtflags boolalpha = fmtflags::_e_boolalpha;

        // Converts integer input or generates integer output in decimal base.
        static const fmtflags dec = fmtflags::_e_dec;

        // Generate floating-point output in fixed-point notation.
        static const fmtflags fixed = fmtflags::_e_fixed;

        // Converts integer input or generates integer output in hexadecimal base.
        static const fmtflags hex = fmtflags::_e_hex;

        // Adds fill characters at a designated internal point in certain
        // generated output, or identical to right if no such point is
        // designated.
        static const fmtflags internal = fmtflags::_e_internal;

        // Adds fill characters on the right (final positions) of certain
        // generated output (i.e., the thing you print is flush left.)
        static const fmtflags left = fmtflags::_e_left;

        // Converts integer input or generates integer output in octal base.
        static const fmtflags oct = fmtflags::_e_oct;

        // Adds fill characters on the left (initial positions) of certain
        // generated output (i.e., the thing you print is flush right.)
        static const fmtflags right = fmtflags::_e_right;

        // Generates floating-point output in scientific notation.
        static const fmtflags scientific = fmtflags::_e_scientific;

        // Generates a prefix indicating the numeric base of generated integer
        // output.
        static const fmtflags showbase = fmtflags::_e_showbase;

        // Generates a decimal-point character unconditionally in generated
        // floating-point output.
        static const fmtflags showpoint = fmtflags::_e_showpoint;

        // Generates a + sign in non-negative generated numeric output.
        static const fmtflags showpos = fmtflags::_e_showpos;

        // Skips leading white space before certain input operations.
        static const fmtflags skipws = fmtflags::_e_skipws;

        // Flushes output after each output operation.
        static const fmtflags unitbuf = fmtflags::_e_unitbuf;

        // Replaces certain lowercase letters with their uppercase equivalents
        // in generated output.
        static const fmtflags uppercase = fmtflags::_e_uppercase;

        // A mask of left|right|internal. Useful for the 2-arg form of setf.
        static const fmtflags adjustfield = fmtflags::_e_adjustfield;

        // A mask of dec|oct|hex. Useful for the 2-arg form of setf.
        static const fmtflags basefield = fmtflags::_e_basefield;

        // A mask of scientific|fixed. Useful for the 2-arg form of setf.
        static const fmtflags floatfield = fmtflags::_e_floatfield;

        // 27.4.2.1.3 Type ios_base::iostate
        // This is a bitmask type.
        //
        // _ios_iostate is implementation-defined, but it is valid to
        // perform bitwise operations on these values and expect the Right
        // Thing to happen. Defined objects of type iostate are:
        // - badbit
        // - eofbit
        // - failbit
        // - goodbit
        //
        using iostate = _ios_iostate;

        // Indicates a loss of integrity in an input or output sequence (such
        // as an irrecoverable read error from a file).
        static const iostate badbit = iostate::_e_badbit;

        // Indicates that an input operation reached the end of an input sequence.
        static const iostate eofbit = iostate::_e_eofbit;

        // Indicates that an input operation failed to read the expected
        // characters, or that an output operation failed to generate the
        // desired characters.
        static const iostate failbit = iostate::_e_failbit;

        // Indicates all is well.
        static const iostate goodbit = iostate::_e_goodbit;

        // 27.4.2.1.4 Type ios_base::openmode
        // This is a bitmask type.
        //
        // _ios_openmode is implementation-defined, but it is valid to
        // perform bitwise operations on these values and expect the Right
        // Thing to happen. Defined objects of type openmode are:
        // - app
        // - ate
        // - binary
        // - in
        // - out
        // - trunc
        //
        using openmode = _ios_openmode;

        // Seek to end before each write.
        static const openmode app = openmode::_e_app;

        // Open and seek to end immediately after opening.
        static const openmode ate = openmode::_e_ate;

        // Perform input and output in binary mode (as opposed to text mode).
        // This is probably not what you think it is; see
        // https://gcc.gnu.org/onlinedocs/libstdc++/manual/fstreams.html#std.io.filestreams.binary
        static const openmode binary = openmode::_e_bin;

        // Open for input. Default for ifstream and fstream.
        static const openmode in = openmode::_e_in;

        // Open for output. Default for ofstream and fstream.
        static const openmode out = openmode::_e_out;

        // Open for input. Default for ofstream.
        static const openmode trunc = openmode::_e_trunc;

        // 27.4.2.1.5 Type ios_base::seekdir
        // This is an enumerated type.
        //
        // _ios_seekdir is implementation-defined. Defined values
        // of type seekdir are:
        // - beg
        // - cur, equivalent to SEEK_CUR in the C standard library.
        // - end, equivalent to SEEK_END in the C standard library.
        //
        using seekdir = _ios_seekdir;

        // Request a seek relative to the beginning of the stream.
        static const seekdir beg = seekdir::_e_beg;

        // Request a seek relative to the current position within the sequence.
        static const seekdir cur = seekdir::_e_cur;

        // Request a seek relative to the current end of the sequence.
        static const seekdir end = seekdir::_e_end;

    protected:
        std::streamsize precision_ = 6;
        std::streamsize width_ = 0;
        fmtflags flags_ = fmtflags::_e_dec | fmtflags::_e_skipws;
        iostate streambuf_state_ = iostate::_e_goodbit;

    public:
        // [27.4.2.2] fmtflags state functions

        // Access to format flags.
        // Return the format control flags for both input and output.
        fmtflags flags() const
        { return flags_; }

        // Setting new format flags all at once.
        // Param f - The new flags to set.
        // Return the previous format control flags.
        //
        // This function overwrites all the format flags with f.
        //
        fmtflags flags(fmtflags f)
        {
            fmtflags old = flags_;
            flags_ = f;
            return old;
        }

        // Setting new format flags.
        // Param f - Additional flags to set.
        // Return the previous format control flags.
        //
        // This function sets additional flags in format control. Flags that
        // were previously set remain set.
        //
        fmtflags setf(fmtflags f)
        {
            fmtflags old = flags_;
            flags_ |= f;
            return old;
        }

        // Setting new format flags.
        // Param f - Additional flags to set.
        // Param mask - The flags mask for f.
        // Return the previous format control flags.
        //
        // This function clears mask in the format flags, then sets
        // f & mask. An example mask isios_base::adjustfield.
        //
        fmtflags setf(fmtflags f, fmtflags mask)
        {
            fmtflags old = flags_;
            flags_ &= ~mask;
            flags_ |= (f & mask);
            return old;
        }

        // Clearing format flags.
        // Param mask - The flags to unset.
        //
        // This function clears mask in the format flags.
        //
        void unsetf(fmtflags mask)
        { flags_ &= ~mask; }

        // Flags access.
        // Return the precision to generate on certain output operations.
        //
        // Be careful if you try to give a definition of precision here; see
        // DR 189.
        //
        std::streamsize precision() const
        { return precision_; }

        // Changing flags.
        // Param prec - The new precision value.
        // Return the previous value of precision().
        std::streamsize precision(std::streamsize prec)
        {
            std::streamsize old = precision_;
            precision_ = prec;
            return old;
        }

        // Flags access.
        // Return the minimum field width to generate on output operations.
        //
        // Minimum field width refers to the number of characters.
        //
        std::streamsize width() const
        { return width_; }

        // Changing flags.
        // Param wide - The new width value.
        // Return the previous value of width().
        std::streamsize width(std::streamsize wide)
        {
            std::streamsize old = width_;
            width_ = wide;
            return old;
        }

        // Destructor
        virtual ~ios_base()
        { }

        // There is no callbacks here, so it should be allowed
        // to copy
        ios_base(const ios_base&) = delete;
        ios_base& operator=(const ios_base&) = delete;

    protected:
        ios_base() { }

        // To be done
        void move_(ios_base&) noexcept { };
        void swap_(ios_base& rhs) noexcept { };
    };

    //
    // [27.4.5.1] fmtflags manipulators
    //

    // Calls base.setf(ios_base::boolalpha)
    inline ios_base& boolalpha(ios_base& base)
    {
        base.setf(ios_base::boolalpha);
        return base;
    }

    // Calls base.unsetf(ios_base::boolalpha)
    inline ios_base& noboolalpha(ios_base& base)
    {
        base.unsetf(ios_base::boolalpha);
        return base;
    }

    // Calls base.setf(ios_base::showbase)
    inline ios_base& showbase(ios_base& base)
    {
        base.setf(ios_base::showbase);
        return base;
    }

    // Calls base.unsetf(ios_base::showbase)
    inline ios_base& noshowbase(ios_base& base)
    {
        base.unsetf(ios_base::showbase);
        return base;
    }

    // Calls base.setf(ios_base::showpoint)
    inline ios_base& showpoint(ios_base& base)
    {
        base.setf(ios_base::showpoint);
        return base;
    }

    // Calls base.unsetf(ios_base::showpoint)
    inline ios_base& noshowpoint(ios_base& base)
    {
        base.unsetf(ios_base::showpoint);
        return base;
    }

    // Calls base.setf(ios_base::showpos)
    inline ios_base& showpos(ios_base& base)
    {
        base.setf(ios_base::showpos);
        return base;
    }

    // Calls base.unsetf(ios_base::showpos)
    inline ios_base& noshowpos(ios_base& base)
    {
        base.unsetf(ios_base::showpos);
        return base;
    }

    // Calls base.setf(ios_base::skipws)
    inline ios_base& skipws(ios_base& base)
    {
        base.setf(ios_base::skipws);
        return base;
    }

    // Calls base.unsetf(ios_base::skipws)
    inline ios_base& noskipws(ios_base& base)
    {
        base.unsetf(ios_base::skipws);
        return base;
    }

    // Calls base.setf(ios_base::uppercase)
    inline ios_base& uppercase(ios_base& base)
    {
        base.setf(ios_base::uppercase);
        return base;
    }

    // Calls base.unsetf(ios_base::uppercase)
    inline ios_base& nouppercase(ios_base& base)
    {
        base.unsetf(ios_base::uppercase);
        return base;
    }

    // Calls base.setf(ios_base::unitbuf)
    inline ios_base& unitbuf(ios_base& base)
    {
        base.setf(ios_base::unitbuf);
        return base;
    }

    // Calls base.unsetf(ios_base::unitbuf)
    inline ios_base& nounitbuf(ios_base& base)
    {
        base.unsetf(ios_base::unitbuf);
        return base;
    }

    //
    // [27.4.5.2] adjustfield manipulators
    //

    // Calls base.setf(ios_base::internal, ios_base::adjustfield)
    inline ios_base& internal(ios_base& base)
    {
        base.setf(ios_base::internal, ios_base::adjustfield);
        return base;
    }

    // Calls base.setf(ios_base::left, ios_base::adjustfield)
    inline ios_base& left(ios_base& base)
    {
        base.setf(ios_base::left, ios_base::adjustfield);
        return base;
    }

    // Calls base.setf(ios_base::right, ios_base::adjustfield)
    inline ios_base& right(ios_base& base)
    {
        base.setf(ios_base::right, ios_base::adjustfield);
        return base;
    }

    //
    // [27.4.5.3] basefield manipulators
    //

    // Calls base.setf(ios_base::dec, ios_base::basefield)
    inline ios_base& dec(ios_base& base)
    {
        base.setf(ios_base::dec, ios_base::basefield);
        return base;
    }

    // Calls base.setf(ios_base::hex, ios_base::basefield)
    inline ios_base& hex(ios_base& base)
    {
        base.setf(ios_base::hex, ios_base::basefield);
        return base;
    }

    // Calls base.setf(ios_base::oct, ios_base::basefield)
    inline ios_base& oct(ios_base& base)
    {
        base.setf(ios_base::oct, ios_base::basefield);
        return base;
    }

    //
    // [27.4.5.4] floatfield manipulators
    //

    // Calls base.setf(ios_base::fixed, ios_base::floatfield)
    inline ios_base& fixed(ios_base& base)
    {
        base.setf(ios_base::fixed, ios_base::floatfield);
        return base;
    }

    // Calls base.setf(ios_base::scientific, ios_base::floatfield)
    inline ios_base& scientific(ios_base& base)
    {
        base.setf(ios_base::scientific, ios_base::floatfield);
        return base;
    }

    // Calls base.setf(ios_base::fixed|ios_base::scientific, ios_base::floatfield)
    inline ios_base& hexfloat(ios_base& base)
    {
        base.setf(ios_base::fixed | ios_base::scientific, ios_base::floatfield);
        return base;
    }

    // Calls base.unsetf(ios_base::floatfield)
    inline ios_base& defaultfloat(ios_base& base)
    {
        base.unsetf(ios_base::floatfield);
        return base;
    }

} // namespace ard

