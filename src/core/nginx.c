
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <nginx.h>


ସ୍ଥିର ଶୂନ୍ୟ ngx_show_version_info(ଶୂନ୍ୟ);
ସ୍ଥିର ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
ସ୍ଥିର ଶୂନ୍ୟ ngx_cleanup_environment(ଶୂନ୍ୟ *data);
ସ୍ଥିର ଶୂନ୍ୟ ngx_cleanup_environment_variable(ଶୂନ୍ୟ *data);
ସ୍ଥିର ngx_int_t ngx_get_options(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv);
ସ୍ଥିର ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
ସ୍ଥିର ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_core_module_create_conf(ngx_cycle_t *cycle);
ସ୍ଥିର ଅକ୍ଷର *ngx_core_module_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
#ଯଦି (NGX_HAVE_DLOPEN)
ସ୍ଥିର ଶୂନ୍ୟ ngx_unload_module(ଶୂନ୍ୟ *data);
#endif


ସ୍ଥିର ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ସ୍ଥିର ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ଖାଲି },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      ଖାଲି },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ଖାଲି },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      ଖାଲି },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      ଖାଲି },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      ଖାଲି },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE12,
      ngx_set_user,
      0,
      0,
      ଖାଲି },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      ଖାଲି },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      ଖାଲି },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_ଅଙ୍କ_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ଖାଲି },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ଖାଲି },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ଖାଲି },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ଖାଲି },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      ଖାଲି },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      ଖାଲି },

      ngx_null_command
};


ସ୍ଥିର ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ଖାଲି,                                  /* init master */
    ଖାଲି,                                  /* init module */
    ଖାଲି,                                  /* init process */
    ଖାଲି,                                  /* init thread */
    ଖାଲି,                                  /* exit thread */
    ଖାଲି,                                  /* exit process */
    ଖାଲି,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ସ୍ଥିର ngx_uint_t   ngx_show_help;
ସ୍ଥିର ngx_uint_t   ngx_show_version;
ସ୍ଥିର ngx_uint_t   ngx_show_configure;
ସ୍ଥିର u_char      *ngx_prefix;
ସ୍ଥିର u_char      *ngx_error_log;
ସ୍ଥିର u_char      *ngx_conf_file;
ସ୍ଥିର u_char      *ngx_conf_params;
#ଯଦି (NGX_CONTROL_API)
ସ୍ଥିର u_char      *ngx_control_addr;
#endif
ସ୍ଥିର ଅକ୍ଷର        *ngx_signal;


