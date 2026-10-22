
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_FILE_H_INCLUDED_
#تعریف_کرو _NGX_FILE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساخت ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      انحراف;
    off_t                      sys_انحراف;

    ngx_log_t                 *لوگارتھم;

#اگر (NGX_دھاگے || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    خالی                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#اگر_ختم

#اگر (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#اگر_ختم

    غیرعلامتی                   valid_info:1;
    غیرعلامتی                   directio:1;
};


#تعریف_کرو NGX_MAX_راستہ_LEVEL  3


قسمتعریف ngx_msec_t (*ngx_راستہ_manager_pt) (خالی *ڈیٹا);
قسمتعریف ngx_msec_t (*ngx_راستہ_purger_pt) (خالی *ڈیٹا);
قسمتعریف خالی (*ngx_راستہ_loader_pt) (خالی *ڈیٹا);


قسمتعریف ساخت {
    ngx_str_t                  name;
    سائز_t                     len;
    سائز_t                     level[NGX_MAX_راستہ_LEVEL];

    ngx_راستہ_manager_pt        manager;
    ngx_راستہ_purger_pt         purger;
    ngx_راستہ_loader_pt         loader;
    خالی                      *ڈیٹا;

    u_char                    *conf_file;
    ngx_uint_t                 سطر;
} ngx_راستہ_t;


قسمتعریف ساخت {
    ngx_str_t                  name;
    سائز_t                     level[NGX_MAX_راستہ_LEVEL];
} ngx_راستہ_init_t;


قسمتعریف ساخت {
    ngx_file_t                 file;
    off_t                      انحراف;
    ngx_راستہ_t                *راستہ;
    ngx_pool_t                *pool;
    حرف                      *warn;

    ngx_uint_t                 access;

    غیرعلامتی                   log_level:8;
    غیرعلامتی                   persistent:1;
    غیرعلامتی                   clean:1;
    غیرعلامتی                   thread_write:1;
} ngx_عارضی_file_t;


قسمتعریف ساخت {
    ngx_uint_t                 access;
    ngx_uint_t                 راستہ_access;
    وقت_t                     وقت;
    ngx_fd_t                   fd;

    غیرعلامتی                   create_راستہ:1;
    غیرعلامتی                   delete_file:1;

    ngx_log_t                 *لوگارتھم;
} ngx_ext_rename_file_t;


قسمتعریف ساخت {
    off_t                      سائز;
    سائز_t                     buf_سائز;

    ngx_uint_t                 access;
    وقت_t                     وقت;

    ngx_log_t                 *لوگارتھم;
} ngx_copy_file_t;


قسمتعریف ساخت ngx_درخت_ctx_s  ngx_درخت_ctx_t;

قسمتعریف ngx_int_t (*ngx_درخت_init_handler_pt) (خالی *ctx, خالی *پچھلا);
قسمتعریف ngx_int_t (*ngx_درخت_handler_pt) (ngx_درخت_ctx_t *ctx, ngx_str_t *name);

ساخت ngx_درخت_ctx_s {
    off_t                      سائز;
    off_t                      fs_سائز;
    ngx_uint_t                 access;
    وقت_t                     mtime;

    ngx_درخت_init_handler_pt   init_handler;
    ngx_درخت_handler_pt        file_handler;
    ngx_درخت_handler_pt        pre_درخت_handler;
    ngx_درخت_handler_pt        post_درخت_handler;
    ngx_درخت_handler_pt        spec_handler;

    خالی                      *ڈیٹا;
    سائز_t                     alloc;

    ngx_log_t                 *لوگارتھم;
};


ngx_int_t ngx_get_بھرا_name(ngx_pool_t *pool, ngx_str_t *سابقہ,
    ngx_str_t *name);

ssize_t ngx_write_chain_مقصد_عارضی_file(ngx_عارضی_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_عارضی_file(ngx_file_t *file, ngx_راستہ_t *راستہ,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
خالی ngx_create_hashed_filename(ngx_راستہ_t *راستہ, u_char *file, سائز_t len);
ngx_int_t ngx_create_راستہ(ngx_file_t *file, ngx_راستہ_t *راستہ);
ngx_err_t ngx_create_بھرا_راستہ(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_جوڑو_راستہ(ngx_conf_t *cf, ngx_راستہ_t **خانہ);
ngx_int_t ngx_create_paths(ngx_چکر_t *چکر, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *مقصد,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *مقصد, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_درخت(ngx_درخت_ctx_t *ctx, ngx_str_t *درخت);

ngx_atomic_uint_t ngx_اگلا_عارضی_نمبر(ngx_uint_t collision);

حرف *ngx_conf_سیٹ_راستہ_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_ملاؤ_راستہ_قدر(ngx_conf_t *cf, ngx_راستہ_t **راستہ,
    ngx_راستہ_t *پچھلا, ngx_راستہ_init_t *init);
حرف *ngx_conf_سیٹ_access_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);


خارجی ngx_atomic_t      *ngx_عارضی_نمبر;
خارجی ngx_atomic_int_t   ngx_random_نمبر;


#اگر_ختم /* _NGX_FILE_H_INCLUDED_ */
