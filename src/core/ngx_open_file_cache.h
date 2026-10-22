
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#ifndef _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#define _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#define NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_मान


प्रकारनिर्देश संरचना {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    समय_प्रकार                   mtime;
    off_t                    size;
    off_t                    fs_size;
    off_t                    directio;
    माप_प्रकार                   read_ahead;

    ngx_err_t                err;
    वर्ण                    *failed;

    समय_प्रकार                   valid;

    ngx_uint_t               min_uses;

#यदि (NGX_HAVE_OPENAT)
    माप_प्रकार                   disable_symlinks_from;
    अचिन्हित                 disable_symlinks:2;
#endif

    अचिन्हित                 test_dir:1;
    अचिन्हित                 test_only:1;
    अचिन्हित                 log:1;
    अचिन्हित                 errors:1;
    अचिन्हित                 events:1;

    अचिन्हित                 is_dir:1;
    अचिन्हित                 is_file:1;
    अचिन्हित                 is_link:1;
    अचिन्हित                 is_exec:1;
    अचिन्हित                 is_directio:1;
} ngx_open_file_info_t;


प्रकारनिर्देश संरचना ngx_cached_open_file_s  ngx_cached_open_file_t;

संरचना ngx_cached_open_file_s {
    ngx_rbtree_गाँठो_t        गाँठो;
    ngx_queue_t              queue;

    u_char                  *name;
    समय_प्रकार                   created;
    समय_प्रकार                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    समय_प्रकार                   mtime;
    off_t                    size;
    ngx_err_t                err;

    uint32_t                 uses;

#यदि (NGX_HAVE_OPENAT)
    माप_प्रकार                   disable_symlinks_from;
    अचिन्हित                 disable_symlinks:2;
#endif

    अचिन्हित                 गन्ती:24;
    अचिन्हित                 बंद:1;
    अचिन्हित                 use_event:1;

    अचिन्हित                 is_dir:1;
    अचिन्हित                 is_file:1;
    अचिन्हित                 is_link:1;
    अचिन्हित                 is_exec:1;
    अचिन्हित                 is_directio:1;

    ngx_event_t             *event;
};


प्रकारनिर्देश संरचना {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_गाँठो_t        sentinel;
    ngx_queue_t              expire_queue;

    ngx_uint_t               current;
    ngx_uint_t               max;
    समय_प्रकार                   inactive;
} ngx_open_file_cache_t;


प्रकारनिर्देश संरचना {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_uses;
    ngx_log_t               *log;
} ngx_open_file_cache_cleanup_t;


प्रकारनिर्देश संरचना {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    शून्य                    *data;
    ngx_event_t             *पठ;
    ngx_event_t             *लिख;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, समय_प्रकार inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool);


#endif /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
