/*
 * AARR-check.c
 *
 *  Created on: 21 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include "AARR.h"
#include "AARR-specs.h"
#include <check.h>
#include <stdio.h>

AARR_DECLARE_TYPE_compar(int)
AARR_DECLARE_TYPE_max_element(int)
AARR_DECLARE_TYPE_min_element(int)
AARR_DECLARE_TYPE_minmax_s(int)
AARR_DECLARE_TYPE_minmax_element(int)

START_TEST(test_minmax_min_max_element)
{
	int a[] = { 5,3,1,2,4 };
	ck_assert_int_eq(*int_max_element(a, sizeof(a)/sizeof(*a), int_compar), 5);
	ck_assert_int_eq(*int_min_element(a, sizeof(a)/sizeof(*a), int_compar), 1);
	struct int_minmax_s result = int_minmax_element(a, sizeof(a)/sizeof(*a), int_compar);
	ck_assert_int_eq(*result.max, 5);
	ck_assert_int_eq(*result.min, 1);
}
END_TEST

int main(void)
{
	Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_minmax_min_max_element);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
