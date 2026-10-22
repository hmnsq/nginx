
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచితం_కాకపోతే _NGX_OPEN_FILE_CACHE_H_INCLUDED_
#నిర్వచించు _NGX_OPEN_FILE_CACHE_H_INCLUDED_


#నిర్వచించు NGX_OPEN_FILE_DIRECTIO_OFF  NGX_MAX_OFF_T_విలువ


రకనిర్వచనం నిర్మాణం {
    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    సమయం_t                   mtime;
    off_t                    కొలత;
    off_t                    fs_కొలత;
    off_t                    directio;
    కొలత_t                   read_ahead;

    ngx_err_t                err;
    అక్షరం                    *failed;

    సమయం_t                   valid;

    ngx_uint_t               min_ఉపయోగిస్తుంది;

#ఒకవేళ (NGX_HAVE_OPENAT)
    కొలత_t                   disable_symlinks_from;
    గుర్తులేని                 disable_symlinks:2;
#అయితే_ముగింపు

    గుర్తులేని                 test_dir:1;
    గుర్తులేని                 test_only:1;
    గుర్తులేని                 సంవర్గమానం:1;
    గుర్తులేని                 errors:1;
    గుర్తులేని                 events:1;

    గుర్తులేని                 is_dir:1;
    గుర్తులేని                 is_file:1;
    గుర్తులేని                 is_link:1;
    గుర్తులేని                 is_exec:1;
    గుర్తులేని                 is_directio:1;
} ngx_open_file_info_t;


రకనిర్వచనం నిర్మాణం ngx_cached_open_file_s  ngx_cached_open_file_t;

నిర్మాణం ngx_cached_open_file_s {
    ngx_rbtree_కణుపు_t        కణుపు;
    ngx_వరుస_t              వరుస;

    u_char                  *name;
    సమయం_t                   created;
    సమయం_t                   accessed;

    ngx_fd_t                 fd;
    ngx_file_uniq_t          uniq;
    సమయం_t                   mtime;
    off_t                    కొలత;
    ngx_err_t                err;

    uint32_t                 ఉపయోగిస్తుంది;

#ఒకవేళ (NGX_HAVE_OPENAT)
    కొలత_t                   disable_symlinks_from;
    గుర్తులేని                 disable_symlinks:2;
#అయితే_ముగింపు

    గుర్తులేని                 లెక్క:24;
    గుర్తులేని                 close:1;
    గుర్తులేని                 use_event:1;

    గుర్తులేని                 is_dir:1;
    గుర్తులేని                 is_file:1;
    గుర్తులేని                 is_link:1;
    గుర్తులేని                 is_exec:1;
    గుర్తులేని                 is_directio:1;

    ngx_event_t             *event;
};


రకనిర్వచనం నిర్మాణం {
    ngx_rbtree_t             rbtree;
    ngx_rbtree_కణుపు_t        sentinel;
    ngx_వరుస_t              expire_వరుస;

    ngx_uint_t               current;
    ngx_uint_t               max;
    సమయం_t                   inactive;
} ngx_open_file_cache_t;


రకనిర్వచనం నిర్మాణం {
    ngx_open_file_cache_t   *cache;
    ngx_cached_open_file_t  *file;
    ngx_uint_t               min_ఉపయోగిస్తుంది;
    ngx_log_t               *సంవర్గమానం;
} ngx_open_file_cache_cleanup_t;


రకనిర్వచనం నిర్మాణం {

    /* ngx_connection_t stub to allow use c->fd as event ident */
    శూన్య                    *దత్తాంశం;
    ngx_event_t             *పఠ;
    ngx_event_t             *లిఖ;
    ngx_fd_t                 fd;

    ngx_cached_open_file_t  *file;
    ngx_open_file_cache_t   *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, సమయం_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *యొక్క, ngx_pool_t *pool);


#అయితే_ముగింపు /* _NGX_OPEN_FILE_CACHE_H_INCLUDED_ */
