
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_FILE_H_INCLUDED_
#घोषणा _NGX_FILE_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


काष्ठा ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      offset;
    off_t                      sys_offset;

    ngx_log_t                 *log;

#अगर (NGX_THREADS || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    व्योम                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#पूर्ण_अगर

#अगर (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#पूर्ण_अगर

    अचिन्हित                   valid_info:1;
    अचिन्हित                   directio:1;
};


#घोषणा NGX_MAX_PATH_LEVEL  3


प्रकार ngx_msec_t (*ngx_path_manager_pt) (व्योम *तथ्य);
प्रकार ngx_msec_t (*ngx_path_purger_pt) (व्योम *तथ्य);
प्रकार व्योम (*ngx_path_loader_pt) (व्योम *तथ्य);


प्रकार काष्ठा {
    ngx_str_t                  name;
    माप_प्रकार                     len;
    माप_प्रकार                     level[NGX_MAX_PATH_LEVEL];

    ngx_path_manager_pt        manager;
    ngx_path_purger_pt         purger;
    ngx_path_loader_pt         loader;
    व्योम                      *तथ्य;

    u_char                    *conf_file;
    ngx_uint_t                 पंक्ति;
} ngx_path_t;


प्रकार काष्ठा {
    ngx_str_t                  name;
    माप_प्रकार                     level[NGX_MAX_PATH_LEVEL];
} ngx_path_init_t;


प्रकार काष्ठा {
    ngx_file_t                 file;
    off_t                      offset;
    ngx_path_t                *path;
    ngx_pool_t                *pool;
    अक्षर                      *warn;

    ngx_uint_t                 access;

    अचिन्हित                   log_level:8;
    अचिन्हित                   persistent:1;
    अचिन्हित                   clean:1;
    अचिन्हित                   thread_write:1;
} ngx_अस्थायी_file_t;


प्रकार काष्ठा {
    ngx_uint_t                 access;
    ngx_uint_t                 path_access;
    समय_प्रकार                     समय;
    ngx_fd_t                   fd;

    अचिन्हित                   create_path:1;
    अचिन्हित                   delete_file:1;

    ngx_log_t                 *log;
} ngx_ext_rename_file_t;


प्रकार काष्ठा {
    off_t                      आकार;
    माप_प्रकार                     buf_आकार;

    ngx_uint_t                 access;
    समय_प्रकार                     समय;

    ngx_log_t                 *log;
} ngx_copy_file_t;


प्रकार काष्ठा ngx_वृक्ष_ctx_s  ngx_वृक्ष_ctx_t;

प्रकार ngx_int_t (*ngx_वृक्ष_init_handler_pt) (व्योम *ctx, व्योम *पूर्व);
प्रकार ngx_int_t (*ngx_वृक्ष_handler_pt) (ngx_वृक्ष_ctx_t *ctx, ngx_str_t *name);

काष्ठा ngx_वृक्ष_ctx_s {
    off_t                      आकार;
    off_t                      fs_आकार;
    ngx_uint_t                 access;
    समय_प्रकार                     mtime;

    ngx_वृक्ष_init_handler_pt   init_handler;
    ngx_वृक्ष_handler_pt        file_handler;
    ngx_वृक्ष_handler_pt        pre_वृक्ष_handler;
    ngx_वृक्ष_handler_pt        post_वृक्ष_handler;
    ngx_वृक्ष_handler_pt        spec_handler;

    व्योम                      *तथ्य;
    माप_प्रकार                     alloc;

    ngx_log_t                 *log;
};


ngx_int_t ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix,
    ngx_str_t *name);

ssize_t ngx_write_chain_to_अस्थायी_file(ngx_अस्थायी_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_अस्थायी_file(ngx_file_t *file, ngx_path_t *path,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
व्योम ngx_create_hashed_filename(ngx_path_t *path, u_char *file, माप_प्रकार len);
ngx_int_t ngx_create_path(ngx_file_t *file, ngx_path_t *path);
ngx_err_t ngx_create_full_path(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot);
ngx_int_t ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_वृक्ष(ngx_वृक्ष_ctx_t *ctx, ngx_str_t *वृक्ष);

ngx_atomic_uint_t ngx_अग्र_अस्थायी_संख्या(ngx_uint_t collision);

अक्षर *ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_merge_path_मूल्य(ngx_conf_t *cf, ngx_path_t **path,
    ngx_path_t *पूर्व, ngx_path_init_t *init);
अक्षर *ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);


बाह्य ngx_atomic_t      *ngx_अस्थायी_संख्या;
बाह्य ngx_atomic_int_t   ngx_random_संख्या;


#पूर्ण_अगर /* _NGX_FILE_H_INCLUDED_ */
