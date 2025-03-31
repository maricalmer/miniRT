#include <CUnit/Basic.h>
#include "minirt.h"

/* Test init_data function */
void test_init_data() {
    t_data data;
    init_data(&data);

    CU_ASSERT_EQUAL(data.n_obj, 0);
    CU_ASSERT_EQUAL(data.n_light, 0);
    CU_ASSERT_EQUAL(data.n_obj_files, 0);
    CU_ASSERT_EQUAL(data.objects_idx, 0);
    CU_ASSERT_EQUAL(data.obj_fd, -1);
    CU_ASSERT_EQUAL(data.rt_fd, -1);
    CU_ASSERT_EQUAL(data.exit, 0);
    CU_ASSERT_EQUAL(data.anti_fa, ANTIALIASING_FACT);
    CU_ASSERT_EQUAL(data.mouse_pressed_l, 0);
    CU_ASSERT_EQUAL(data.mouse_pressed_r, 0);
    CU_ASSERT_EQUAL(data.cam.mode, 0);
}

/* Test input checking */
void test_check_input() {
    t_data data;
    char *valid_args[] = {"./miniRT", "scenes/test_scene.rt"};
    char *invalid_args[] = {"./miniRT"};

    CU_ASSERT_EQUAL(check_input(2, valid_args, &data), EXIT_SUCCESS);
    CU_ASSERT_EQUAL(check_input(1, invalid_args, &data), EXIT_FAILURE);
}

/* Register tests */
int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("MiniRT Tests", NULL, NULL);

    CU_add_test(suite, "test_init_data", test_init_data);
    CU_add_test(suite, "test_check_input", test_check_input);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
