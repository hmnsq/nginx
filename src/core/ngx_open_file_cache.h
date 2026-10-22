
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جے_بنا_تعریف _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#تعریف_کرو _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#تعریف_کرو NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_قدر


تعريفنوع بنية {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ویلا_t                   mtime;
    off_t                    ناپ;
    off_t                    fs_ناپ;
    off_t                    directio;
    ناپ_t                   read_ahead;

    ngx_err_t                err;
    حرف                    *failed;

    ویلا_t                   valid;

    ngx_uint_t               min_ورتدا;

#إذا (NGX_HAVE_OPENAT)
    ناپ_t                   disable_symlinks_from;
    بنانشانی                 disable_symlinks:2;
#جے_انت

    بنانشانی                 test_dir:1;
    بنانشانی                 test_only:1;
    بنانشانی                 لوگارتھم:1;
    بنانشانی                 errors:1;
    بنانشانی                 events:1;

    بنانشانی                 is_dir:1;
    بنانشانی                 is_file:1;
    بنانشانی                 is_link:1;
    بنانشانی                 is_exec:1;
    بنانشانی                 is_directio:1;
} ngx_open_file_info_t;


تعريفنوع بنية ngx_cached_open_file_s  ngx_cached_open_file_t;

بنية ngx_cached_open_file_s {
    ngx_rbtree_گرہ_t        گرہ;
    ngx_قطار_t              قطار;

    u_char                  *name;
    ویلا_t                   created;
    ویلا_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ویلا_t                   mtime;
    off_t                    ناپ;
    ngx_err_t                err;

    uint32_t                 ورتدا;

#إذا (NGX_HAVE_OPENAT)
    ناپ_t                   disable_symlinks_from;
    بنانشانی                 disable_symlinks:2;
#جے_انت

    بنانشانی                 گنتی:24;
    بنانشانی                 close:1;
    بنانشانی                 use_event:1;

    بنانشانی                 is_dir:1;
    بنانشانی                 is_file:1;
    بنانشانی                 is_link:1;
    بنانشانی                 is_exec:1;
    بنانشانی                 is_directio:1;

    ngx_event_t             *event;
};


تعريفنوع بنية {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_گرہ_t        sentinel;
    ngx_قطار_t              expire_قطار;

    ngx_uint_t               current;
    ngx_uint_t               max;
    ویلا_t                   inactive;
} ngx_open_file_cache_t;


تعريفنوع بنية {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_ورتدا;
    ngx_log_t               *لوگارتھم;
} ngx_open_file_cache_cleanup_t;


تعريفنوع بنية {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    فراغ                    *انکڑے;
    ngx_event_t             *اقرأ;
    ngx_event_t             *اكتب;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, ویلا_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *دا, ngx_pool_t *pool);


#جے_انت /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
