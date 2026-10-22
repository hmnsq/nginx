
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_FILE_H_INCLUDED_
#وصف_ڪريو _NGX_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


بنية ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      وٿي;
    off_t                      sys_وٿي;

    ngx_log_t                 *لاگرٿم;

#إذا (NGX_ڌاڳا || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    فراغ                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#جيڪڏهن_پورو

    بنانشان                   valid_info:1;
    بنانشان                   directio:1;
};


#وصف_ڪريو NGX_MAX_رستو_LEVEL  3


تعريفنوع ngx_msec_t (*ngx_رستو_manager_pt) (فراغ *معلومات);
تعريفنوع ngx_msec_t (*ngx_رستو_purger_pt) (فراغ *معلومات);
تعريفنوع فراغ (*ngx_رستو_loader_pt) (فراغ *معلومات);


تعريفنوع بنية {
    ngx_str_t                  name;
    سائيز_t                     len;
    سائيز_t                     level[NGX_MAX_رستو_LEVEL];

    ngx_رستو_manager_pt        manager;
    ngx_رستو_purger_pt         purger;
    ngx_رستو_loader_pt         loader;
    فراغ                      *معلومات;

    u_char                    *conf_file;
    ngx_uint_t                 سٽ;
} ngx_رستو_t;


تعريفنوع بنية {
    ngx_str_t                  name;
    سائيز_t                     level[NGX_MAX_رستو_LEVEL];
} ngx_رستو_init_t;


تعريفنوع بنية {
    ngx_file_t                 file;
    off_t                      وٿي;
    ngx_رستو_t                *رستو;
    ngx_pool_t                *pool;
    حرف                      *warn;

    ngx_uint_t                 access;

    بنانشان                   log_level:8;
    بنانشان                   persistent:1;
    بنانشان                   clean:1;
    بنانشان                   thread_write:1;
} ngx_عارضي_file_t;


تعريفنوع بنية {
    ngx_uint_t                 access;
    ngx_uint_t                 رستو_access;
    وقت_t                     وقت;
    ngx_fd_t                   fd;

    بنانشان                   create_رستو:1;
    بنانشان                   delete_file:1;

    ngx_log_t                 *لاگرٿم;
} ngx_ext_rename_file_t;


تعريفنوع بنية {
    off_t                      سائيز;
    سائيز_t                     buf_سائيز;

    ngx_uint_t                 access;
    وقت_t                     وقت;

    ngx_log_t                 *لاگرٿم;
} ngx_copy_file_t;


تعريفنوع بنية ngx_وڻ_ctx_s  ngx_وڻ_ctx_t;

تعريفنوع ngx_int_t (*ngx_وڻ_init_handler_pt) (فراغ *ctx, فراغ *prev);
تعريفنوع ngx_int_t (*ngx_وڻ_handler_pt) (ngx_وڻ_ctx_t *ctx, ngx_str_t *name);

بنية ngx_وڻ_ctx_s {
    off_t                      سائيز;
    off_t                      fs_سائيز;
    ngx_uint_t                 access;
    وقت_t                     mtime;

    ngx_وڻ_init_handler_pt   init_handler;
    ngx_وڻ_handler_pt        file_handler;
    ngx_وڻ_handler_pt        pre_وڻ_handler;
    ngx_وڻ_handler_pt        post_وڻ_handler;
    ngx_وڻ_handler_pt        spec_handler;

    فراغ                      *معلومات;
    سائيز_t                     alloc;

    ngx_log_t                 *لاگرٿم;
};


ngx_int_t ngx_get_ڀريل_name(ngx_pool_t *pool, ngx_str_t *اڳياڙي,
    ngx_str_t *name);

ssize_t ngx_write_chain_منزل_عارضي_file(ngx_عارضي_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_عارضي_file(ngx_file_t *file, ngx_رستو_t *رستو,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
فراغ ngx_create_hashed_filename(ngx_رستو_t *رستو, u_char *file, سائيز_t len);
ngx_int_t ngx_create_رستو(ngx_file_t *file, ngx_رستو_t *رستو);
ngx_err_t ngx_create_ڀريل_رستو(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_جوڙيو_رستو(ngx_conf_t *cf, ngx_رستو_t **جڳهه);
ngx_int_t ngx_create_paths(ngx_چڪر_t *چڪر, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *منزل,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_وڻ(ngx_وڻ_ctx_t *ctx, ngx_str_t *وڻ);

ngx_atomic_uint_t ngx_ايندڙ_عارضي_عدد(ngx_uint_t collision);

حرف *ngx_conf_سيٽ_رستو_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_ملايو_رستو_قدر(ngx_conf_t *cf, ngx_رستو_t **رستو,
    ngx_رستو_t *prev, ngx_رستو_init_t *init);
حرف *ngx_conf_سيٽ_access_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


خارجي ngx_atomic_t      *ngx_عارضي_عدد;
خارجي ngx_atomic_int_t   ngx_random_عدد;


#جيڪڏهن_پورو /* _NGX_FILE_H_INCLUDED_ */
