
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, కొలత_t కొలత)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, పరిమాణం(ngx_list_t));
    ఒకవేళ (list == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_list_init(list, pool, n, కొలత) != NGX_OK) {
        తిరిగి ఖాళీ;
    }

    తిరిగి list;
}


శూన్య *
ngx_list_నెట్టు(ngx_list_t *l)
{
    శూన్య             *elt;
    ngx_list_part_t  *చివరి;

    చివరి = l->చివరి;

    ఒకవేళ (చివరి->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        చివరి = ngx_palloc(l->pool, పరిమాణం(ngx_list_part_t));
        ఒకవేళ (చివరి == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        చివరి->elts = ngx_palloc(l->pool, l->nalloc * l->కొలత);
        ఒకవేళ (చివరి->elts == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        చివరి->nelts = 0;
        చివరి->తదుపరి = ఖాళీ;

        l->చివరి->తదుపరి = చివరి;
        l->చివరి = చివరి;
    }

    elt = (అక్షరం *) చివరి->elts + l->కొలత * చివరి->nelts;
    చివరి->nelts++;

    తిరిగి elt;
}
