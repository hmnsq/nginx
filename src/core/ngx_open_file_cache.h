
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ifndef _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#define _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#define NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_ମୂଲ୍ୟ


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    time_t                   mtime;
    off_t                    size;
    off_t                    fs_size;
    off_t                    directio;
    size_t                   read_ahead;

    ngx_err_t                err;
    ଅକ୍ଷର                    *failed;

    time_t                   valid;

    ngx_uint_t               min_uses;

#ଯଦି (NGX_HAVE_OPENAT)
    size_t                   disable_symlinks_from;
    unsigned                 disable_symlinks:2;
#endif

    unsigned                 test_dir:1;
    unsigned                 test_only:1;
    unsigned                 log:1;
    unsigned                 errors:1;
    unsigned                 events:1;

    unsigned                 is_dir:1;
    unsigned                 is_file:1;
    unsigned                 is_link:1;
    unsigned                 is_exec:1;
    unsigned                 is_directio:1;
} ngx_open_file_info_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_cached_open_file_s  ngx_cached_open_file_t;

ସଂରଚନା ngx_cached_open_file_s {
    ngx_rbtree_ଗଣ୍ଠି_t        ଗଣ୍ଠି;
    ngx_queue_t              queue;

    u_char                  *name;
    time_t                   created;
    time_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    time_t                   mtime;
    off_t                    size;
    ngx_err_t                err;

    uint32_t                 uses;

#ଯଦି (NGX_HAVE_OPENAT)
    size_t                   disable_symlinks_from;
    unsigned                 disable_symlinks:2;
#endif

    unsigned                 ଗଣନା_2:24;
    unsigned                 close:1;
    unsigned                 use_event:1;

    unsigned                 is_dir:1;
    unsigned                 is_file:1;
    unsigned                 is_link:1;
    unsigned                 is_exec:1;
    unsigned                 is_directio:1;

    ngx_event_t             *event;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_ଗଣ୍ଠି_t        sentinel;
    ngx_queue_t              expire_queue;

    ngx_uint_t               current;
    ngx_uint_t               max;
    time_t                   inactive;
} ngx_open_file_cache_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_uses;
    ngx_log_t               *log;
} ngx_open_file_cache_cleanup_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    ଶୂନ୍ୟ                    *data;
    ngx_event_t             *ପଠ;
    ngx_event_t             *ଲିଖ;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, time_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool);


#endif /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
