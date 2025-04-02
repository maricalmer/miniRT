#include <CUnit/Basic.h>
#include "minirt.h"
#include "data.h"
#include "functions.h"
#include "settings.h"

void test_dot_inplace_34_13() {
    double a[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    float b[3] = {1, 1, 1};
    
    // Call the function
    dot_inplace_34_13(a, b);

    // Assert that the results match the expected values
    CU_ASSERT_DOUBLE_EQUAL(b[0], 10.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[1], 26.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[2], 42.0, 0.0001);
}

void test_dot_inplace_33_13() {
    float a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float b[3] = {1, 1, 1};
    
    // Call the function
    dot_inplace_33_13(a, b);

    // Assert that the results match the expected values
    CU_ASSERT_DOUBLE_EQUAL(b[0], 6.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[1], 15.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[2], 24.0, 0.0001);
}

void test_dot_inplace_33_33() {
    float a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    float b[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    
    // Call the function
    dot_inplace_33_33(a, b);

    // Assert that the results match the expected values
    CU_ASSERT_DOUBLE_EQUAL(b[0][0], 30.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[0][1], 24.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[0][2], 16.0, 0.0001);

    CU_ASSERT_DOUBLE_EQUAL(b[1][0], 78.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[1][1], 64.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[1][2], 48.0, 0.0001);

    CU_ASSERT_DOUBLE_EQUAL(b[2][0], 126.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[2][1], 102.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b[2][2], 85.0, 0.0001);
}

int main() {
    CU_initialize_registry();
    
    CU_pSuite suite = CU_add_suite("Math Tests", NULL, NULL);
    CU_add_test(suite, "test_dot_inplace_34_13", test_dot_inplace_34_13);
    CU_add_test(suite, "test_dot_inplace_33_13", test_dot_inplace_33_13);
    CU_add_test(suite, "test_dot_inplace_33_33", test_dot_inplace_33_33);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
