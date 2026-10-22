
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, आकार_t आकार)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, परिमाण(ngx_list_t));
    यदि (list == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ngx_list_init(list, pool, n, आकार) != NGX_OK) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम list;
}


शून्य *
ngx_list_प्रक्षिप(ngx_list_t *l)
{
    शून्य             *elt;
    ngx_list_part_t  *चरमः;

    चरमः = l->चरमः;

    यदि (चरमः->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        चरमः = ngx_palloc(l->pool, परिमाण(ngx_list_part_t));
        यदि (चरमः == रिक्त) {
            प्रत्यागम रिक्त;
        }

        चरमः->elts = ngx_palloc(l->pool, l->nalloc * l->आकार);
        यदि (चरमः->elts == रिक्त) {
            प्रत्यागम रिक्त;
        }

        चरमः->nelts = 0;
        चरमः->अग्र = रिक्त;

        l->चरमः->अग्र = चरमः;
        l->चरमः = चरमः;
    }

    elt = (वर्ण *) चरमः->elts + l->आकार * चरमः->nelts;
    चरमः->nelts++;

    प्रत्यागम elt;
}
