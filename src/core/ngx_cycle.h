
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_دوره_H_INCLUDED
#تعریف_کړه NGX_دوره_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#که_نه_تعریف_شوی NGX_دوره_POOL_اندازه
#تعریف_کړه NGX_دوره_POOL_اندازه     NGX_DEFAULT_POOL_اندازه
#د_که_پای


#تعریف_کړه NGX_DEBUG_POINTS_STOP   1
#تعریف_کړه NGX_DEBUG_POINTS_ABORT  2


تعريفنوع بنية ngx_shm_zone_s  ngx_shm_zone_t;

تعريفنوع ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, فراغ *معلومات);

بنية ngx_shm_zone_s {
    فراغ                     *معلومات;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    فراغ                     *نښان;
    فراغ                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


بنية ngx_دوره_s {
    فراغ                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *لوګاریتم;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_کارول_شوی;    /* unsigned  modules_used:1; */

    ngx_کتار_t               reusable_connections_کتار;
    ngx_uint_t                reusable_connections_n;
    وخت_t                    connections_reuse_وخت;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_غوټه_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_حافظه;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_دوره_t              *old_دوره;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_مختاړی;
    ngx_str_t                 مختاړی;
    ngx_str_t                 error_log;
    ngx_str_t                 قلف_file;
    ngx_str_t                 hostname;
};


تعريفنوع بنية {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 کارګر_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    صحيح                       لومړیتوب;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    حرف                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 ډله;

    ngx_str_t                 working_directory;
    ngx_str_t                 قلف_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    حرف                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#تعریف_کړه ngx_is_init_دوره(دوره)  (دوره->conf_ctx == عدم)


ngx_دوره_t *ngx_init_دوره(ngx_دوره_t *old_دوره);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لوګاریتم);
فراغ ngx_delete_pidfile(ngx_دوره_t *دوره);
ngx_int_t ngx_signal_process(ngx_دوره_t *دوره, حرف *sig);
فراغ ngx_reopen_files(ngx_دوره_t *دوره, ngx_uid_t user);
حرف **ngx_سېټ_environment(ngx_دوره_t *دوره, ngx_uint_t *وروستی);
ngx_pid_t ngx_exec_new_دوګونی(ngx_دوره_t *دوره, حرف *ثابت *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_حافظه_جمع_کړه(ngx_conf_t *cf, ngx_str_t *name,
    اندازه_t اندازه, فراغ *نښان);
فراغ ngx_سېټ_shutdown_timer(ngx_دوره_t *دوره);


خارجي متطاير ngx_دوره_t  *ngx_دوره;
خارجي ngx_array_t            ngx_old_دورې;
خارجي ngx_module_t           ngx_core_module;
خارجي ngx_uint_t             ngx_test_config;
خارجي ngx_uint_t             ngx_dump_config;
خارجي ngx_uint_t             ngx_quiet_ډېرپېښ;


#د_که_پای /* _NGX_CYCLE_H_INCLUDED_ */
