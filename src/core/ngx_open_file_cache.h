
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#اگر_تعریف_نشده _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#تعریف_کن _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#تعریف_کن NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_مقدار


تعريفنوع بنية {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    زمان_t                   mtime;
    off_t                    اندازه;
    off_t                    fs_اندازه;
    off_t                    directio;
    اندازه_t                   read_ahead;

    ngx_err_t                err;
    حرف                    *failed;

    زمان_t                   valid;

    ngx_uint_t               min_استفاده_میکند;

#إذا (NGX_HAVE_OPENAT)
    اندازه_t                   disable_symlinks_from;
    بیعلامت                 disable_symlinks:2;
#پایان_اگر

    بیعلامت                 test_dir:1;
    بیعلامت                 test_only:1;
    بیعلامت                 لگاریتم:1;
    بیعلامت                 errors:1;
    بیعلامت                 events:1;

    بیعلامت                 is_dir:1;
    بیعلامت                 is_file:1;
    بیعلامت                 is_link:1;
    بیعلامت                 is_exec:1;
    بیعلامت                 is_directio:1;
} ngx_open_file_info_t;


تعريفنوع بنية ngx_cached_open_file_s  ngx_cached_open_file_t;

بنية ngx_cached_open_file_s {
    ngx_rbtree_گره_t        گره;
    ngx_صف_t              صف;

    u_char                  *name;
    زمان_t                   created;
    زمان_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    زمان_t                   mtime;
    off_t                    اندازه;
    ngx_err_t                err;

    uint32_t                 استفاده_میکند;

#إذا (NGX_HAVE_OPENAT)
    اندازه_t                   disable_symlinks_from;
    بیعلامت                 disable_symlinks:2;
#پایان_اگر

    بیعلامت                 شمار:24;
    بیعلامت                 close:1;
    بیعلامت                 use_event:1;

    بیعلامت                 is_dir:1;
    بیعلامت                 is_file:1;
    بیعلامت                 is_link:1;
    بیعلامت                 is_exec:1;
    بیعلامت                 is_directio:1;

    ngx_event_t             *event;
};


تعريفنوع بنية {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_گره_t        sentinel;
    ngx_صف_t              expire_صف;

    ngx_uint_t               current;
    ngx_uint_t               max;
    زمان_t                   inactive;
} ngx_open_file_cache_t;


تعريفنوع بنية {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_استفاده_میکند;
    ngx_log_t               *لگاریتم;
} ngx_open_file_cache_cleanup_t;


تعريفنوع بنية {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    فراغ                    *داده;
    ngx_event_t             *اقرأ;
    ngx_event_t             *اكتب;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, زمان_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *از, ngx_pool_t *pool);


#پایان_اگر /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
