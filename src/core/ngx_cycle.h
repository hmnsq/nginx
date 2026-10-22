
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ಚಕ್ರ_H_INCLUDED
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ಚಕ್ರ_H_INCLUDED


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ಚಕ್ರ_POOL_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ಚಕ್ರ_POOL_ಗಾತ್ರ     NGX_DEFAULT_POOL_ಗಾತ್ರ
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_DEBUG_POINTS_STOP   1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_DEBUG_POINTS_ABORT  2


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_shm_zone_s  ngx_shm_zone_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, ಶೂನ್ಯ *ದತ್ತಾಂಶ);

ರಚನೆ ngx_shm_zone_s {
    ಶೂನ್ಯ                     *ದತ್ತಾಂಶ;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    ಶೂನ್ಯ                     *ಟ್ಯಾಗ್;
    ಶೂನ್ಯ                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


ರಚನೆ ngx_ಚಕ್ರ_s {
    ಶೂನ್ಯ                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *ಲಘುಗಣಕ;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_ಬಳಸಿದ;    /* unsigned  modules_used:1; */

    ngx_ಸರತಿ_t               reusable_connections_ಸರತಿ;
    ngx_uint_t                reusable_connections_n;
    ಸಮಯ_t                    connections_reuse_ಸಮಯ;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_ಗಂಟು_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_ಸ್ಮರಣೆ;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_ಚಕ್ರ_t              *old_ಚಕ್ರ;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_ಪೂರ್ವಪ್ರತ್ಯಯ;
    ngx_str_t                 ಪೂರ್ವಪ್ರತ್ಯಯ;
    ngx_str_t                 error_log;
    ngx_str_t                 ಬೀಗ_file;
    ngx_str_t                 hostname;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 ಕೆಲಸಗಾರ_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    ಪೂರ್ಣಾಂಕ                       ಆದ್ಯತೆ;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    ಅಕ್ಷರ                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 ಗುಂಪು;

    ngx_str_t                 working_directory;
    ngx_str_t                 ಬೀಗ_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    ಅಕ್ಷರ                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_is_init_ಚಕ್ರ(ಚಕ್ರ)  (ಚಕ್ರ->conf_ctx == ಖಾಲಿ)


ngx_ಚಕ್ರ_t *ngx_init_ಚಕ್ರ(ngx_ಚಕ್ರ_t *old_ಚಕ್ರ);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *ಲಘುಗಣಕ);
ಶೂನ್ಯ ngx_delete_pidfile(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_signal_process(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಅಕ್ಷರ *sig);
ಶೂನ್ಯ ngx_reopen_files(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uid_t user);
ಅಕ್ಷರ **ngx_ಗಣ_environment(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t *ಕೊನೆಯ);
ngx_pid_t ngx_exec_new_ದ್ವಿಮಾನ(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_ಸ್ಮರಣೆ_ಕೂಡಿಸು(ngx_conf_t *cf, ngx_str_t *name,
    ಗಾತ್ರ_t ಗಾತ್ರ_2, ಶೂನ್ಯ *ಟ್ಯಾಗ್);
ಶೂನ್ಯ ngx_ಗಣ_shutdown_timer(ngx_ಚಕ್ರ_t *ಚಕ್ರ);


ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_ಚಕ್ರ_t  *ngx_ಚಕ್ರ;
ಬಾಹ್ಯ ngx_array_t            ngx_old_ಚಕ್ರಗಳು;
ಬಾಹ್ಯ ngx_module_t           ngx_core_module;
ಬಾಹ್ಯ ngx_uint_t             ngx_test_config;
ಬಾಹ್ಯ ngx_uint_t             ngx_dump_config;
ಬಾಹ್ಯ ngx_uint_t             ngx_quiet_ಬಹುಲಕ;


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_CYCLE_H_INCLUDED_ */
