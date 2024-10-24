/**
 * Copyright (C) 2021-2022, by Wu Jianhua (toqsxw@outlook.com)
 *
 * This library is distributed under the Apache-2.0 license.
 */

#pragma once

#ifndef SLINTRINSIC_H_
#define SLINTRINSIC_H_

#if defined(_MSC_VER) && !defined(__clang__)
#define SL_HAVE_INTRISIC
#endif

#include <cstdint>
#include <concepts>
#include <immintrin.h>

struct FLOATX4
{
public:
    using value_type = __m128;

    FLOATX4() noexcept
    {
    }

    FLOATX4(__m128 other) noexcept :
        v{ other }
    {
    }

    FLOATX4(float value) noexcept :
        v{ _mm_set1_ps(value) }
    {
    }

    FLOATX4(float _0, float _1, float _2, float _3) noexcept :
        v{ _mm_set_ps(_3, _2, _1, _0) }
    {
    }

    FLOATX4  operator+(const FLOATX4 &other) noexcept
    {
        return _mm_add_ps(this->v, other.v);
    }

    FLOATX4  operator*(const FLOATX4 &other) noexcept
    {
        return _mm_mul_ps(this->v, other.v);
    }

    FLOATX4  operator-(const FLOATX4 &other) noexcept
    {
        return _mm_sub_ps(this->v, other.v);
    }

    FLOATX4  operator/(const FLOATX4 &other) noexcept
    {
        return _mm_div_ps(this->v, other.v);
    }

    operator __m128 &() noexcept
    {
        return v;
    }

    operator const __m128 &() const noexcept
    {
        return v;
    }

    void load(const float *src) noexcept
    {
        v = _mm_load_ps(src);
    }

    void store(float *dst) noexcept
    {
        _mm_store_ps(dst, v);
    }

    void loadu(const float *src) noexcept
    {
        v = _mm_loadu_ps(src);
    }

    void storeu(float *dst) noexcept
    {
        _mm_storeu_ps(dst, v);
    }

    __m128i cvt2int32() const noexcept
    {
        return _mm_cvtps_epi32(v);
    }

    FLOATX4 fmadd(const FLOATX4 &a, const FLOATX4 &b) const noexcept
    {
        return _mm_fmadd_ps(v, a, b);
    }

    template <int imm8>
    FLOATX4 shuffle(const FLOATX4 &a) const noexcept
    {
        return _mm_shuffle_ps(v, a, imm8);
    }

    template <int rounding>
    FLOATX4 round() const noexcept
    {
        return _mm_round_ps(v, rounding);
    }

    FLOATX4 floor() const noexcept
    {
        return round<_MM_FROUND_FLOOR>();
    }

    FLOATX4 ceil() const noexcept
    {
        return round<_MM_FROUND_CEIL>();
    }

public:
    __m128 v;
};

struct FLOATX8
{
public:
    using value_type = __m256;

    FLOATX8() noexcept
    {
    }

    FLOATX8(__m256 other) noexcept :
        v{ other }
    {
    }

    FLOATX8(float value) noexcept :
        v{ _mm256_set1_ps(value) }
    {
    }

