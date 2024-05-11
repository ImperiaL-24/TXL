#include "impl/iter.h"
#include "list/list.h"
#include "str.h"
#include <stdio.h>

#include "impl/int_impl.h"
#include "map/hmap.h"
#include "map/omap.h"

#include "err.h"

#include "impl/clone.h"

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

	omap_t omap = OMAP_NEW(uint32_t, uint32_t);
	key = 5;
	value = 11;
	om_set(&omap, &key, &value);
	DIE(*(int *)om_get(&omap, &key) != 11, "TEST 7 1 FAIL");
	DIE(omap.access.length != 1, "TEST 7 FAIL");

	value = 18;
	om_set(&omap, &key, &value);
	DIE(*(int *)om_get(&omap, &key) != 18, "TEST 7 2 FAIL");
	DIE(omap.access.length != 1, "TEST 7 3 FAIL");

	key = 444;
	value = 21;
	om_set(&omap, &key, &value);
	DIE(*(int *)om_get(&omap, &key) != 21, "TEST 7 4 FAIL");
	DIE(omap.access.length != 2, "TEST 7 5 FAIL");

	DIE(!om_has(&omap, &key), "TEST 8 FAIL");
	key = 5;
	DIE(!om_has(&omap, &key), "TEST 8 FAIL");
	key = 7;
	DIE(om_has(&omap, &key), "TEST 8 FAIL");
	printf("\n");

	key = 1;
	value = 121;
	om_set(&omap, &key, &value);
	for_iter(omap_t, i, &omap) { printf("%d ", OMITER_VAL(i, uint32_t)); }
	printf("\n");
	sll_free(&list);
	hm_free(&hmap);
	om_free(&omap);

	omap = OMAP_NEW(str_t, str_t);
	str_t key_s = str_ref("test1");
	str_t key_s1 = str_ref("key1");

	om_set(&omap, &key_s, &str);
	om_set(&omap, &key_s, &str2);
	om_set(&omap, &key_s1, &str2);

	DIE(!str_eq(*(str_t *)om_get(&omap, &key_s), str2), "TEST 9 FAIL");
	DIE(!str_eq(*(str_t *)om_get(&omap, &key_s1), str2), "TEST 9 FAIL");

	om_remove(&omap, &key_s1);
	DIE(om_has(&omap, &key_s1), "TEST 9 FAIL");
	DIE(omap.access.length != 1, "TEST 9 FAIL");

	printf("%s\n", *(str_t *)om_get(&omap, &key_s));
	om_free(&omap);

	omap = OMAP_NEW(uint32_t, uint32_t);
	for (int i = 0; i < 10; i++) {
		om_set(&omap, &i, &i);
		printf("NR: %d, HASH: %u\n", i, omap.access.key_proto->hash(&i));
	}
	for_iter(omap_t, i, &omap)
	{
		printf("VALUE: %d\n", OMITER_VAL(i, uint32_t));
	}
	om_remove_first(&omap);
	for_iter(omap_t, i, &omap)
	{
		printf("VALUE: %d\n", OMITER_VAL(i, uint32_t));
	}
	om_remove_first(&omap);
	for_iter(omap_t, i, &omap)
	{
		printf("VALUE: %d\n", OMITER_VAL(i, uint32_t));
	}
	om_remove_first(&omap);
	for_iter(omap_t, i, &omap)
	{
		printf("VALUE: %d\n", OMITER_VAL(i, uint32_t));
	}
	om_free(&omap);

	omap = OMAP_NEW(uint32_t, test_t);

	test_t val = (test_t){.code = 'a', .val = 5};
	key = 5;
	om_set(&omap, &key, &val);
	DIE(((test_t *)om_get(&omap, &key))->code != 'a', "TEST 10 FAIL");
	DIE(((test_t *)om_get(&omap, &key))->val != 5, "TEST 10 FAIL");
	key = 6;
	om_set(&omap, &key, &val);
	DIE(((test_t *)om_get(&omap, &key))->code != 'a', "TEST 10 FAIL");
	DIE(((test_t *)om_get(&omap, &key))->val != 5, "TEST 10 FAIL");
	om_remove_first(&omap);
	om_remove_first(&omap);
	DIE(omap.data.size, "TEST 10 FAIL");

	om_free(&omap);
	// TODO: tests for int key and struct value;
}