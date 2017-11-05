/*
 * AARR-check.c
 *
 *  Created on: 21 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include <check.h>
#include <stdio.h>

#define TYPE    int
#define NAMESPACE(x)   INT_ ## x
#include "vector.t.c"

#define printd(str, ...) fprintf(stderr, "%d:%d: " str "\n", __FUNCTION__, __LINE__, ## __VA_ARGS__)

START_TEST(test_insert_range)
{
	int a[] = {1,2,3,4};
	INT_VECTOR *v = INT_vector();
	INT_insert_range(v, INT_end(v), a, 4);
	ck_assert_int_eq(*INT_at(v,0), 1);
	ck_assert_int_eq(*INT_at(v,1), 2);
	ck_assert_int_eq(*INT_at(v,2), 3);
	ck_assert_int_eq(*INT_at(v,3), 4);
	ck_assert_int_eq(INT_size(v), 4);

	INT_insert_range(v, INT_at(v,1), (int[]){9,10}, 2);
	ck_assert_int_eq(*INT_at(v,0), 1);
	ck_assert_int_eq(*INT_at(v,1), 9);
	ck_assert_int_eq(*INT_at(v,2), 10);
	ck_assert_int_eq(*INT_at(v,3), 2);
	ck_assert_int_eq(*INT_at(v,4), 3);
	ck_assert_int_eq(*INT_at(v,5), 4);
	ck_assert_int_eq(INT_size(v), 6);

	INT_vector_destructor(&v);
} END_TEST

START_TEST(test_erase_range)
{
	int a[] = {1,2,3,4};
	INT_VECTOR *v = INT_vector();
	INT_insert_range(v, INT_end(v), a, 4);
	ck_assert_int_eq(*INT_at(v,0), 1);
	ck_assert_int_eq(*INT_at(v,1), 2);
	ck_assert_int_eq(*INT_at(v,2), 3);
	ck_assert_int_eq(*INT_at(v,3), 4);
	ck_assert_int_eq(INT_size(v), 4);

	INT_erase_range(v, INT_at(v,1), 2);
	ck_assert_int_eq(*INT_at(v,0), 1);
	ck_assert_int_eq(*INT_at(v,1), 4);
	ck_assert_int_eq(INT_size(v), 2);

	INT_vector_destructor(&v);
} END_TEST

START_TEST(test_for)
{
	int a[] = {1,2,3,4};
	INT_VECTOR *v = INT_vector();
	INT_insert_range(v, INT_end(v), a, 4);

	for(int * i = INT_begin(v); i != INT_end(v); ++i) {
		printf("%d \n", *i);
	}

	INT_vector_destructor(&v);
} END_TEST

START_TEST(test_init)
{
	INT_VECTOR *v = INT_vector();

	int a[] = {1,2,3,4};
	INT_insert_range(v, INT_end(v), a, 4);
	ck_assert_int_eq(*INT_at(v,0), 1);
	ck_assert_int_eq(*INT_at(v,1), 2);
	ck_assert_int_eq(*INT_at(v,2), 3);
	ck_assert_int_eq(*INT_at(v,3), 4);

	INT_vector_destructor(&v);
} END_TEST

START_TEST(test_size)
{
	INT_VECTOR *v;
	v = INT_vector();
	ck_assert_int_eq(INT_size(v), 0);
	INT_push_back(v, (int[]){10});
	ck_assert_int_eq(INT_size(v), 1);
	INT_push_back(v, (int[]){10});
	ck_assert_int_eq(INT_size(v), 2);
	INT_vector_destructor(&v);
} END_TEST

START_TEST(test_vector_erase)
{
	INT_VECTOR *myvector = INT_vector();

	// set some values (from 1 to 10)
	for (int i=1; i<=10; i++) INT_push_back(myvector, &i);

	ck_assert_int_eq(INT_size(myvector), 10);
	for (int i=1; i<=10; i++)
		ck_assert_int_eq(*INT_at(myvector,i-1), i);

	// erase the 6th element
	INT_erase(myvector, INT_at(myvector,5));

	// erase the first 3 elements:
	INT_erase_range(myvector, INT_begin(myvector), 3);

	int res[] = { 4, 5, 7, 8, 9, 10};
	for (unsigned i=0; i<INT_size(myvector); ++i)
		ck_assert_int_eq(*(int*)INT_at(myvector,i), res[i]);
	ck_assert_int_eq(INT_size(myvector), 6);

	INT_vector_destructor(&myvector);
} END_TEST

int main(void)
{
	Suite *s = suite_create("Core");
    TCase *tc = tcase_create("Core");
    SRunner *sr = srunner_create(s);
    int nf;

    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_insert_range);
    tcase_add_test(tc, test_erase_range);
    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_vector_erase);
    tcase_add_test(tc, test_size);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
