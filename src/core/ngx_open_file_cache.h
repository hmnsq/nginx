
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#که_نه_تعریف_شوی _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#تعریف_کړه _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#تعریف_کړه NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_ارزښت


تعريفنوع بنية {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وخت_t                   mtime;
    off_t                    اندازه;
    off_t                    fs_اندازه;
    off_t                    directio;
    اندازه_t                   read_ahead;

    ngx_err_t                err;
    حرف                    *failed;

    وخت_t                   valid;

    ngx_uint_t               min_کاروي;

#إذا (NGX_HAVE_OPENAT)
    اندازه_t                   disable_symlinks_from;
    بینښه                 disable_symlinks:2;
#د_که_پای

    بینښه                 test_dir:1;
    بینښه                 test_only:1;
    بینښه                 لوګاریتم:1;
    بینښه                 errors:1;
    بینښه                 events:1;

    بینښه                 is_dir:1;
    بینښه                 is_file:1;
    بینښه                 is_link:1;
    بینښه                 is_exec:1;
    بینښه                 is_directio:1;
} ngx_open_file_info_t;


تعريفنوع بنية ngx_cached_open_file_s  ngx_cached_open_file_t;

بنية ngx_cached_open_file_s {
    ngx_rbtree_غوټه_t        غوټه;
    ngx_کتار_t              کتار;

    u_char                  *name;
    وخت_t                   created;
    وخت_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وخت_t                   mtime;
    off_t                    اندازه;
    ngx_err_t                err;

    uint32_t                 کاروي;

#إذا (NGX_HAVE_OPENAT)
    اندازه_t                   disable_symlinks_from;
    بینښه                 disable_symlinks:2;
#د_که_پای

    بینښه                 شمېر:24;
    بینښه                 close:1;
    بینښه                 use_event:1;

    بینښه                 is_dir:1;
    بینښه                 is_file:1;
    بینښه                 is_link:1;
    بینښه                 is_exec:1;
    بینښه                 is_directio:1;

    ngx_event_t             *event;
};


تعريفنوع بنية {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_غوټه_t        sentinel;
    ngx_کتار_t              expire_کتار;

    ngx_uint_t               current;
    ngx_uint_t               max;
    وخت_t                   inactive;
} ngx_open_file_cache_t;


تعريفنوع بنية {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_کاروي;
    ngx_log_t               *لوګاریتم;
} ngx_open_file_cache_cleanup_t;


تعريفنوع بنية {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    فراغ                    *معلومات;
    ngx_event_t             *اقرأ;
    ngx_event_t             *اكتب;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, وخت_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *د, ngx_pool_t *pool);


#د_که_پای /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
