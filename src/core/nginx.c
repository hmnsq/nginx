
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <nginx.h>


ਸਥਿਰ ਖਾਲੀ ngx_show_ਸੰਸਕਰਣ_info(ਖਾਲੀ);
ਸਥਿਰ ngx_int_t ngx_ਜੋੜੋ_inherited_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ਖਾਲੀ ngx_cleanup_environment(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ ngx_cleanup_environment_variable(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ngx_int_t ngx_get_options(ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv);
ਸਥਿਰ ngx_int_t ngx_process_options(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ngx_int_t ngx_save_argv(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv);
ਸਥਿਰ ਖਾਲੀ *ngx_core_module_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ਅੱਖਰ *ngx_core_module_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਸੈੱਟ_user(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਸੈੱਟ_env(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਸੈੱਟ_ਤਰਜੀਹ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਸੈੱਟ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਸੈੱਟ_ਕਾਮਾ_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_ਲੱਦੋ_module(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
#ਜੇ (NGX_HAVE_DLOPEN)
ਸਥਿਰ ਖਾਲੀ ngx_unload_module(ਖਾਲੀ *ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ


ਸਥਿਰ ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ਸਥਿਰ ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_FLAG,
      ngx_conf_ਸੈੱਟ_flag_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ਨਿਰਾ },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_FLAG,
      ngx_conf_ਸੈੱਟ_flag_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, master),
      ਨਿਰਾ },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_msec_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ਨਿਰਾ },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_str_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, pid),
      ਨਿਰਾ },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_str_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, ਤਾਲਾ_file),
      ਨਿਰਾ },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_ਸੈੱਟ_ਕਾਮਾ_processes,
      0,
      0,
      ਨਿਰਾ },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_enum_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE12,
      ngx_ਸੈੱਟ_user,
      0,
      0,
      ਨਿਰਾ },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_ਸੈੱਟ_ਤਰਜੀਹ,
      0,
      0,
      ਨਿਰਾ },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_1_ਹੋਰ,
      ngx_ਸੈੱਟ_cpu_affinity,
      0,
      0,
      ਨਿਰਾ },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_ਅੰਕ_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ਨਿਰਾ },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_off_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ਨਿਰਾ },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_msec_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ਨਿਰਾ },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ਸੈੱਟ_str_ਥਾਂ,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ਨਿਰਾ },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_ਸੈੱਟ_env,
      0,
      0,
      ਨਿਰਾ },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_TAKE1,
      ngx_ਲੱਦੋ_module,
      0,
      0,
      ਨਿਰਾ },

      ngx_null_command
};


ਸਥਿਰ ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ਨਿਰਾ,                                  /* init master */
    ਨਿਰਾ,                                  /* init module */
    ਨਿਰਾ,                                  /* init process */
    ਨਿਰਾ,                                  /* init thread */
    ਨਿਰਾ,                                  /* exit thread */
    ਨਿਰਾ,                                  /* exit process */
    ਨਿਰਾ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ਸਥਿਰ ngx_uint_t   ngx_show_help;
ਸਥਿਰ ngx_uint_t   ngx_show_ਸੰਸਕਰਣ;
ਸਥਿਰ ngx_uint_t   ngx_show_configure;
ਸਥਿਰ u_char      *ngx_ਅਗੇਤਰ;
ਸਥਿਰ u_char      *ngx_error_log;
ਸਥਿਰ u_char      *ngx_conf_file;
ਸਥਿਰ u_char      *ngx_conf_params;
#ਜੇ (NGX_CONTROL_API)
ਸਥਿਰ u_char      *ngx_control_addr;
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਅੱਖਰ        *ngx_signal;


ਸਥਿਰ ਅੱਖਰ **ngx_os_environ;


