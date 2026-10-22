
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् NGX_आवृत्तिः_H_INCLUDED
#परिभाषय NGX_आवृत्तिः_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि_अपरिभाषितम् NGX_आवृत्तिः_POOL_आकार
#परिभाषय NGX_आवृत्तिः_POOL_आकार     NGX_DEFAULT_POOL_आकार
#यदि_अन्तः


#परिभाषय NGX_DEBUG_POINTS_STOP   1
#परिभाषय NGX_DEBUG_POINTS_ABORT  2


प्रकारनिर्देश संरचना ngx_shm_zone_s  ngx_shm_zone_t;

प्रकारनिर्देश ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, शून्य *दत्त);

संरचना ngx_shm_zone_s {
    शून्य                     *दत्त;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    शून्य                     *चिह्नकम्;
    शून्य                     *sync;
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


संरचना ngx_आवृत्तिः_s {
    शून्य                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *लघुगणकः;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_प्रयुक्तम्;    /* unsigned  modules_used:1; */

    ngx_queue_t               reusable_connections_queue;
    ngx_uint_t                reusable_connections_n;
    time_t                    connections_reuse_time;

    ngx_array_t               listening;
    ngx_array_t               paths;

    ngx_array_t               config_dump;
    ngx_rbtree_t              config_dump_rbtree;
    ngx_rbtree_ग्रन्थिः_t         config_dump_sentinel;

    ngx_list_t                open_files;
    ngx_list_t                shared_स्मृतिः;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_आवृत्तिः_t              *old_आवृत्तिः;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_उपसर्गः;
    ngx_str_t                 उपसर्गः;
    ngx_str_t                 error_log;
    ngx_str_t                 तालकम्_file;
    ngx_str_t                 hostname;
};


प्रकारनिर्देश संरचना {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;
    ngx_msec_t                shutdown_timeout;

    ngx_int_t                 कार्यकर्ता_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    पूर्णाङ्क                       प्राथमिकता;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    वर्ण                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 गणः;

    ngx_str_t                 working_directory;
    ngx_str_t                 तालकम्_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    वर्ण                    **environment;

    ngx_uint_t                transparent;  /* unsigned  transparent:1; */
} ngx_core_conf_t;


#परिभाषय ngx_is_init_आवृत्तिः(आवृत्तिः)  (आवृत्तिः->conf_ctx == रिक्त)


ngx_आवृत्तिः_t *ngx_init_आवृत्तिः(ngx_आवृत्तिः_t *old_आवृत्तिः);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *लघुगणकः);
शून्य ngx_delete_pidfile(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_signal_process(ngx_आवृत्तिः_t *आवृत्तिः, वर्ण *sig);
शून्य ngx_reopen_files(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uid_t user);
वर्ण **ngx_समुच्चयः_environment(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t *चरमः);
ngx_pid_t ngx_exec_new_द्विमान(ngx_आवृत्तिः_t *आवृत्तिः, वर्ण *ध्रुव *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_स्मृतिः_सङ्कलय(ngx_conf_t *cf, ngx_str_t *name,
    आकार_t आकार, शून्य *चिह्नकम्);
शून्य ngx_समुच्चयः_shutdown_timer(ngx_आवृत्तिः_t *आवृत्तिः);


बाह्य चञ्चल ngx_आवृत्तिः_t  *ngx_आवृत्तिः;
बाह्य ngx_array_t            ngx_old_आवृत्तयः;
बाह्य ngx_module_t           ngx_core_module;
बाह्य ngx_uint_t             ngx_test_config;
बाह्य ngx_uint_t             ngx_dump_config;
बाह्य ngx_uint_t             ngx_quiet_बहुलकम्;


#यदि_अन्तः /* _NGX_CYCLE_H_INCLUDED_ */
