
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਚੱਕਰ_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਚੱਕਰ_H_INCLUDED


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਚੱਕਰ_POOL_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਚੱਕਰ_POOL_ਮਾਪ     NGX_DEFAULT_POOL_ਮਾਪ
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DEBUG_POINTS_STOP   1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DEBUG_POINTS_ABORT  2


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_shm_zone_s  ngx_shm_zone_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, ਖਾਲੀ *ਅੰਕੜੇ);

ਬਣਤਰ ngx_shm_zone_s {
    ਖਾਲੀ                     *ਅੰਕੜੇ;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    ਖਾਲੀ                     *ਟੈਗ;
    ਖਾਲੀ                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


ਬਣਤਰ ngx_ਚੱਕਰ_s {
    ਖਾਲੀ                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *ਲਘੂਗਣਕ;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_ਵਰਤਿਆ;    /* unsigned  modules_used:1; */

    ngx_ਕਤਾਰ_t               reusable_connections_ਕਤਾਰ;
    ngx_uint_t                reusable_connections_n;
    ਸਮਾਂ_t                    connections_reuse_ਸਮਾਂ;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_ਗੰਢ_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_ਸਮ੍ਰਿਤੀ;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_ਚੱਕਰ_t              *old_ਚੱਕਰ;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_ਅਗੇਤਰ;
    ngx_str_t                 ਅਗੇਤਰ;
    ngx_str_t                 error_log;
    ngx_str_t                 ਤਾਲਾ_file;
    ngx_str_t                 hostname;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 ਕਾਮਾ_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    ਪੂਰਨ                       ਤਰਜੀਹ;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    ਅੱਖਰ                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 ਸਮੂਹ;

    ngx_str_t                 working_directory;
    ngx_str_t                 ਤਾਲਾ_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    ਅੱਖਰ                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_is_init_ਚੱਕਰ(ਚੱਕਰ)  (ਚੱਕਰ->conf_ctx == ਨਿਰਾ)


ngx_ਚੱਕਰ_t *ngx_init_ਚੱਕਰ(ngx_ਚੱਕਰ_t *old_ਚੱਕਰ);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *ਲਘੂਗਣਕ);
ਖਾਲੀ ngx_delete_pidfile(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_signal_process(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਅੱਖਰ *sig);
ਖਾਲੀ ngx_reopen_files(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uid_t user);
ਅੱਖਰ **ngx_ਸੈੱਟ_environment(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t *ਆਖਰੀ);
ngx_pid_t ngx_exec_new_ਦੁਆਧਾਰੀ(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਅੱਖਰ *ਅਚਲ *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_ਸਮ੍ਰਿਤੀ_ਜੋੜੋ(ngx_conf_t *cf, ngx_str_t *name,
    ਮਾਪ_t ਮਾਪ, ਖਾਲੀ *ਟੈਗ);
ਖਾਲੀ ngx_ਸੈੱਟ_shutdown_timer(ngx_ਚੱਕਰ_t *ਚੱਕਰ);


ਬਾਹਰੀ ਅਸਥਿਰ ngx_ਚੱਕਰ_t  *ngx_ਚੱਕਰ;
ਬਾਹਰੀ ngx_array_t            ngx_old_ਚੱਕਰਾਂ;
ਬਾਹਰੀ ngx_module_t           ngx_core_module;
ਬਾਹਰੀ ngx_uint_t             ngx_test_config;
ਬਾਹਰੀ ngx_uint_t             ngx_dump_config;
ਬਾਹਰੀ ngx_uint_t             ngx_quiet_ਬਹੁਲਕ;


#ਜੇ_ਅੰਤ /* _NGX_CYCLE_H_INCLUDED_ */
