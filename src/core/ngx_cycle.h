
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CYCLE_H_INCLUDED_
#घोषणा _NGX_CYCLE_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर_अघोषित NGX_CYCLE_POOL_आकार
#घोषणा NGX_CYCLE_POOL_आकार     NGX_DEFAULT_POOL_आकार
#पूर्ण_अगर


#घोषणा NGX_DEBUG_POINTS_STOP   1
#घोषणा NGX_DEBUG_POINTS_ABORT  2


प्रकार काष्ठा ngx_shm_zone_s  ngx_shm_zone_t;

प्रकार ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, व्योम *तथ्य);

काष्ठा ngx_shm_zone_s {
    व्योम                     *तथ्य;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    व्योम                     *tag;
    व्योम                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


काष्ठा ngx_cycle_s {
    व्योम                  ****conf_ctx;
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
    समय_प्रकार                    connections_reuse_time;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_गाँठ_t         config_dump_sentinel;

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


प्रकार काष्ठा {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 worker_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    पूर्णांक                       priority;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    अक्षर                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 group;

    ngx_str_t                 working_directory;
    ngx_str_t                 lock_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    अक्षर                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#घोषणा ngx_is_init_cycle(cycle)  (cycle->conf_ctx == शून्य)


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
व्योम ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, अक्षर *sig);
व्योम ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
अक्षर **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_द्विआधारी(ngx_cycle_t *cycle, अक्षर *स्थिर *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    माप_प्रकार आकार, व्योम *tag);
व्योम ngx_set_shutdown_timer(ngx_cycle_t *cycle);


बाह्य अस्थिर ngx_cycle_t  *ngx_cycle;
बाह्य ngx_array_t            ngx_old_cycles;
बाह्य ngx_module_t           ngx_core_module;
बाह्य ngx_uint_t             ngx_test_config;
बाह्य ngx_uint_t             ngx_dump_config;
बाह्य ngx_uint_t             ngx_quiet_mode;


#पूर्ण_अगर /* _NGX_CYCLE_H_INCLUDED_ */
