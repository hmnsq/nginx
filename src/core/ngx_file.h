
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_FILE_H_INCLUDED_
#تعریف_کرو _NGX_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


بنية ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      وتھ;
    off_t                      sys_وتھ;

    ngx_log_t                 *لوگارتھم;

#إذا (NGX_دھاگے || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    فراغ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#جے_انت

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#جے_انت

    بنانشانی                   valid_info:1;
    بنانشانی                   directio:1;
};


#تعریف_کرو NGX_MAX_رستہ_LEVEL  3


تعريفنوع ngx_msec_t (*ngx_رستہ_manager_pt) (فراغ *انکڑے);
تعريفنوع ngx_msec_t (*ngx_رستہ_purger_pt) (فراغ *انکڑے);
تعريفنوع فراغ (*ngx_رستہ_loader_pt) (فراغ *انکڑے);


تعريفنوع بنية {
    ngx_str_t                  name;
    ناپ_t                     len;
    ناپ_t                     level[NGX_MAX_رستہ_LEVEL];

    ngx_رستہ_manager_pt        manager;
    ngx_رستہ_purger_pt         purger;
    ngx_رستہ_loader_pt         loader;
    فراغ                      *انکڑے;

    u_char                    *conf_file;
    ngx_uint_t                 سطر;
} ngx_رستہ_t;


تعريفنوع بنية {
    ngx_str_t                  name;
    ناپ_t                     level[NGX_MAX_رستہ_LEVEL];
} ngx_رستہ_init_t;


تعريفنوع بنية {
    ngx_file_t                 file;
    off_t                      وتھ;
    ngx_رستہ_t                *رستہ;
    ngx_pool_t                *pool;
    حرف                      *warn;

    ngx_uint_t                 access;

    بنانشانی                   log_level:8;
    بنانشانی                   persistent:1;
    بنانشانی                   clean:1;
    بنانشانی                   thread_write:1;
} ngx_عارضی_file_t;


تعريفنوع بنية {
    ngx_uint_t                 access;
    ngx_uint_t                 رستہ_access;
    ویلا_t                     ویلا;
    ngx_fd_t                   fd;

    بنانشانی                   create_رستہ:1;
    بنانشانی                   delete_file:1;

    ngx_log_t                 *لوگارتھم;
} ngx_ext_rename_file_t;


تعريفنوع بنية {
    off_t                      ناپ;
    ناپ_t                     buf_ناپ;

    ngx_uint_t                 access;
    ویلا_t                     ویلا;

    ngx_log_t                 *لوگارتھم;
} ngx_copy_file_t;


تعريفنوع بنية ngx_درخت_ctx_s  ngx_درخت_ctx_t;

تعريفنوع ngx_int_t (*ngx_درخت_init_handler_pt) (فراغ *ctx, فراغ *prev);
تعريفنوع ngx_int_t (*ngx_درخت_handler_pt) (ngx_درخت_ctx_t *ctx, ngx_str_t *name);

بنية ngx_درخت_ctx_s {
    off_t                      ناپ;
    off_t                      fs_ناپ;
    ngx_uint_t                 access;
    ویلا_t                     mtime;

    ngx_درخت_init_handler_pt   init_handler;
    ngx_درخت_handler_pt        file_handler;
    ngx_درخت_handler_pt        pre_درخت_handler;
    ngx_درخت_handler_pt        post_درخت_handler;
    ngx_درخت_handler_pt        spec_handler;

    فراغ                      *انکڑے;
    ناپ_t                     alloc;

    ngx_log_t                 *لوگارتھم;
};


ngx_int_t ngx_get_بھریا_name(ngx_pool_t *pool, ngx_str_t *اگیتر,
    ngx_str_t *name);

ssize_t ngx_write_chain_منزل_عارضی_file(ngx_عارضی_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_عارضی_file(ngx_file_t *file, ngx_رستہ_t *رستہ,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
فراغ ngx_create_hashed_filename(ngx_رستہ_t *رستہ, u_char *file, ناپ_t len);
ngx_int_t ngx_create_رستہ(ngx_file_t *file, ngx_رستہ_t *رستہ);
ngx_err_t ngx_create_بھریا_رستہ(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_جوڑو_رستہ(ngx_conf_t *cf, ngx_رستہ_t **تھاں);
ngx_int_t ngx_create_paths(ngx_چکر_t *چکر, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *منزل,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_درخت(ngx_درخت_ctx_t *ctx, ngx_str_t *درخت);

ngx_atomic_uint_t ngx_اگلا_عارضی_نمبر(ngx_uint_t collision);

حرف *ngx_conf_سیٹ_رستہ_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_ملاؤ_رستہ_قدر(ngx_conf_t *cf, ngx_رستہ_t **رستہ,
    ngx_رستہ_t *prev, ngx_رستہ_init_t *init);
حرف *ngx_conf_سیٹ_access_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


خارجي ngx_atomic_t      *ngx_عارضی_نمبر;
خارجي ngx_atomic_int_t   ngx_random_نمبر;


#جے_انت /* _NGX_FILE_H_INCLUDED_ */
