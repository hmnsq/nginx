
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جيڪڏهن_وصف_نه_ٿيل _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#وصف_ڪريو _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#وصف_ڪريو NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_قدر


تعريفنوع بنية {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وقت_t                   mtime;
    off_t                    سائيز;
    off_t                    fs_سائيز;
    off_t                    directio;
    سائيز_t                   read_ahead;

    ngx_err_t                err;
    حرف                    *failed;

    وقت_t                   valid;

    ngx_uint_t               min_استعمال_ڪري;

#إذا (NGX_HAVE_OPENAT)
    سائيز_t                   disable_symlinks_from;
    بنانشان                 disable_symlinks:2;
#جيڪڏهن_پورو

    بنانشان                 test_dir:1;
    بنانشان                 test_only:1;
    بنانشان                 لاگرٿم:1;
    بنانشان                 errors:1;
    بنانشان                 events:1;

    بنانشان                 is_dir:1;
    بنانشان                 is_file:1;
    بنانشان                 is_link:1;
    بنانشان                 is_exec:1;
    بنانشان                 is_directio:1;
} ngx_open_file_info_t;


تعريفنوع بنية ngx_cached_open_file_s  ngx_cached_open_file_t;

بنية ngx_cached_open_file_s {
    ngx_rbtree_ڳنڍ_t        ڳنڍ;
    ngx_قطار_t              قطار;

    u_char                  *name;
    وقت_t                   created;
    وقت_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    وقت_t                   mtime;
    off_t                    سائيز;
    ngx_err_t                err;

    uint32_t                 استعمال_ڪري;

#إذا (NGX_HAVE_OPENAT)
    سائيز_t                   disable_symlinks_from;
    بنانشان                 disable_symlinks:2;
#جيڪڏهن_پورو

    بنانشان                 ڳڻپ:24;
    بنانشان                 close:1;
    بنانشان                 use_event:1;

    بنانشان                 is_dir:1;
    بنانشان                 is_file:1;
    بنانشان                 is_link:1;
    بنانشان                 is_exec:1;
    بنانشان                 is_directio:1;

    ngx_event_t             *event;
};


تعريفنوع بنية {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_ڳنڍ_t        sentinel;
    ngx_قطار_t              expire_قطار;

    ngx_uint_t               current;
    ngx_uint_t               max;
    وقت_t                   inactive;
} ngx_open_file_cache_t;


تعريفنوع بنية {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_استعمال_ڪري;
    ngx_log_t               *لاگرٿم;
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
    ngx_uint_t max, وقت_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *جو, ngx_pool_t *pool);


#جيڪڏهن_پورو /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
