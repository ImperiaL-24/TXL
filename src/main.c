#include <stdio.h>
#include "impl/iter.h"
#include "list/list.h"
#include "str.h"

#include "map/hmap.h"
#include "impl/int_impl.h"
#include "map/omap.h"

int main() {
    list_t list = LIST_NEW(sizeof(int));
    int data = 5;
    sll_addh(&list, &data);
    data = 7;
    sll_addh(&list, &data);
    data = 13;
    sll_addh(&list, &data);
    data = 9;
    sll_add_index(&list, 100 ,&data);

    sll_remh(&list);
    sll_rem_index(&list, 0);
    data = 11;
    sll_addt(&list, &data);
    for_iter(list_t, i, &list) {
        printf("%d ", SLITER_VAL(i, int));
    }
    str_t str = str_from("test");
    
    printf("%s ", str.data);
    str_eq(str, str);

    hmap_t hmap = HMAP_NEW(uint32_t, uint32_t);
    uint32_t key = 5;
    uint32_t value = 11;
    hm_set(&hmap, &key, &value);
    printf("%d ", *(int*)hm_get(&hmap, &key));
    value = 18;

    hm_set(&hmap, &key, &value);
    printf("%d ", *(int*)hm_get(&hmap, &key));
    key = 444;
    hm_set(&hmap, &key, &value);
    printf("%d ", *(int*)hm_get(&hmap, &key));
    printf("%ld ", hmap.length);
    printf("%ld ", hm_has(&hmap, &key));
    key = 5;
    printf("%ld ", hm_has(&hmap, &key));
    key = 7;
    printf("%ld ", hm_has(&hmap, &key));

    omap_t omap = OMAP_NEW(uint32_t, uint32_t);
    key = 5;
    value = 11;
    om_set(&omap, &key, &value);
    printf("%d\n", *(int*)om_get(&omap, &key));
    value = 18;

    om_set(&omap, &key, &value);
    printf("%d ", *(int*)om_get(&omap, &key));
    key = 444;
    om_set(&omap, &key, &value);
    printf("%d ", *(int*)om_get(&omap, &key));
    printf("%ld ", omap.data.size);
    printf("%ld ", om_has(&omap, &key));
    key = 5;
    printf("%ld ", om_has(&omap, &key));
    key = 7;
    printf("%ld ", om_has(&omap, &key));

    printf("\n");

}