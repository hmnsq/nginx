
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_FILE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_FILE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਬਣਤਰ ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      ਵਿੱਥ;
    off_t                      sys_ਵਿੱਥ;

    ngx_log_t                 *ਲਘੂਗਣਕ;

#ਜੇ (NGX_ਧਾਗੇ || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    ਖਾਲੀ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ                   valid_info:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                   directio:1;
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_ਰਸਤਾ_LEVEL  3


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_msec_t (*ngx_ਰਸਤਾ_manager_pt) (ਖਾਲੀ *ਅੰਕੜੇ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_msec_t (*ngx_ਰਸਤਾ_purger_pt) (ਖਾਲੀ *ਅੰਕੜੇ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_ਰਸਤਾ_loader_pt) (ਖਾਲੀ *ਅੰਕੜੇ);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                  name;
    ਮਾਪ_t                     len;
    ਮਾਪ_t                     level[NGX_MAX_ਰਸਤਾ_LEVEL];

    ngx_ਰਸਤਾ_manager_pt        manager;
    ngx_ਰਸਤਾ_purger_pt         purger;
    ngx_ਰਸਤਾ_loader_pt         loader;
    ਖਾਲੀ                      *ਅੰਕੜੇ;

    u_char                    *conf_file;
    ngx_uint_t                 ਸਤਰ;
} ngx_ਰਸਤਾ_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                  name;
    ਮਾਪ_t                     level[NGX_MAX_ਰਸਤਾ_LEVEL];
} ngx_ਰਸਤਾ_init_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_file_t                 file;
    off_t                      ਵਿੱਥ;
    ngx_ਰਸਤਾ_t                *ਰਸਤਾ;
    ngx_pool_t                *pool;
    ਅੱਖਰ                      *warn;

    ngx_uint_t                 access;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                   log_level:8;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                   persistent:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                   clean:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                   thread_write:1;
} ngx_ਆਰਜ਼ੀ_file_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t                 access;
    ngx_uint_t                 ਰਸਤਾ_access;
    ਸਮਾਂ_t                     ਸਮਾਂ;
    ngx_fd_t                   fd;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                   create_ਰਸਤਾ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                   delete_file:1;

    ngx_log_t                 *ਲਘੂਗਣਕ;
} ngx_ext_rename_file_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    off_t                      ਮਾਪ;
    ਮਾਪ_t                     buf_ਮਾਪ;

    ngx_uint_t                 access;
    ਸਮਾਂ_t                     ਸਮਾਂ;

    ngx_log_t                 *ਲਘੂਗਣਕ;
} ngx_copy_file_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ਰੁੱਖ_ctx_s  ngx_ਰੁੱਖ_ctx_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t (*ngx_ਰੁੱਖ_init_handler_pt) (ਖਾਲੀ *ctx, ਖਾਲੀ *prev);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t (*ngx_ਰੁੱਖ_handler_pt) (ngx_ਰੁੱਖ_ctx_t *ctx, ngx_str_t *name);

ਬਣਤਰ ngx_ਰੁੱਖ_ctx_s {
    off_t                      ਮਾਪ;
    off_t                      fs_ਮਾਪ;
    ngx_uint_t                 access;
    ਸਮਾਂ_t                     mtime;

    ngx_ਰੁੱਖ_init_handler_pt   init_handler;
    ngx_ਰੁੱਖ_handler_pt        file_handler;
    ngx_ਰੁੱਖ_handler_pt        pre_ਰੁੱਖ_handler;
    ngx_ਰੁੱਖ_handler_pt        post_ਰੁੱਖ_handler;
    ngx_ਰੁੱਖ_handler_pt        spec_handler;

    ਖਾਲੀ                      *ਅੰਕੜੇ;
    ਮਾਪ_t                     alloc;

    ngx_log_t                 *ਲਘੂਗਣਕ;
};


ngx_int_t ngx_get_ਭਰਿਆ_name(ngx_pool_t *pool, ngx_str_t *ਅਗੇਤਰ,
    ngx_str_t *name);

ssize_t ngx_write_chain_ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ_file(ngx_ਆਰਜ਼ੀ_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_ਆਰਜ਼ੀ_file(ngx_file_t *file, ngx_ਰਸਤਾ_t *ਰਸਤਾ,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
ਖਾਲੀ ngx_create_hashed_filename(ngx_ਰਸਤਾ_t *ਰਸਤਾ, u_char *file, ਮਾਪ_t len);
ngx_int_t ngx_create_ਰਸਤਾ(ngx_file_t *file, ngx_ਰਸਤਾ_t *ਰਸਤਾ);
ngx_err_t ngx_create_ਭਰਿਆ_ਰਸਤਾ(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_ਜੋੜੋ_ਰਸਤਾ(ngx_conf_t *cf, ngx_ਰਸਤਾ_t **ਥਾਂ);
ngx_int_t ngx_create_paths(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *ਮੰਜ਼ਿਲ,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *ਮੰਜ਼ਿਲ, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_ਰੁੱਖ(ngx_ਰੁੱਖ_ctx_t *ctx, ngx_str_t *ਰੁੱਖ);

ngx_atomic_uint_t ngx_ਅਗਲਾ_ਆਰਜ਼ੀ_ਸੰਖਿਆ(ngx_uint_t collision);

ਅੱਖਰ *ngx_conf_ਸੈੱਟ_ਰਸਤਾ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਮਿਲਾਓ_ਰਸਤਾ_ਮੁੱਲ(ngx_conf_t *cf, ngx_ਰਸਤਾ_t **ਰਸਤਾ,
    ngx_ਰਸਤਾ_t *prev, ngx_ਰਸਤਾ_init_t *init);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_access_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);


ਬਾਹਰੀ ngx_atomic_t      *ngx_ਆਰਜ਼ੀ_ਸੰਖਿਆ;
ਬਾਹਰੀ ngx_atomic_int_t   ngx_random_ਸੰਖਿਆ;


#ਜੇ_ਅੰਤ /* _NGX_FILE_H_INCLUDED_ */
