#include <immintrin.h>  // AVX intrinsics
# include <stdio.h>

void bitonic_swap(__m256 values, __m256i indices, __m256 mask, __m256i perm) ;

// Helper function to perform a bitonic swap
inline void bitonic_swap(__m256 values, __m256i indices, __m256 mask, __m256i perm) {
    __m256 v_perm = _mm256_permutevar_ps(values, perm);  // Shuffle values
    __m256i i_perm = _mm256_permutevar8x32_epi32(indices, perm);  // Shuffle indices

    __m256 min_vals = _mm256_min_ps(values, v_perm);  // Find min values
    __m256 max_vals = _mm256_max_ps(values, v_perm);  // Find max values

    __m256i min_inds = _mm256_blendv_epi8(indices, i_perm, _mm256_castps_si256(mask));
    __m256i max_inds = _mm256_blendv_epi8(i_perm, indices, _mm256_castps_si256(mask));

    // Store sorted values and indices
    values = _mm256_blendv_ps(max_vals, min_vals, mask);
    indices = _mm256_blendv_epi8(max_inds, min_inds, _mm256_castps_si256(mask));
}

// AVX Bitonic Sort for 8 floats
void bitonic_sort(__m256 v_values, __m256i v_indices) {
    const __m256i idx_identity = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    v_indices = idx_identity;  // Initialize indices

    // Bitonic sorting network for 8 elements
    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permute_ps(v_values, 0b10110001), _CMP_GT_OS), _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1));
    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permute_ps(v_values, 0b01001110), _CMP_GT_OS), _mm256_set_epi32(5, 4, 7, 6, 1, 0, 3, 2));
    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permute_ps(v_values, 0b11110000), _CMP_GT_OS), _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4));

    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permutevar8x32_ps(v_values, _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4)), _CMP_GT_OS), _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4));
    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permutevar8x32_ps(v_values, _mm256_set_epi32(5, 4, 7, 6, 1, 0, 3, 2)), _CMP_GT_OS), _mm256_set_epi32(5, 4, 7, 6, 1, 0, 3, 2));
    bitonic_swap(v_values, v_indices, _mm256_cmp_ps(v_values, _mm256_permutevar8x32_ps(v_values, _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1)), _CMP_GT_OS), _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1));
}

int main() {
    // Original values
    float values[8] = {5.2f, 1.3f, 8.7f, 3.6f, 7.1f, 2.4f, 6.5f, 4.8f};

    // Load values into AVX registers
    __m256 v_values = _mm256_loadu_ps(values);
    __m256i v_indices = _mm256_setzero_si256();  // Initialize index vector

    bitonic_sort(v_values, v_indices);  // Perform SIMD Bitonic Sort

    // Store sorted results
    float sorted_values[8];
    int sorted_indices[8];

    _mm256_storeu_ps(sorted_values, v_values);
    _mm256_storeu_si256((__m256i*)sorted_indices, v_indices);

    // Print sorted values
    printf("Sorted Values: ");
    for (int i = 0; i < 8; i++) {
        printf("%f ", sorted_values[i]);
    }
    printf("\n");

    // Print sorted indices
    printf("Sorted Indices: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", sorted_indices[i]);
    }
    printf("\n");

    return 0;
}

