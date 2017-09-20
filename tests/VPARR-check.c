/*
 * ecf-check.c
 *
 *  Created on: 18 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include <VPARR.h>
#include <check.h>
#include <stdio.h>

void VPARR_int_printf(VPARR *arr)
{
	printf("array printf %lu=", arr->base);
	VPARR_FOR_RANGE_BASED(base, arr) {
		printf(" %d", *(int*)base);
	}
	printf("\n");
}
int compar_int(const void *a0, const void *b0)
{
	const int *a = a0, *b = b0;
	return *a > *b ? 1 : -1;
}

bool IsOdd (void *a) {
  return (((*(int*)a)%2)==1);
}

START_TEST(test_VPARR_merge)
{
	// http://www.cplusplus.com/reference/algorithm/merge/
	int first0[] = {5,10,15,20,25};
	VPARR first = VPARR_INIT_FROM_STATIC(first0);
	int second0[] = {50,40,30,20,10};
	VPARR second = VPARR_INIT_FROM_STATIC(second0);
	int v0[10];
	VPARR v = VPARR_INIT_FROM_STATIC(v0);

	VPARR_sort(&first, compar_int);
	VPARR_int_printf(&first);
	VPARR_sort(&second, compar_int);
	VPARR_int_printf(&second);
	VPARR_merge(&first, &second, &v, compar_int);

	printf("The resulting vector contains:");
	VPARR_FOR_RANGE_BASED(base, &v) {
		printf(" %d", *(int*)base);
	}
	printf("\n");


	int res_v0[10] = {5,10,10,15,20,20,25,30,40,50};
	_ck_assert_int(10, ==, v.nmemb);
	VPARR_FOR_BEGIN(i, base, &v) {
		_ck_assert_int(res_v0[i], ==, *(int*)base);
	} VPARR_FOR_END;
	printf("\n");
}
END_TEST

START_TEST(test_VPARR_find)
{
	// http://www.cplusplus.com/reference/algorithm/find/
	// using std::find with array and pointer:
	int myints0[] = { 10, 20, 30, 40 };
	VPARR myints = VPARR_INIT_FROM_STATIC(myints0);
	int *p;

	int elem = 30;
	p = VPARR_find(&myints, &elem);
	if (p != NULL)
		printf("Element found in myints: %d \n", *(int*)p);
	else
		printf("Element not found in myints\n");

	ck_assert_msg(p == &myints0[2], "p == &myints0[2] error");
}
END_TEST

START_TEST(test_VPARR_find_if)
{
	VPARR myvector = VPARR_INIT_TYPE(int);
	int elem;
	elem = 10; VPARR_push_back(&myvector, &elem);
	elem = 25; VPARR_push_back(&myvector, &elem);
	elem = 40; VPARR_push_back(&myvector, &elem);
	elem = 55; VPARR_push_back(&myvector, &elem);

	void *p = VPARR_find_if(&myvector, IsOdd);
	printf("The first odd value if %d \n", *(int*)p);

	ck_assert(myvector.size == sizeof(int));
	ck_assert(myvector.nmemb == 4);
	ck_assert(((int*)myvector.base)[0] == 10);
	ck_assert(((int*)myvector.base)[1] == 25);
	ck_assert(((int*)myvector.base)[2] == 40);
	ck_assert(((int*)myvector.base)[3] == 55);
	ck_assert(!memcmp(myvector.base, (int[4]){10,25,40,55}, 4 * myvector.size));
	ck_assert(p == myvector.base+1*myvector.size);

	VPARR_destruct(&myvector);
}
END_TEST

START_TEST(test_VPARR_copy)
{
	int myints[]={10,20,30,40,50,60,70};
	VPARR myvector = VPARR_ALLOC_TYPE(int,7);
	if ( myvector.base == NULL ) {
		printf("Out of memory!");
		ck_assert(false);
		return;
	}

	VPARR_memcopy(&myvector, myints, 7);

	printf("myvector contains:");
	VPARR_FOR_RANGE_BASED(base, &myvector) {
		printf(" %d", *(int*)base);
	}
	printf("\n");

	ck_assert(myvector.size == sizeof(int));
	ck_assert(myvector.nmemb == 7);
	ck_assert(!VPARR_memcmp(&myvector, myints, 7));
	ck_assert(!memcmp(myvector.base, myints, 7 * myvector.size));
}
END_TEST

START_TEST(test_VPARR_minmax_element)
{
	int vbase[] = {3,9,1,4,2,5,9};
	VPARR v = VPARR_INIT_FROM_STATIC(vbase);

	struct VPARR_minmax_s result = VPARR_minmax_element(&v, compar_int);
	ck_assert_int_eq(*(int*)result.min, 1);
	ck_assert_int_eq(*(int*)result.max, 9);

	ck_assert_int_eq(*(int*)VPARR_min_element(&v, compar_int), 1);
	ck_assert_int_eq(*(int*)VPARR_max_element(&v, compar_int), 9);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_VPARR_merge);
    tcase_add_test(tc1_1, test_VPARR_find);
    tcase_add_test(tc1_1, test_VPARR_find_if);
    tcase_add_test(tc1_1, test_VPARR_copy);


    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
