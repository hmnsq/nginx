
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_FILE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_FILE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ರಚನೆ ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      ಸ್ಥಾನಾಂತರ;
    off_t                      sys_ಸ್ಥಾನಾಂತರ;

    ngx_log_t                 *ಲಘುಗಣಕ;

#ಆದರೆ (NGX_ಎಳೆಗಳು || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    ಶೂನ್ಯ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#ಆದರೆ_ಅಂತ್ಯ

    ಚಿಹ್ನೆರಹಿತ                   valid_info:1;
    ಚಿಹ್ನೆರಹಿತ                   directio:1;
};


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_ಮಾರ್ಗ_LEVEL  3


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_msec_t (*ngx_ಮಾರ್ಗ_manager_pt) (ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_msec_t (*ngx_ಮಾರ್ಗ_purger_pt) (ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_ಮಾರ್ಗ_loader_pt) (ಶೂನ್ಯ *ದತ್ತಾಂಶ);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                  name;
    ಗಾತ್ರ_t                     len;
    ಗಾತ್ರ_t                     level[NGX_MAX_ಮಾರ್ಗ_LEVEL];

    ngx_ಮಾರ್ಗ_manager_pt        manager;
    ngx_ಮಾರ್ಗ_purger_pt         purger;
    ngx_ಮಾರ್ಗ_loader_pt         loader;
    ಶೂನ್ಯ                      *ದತ್ತಾಂಶ;

    u_char                    *conf_file;
    ngx_uint_t                 ಸಾಲು;
} ngx_ಮಾರ್ಗ_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                  name;
    ಗಾತ್ರ_t                     level[NGX_MAX_ಮಾರ್ಗ_LEVEL];
} ngx_ಮಾರ್ಗ_init_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_file_t                 file;
    off_t                      ಸ್ಥಾನಾಂತರ;
    ngx_ಮಾರ್ಗ_t                *ಮಾರ್ಗ;
    ngx_pool_t                *pool;
    ಅಕ್ಷರ                      *warn;

    ngx_uint_t                 access;

    ಚಿಹ್ನೆರಹಿತ                   log_level:8;
    ಚಿಹ್ನೆರಹಿತ                   persistent:1;
    ಚಿಹ್ನೆರಹಿತ                   clean:1;
    ಚಿಹ್ನೆರಹಿತ                   thread_write:1;
} ngx_ತಾತ್ಕಾಲಿಕ_file_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t                 access;
    ngx_uint_t                 ಮಾರ್ಗ_access;
    ಸಮಯ_t                     ಸಮಯ;
    ngx_fd_t                   fd;

    ಚಿಹ್ನೆರಹಿತ                   create_ಮಾರ್ಗ:1;
    ಚಿಹ್ನೆರಹಿತ                   delete_file:1;

    ngx_log_t                 *ಲಘುಗಣಕ;
} ngx_ext_rename_file_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    off_t                      ಗಾತ್ರ_2;
    ಗಾತ್ರ_t                     buf_ಗಾತ್ರ;

    ngx_uint_t                 access;
    ಸಮಯ_t                     ಸಮಯ;

    ngx_log_t                 *ಲಘುಗಣಕ;
} ngx_copy_file_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_ವೃಕ್ಷ_ctx_s  ngx_ವೃಕ್ಷ_ctx_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t (*ngx_ವೃಕ್ಷ_init_handler_pt) (ಶೂನ್ಯ *ctx, ಶೂನ್ಯ *prev);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t (*ngx_ವೃಕ್ಷ_handler_pt) (ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *name);

ರಚನೆ ngx_ವೃಕ್ಷ_ctx_s {
    off_t                      ಗಾತ್ರ_2;
    off_t                      fs_ಗಾತ್ರ;
    ngx_uint_t                 access;
    ಸಮಯ_t                     mtime;

    ngx_ವೃಕ್ಷ_init_handler_pt   init_handler;
    ngx_ವೃಕ್ಷ_handler_pt        file_handler;
    ngx_ವೃಕ್ಷ_handler_pt        pre_ವೃಕ್ಷ_handler;
    ngx_ವೃಕ್ಷ_handler_pt        post_ವೃಕ್ಷ_handler;
    ngx_ವೃಕ್ಷ_handler_pt        spec_handler;

    ಶೂನ್ಯ                      *ದತ್ತಾಂಶ;
    ಗಾತ್ರ_t                     alloc;

    ngx_log_t                 *ಲಘುಗಣಕ;
};


ngx_int_t ngx_get_ತುಂಬಿದ_name(ngx_pool_t *pool, ngx_str_t *ಪೂರ್ವಪ್ರತ್ಯಯ,
    ngx_str_t *name);

ssize_t ngx_write_chain_ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ_file(ngx_ತಾತ್ಕಾಲಿಕ_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_ತಾತ್ಕಾಲಿಕ_file(ngx_file_t *file, ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
ಶೂನ್ಯ ngx_create_hashed_filename(ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ, u_char *file, ಗಾತ್ರ_t len);
ngx_int_t ngx_create_ಮಾರ್ಗ(ngx_file_t *file, ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ);
ngx_err_t ngx_create_ತುಂಬಿದ_ಮಾರ್ಗ(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_ಕೂಡಿಸು_ಮಾರ್ಗ(ngx_conf_t *cf, ngx_ಮಾರ್ಗ_t **ಸ್ಥಾನ);
ngx_int_t ngx_create_paths(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *ಗಮ್ಯ,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *ಗಮ್ಯ, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_ವೃಕ್ಷ(ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *ವೃಕ್ಷ);

ngx_atomic_uint_t ngx_ಮುಂದಿನ_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ(ngx_uint_t collision);

ಅಕ್ಷರ *ngx_conf_ಗಣ_ಮಾರ್ಗ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ವಿಲೀನ_ಮಾರ್ಗ_ಮೌಲ್ಯ(ngx_conf_t *cf, ngx_ಮಾರ್ಗ_t **ಮಾರ್ಗ,
    ngx_ಮಾರ್ಗ_t *prev, ngx_ಮಾರ್ಗ_init_t *init);
ಅಕ್ಷರ *ngx_conf_ಗಣ_access_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


ಬಾಹ್ಯ ngx_atomic_t      *ngx_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ;
ಬಾಹ್ಯ ngx_atomic_int_t   ngx_random_ಸಂಖ್ಯೆ;


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_FILE_H_INCLUDED_ */
