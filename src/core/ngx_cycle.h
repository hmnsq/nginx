
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_چرخه_H_INCLUDED
#تعریف_کن NGX_چرخه_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#اگر_تعریف_نشده NGX_چرخه_POOL_اندازه
#تعریف_کن NGX_چرخه_POOL_اندازه     NGX_DEFAULT_POOL_اندازه
#پایان_اگر


#تعریف_کن NGX_DEBUG_POINTS_STOP   1
#تعریف_کن NGX_DEBUG_POINTS_ABORT  2


تعريفنوع بنية ngx_shm_zone_s  ngx_shm_zone_t;

تعريفنوع ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, فراغ *داده);

بنية ngx_shm_zone_s {
    فراغ                     *داده;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    فراغ                     *برچسب;
    فراغ                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


بنية ngx_چرخه_s {
    فراغ                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *لگاریتم;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_استفاده_شده;    /* unsigned  modules_used:1; */

    ngx_صف_t               reusable_connections_صف;
    ngx_uint_t                reusable_connections_n;
    زمان_t                    connections_reuse_زمان;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_گره_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_حافظه;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_چرخه_t              *old_چرخه;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_پیشوند;
    ngx_str_t                 پیشوند;
    ngx_str_t                 error_log;
    ngx_str_t                 قفل_file;
    ngx_str_t                 hostname;
};


تعريفنوع بنية {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 کارگر_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    صحيح                       اولویت;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    حرف                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 گروه;

    ngx_str_t                 working_directory;
    ngx_str_t                 قفل_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    حرف                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#تعریف_کن ngx_is_init_چرخه(چرخه)  (چرخه->conf_ctx == عدم)


ngx_چرخه_t *ngx_init_چرخه(ngx_چرخه_t *old_چرخه);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لگاریتم);
فراغ ngx_delete_pidfile(ngx_چرخه_t *چرخه);
ngx_int_t ngx_signal_process(ngx_چرخه_t *چرخه, حرف *sig);
فراغ ngx_reopen_files(ngx_چرخه_t *چرخه, ngx_uid_t user);
حرف **ngx_مجموعه_environment(ngx_چرخه_t *چرخه, ngx_uint_t *آخرین);
ngx_pid_t ngx_exec_new_دودویی(ngx_چرخه_t *چرخه, حرف *ثابت *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_حافظه_جمع_کن(ngx_conf_t *cf, ngx_str_t *name,
    اندازه_t اندازه, فراغ *برچسب);
فراغ ngx_مجموعه_shutdown_timer(ngx_چرخه_t *چرخه);


خارجي متطاير ngx_چرخه_t  *ngx_چرخه;
خارجي ngx_array_t            ngx_old_چرخهها;
خارجي ngx_module_t           ngx_core_module;
خارجي ngx_uint_t             ngx_test_config;
خارجي ngx_uint_t             ngx_dump_config;
خارجي ngx_uint_t             ngx_quiet_نما;


#پایان_اگر /* _NGX_CYCLE_H_INCLUDED_ */