ସ୍ଥିର ଅକ୍ଷର **ngx_os_environ;


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngx_cdecl
ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    ଯଦି (ngx_strerror_init() != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_get_options(argc, argv) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_show_version) {
        ngx_show_version_info();

        ଯଦି (!ngx_test_config) {
            ଫେରନ୍ତୁ 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#ଯଦି (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    ଯଦି (log == ଖାଲି) {
        ଫେରନ୍ତୁ 1;
    }

    /* STUB */
#ଯଦି (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, ଆକାର(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    ଯଦି (init_cycle.pool == ଖାଲି) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_process_options(&init_cycle) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_os_init(log) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    ଯଦି (ngx_crc32_table_init() != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    ଯଦି (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

#ଯଦି (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    ଯଦି (ngx_preinit_modules() != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    ଯଦି (cycle == ଖାଲି) {
        ଯଦି (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_test_config) {
        ଯଦି (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        ଯଦି (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            ପାଇଁ (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (ଶୂନ୍ୟ) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (ଶୂନ୍ୟ) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        ଫେରନ୍ତୁ 0;
    }

    ଯଦି (ngx_signal) {
        ଫେରନ୍ତୁ ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ଯଦି (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#ଯଦି !(NGX_WIN32)

    ଯଦି (ngx_init_signals(cycle->log) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (!ngx_inherited && ccf->daemon) {
        ଯଦି (ngx_daemon(cycle->log) != NGX_OK) {
            ଫେରନ୍ତୁ 1;
        }

        ngx_daemonized = 1;
    }

    ଯଦି (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#ଯଦି (NGX_CONTROL_API)
    ଯଦି (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } ଅନ୍ୟଥା {
        ଯଦି (ngx_control_init(ngx_control_addr) != NGX_OK) {
            ଫେରନ୍ତୁ 1;
        }
    }
#endif

    ଯଦି (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି (log->file->fd != ngx_stderr) {
        ଯଦି (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    ଯଦି (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } ଅନ୍ୟଥା {
        ngx_master_process_cycle(cycle);
    }

    ଫେରନ୍ତୁ 0;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_show_version_info(ଶୂନ୍ୟ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    ଯଦି (ngx_show_help) {
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
#ଯଦି (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#ଅନ୍ୟଥା
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#ଅନ୍ୟଥା
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    ଯଦି (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#ଯଦି (NGX_SSL)
        ଯଦି (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } ଅନ୍ୟଥା {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((ଅକ୍ଷର *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#ଅନ୍ୟଥା
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ସ୍ଥିର ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    ଯଦି (inherited == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    ଯଦି (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       ଆକାର(ngx_listening_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ (p = inherited, v = p; *p; p++) {
        ଯଦି (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            ଯଦି (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                ଭାଙ୍ଗ;
            }

            v = p + 1;

            ls = ngx_array_ଠେଲ(&cycle->listening);
            ଯଦି (ls == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ngx_memzero(ls, ଆକାର(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    ଯଦି (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    ଫେରନ୍ତୁ ngx_set_inherited_sockets(cycle);
}


ଅକ୍ଷର **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    ଅକ୍ଷର                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ଯଦି (last == ଖାଲି && ccf->environment) {
        ଫେରନ୍ତୁ ccf->environment;
    }

    var = ccf->env.elts;

    ପାଇଁ (i = 0; i < ccf->env.nelts; i++) {
        ଯଦି (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            ଯାଅ tz_found;
        }
    }

    var = ngx_array_ଠେଲ(&ccf->env);
    ଯଦି (var == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    ପାଇଁ (i = 0; i < ccf->env.nelts; i++) {

        ଯଦି (var[i].data[var[i].len] == '=') {
            n++;
            ଜାରି;
        }

        ପାଇଁ (p = ngx_os_environ; *p; p++) {

            ଯଦି (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                ଭାଙ୍ଗ;
            }
        }
    }

    ଯଦି (last) {
        env = ngx_alloc((*last + n + 1) * ଆକାର(ଅକ୍ଷର *), cycle->log);
        ଯଦି (env == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        *last = n;

    } ଅନ୍ୟଥା {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        ଯଦି (cln == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        env = ngx_alloc((n + 1) * ଆକାର(ଅକ୍ଷର *), cycle->log);
        ଯଦି (env == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    ପାଇଁ (i = 0; i < ccf->env.nelts; i++) {

        ଯଦି (var[i].data[var[i].len] == '=') {

            ଯଦି (last) {
                env[n++] = (ଅକ୍ଷର *) var[i].data;
                ଜାରି;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            ଯଦି (cln == ଖାଲି) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            ଯଦି (str == ଖାଲି) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            ଜାରି;
        }

        ପାଇଁ (p = ngx_os_environ; *p; p++) {

            ଯଦି (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                ଭାଙ୍ଗ;
            }
        }
    }

    env[n] = ଖାଲି;

    ଯଦି (last == ଖାଲି) {
        ccf->environment = env;
        environ = env;
    }

    ଫେରନ୍ତୁ env;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_cleanup_environment(ଶୂନ୍ୟ *data)
{
    ଅକ୍ଷର  **env = data;

    ଯଦି (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        ଫେରନ୍ତୁ;
    }

    ngx_free(env);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_cleanup_environment_variable(ଶୂନ୍ୟ *data)
{
    ଅକ୍ଷର  *var = data;

    ଅକ୍ଷର  **p;

    ପାଇଁ (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        ଯଦି (*p == var) {
            ଫେରନ୍ତୁ;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ଦ୍ୱିମିକ(ngx_cycle_t *cycle, ଅକ୍ଷର *ଅଚଳ *argv)
{
    ଅକ୍ଷର             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, ଆକାର(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    ଯଦି (env == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_INVALID_PID;
    }

    var = ngx_alloc(ଆକାର(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    ଯଦି (var == ଖାଲି) {
        ngx_free(env);
        ଫେରନ୍ତୁ NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", ଆକାର(NGINX_VAR));

    ls = cycle->listening.elts;
    ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {
        ଯଦି (ls[i].ignore) {
            ଜାରି;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#ଯଦି (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#ଯଦି (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    ଯଦି (env[n] != ଖାଲି) {
        n++;
    }
#endif

    env[n] = ଖାଲି;

#ଯଦି (NGX_DEBUG)
    {
    ଅକ୍ଷର  **e;
    ପାଇଁ (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (ଅକ୍ଷର *ଅଚଳ *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ଯଦି (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        ଫେରନ୍ତୁ NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    ଯଦି (pid == NGX_INVALID_PID) {
        ଯଦି (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.data, ccf->pid.data, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    ଫେରନ୍ତୁ pid;
}


ସ୍ଥିର ngx_int_t
ngx_get_options(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv)
{
    u_char     *p;
    ngx_int_t   i;

    ପାଇଁ (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        ଯଦି (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯେତେବେଳେ (*p) {

            ବାଛ (*p++) {

            ସ୍ଥିତି '?':
            ସ୍ଥିତି 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'v':
                ngx_show_version = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 't':
                ngx_test_config = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'q':
                ngx_quiet_mode = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'p':
                ଯଦି (*p) {
                    ngx_prefix = p;
                    ଯାଅ next;
                }

                ଯଦି (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    ଯାଅ next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                ଫେରନ୍ତୁ NGX_ERROR;

            ସ୍ଥିତି 'e':
                ଯଦି (*p) {
                    ngx_error_log = p;

                } ଅନ୍ୟଥା ଯଦି (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } ଅନ୍ୟଥା {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                ଯାଅ next;

            ସ୍ଥିତି 'c':
                ଯଦି (*p) {
                    ngx_conf_file = p;
                    ଯାଅ next;
                }

                ଯଦି (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    ଯାଅ next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                ଫେରନ୍ତୁ NGX_ERROR;

            ସ୍ଥିତି 'g':
                ଯଦି (*p) {
                    ngx_conf_params = p;
                    ଯାଅ next;
                }

                ଯଦି (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    ଯାଅ next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                ଫେରନ୍ତୁ NGX_ERROR;

            ସ୍ଥିତି 's':
                ଯଦି (*p) {
                    ngx_signal = (ଅକ୍ଷର *) p;

                } ଅନ୍ୟଥା ଯଦି (argv[++i]) {
                    ngx_signal = argv[i];

                } ଅନ୍ୟଥା {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    ଯାଅ next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                ଫେରନ୍ତୁ NGX_ERROR;

#ଯଦି (NGX_CONTROL_API)
            ସ୍ଥିତି 'l':
                ngx_control_api_enabled = 1;

                ଯଦି (*p) {
                    ngx_control_addr = p;
                    ଯାଅ next;
                }

                ଯଦି (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    ଯାଅ next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                ଫେରନ୍ତୁ NGX_ERROR;
#endif

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

    next:

        ଜାରି;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv)
{
#ଯଦି (NGX_FREEBSD)

    ngx_os_argv = (ଅକ୍ଷର **) argv;
    ngx_argc = argc;
    ngx_argv = (ଅକ୍ଷର **) argv;

#ଅନ୍ୟଥା
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (ଅକ୍ଷର **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * ଆକାର(ଅକ୍ଷର *), cycle->log);
    ଯଦି (ngx_argv == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        ଯଦି (ngx_argv[i] == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        (ଶୂନ୍ୟ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ଖାଲି;

#endif

    ngx_os_environ = environ;

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    ଯଦି (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        ଯଦି (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            ଯଦି (p == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } ଅନ୍ୟଥା {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#ଅନ୍ୟଥା

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#ଅନ୍ୟଥା
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    ଯଦି (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } ଅନ୍ୟଥା {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    ଯଦି (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        ଯଦି (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } ଅନ୍ୟଥା {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    ଯଦି (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    ଯଦି (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, ଆକାର(ngx_core_conf_t));
    ଯଦି (ccf == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
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

    ccf->worker_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->group = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    ଯଦି (ngx_array_init(&ccf->env, cycle->pool, 1, ଆକାର(ngx_str_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ ccf;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_core_module_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_ମୂଲ୍ୟ(ccf->daemon, 1);
    ngx_conf_init_ମୂଲ୍ୟ(ccf->master, 1);
    ngx_conf_init_msec_ମୂଲ୍ୟ(ccf->timer_resolution, 0);
    ngx_conf_init_msec_ମୂଲ୍ୟ(ccf->shutdown_timeout, 0);

    ngx_conf_init_ମୂଲ୍ୟ(ccf->worker_processes, 1);
    ngx_conf_init_ମୂଲ୍ୟ(ccf->debug_points, 0);

#ଯଦି (NGX_HAVE_CPU_AFFINITY)

    ଯଦି (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->worker_processes)
    {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#endif


    ଯଦି (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    ଯଦି (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + ଆକାର(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    ଯଦି (ccf->oldpid.data == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, ଆକାର(NGX_OLDPID_EXT));


    ଯଦି (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        ଯଦି (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ଯଦି (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ଯଦି (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }


#ଯଦି !(NGX_WIN32)

    ଯଦି (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ସଂରଚନା group   *grp;
        ସଂରଚନା passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        ଯଦି (pwd == ଖାଲି) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        ଯଦି (grp == ଖାଲି) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    ଯଦି (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    ଯଦି (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    ଯଦି (lock_file.len) {
        lock_file.len--;

        ଯଦି (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += ଆକାର(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        ଯଦି (cycle->lock_file.data == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

    } ଅନ୍ୟଥା {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + ଆକାର(".accept"));
        ଯଦି (cycle->lock_file.data == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", ଆକାର(".accept"));
    }
    }

#endif

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
#ଯଦି (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    ଫେରନ୍ତୁ NGX_CONF_OK;

#ଅନ୍ୟଥା

    ngx_core_conf_t  *ccf = conf;

    ଅକ୍ଷର             *group;
    ସଂରଚନା passwd    *pwd;
    ସଂରଚନା group     *grp;
    ngx_str_t        *ମୂଲ୍ୟ;

    ଯଦି (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ଯଦି (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ccf->username = (ଅକ୍ଷର *) ମୂଲ୍ୟ[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ଅଚଳ ଅକ୍ଷର *) ମୂଲ୍ୟ[1].data);
    ଯଦି (pwd == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", ମୂଲ୍ୟ[1].data);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (ଅକ୍ଷର *) ((cf->args->nelts == 2) ? ମୂଲ୍ୟ[1].data : ମୂଲ୍ୟ[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    ଯଦି (grp == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    ଫେରନ୍ତୁ NGX_CONF_OK;

#endif
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *ମୂଲ୍ୟ, *var;
    ngx_uint_t   i;

    var = ngx_array_ଠେଲ(&ccf->env);
    ଯଦି (var == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ମୂଲ୍ୟ = cf->args->elts;
    *var = ମୂଲ୍ୟ[1];

    ପାଇଁ (i = 0; i < ମୂଲ୍ୟ[1].len; i++) {

        ଯଦି (ମୂଲ୍ୟ[1].data[i] == '=') {

            var->len = i;

            ଫେରନ୍ତୁ NGX_CONF_OK;
        }
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_uint_t        n, minus;

    ଯଦି (ccf->priority != 0) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ଯଦି (ମୂଲ୍ୟ[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } ଅନ୍ୟଥା ଯଦି (ମୂଲ୍ୟ[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } ଅନ୍ୟଥା {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&ମୂଲ୍ୟ[1].data[n], ମୂଲ୍ୟ[1].len - n);
    ଯଦି (ccf->priority == NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid number";
    }

    ଯଦି (minus) {
        ccf->priority = -ccf->priority;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
#ଯଦି (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    ଯଦି (ccf->cpu_affinity) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * ଆକାର(ngx_cpuset_t));
    ଯଦି (mask == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    ମୂଲ୍ୟ = cf->args->elts;

    ଯଦି (ngx_strcmp(ମୂଲ୍ୟ[1].data, "auto") == 0) {

        ଯଦି (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        ପାଇଁ (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } ଅନ୍ୟଥା {
        n = 1;
    }

    ପାଇଁ ( /* void */ ; n < cf->args->nelts; n++) {

        ଯଦି (ମୂଲ୍ୟ[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        ପାଇଁ (p = ମୂଲ୍ୟ[n].data + ମୂଲ୍ୟ[n].len - 1;
             p >= ମୂଲ୍ୟ[n].data;
             p--)
        {
            ch = *p;

            ଯଦି (ch == ' ') {
                ଜାରି;
            }

            i++;

            ଯଦି (ch == '0') {
                ଜାରି;
            }

            ଯଦି (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                ଜାରି;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

#ଅନ୍ୟଥା

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#ଯଦି (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    ସ୍ଥିର ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    ଯଦି (ccf->cpu_affinity == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        ପାଇଁ (i = 0, j = n; /* void */ ; i++) {

            ଯଦି (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                ଫେରନ୍ତୁ ଖାଲି;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        ଫେରନ୍ତୁ &result;
    }

    ଯଦି (ccf->cpu_affinity_n > n) {
        ଫେରନ୍ତୁ &ccf->cpu_affinity[n];
    }

    ଫେରନ୍ତୁ &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#ଅନ୍ୟଥା

    ଫେରନ୍ତୁ ଖାଲି;

#endif
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    ଯଦି (ccf->worker_processes != NGX_CONF_UNSET) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ଯଦି (ngx_strcmp(ମୂଲ୍ୟ[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(ମୂଲ୍ୟ[1].data, ମୂଲ୍ୟ[1].len);

    ଯଦି (ccf->worker_processes == NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
#ଯଦି (NGX_HAVE_DLOPEN)
    ଶୂନ୍ୟ                *handle;
    ଅକ୍ଷର               **names, **order;
    ngx_str_t           *ମୂଲ୍ୟ, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    ଯଦି (cf->cycle->modules_used) {
        ଫେରନ୍ତୁ "is specified too late";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    file = ମୂଲ୍ୟ[1];

    ଯଦି (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    ଯଦି (cln == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    ଯଦି (handle == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    ଯଦି (modules == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ମୂଲ୍ୟ[1], "ngx_modules", ngx_dlerror());
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    ଯଦି (names == ଖାଲି) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ମୂଲ୍ୟ[1], "ngx_module_names", ngx_dlerror());
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    ପାଇଁ (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        ଯଦି (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;

#ଅନ୍ୟଥା

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    ଫେରନ୍ତୁ NGX_CONF_ERROR;

#endif
}


#ଯଦି (NGX_HAVE_DLOPEN)

ସ୍ଥିର ଶୂନ୍ୟ
ngx_unload_module(ଶୂନ୍ୟ *data)
{
    ଶୂନ୍ୟ  *handle = data;

    ଯଦି (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
