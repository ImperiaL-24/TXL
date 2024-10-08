#include "impl/iter.h"
#include "list/list.h"
#include "str.h"
#include <stdio.h>

#include "impl/int_impl.h"
#include "map/hmap.h"
#include "map/lmap.h"

#include "err.h"

#include "impl/clone.h"

#include "vec.h"

#include "set/hset.h"
#include "set/tset.h"

#include "impl/set.h"

typedef struct {
	char code;
	int val;
} test_t;

impl_default_clone(test_t);
DEFINE_PROTO(test_t, CLONE, FREE_DEFAULT);

int main()
{

	list_t list = LIST_NEW(uint32_t);
	uint32_t data = 5;
	// scanf("%d", &data);
	sll_addh(&list, &data);
	data = 7;
	sll_addh(&list, &data);
	data = 13;
	sll_addh(&list, &data);
	data = 9;
	sll_add_index(&list, 100, &data);

	vec_t vec = vec_new(PROTOTYPE(uint32_t));

	for (int i = 0; i < 10; i++)
		vec_insert(&vec, 0, &i);
	for_iter(vec_t, i, &vec) { printf("%d ", ITER_VAL(i, uint32_t)); }
	vec_free(&vec);

	DIE(*(uint32_t *)sll_get(&list, 0) != 13, "TEST 1 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 1) != 7, "TEST 1 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 2) != 5, "TEST 1 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 3) != 9, "TEST 1 FAIL");

	sll_remh(&list);
	sll_rem_index(&list, 0);

	DIE(*(uint32_t *)sll_get(&list, 0) != 5, "TEST 2 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 1) != 9, "TEST 2 FAIL");

	data = 11;
	sll_addt(&list, &data);
	data = 25;
	sll_add_after(&list, list.head, &data);

	DIE(*(uint32_t *)sll_get(&list, 0) != 5, "TEST 3 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 1) != 25, "TEST 3 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 2) != 9, "TEST 3 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 3) != 11, "TEST 3 FAIL");

	sll_rem_after(&list, list.head->next);

	DIE(*(uint32_t *)sll_get(&list, 0) != 5, "TEST 3.1 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 1) != 25, "TEST 3.1 FAIL");
	DIE(*(uint32_t *)sll_get(&list, 2) != 11, "TEST 3.1 FAIL");

	str_t str = str_ref("test");
	str_t str2 = str_ref("test2");
	DIE(str_eq(str, str2) != 0, "TEST 4 FAIL");
	DIE(str_eq(str, str) != 1, "TEST 4 FAIL");

	hmap_t hmap = HMAP_NEW(uint32_t, uint32_t);
	uint32_t key = 5;
	uint32_t value = 11;
	hm_set(&hmap, &key, &value);
	DIE(*(int *)hm_get(&hmap, &key) != 11, "TEST 5 FAIL");
	DIE(hmap.length != 1, "TEST 5 FAIL");

	value = 18;
	hm_set(&hmap, &key, &value);
	DIE(*(int *)hm_get(&hmap, &key) != 18, "TEST 5 FAIL");
	DIE(hmap.length != 1, "TEST 5 FAIL");

	key = 444;
	value = 21;
	hm_set(&hmap, &key, &value);
	DIE(*(int *)hm_get(&hmap, &key) != 21, "TEST 5 FAIL");
	DIE(hmap.length != 2, "TEST 5 FAIL");

	DIE(!hm_has(&hmap, &key), "TEST 6 FAIL");
	key = 5;
	DIE(!hm_has(&hmap, &key), "TEST 6 FAIL");
	key = 7;
	DIE(hm_has(&hmap, &key), "TEST 6 FAIL");

	key = 444;
	hm_remove(&hmap, &key);
	DIE(hm_has(&hmap, &key), "TEST 6 R FAIL");

	lmap_t lmap = LMAP_NEW(uint32_t, uint32_t);
	key = 5;
	value = 11;
	lm_set(&lmap, &key, &value);
	DIE(*(int *)lm_get(&lmap, &key) != 11, "TEST 7 1 FAIL");
	DIE(lmap.access.length != 1, "TEST 7 FAIL");

	value = 18;
	lm_set(&lmap, &key, &value);
	DIE(*(int *)lm_get(&lmap, &key) != 18, "TEST 7 2 FAIL");
	DIE(lmap.access.length != 1, "TEST 7 3 FAIL");

	key = 444;
	value = 21;
	lm_set(&lmap, &key, &value);
	DIE(*(int *)lm_get(&lmap, &key) != 21, "TEST 7 4 FAIL");
	DIE(lmap.access.length != 2, "TEST 7 5 FAIL");

	DIE(!lm_has(&lmap, &key), "TEST 8 FAIL");
	key = 5;
	DIE(!lm_has(&lmap, &key), "TEST 8 FAIL");
	key = 7;
	DIE(lm_has(&lmap, &key), "TEST 8 FAIL");
	printf("\n");

	key = 1;
	value = 121;
	lm_set(&lmap, &key, &value);
	for_iter(lmap_t, i, &lmap)
	{
		printf("%d ", *(uint32_t *)ITER_VAL(i, kv_pair_t).value);
	}
	printf("\n");
	sll_free(&list);
	hm_free(&hmap);
	lm_free(&lmap);

	lmap = LMAP_NEW(str_t, str_t);
	str_t key_s = str_ref("test1");
	str_t key_s1 = str_ref("key1");

	lm_set(&lmap, &key_s, &str);
	lm_set(&lmap, &key_s, &str2);
	lm_set(&lmap, &key_s1, &str2);

	DIE(!str_eq(*(str_t *)lm_get(&lmap, &key_s), str2), "TEST 9 FAIL");
	DIE(!str_eq(*(str_t *)lm_get(&lmap, &key_s1), str2), "TEST 9 FAIL");

	lm_remove(&lmap, &key_s1);
	DIE(lm_has(&lmap, &key_s1), "TEST 9 FAIL");
	DIE(lmap.access.length != 1, "TEST 9 FAIL");

	printf("%s\n", *(str_t *)lm_get(&lmap, &key_s));
	lm_free(&lmap);

	lmap = LMAP_NEW(uint32_t, uint32_t);
	for (int i = 0; i < 10; i++) {
		lm_set(&lmap, &i, &i);
		printf("NR: %d, HASH: %u\n", i, lmap.access.key_proto->hash(&i));
	}
	for_iter(lmap_t, i, &lmap)
	{
		printf("VALUE: %d\n", *(uint32_t *)ITER_VAL(i, kv_pair_t).value);
	}
	lm_remove_first(&lmap);
	for_iter(lmap_t, i, &lmap)
	{
		printf("VALUE: %d\n", *(uint32_t *)ITER_VAL(i, kv_pair_t).value);
	}
	lm_remove_first(&lmap);
	for_iter(lmap_t, i, &lmap)
	{
		printf("VALUE: %d\n", *(uint32_t *)ITER_VAL(i, kv_pair_t).value);
	}
	lm_remove_first(&lmap);
	for_iter(lmap_t, i, &lmap)
	{
		printf("VALUE: %d\n", *(uint32_t *)ITER_VAL(i, kv_pair_t).value);
	}
	lm_free(&lmap);

	lmap = LMAP_NEW(uint32_t, test_t);

	test_t val = (test_t){.code = 'a', .val = 5};
	key = 5;
	lm_set(&lmap, &key, &val);
	DIE(((test_t *)lm_get(&lmap, &key))->code != 'a', "TEST 10 FAIL");
	DIE(((test_t *)lm_get(&lmap, &key))->val != 5, "TEST 10 FAIL");
	key = 6;
	lm_set(&lmap, &key, &val);
	DIE(((test_t *)lm_get(&lmap, &key))->code != 'a', "TEST 10 FAIL");
	DIE(((test_t *)lm_get(&lmap, &key))->val != 5, "TEST 10 FAIL");
	lm_remove_first(&lmap);
	lm_remove_first(&lmap);
	DIE(lmap.data.size, "TEST 10 FAIL");

	lm_free(&lmap);

	hset_t hset = HSET_NEW(uint32_t);
	value = 5;
	hs_add(&hset, &value);

	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");

	DIE(!hs_has(&hset, &value), "TEST 11 1 FAIL");
	DIE(hset.length != 1, "TEST 11 1 FAIL");

	hs_remove(&hset, &value);

	DIE(hs_has(&hset, &value), "TEST 11 2 FAIL");
	DIE(hset.length != 0, "TEST 11 2 FAIL");

	for (int i = 0; i < 100; i++) {
		hs_add(&hset, &value);
	}

	DIE(!hs_has(&hset, &value), "TEST 11 3 AIL");
	DIE(hset.length != 1, "TEST 11 3 FAIL");

	for (int i = 0; i < 100; i++) {
		hs_remove(&hset, &value);
	}

	DIE(hs_has(&hset, &value), "TEST 11 4 FAIL");
	DIE(hset.length != 0, "TEST 11 4 FAIL");

	for (int i = 0; i < 100; i++) {
		hs_add(&hset, &i);
	}

	DIE(!hs_has(&hset, &value), "TEST 11 5 1 FAIL");
	DIE(hset.length != 100, "TEST 11 5 2 FAIL");
	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");

	hs_free(&hset);

	hset = HSET_NEW(uint32_t);
	hset_t hset2 = HSET_NEW(uint32_t);

	for (int i = 0; i < 5; i++) {
		hs_add(&hset, &i);
	}
	DIE(hset.length != 5, "TEST 12 FAIL");
	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	for (int i = 5; i < 10; i++) {
		hs_add(&hset2, &i);
	}
	for_iter(hset_t, i, &hset2) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&hset, &hset2, PROTOTYPE(hset_t), PROTOTYPE(hset_t));

	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	DIE(hset.length != 10, "TEST 12 FAIL");

	set_subtract(&hset, &hset2, PROTOTYPE(hset_t), PROTOTYPE(hset_t));
	DIE(hset.length != 5, "TEST 12 FAIL");

	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&hset, &hset2, PROTOTYPE(hset_t), PROTOTYPE(hset_t));
	set_intersect(&hset, &hset2, PROTOTYPE(hset_t), PROTOTYPE(hset_t));

	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	DIE(hset.length != 5, "TEST 12 FAIL");

	hs_free(&hset);
	hs_free(&hset2);

	/* TSET */
	printf("TSET START\n");

	tset_t tset = TSET_NEW(uint32_t);
	value = 5;
	ts_add(&tset, &value);

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");

	DIE(!ts_has(&tset, &value), "TEST 13 FAIL");

	ts_remove(&tset, &value);

	DIE(ts_has(&tset, &value), "TEST 13 FAIL");

	for (int i = 0; i < 100; i++) {
		ts_add(&tset, &value);
	}

	DIE(!ts_has(&tset, &value), "TEST 13 FAIL");

	for (int i = 0; i < 100; i++) {
		ts_remove(&tset, &value);
	}

	DIE(ts_has(&tset, &value), "TEST 13 FAIL");

	for (int i = 0; i < 100; i++) {
		ts_add(&tset, &i);
	}

	DIE(!ts_has(&tset, &value), "TEST 13 FAIL");
	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");

	for (int i = 0; i < 80; i += 5) {
		ts_remove(&tset, &i);
	}

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	ts_free(&tset);
	printf(" TSET OPERATIONS \n\n");

	tset = TSET_NEW(uint32_t);
	tset_t tset2 = TSET_NEW(uint32_t);

	for (int i = 0; i < 5; i++) {
		ts_add(&tset, &i);
	}
	int testttt = 1;
	ts_add(&tset, &testttt);

	// // DIE(tset.length != 5, "TEST 12 FAIL");
	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	for (int i = 5; i < 10; i++) {
		ts_add(&tset2, &i);
	}
	for_iter(tset_t, i, &tset2) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&tset, &tset2, PROTOTYPE(tset_t), PROTOTYPE(tset_t));

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	// // DIE(tset.length != 10, "TEST 12 FAIL");

	set_subtract(&tset, &tset2, PROTOTYPE(tset_t), PROTOTYPE(tset_t));
	// // DIE(tset.length != 5, "TEST 12 FAIL");

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&tset, &tset2, PROTOTYPE(tset_t), PROTOTYPE(tset_t));
	set_reunion(&tset, &tset2, PROTOTYPE(tset_t), PROTOTYPE(tset_t));

	// set_intersect(&tset, &tset2, PROTOTYPE(tset_t), PROTOTYPE(tset_t));

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	// // DIE(tset.length != 5, "TEST 12 FAIL");
	ts_free(&tset);
	ts_free(&tset2);

	printf(" HYBRID OPERATIONS \n\n");

	tset = TSET_NEW(uint32_t);
	hset = HSET_NEW(uint32_t);

	for (int i = 0; i < 5; i++) {
		ts_add(&tset, &i);
	}
	testttt = 1;
	ts_add(&tset, &testttt);

	// // DIE(tset.length != 5, "TEST 12 FAIL");
	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	for (int i = 5; i < 10; i++) {
		hs_add(&hset, &i);
	}
	for_iter(hset_t, i, &hset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&tset, &hset, PROTOTYPE(tset_t), PROTOTYPE(hset_t));

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	// // DIE(tset.length != 10, "TEST 12 FAIL");

	set_subtract(&tset, &hset, PROTOTYPE(tset_t), PROTOTYPE(hset_t));
	// // DIE(tset.length != 5, "TEST 12 FAIL");

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	set_reunion(&tset, &hset, PROTOTYPE(tset_t), PROTOTYPE(hset_t));
	set_reunion(&tset, &hset, PROTOTYPE(tset_t), PROTOTYPE(hset_t));

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");

	set_intersect(&tset, &hset, PROTOTYPE(tset_t), PROTOTYPE(hset_t));

	for_iter(tset_t, i, &tset) { printf("%d ", ITER_VAL(i, uint32_t)); }
	printf("\n");
	// // DIE(tset.length != 5, "TEST 12 FAIL");
	ts_free(&tset);
	hs_free(&hset);
}