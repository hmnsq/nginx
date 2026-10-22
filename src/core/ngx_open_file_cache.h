
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر_غیر_تعریف_شدہ _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#تعریف_کرو _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#تعریف_کرو NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_قدر


قسمتعریف ساخت {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وقت_t                   mtime;
    off_t                    سائز;
    off_t                    fs_سائز;
    off_t                    directio;
    سائز_t                   read_ahead;

    ngx_err_t                err;
    حرف                    *failed;

    وقت_t                   valid;

    ngx_uint_t               min_استعمال_کرتا;

#اگر (NGX_HAVE_OPENAT)
    سائز_t                   disable_symlinks_from;
    غیرعلامتی                 disable_symlinks:2;
#اگر_ختم

    غیرعلامتی                 test_dir:1;
    غیرعلامتی                 test_only:1;
    غیرعلامتی                 لوگارتھم:1;
    غیرعلامتی                 errors:1;
    غیرعلامتی                 events:1;

    غیرعلامتی                 is_dir:1;
    غیرعلامتی                 is_file:1;
    غیرعلامتی                 is_link:1;
    غیرعلامتی                 is_exec:1;
    غیرعلامتی                 is_directio:1;
} ngx_open_file_info_t;


قسمتعریف ساخت ngx_cached_open_file_s  ngx_cached_open_file_t;

ساخت ngx_cached_open_file_s {
    ngx_rbtree_گرہ_t        گرہ;
    ngx_قطار_t              قطار;

    u_char                  *name;
    وقت_t                   created;
    وقت_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وقت_t                   mtime;
    off_t                    سائز;
    ngx_err_t                err;

    uint32_t                 استعمال_کرتا;

#اگر (NGX_HAVE_OPENAT)
    سائز_t                   disable_symlinks_from;
    غیرعلامتی                 disable_symlinks:2;
#اگر_ختم

    غیرعلامتی                 گنتی:24;
    غیرعلامتی                 close:1;
    غیرعلامتی                 use_event:1;

    غیرعلامتی                 is_dir:1;
    غیرعلامتی                 is_file:1;
    غیرعلامتی                 is_link:1;
    غیرعلامتی                 is_exec:1;
    غیرعلامتی                 is_directio:1;

    ngx_event_t             *event;
};


قسمتعریف ساخت {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_گرہ_t        sentinel;
    ngx_قطار_t              expire_قطار;

    ngx_uint_t               current;
    ngx_uint_t               max;
    وقت_t                   inactive;
} ngx_open_file_cache_t;


قسمتعریف ساخت {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_استعمال_کرتا;
    ngx_log_t               *لوگارتھم;
} ngx_open_file_cache_cleanup_t;


قسمتعریف ساخت {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    خالی                    *ڈیٹا;
    ngx_event_t             *اقرأ;
    ngx_event_t             *اكتب;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, وقت_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *کا, ngx_pool_t *pool);


#اگر_ختم /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
