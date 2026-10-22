
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि_अपरिभाषितम् _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#परिभाषय _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#परिभाषय NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_मानम्


प्रकारनिर्देश संरचना {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    time_t                   mtime;
    off_t                    आकार;
    off_t                    fs_आकार;
    off_t                    directio;
    आकार_t                   read_ahead;

    ngx_err_t                err;
    वर्ण                    *failed;

    time_t                   valid;

    ngx_uint_t               min_उपयुङ्क्ते;

#यदि (NGX_HAVE_OPENAT)
    आकार_t                   disable_symlinks_from;
    अचिह्नित                 disable_symlinks:2;
#यदि_अन्तः

    अचिह्नित                 test_dir:1;
    अचिह्नित                 test_only:1;
    अचिह्नित                 लघुगणकः:1;
    अचिह्नित                 errors:1;
    अचिह्नित                 events:1;

    अचिह्नित                 is_dir:1;
    अचिह्नित                 is_file:1;
    अचिह्नित                 is_link:1;
    अचिह्नित                 is_exec:1;
    अचिह्नित                 is_directio:1;
} ngx_open_file_info_t;


प्रकारनिर्देश संरचना ngx_cached_open_file_s  ngx_cached_open_file_t;

संरचना ngx_cached_open_file_s {
    ngx_rbtree_ग्रन्थिः_t        ग्रन्थिः;
    ngx_queue_t              queue;

    u_char                  *name;
    time_t                   created;
    time_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    time_t                   mtime;
    off_t                    आकार;
    ngx_err_t                err;

    uint32_t                 उपयुङ्क्ते;

#यदि (NGX_HAVE_OPENAT)
    आकार_t                   disable_symlinks_from;
    अचिह्नित                 disable_symlinks:2;
#यदि_अन्तः

    अचिह्नित                 गणनम्:24;
    अचिह्नित                 close:1;
    अचिह्नित                 use_event:1;

    अचिह्नित                 is_dir:1;
    अचिह्नित                 is_file:1;
    अचिह्नित                 is_link:1;
    अचिह्नित                 is_exec:1;
    अचिह्नित                 is_directio:1;

    ngx_event_t             *event;
};


प्रकारनिर्देश संरचना {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_ग्रन्थिः_t        sentinel;
    ngx_queue_t              expire_queue;

    ngx_uint_t               current;
    ngx_uint_t               max;
    time_t                   inactive;
} ngx_open_file_cache_t;


प्रकारनिर्देश संरचना {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_उपयुङ्क्ते;
    ngx_log_t               *लघुगणकः;
} ngx_open_file_cache_cleanup_t;


प्रकारनिर्देश संरचना {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    शून्य                    *दत्त;
    ngx_event_t             *पठ;
    ngx_event_t             *लिख;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, time_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *सम्बन्धि, ngx_pool_t *pool);


#यदि_अन्तः /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
