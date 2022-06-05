
#ifndef __INSTRUMENTED_INTRINSICS_H__
#define __INSTRUMENTED_INTRINSICS_H__


#ifndef INSTRUMENTED
#include <immintrin.h>
#else
#include <instrumented_index.h>

namespace real_intrinsics {
    #include <immintrin.h>
}

namespace mocked_intrinsics {

typedef real_intrinsics::__m256i __m256i;
typedef real_intrinsics::__m128i __m128i;
auto static inline _mm256_set_epi32(auto i_0, auto i_1, auto i_2, auto i_3, auto i_4, auto i_5, auto i_6, auto i_7) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_set_epi32(i_0, i_1, i_2, i_3, i_4, i_5, i_6, i_7);
}
auto static inline _mm256_set1_epi32(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_set1_epi32(i_0);
}
auto static inline _mm256_setzero_si256() {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_setzero_si256();
}
auto static inline _mm256_movemask_epi8(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_movemask_epi8(i_0);
}
auto static inline _mm_set1_epi32(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_set1_epi32(i_0);
}
auto static inline _mm_load_si128(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_load_si128(i_0);
}
auto static inline _mm_cmpeq_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm_cmpeq_epi32(i_0, i_1);
}
auto static inline _mm_setzero_si128() {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_setzero_si128();
}
auto static inline _mm_movemask_epi8(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_movemask_epi8(i_0);
}
auto static inline _mm256_permutevar8x32_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_permutevar8x32_epi32(i_0, i_1);
}
auto static inline _mm256_permute2f128_si256(auto i_0, auto i_1, auto i_2) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_permute2f128_si256(i_0, i_1, i_2);
}
auto static inline _mm256_min_epu32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_min_epu32(i_0, i_1);
}
auto static inline _mm256_max_epu32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_max_epu32(i_0, i_1);
}
auto static inline _mm256_blend_epi32(auto i_0, auto i_1, auto i_2) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_blend_epi32(i_0, i_1, i_2);
}
auto static inline _mm256_set_epi64x(auto i_0, auto i_1, auto i_2, auto i_3) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_set_epi64x(i_0, i_1, i_2, i_3);
}
auto static inline _mm256_set1_epi64x(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_set1_epi64x(i_0);
}
auto static inline _mm256_store_si256(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
real_intrinsics::_mm256_store_si256(i_0, i_1);
}
auto static inline _mm256_cmpgt_epi64(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm256_cmpgt_epi64(i_0, i_1);
}
auto static inline _mm256_cmpeq_epi64(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm256_cmpeq_epi64(i_0, i_1);
}
auto static inline _mm256_and_si256(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_and_si256(i_0, i_1);
}
auto static inline _mm256_or_si256(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_or_si256(i_0, i_1);
}
auto static inline _mm256_andnot_si256(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(16);
    return real_intrinsics::_mm256_andnot_si256(i_0, i_1);
}
auto static inline _mm256_add_epi64(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm256_add_epi64(i_0, i_1);
}
auto static inline _mm256_cmpgt_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_cmpgt_epi32(i_0, i_1);
}
auto static inline _mm256_cmpeq_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_cmpeq_epi32(i_0, i_1);
}
auto static inline _mm256_add_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(8);
    return real_intrinsics::_mm256_add_epi32(i_0, i_1);
}
auto static inline _mm256_i32gather_epi32(auto i_0, auto i_1, auto i_2) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_i32gather_epi32(i_0, i_1, i_2);
}
auto static inline _mm_add_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm_add_epi32(i_0, i_1);
}
auto static inline _mm_or_si128(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm_or_si128(i_0, i_1);
}
auto static inline _mm256_castsi256_si128(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_castsi256_si128(i_0);
}
auto static inline _mm256_extracti128_si256(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_extracti128_si256(i_0, i_1);
}
auto static inline _mm_unpackhi_epi64(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_unpackhi_epi64(i_0, i_1);
}
auto static inline _mm_shuffle_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_shuffle_epi32(i_0, i_1);
}
auto static inline _mm_cvtsi128_si32(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_cvtsi128_si32(i_0);
}
auto static inline _mm_loadu_si128(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_loadu_si128(i_0);
}
auto static inline _mm_unpacklo_epi16(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_unpacklo_epi16(i_0, i_1);
}
auto static inline _mm_unpackhi_epi16(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm_unpackhi_epi16(i_0, i_1);
}
auto static inline _mm256_castsi128_si256(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_castsi128_si256(i_0);
}
auto static inline _mm256_inserti128_si256(auto i_0, auto i_1, auto i_2) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_inserti128_si256(i_0, i_1, i_2);
}
auto static inline _mm256_permute4x64_epi64(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_permute4x64_epi64(i_0, i_1);
}
auto static inline _mm256_broadcastsi128_si256(auto i_0) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_broadcastsi128_si256(i_0);
}
auto static inline _mm256_shufflelo_epi16(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_shufflelo_epi16(i_0, i_1);
}
auto static inline _mm256_unpacklo_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_unpacklo_epi32(i_0, i_1);
}
auto static inline _mm256_cmpeq_epi16(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(16);
    return real_intrinsics::_mm256_cmpeq_epi16(i_0, i_1);
}

}


using namespace mocked_intrinsics;
#endif

#endif

