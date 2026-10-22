
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_FILE_H_INCLUDED_
#تعریف_کړه _NGX_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


بنية ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      دننه_واټن;
    off_t                      sys_دننه_واټن;

    ngx_log_t                 *لوګاریتم;

#إذا (NGX_تارونه || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    فراغ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#د_که_پای

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#د_که_پای

    بینښه                   valid_info:1;
    بینښه                   directio:1;
};


#تعریف_کړه NGX_MAX_لار_LEVEL  3


تعريفنوع ngx_msec_t (*ngx_لار_manager_pt) (فراغ *معلومات);
تعريفنوع ngx_msec_t (*ngx_لار_purger_pt) (فراغ *معلومات);
تعريفنوع فراغ (*ngx_لار_loader_pt) (فراغ *معلومات);


تعريفنوع بنية {
    ngx_str_t                  name;
    اندازه_t                     len;
    اندازه_t                     level[NGX_MAX_لار_LEVEL];

    ngx_لار_manager_pt        manager;
    ngx_لار_purger_pt         purger;
    ngx_لار_loader_pt         loader;
    فراغ                      *معلومات;

    u_char                    *conf_file;
    ngx_uint_t                 کرښه;
} ngx_لار_t;


تعريفنوع بنية {
    ngx_str_t                  name;
    اندازه_t                     level[NGX_MAX_لار_LEVEL];
} ngx_لار_init_t;


تعريفنوع بنية {
    ngx_file_t                 file;
    off_t                      دننه_واټن;
    ngx_لار_t                *لار;
    ngx_pool_t                *pool;
    حرف                      *warn;

    ngx_uint_t                 access;

    بینښه                   log_level:8;
    بینښه                   persistent:1;
    بینښه                   clean:1;
    بینښه                   thread_write:1;
} ngx_لنډمهاله_file_t;


تعريفنوع بنية {
    ngx_uint_t                 access;
    ngx_uint_t                 لار_access;
    وخت_t                     وخت;
    ngx_fd_t                   fd;

    بینښه                   create_لار:1;
    بینښه                   delete_file:1;

    ngx_log_t                 *لوګاریتم;
} ngx_ext_rename_file_t;


تعريفنوع بنية {
    off_t                      اندازه;
    اندازه_t                     buf_اندازه;

    ngx_uint_t                 access;
    وخت_t                     وخت;

    ngx_log_t                 *لوګاریتم;
} ngx_copy_file_t;


تعريفنوع بنية ngx_ونه_ctx_s  ngx_ونه_ctx_t;

تعريفنوع ngx_int_t (*ngx_ونه_init_handler_pt) (فراغ *ctx, فراغ *prev);
تعريفنوع ngx_int_t (*ngx_ونه_handler_pt) (ngx_ونه_ctx_t *ctx, ngx_str_t *name);

بنية ngx_ونه_ctx_s {
    off_t                      اندازه;
    off_t                      fs_اندازه;
    ngx_uint_t                 access;
    وخت_t                     mtime;

    ngx_ونه_init_handler_pt   init_handler;
    ngx_ونه_handler_pt        file_handler;
    ngx_ونه_handler_pt        pre_ونه_handler;
    ngx_ونه_handler_pt        post_ونه_handler;
    ngx_ونه_handler_pt        spec_handler;

    فراغ                      *معلومات;
    اندازه_t                     alloc;

    ngx_log_t                 *لوګاریتم;
};


ngx_int_t ngx_get_ډک_name(ngx_pool_t *pool, ngx_str_t *مختاړی,
    ngx_str_t *name);

ssize_t ngx_write_chain_منزل_لنډمهاله_file(ngx_لنډمهاله_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_لنډمهاله_file(ngx_file_t *file, ngx_لار_t *لار,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
فراغ ngx_create_hashed_filename(ngx_لار_t *لار, u_char *file, اندازه_t len);
ngx_int_t ngx_create_لار(ngx_file_t *file, ngx_لار_t *لار);
ngx_err_t ngx_create_ډک_لار(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_جمع_کړه_لار(ngx_conf_t *cf, ngx_لار_t **خونه);
ngx_int_t ngx_create_paths(ngx_دوره_t *دوره, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *منزل,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_ونه(ngx_ونه_ctx_t *ctx, ngx_str_t *ونه);

ngx_atomic_uint_t ngx_بل_لنډمهاله_شمېره(ngx_uint_t collision);

حرف *ngx_conf_سېټ_لار_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_یوځای_لار_ارزښت(ngx_conf_t *cf, ngx_لار_t **لار,
    ngx_لار_t *prev, ngx_لار_init_t *init);
حرف *ngx_conf_سېټ_access_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


خارجي ngx_atomic_t      *ngx_لنډمهاله_شمېره;
خارجي ngx_atomic_int_t   ngx_random_شمېره;


#د_که_پای /* _NGX_FILE_H_INCLUDED_ */
