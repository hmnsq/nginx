
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_చక్రం_H_INCLUDED
#నిర్వచించు NGX_చక్రం_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచితం_కాకపోతే NGX_చక్రం_POOL_కొలత
#నిర్వచించు NGX_చక్రం_POOL_కొలత     NGX_DEFAULT_POOL_కొలత
#అయితే_ముగింపు


#నిర్వచించు NGX_DEBUG_POINTS_STOP   1
#నిర్వచించు NGX_DEBUG_POINTS_ABORT  2


రకనిర్వచనం నిర్మాణం ngx_shm_zone_s  ngx_shm_zone_t;

రకనిర్వచనం ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, శూన్య *దత్తాంశం);

నిర్మాణం ngx_shm_zone_s {
    శూన్య                     *దత్తాంశం;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    శూన్య                     *ట్యాగ్;
    శూన్య                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


నిర్మాణం ngx_చక్రం_s {
    శూన్య                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *సంవర్గమానం;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_వాడిన;    /* unsigned  modules_used:1; */

    ngx_వరుస_t               reusable_connections_వరుస;
    ngx_uint_t                reusable_connections_n;
    సమయం_t                    connections_reuse_సమయం;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_కణుపు_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_జ్ఞాపకం;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_చక్రం_t              *old_చక్రం;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_ఉపసర్గ;
    ngx_str_t                 ఉపసర్గ;
    ngx_str_t                 error_log;
    ngx_str_t                 తాళం_file;
    ngx_str_t                 hostname;
};


రకనిర్వచనం నిర్మాణం {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 కార్మికుడు_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    పూర్ణాంకం                       ప్రాధాన్యత;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    అక్షరం                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 సమూహం;

    ngx_str_t                 working_directory;
    ngx_str_t                 తాళం_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    అక్షరం                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#నిర్వచించు ngx_is_init_చక్రం(చక్రం)  (చక్రం->conf_ctx == ఖాళీ)


ngx_చక్రం_t *ngx_init_చక్రం(ngx_చక్రం_t *old_చక్రం);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *సంవర్గమానం);
శూన్య ngx_delete_pidfile(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_signal_process(ngx_చక్రం_t *చక్రం, అక్షరం *sig);
శూన్య ngx_reopen_files(ngx_చక్రం_t *చక్రం, ngx_uid_t user);
అక్షరం **ngx_సమితి_environment(ngx_చక్రం_t *చక్రం, ngx_uint_t *చివరి);
ngx_pid_t ngx_exec_new_ద్విమాన(ngx_చక్రం_t *చక్రం, అక్షరం *స్థిరరాశి *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_జ్ఞాపకం_కూడు(ngx_conf_t *cf, ngx_str_t *name,
    కొలత_t కొలత, శూన్య *ట్యాగ్);
శూన్య ngx_సమితి_shutdown_timer(ngx_చక్రం_t *చక్రం);


బాహ్య అస్థిర ngx_చక్రం_t  *ngx_చక్రం;
బాహ్య ngx_array_t            ngx_old_చక్రాలు;
బాహ్య ngx_module_t           ngx_core_module;
బాహ్య ngx_uint_t             ngx_test_config;
బాహ్య ngx_uint_t             ngx_dump_config;
బాహ్య ngx_uint_t             ngx_quiet_బహుళకం;


#అయితే_ముగింపు /* _NGX_CYCLE_H_INCLUDED_ */
