
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_FILE_H_INCLUDED_
#تعریف_کن _NGX_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


بنية ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      جابه_جایی;
    off_t                      sys_جابه_جایی;

    ngx_log_t                 *لگاریتم;

#إذا (NGX_نخها || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    فراغ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#پایان_اگر

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#پایان_اگر

    بیعلامت                   valid_info:1;
    بیعلامت                   directio:1;
};


#تعریف_کن NGX_MAX_مسیر_LEVEL  3


تعريفنوع ngx_msec_t (*ngx_مسیر_manager_pt) (فراغ *داده);
تعريفنوع ngx_msec_t (*ngx_مسیر_purger_pt) (فراغ *داده);
تعريفنوع فراغ (*ngx_مسیر_loader_pt) (فراغ *داده);


تعريفنوع بنية {
    ngx_str_t                  name;
    اندازه_t                     len;
    اندازه_t                     level[NGX_MAX_مسیر_LEVEL];

    ngx_مسیر_manager_pt        manager;
    ngx_مسیر_purger_pt         purger;
    ngx_مسیر_loader_pt         loader;
    فراغ                      *داده;

    u_char                    *conf_file;
    ngx_uint_t                 سطر;
} ngx_مسیر_t;


تعريفنوع بنية {
    ngx_str_t                  name;
    اندازه_t                     level[NGX_MAX_مسیر_LEVEL];
} ngx_مسیر_init_t;


تعريفنوع بنية {
    ngx_file_t                 file;
    off_t                      جابه_جایی;
    ngx_مسیر_t                *مسیر;
    ngx_pool_t                *pool;
    حرف                      *warn;

    ngx_uint_t                 access;

    بیعلامت                   log_level:8;
    بیعلامت                   persistent:1;
    بیعلامت                   clean:1;
    بیعلامت                   thread_write:1;
} ngx_موقت_file_t;


تعريفنوع بنية {
    ngx_uint_t                 access;
    ngx_uint_t                 مسیر_access;
    زمان_t                     زمان;
    ngx_fd_t                   fd;

    بیعلامت                   create_مسیر:1;
    بیعلامت                   delete_file:1;

    ngx_log_t                 *لگاریتم;
} ngx_ext_rename_file_t;


تعريفنوع بنية {
    off_t                      اندازه;
    اندازه_t                     buf_اندازه;

    ngx_uint_t                 access;
    زمان_t                     زمان;

    ngx_log_t                 *لگاریتم;
} ngx_copy_file_t;


تعريفنوع بنية ngx_درخت_ctx_s  ngx_درخت_ctx_t;

تعريفنوع ngx_int_t (*ngx_درخت_init_handler_pt) (فراغ *ctx, فراغ *prev);
تعريفنوع ngx_int_t (*ngx_درخت_handler_pt) (ngx_درخت_ctx_t *ctx, ngx_str_t *name);

بنية ngx_درخت_ctx_s {
    off_t                      اندازه;
    off_t                      fs_اندازه;
    ngx_uint_t                 access;
    زمان_t                     mtime;

    ngx_درخت_init_handler_pt   init_handler;
    ngx_درخت_handler_pt        file_handler;
    ngx_درخت_handler_pt        pre_درخت_handler;
    ngx_درخت_handler_pt        post_درخت_handler;
    ngx_درخت_handler_pt        spec_handler;

    فراغ                      *داده;
    اندازه_t                     alloc;

    ngx_log_t                 *لگاریتم;
};


ngx_int_t ngx_get_پر_name(ngx_pool_t *pool, ngx_str_t *پیشوند,
    ngx_str_t *name);

ssize_t ngx_write_chain_مقصد_موقت_file(ngx_موقت_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_موقت_file(ngx_file_t *file, ngx_مسیر_t *مسیر,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
فراغ ngx_create_hashed_filename(ngx_مسیر_t *مسیر, u_char *file, اندازه_t len);
ngx_int_t ngx_create_مسیر(ngx_file_t *file, ngx_مسیر_t *مسیر);
ngx_err_t ngx_create_پر_مسیر(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_جمع_کن_مسیر(ngx_conf_t *cf, ngx_مسیر_t **جایگاه);
ngx_int_t ngx_create_paths(ngx_چرخه_t *چرخه, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *مقصد,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *مقصد, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_درخت(ngx_درخت_ctx_t *ctx, ngx_str_t *درخت);

ngx_atomic_uint_t ngx_بعدی_موقت_عدد(ngx_uint_t collision);

حرف *ngx_conf_مجموعه_مسیر_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_ادغام_مسیر_مقدار(ngx_conf_t *cf, ngx_مسیر_t **مسیر,
    ngx_مسیر_t *prev, ngx_مسیر_init_t *init);
حرف *ngx_conf_مجموعه_access_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


خارجي ngx_atomic_t      *ngx_موقت_عدد;
خارجي ngx_atomic_int_t   ngx_random_عدد;


#پایان_اگر /* _NGX_FILE_H_INCLUDED_ */
