
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_FILE_H_INCLUDED_
#define _NGX_FILE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸಂರಚನಾ ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      offset;
    off_t                      sys_offset;

    ngx_log_t                 *log;

#ಯದಿ (NGX_THREADS || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    ಶೂನ್ಯ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#endif

#ಯದಿ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#endif

    unsigned                   valid_info:1;
    unsigned                   directio:1;
};


#define NGX_MAX_PATH_LEVEL  3


ಪ್ರಕಾರನಿರ್ದೇಶ ngx_msec_t (*ngx_path_manager_pt) (ಶೂನ್ಯ *data);
ಪ್ರಕಾರನಿರ್ದೇಶ ngx_msec_t (*ngx_path_purger_pt) (ಶೂನ್ಯ *data);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ngx_path_loader_pt) (ಶೂನ್ಯ *data);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                  name;
    size_t                     len;
    size_t                     level[NGX_MAX_PATH_LEVEL];

    ngx_path_manager_pt        manager;
    ngx_path_purger_pt         purger;
    ngx_path_loader_pt         loader;
    ಶೂನ್ಯ                      *data;

    u_char                    *conf_file;
    ngx_uint_t                 ಸಾಲು;
} ngx_path_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                  name;
    size_t                     level[NGX_MAX_PATH_LEVEL];
} ngx_path_init_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_file_t                 file;
    off_t                      offset;
    ngx_path_t                *path;
    ngx_pool_t                *pool;
    ವರ್ಣ                      *warn;

    ngx_uint_t                 access;

    unsigned                   log_level:8;
    unsigned                   persistent:1;
    unsigned                   clean:1;
    unsigned                   thread_write:1;
} ngx_temp_file_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_uint_t                 access;
    ngx_uint_t                 path_access;
    time_t                     time;
    ngx_fd_t                   fd;

    unsigned                   create_path:1;
    unsigned                   delete_file:1;

    ngx_log_t                 *log;
} ngx_ext_rename_file_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    off_t                      size;
    size_t                     buf_size;

    ngx_uint_t                 access;
    time_t                     time;

    ngx_log_t                 *log;
} ngx_copy_file_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_ವೃಕ್ಷ_ctx_s  ngx_ವೃಕ್ಷ_ctx_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ngx_int_t (*ngx_ವೃಕ್ಷ_init_handler_pt) (ಶೂನ್ಯ *ctx, ಶೂನ್ಯ *prev);
ಪ್ರಕಾರನಿರ್ದೇಶ ngx_int_t (*ngx_ವೃಕ್ಷ_handler_pt) (ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *name);

ಸಂರಚನಾ ngx_ವೃಕ್ಷ_ctx_s {
    off_t                      size;
    off_t                      fs_size;
    ngx_uint_t                 access;
    time_t                     mtime;

    ngx_ವೃಕ್ಷ_init_handler_pt   init_handler;
    ngx_ವೃಕ್ಷ_handler_pt        file_handler;
    ngx_ವೃಕ್ಷ_handler_pt        pre_ವೃಕ್ಷ_handler;
    ngx_ವೃಕ್ಷ_handler_pt        post_ವೃಕ್ಷ_handler;
    ngx_ವೃಕ್ಷ_handler_pt        spec_handler;

    ಶೂನ್ಯ                      *data;
    size_t                     alloc;

    ngx_log_t                 *log;
};


ngx_int_t ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix,
    ngx_str_t *name);

ssize_t ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_temp_file(ngx_file_t *file, ngx_path_t *path,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
ಶೂನ್ಯ ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len);
ngx_int_t ngx_create_path(ngx_file_t *file, ngx_path_t *path);
ngx_err_t ngx_create_full_path(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot);
ngx_int_t ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_ವೃಕ್ಷ(ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *ವೃಕ್ಷ);

ngx_atomic_uint_t ngx_next_temp_ಸಂಖ್ಯೆ(ngx_uint_t collision);

ವರ್ಣ *ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_merge_path_ಮೌಲ್ಯ(ngx_conf_t *cf, ngx_path_t **path,
    ngx_path_t *prev, ngx_path_init_t *init);
ವರ್ಣ *ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


ಬಾಹ್ಯ ngx_atomic_t      *ngx_temp_ಸಂಖ್ಯೆ;
ಬಾಹ್ಯ ngx_atomic_int_t   ngx_random_ಸಂಖ್ಯೆ;


#endif /* _NGX_FILE_H_INCLUDED_ */
