
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_FILE_H_INCLUDED_
#परिभाषय _NGX_FILE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


संरचना ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      विस्थापनम्;
    off_t                      sys_विस्थापनम्;

    ngx_log_t                 *लघुगणकः;

#यदि (NGX_तन्तवः || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    शून्य                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#यदि_अन्तः

#यदि (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#यदि_अन्तः

    अचिह्नित                   valid_info:1;
    अचिह्नित                   directio:1;
};


#परिभाषय NGX_MAX_मार्गः_LEVEL  3


प्रकारनिर्देश ngx_msec_t (*ngx_मार्गः_manager_pt) (शून्य *दत्त);
प्रकारनिर्देश ngx_msec_t (*ngx_मार्गः_purger_pt) (शून्य *दत्त);
प्रकारनिर्देश शून्य (*ngx_मार्गः_loader_pt) (शून्य *दत्त);


प्रकारनिर्देश संरचना {
    ngx_str_t                  name;
    आकार_t                     len;
    आकार_t                     level[NGX_MAX_मार्गः_LEVEL];

    ngx_मार्गः_manager_pt        manager;
    ngx_मार्गः_purger_pt         purger;
    ngx_मार्गः_loader_pt         loader;
    शून्य                      *दत्त;

    u_char                    *conf_file;
    ngx_uint_t                 पङ्क्तिः;
} ngx_मार्गः_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                  name;
    आकार_t                     level[NGX_MAX_मार्गः_LEVEL];
} ngx_मार्गः_init_t;


प्रकारनिर्देश संरचना {
    ngx_file_t                 file;
    off_t                      विस्थापनम्;
    ngx_मार्गः_t                *मार्गः;
    ngx_pool_t                *pool;
    वर्ण                      *warn;

    ngx_uint_t                 access;

    अचिह्नित                   log_level:8;
    अचिह्नित                   persistent:1;
    अचिह्नित                   clean:1;
    अचिह्नित                   thread_write:1;
} ngx_अस्थायी_file_t;


प्रकारनिर्देश संरचना {
    ngx_uint_t                 access;
    ngx_uint_t                 मार्गः_access;
    time_t                     time;
    ngx_fd_t                   fd;

    अचिह्नित                   create_मार्गः:1;
    अचिह्नित                   delete_file:1;

    ngx_log_t                 *लघुगणकः;
} ngx_ext_rename_file_t;


प्रकारनिर्देश संरचना {
    off_t                      आकार;
    आकार_t                     buf_आकार;

    ngx_uint_t                 access;
    time_t                     time;

    ngx_log_t                 *लघुगणकः;
} ngx_copy_file_t;


प्रकारनिर्देश संरचना ngx_वृक्षः_ctx_s  ngx_वृक्षः_ctx_t;

प्रकारनिर्देश ngx_int_t (*ngx_वृक्षः_init_handler_pt) (शून्य *ctx, शून्य *पूर्व);
प्रकारनिर्देश ngx_int_t (*ngx_वृक्षः_handler_pt) (ngx_वृक्षः_ctx_t *ctx, ngx_str_t *name);

संरचना ngx_वृक्षः_ctx_s {
    off_t                      आकार;
    off_t                      fs_आकार;
    ngx_uint_t                 access;
    time_t                     mtime;

    ngx_वृक्षः_init_handler_pt   init_handler;
    ngx_वृक्षः_handler_pt        file_handler;
    ngx_वृक्षः_handler_pt        pre_वृक्षः_handler;
    ngx_वृक्षः_handler_pt        post_वृक्षः_handler;
    ngx_वृक्षः_handler_pt        spec_handler;

    शून्य                      *दत्त;
    आकार_t                     alloc;

    ngx_log_t                 *लघुगणकः;
};


ngx_int_t ngx_get_पूर्णम्_name(ngx_pool_t *pool, ngx_str_t *उपसर्गः,
    ngx_str_t *name);

ssize_t ngx_write_chain_गन्तव्यम्_अस्थायी_file(ngx_अस्थायी_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_अस्थायी_file(ngx_file_t *file, ngx_मार्गः_t *मार्गः,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
शून्य ngx_create_hashed_filename(ngx_मार्गः_t *मार्गः, u_char *file, आकार_t len);
ngx_int_t ngx_create_मार्गः(ngx_file_t *file, ngx_मार्गः_t *मार्गः);
ngx_err_t ngx_create_पूर्णम्_मार्गः(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_सङ्कलय_मार्गः(ngx_conf_t *cf, ngx_मार्गः_t **slot);
ngx_int_t ngx_create_paths(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *गन्तव्यम्,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *गन्तव्यम्, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_वृक्षः(ngx_वृक्षः_ctx_t *ctx, ngx_str_t *वृक्षः);

ngx_atomic_uint_t ngx_अग्र_अस्थायी_सङ्ख्या(ngx_uint_t collision);

वर्ण *ngx_conf_समुच्चयः_मार्गः_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_merge_मार्गः_मानम्(ngx_conf_t *cf, ngx_मार्गः_t **मार्गः,
    ngx_मार्गः_t *पूर्व, ngx_मार्गः_init_t *init);
वर्ण *ngx_conf_समुच्चयः_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


बाह्य ngx_atomic_t      *ngx_अस्थायी_सङ्ख्या;
बाह्य ngx_atomic_int_t   ngx_random_सङ्ख्या;


#यदि_अन्तः /* _NGX_FILE_H_INCLUDED_ */
