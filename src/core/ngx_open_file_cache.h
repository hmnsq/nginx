
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_ਮੁੱਲ


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ਸਮਾਂ_t                   mtime;
    off_t                    ਮਾਪ;
    off_t                    fs_ਮਾਪ;
    off_t                    directio;
    ਮਾਪ_t                   read_ahead;

    ngx_err_t                err;
    ਅੱਖਰ                    *failed;

    ਸਮਾਂ_t                   valid;

    ngx_uint_t               min_ਵਰਤਦਾ;

#ਜੇ (NGX_HAVE_OPENAT)
    ਮਾਪ_t                   disable_symlinks_from;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 disable_symlinks:2;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ                 test_dir:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 test_only:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 ਲਘੂਗਣਕ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 errors:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 events:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_dir:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_file:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_link:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_exec:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_directio:1;
} ngx_open_file_info_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_cached_open_file_s  ngx_cached_open_file_t;

ਬਣਤਰ ngx_cached_open_file_s {
    ngx_rbtree_ਗੰਢ_t        ਗੰਢ;
    ngx_ਕਤਾਰ_t              ਕਤਾਰ;

    u_char                  *name;
    ਸਮਾਂ_t                   created;
    ਸਮਾਂ_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    ਸਮਾਂ_t                   mtime;
    off_t                    ਮਾਪ;
    ngx_err_t                err;

    uint32_t                 ਵਰਤਦਾ;

#ਜੇ (NGX_HAVE_OPENAT)
    ਮਾਪ_t                   disable_symlinks_from;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 disable_symlinks:2;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ                 ਗਿਣਤੀ_2:24;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 close:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 use_event:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_dir:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_file:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_link:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_exec:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                 is_directio:1;

    ngx_event_t             *event;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_ਗੰਢ_t        sentinel;
    ngx_ਕਤਾਰ_t              expire_ਕਤਾਰ;

    ngx_uint_t               current;
    ngx_uint_t               max;
    ਸਮਾਂ_t                   inactive;
} ngx_open_file_cache_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_ਵਰਤਦਾ;
    ngx_log_t               *ਲਘੂਗਣਕ;
} ngx_open_file_cache_cleanup_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    ਖਾਲੀ                    *ਅੰਕੜੇ;
    ngx_event_t             *ਪਠ;
    ngx_event_t             *ਲਿਖ;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, ਸਮਾਂ_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *ਦਾ, ngx_pool_t *pool);


#ਜੇ_ਅੰਤ /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
