
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل NGX_چڪر_H_INCLUDED
#وصف_ڪريو NGX_چڪر_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جيڪڏهن_وصف_نه_ٿيل NGX_چڪر_POOL_سائيز
#وصف_ڪريو NGX_چڪر_POOL_سائيز     NGX_DEFAULT_POOL_سائيز
#جيڪڏهن_پورو


#وصف_ڪريو NGX_DEBUG_POINTS_STOP   1
#وصف_ڪريو NGX_DEBUG_POINTS_ABORT  2


تعريفنوع بنية ngx_shm_zone_s  ngx_shm_zone_t;

تعريفنوع ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, فراغ *معلومات);

بنية ngx_shm_zone_s {
    فراغ                     *معلومات;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    فراغ                     *ٽيگ;
    فراغ                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


بنية ngx_چڪر_s {
    فراغ                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *لاگرٿم;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_استعمال_ٿيل;    /* unsigned  modules_used:1; */

    ngx_قطار_t               reusable_connections_قطار;
    ngx_uint_t                reusable_connections_n;
    وقت_t                    connections_reuse_وقت;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_ڳنڍ_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_يادگيري;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_چڪر_t              *old_چڪر;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_اڳياڙي;
    ngx_str_t                 اڳياڙي;
    ngx_str_t                 error_log;
    ngx_str_t                 تالو_file;
    ngx_str_t                 hostname;
};


تعريفنوع بنية {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 ڪم_ڪندڙ_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    صحيح                       ترجيح;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    حرف                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 گروهه;

    ngx_str_t                 working_directory;
    ngx_str_t                 تالو_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    حرف                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#وصف_ڪريو ngx_is_init_چڪر(چڪر)  (چڪر->conf_ctx == عدم)


ngx_چڪر_t *ngx_init_چڪر(ngx_چڪر_t *old_چڪر);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لاگرٿم);
فراغ ngx_delete_pidfile(ngx_چڪر_t *چڪر);
ngx_int_t ngx_signal_process(ngx_چڪر_t *چڪر, حرف *sig);
فراغ ngx_reopen_files(ngx_چڪر_t *چڪر, ngx_uid_t user);
حرف **ngx_سيٽ_environment(ngx_چڪر_t *چڪر, ngx_uint_t *آخري);
ngx_pid_t ngx_exec_new_ٻٽو(ngx_چڪر_t *چڪر, حرف *ثابت *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_يادگيري_جوڙيو(ngx_conf_t *cf, ngx_str_t *name,
    سائيز_t سائيز, فراغ *ٽيگ);
فراغ ngx_سيٽ_shutdown_timer(ngx_چڪر_t *چڪر);


خارجي متطاير ngx_چڪر_t  *ngx_چڪر;
خارجي ngx_array_t            ngx_old_چڪرن;
خارجي ngx_module_t           ngx_core_module;
خارجي ngx_uint_t             ngx_test_config;
خارجي ngx_uint_t             ngx_dump_config;
خارجي ngx_uint_t             ngx_quiet_گهڻائي;


#جيڪڏهن_پورو /* _NGX_CYCLE_H_INCLUDED_ */
