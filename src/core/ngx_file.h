
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_FILE_H_INCLUDED_
#నిర్వచించు _NGX_FILE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


నిర్మాణం ngx_file_s {
    ngx_fd_t                   fd;
    ngx_str_t                  name;
    ngx_file_info_t            info;

    off_t                      స్థానభ్రంశం;
    off_t                      sys_స్థానభ్రంశం;

    ngx_log_t                 *సంవర్గమానం;

#ఒకవేళ (NGX_దారాలు || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    శూన్య                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#అయితే_ముగింపు

    గుర్తులేని                   valid_info:1;
    గుర్తులేని                   directio:1;
};


#నిర్వచించు NGX_MAX_మార్గం_LEVEL  3


రకనిర్వచనం ngx_msec_t (*ngx_మార్గం_manager_pt) (శూన్య *దత్తాంశం);
రకనిర్వచనం ngx_msec_t (*ngx_మార్గం_purger_pt) (శూన్య *దత్తాంశం);
రకనిర్వచనం శూన్య (*ngx_మార్గం_loader_pt) (శూన్య *దత్తాంశం);


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                  name;
    కొలత_t                     len;
    కొలత_t                     level[NGX_MAX_మార్గం_LEVEL];

    ngx_మార్గం_manager_pt        manager;
    ngx_మార్గం_purger_pt         purger;
    ngx_మార్గం_loader_pt         loader;
    శూన్య                      *దత్తాంశం;

    u_char                    *conf_file;
    ngx_uint_t                 పంక్తి;
} ngx_మార్గం_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                  name;
    కొలత_t                     level[NGX_MAX_మార్గం_LEVEL];
} ngx_మార్గం_init_t;


రకనిర్వచనం నిర్మాణం {
    ngx_file_t                 file;
    off_t                      స్థానభ్రంశం;
    ngx_మార్గం_t                *మార్గం;
    ngx_pool_t                *pool;
    అక్షరం                      *warn;

    ngx_uint_t                 access;

    గుర్తులేని                   log_level:8;
    గుర్తులేని                   persistent:1;
    గుర్తులేని                   clean:1;
    గుర్తులేని                   thread_write:1;
} ngx_తాత్కాలిక_file_t;


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t                 access;
    ngx_uint_t                 మార్గం_access;
    సమయం_t                     సమయం;
    ngx_fd_t                   fd;

    గుర్తులేని                   create_మార్గం:1;
    గుర్తులేని                   delete_file:1;

    ngx_log_t                 *సంవర్గమానం;
} ngx_ext_rename_file_t;


రకనిర్వచనం నిర్మాణం {
    off_t                      కొలత;
    కొలత_t                     buf_కొలత;

    ngx_uint_t                 access;
    సమయం_t                     సమయం;

    ngx_log_t                 *సంవర్గమానం;
} ngx_copy_file_t;


రకనిర్వచనం నిర్మాణం ngx_వృక్షం_ctx_s  ngx_వృక్షం_ctx_t;

రకనిర్వచనం ngx_int_t (*ngx_వృక్షం_init_handler_pt) (శూన్య *ctx, శూన్య *prev);
రకనిర్వచనం ngx_int_t (*ngx_వృక్షం_handler_pt) (ngx_వృక్షం_ctx_t *ctx, ngx_str_t *name);

నిర్మాణం ngx_వృక్షం_ctx_s {
    off_t                      కొలత;
    off_t                      fs_కొలత;
    ngx_uint_t                 access;
    సమయం_t                     mtime;

    ngx_వృక్షం_init_handler_pt   init_handler;
    ngx_వృక్షం_handler_pt        file_handler;
    ngx_వృక్షం_handler_pt        pre_వృక్షం_handler;
    ngx_వృక్షం_handler_pt        post_వృక్షం_handler;
    ngx_వృక్షం_handler_pt        spec_handler;

    శూన్య                      *దత్తాంశం;
    కొలత_t                     alloc;

    ngx_log_t                 *సంవర్గమానం;
};


ngx_int_t ngx_get_నిండిన_name(ngx_pool_t *pool, ngx_str_t *ఉపసర్గ,
    ngx_str_t *name);

ssize_t ngx_write_chain_గమ్యం_తాత్కాలిక_file(ngx_తాత్కాలిక_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_తాత్కాలిక_file(ngx_file_t *file, ngx_మార్గం_t *మార్గం,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
శూన్య ngx_create_hashed_filename(ngx_మార్గం_t *మార్గం, u_char *file, కొలత_t len);
ngx_int_t ngx_create_మార్గం(ngx_file_t *file, ngx_మార్గం_t *మార్గం);
ngx_err_t ngx_create_నిండిన_మార్గం(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_కూడు_మార్గం(ngx_conf_t *cf, ngx_మార్గం_t **స్థానం);
ngx_int_t ngx_create_paths(ngx_చక్రం_t *చక్రం, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *గమ్యం,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *గమ్యం, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_వృక్షం(ngx_వృక్షం_ctx_t *ctx, ngx_str_t *వృక్షం);

ngx_atomic_uint_t ngx_తదుపరి_తాత్కాలిక_సంఖ్య(ngx_uint_t collision);

అక్షరం *ngx_conf_సమితి_మార్గం_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_విలీనం_మార్గం_విలువ(ngx_conf_t *cf, ngx_మార్గం_t **మార్గం,
    ngx_మార్గం_t *prev, ngx_మార్గం_init_t *init);
అక్షరం *ngx_conf_సమితి_access_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);


బాహ్య ngx_atomic_t      *ngx_తాత్కాలిక_సంఖ్య;
బాహ్య ngx_atomic_int_t   ngx_random_సంఖ్య;


#అయితే_ముగింపు /* _NGX_FILE_H_INCLUDED_ */
