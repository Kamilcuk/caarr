/*
 * AARR-check.c
 *
 *  Created on: 21 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include <check.h>
#include <stdio.h>

#define VECTOR_TYPE    int
#define NAMESPACE(x)   INT_ ## x
#include "template_"
#include "void_vector.h"

//START_TEST(test_minmax_min_max_element)
//{
//	int a[] = { 5,3,1,2,4 };
//	ck_assert_int_eq(*int_max_element(a, sizeof(a)/sizeof(*a), int_compar), 5);
//	ck_assert_int_eq(*int_min_element(a, sizeof(a)/sizeof(*a), int_compar), 1);
//	struct int_minmax_s result = int_minmax_element(a, sizeof(a)/sizeof(*a), int_compar);
//	ck_assert_int_eq(*result.max, 5);
//	ck_assert_int_eq(*result.min, 1);
//}
//END_TEST

START_TEST(test_init)
{
	VOID_VECTOR *v = VOID_vector(sizeof(int));

	VOID_emplace_back(v, (int[]){1,2,3,4}, 4);
	ck_assert_int_eq(*(int*)VOID_at(v,0), 1);
	ck_assert_int_eq(*(int*)VOID_at(v,1), 2);
	ck_assert_int_eq(*(int*)VOID_at(v,2), 3);
	ck_assert_int_eq(*(int*)VOID_at(v,3), 4);

	VOID_vector_desctructor(&v);
} END_TEST

START_TEST(test_vector_erase)
{
	VOID_VECTOR *myvector = VOID_vector(sizeof(int));

	// set some values (from 1 to 10)
	for (int i=1; i<=10; i++) VOID_push_back(myvector, &i);

	ck_assert_int_eq(VOID_size(myvector), 10);
	for (int i=1; i<=10; i++)
		ck_assert_int_eq(*(int*)VOID_at(myvector,i), i);

	// erase the 6th element
	VOID_erase(myvector, VOID_at(myvector,5));

	// erase the first 3 elements:
	VOID_erase_range(myvector, VOID_begin(myvector), VOID_at(myvector, 3));

	int res[] = { 4, 5, 7, 8, 9, 10};
	for (unsigned i=0; i<VOID_size(myvector); ++i)
		ck_assert_int_eq(*(int*)VOID_at(myvector,i), res[i]);
	ck_assert_int_eq(VOID_size(myvector), 6);

} END_TEST

int main(void)
{
	Suite *s = suite_create("Core");
    TCase *tc = tcase_create("Core");
    SRunner *sr = srunner_create(s);
    int nf;

    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_vector_erase);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