ਪੂਰਨ ngx_cdecl
ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *ਲਘੂਗਣਕ;
    ngx_uint_t        i;
    ngx_ਚੱਕਰ_t      *ਚੱਕਰ, init_ਚੱਕਰ;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    ਜੇ (ngx_strerror_init() != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_get_options(argc, argv) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_show_ਸੰਸਕਰਣ) {
        ngx_show_ਸੰਸਕਰਣ_info();

        ਜੇ (!ngx_test_config) {
            ਵਾਪਸ 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_ਸਮਾਂ_init();

#ਜੇ (NGX_PCRE)
    ngx_regex_init();
#ਜੇ_ਅੰਤ

    ngx_pid = ngx_getpid();
    ngx_ਮਾਪਾ = ngx_getppid();

    ਲਘੂਗਣਕ = ngx_log_init(ngx_ਅਗੇਤਰ, ngx_error_log);
    ਜੇ (ਲਘੂਗਣਕ == ਨਿਰਾ) {
        ਵਾਪਸ 1;
    }

    /* STUB */
#ਜੇ (NGX_OPENSSL)
    ngx_ssl_init(ਲਘੂਗਣਕ);
#ਜੇ_ਅੰਤ

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_ਚੱਕਰ, ਆਕਾਰ(ngx_ਚੱਕਰ_t));
    init_ਚੱਕਰ.ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;
    ngx_ਚੱਕਰ = &init_ਚੱਕਰ;

    init_ਚੱਕਰ.pool = ngx_create_pool(1024, ਲਘੂਗਣਕ);
    ਜੇ (init_ਚੱਕਰ.pool == ਨਿਰਾ) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_save_argv(&init_ਚੱਕਰ, argc, argv) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_process_options(&init_ਚੱਕਰ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_os_init(ਲਘੂਗਣਕ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    ਜੇ (ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਾਰਣੀ_init() != NGX_OK) {
        ਵਾਪਸ 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    ਜੇ (ngx_ਜੋੜੋ_inherited_sockets(&init_ਚੱਕਰ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

#ਜੇ (NGX_CONTROL_API)
    ngx_control_preinit();
#ਜੇ_ਅੰਤ

    ਜੇ (ngx_preinit_modules() != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਚੱਕਰ = ngx_init_ਚੱਕਰ(&init_ਚੱਕਰ);
    ਜੇ (ਚੱਕਰ == ਨਿਰਾ) {
        ਜੇ (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_ਚੱਕਰ.conf_file.ਅੰਕੜੇ);
        }

        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_test_config) {
        ਜੇ (!ngx_quiet_ਬਹੁਲਕ) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           ਚੱਕਰ->conf_file.ਅੰਕੜੇ);
        }

        ਜੇ (ngx_dump_config) {
            cd = ਚੱਕਰ->config_dump.elts;

            ਲਈ (i = 0; i < ਚੱਕਰ->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (ਖਾਲੀ) ngx_write_fd(ngx_stdout, cd[i].name.ਅੰਕੜੇ,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].ਬਫਰ;

                (ਖਾਲੀ) ngx_write_fd(ngx_stdout, b->pos, b->ਆਖਰੀ - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        ਵਾਪਸ 0;
    }

    ਜੇ (ngx_signal) {
        ਵਾਪਸ ngx_signal_process(ਚੱਕਰ, ngx_signal);
    }

    ngx_os_ਸਥਿਤੀ(ਚੱਕਰ->ਲਘੂਗਣਕ);

    ngx_ਚੱਕਰ = ਚੱਕਰ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਜੇ (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#ਜੇ !(NGX_WIN32)

    ਜੇ (ngx_init_ਸੰਕੇਤ(ਚੱਕਰ->ਲਘੂਗਣਕ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (!ngx_inherited && ccf->daemon) {
        ਜੇ (ngx_daemon(ਚੱਕਰ->ਲਘੂਗਣਕ) != NGX_OK) {
            ਵਾਪਸ 1;
        }

        ngx_daemonized = 1;
    }

    ਜੇ (ngx_inherited) {
        ngx_daemonized = 1;
    }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_CONTROL_API)
    ਜੇ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "control API unavailable in single process mode");

    } ਨਹੀਂਤਾਂ {
        ਜੇ (ngx_control_init(ngx_control_addr) != NGX_OK) {
            ਵਾਪਸ 1;
        }
    }
#ਜੇ_ਅੰਤ

    ਜੇ (ngx_create_pidfile(&ccf->pid, ਚੱਕਰ->ਲਘੂਗਣਕ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ngx_log_redirect_stderr(ਚੱਕਰ) != NGX_OK) {
        ਵਾਪਸ 1;
    }

    ਜੇ (ਲਘੂਗਣਕ->file->fd != ngx_stderr) {
        ਜੇ (ngx_close_file(ਲਘੂਗਣਕ->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    ਜੇ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_ਚੱਕਰ(ਚੱਕਰ);

    } ਨਹੀਂਤਾਂ {
        ngx_master_process_ਚੱਕਰ(ਚੱਕਰ);
    }

    ਵਾਪਸ 0;
}


ਸਥਿਰ ਖਾਲੀ
ngx_show_ਸੰਸਕਰਣ_info(ਖਾਲੀ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    ਜੇ (ngx_show_help) {
        ngx_write_stderr(
            "Usage: nginx [-?hvVtTq] [-s signal] [-p prefix]" NGX_LINEFEED
            "             [-e filename] [-c filename] [-g directives]"
                          NGX_LINEFEED NGX_LINEFEED
            "Options:" NGX_LINEFEED
            "  -?,-h         : this help" NGX_LINEFEED
            "  -v            : show version and exit" NGX_LINEFEED
            "  -V            : show version and configure options then exit"
                               NGX_LINEFEED
            "  -t            : test configuration and exit" NGX_LINEFEED
            "  -T            : test configuration, dump it and exit"
                               NGX_LINEFEED
            "  -q            : suppress non-error messages "
                               "during configuration testing" NGX_LINEFEED
            "  -s signal     : send signal to a master process: "
                               "stop, quit, reopen, reload" NGX_LINEFEED
#ਜੇ (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NGX_ਅਗੇਤਰ
            "  -p prefix     : set prefix path (default: " NGX_ਅਗੇਤਰ ")"
                               NGX_LINEFEED
#ਨਹੀਂਤਾਂ
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#ਜੇ_ਅੰਤ
            "  -e filename   : set error log file (default: "
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#ਨਹੀਂਤਾਂ
                               NGX_ERROR_LOG_ਰਸਤਾ ")" NGX_LINEFEED
#ਜੇ_ਅੰਤ
            "  -c filename   : set configuration file (default: " NGX_CONF_ਰਸਤਾ
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    ਜੇ (ngx_show_configure) {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_SSL)
        ਜੇ (ngx_strcmp(ngx_ssl_ਸੰਸਕਰਣ(), OPENSSL_ਸੰਸਕਰਣ_ਪਾਠ) == 0) {
            ngx_write_stderr("built with " OPENSSL_ਸੰਸਕਰਣ_ਪਾਠ NGX_LINEFEED);
        } ਨਹੀਂਤਾਂ {
            ngx_write_stderr("built with " OPENSSL_ਸੰਸਕਰਣ_ਪਾਠ
                             " (running with ");
            ngx_write_stderr((ਅੱਖਰ *) (uintptr_t) ngx_ssl_ਸੰਸਕਰਣ());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SSL_CTRL_ਸੈੱਟ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#ਨਹੀਂਤਾਂ
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ਸਥਿਰ ngx_int_t
ngx_ਜੋੜੋ_inherited_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    ਜੇ (inherited == ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                  "using inherited sockets from \"%s\"", inherited);

    ਜੇ (ngx_array_init(&ਚੱਕਰ->listening, ਚੱਕਰ->pool, 10,
                       ਆਕਾਰ(ngx_listening_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (p = inherited, v = p; *p; p++) {
        ਜੇ (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            ਜੇ (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                ਰੋਕੋ;
            }

            v = p + 1;

            ls = ngx_array_ਧੱਕੋ(&ਚੱਕਰ->listening);
            ਜੇ (ls == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ngx_memzero(ls, ਆਕਾਰ(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    ਜੇ (v != p) {
        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    ਵਾਪਸ ngx_ਸੈੱਟ_inherited_sockets(ਚੱਕਰ);
}


ਅੱਖਰ **
ngx_ਸੈੱਟ_environment(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t *ਆਖਰੀ)
{
    ਅੱਖਰ                **p, **env, *str;
    ਮਾਪ_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਜੇ (ਆਖਰੀ == ਨਿਰਾ && ccf->environment) {
        ਵਾਪਸ ccf->environment;
    }

    var = ccf->env.elts;

    ਲਈ (i = 0; i < ccf->env.nelts; i++) {
        ਜੇ (ngx_strcmp(var[i].ਅੰਕੜੇ, "TZ") == 0
            || ngx_strncmp(var[i].ਅੰਕੜੇ, "TZ=", 3) == 0)
        {
            ਜਾਓ tz_ਮਿਲਿਆ;
        }
    }

    var = ngx_array_ਧੱਕੋ(&ccf->env);
    ਜੇ (var == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    var->len = 2;
    var->ਅੰਕੜੇ = (u_char *) "TZ";

    var = ccf->env.elts;

tz_ਮਿਲਿਆ:

    n = 0;

    ਲਈ (i = 0; i < ccf->env.nelts; i++) {

        ਜੇ (var[i].ਅੰਕੜੇ[var[i].len] == '=') {
            n++;
            ਜਾਰੀ;
        }

        ਲਈ (p = ngx_os_environ; *p; p++) {

            ਜੇ (ngx_strncmp(*p, var[i].ਅੰਕੜੇ, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                ਰੋਕੋ;
            }
        }
    }

    ਜੇ (ਆਖਰੀ) {
        env = ngx_alloc((*ਆਖਰੀ + n + 1) * ਆਕਾਰ(ਅੱਖਰ *), ਚੱਕਰ->ਲਘੂਗਣਕ);
        ਜੇ (env == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        *ਆਖਰੀ = n;

    } ਨਹੀਂਤਾਂ {
        cln = ngx_pool_cleanup_ਜੋੜੋ(ਚੱਕਰ->pool, 0);
        ਜੇ (cln == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        env = ngx_alloc((n + 1) * ਆਕਾਰ(ਅੱਖਰ *), ਚੱਕਰ->ਲਘੂਗਣਕ);
        ਜੇ (env == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        cln->handler = ngx_cleanup_environment;
        cln->ਅੰਕੜੇ = env;
    }

    n = 0;

    ਲਈ (i = 0; i < ccf->env.nelts; i++) {

        ਜੇ (var[i].ਅੰਕੜੇ[var[i].len] == '=') {

            ਜੇ (ਆਖਰੀ) {
                env[n++] = (ਅੱਖਰ *) var[i].ਅੰਕੜੇ;
                ਜਾਰੀ;
            }

            cln = ngx_pool_cleanup_ਜੋੜੋ(ਚੱਕਰ->pool, 0);
            ਜੇ (cln == ਨਿਰਾ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            len = ngx_strlen(var[i].ਅੰਕੜੇ) + 1;

            str = ngx_alloc(len, ਚੱਕਰ->ਲਘੂਗਣਕ);
            ਜੇ (str == ਨਿਰਾ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            ngx_memcpy(str, var[i].ਅੰਕੜੇ, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->ਅੰਕੜੇ = str;

            env[n++] = str;

            ਜਾਰੀ;
        }

        ਲਈ (p = ngx_os_environ; *p; p++) {

            ਜੇ (ngx_strncmp(*p, var[i].ਅੰਕੜੇ, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                ਰੋਕੋ;
            }
        }
    }

    env[n] = ਨਿਰਾ;

    ਜੇ (ਆਖਰੀ == ਨਿਰਾ) {
        ccf->environment = env;
        environ = env;
    }

    ਵਾਪਸ env;
}


ਸਥਿਰ ਖਾਲੀ
ngx_cleanup_environment(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਅੱਖਰ  **env = ਅੰਕੜੇ;

    ਜੇ (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        ਵਾਪਸ;
    }

    ngx_free(env);
}


ਸਥਿਰ ਖਾਲੀ
ngx_cleanup_environment_variable(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਅੱਖਰ  *var = ਅੰਕੜੇ;

    ਅੱਖਰ  **p;

    ਲਈ (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        ਜੇ (*p == var) {
            ਵਾਪਸ;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ਦੁਆਧਾਰੀ(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਅੱਖਰ *ਅਚਲ *argv)
{
    ਅੱਖਰ             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, ਆਕਾਰ(ngx_exec_ctx_t));

    ctx.ਰਸਤਾ = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_ਸੈੱਟ_environment(ਚੱਕਰ, &n);
    ਜੇ (env == ਨਿਰਾ) {
        ਵਾਪਸ NGX_INVALID_PID;
    }

    var = ngx_alloc(ਆਕਾਰ(NGINX_VAR)
                    + ਚੱਕਰ->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    ਚੱਕਰ->ਲਘੂਗਣਕ);
    ਜੇ (var == ਨਿਰਾ) {
        ngx_free(env);
        ਵਾਪਸ NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", ਆਕਾਰ(NGINX_VAR));

    ls = ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {
        ਜੇ (ls[i].ignore) {
            ਜਾਰੀ;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#ਜੇ (NGX_SETPROCTITLE_ਵਰਤਦਾ_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    ਜੇ (env[n] != ਨਿਰਾ) {
        n++;
    }
#ਜੇ_ਅੰਤ

    env[n] = ਨਿਰਾ;

#ਜੇ (NGX_DEBUG)
    {
    ਅੱਖਰ  **e;
    ਲਈ (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "env: %s", *e);
    }
    }
#ਜੇ_ਅੰਤ

    ctx.envp = (ਅੱਖਰ *ਅਚਲ *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਜੇ (ngx_rename_file(ccf->pid.ਅੰਕੜੇ, ccf->oldpid.ਅੰਕੜੇ) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.ਅੰਕੜੇ, ccf->oldpid.ਅੰਕੜੇ, argv[0]);

        ngx_free(env);
        ngx_free(var);

        ਵਾਪਸ NGX_INVALID_PID;
    }

    pid = ngx_execute(ਚੱਕਰ, &ctx);

    ਜੇ (pid == NGX_INVALID_PID) {
        ਜੇ (ngx_rename_file(ccf->oldpid.ਅੰਕੜੇ, ccf->pid.ਅੰਕੜੇ)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.ਅੰਕੜੇ, ccf->pid.ਅੰਕੜੇ, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    ਵਾਪਸ pid;
}


ਸਥਿਰ ngx_int_t
ngx_get_options(ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv)
{
    u_char     *p;
    ngx_int_t   i;

    ਲਈ (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        ਜੇ (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            ਵਾਪਸ NGX_ERROR;
        }

        ਜਦੋਂ (*p) {

            ਚੋਣ (*p++) {

            ਹਾਲਤ '?':
            ਹਾਲਤ 'h':
                ngx_show_ਸੰਸਕਰਣ = 1;
                ngx_show_help = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'v':
                ngx_show_ਸੰਸਕਰਣ = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'V':
                ngx_show_ਸੰਸਕਰਣ = 1;
                ngx_show_configure = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 't':
                ngx_test_config = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'q':
                ngx_quiet_ਬਹੁਲਕ = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'p':
                ਜੇ (*p) {
                    ngx_ਅਗੇਤਰ = p;
                    ਜਾਓ ਅਗਲਾ;
                }

                ਜੇ (argv[++i]) {
                    ngx_ਅਗੇਤਰ = (u_char *) argv[i];
                    ਜਾਓ ਅਗਲਾ;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                ਵਾਪਸ NGX_ERROR;

            ਹਾਲਤ 'e':
                ਜੇ (*p) {
                    ngx_error_log = p;

                } ਨਹੀਂਤਾਂ ਜੇ (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } ਨਹੀਂਤਾਂ {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                ਜਾਓ ਅਗਲਾ;

            ਹਾਲਤ 'c':
                ਜੇ (*p) {
                    ngx_conf_file = p;
                    ਜਾਓ ਅਗਲਾ;
                }

                ਜੇ (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    ਜਾਓ ਅਗਲਾ;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                ਵਾਪਸ NGX_ERROR;

            ਹਾਲਤ 'g':
                ਜੇ (*p) {
                    ngx_conf_params = p;
                    ਜਾਓ ਅਗਲਾ;
                }

                ਜੇ (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    ਜਾਓ ਅਗਲਾ;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                ਵਾਪਸ NGX_ERROR;

            ਹਾਲਤ 's':
                ਜੇ (*p) {
                    ngx_signal = (ਅੱਖਰ *) p;

                } ਨਹੀਂਤਾਂ ਜੇ (argv[++i]) {
                    ngx_signal = argv[i];

                } ਨਹੀਂਤਾਂ {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    ਜਾਓ ਅਗਲਾ;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                ਵਾਪਸ NGX_ERROR;

#ਜੇ (NGX_CONTROL_API)
            ਹਾਲਤ 'l':
                ngx_control_api_enabled = 1;

                ਜੇ (*p) {
                    ngx_control_addr = p;
                    ਜਾਓ ਅਗਲਾ;
                }

                ਜੇ (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    ਜਾਓ ਅਗਲਾ;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                ਵਾਪਸ NGX_ERROR;
#ਜੇ_ਅੰਤ

            ਮੂਲ:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                ਵਾਪਸ NGX_ERROR;
            }
        }

    ਅਗਲਾ:

        ਜਾਰੀ;
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_save_argv(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv)
{
#ਜੇ (NGX_FREEBSD)

    ngx_os_argv = (ਅੱਖਰ **) argv;
    ngx_argc = argc;
    ngx_argv = (ਅੱਖਰ **) argv;

#ਨਹੀਂਤਾਂ
    ਮਾਪ_t     len;
    ngx_int_t  i;

    ngx_os_argv = (ਅੱਖਰ **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * ਆਕਾਰ(ਅੱਖਰ *), ਚੱਕਰ->ਲਘੂਗਣਕ);
    ਜੇ (ngx_argv == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, ਚੱਕਰ->ਲਘੂਗਣਕ);
        ਜੇ (ngx_argv[i] == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        (ਖਾਲੀ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ਨਿਰਾ;

#ਜੇ_ਅੰਤ

    ngx_os_environ = environ;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_process_options(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    u_char  *p;
    ਮਾਪ_t   len;

    ਜੇ (ngx_ਅਗੇਤਰ) {
        len = ngx_strlen(ngx_ਅਗੇਤਰ);
        p = ngx_ਅਗੇਤਰ;

        ਜੇ (len && !ngx_ਰਸਤਾ_separator(p[len - 1])) {
            p = ngx_pnalloc(ਚੱਕਰ->pool, len + 1);
            ਜੇ (p == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ngx_memcpy(p, ngx_ਅਗੇਤਰ, len);
            p[len++] = '/';
        }

        ਚੱਕਰ->conf_ਅਗੇਤਰ.len = len;
        ਚੱਕਰ->conf_ਅਗੇਤਰ.ਅੰਕੜੇ = p;
        ਚੱਕਰ->ਅਗੇਤਰ.len = len;
        ਚੱਕਰ->ਅਗੇਤਰ.ਅੰਕੜੇ = p;

    } ਨਹੀਂਤਾਂ {

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਅਗੇਤਰ

        p = ngx_pnalloc(ਚੱਕਰ->pool, NGX_MAX_ਰਸਤਾ);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ngx_getcwd(p, NGX_MAX_ਰਸਤਾ) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            ਵਾਪਸ NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        ਚੱਕਰ->conf_ਅਗੇਤਰ.len = len;
        ਚੱਕਰ->conf_ਅਗੇਤਰ.ਅੰਕੜੇ = p;
        ਚੱਕਰ->ਅਗੇਤਰ.len = len;
        ਚੱਕਰ->ਅਗੇਤਰ.ਅੰਕੜੇ = p;

#ਨਹੀਂਤਾਂ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NGX_CONF_ਅਗੇਤਰ
        ngx_str_ਸੈੱਟ(&ਚੱਕਰ->conf_ਅਗੇਤਰ, NGX_CONF_ਅਗੇਤਰ);
#ਨਹੀਂਤਾਂ
        ngx_str_ਸੈੱਟ(&ਚੱਕਰ->conf_ਅਗੇਤਰ, NGX_ਅਗੇਤਰ);
#ਜੇ_ਅੰਤ
        ngx_str_ਸੈੱਟ(&ਚੱਕਰ->ਅਗੇਤਰ, NGX_ਅਗੇਤਰ);

#ਜੇ_ਅੰਤ
    }

    ਜੇ (ngx_conf_file) {
        ਚੱਕਰ->conf_file.len = ngx_strlen(ngx_conf_file);
        ਚੱਕਰ->conf_file.ਅੰਕੜੇ = ngx_conf_file;

    } ਨਹੀਂਤਾਂ {
        ngx_str_ਸੈੱਟ(&ਚੱਕਰ->conf_file, NGX_CONF_ਰਸਤਾ);
    }

    ਜੇ (ngx_conf_ਭਰਿਆ_name(ਚੱਕਰ, &ਚੱਕਰ->conf_file, 0) != NGX_OK) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (p = ਚੱਕਰ->conf_file.ਅੰਕੜੇ + ਚੱਕਰ->conf_file.len - 1;
         p > ਚੱਕਰ->conf_file.ਅੰਕੜੇ;
         p--)
    {
        ਜੇ (ngx_ਰਸਤਾ_separator(*p)) {
            ਚੱਕਰ->conf_ਅਗੇਤਰ.len = p - ਚੱਕਰ->conf_file.ਅੰਕੜੇ + 1;
            ਚੱਕਰ->conf_ਅਗੇਤਰ.ਅੰਕੜੇ = ਚੱਕਰ->conf_file.ਅੰਕੜੇ;
            ਰੋਕੋ;
        }
    }

    ਜੇ (ngx_error_log) {
        ਚੱਕਰ->error_log.len = ngx_strlen(ngx_error_log);
        ਚੱਕਰ->error_log.ਅੰਕੜੇ = ngx_error_log;

    } ਨਹੀਂਤਾਂ {
        ngx_str_ਸੈੱਟ(&ਚੱਕਰ->error_log, NGX_ERROR_LOG_ਰਸਤਾ);
    }

    ਜੇ (ngx_conf_params) {
        ਚੱਕਰ->conf_param.len = ngx_strlen(ngx_conf_params);
        ਚੱਕਰ->conf_param.ਅੰਕੜੇ = ngx_conf_params;
    }

    ਜੇ (ngx_test_config) {
        ਚੱਕਰ->ਲਘੂਗਣਕ->log_level = NGX_LOG_INFO;
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ *
ngx_core_module_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(ਚੱਕਰ->pool, ਆਕਾਰ(ngx_core_conf_t));
    ਜੇ (ccf == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    /*
     * set by ngx_pcalloc()
     *
     *     ccf->pid = NULL;
     *     ccf->oldpid = NULL;
     *     ccf->priority = 0;
     *     ccf->cpu_affinity_auto = 0;
     *     ccf->cpu_affinity_n = 0;
     *     ccf->cpu_affinity = NULL;
     */

    ccf->daemon = NGX_CONF_UNSET;
    ccf->master = NGX_CONF_UNSET;
    ccf->timer_resolution = NGX_CONF_UNSET_MSEC;
    ccf->shutdown_timeout = NGX_CONF_UNSET_MSEC;

    ccf->ਕਾਮਾ_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->ਸਮੂਹ = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    ਜੇ (ngx_array_init(&ccf->env, ਚੱਕਰ->pool, 1, ਆਕਾਰ(ngx_str_t))
        != NGX_OK)
    {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ccf;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_core_module_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_ਮੁੱਲ(ccf->daemon, 1);
    ngx_conf_init_ਮੁੱਲ(ccf->master, 1);
    ngx_conf_init_msec_ਮੁੱਲ(ccf->timer_resolution, 0);
    ngx_conf_init_msec_ਮੁੱਲ(ccf->shutdown_timeout, 0);

    ngx_conf_init_ਮੁੱਲ(ccf->ਕਾਮਾ_processes, 1);
    ngx_conf_init_ਮੁੱਲ(ccf->debug_points, 0);

#ਜੇ (NGX_HAVE_CPU_AFFINITY)

    ਜੇ (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->ਕਾਮਾ_processes)
    {
        ngx_log_error(NGX_LOG_WARN, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#ਜੇ_ਅੰਤ


    ਜੇ (ccf->pid.len == 0) {
        ngx_str_ਸੈੱਟ(&ccf->pid, NGX_PID_ਰਸਤਾ);
    }

    ਜੇ (ngx_conf_ਭਰਿਆ_name(ਚੱਕਰ, &ccf->pid, 0) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + ਆਕਾਰ(NGX_OLDPID_EXT);

    ccf->oldpid.ਅੰਕੜੇ = ngx_pnalloc(ਚੱਕਰ->pool, ccf->oldpid.len);
    ਜੇ (ccf->oldpid.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.ਅੰਕੜੇ, ccf->pid.ਅੰਕੜੇ, ccf->pid.len),
               NGX_OLDPID_EXT, ਆਕਾਰ(NGX_OLDPID_EXT));


    ਜੇ (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        ਜੇ (ngx_conf_ਭਰਿਆ_name(ਚੱਕਰ, &ccf->working_directory, 0) != NGX_OK) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ਜੇ (ngx_file_info(ccf->working_directory.ਅੰਕੜੇ, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.ਅੰਕੜੇ);
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ਜੇ (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.ਅੰਕੜੇ);
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }


#ਜੇ !(NGX_WIN32)

    ਜੇ (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ਬਣਤਰ ਸਮੂਹ   *grp;
        ਬਣਤਰ passwd  *pwd;

        ngx_ਸੈੱਟ_errno(0);
        pwd = getpwnam(NGX_USER);
        ਜੇ (pwd == ਨਿਰਾ) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_ਸੈੱਟ_errno(0);
        grp = getgrnam(NGX_ਸਮੂਹ);
        ਜੇ (grp == ਨਿਰਾ) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          "getgrnam(\"" NGX_ਸਮੂਹ "\") failed");
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ccf->ਸਮੂਹ = grp->gr_gid;
    }


    ਜੇ (ccf->ਤਾਲਾ_file.len == 0) {
        ngx_str_ਸੈੱਟ(&ccf->ਤਾਲਾ_file, NGX_ਤਾਲਾ_ਰਸਤਾ);
    }

    ਜੇ (ngx_conf_ਭਰਿਆ_name(ਚੱਕਰ, &ccf->ਤਾਲਾ_file, 0) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    {
    ngx_str_t  ਤਾਲਾ_file;

    ਤਾਲਾ_file = ਚੱਕਰ->old_ਚੱਕਰ->ਤਾਲਾ_file;

    ਜੇ (ਤਾਲਾ_file.len) {
        ਤਾਲਾ_file.len--;

        ਜੇ (ccf->ਤਾਲਾ_file.len != ਤਾਲਾ_file.len
            || ngx_strncmp(ccf->ਤਾਲਾ_file.ਅੰਕੜੇ, ਤਾਲਾ_file.ਅੰਕੜੇ, ਤਾਲਾ_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        ਚੱਕਰ->ਤਾਲਾ_file.len = ਤਾਲਾ_file.len + 1;
        ਤਾਲਾ_file.len += ਆਕਾਰ(".accept");

        ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ = ngx_pstrdup(ਚੱਕਰ->pool, &ਤਾਲਾ_file);
        ਜੇ (ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

    } ਨਹੀਂਤਾਂ {
        ਚੱਕਰ->ਤਾਲਾ_file.len = ccf->ਤਾਲਾ_file.len + 1;
        ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ = ngx_pnalloc(ਚੱਕਰ->pool,
                                      ccf->ਤਾਲਾ_file.len + ਆਕਾਰ(".accept"));
        ਜੇ (ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ, ccf->ਤਾਲਾ_file.ਅੰਕੜੇ,
                              ccf->ਤਾਲਾ_file.len),
                   ".accept", ਆਕਾਰ(".accept"));
    }
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਸੈੱਟ_user(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
#ਜੇ (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    ਵਾਪਸ NGX_CONF_OK;

#ਨਹੀਂਤਾਂ

    ngx_core_conf_t  *ccf = conf;

    ਅੱਖਰ             *ਸਮੂਹ;
    ਬਣਤਰ passwd    *pwd;
    ਬਣਤਰ ਸਮੂਹ     *grp;
    ngx_str_t        *ਮੁੱਲ;

    ਜੇ (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        ਵਾਪਸ "is duplicate";
    }

    ਜੇ (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        ਵਾਪਸ NGX_CONF_OK;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ccf->username = (ਅੱਖਰ *) ਮੁੱਲ[1].ਅੰਕੜੇ;

    ngx_ਸੈੱਟ_errno(0);
    pwd = getpwnam((ਅਚਲ ਅੱਖਰ *) ਮੁੱਲ[1].ਅੰਕੜੇ);
    ਜੇ (pwd == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", ਮੁੱਲ[1].ਅੰਕੜੇ);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    ਸਮੂਹ = (ਅੱਖਰ *) ((cf->ਦਲੀਲਾਂ->nelts == 2) ? ਮੁੱਲ[1].ਅੰਕੜੇ : ਮੁੱਲ[2].ਅੰਕੜੇ);

    ngx_ਸੈੱਟ_errno(0);
    grp = getgrnam(ਸਮੂਹ);
    ਜੇ (grp == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", ਸਮੂਹ);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ccf->ਸਮੂਹ = grp->gr_gid;

    ਵਾਪਸ NGX_CONF_OK;

#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਸੈੱਟ_env(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *ਮੁੱਲ, *var;
    ngx_uint_t   i;

    var = ngx_array_ਧੱਕੋ(&ccf->env);
    ਜੇ (var == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;
    *var = ਮੁੱਲ[1];

    ਲਈ (i = 0; i < ਮੁੱਲ[1].len; i++) {

        ਜੇ (ਮੁੱਲ[1].ਅੰਕੜੇ[i] == '=') {

            var->len = i;

            ਵਾਪਸ NGX_CONF_OK;
        }
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਸੈੱਟ_ਤਰਜੀਹ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *ਮੁੱਲ;
    ngx_uint_t        n, minus;

    ਜੇ (ccf->ਤਰਜੀਹ != 0) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਜੇ (ਮੁੱਲ[1].ਅੰਕੜੇ[0] == '-') {
        n = 1;
        minus = 1;

    } ਨਹੀਂਤਾਂ ਜੇ (ਮੁੱਲ[1].ਅੰਕੜੇ[0] == '+') {
        n = 1;
        minus = 0;

    } ਨਹੀਂਤਾਂ {
        n = 0;
        minus = 0;
    }

    ccf->ਤਰਜੀਹ = ngx_atoi(&ਮੁੱਲ[1].ਅੰਕੜੇ[n], ਮੁੱਲ[1].len - n);
    ਜੇ (ccf->ਤਰਜੀਹ == NGX_ERROR) {
        ਵਾਪਸ "invalid number";
    }

    ਜੇ (minus) {
        ccf->ਤਰਜੀਹ = -ccf->ਤਰਜੀਹ;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਸੈੱਟ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
#ਜੇ (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *ਮੁੱਲ;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *ਨਕਾਬ;

    ਜੇ (ccf->cpu_affinity) {
        ਵਾਪਸ "is duplicate";
    }

    ਨਕਾਬ = ngx_palloc(cf->pool, (cf->ਦਲੀਲਾਂ->nelts - 1) * ਆਕਾਰ(ngx_cpuset_t));
    ਜੇ (ਨਕਾਬ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->ਦਲੀਲਾਂ->nelts - 1;
    ccf->cpu_affinity = ਨਕਾਬ;

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਜੇ (ngx_strcmp(ਮੁੱਲ[1].ਅੰਕੜੇ, "auto") == 0) {

        ਜੇ (cf->ਦਲੀਲਾਂ->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&ਨਕਾਬ[0]);
        ਲਈ (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_ਸੈੱਟ(i, &ਨਕਾਬ[0]);
        }

        n = 2;

    } ਨਹੀਂਤਾਂ {
        n = 1;
    }

    ਲਈ ( /* void */ ; n < cf->ਦਲੀਲਾਂ->nelts; n++) {

        ਜੇ (ਮੁੱਲ[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&ਨਕਾਬ[n - 1]);

        ਲਈ (p = ਮੁੱਲ[n].ਅੰਕੜੇ + ਮੁੱਲ[n].len - 1;
             p >= ਮੁੱਲ[n].ਅੰਕੜੇ;
             p--)
        {
            ch = *p;

            ਜੇ (ch == ' ') {
                ਜਾਰੀ;
            }

            i++;

            ਜੇ (ch == '0') {
                ਜਾਰੀ;
            }

            ਜੇ (ch == '1') {
                CPU_ਸੈੱਟ(i - 1, &ਨਕਾਬ[n - 1]);
                ਜਾਰੀ;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

#ਨਹੀਂਤਾਂ

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#ਜੇ (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *ਨਕਾਬ;
    ngx_core_conf_t  *ccf;

    ਸਥਿਰ ngx_cpuset_t  ਨਤੀਜਾ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_ਚੱਕਰ->conf_ctx,
                                           ngx_core_module);

    ਜੇ (ccf->cpu_affinity == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ccf->cpu_affinity_auto) {
        ਨਕਾਬ = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        ਲਈ (i = 0, j = n; /* void */ ; i++) {

            ਜੇ (CPU_ISSET(i % CPU_SETSIZE, ਨਕਾਬ) && j-- == 0) {
                ਰੋਕੋ;
            }

            ਜੇ (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                ਵਾਪਸ ਨਿਰਾ;
            }

            /* void */
        }

        CPU_ZERO(&ਨਤੀਜਾ);
        CPU_ਸੈੱਟ(i % CPU_SETSIZE, &ਨਤੀਜਾ);

        ਵਾਪਸ &ਨਤੀਜਾ;
    }

    ਜੇ (ccf->cpu_affinity_n > n) {
        ਵਾਪਸ &ccf->cpu_affinity[n];
    }

    ਵਾਪਸ &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#ਨਹੀਂਤਾਂ

    ਵਾਪਸ ਨਿਰਾ;

#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਸੈੱਟ_ਕਾਮਾ_processes(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ngx_str_t        *ਮੁੱਲ;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    ਜੇ (ccf->ਕਾਮਾ_processes != NGX_CONF_UNSET) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਜੇ (ngx_strcmp(ਮੁੱਲ[1].ਅੰਕੜੇ, "auto") == 0) {
        ccf->ਕਾਮਾ_processes = ngx_ncpu;
        ਵਾਪਸ NGX_CONF_OK;
    }

    ccf->ਕਾਮਾ_processes = ngx_atoi(ਮੁੱਲ[1].ਅੰਕੜੇ, ਮੁੱਲ[1].len);

    ਜੇ (ccf->ਕਾਮਾ_processes == NGX_ERROR) {
        ਵਾਪਸ "invalid value";
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_ਲੱਦੋ_module(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
#ਜੇ (NGX_HAVE_DLOPEN)
    ਖਾਲੀ                *handle;
    ਅੱਖਰ               **ਨਾਂ, **ਕ੍ਰਮ;
    ngx_str_t           *ਮੁੱਲ, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    ਜੇ (cf->ਚੱਕਰ->modules_ਵਰਤਿਆ) {
        ਵਾਪਸ "is specified too late";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    file = ਮੁੱਲ[1];

    ਜੇ (ngx_conf_ਭਰਿਆ_name(cf->ਚੱਕਰ, &file, 0) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_ਜੋੜੋ(cf->ਚੱਕਰ->pool, 0);
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.ਅੰਕੜੇ);
    ਜੇ (handle == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.ਅੰਕੜੇ, ngx_dlerror());
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->ਅੰਕੜੇ = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    ਜੇ (modules == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ਮੁੱਲ[1], "ngx_modules", ngx_dlerror());
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਨਾਂ = ngx_dlsym(handle, "ngx_module_names");
    ਜੇ (ਨਾਂ == ਨਿਰਾ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ਮੁੱਲ[1], "ngx_module_names", ngx_dlerror());
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਕ੍ਰਮ = ngx_dlsym(handle, "ngx_module_order");

    ਲਈ (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = ਨਾਂ[i];

        ਜੇ (ngx_ਜੋੜੋ_module(cf, &file, module, ਕ੍ਰਮ) != NGX_OK) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0, "module: %s i:%ui",
                       module->name, module->ਸੂਚਕਾਂਕ);
    }

    ਵਾਪਸ NGX_CONF_OK;

#ਨਹੀਂਤਾਂ

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    ਵਾਪਸ NGX_CONF_ERROR;

#ਜੇ_ਅੰਤ
}


#ਜੇ (NGX_HAVE_DLOPEN)

ਸਥਿਰ ਖਾਲੀ
ngx_unload_module(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਖਾਲੀ  *handle = ਅੰਕੜੇ;

    ਜੇ (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#ਜੇ_ਅੰਤ
