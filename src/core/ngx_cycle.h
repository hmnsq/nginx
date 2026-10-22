
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CYCLE_H_INCLUDED_
#define _NGX_CYCLE_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#ifndef NGX_CYCLE_POOL_আকার
#define NGX_CYCLE_POOL_আকার     NGX_DEFAULT_POOL_আকার
#endif


#define NGX_DEBUG_POINTS_STOP   1
#define NGX_DEBUG_POINTS_ABORT  2


প্রকারলেখ কাঠামো ngx_shm_zone_s  ngx_shm_zone_t;

প্রকারলেখ ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, শূন্য *তথ্য);

কাঠামো ngx_shm_zone_s {
    শূন্য                     *তথ্য;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    শূন্য                     *tag;
    শূন্য                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


কাঠামো ngx_cycle_s {
    শূন্য                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *log;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_used;    /* unsigned  modules_used:1; */

    ngx_queue_t               reusable_connections_queue;
    ngx_uint_t                reusable_connections_n;
    time_t                    connections_reuse_time;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_গ্রন্থি_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_memory;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_cycle_t              *old_cycle;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_prefix;
    ngx_str_t                 prefix;
    ngx_str_t                 error_log;
    ngx_str_t                 lock_file;
    ngx_str_t                 hostname;
};


প্রকারলেখ কাঠামো {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 worker_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    পূর্ণসংখ্যা                       priority;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    অক্ষর                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 group;

    ngx_str_t                 working_directory;
    ngx_str_t                 lock_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    অক্ষর                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#define ngx_is_init_cycle(cycle)  (cycle->conf_ctx == ফাঁকা)


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
শূন্য ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, অক্ষর *sig);
শূন্য ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
অক্ষর **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_দ্বিমিক(ngx_cycle_t *cycle, অক্ষর *ধ্রুবক *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    আকার_t আকার_2, শূন্য *tag);
শূন্য ngx_set_shutdown_timer(ngx_cycle_t *cycle);


বাহ্যিক অস্থির ngx_cycle_t  *ngx_cycle;
বাহ্যিক ngx_array_t            ngx_old_cycles;
বাহ্যিক ngx_module_t           ngx_core_module;
বাহ্যিক ngx_uint_t             ngx_test_config;
বাহ্যিক ngx_uint_t             ngx_dump_config;
বাহ্যিক ngx_uint_t             ngx_quiet_mode;


#endif /* _NGX_CYCLE_H_INCLUDED_ */
