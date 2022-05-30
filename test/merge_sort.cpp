#include <immintrin.h> 


#include <iostream>
typedef uint32_t index_t;

// _mm256_load_si256
// _mm256_store_si256

inline __m256i load_reg(index_t* ptr) {
  return _mm256_load_si256((__m256i *)ptr);
}

inline void store_reg(index_t* ptr, __m256i reg) {
  _mm256_store_si256((__m256i *)ptr, reg);
}


void print_vec(__m256i a) {
    index_t data[8];
    store_reg(data, a);
    for (int i = 0; i < 8; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}


void merge_two_regs(__m256i& a, __m256i& b) {
    __m256i rev_mask = _mm256_set_epi32(0,1,2,3,4,5,6,7); 
    __m256i b_rev = _mm256_permutevar8x32_epi32(b, rev_mask);


    __m256i min = _mm256_min_epi32(a, b_rev);
    __m256i max = _mm256_max_epi32(a, b_rev);


    a = min;
    b = max;
}






int main() {


    __m256i a = _mm256_set_epi32(1,3,5,7,9,10,11,18); 
    __m256i b = _mm256_set_epi32(0,2,4,6,8,12,13,14);
    merge_two_regs(a,b);
    print_vec(a);
    print_vec(b);

    std::cout << "Haloo"<< std::endl;
}