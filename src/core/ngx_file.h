
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_FILE_H_INCLUDED_
#define _NGX_FILE_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ഘടന ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      offset;
    off_t                      sys_offset;

    ngx_log_t                 *log;

#എങ്കിൽ (NGX_THREADS || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    ശൂന്യം                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#endif

#എങ്കിൽ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#endif

    unsigned                   valid_info:1;
    unsigned                   directio:1;
};


#define NGX_MAX_PATH_LEVEL  3


തരനിർവചനം ngx_msec_t (*ngx_path_manager_pt) (ശൂന്യം *data);
തരനിർവചനം ngx_msec_t (*ngx_path_purger_pt) (ശൂന്യം *data);
തരനിർവചനം ശൂന്യം (*ngx_path_loader_pt) (ശൂന്യം *data);


തരനിർവചനം ഘടന {
    ngx_str_t                  name;
    size_t                     len;
    size_t                     level[NGX_MAX_PATH_LEVEL];

    ngx_path_manager_pt        manager;
    ngx_path_purger_pt         purger;
    ngx_path_loader_pt         loader;
    ശൂന്യം                      *data;

    u_char                    *conf_file;
    ngx_uint_t                 വരി;
} ngx_path_t;


തരനിർവചനം ഘടന {
    ngx_str_t                  name;
    size_t                     level[NGX_MAX_PATH_LEVEL];
} ngx_path_init_t;


തരനിർവചനം ഘടന {
    ngx_file_t                 file;
    off_t                      offset;
    ngx_path_t                *path;
    ngx_pool_t                *pool;
    അക്ഷരം                      *warn;

    ngx_uint_t                 access;

    unsigned                   log_level:8;
    unsigned                   persistent:1;
    unsigned                   clean:1;
    unsigned                   thread_write:1;
} ngx_temp_file_t;


തരനിർവചനം ഘടന {
    ngx_uint_t                 access;
    ngx_uint_t                 path_access;
    time_t                     time;
    ngx_fd_t                   fd;

    unsigned                   create_path:1;
    unsigned                   delete_file:1;

    ngx_log_t                 *log;
} ngx_ext_rename_file_t;


തരനിർവചനം ഘടന {
    off_t                      size;
    size_t                     buf_size;

    ngx_uint_t                 access;
    time_t                     time;

    ngx_log_t                 *log;
} ngx_copy_file_t;


തരനിർവചനം ഘടന ngx_വൃക്ഷം_ctx_s  ngx_വൃക്ഷം_ctx_t;

തരനിർവചനം ngx_int_t (*ngx_വൃക്ഷം_init_handler_pt) (ശൂന്യം *ctx, ശൂന്യം *prev);
തരനിർവചനം ngx_int_t (*ngx_വൃക്ഷം_handler_pt) (ngx_വൃക്ഷം_ctx_t *ctx, ngx_str_t *name);

ഘടന ngx_വൃക്ഷം_ctx_s {
    off_t                      size;
    off_t                      fs_size;
    ngx_uint_t                 access;
    time_t                     mtime;

    ngx_വൃക്ഷം_init_handler_pt   init_handler;
    ngx_വൃക്ഷം_handler_pt        file_handler;
    ngx_വൃക്ഷം_handler_pt        pre_വൃക്ഷം_handler;
    ngx_വൃക്ഷം_handler_pt        post_വൃക്ഷം_handler;
    ngx_വൃക്ഷം_handler_pt        spec_handler;

    ശൂന്യം                      *data;
    size_t                     alloc;

    ngx_log_t                 *log;
};


ngx_int_t ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix,
    ngx_str_t *name);

ssize_t ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_temp_file(ngx_file_t *file, ngx_path_t *path,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
ശൂന്യം ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len);
ngx_int_t ngx_create_path(ngx_file_t *file, ngx_path_t *path);
ngx_err_t ngx_create_full_path(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot);
ngx_int_t ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_വൃക്ഷം(ngx_വൃക്ഷം_ctx_t *ctx, ngx_str_t *വൃക്ഷം);

ngx_atomic_uint_t ngx_next_temp_സംഖ്യ(ngx_uint_t collision);

അക്ഷരം *ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_merge_path_മൂല്യം(ngx_conf_t *cf, ngx_path_t **path,
    ngx_path_t *prev, ngx_path_init_t *init);
അക്ഷരം *ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);


ബാഹ്യ ngx_atomic_t      *ngx_temp_സംഖ്യ;
ബാഹ്യ ngx_atomic_int_t   ngx_random_സംഖ്യ;


#endif /* _NGX_FILE_H_INCLUDED_ */
