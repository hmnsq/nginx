
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_HASH_H_INCLUDED_
#నిర్వచించు _NGX_HASH_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    శూన్య             *విలువ;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


రకనిర్వచనం నిర్మాణం {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        కొలత;
} ngx_hash_t;


రకనిర్వచనం నిర్మాణం {
    ngx_hash_t        hash;
    శూన్య             *విలువ;
} ngx_hash_wildcard_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t         కీలకం;
    ngx_uint_t        కీలకం_hash;
    శూన్య             *విలువ;
} ngx_hash_కీలకం_t;


రకనిర్వచనం ngx_uint_t (*ngx_hash_కీలకం_pt) (u_char *దత్తాంశం, కొలత_t len);


రకనిర్వచనం నిర్మాణం {
    ngx_hash_t            hash;
    ngx_hash_wildcard_t  *wc_తల;
    ngx_hash_wildcard_t  *wc_తోక;
} ngx_hash_combined_t;


రకనిర్వచనం నిర్మాణం {
    ngx_hash_t       *hash;
    ngx_hash_కీలకం_pt   కీలకం;

    ngx_uint_t        max_కొలత;
    ngx_uint_t        పాత్ర_కొలత;

    అక్షరం             *name;
    ngx_pool_t       *pool;
    ngx_pool_t       *తాత్కాలిక_pool;
} ngx_hash_init_t;


#నిర్వచించు NGX_HASH_SMALL            1
#నిర్వచించు NGX_HASH_LARGE            2

#నిర్వచించు NGX_HASH_LARGE_ASIZE      16384
#నిర్వచించు NGX_HASH_LARGE_HSIZE      10007

#నిర్వచించు NGX_HASH_WILDCARD_కీలకం     1
#నిర్వచించు NGX_HASH_READONLY_కీలకం     2


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t        hsize;

    ngx_pool_t       *pool;
    ngx_pool_t       *తాత్కాలిక_pool;

    ngx_array_t       కీలకాలు;
    ngx_array_t      *కీలకాలు_hash;

    ngx_array_t       dns_wc_తల;
    ngx_array_t      *dns_wc_తల_hash;

    ngx_array_t       dns_wc_తోక;
    ngx_array_t      *dns_wc_తోక_hash;
} ngx_hash_కీలకాలు_arrays_t;


రకనిర్వచనం నిర్మాణం ngx_పట్టిక_elt_s  ngx_పట్టిక_elt_t;

నిర్మాణం ngx_పట్టిక_elt_s {
    ngx_uint_t        hash;
    ngx_str_t         కీలకం;
    ngx_str_t         విలువ;
    u_char           *lowcase_కీలకం;
    ngx_పట్టిక_elt_t  *తదుపరి;
};


శూన్య *ngx_hash_కనుగొను(ngx_hash_t *hash, ngx_uint_t కీలకం, u_char *name, కొలత_t len);
శూన్య *ngx_hash_కనుగొను_wc_తల(ngx_hash_wildcard_t *hwc, u_char *name, కొలత_t len);
శూన్య *ngx_hash_కనుగొను_wc_తోక(ngx_hash_wildcard_t *hwc, u_char *name, కొలత_t len);
శూన్య *ngx_hash_కనుగొను_combined(ngx_hash_combined_t *hash, ngx_uint_t కీలకం,
    u_char *name, కొలత_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_కీలకం_t *పేర్లు,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_కీలకం_t *పేర్లు,
    ngx_uint_t nelts);

#నిర్వచించు ngx_hash(కీలకం, c)   ((ngx_uint_t) కీలకం * 31 + c)
ngx_uint_t ngx_hash_కీలకం(u_char *దత్తాంశం, కొలత_t len);
ngx_uint_t ngx_hash_కీలకం_lc(u_char *దత్తాంశం, కొలత_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, కొలత_t n);


ngx_int_t ngx_hash_కీలకాలు_array_init(ngx_hash_కీలకాలు_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_కూడు_కీలకం(ngx_hash_కీలకాలు_arrays_t *ha, ngx_str_t *కీలకం,
    శూన్య *విలువ, ngx_uint_t flags);


#అయితే_ముగింపు /* _NGX_HASH_H_INCLUDED_ */