    FLOATX8(float _0, float _1, float _2, float _3, float _4, float _5, float _6, float _7) noexcept :
        v{ _mm256_set_ps(_7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    FLOATX8  operator+(const FLOATX8 &other) noexcept
    {
        return _mm256_add_ps(this->v, other.v);
    }

    FLOATX8  operator*(const FLOATX8 &other) noexcept
    {
        return _mm256_mul_ps(this->v, other.v);
    }

    FLOATX8  operator-(const FLOATX8 &other) noexcept
    {
        return _mm256_sub_ps(this->v, other.v);
    }

    FLOATX8  operator/(const FLOATX8 &other) noexcept
    {
        return _mm256_div_ps(this->v, other.v);
    }

    operator __m256 &() noexcept
    {
        return v;
    }

    operator const __m256 &() const noexcept
    {
        return v;
    }

    void load(const float *src) noexcept
    {
        v = _mm256_load_ps(src);
    }

    void store(float *dst) noexcept
    {
        _mm256_store_ps(dst, v);
    }

    void loadu(const float *src) noexcept
    {
        v = _mm256_loadu_ps(src);
    }

    void storeu(float *dst) noexcept
    {
        _mm256_storeu_ps(dst, v);
    }

    __m256i cvt2int32() const noexcept
    {
        return _mm256_cvtps_epi32(v);
    }

    FLOATX8 fmadd(const FLOATX8 &a, const FLOATX8 &b) const noexcept
    {
        return _mm256_fmadd_ps(v, a, b);
    }

    template <int imm8>
    FLOATX8 shuffle(const FLOATX8 &a) const noexcept
    {
        return _mm256_shuffle_ps(v, a, imm8);
    }

    template <int rounding>
    FLOATX8 round() const noexcept
    {
        return _mm256_round_ps(v, rounding);
    }

    FLOATX8 floor() const noexcept
    {
        return _mm256_floor_ps(v);
    }

    FLOATX8 ceil() const noexcept
    {
        return _mm256_ceil_ps(v);
    }

public:
    __m256 v;
};

struct FLOATX16
{
public:
    using value_type = __m512;

    FLOATX16() noexcept
    {
    }

    FLOATX16(__m512 other) noexcept :
        v{ other }
    {
    }

    FLOATX16(float value) noexcept :
        v{ _mm512_set1_ps(value) }
    {
    }

    FLOATX16  operator+(const FLOATX16 &other) noexcept
    {
        return _mm512_add_ps(this->v, other.v);
    }

    FLOATX16  operator*(const FLOATX16 &other) noexcept
    {
        return _mm512_mul_ps(this->v, other.v);
    }

    FLOATX16  operator-(const FLOATX16 &other) noexcept
    {
        return _mm512_sub_ps(this->v, other.v);
    }

    FLOATX16  operator/(const FLOATX16 &other) noexcept
    {
        return _mm512_div_ps(this->v, other.v);
    }

    operator __m512 &() noexcept
    {
        return v;
    }

    operator const __m512 &() const noexcept
    {
        return v;
    }

    void load(const float *src) noexcept
    {
        v = _mm512_load_ps(src);
    }

    void store(float *dst) noexcept
    {
        _mm512_store_ps(dst, v);
    }

    void loadu(const float *src) noexcept
    {
        v = _mm512_loadu_ps(src);
    }

    void storeu(float *dst) noexcept
    {
        _mm512_storeu_ps(dst, v);
    }

    __m512i cvt2int32() const noexcept
    {
        return _mm512_cvtps_epi32(v);
    }

    FLOATX16 fmadd(const FLOATX16 &a, const FLOATX16 &b) const noexcept
    {
        return _mm512_fmadd_ps(v, a, b);
    }

    template <int imm8>
    FLOATX16 shuffle(const FLOATX16 &a) const noexcept
    {
        return _mm512_shuffle_ps(v, a, imm8);
    }

    FLOATX16 floor() const noexcept
    {
        return _mm512_floor_ps(v);
    }

    FLOATX16 ceil() const noexcept
    {
        return _mm512_ceil_ps(v);
    }

public:
    __m512 v;
};

struct DOUBLEX2
{
public:
    using value_type = __m128d;

    DOUBLEX2() noexcept
    {
    }

    DOUBLEX2(__m128d other) noexcept :
        v{ other }
    {
    }

    DOUBLEX2(double value) noexcept :
        v{ _mm_set1_pd(value) }
    {
    }

    DOUBLEX2(double _0, double _1) noexcept :
        v{ _mm_set_pd(_1, _0) }
    {
    }

    DOUBLEX2  operator+(const DOUBLEX2 &other) noexcept
    {
        return _mm_add_pd(this->v, other.v);
    }

    DOUBLEX2  operator*(const DOUBLEX2 &other) noexcept
    {
        return _mm_mul_pd(this->v, other.v);
    }

    DOUBLEX2  operator-(const DOUBLEX2 &other) noexcept
    {
        return _mm_sub_pd(this->v, other.v);
    }

    DOUBLEX2  operator/(const DOUBLEX2 &other) noexcept
    {
        return _mm_div_pd(this->v, other.v);
    }

    operator __m128d &() noexcept
    {
        return v;
    }

    operator const __m128d &() const noexcept
    {
        return v;
    }

    void load(const double *src) noexcept
    {
        v = _mm_load_pd(src);
    }

    void store(double *dst) noexcept
    {
        _mm_store_pd(dst, v);
    }

    void loadu(const double *src) noexcept
    {
        v = _mm_loadu_pd(src);
    }

    void storeu(double *dst) noexcept
    {
        _mm_storeu_pd(dst, v);
    }

    template <int imm8>
    DOUBLEX2 shuffle(const DOUBLEX2 &a) const noexcept
    {
        return _mm_shuffle_pd(v, a, imm8);
    }

    template <int rounding>
    DOUBLEX2 round() const noexcept
    {
        return _mm_round_pd(v, rounding);
    }

    DOUBLEX2 floor() const noexcept
    {
        return round<_MM_FROUND_FLOOR>();
    }

    DOUBLEX2 ceil() const noexcept
    {
        return round<_MM_FROUND_CEIL>();
    }

public:
    __m128d v;
};

struct DOUBLEX4
{
public:
    using value_type = __m256d;

    DOUBLEX4() noexcept
    {
    }

    DOUBLEX4(__m256d other) noexcept :
        v{ other }
    {
    }

    DOUBLEX4(double value) noexcept :
        v{ _mm256_set1_pd(value) }
    {
    }

    DOUBLEX4(double _0, double _1, double _2, double _3) noexcept :
        v{ _mm256_set_pd(_3, _2, _1, _0) }
    {
    }

    DOUBLEX4  operator+(const DOUBLEX4 &other) noexcept
    {
        return _mm256_add_pd(this->v, other.v);
    }

    DOUBLEX4  operator*(const DOUBLEX4 &other) noexcept
    {
        return _mm256_mul_pd(this->v, other.v);
    }

    DOUBLEX4  operator-(const DOUBLEX4 &other) noexcept
    {
        return _mm256_sub_pd(this->v, other.v);
    }

    DOUBLEX4  operator/(const DOUBLEX4 &other) noexcept
    {
        return _mm256_div_pd(this->v, other.v);
    }

    operator __m256d &() noexcept
    {
        return v;
    }

    operator const __m256d &() const noexcept
    {
        return v;
    }

    void load(const double *src) noexcept
    {
        v = _mm256_load_pd(src);
    }

    void store(double *dst) noexcept
    {
        _mm256_store_pd(dst, v);
    }

    void loadu(const double *src) noexcept
    {
        v = _mm256_loadu_pd(src);
    }

    void storeu(double *dst) noexcept
    {
        _mm256_storeu_pd(dst, v);
    }

    template <int imm8>
    DOUBLEX4 shuffle(const DOUBLEX4 &a) const noexcept
    {
        return _mm256_shuffle_pd(v, a, imm8);
    }

    template <int rounding>
    DOUBLEX4 round() const noexcept
    {
        return _mm256_round_pd(v, rounding);
    }

    DOUBLEX4 floor() const noexcept
    {
        return _mm256_floor_pd(v);
    }

    DOUBLEX4 ceil() const noexcept
    {
        return _mm256_ceil_pd(v);
    }

public:
    __m256d v;
};

struct DOUBLEX8
{
public:
    using value_type = __m512d;

    DOUBLEX8() noexcept
    {
    }

    DOUBLEX8(__m512d other) noexcept :
        v{ other }
    {
    }

    DOUBLEX8(double value) noexcept :
        v{ _mm512_set1_pd(value) }
    {
    }

    DOUBLEX8  operator+(const DOUBLEX8 &other) noexcept
    {
        return _mm512_add_pd(this->v, other.v);
    }

    DOUBLEX8  operator*(const DOUBLEX8 &other) noexcept
    {
        return _mm512_mul_pd(this->v, other.v);
    }

    DOUBLEX8  operator-(const DOUBLEX8 &other) noexcept
    {
        return _mm512_sub_pd(this->v, other.v);
    }

    DOUBLEX8  operator/(const DOUBLEX8 &other) noexcept
    {
        return _mm512_div_pd(this->v, other.v);
    }

    operator __m512d &() noexcept
    {
        return v;
    }

    operator const __m512d &() const noexcept
    {
        return v;
    }

    void load(const double *src) noexcept
    {
        v = _mm512_load_pd(src);
    }

    void store(double *dst) noexcept
    {
        _mm512_store_pd(dst, v);
    }

    void loadu(const double *src) noexcept
    {
        v = _mm512_loadu_pd(src);
    }

    void storeu(double *dst) noexcept
    {
        _mm512_storeu_pd(dst, v);
    }

    template <int imm8>
    DOUBLEX8 shuffle(const DOUBLEX8 &a) const noexcept
    {
        return _mm512_shuffle_pd(v, a, imm8);
    }

    DOUBLEX8 floor() const noexcept
    {
        return _mm512_floor_pd(v);
    }

    DOUBLEX8 ceil() const noexcept
    {
        return _mm512_ceil_pd(v);
    }

public:
    __m512d v;
};

struct INT8X16
{
public:
    using value_type = __m128i;

    INT8X16() noexcept
    {
    }

    INT8X16(__m128i other) noexcept :
        v{ other }
    {
    }

    INT8X16(int8_t value) noexcept :
        v{ _mm_set1_epi8(value) }
    {
    }

    INT8X16(int8_t _0, int8_t _1, int8_t _2, int8_t _3, int8_t _4, int8_t _5, int8_t _6, int8_t _7, int8_t _8, int8_t _9, int8_t _10, int8_t _11, int8_t _12, int8_t _13, int8_t _14, int8_t _15) noexcept :
        v{ _mm_set_epi8(_15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    INT8X16  operator+(const INT8X16 &other) noexcept
    {
        return _mm_add_epi8(this->v, other.v);
    }

    INT8X16  operator-(const INT8X16 &other) noexcept
    {
        return _mm_sub_epi8(this->v, other.v);
    }

    INT8X16  operator/(const INT8X16 &other) noexcept
    {
        return _mm_div_epi8(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const int8_t *src) noexcept
    {
        loadu(src);
    }

    void store(int8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int8_t *src) noexcept
    {
        v = _mm_loadu_epi8(src);
    }

    void storeu(int8_t *dst) noexcept
    {
        _mm_storeu_epi8(dst, v);
    }

public:
    __m128i v;
};

struct UINT8X16
{
public:
    using value_type = __m128i;

    UINT8X16() noexcept
    {
    }

    UINT8X16(__m128i other) noexcept :
        v{ other }
    {
    }

    UINT8X16(uint8_t value) noexcept :
        v{ _mm_set1_epi8(value) }
    {
    }

    UINT8X16(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7, uint8_t _8, uint8_t _9, uint8_t _10, uint8_t _11, uint8_t _12, uint8_t _13, uint8_t _14, uint8_t _15) noexcept :
        v{ _mm_set_epi8(_15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    UINT8X16  operator+(const UINT8X16 &other) noexcept
    {
        return _mm_add_epi8(this->v, other.v);
    }

    UINT8X16  operator-(const UINT8X16 &other) noexcept
    {
        return _mm_sub_epi8(this->v, other.v);
    }

    UINT8X16  operator/(const UINT8X16 &other) noexcept
    {
        return _mm_div_epi8(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const uint8_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint8_t *src) noexcept
    {
        v = _mm_loadu_epi8(src);
    }

    void storeu(uint8_t *dst) noexcept
    {
        _mm_storeu_epi8(dst, v);
    }

public:
    __m128i v;
};

struct INT16X8
{
public:
    using value_type = __m128i;

    INT16X8() noexcept
    {
    }

    INT16X8(__m128i other) noexcept :
        v{ other }
    {
    }

    INT16X8(int16_t value) noexcept :
        v{ _mm_set1_epi16(value) }
    {
    }

    INT16X8(int16_t _0, int16_t _1, int16_t _2, int16_t _3, int16_t _4, int16_t _5, int16_t _6, int16_t _7) noexcept :
        v{ _mm_set_epi16(_7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    INT16X8  operator+(const INT16X8 &other) noexcept
    {
        return _mm_add_epi16(this->v, other.v);
    }

    INT16X8  operator*(const INT16X8 &other) noexcept
    {
        return _mm_mullo_epi16(this->v, other.v);
    }

    INT16X8  operator-(const INT16X8 &other) noexcept
    {
        return _mm_sub_epi16(this->v, other.v);
    }

    INT16X8  operator/(const INT16X8 &other) noexcept
    {
        return _mm_div_epi16(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const int16_t *src) noexcept
    {
        loadu(src);
    }

    void store(int16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int16_t *src) noexcept
    {
        v = _mm_loadu_epi16(src);
    }

    void storeu(int16_t *dst) noexcept
    {
        _mm_storeu_epi16(dst, v);
    }

    __m128i cvt2uint8() const noexcept
    {
        return _mm_cvtepi16_epi8(v);
    }

public:
    __m128i v;
};

struct UINT16X8
{
public:
    using value_type = __m128i;

    UINT16X8() noexcept
    {
    }

    UINT16X8(__m128i other) noexcept :
        v{ other }
    {
    }

    UINT16X8(uint16_t value) noexcept :
        v{ _mm_set1_epi16(value) }
    {
    }

    UINT16X8(uint16_t _0, uint16_t _1, uint16_t _2, uint16_t _3, uint16_t _4, uint16_t _5, uint16_t _6, uint16_t _7) noexcept :
        v{ _mm_set_epi16(_7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    UINT16X8  operator+(const UINT16X8 &other) noexcept
    {
        return _mm_add_epi16(this->v, other.v);
    }

    UINT16X8  operator*(const UINT16X8 &other) noexcept
    {
        return _mm_mullo_epi16(this->v, other.v);
    }

    UINT16X8  operator-(const UINT16X8 &other) noexcept
    {
        return _mm_sub_epi16(this->v, other.v);
    }

    UINT16X8  operator/(const UINT16X8 &other) noexcept
    {
        return _mm_div_epi16(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const uint16_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint16_t *src) noexcept
    {
        v = _mm_loadu_epi16(src);
    }

    void storeu(uint16_t *dst) noexcept
    {
        _mm_storeu_epi16(dst, v);
    }

    __m128i cvt2uint8() const noexcept
    {
        return _mm_cvtepi16_epi8(v);
    }

public:
    __m128i v;
};

struct INT32X4
{
public:
    using value_type = __m128i;

    INT32X4() noexcept
    {
    }

    INT32X4(__m128i other) noexcept :
        v{ other }
    {
    }

    INT32X4(int32_t value) noexcept :
        v{ _mm_set1_epi32(value) }
    {
    }

    INT32X4(int32_t _0, int32_t _1, int32_t _2, int32_t _3) noexcept :
        v{ _mm_set_epi32(_3, _2, _1, _0) }
    {
    }

    INT32X4  operator+(const INT32X4 &other) noexcept
    {
        return _mm_add_epi32(this->v, other.v);
    }

    INT32X4  operator*(const INT32X4 &other) noexcept
    {
        return _mm_mullo_epi32(this->v, other.v);
    }

    INT32X4  operator-(const INT32X4 &other) noexcept
    {
        return _mm_sub_epi32(this->v, other.v);
    }

    INT32X4  operator/(const INT32X4 &other) noexcept
    {
        return _mm_div_epi32(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const int32_t *src) noexcept
    {
        loadu(src);
    }

    void store(int32_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int32_t *src) noexcept
    {
        v = _mm_loadu_epi32(src);
    }

    void storeu(int32_t *dst) noexcept
    {
        _mm_storeu_epi32(dst, v);
    }

public:
    __m128i v;
};

struct UINT32X4
{
public:
    using value_type = __m128i;

    UINT32X4() noexcept
    {
    }

    UINT32X4(__m128i other) noexcept :
        v{ other }
    {
    }

    UINT32X4(uint32_t value) noexcept :
        v{ _mm_set1_epi32(value) }
    {
    }

    UINT32X4(uint32_t _0, uint32_t _1, uint32_t _2, uint32_t _3) noexcept :
        v{ _mm_set_epi32(_3, _2, _1, _0) }
    {
    }

    UINT32X4  operator+(const UINT32X4 &other) noexcept
    {
        return _mm_add_epi32(this->v, other.v);
    }

    UINT32X4  operator*(const UINT32X4 &other) noexcept
    {
        return _mm_mullo_epi32(this->v, other.v);
    }

    UINT32X4  operator-(const UINT32X4 &other) noexcept
    {
        return _mm_sub_epi32(this->v, other.v);
    }

    UINT32X4  operator/(const UINT32X4 &other) noexcept
    {
        return _mm_div_epi32(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const uint32_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint32_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint32_t *src) noexcept
    {
        v = _mm_loadu_epi32(src);
    }

    void storeu(uint32_t *dst) noexcept
    {
        _mm_storeu_epi32(dst, v);
    }

public:
    __m128i v;
};

struct INT64X2
{
public:
    using value_type = __m128i;

    INT64X2() noexcept
    {
    }

    INT64X2(__m128i other) noexcept :
        v{ other }
    {
    }

    INT64X2(int64_t value) noexcept :
        v{ _mm_set1_epi64x(value) }
    {
    }

    INT64X2(int64_t _0, int64_t _1) noexcept :
        v{ _mm_set_epi64x(_1, _0) }
    {
    }

    INT64X2  operator+(const INT64X2 &other) noexcept
    {
        return _mm_add_epi64(this->v, other.v);
    }

    INT64X2  operator-(const INT64X2 &other) noexcept
    {
        return _mm_sub_epi64(this->v, other.v);
    }

    INT64X2  operator/(const INT64X2 &other) noexcept
    {
        return _mm_div_epi64(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const int64_t *src) noexcept
    {
        loadu(src);
    }

    void store(int64_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int64_t *src) noexcept
    {
        v = _mm_loadu_epi64(src);
    }

    void storeu(int64_t *dst) noexcept
    {
        _mm_storeu_epi64(dst, v);
    }

public:
    __m128i v;
};

struct UINT64X2
{
public:
    using value_type = __m128i;

    UINT64X2() noexcept
    {
    }

    UINT64X2(__m128i other) noexcept :
        v{ other }
    {
    }

    UINT64X2(uint64_t value) noexcept :
        v{ _mm_set1_epi64x(value) }
    {
    }

    UINT64X2(uint64_t _0, uint64_t _1) noexcept :
        v{ _mm_set_epi64x(_1, _0) }
    {
    }

    UINT64X2  operator+(const UINT64X2 &other) noexcept
    {
        return _mm_add_epi64(this->v, other.v);
    }

    UINT64X2  operator-(const UINT64X2 &other) noexcept
    {
        return _mm_sub_epi64(this->v, other.v);
    }

    UINT64X2  operator/(const UINT64X2 &other) noexcept
    {
        return _mm_div_epi64(this->v, other.v);
    }

    operator __m128i &() noexcept
    {
        return v;
    }

    operator const __m128i &() const noexcept
    {
        return v;
    }

    void load(const uint64_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint64_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint64_t *src) noexcept
    {
        v = _mm_loadu_epi64(src);
    }

    void storeu(uint64_t *dst) noexcept
    {
        _mm_storeu_epi64(dst, v);
    }

public:
    __m128i v;
};

struct INT8X32
{
public:
    using value_type = __m256i;

    INT8X32() noexcept
    {
    }

    INT8X32(__m256i other) noexcept :
        v{ other }
    {
    }

    INT8X32(int8_t value) noexcept :
        v{ _mm256_set1_epi8(value) }
    {
    }

    INT8X32(int8_t _0, int8_t _1, int8_t _2, int8_t _3, int8_t _4, int8_t _5, int8_t _6, int8_t _7, int8_t _8, int8_t _9, int8_t _10, int8_t _11, int8_t _12, int8_t _13, int8_t _14, int8_t _15, int8_t _16, int8_t _17, int8_t _18, int8_t _19, int8_t _20, int8_t _21, int8_t _22, int8_t _23, int8_t _24, int8_t _25, int8_t _26, int8_t _27, int8_t _28, int8_t _29, int8_t _30, int8_t _31) noexcept :
        v{ _mm256_set_epi8(_31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    INT8X32  operator+(const INT8X32 &other) noexcept
    {
        return _mm256_add_epi8(this->v, other.v);
    }

    INT8X32  operator-(const INT8X32 &other) noexcept
    {
        return _mm256_sub_epi8(this->v, other.v);
    }

    INT8X32  operator/(const INT8X32 &other) noexcept
    {
        return _mm256_div_epi8(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const int8_t *src) noexcept
    {
        loadu(src);
    }

    void store(int8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int8_t *src) noexcept
    {
        v = _mm256_loadu_epi8(src);
    }

    void storeu(int8_t *dst) noexcept
    {
        _mm256_storeu_epi8(dst, v);
    }

public:
    __m256i v;
};

struct UINT8X32
{
public:
    using value_type = __m256i;

    UINT8X32() noexcept
    {
    }

    UINT8X32(__m256i other) noexcept :
        v{ other }
    {
    }

    UINT8X32(uint8_t value) noexcept :
        v{ _mm256_set1_epi8(value) }
    {
    }

    UINT8X32(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7, uint8_t _8, uint8_t _9, uint8_t _10, uint8_t _11, uint8_t _12, uint8_t _13, uint8_t _14, uint8_t _15, uint8_t _16, uint8_t _17, uint8_t _18, uint8_t _19, uint8_t _20, uint8_t _21, uint8_t _22, uint8_t _23, uint8_t _24, uint8_t _25, uint8_t _26, uint8_t _27, uint8_t _28, uint8_t _29, uint8_t _30, uint8_t _31) noexcept :
        v{ _mm256_set_epi8(_31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    UINT8X32  operator+(const UINT8X32 &other) noexcept
    {
        return _mm256_add_epi8(this->v, other.v);
    }

    UINT8X32  operator-(const UINT8X32 &other) noexcept
    {
        return _mm256_sub_epi8(this->v, other.v);
    }

    UINT8X32  operator/(const UINT8X32 &other) noexcept
    {
        return _mm256_div_epi8(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const uint8_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint8_t *src) noexcept
    {
        v = _mm256_loadu_epi8(src);
    }

    void storeu(uint8_t *dst) noexcept
    {
        _mm256_storeu_epi8(dst, v);
    }

public:
    __m256i v;
};

struct INT16X16
{
public:
    using value_type = __m256i;

    INT16X16() noexcept
    {
    }

    INT16X16(__m256i other) noexcept :
        v{ other }
    {
    }

    INT16X16(int16_t value) noexcept :
        v{ _mm256_set1_epi16(value) }
    {
    }

    INT16X16(int16_t _0, int16_t _1, int16_t _2, int16_t _3, int16_t _4, int16_t _5, int16_t _6, int16_t _7, int16_t _8, int16_t _9, int16_t _10, int16_t _11, int16_t _12, int16_t _13, int16_t _14, int16_t _15) noexcept :
        v{ _mm256_set_epi16(_15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    INT16X16  operator+(const INT16X16 &other) noexcept
    {
        return _mm256_add_epi16(this->v, other.v);
    }

    INT16X16  operator*(const INT16X16 &other) noexcept
    {
        return _mm256_mullo_epi16(this->v, other.v);
    }

    INT16X16  operator-(const INT16X16 &other) noexcept
    {
        return _mm256_sub_epi16(this->v, other.v);
    }

    INT16X16  operator/(const INT16X16 &other) noexcept
    {
        return _mm256_div_epi16(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const int16_t *src) noexcept
    {
        loadu(src);
    }

    void store(int16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int16_t *src) noexcept
    {
        v = _mm256_loadu_epi16(src);
    }

    void storeu(int16_t *dst) noexcept
    {
        _mm256_storeu_epi16(dst, v);
    }

    __m128i cvt2uint8() const noexcept
    {
        return _mm256_cvtepi16_epi8(v);
    }

public:
    __m256i v;
};

struct UINT16X16
{
public:
    using value_type = __m256i;

    UINT16X16() noexcept
    {
    }

    UINT16X16(__m256i other) noexcept :
        v{ other }
    {
    }

    UINT16X16(uint16_t value) noexcept :
        v{ _mm256_set1_epi16(value) }
    {
    }

    UINT16X16(uint16_t _0, uint16_t _1, uint16_t _2, uint16_t _3, uint16_t _4, uint16_t _5, uint16_t _6, uint16_t _7, uint16_t _8, uint16_t _9, uint16_t _10, uint16_t _11, uint16_t _12, uint16_t _13, uint16_t _14, uint16_t _15) noexcept :
        v{ _mm256_set_epi16(_15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    UINT16X16  operator+(const UINT16X16 &other) noexcept
    {
        return _mm256_add_epi16(this->v, other.v);
    }

    UINT16X16  operator*(const UINT16X16 &other) noexcept
    {
        return _mm256_mullo_epi16(this->v, other.v);
    }

    UINT16X16  operator-(const UINT16X16 &other) noexcept
    {
        return _mm256_sub_epi16(this->v, other.v);
    }

    UINT16X16  operator/(const UINT16X16 &other) noexcept
    {
        return _mm256_div_epi16(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const uint16_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint16_t *src) noexcept
    {
        v = _mm256_loadu_epi16(src);
    }

    void storeu(uint16_t *dst) noexcept
    {
        _mm256_storeu_epi16(dst, v);
    }

    __m128i cvt2uint8() const noexcept
    {
        return _mm256_cvtepi16_epi8(v);
    }

public:
    __m256i v;
};

struct INT32X8
{
public:
    using value_type = __m256i;

    INT32X8() noexcept
    {
    }

    INT32X8(__m256i other) noexcept :
        v{ other }
    {
    }

    INT32X8(int32_t value) noexcept :
        v{ _mm256_set1_epi32(value) }
    {
    }

    INT32X8(int32_t _0, int32_t _1, int32_t _2, int32_t _3, int32_t _4, int32_t _5, int32_t _6, int32_t _7) noexcept :
        v{ _mm256_set_epi32(_7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    INT32X8  operator+(const INT32X8 &other) noexcept
    {
        return _mm256_add_epi32(this->v, other.v);
    }

    INT32X8  operator*(const INT32X8 &other) noexcept
    {
        return _mm256_mullo_epi32(this->v, other.v);
    }

    INT32X8  operator-(const INT32X8 &other) noexcept
    {
        return _mm256_sub_epi32(this->v, other.v);
    }

    INT32X8  operator/(const INT32X8 &other) noexcept
    {
        return _mm256_div_epi32(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const int32_t *src) noexcept
    {
        loadu(src);
    }

    void store(int32_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int32_t *src) noexcept
    {
        v = _mm256_loadu_epi32(src);
    }

    void storeu(int32_t *dst) noexcept
    {
        _mm256_storeu_epi32(dst, v);
    }

public:
    __m256i v;
};

struct UINT32X8
{
public:
    using value_type = __m256i;

    UINT32X8() noexcept
    {
    }

    UINT32X8(__m256i other) noexcept :
        v{ other }
    {
    }

    UINT32X8(uint32_t value) noexcept :
        v{ _mm256_set1_epi32(value) }
    {
    }

    UINT32X8(uint32_t _0, uint32_t _1, uint32_t _2, uint32_t _3, uint32_t _4, uint32_t _5, uint32_t _6, uint32_t _7) noexcept :
        v{ _mm256_set_epi32(_7, _6, _5, _4, _3, _2, _1, _0) }
    {
    }

    UINT32X8  operator+(const UINT32X8 &other) noexcept
    {
        return _mm256_add_epi32(this->v, other.v);
    }

    UINT32X8  operator*(const UINT32X8 &other) noexcept
    {
        return _mm256_mullo_epi32(this->v, other.v);
    }

    UINT32X8  operator-(const UINT32X8 &other) noexcept
    {
        return _mm256_sub_epi32(this->v, other.v);
    }

    UINT32X8  operator/(const UINT32X8 &other) noexcept
    {
        return _mm256_div_epi32(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const uint32_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint32_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint32_t *src) noexcept
    {
        v = _mm256_loadu_epi32(src);
    }

    void storeu(uint32_t *dst) noexcept
    {
        _mm256_storeu_epi32(dst, v);
    }

public:
    __m256i v;
};

struct INT64X4
{
public:
    using value_type = __m256i;

    INT64X4() noexcept
    {
    }

    INT64X4(__m256i other) noexcept :
        v{ other }
    {
    }

    INT64X4(int64_t value) noexcept :
        v{ _mm256_set1_epi64x(value) }
    {
    }

    INT64X4(int64_t _0, int64_t _1, int64_t _2, int64_t _3) noexcept :
        v{ _mm256_set_epi64x(_3, _2, _1, _0) }
    {
    }

    INT64X4  operator+(const INT64X4 &other) noexcept
    {
        return _mm256_add_epi64(this->v, other.v);
    }

    INT64X4  operator-(const INT64X4 &other) noexcept
    {
        return _mm256_sub_epi64(this->v, other.v);
    }

    INT64X4  operator/(const INT64X4 &other) noexcept
    {
        return _mm256_div_epi64(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const int64_t *src) noexcept
    {
        loadu(src);
    }

    void store(int64_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int64_t *src) noexcept
    {
        v = _mm256_loadu_epi64(src);
    }

    void storeu(int64_t *dst) noexcept
    {
        _mm256_storeu_epi64(dst, v);
    }

public:
    __m256i v;
};

struct UINT64X4
{
public:
    using value_type = __m256i;

    UINT64X4() noexcept
    {
    }

    UINT64X4(__m256i other) noexcept :
        v{ other }
    {
    }

    UINT64X4(uint64_t value) noexcept :
        v{ _mm256_set1_epi64x(value) }
    {
    }

    UINT64X4(uint64_t _0, uint64_t _1, uint64_t _2, uint64_t _3) noexcept :
        v{ _mm256_set_epi64x(_3, _2, _1, _0) }
    {
    }

    UINT64X4  operator+(const UINT64X4 &other) noexcept
    {
        return _mm256_add_epi64(this->v, other.v);
    }

    UINT64X4  operator-(const UINT64X4 &other) noexcept
    {
        return _mm256_sub_epi64(this->v, other.v);
    }

    UINT64X4  operator/(const UINT64X4 &other) noexcept
    {
        return _mm256_div_epi64(this->v, other.v);
    }

    operator __m256i &() noexcept
    {
        return v;
    }

    operator const __m256i &() const noexcept
    {
        return v;
    }

    void load(const uint64_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint64_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint64_t *src) noexcept
    {
        v = _mm256_loadu_epi64(src);
    }

    void storeu(uint64_t *dst) noexcept
    {
        _mm256_storeu_epi64(dst, v);
    }

public:
    __m256i v;
};

struct INT8X64
{
public:
    using value_type = __m512i;

    INT8X64() noexcept
    {
    }

    INT8X64(__m512i other) noexcept :
        v{ other }
    {
    }

    INT8X64(int8_t value) noexcept :
        v{ _mm512_set1_epi8(value) }
    {
    }

    INT8X64  operator+(const INT8X64 &other) noexcept
    {
        return _mm512_add_epi8(this->v, other.v);
    }

    INT8X64  operator-(const INT8X64 &other) noexcept
    {
        return _mm512_sub_epi8(this->v, other.v);
    }

    INT8X64  operator/(const INT8X64 &other) noexcept
    {
        return _mm512_div_epi8(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const int8_t *src) noexcept
    {
        loadu(src);
    }

    void store(int8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int8_t *src) noexcept
    {
        v = _mm512_loadu_epi8(src);
    }

    void storeu(int8_t *dst) noexcept
    {
        _mm512_storeu_epi8(dst, v);
    }

public:
    __m512i v;
};

struct UINT8X64
{
public:
    using value_type = __m512i;

    UINT8X64() noexcept
    {
    }

    UINT8X64(__m512i other) noexcept :
        v{ other }
    {
    }

    UINT8X64(uint8_t value) noexcept :
        v{ _mm512_set1_epi8(value) }
    {
    }

    UINT8X64  operator+(const UINT8X64 &other) noexcept
    {
        return _mm512_add_epi8(this->v, other.v);
    }

    UINT8X64  operator-(const UINT8X64 &other) noexcept
    {
        return _mm512_sub_epi8(this->v, other.v);
    }

    UINT8X64  operator/(const UINT8X64 &other) noexcept
    {
        return _mm512_div_epi8(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const uint8_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint8_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint8_t *src) noexcept
    {
        v = _mm512_loadu_epi8(src);
    }

    void storeu(uint8_t *dst) noexcept
    {
        _mm512_storeu_epi8(dst, v);
    }

public:
    __m512i v;
};

struct INT16X32
{
public:
    using value_type = __m512i;

    INT16X32() noexcept
    {
    }

    INT16X32(__m512i other) noexcept :
        v{ other }
    {
    }

    INT16X32(int16_t value) noexcept :
        v{ _mm512_set1_epi16(value) }
    {
    }

    INT16X32  operator+(const INT16X32 &other) noexcept
    {
        return _mm512_add_epi16(this->v, other.v);
    }

    INT16X32  operator*(const INT16X32 &other) noexcept
    {
        return _mm512_mullo_epi16(this->v, other.v);
    }

    INT16X32  operator-(const INT16X32 &other) noexcept
    {
        return _mm512_sub_epi16(this->v, other.v);
    }

    INT16X32  operator/(const INT16X32 &other) noexcept
    {
        return _mm512_div_epi16(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const int16_t *src) noexcept
    {
        loadu(src);
    }

    void store(int16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const int16_t *src) noexcept
    {
        v = _mm512_loadu_epi16(src);
    }

    void storeu(int16_t *dst) noexcept
    {
        _mm512_storeu_epi16(dst, v);
    }

    __m256i cvt2uint8() const noexcept
    {
        return _mm512_cvtepi16_epi8(v);
    }

public:
    __m512i v;
};

struct UINT16X32
{
public:
    using value_type = __m512i;

    UINT16X32() noexcept
    {
    }

    UINT16X32(__m512i other) noexcept :
        v{ other }
    {
    }

    UINT16X32(uint16_t value) noexcept :
        v{ _mm512_set1_epi16(value) }
    {
    }

    UINT16X32  operator+(const UINT16X32 &other) noexcept
    {
        return _mm512_add_epi16(this->v, other.v);
    }

    UINT16X32  operator*(const UINT16X32 &other) noexcept
    {
        return _mm512_mullo_epi16(this->v, other.v);
    }

    UINT16X32  operator-(const UINT16X32 &other) noexcept
    {
        return _mm512_sub_epi16(this->v, other.v);
    }

    UINT16X32  operator/(const UINT16X32 &other) noexcept
    {
        return _mm512_div_epi16(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const uint16_t *src) noexcept
    {
        loadu(src);
    }

    void store(uint16_t *dst) noexcept
    {
        storeu(dst);
    }

    void loadu(const uint16_t *src) noexcept
    {
        v = _mm512_loadu_epi16(src);
    }

    void storeu(uint16_t *dst) noexcept
    {
        _mm512_storeu_epi16(dst, v);
    }

    __m256i cvt2uint8() const noexcept
    {
        return _mm512_cvtepi16_epi8(v);
    }

public:
    __m512i v;
};

struct INT32X16
{
public:
    using value_type = __m512i;

    INT32X16() noexcept
    {
    }

    INT32X16(__m512i other) noexcept :
        v{ other }
    {
    }

    INT32X16(int32_t value) noexcept :
        v{ _mm512_set1_epi32(value) }
    {
    }

    INT32X16  operator+(const INT32X16 &other) noexcept
    {
        return _mm512_add_epi32(this->v, other.v);
    }

    INT32X16  operator*(const INT32X16 &other) noexcept
    {
        return _mm512_mullo_epi32(this->v, other.v);
    }

    INT32X16  operator-(const INT32X16 &other) noexcept
    {
        return _mm512_sub_epi32(this->v, other.v);
    }

    INT32X16  operator/(const INT32X16 &other) noexcept
    {
        return _mm512_div_epi32(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const int32_t *src) noexcept
    {
        v = _mm512_load_epi32(src);
    }

    void store(int32_t *dst) noexcept
    {
        _mm512_store_epi32(dst, v);
    }

    void loadu(const int32_t *src) noexcept
    {
        v = _mm512_loadu_epi32(src);
    }

    void storeu(int32_t *dst) noexcept
    {
        _mm512_storeu_epi32(dst, v);
    }

public:
    __m512i v;
};

struct UINT32X16
{
public:
    using value_type = __m512i;

    UINT32X16() noexcept
    {
    }

    UINT32X16(__m512i other) noexcept :
        v{ other }
    {
    }

    UINT32X16(uint32_t value) noexcept :
        v{ _mm512_set1_epi32(value) }
    {
    }

    UINT32X16  operator+(const UINT32X16 &other) noexcept
    {
        return _mm512_add_epi32(this->v, other.v);
    }

    UINT32X16  operator*(const UINT32X16 &other) noexcept
    {
        return _mm512_mullo_epi32(this->v, other.v);
    }

    UINT32X16  operator-(const UINT32X16 &other) noexcept
    {
        return _mm512_sub_epi32(this->v, other.v);
    }

    UINT32X16  operator/(const UINT32X16 &other) noexcept
    {
        return _mm512_div_epi32(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const uint32_t *src) noexcept
    {
        v = _mm512_load_epi32(src);
    }

    void store(uint32_t *dst) noexcept
    {
        _mm512_store_epi32(dst, v);
    }

    void loadu(const uint32_t *src) noexcept
    {
        v = _mm512_loadu_epi32(src);
    }

    void storeu(uint32_t *dst) noexcept
    {
        _mm512_storeu_epi32(dst, v);
    }

public:
    __m512i v;
};

struct INT64X8
{
public:
    using value_type = __m512i;

    INT64X8() noexcept
    {
    }

    INT64X8(__m512i other) noexcept :
        v{ other }
    {
    }

    INT64X8(int64_t value) noexcept :
        v{ _mm512_set1_epi64(value) }
    {
    }

    INT64X8  operator+(const INT64X8 &other) noexcept
    {
        return _mm512_add_epi64(this->v, other.v);
    }

    INT64X8  operator-(const INT64X8 &other) noexcept
    {
        return _mm512_sub_epi64(this->v, other.v);
    }

    INT64X8  operator/(const INT64X8 &other) noexcept
    {
        return _mm512_div_epi64(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const int64_t *src) noexcept
    {
        v = _mm512_load_epi64(src);
    }

    void store(int64_t *dst) noexcept
    {
        _mm512_store_epi64(dst, v);
    }

    void loadu(const int64_t *src) noexcept
    {
        v = _mm512_loadu_epi64(src);
    }

    void storeu(int64_t *dst) noexcept
    {
        _mm512_storeu_epi64(dst, v);
    }

public:
    __m512i v;
};

struct UINT64X8
{
public:
    using value_type = __m512i;

    UINT64X8() noexcept
    {
    }

    UINT64X8(__m512i other) noexcept :
        v{ other }
    {
    }

    UINT64X8(uint64_t value) noexcept :
        v{ _mm512_set1_epi64(value) }
    {
    }

    UINT64X8  operator+(const UINT64X8 &other) noexcept
    {
        return _mm512_add_epi64(this->v, other.v);
    }

    UINT64X8  operator-(const UINT64X8 &other) noexcept
    {
        return _mm512_sub_epi64(this->v, other.v);
    }

    UINT64X8  operator/(const UINT64X8 &other) noexcept
    {
        return _mm512_div_epi64(this->v, other.v);
    }

    operator __m512i &() noexcept
    {
        return v;
    }

    operator const __m512i &() const noexcept
    {
        return v;
    }

    void load(const uint64_t *src) noexcept
    {
        v = _mm512_load_epi64(src);
    }

    void store(uint64_t *dst) noexcept
    {
        _mm512_store_epi64(dst, v);
    }

    void loadu(const uint64_t *src) noexcept
    {
        v = _mm512_loadu_epi64(src);
    }

    void storeu(uint64_t *dst) noexcept
    {
        _mm512_storeu_epi64(dst, v);
    }

public:
    __m512i v;
};

static inline FLOATX4 pow(const FLOATX4 &a, const FLOATX4 &b) noexcept
{
    return _mm_pow_ps(a, b);
}

static inline FLOATX4 sin(const FLOATX4 &a) noexcept
{
    return _mm_sin_ps(a);
}

static inline FLOATX4 cos(const FLOATX4 &a) noexcept
{
    return _mm_cos_ps(a);
}

static inline FLOATX4 asin(const FLOATX4 &a) noexcept
{
    return _mm_asin_ps(a);
}

static inline FLOATX4 acos(const FLOATX4 &a) noexcept
{
    return _mm_acos_ps(a);
}

static inline FLOATX4 min(const FLOATX4 &a, const FLOATX4 &b) noexcept
{
    return _mm_min_ps(a, b);
}

static inline FLOATX4 max(const FLOATX4 &a, const FLOATX4 &b) noexcept
{
    return _mm_max_ps(a, b);
}

static inline FLOATX8 pow(const FLOATX8 &a, const FLOATX8 &b) noexcept
{
    return _mm256_pow_ps(a, b);
}

static inline FLOATX8 sin(const FLOATX8 &a) noexcept
{
    return _mm256_sin_ps(a);
}

static inline FLOATX8 cos(const FLOATX8 &a) noexcept
{
    return _mm256_cos_ps(a);
}

static inline FLOATX8 asin(const FLOATX8 &a) noexcept
{
    return _mm256_asin_ps(a);
}

static inline FLOATX8 acos(const FLOATX8 &a) noexcept
{
    return _mm256_acos_ps(a);
}

static inline FLOATX8 min(const FLOATX8 &a, const FLOATX8 &b) noexcept
{
    return _mm256_min_ps(a, b);
}

static inline FLOATX8 max(const FLOATX8 &a, const FLOATX8 &b) noexcept
{
    return _mm256_max_ps(a, b);
}

static inline FLOATX16 pow(const FLOATX16 &a, const FLOATX16 &b) noexcept
{
    return _mm512_pow_ps(a, b);
}

static inline FLOATX16 sin(const FLOATX16 &a) noexcept
{
    return _mm512_sin_ps(a);
}

static inline FLOATX16 cos(const FLOATX16 &a) noexcept
{
    return _mm512_cos_ps(a);
}

static inline FLOATX16 asin(const FLOATX16 &a) noexcept
{
    return _mm512_asin_ps(a);
}

static inline FLOATX16 acos(const FLOATX16 &a) noexcept
{
    return _mm512_acos_ps(a);
}

static inline FLOATX16 min(const FLOATX16 &a, const FLOATX16 &b) noexcept
{
    return _mm512_min_ps(a, b);
}

static inline FLOATX16 max(const FLOATX16 &a, const FLOATX16 &b) noexcept
{
    return _mm512_max_ps(a, b);
}

static inline DOUBLEX2 min(const DOUBLEX2 &a, const DOUBLEX2 &b) noexcept
{
    return _mm_min_pd(a, b);
}

static inline DOUBLEX2 max(const DOUBLEX2 &a, const DOUBLEX2 &b) noexcept
{
    return _mm_max_pd(a, b);
}

static inline DOUBLEX4 min(const DOUBLEX4 &a, const DOUBLEX4 &b) noexcept
{
    return _mm256_min_pd(a, b);
}

static inline DOUBLEX4 max(const DOUBLEX4 &a, const DOUBLEX4 &b) noexcept
{
    return _mm256_max_pd(a, b);
}

static inline DOUBLEX8 min(const DOUBLEX8 &a, const DOUBLEX8 &b) noexcept
{
    return _mm512_min_pd(a, b);
}

static inline DOUBLEX8 max(const DOUBLEX8 &a, const DOUBLEX8 &b) noexcept
{
    return _mm512_max_pd(a, b);
}

static inline INT8X16 min(const INT8X16 &a, const INT8X16 &b) noexcept
{
    return _mm_min_epi8(a, b);
}

static inline INT8X16 max(const INT8X16 &a, const INT8X16 &b) noexcept
{
    return _mm_max_epi8(a, b);
}

static inline UINT8X16 min(const UINT8X16 &a, const UINT8X16 &b) noexcept
{
    return _mm_min_epi8(a, b);
}

static inline UINT8X16 max(const UINT8X16 &a, const UINT8X16 &b) noexcept
{
    return _mm_max_epi8(a, b);
}

static inline INT16X8 min(const INT16X8 &a, const INT16X8 &b) noexcept
{
    return _mm_min_epi16(a, b);
}

static inline INT16X8 max(const INT16X8 &a, const INT16X8 &b) noexcept
{
    return _mm_max_epi16(a, b);
}

static inline UINT16X8 min(const UINT16X8 &a, const UINT16X8 &b) noexcept
{
    return _mm_min_epi16(a, b);
}

static inline UINT16X8 max(const UINT16X8 &a, const UINT16X8 &b) noexcept
{
    return _mm_max_epi16(a, b);
}

static inline INT32X4 min(const INT32X4 &a, const INT32X4 &b) noexcept
{
    return _mm_min_epi32(a, b);
}

static inline INT32X4 max(const INT32X4 &a, const INT32X4 &b) noexcept
{
    return _mm_max_epi32(a, b);
}

static inline UINT32X4 min(const UINT32X4 &a, const UINT32X4 &b) noexcept
{
    return _mm_min_epi32(a, b);
}

static inline UINT32X4 max(const UINT32X4 &a, const UINT32X4 &b) noexcept
{
    return _mm_max_epi32(a, b);
}

static inline INT64X2 min(const INT64X2 &a, const INT64X2 &b) noexcept
{
    return _mm_min_epi64(a, b);
}

static inline INT64X2 max(const INT64X2 &a, const INT64X2 &b) noexcept
{
    return _mm_max_epi64(a, b);
}

static inline UINT64X2 min(const UINT64X2 &a, const UINT64X2 &b) noexcept
{
    return _mm_min_epi64(a, b);
}

static inline UINT64X2 max(const UINT64X2 &a, const UINT64X2 &b) noexcept
{
    return _mm_max_epi64(a, b);
}

static inline INT8X32 min(const INT8X32 &a, const INT8X32 &b) noexcept
{
    return _mm256_min_epi8(a, b);
}

static inline INT8X32 max(const INT8X32 &a, const INT8X32 &b) noexcept
{
    return _mm256_max_epi8(a, b);
}

static inline UINT8X32 min(const UINT8X32 &a, const UINT8X32 &b) noexcept
{
    return _mm256_min_epi8(a, b);
}

static inline UINT8X32 max(const UINT8X32 &a, const UINT8X32 &b) noexcept
{
    return _mm256_max_epi8(a, b);
}

static inline INT16X16 min(const INT16X16 &a, const INT16X16 &b) noexcept
{
    return _mm256_min_epi16(a, b);
}

static inline INT16X16 max(const INT16X16 &a, const INT16X16 &b) noexcept
{
    return _mm256_max_epi16(a, b);
}

static inline UINT16X16 min(const UINT16X16 &a, const UINT16X16 &b) noexcept
{
    return _mm256_min_epi16(a, b);
}

static inline UINT16X16 max(const UINT16X16 &a, const UINT16X16 &b) noexcept
{
    return _mm256_max_epi16(a, b);
}

static inline INT32X8 min(const INT32X8 &a, const INT32X8 &b) noexcept
{
    return _mm256_min_epi32(a, b);
}

static inline INT32X8 max(const INT32X8 &a, const INT32X8 &b) noexcept
{
    return _mm256_max_epi32(a, b);
}

static inline UINT32X8 min(const UINT32X8 &a, const UINT32X8 &b) noexcept
{
    return _mm256_min_epi32(a, b);
}

static inline UINT32X8 max(const UINT32X8 &a, const UINT32X8 &b) noexcept
{
    return _mm256_max_epi32(a, b);
}

static inline INT64X4 min(const INT64X4 &a, const INT64X4 &b) noexcept
{
    return _mm256_min_epi64(a, b);
}

static inline INT64X4 max(const INT64X4 &a, const INT64X4 &b) noexcept
{
    return _mm256_max_epi64(a, b);
}

static inline UINT64X4 min(const UINT64X4 &a, const UINT64X4 &b) noexcept
{
    return _mm256_min_epi64(a, b);
}

static inline UINT64X4 max(const UINT64X4 &a, const UINT64X4 &b) noexcept
{
    return _mm256_max_epi64(a, b);
}

static inline INT8X64 min(const INT8X64 &a, const INT8X64 &b) noexcept
{
    return _mm512_min_epi8(a, b);
}

static inline INT8X64 max(const INT8X64 &a, const INT8X64 &b) noexcept
{
    return _mm512_max_epi8(a, b);
}

static inline UINT8X64 min(const UINT8X64 &a, const UINT8X64 &b) noexcept
{
    return _mm512_min_epi8(a, b);
}

static inline UINT8X64 max(const UINT8X64 &a, const UINT8X64 &b) noexcept
{
    return _mm512_max_epi8(a, b);
}

static inline INT16X32 min(const INT16X32 &a, const INT16X32 &b) noexcept
{
    return _mm512_min_epi16(a, b);
}

static inline INT16X32 max(const INT16X32 &a, const INT16X32 &b) noexcept
{
    return _mm512_max_epi16(a, b);
}

static inline UINT16X32 min(const UINT16X32 &a, const UINT16X32 &b) noexcept
{
    return _mm512_min_epi16(a, b);
}

static inline UINT16X32 max(const UINT16X32 &a, const UINT16X32 &b) noexcept
{
    return _mm512_max_epi16(a, b);
}

static inline INT32X16 min(const INT32X16 &a, const INT32X16 &b) noexcept
{
    return _mm512_min_epi32(a, b);
}

static inline INT32X16 max(const INT32X16 &a, const INT32X16 &b) noexcept
{
    return _mm512_max_epi32(a, b);
}

static inline UINT32X16 min(const UINT32X16 &a, const UINT32X16 &b) noexcept
{
    return _mm512_min_epi32(a, b);
}

static inline UINT32X16 max(const UINT32X16 &a, const UINT32X16 &b) noexcept
{
    return _mm512_max_epi32(a, b);
}

static inline INT64X8 min(const INT64X8 &a, const INT64X8 &b) noexcept
{
    return _mm512_min_epi64(a, b);
}

static inline INT64X8 max(const INT64X8 &a, const INT64X8 &b) noexcept
{
    return _mm512_max_epi64(a, b);
}

static inline UINT64X8 min(const UINT64X8 &a, const UINT64X8 &b) noexcept
{
    return _mm512_min_epi64(a, b);
}

static inline UINT64X8 max(const UINT64X8 &a, const UINT64X8 &b) noexcept
{
    return _mm512_max_epi64(a, b);
}

template <class T>
concept IntrinsicType = (
    std::is_same_v<T, FLOATX4> ||
    std::is_same_v<T, FLOATX8> ||
    std::is_same_v<T, FLOATX16> ||
    std::is_same_v<T, DOUBLEX2> ||
    std::is_same_v<T, DOUBLEX4> ||
    std::is_same_v<T, DOUBLEX8> ||
    std::is_same_v<T, INT8X16> ||
    std::is_same_v<T, UINT8X16> ||
    std::is_same_v<T, INT16X8> ||
    std::is_same_v<T, UINT16X8> ||
    std::is_same_v<T, INT32X4> ||
    std::is_same_v<T, UINT32X4> ||
    std::is_same_v<T, INT64X2> ||
    std::is_same_v<T, UINT64X2> ||
    std::is_same_v<T, INT8X32> ||
    std::is_same_v<T, UINT8X32> ||
    std::is_same_v<T, INT16X16> ||
    std::is_same_v<T, UINT16X16> ||
    std::is_same_v<T, INT32X8> ||
    std::is_same_v<T, UINT32X8> ||
    std::is_same_v<T, INT64X4> ||
    std::is_same_v<T, UINT64X4> ||
    std::is_same_v<T, INT8X64> ||
    std::is_same_v<T, UINT8X64> ||
    std::is_same_v<T, INT16X32> ||
    std::is_same_v<T, UINT16X32> ||
    std::is_same_v<T, INT32X16> ||
    std::is_same_v<T, UINT32X16> ||
    std::is_same_v<T, INT64X8> ||
    std::is_same_v<T, UINT64X8>
);

template <IntrinsicType T>
static inline T clip(const T &a, const T &b, const T &c) noexcept
{
    return min(max(a, b), c);
}

#endif
