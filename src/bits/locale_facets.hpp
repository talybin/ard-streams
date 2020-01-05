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

// Locale is not supported by this implementation.
// Extracting required minimum.
namespace ard
{
    // Fake locale ctype class
    template <class CharT>
    struct ctype;

    template <>
    struct ctype<char>
    {
        static char widen(char c)
        { return c; }

        static bool is_wsp(char c)
        { return std::isspace(c); }

        static const char* truename()
        { return "true"; }

        static const char* falsename()
        { return "false"; }

        static int digit(char c)
        {
            if (c >= '0' && c <= '9')
                return c - '0';
            if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
            if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
            return -1;
        }
    };


    // Primary class template num_get
    template <class CharT, class InIter>
    struct num_get
    {
        using char_type = CharT;
        using iter_type = InIter;

        // Constructor performs initialization
        explicit num_get(size_t = 0)
        { }

        // Destructor
        virtual ~num_get()
        { }

        // Parses the input stream into the bool
        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, bool& v) const
        { return this->do_get(in, end, io, err, v); }

        // Parses the input stream into the integral variable

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, long& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, unsigned short& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, unsigned int& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, unsigned long& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, long long& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, unsigned long long& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, float& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, double& v) const
        { return this->do_get(in, end, io, err, v); }

        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, long double& v) const
        { return this->do_get(in, end, io, err, v); }

        // Parses the input stream into the pointer variable
        iter_type get(iter_type in, iter_type end, ios_base& io,
                      ios_base::iostate& err, void*& v) const
        { return this->do_get(in, end, io, err, v); }

    protected:
        iter_type extract_float_(
            iter_type, iter_type, ios_base&, ios_base::iostate&, std::string&) const;

        template <class ValueT>
        iter_type extract_int_(
            iter_type, iter_type, ios_base&, ios_base::iostate&, ValueT&) const;

        // Numeric parsing

        virtual iter_type
        do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, bool&) const;

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, long& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, unsigned short& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, unsigned int& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, unsigned long& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, long long& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type beg, iter_type end, ios_base& io,
               ios_base::iostate& err, unsigned long long& v) const
        { return extract_int_(beg, end, io, err, v); }

        virtual iter_type
        do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, float&) const;

        virtual iter_type
        do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, double&) const;

        virtual iter_type
        do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, long double&) const;

        virtual iter_type
        do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, void*&) const;
    };


    // Primary class template num_put
    template <class CharT, class OutIter>
    struct num_put
    {
        using char_type = CharT;
        using iter_type = OutIter;

        // Constructor performs initialization
        explicit num_put(size_t = 0)
        { }

        // Destructor
        virtual ~num_put()
        { };

        // Formats the boolean and inserts it into a stream
        iter_type put(iter_type s, ios_base& io, char_type fill, bool v) const
        { return this->do_put(s, io, fill, v); }

        // Format the integral value and insert it into a stream

        iter_type put(iter_type s, ios_base& io, char_type fill, long v) const
        { return this->do_put(s, io, fill, v); }

        iter_type put(iter_type s, ios_base& io, char_type fill, unsigned long v) const
        { return this->do_put(s, io, fill, v); }

        iter_type put(iter_type s, ios_base& io, char_type fill, long long v) const
        { return this->do_put(s, io, fill, v); }

        iter_type put(iter_type s, ios_base& io, char_type fill, unsigned long long v) const
        { return this->do_put(s, io, fill, v); }

        // Format the floating point value and insert it into a stream

        iter_type put(iter_type s, ios_base& io, char_type fill, double v) const
        { return this->do_put(s, io, fill, v); }

        iter_type put(iter_type s, ios_base& io, char_type fill, long double v) const
        { return this->do_put(s, io, fill, v); }

        // Formats the pointer value and inserts it into a stream
        iter_type put(iter_type s, ios_base& io, char_type fill, const void* v) const
        { return this->do_put(s, io, fill, v); }

    protected:
        template <class ValueT>
        iter_type insert_float_(
            iter_type, ios_base& io, char_type fill, char mod, ValueT v) const;

        template <class ValueT>
        iter_type insert_int_(
            iter_type, ios_base& io, char_type fill, ValueT v) const;

        void pad_(char_type fill, std::streamsize w, ios_base& io,
                  char_type* n, const char_type* cs, int& len) const;

        // These functions do the work of formatting numeric values and
        // inserting them into a stream. This function is a hook for derived
        // classes to change the value returned

        virtual iter_type
        do_put(iter_type s, ios_base& io, char_type fill, bool v) const;

        virtual iter_type
        do_put(iter_type s, ios_base& io, char_type fill, long v) const
        { return insert_int_(s, io, fill, v); }

        virtual iter_type
        do_put(iter_type s, ios_base& io, char_type fill, unsigned long v) const
        { return insert_int_(s, io, fill, v); }

        virtual iter_type
        do_put(iter_type s, ios_base& io, char_type fill, long long v) const
        { return insert_int_(s, io, fill, v); }

        virtual iter_type
        do_put(iter_type s, ios_base& io, char_type fill, unsigned long long v) const
        { return insert_int_(s, io, fill, v); }

        virtual iter_type
        do_put(iter_type, ios_base&, char_type, double) const;

        virtual iter_type
        do_put(iter_type, ios_base&, char_type, long double) const;

        virtual iter_type
        do_put(iter_type, ios_base&, char_type, const void*) const;
    };


    //
    // Methods of num_get
    //

    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    extract_float_(InIter beg, InIter end, ios_base& io,
                   ios_base::iostate& err, std::string& xtrc) const
    {
        using ct = ctype<CharT>;

        char_type c = char_type();
        char_type dec_point = ct::widen('.');
        char_type zero = ct::widen('0');
        char_type plus = ct::widen('+');
        char_type minus = ct::widen('-');

        // True if beg becomes equal to end
        bool testeof = beg == end;

        // First check for sign
        if (!testeof) {
            c = *beg;
            const bool is_plus = c == plus;
            if (is_plus || c == minus) {
                xtrc += is_plus ? '+' : '-';
                if (++beg != end)
                    c = *beg;
                else
                    testeof = true;
            }
        }

        // Next, look for leading zeros
        bool found_mantissa = false;
        int sep_pos = 0;
        while (!testeof && c == zero) {
            if (!found_mantissa) {
                xtrc += '0';
                found_mantissa = true;
            }
            ++sep_pos;

            if (++beg != end)
                c = *beg;
            else
                testeof = true;
        }

        // Only need acceptable digits for floating point numbers
        bool found_dec = false;
        bool found_sci = false;

        while (!testeof) {
            const int digit = ct::digit(c);
            if (digit >= 0 && digit <= 9) {
                xtrc += '0' + digit;
                found_mantissa = true;
            }
            else if (c == dec_point && !found_dec && !found_sci) {
                xtrc += '.';
                found_dec = true;
            }
            else if ((c == ct::widen('e') || c == ct::widen('E')) &&
                    !found_sci && found_mantissa)
            {
                // Scientific notation
                xtrc += 'e';
                found_sci = true;

                // Remove optional plus or minus sign, if they exist
                if (++beg != end) {
                    c = *beg;
                    const bool is_plus = c == plus;
                    if (is_plus || c == minus)
                        xtrc += is_plus ? '+' : '-';
                    else
                        continue;
                }
                else {
                    testeof = true;
                    break;
                }
            }
            else break;

            if (++beg != end)
                c = *beg;
            else
                testeof = true;
        }
        return beg;
    }

    template <class CharT, class InIter>
    template <class ValueT>
    inline InIter num_get<CharT, InIter>::
    extract_int_(InIter beg, InIter end, ios_base& io,
                   ios_base::iostate& err, ValueT& v) const
    {
        using unsigned_type = typename std::make_unsigned<ValueT>::type;
        using ct = ctype<CharT>;

        char_type c = char_type();
        char_type zero = ct::widen('0');
        char_type plus = ct::widen('+');
        char_type minus = ct::widen('-');

        // NB: Iff basefield == 0, base can change based on contents
        const ios_base::fmtflags basefield = io.flags() & ios_base::basefield;
        const bool oct = basefield == ios_base::oct;
        int base = oct ? 8 : (basefield == ios_base::hex ? 16 : 10);

        // True if beg becomes equal to end.
        bool testeof = beg == end;

        // First check for sign.
        bool negative = false;
        if (!testeof) {
            c = *beg;
            negative = c == minus;
            if (negative || c == plus) {
                if (++beg != end)
                    c = *beg;
                else
                    testeof = true;
            }
        }

        // Next, look for leading zeros and check required digits
        // for base formats.
        bool found_zero = false;
        int sep_pos = 0;
        while (!testeof) {
            if (c == zero && (!found_zero || base == 10)) {
                found_zero = true;
                ++sep_pos;
                if (int(basefield) == 0)
                    base = 8;
                if (base == 8)
                    sep_pos = 0;
            }
            else if (found_zero && (
                    c == ct::widen('x') || c == ct::widen('X')))
            {
                if (int(basefield) == 0)
                    base = 16;
                if (base == 16) {
                    found_zero = false;
                    sep_pos = 0;
                }
                else break;
            }
            else break;

            if (++beg != end) {
                c = *beg;
                if (!found_zero)
                    break;
            }
            else
                testeof = true;
        }

        // At this point, base is determined. Extract.
        const unsigned_type max = (negative && std::is_signed<ValueT>::value)
            ? -std::numeric_limits<ValueT>::min() : std::numeric_limits<ValueT>::max();
        const unsigned_type smax = max / base;
        unsigned_type result = 0;
        bool testoverflow = false;

        while (!testeof) {
            const int digit = ct::digit(c);
            // Only allow base digits as valid input
            if (digit < 0 || digit >= base)
                break;

            if (result > smax)
                testoverflow = true;
            else {
                result *= base;
                testoverflow |= result > (max - digit);
                result += digit;
                ++sep_pos;
            }

            if (++beg != end)
                c = *beg;
            else
                testeof = true;
        }

        // 23. Num_get overflow result.
        if (!sep_pos && !found_zero) {
            v = 0;
            err = ios_base::failbit;
        }
        else if (testoverflow) {
            if (negative && std::is_signed<ValueT>::value)
                v = std::numeric_limits<ValueT>::min();
            else
                v = std::numeric_limits<ValueT>::max();
            err = ios_base::failbit;
        }
        else
            v = negative ? -result : result;

        if (testeof)
            err |= ios_base::eofbit;
        return beg;
    }

    // Bad bool parsing
    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    do_get(iter_type beg, iter_type end, ios_base& io,
           ios_base::iostate& err, bool& v) const
    {
        using char_type = CharT;
        using ct = ctype<char_type>;

        if (!(io.flags() & ios_base::boolalpha)) {
            // Parse bool values as long
            // NB: We can't just call do_get(long) here, as it might
            // refer to a derived class.
            long l = -1;
            beg = extract_int_(beg, end, io, err, l);
            if (l == 0 || l == 1)
                v = bool(l);
            else {
                // Num_get overflow result
                v = true;
                err = ios_base::failbit;
                if (beg == end)
                    err |= ios_base::eofbit;
            }
        }
        else {
            // Parse bool values as alphanumeric
            const char_type* tname = ct::truename();
            const char_type* fname = ct::falsename();
            const char_type* name = nullptr;

            bool testeof = false;
            while (true) {
                testeof = beg == end;
                if (testeof)
                    break;
                const char_type c = *beg;
                if (!name) {
                    v = c == *tname;
                    if (v)
                        name = tname;
                    else if (c == *fname)
                        name = fname;
                    else break;
                }
                else if (!*name || c != *name)
                    break;
                ++name;
                ++beg;
            }

            if (!name || *name)
                err = ios_base::failbit;
            if (testeof)
                err |= ios_base::eofbit;
        }
        return beg;
    }

    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    do_get(iter_type beg, iter_type end, ios_base& io,
           ios_base::iostate& err, float& v) const
    {
        std::string xtrc;
        xtrc.reserve(32);
        beg = extract_float_(beg, end, io, err, xtrc);
        v = strtof(xtrc.c_str(), nullptr);
        if (beg == end)
            err |= ios_base::eofbit;
        return beg;
    }

    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    do_get(iter_type beg, iter_type end, ios_base& io,
           ios_base::iostate& err, double& v) const
    {
        std::string xtrc;
        xtrc.reserve(32);
        beg = extract_float_(beg, end, io, err, xtrc);
        v = strtod(xtrc.c_str(), nullptr);
        if (beg == end)
            err |= ios_base::eofbit;
        return beg;
    }

    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    do_get(iter_type beg, iter_type end, ios_base& io,
           ios_base::iostate& err, long double& v) const
    {
        std::string xtrc;
        xtrc.reserve(32);
        beg = extract_float_(beg, end, io, err, xtrc);
        v = strtold(xtrc.c_str(), nullptr);
        if (beg == end)
            err |= ios_base::eofbit;
        return beg;
    }

    template <class CharT, class InIter>
    inline InIter num_get<CharT, InIter>::
    do_get(iter_type beg, iter_type end, ios_base& io,
           ios_base::iostate& err, void*& v) const
    {
        // Prepare for hex formatted input
        const ios_base::fmtflags fmt = io.flags();
        io.flags((fmt & ~ios_base::basefield) | ios_base::hex);

        using uintptr_type = typename std::conditional<
            sizeof(void*) <= sizeof(unsigned long),
            unsigned long,
            unsigned long long>::type;

        uintptr_type ul;
        beg = extract_int_(beg, end, io, err, ul);

        // Reset from hex formatted input.
        io.flags(fmt);

        v = reinterpret_cast<void*>(ul);
        return beg;
    }

    //
    // Methods of num_put
    //

    // Construct correctly padded string, as per 22.2.2.2.2
    // For use by integer and floating-point types after they have been
    // converted into a char_type string.
    template <class CharT, class OutIter>
    inline void num_put<CharT, OutIter>::
    pad_(CharT fill, std::streamsize newlen, ios_base& io,
         CharT* news, const CharT* olds, int& len) const
    {
        using traits_type = char_traits<CharT>;
        using ct = ctype<CharT>;

        std::streamsize oldlen = len;
        const size_t plen = static_cast<size_t>(newlen - oldlen);
        const ios_base::fmtflags adjust = io.flags() & ios_base::adjustfield;

        len = static_cast<int>(newlen);

        // Padding last
        if (adjust == ios_base::left) {
            traits_type::copy(news, olds, oldlen);
            traits_type::assign(news + oldlen, plen, fill);
            return;
        }

        size_t mod = 0;
        if (adjust == ios_base::internal) {
            // Pad after the sign, if there is one.
            // Pad after 0[xX], if there is one.
            // Who came up with these rules, anyway? Jeeze.
            if (ct::widen('-') == olds[0] || ct::widen('+') == olds[0]) {
                news[0] = olds[0];
                mod = 1;
                ++news;
            }
            else if (ct::widen('0') == olds[0]
                    && oldlen > 1
                    && (ct::widen('x') == olds[1] || ct::widen('X') == olds[1]))
            {
                news[0] = olds[0];
                news[1] = olds[1];
                mod = 2;
                news += 2;
            }
            // else Padding first
        }
        traits_type::assign(news, plen, fill);
        traits_type::copy(news + plen, olds + mod, oldlen - mod);
    }

    // Non-member
    template <class CharT, class ValueT>
    inline int int_to_char(CharT* bufend, ValueT v, ios_base::fmtflags flags, bool dec)
    {
        using ct = ctype<CharT>;

        const CharT zero = ct::widen('0');
        CharT* buf = bufend;

        if (dec) {
            // Decimal
            do {
                *--buf = zero + (v % 10);
                v /= 10;
            }
            while (v != 0);
        }
        else if ((flags & ios_base::basefield) == ios_base::oct) {
            // Octal
            do {
                *--buf = zero + (v & 0x7);
                v >>= 3;
            }
            while (v != 0);
        }
        else {
            // Hex
            const bool uppercase = flags & ios_base::uppercase;
            const CharT alpha = ct::widen("aA"[uppercase]);
            do {
                int nibble = v & 0xf;
                *--buf = nibble + (nibble < 10 ? zero : (alpha - 10));
                v >>= 4;
            }
            while (v != 0);
        }
        return bufend - buf;
    }

    template <class CharT, class OutIter>
    template <class ValueT>
    inline OutIter num_put<CharT, OutIter>::
    insert_int_(OutIter s, ios_base& io, CharT fill, ValueT v) const
    {
        using char_type = CharT;
        using unsigned_type = typename std::make_unsigned<ValueT>::type;
        using ct = ctype<char_type>;

        const ios_base::fmtflags flags = io.flags();

        // Long enough to hold hex, dec, and octal representations
        const int ilen = 5 * sizeof(ValueT);
        char_type buf[ilen];
        char_type* cs = buf;

        // [22.2.2.2.2] Stage 1, numeric conversion to character.
        // Result is returned right-justified in the buffer.
        const ios_base::fmtflags basefield = flags & ios_base::basefield;
        const bool dec = (basefield != ios_base::oct && basefield != ios_base::hex);
        const unsigned_type u = ((v > 0 || !dec)
                     ? unsigned_type(v) : -unsigned_type(v));
        int len = int_to_char(cs + ilen, u, flags, dec);
        cs += ilen - len;

        // Complete Stage 1, prepend numeric base or sign.
        if (dec) {
            // Decimal
            if (v >= 0) {
                if ((flags & ios_base::showpos) &&
                    std::is_signed<ValueT>::value)
                {
                    *--cs = ct::widen('+');
                    ++len;
                }
            }
            else {
                *--cs = ct::widen('-');
                ++len;
            }
        }
        else if ((flags & ios_base::showbase) && v) {
            if (basefield == ios_base::oct)
                *--cs = ct::widen('0'), ++len;
            else {
                // 'x' or 'X'
                const bool uppercase = flags & ios_base::uppercase;
                *--cs = ct::widen("xX"[uppercase]);
                // '0'
                *--cs = ct::widen('0');
                len += 2;
            }
        }

        // Pad
        const std::streamsize w = io.width();
        char_type cs3[w];
        if (w > static_cast<std::streamsize>(len)) {
            pad_(fill, w, io, cs3, cs, len);
            cs = cs3;
        }
        io.width(0);

        // [22.2.2.2.2] Stage 4.
        // Write resulting, fully-formatted string to output iterator.
        return std::copy_n(cs, len, s);
    }

    // Non-member
    inline void format_float(const ios_base& io, char* fptr, char mod)
    {
        ios_base::fmtflags flags = io.flags();
        *fptr++ = '%';
        // [22.2.2.2.2] Table 60
        if (flags & ios_base::showpos)
            *fptr++ = '+';
        if (flags & ios_base::showpoint)
            *fptr++ = '#';

        ios_base::fmtflags fltfield = flags & ios_base::floatfield;

        // Precision is always used except for hexfloat format.
        if (fltfield != (ios_base::fixed | ios_base::scientific))
        {
            // As per DR 231: not only when flags & ios_base::fixed || prec > 0
            *fptr++ = '.';
            *fptr++ = '*';
        }

        if (mod)
            *fptr++ = mod;
        // [22.2.2.2.2] Table 58
        if (fltfield == ios_base::fixed)
            *fptr++ = 'f';
        else if (fltfield == ios_base::scientific)
            *fptr++ = (flags & ios_base::uppercase) ? 'E' : 'e';
        else if (fltfield == (ios_base::fixed | ios_base::scientific))
            *fptr++ = (flags & ios_base::uppercase) ? 'A' : 'a';
        else
            *fptr++ = (flags & ios_base::uppercase) ? 'G' : 'g';
        *fptr = '\0';
    }

    // The following code uses vsnprintf (or vsprintf(), when
    // _GLIBCXX_USE_C99_STDIO is not defined) to convert floating point
    // values for insertion into a stream. An optimization would be to
    // replace them with code that works directly on a wide buffer and
    // then use pad to do the padding. It would be good to replace
    // them anyway to gain back the efficiency that C++ provides by
    // knowing up front the type of the values to insert. Also, sprintf
    // is dangerous since may lead to accidental buffer overruns.  This
    // implementation follows the C++ standard fairly directly as
    // outlined in 22.2.2.2 [lib.locale.num.put]
    template <class CharT, class OutIter>
    template <class ValueT>
    inline OutIter num_put<CharT, OutIter>::
    insert_float_(OutIter s, ios_base& io, CharT fill, char mod, ValueT v) const
    {
        // Use default precision if out of range
        const std::streamsize prec = io.precision() < 0 ? 6 : io.precision();

        // [22.2.2.2.2] Stage 1, numeric conversion to character.
        int len;
        // Long enough for the max format spec.
        char fbuf[16];
        format_float(io, fbuf, mod);

        // Precision is always used except for hexfloat format.
        const bool use_prec =
            (io.flags() & ios_base::floatfield) != ios_base::floatfield;

        // First determine the size of buffer needed
        if (use_prec)
            len = snprintf(nullptr, 0, fbuf, prec, v);
        else
            len = snprintf(nullptr, 0, fbuf, v);

        // Now convert to string
        int cs_size = len + 1;
        char cs[cs_size];
        if (use_prec)
            len = snprintf(cs, cs_size, fbuf, prec, v);
        else
            len = snprintf(cs, cs_size, fbuf, v);

        // [22.2.2.2.2] Stage 2, convert to char_type, using correct
        // numpunct.decimal_point() values for '.' and adding grouping.
        // Skipping, no locale implemented...

        // Add grouping, if necessary.
        // N.B. Make sure to not group things like 2e20, i.e., no decimal
        // point, scientific notation.
        // Skipping, no locale implemented...

        // Pad
        const std::streamsize w = io.width();
        char_type ws3[w];
        char* ws = cs;
        if (w > static_cast<std::streamsize>(len)) {
            pad_(fill, w, io, ws3, ws, len);
            ws = ws3;
        }
        io.width(0);

        // [22.2.2.2.2] Stage 4.
        // Write resulting, fully-formatted string to output iterator.
        return std::copy_n(ws, len, s);
      }

    template <class CharT, class OutIter>
    inline OutIter num_put<CharT, OutIter>::
    do_put(iter_type s, ios_base& io, char_type fill, bool v) const
    {
        const ios_base::fmtflags flags = io.flags();
        if (!(flags & ios_base::boolalpha)) {
            const long l = v;
            s = insert_int_(s, io, fill, l);
        }
        else {
            using char_type = CharT;
            using traits_type = char_traits<char_type>;
            using ct = ctype<char_type>;

            const char_type* name =
                v ? ct::truename() : ct::falsename();
            int len = traits_type::length(name);

            const std::streamsize w = io.width();
            if (w > static_cast<std::streamsize>(len)) {
                const std::streamsize plen = w - len;
                char_type ps[plen];

                traits_type::assign(ps, plen, fill);
                io.width(0);
                if ((flags & ios_base::adjustfield) == ios_base::left) {
                    s = std::copy_n(name, len, s);
                    s = std::copy_n(ps, plen, s);
                }
                else {
                    s = std::copy_n(ps, plen, s);
                    s = std::copy_n(name, len, s);
                }
                return s;
            }
            io.width(0);
            s = std::copy_n(name, len, s);
        }
        return s;
    }

    template <class CharT, class OutIter>
    inline OutIter num_put<CharT, OutIter>::
    do_put(iter_type s, ios_base& io, char_type fill, double v) const
    { return insert_float_(s, io, fill, char(), v); }

    template <class CharT, class OutIter>
    inline OutIter num_put<CharT, OutIter>::
    do_put(iter_type s, ios_base& io, char_type fill, long double v) const
    { return insert_float_(s, io, fill, 'L', v); }

    template <class CharT, class OutIter>
    inline OutIter num_put<CharT, OutIter>::
    do_put(iter_type s, ios_base& io, char_type fill, const void* v) const
    {
        const ios_base::fmtflags flags = io.flags();
        const ios_base::fmtflags fmt = ~(ios_base::basefield | ios_base::uppercase);

        io.flags((flags & fmt) | (ios_base::hex | ios_base::showbase));

        using uintptr_type = typename std::conditional<
            sizeof(const void*) <= sizeof(unsigned long),
            unsigned long,
            unsigned long long>::type;

        s = insert_int_(s, io, fill, reinterpret_cast<uintptr_type>(v));
        io.flags(flags);
        return s;
    }

} // namespace ard

