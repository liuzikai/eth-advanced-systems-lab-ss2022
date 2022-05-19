
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


typedef real_intrinsics::__m256i __m256i;
auto static inline _mm256_set_epi32(auto i_0, auto i_1, auto i_2, auto i_3, auto i_4, auto i_5, auto i_6, auto i_7) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_set_epi32(i_0, i_1, i_2, i_3, i_4, i_5, i_6, i_7);
}
auto static inline _mm256_permutevar8x32_epi32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_permutevar8x32_epi32(i_0, i_1);
}
auto static inline _mm256_min_epu32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(0);
    return real_intrinsics::_mm256_min_epu32(i_0, i_1);
}
auto static inline _mm256_max_epu32(auto i_0, auto i_1) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm256_max_epu32(i_0, i_1);
}
auto static inline _mm256_blend_epi32(auto i_0, auto i_1, auto i_2) {
    AVX2::increment_op_count_by(4);
    return real_intrinsics::_mm256_blend_epi32(i_0, i_1, i_2);
}

}


using namespace mocked_intrinsics;
#endif

#endif

