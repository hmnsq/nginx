
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ NGX_چکر_H_INCLUDED
#تعریف_کرو NGX_چکر_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر_غیر_تعریف_شدہ NGX_چکر_POOL_سائز
#تعریف_کرو NGX_چکر_POOL_سائز     NGX_DEFAULT_POOL_سائز
#اگر_ختم


#تعریف_کرو NGX_DEBUG_POINTS_STOP   1
#تعریف_کرو NGX_DEBUG_POINTS_ABORT  2


قسمتعریف ساخت ngx_shm_zone_s  ngx_shm_zone_t;

قسمتعریف ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, خالی *ڈیٹا);

ساخت ngx_shm_zone_s {
    خالی                     *ڈیٹا;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    خالی                     *لیبل;
    خالی                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


ساخت ngx_چکر_s {
    خالی                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *لوگارتھم;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_مستعمل;    /* unsigned  modules_used:1; */

    ngx_قطار_t               reusable_connections_قطار;
    ngx_uint_t                reusable_connections_n;
    وقت_t                    connections_reuse_وقت;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_گرہ_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_حافظہ;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_چکر_t              *old_چکر;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_سابقہ;
    ngx_str_t                 سابقہ;
    ngx_str_t                 error_log;
    ngx_str_t                 تالا_file;
    ngx_str_t                 hostname;
};


قسمتعریف ساخت {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 کارکن_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    صحیح                       ترجیح;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    حرف                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 گروہ;

    ngx_str_t                 working_directory;
    ngx_str_t                 تالا_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    حرف                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#تعریف_کرو ngx_is_init_چکر(چکر)  (چکر->conf_ctx == تہی)


ngx_چکر_t *ngx_init_چکر(ngx_چکر_t *old_چکر);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لوگارتھم);
خالی ngx_delete_pidfile(ngx_چکر_t *چکر);
ngx_int_t ngx_signal_process(ngx_چکر_t *چکر, حرف *sig);
خالی ngx_reopen_files(ngx_چکر_t *چکر, ngx_uid_t user);
حرف **ngx_سیٹ_environment(ngx_چکر_t *چکر, ngx_uint_t *آخری);
ngx_pid_t ngx_exec_new_ثنائی(ngx_چکر_t *چکر, حرف *ثابت *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_حافظہ_جوڑو(ngx_conf_t *cf, ngx_str_t *name,
    سائز_t سائز, خالی *لیبل);
خالی ngx_سیٹ_shutdown_timer(ngx_چکر_t *چکر);


خارجی متلون ngx_چکر_t  *ngx_چکر;
خارجی ngx_array_t            ngx_old_ادوار;
خارجی ngx_module_t           ngx_core_module;
خارجی ngx_uint_t             ngx_test_config;
خارجی ngx_uint_t             ngx_dump_config;
خارجی ngx_uint_t             ngx_quiet_منوال;


#اگر_ختم /* _NGX_CYCLE_H_INCLUDED_ */
