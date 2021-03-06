//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLanguagePch.h"


#if _M_IX86 || _M_AMD64

namespace Js
{
    // SIMD.Uint16x8 operation wrappers that cover instrinsics for x86/x64 system
    SIMDValue SIMDUint16x8Operation::OpUint16x8(uint16 x0, uint16 x1, uint16 x2, uint16 x3
        , uint16 x4, uint16 x5, uint16 x6, uint16 x7)
    {
        X86SIMDValue x86Result;
        // Sets the 8 signed 16-bit integer values, note in revised order: starts with x7 below
        x86Result.m128i_value = _mm_set_epi16((int16)x7, (int16)x6, (int16)x5, (int16)x4, (int16)x3, (int16)x2, (int16)x1, (int16)x0);

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpMin(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);

        // _mm_min_epu16 is SSE4.1
        //x86Result.m128i_value = _mm_min_epu16(tmpaValue.m128i_value, tmpbValue.m128i_value);

        // XOR the sign bits so the comparison comes out correct for unsigned
        tmpaValue.m128i_value = _mm_xor_si128(tmpaValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        tmpbValue.m128i_value = _mm_xor_si128(tmpbValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        x86Result.m128i_value = _mm_min_epi16(tmpaValue.m128i_value, tmpbValue.m128i_value);

        x86Result.m128i_value = _mm_xor_si128(x86Result.m128i_value, X86_WORD_SIGNBITS.m128i_value);

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpMax(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);

        // _mm_max_epu16 is SSE4.1
        //x86Result.m128i_value = _mm_max_epu16(tmpaValue.m128i_value, tmpbValue.m128i_value);

        // XOR the sign bits so the comparison comes out correct for unsigned
        tmpaValue.m128i_value = _mm_xor_si128(tmpaValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        tmpbValue.m128i_value = _mm_xor_si128(tmpbValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        x86Result.m128i_value = _mm_max_epi16(tmpaValue.m128i_value, tmpbValue.m128i_value);

        x86Result.m128i_value = _mm_xor_si128(x86Result.m128i_value, X86_WORD_SIGNBITS.m128i_value);

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpLessThan(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);

        // Signed comparison of unsigned ints can be done if the ints have the "sign" bit xored with 1
        tmpaValue.m128i_value = _mm_xor_si128(tmpaValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        tmpbValue.m128i_value = _mm_xor_si128(tmpbValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        x86Result.m128i_value = _mm_cmplt_epi16(tmpaValue.m128i_value, tmpbValue.m128i_value); // compare a < b?

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpLessThanOrEqual(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);

        // Signed comparison of unsigned ints can be done if the ints have the "sign" bit xored with 1
        tmpaValue.m128i_value = _mm_xor_si128(tmpaValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        tmpbValue.m128i_value = _mm_xor_si128(tmpbValue.m128i_value, X86_WORD_SIGNBITS.m128i_value);
        x86Result.m128i_value = _mm_cmplt_epi16(tmpaValue.m128i_value, tmpbValue.m128i_value); // compare a < b?
        tmpaValue.m128i_value = _mm_cmpeq_epi16(tmpaValue.m128i_value, tmpbValue.m128i_value); // compare a == b?
        x86Result.m128i_value = _mm_or_si128(x86Result.m128i_value, tmpaValue.m128i_value);   // result = (a<b)|(a==b)

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpShiftRightByScalar(const SIMDValue& value, int count)
    {
        X86SIMDValue x86Result = { { 0, 0, 0, 0 } };
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(value);

        if (count < 16)
        {
            x86Result.m128i_value = _mm_srli_epi16(tmpaValue.m128i_value, count);
        }

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpAddSaturate(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);
        x86Result.m128i_value = _mm_adds_epu16(tmpaValue.m128i_value, tmpbValue.m128i_value); // a + b saturated

        return X86SIMDValue::ToSIMDValue(x86Result);
    }

    SIMDValue SIMDUint16x8Operation::OpSubSaturate(const SIMDValue& aValue, const SIMDValue& bValue)
    {
        X86SIMDValue x86Result;
        X86SIMDValue tmpaValue = X86SIMDValue::ToX86SIMDValue(aValue);
        X86SIMDValue tmpbValue = X86SIMDValue::ToX86SIMDValue(bValue);
        x86Result.m128i_value = _mm_subs_epu16(tmpaValue.m128i_value, tmpbValue.m128i_value); // a - b saturated

        return X86SIMDValue::ToSIMDValue(x86Result);
    }
}

#endif
