
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <nginx.h>


স্থির শূন্য ngx_show_version_info(শূন্য);
স্থির ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
স্থির শূন্য ngx_cleanup_environment(শূন্য *data);
স্থির শূন্য ngx_cleanup_environment_variable(শূন্য *data);
স্থির ngx_int_t ngx_get_options(পূর্ণাঙ্ক argc, বর্ণ *ধ্রুব *argv);
স্থির ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
স্থির ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, পূর্ণাঙ্ক argc, বর্ণ *ধ্রুব *argv);
স্থির শূন্য *ngx_core_module_create_conf(ngx_cycle_t *cycle);
স্থির বর্ণ *ngx_core_module_init_conf(ngx_cycle_t *cycle, শূন্য *conf);
স্থির বর্ণ *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
স্থির বর্ণ *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
স্থির বর্ণ *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
স্থির বর্ণ *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    শূন্য *conf);
স্থির বর্ণ *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    শূন্য *conf);
স্থির বর্ণ *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
#যদি (NGX_HAVE_DLOPEN)
স্থির শূন্য ngx_unload_module(শূন্য *data);
#endif


স্থির ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


স্থির ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      রিক্ত },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      রিক্ত },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      রিক্ত },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      রিক্ত },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      রিক্ত },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      রিক্ত },

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
      রিক্ত },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      রিক্ত },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      রিক্ত },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_অঙ্ক_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      রিক্ত },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      রিক্ত },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      রিক্ত },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      রিক্ত },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      রিক্ত },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      রিক্ত },

      ngx_null_command
};


স্থির ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    রিক্ত,                                  /* init master */
    রিক্ত,                                  /* init module */
    রিক্ত,                                  /* init process */
    রিক্ত,                                  /* init thread */
    রিক্ত,                                  /* exit thread */
    রিক্ত,                                  /* exit process */
    রিক্ত,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_uint_t   ngx_show_help;
স্থির ngx_uint_t   ngx_show_version;
স্থির ngx_uint_t   ngx_show_configure;
স্থির u_char      *ngx_prefix;
স্থির u_char      *ngx_error_log;
স্থির u_char      *ngx_conf_file;
স্থির u_char      *ngx_conf_params;
#যদি (NGX_CONTROL_API)
স্থির u_char      *ngx_control_addr;
#endif
স্থির বর্ণ        *ngx_signal;


স্থির বর্ণ **ngx_os_environ;


পূর্ণাঙ্ক ngx_cdecl
main(পূর্ণাঙ্ক argc, বর্ণ *ধ্রুব *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    যদি (ngx_strerror_init() != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (ngx_get_options(argc, argv) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (ngx_show_version) {
        ngx_show_version_info();

        যদি (!ngx_test_config) {
            প্রত্যাগম 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#যদি (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    যদি (log == রিক্ত) {
        প্রত্যাগম 1;
    }

    /* STUB */
#যদি (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, পরিমাণ(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    যদি (init_cycle.pool == রিক্ত) {
        প্রত্যাগম 1;
    }

    যদি (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (ngx_process_options(&init_cycle) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (ngx_os_init(log) != NGX_OK) {
        প্রত্যাগম 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    যদি (ngx_crc32_table_init() != NGX_OK) {
        প্রত্যাগম 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    যদি (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        প্রত্যাগম 1;
    }

#যদি (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    যদি (ngx_preinit_modules() != NGX_OK) {
        প্রত্যাগম 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    যদি (cycle == রিক্ত) {
        যদি (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        প্রত্যাগম 1;
    }

    যদি (ngx_test_config) {
        যদি (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        যদি (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            প্রতি (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (শূন্য) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (শূন্য) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        প্রত্যাগম 0;
    }

    যদি (ngx_signal) {
        প্রত্যাগম ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#যদি !(NGX_WIN32)

    যদি (ngx_init_signals(cycle->log) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (!ngx_inherited && ccf->daemon) {
        যদি (ngx_daemon(cycle->log) != NGX_OK) {
            প্রত্যাগম 1;
        }

        ngx_daemonized = 1;
    }

    যদি (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#যদি (NGX_CONTROL_API)
    যদি (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } অন্যথা {
        যদি (ngx_control_init(ngx_control_addr) != NGX_OK) {
            প্রত্যাগম 1;
        }
    }
#endif

    যদি (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        প্রত্যাগম 1;
    }

    যদি (log->file->fd != ngx_stderr) {
        যদি (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    যদি (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } অন্যথা {
        ngx_master_process_cycle(cycle);
    }

    প্রত্যাগম 0;
}


স্থির শূন্য
ngx_show_version_info(শূন্য)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    যদি (ngx_show_help) {
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
#যদি (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#অন্যথা
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#অন্যথা
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    যদি (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#যদি (NGX_SSL)
        যদি (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } অন্যথা {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((বর্ণ *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#অন্যথা
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


স্থির ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    যদি (inherited == রিক্ত) {
        প্রত্যাগম NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    যদি (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       পরিমাণ(ngx_listening_t))
        != NGX_OK)
    {
        প্রত্যাগম NGX_ERROR;
    }

    প্রতি (p = inherited, v = p; *p; p++) {
        যদি (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            যদি (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                ভঙ্গ;
            }

            v = p + 1;

            ls = ngx_array_ঠেলো(&cycle->listening);
            যদি (ls == রিক্ত) {
                প্রত্যাগম NGX_ERROR;
            }

            ngx_memzero(ls, পরিমাণ(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    যদি (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    প্রত্যাগম ngx_set_inherited_sockets(cycle);
}


বর্ণ **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    বর্ণ                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (last == রিক্ত && ccf->environment) {
        প্রত্যাগম ccf->environment;
    }

    var = ccf->env.elts;

    প্রতি (i = 0; i < ccf->env.nelts; i++) {
        যদি (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            গচ্ছ tz_found;
        }
    }

    var = ngx_array_ঠেলো(&ccf->env);
    যদি (var == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    প্রতি (i = 0; i < ccf->env.nelts; i++) {

        যদি (var[i].data[var[i].len] == '=') {
            n++;
            অনুবর্ত;
        }

        প্রতি (p = ngx_os_environ; *p; p++) {

            যদি (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                ভঙ্গ;
            }
        }
    }

    যদি (last) {
        env = ngx_alloc((*last + n + 1) * পরিমাণ(বর্ণ *), cycle->log);
        যদি (env == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        *last = n;

    } অন্যথা {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        যদি (cln == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        env = ngx_alloc((n + 1) * পরিমাণ(বর্ণ *), cycle->log);
        যদি (env == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    প্রতি (i = 0; i < ccf->env.nelts; i++) {

        যদি (var[i].data[var[i].len] == '=') {

            যদি (last) {
                env[n++] = (বর্ণ *) var[i].data;
                অনুবর্ত;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            যদি (cln == রিক্ত) {
                প্রত্যাগম রিক্ত;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            যদি (str == রিক্ত) {
                প্রত্যাগম রিক্ত;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            অনুবর্ত;
        }

        প্রতি (p = ngx_os_environ; *p; p++) {

            যদি (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                ভঙ্গ;
            }
        }
    }

    env[n] = রিক্ত;

    যদি (last == রিক্ত) {
        ccf->environment = env;
        environ = env;
    }

    প্রত্যাগম env;
}


স্থির শূন্য
ngx_cleanup_environment(শূন্য *data)
{
    বর্ণ  **env = data;

    যদি (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        প্রত্যাগম;
    }

    ngx_free(env);
}


স্থির শূন্য
ngx_cleanup_environment_variable(শূন্য *data)
{
    বর্ণ  *var = data;

    বর্ণ  **p;

    প্রতি (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        যদি (*p == var) {
            প্রত্যাগম;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_দ্বিমিক(ngx_cycle_t *cycle, বর্ণ *ধ্রুব *argv)
{
    বর্ণ             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, পরিমাণ(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    যদি (env == রিক্ত) {
        প্রত্যাগম NGX_INVALID_PID;
    }

    var = ngx_alloc(পরিমাণ(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    যদি (var == রিক্ত) {
        ngx_free(env);
        প্রত্যাগম NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", পরিমাণ(NGINX_VAR));

    ls = cycle->listening.elts;
    প্রতি (i = 0; i < cycle->listening.nelts; i++) {
        যদি (ls[i].ignore) {
            অনুবর্ত;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#যদি (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#যদি (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    যদি (env[n] != রিক্ত) {
        n++;
    }
#endif

    env[n] = রিক্ত;

#যদি (NGX_DEBUG)
    {
    বর্ণ  **e;
    প্রতি (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (বর্ণ *ধ্রুব *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        প্রত্যাগম NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    যদি (pid == NGX_INVALID_PID) {
        যদি (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    প্রত্যাগম pid;
}


স্থির ngx_int_t
ngx_get_options(পূর্ণাঙ্ক argc, বর্ণ *ধ্রুব *argv)
{
    u_char     *p;
    ngx_int_t   i;

    প্রতি (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        যদি (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            প্রত্যাগম NGX_ERROR;
        }

        যাবত্ (*p) {

            বিকল্প (*p++) {

            পক্ষ '?':
            পক্ষ 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                ভঙ্গ;

            পক্ষ 'v':
                ngx_show_version = 1;
                ভঙ্গ;

            পক্ষ 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                ভঙ্গ;

            পক্ষ 't':
                ngx_test_config = 1;
                ভঙ্গ;

            পক্ষ 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                ভঙ্গ;

            পক্ষ 'q':
                ngx_quiet_mode = 1;
                ভঙ্গ;

            পক্ষ 'p':
                যদি (*p) {
                    ngx_prefix = p;
                    গচ্ছ next;
                }

                যদি (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    গচ্ছ next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                প্রত্যাগম NGX_ERROR;

            পক্ষ 'e':
                যদি (*p) {
                    ngx_error_log = p;

                } অন্যথা যদি (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } অন্যথা {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    প্রত্যাগম NGX_ERROR;
                }

                যদি (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                গচ্ছ next;

            পক্ষ 'c':
                যদি (*p) {
                    ngx_conf_file = p;
                    গচ্ছ next;
                }

                যদি (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    গচ্ছ next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                প্রত্যাগম NGX_ERROR;

            পক্ষ 'g':
                যদি (*p) {
                    ngx_conf_params = p;
                    গচ্ছ next;
                }

                যদি (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    গচ্ছ next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                প্রত্যাগম NGX_ERROR;

            পক্ষ 's':
                যদি (*p) {
                    ngx_signal = (বর্ণ *) p;

                } অন্যথা যদি (argv[++i]) {
                    ngx_signal = argv[i];

                } অন্যথা {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    প্রত্যাগম NGX_ERROR;
                }

                যদি (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    গচ্ছ next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                প্রত্যাগম NGX_ERROR;

#যদি (NGX_CONTROL_API)
            পক্ষ 'l':
                ngx_control_api_enabled = 1;

                যদি (*p) {
                    ngx_control_addr = p;
                    গচ্ছ next;
                }

                যদি (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    গচ্ছ next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                প্রত্যাগম NGX_ERROR;
#endif

            স্বভাব:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                প্রত্যাগম NGX_ERROR;
            }
        }

    next:

        অনুবর্ত;
    }

    প্রত্যাগম NGX_OK;
}


স্থির ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, পূর্ণাঙ্ক argc, বর্ণ *ধ্রুব *argv)
{
#যদি (NGX_FREEBSD)

    ngx_os_argv = (বর্ণ **) argv;
    ngx_argc = argc;
    ngx_argv = (বর্ণ **) argv;

#অন্যথা
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (বর্ণ **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * পরিমাণ(বর্ণ *), cycle->log);
    যদি (ngx_argv == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    প্রতি (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        যদি (ngx_argv[i] == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        (শূন্য) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = রিক্ত;

#endif

    ngx_os_environ = environ;

    প্রত্যাগম NGX_OK;
}


স্থির ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    যদি (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        যদি (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            যদি (p == রিক্ত) {
                প্রত্যাগম NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } অন্যথা {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        যদি (p == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        যদি (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            প্রত্যাগম NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#অন্যথা

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#অন্যথা
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    যদি (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } অন্যথা {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    যদি (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        প্রত্যাগম NGX_ERROR;
    }

    প্রতি (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        যদি (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            ভঙ্গ;
        }
    }

    যদি (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } অন্যথা {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    যদি (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    যদি (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    প্রত্যাগম NGX_OK;
}


স্থির শূন্য *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, পরিমাণ(ngx_core_conf_t));
    যদি (ccf == রিক্ত) {
        প্রত্যাগম রিক্ত;
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

    যদি (ngx_array_init(&ccf->env, cycle->pool, 1, পরিমাণ(ngx_str_t))
        != NGX_OK)
    {
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম ccf;
}


স্থির বর্ণ *
ngx_core_module_init_conf(ngx_cycle_t *cycle, শূন্য *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_মান(ccf->daemon, 1);
    ngx_conf_init_মান(ccf->master, 1);
    ngx_conf_init_msec_মান(ccf->timer_resolution, 0);
    ngx_conf_init_msec_মান(ccf->shutdown_timeout, 0);

    ngx_conf_init_মান(ccf->worker_processes, 1);
    ngx_conf_init_মান(ccf->debug_points, 0);

#যদি (NGX_HAVE_CPU_AFFINITY)

    যদি (!ccf->cpu_affinity_auto
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


    যদি (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    যদি (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + পরিমাণ(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    যদি (ccf->oldpid.data == রিক্ত) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, পরিমাণ(NGX_OLDPID_EXT));


    যদি (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        যদি (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            প্রত্যাগম NGX_CONF_ERROR;
        }

        যদি (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            প্রত্যাগম NGX_CONF_ERROR;
        }

        যদি (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            প্রত্যাগম NGX_CONF_ERROR;
        }
    }


#যদি !(NGX_WIN32)

    যদি (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        সংরচনা group   *grp;
        সংরচনা passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        যদি (pwd == রিক্ত) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            প্রত্যাগম NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        যদি (grp == রিক্ত) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            প্রত্যাগম NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    যদি (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    যদি (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    যদি (lock_file.len) {
        lock_file.len--;

        যদি (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += পরিমাণ(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        যদি (cycle->lock_file.data == রিক্ত) {
            প্রত্যাগম NGX_CONF_ERROR;
        }

    } অন্যথা {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + পরিমাণ(".accept"));
        যদি (cycle->lock_file.data == রিক্ত) {
            প্রত্যাগম NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", পরিমাণ(".accept"));
    }
    }

#endif

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
#যদি (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    প্রত্যাগম NGX_CONF_OK;

#অন্যথা

    ngx_core_conf_t  *ccf = conf;

    বর্ণ             *group;
    সংরচনা passwd    *pwd;
    সংরচনা group     *grp;
    ngx_str_t        *মান;

    যদি (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        প্রত্যাগম "is duplicate";
    }

    যদি (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        প্রত্যাগম NGX_CONF_OK;
    }

    মান = cf->args->elts;

    ccf->username = (বর্ণ *) মান[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ধ্রুব বর্ণ *) মান[1].data);
    যদি (pwd == রিক্ত) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", মান[1].data);
        প্রত্যাগম NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (বর্ণ *) ((cf->args->nelts == 2) ? মান[1].data : মান[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    যদি (grp == রিক্ত) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        প্রত্যাগম NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    প্রত্যাগম NGX_CONF_OK;

#endif
}


স্থির বর্ণ *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *মান, *var;
    ngx_uint_t   i;

    var = ngx_array_ঠেলো(&ccf->env);
    যদি (var == রিক্ত) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    মান = cf->args->elts;
    *var = মান[1];

    প্রতি (i = 0; i < মান[1].len; i++) {

        যদি (মান[1].data[i] == '=') {

            var->len = i;

            প্রত্যাগম NGX_CONF_OK;
        }
    }

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *মান;
    ngx_uint_t        n, minus;

    যদি (ccf->priority != 0) {
        প্রত্যাগম "is duplicate";
    }

    মান = cf->args->elts;

    যদি (মান[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } অন্যথা যদি (মান[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } অন্যথা {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&মান[1].data[n], মান[1].len - n);
    যদি (ccf->priority == NGX_ERROR) {
        প্রত্যাগম "invalid number";
    }

    যদি (minus) {
        ccf->priority = -ccf->priority;
    }

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
#যদি (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *মান;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    যদি (ccf->cpu_affinity) {
        প্রত্যাগম "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * পরিমাণ(ngx_cpuset_t));
    যদি (mask == রিক্ত) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    মান = cf->args->elts;

    যদি (ngx_strcmp(মান[1].data, "auto") == 0) {

        যদি (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            প্রত্যাগম NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        প্রতি (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } অন্যথা {
        n = 1;
    }

    প্রতি ( /* void */ ; n < cf->args->nelts; n++) {

        যদি (মান[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            প্রত্যাগম NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        প্রতি (p = মান[n].data + মান[n].len - 1;
             p >= মান[n].data;
             p--)
        {
            ch = *p;

            যদি (ch == ' ') {
                অনুবর্ত;
            }

            i++;

            যদি (ch == '0') {
                অনুবর্ত;
            }

            যদি (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                অনুবর্ত;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            প্রত্যাগম NGX_CONF_ERROR;
        }
    }

#অন্যথা

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    প্রত্যাগম NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#যদি (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    স্থির ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    যদি (ccf->cpu_affinity == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    যদি (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        প্রতি (i = 0, j = n; /* void */ ; i++) {

            যদি (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                ভঙ্গ;
            }

            যদি (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                প্রত্যাগম রিক্ত;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        প্রত্যাগম &result;
    }

    যদি (ccf->cpu_affinity_n > n) {
        প্রত্যাগম &ccf->cpu_affinity[n];
    }

    প্রত্যাগম &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#অন্যথা

    প্রত্যাগম রিক্ত;

#endif
}


স্থির বর্ণ *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_str_t        *মান;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    যদি (ccf->worker_processes != NGX_CONF_UNSET) {
        প্রত্যাগম "is duplicate";
    }

    মান = cf->args->elts;

    যদি (ngx_strcmp(মান[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        প্রত্যাগম NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(মান[1].data, মান[1].len);

    যদি (ccf->worker_processes == NGX_ERROR) {
        প্রত্যাগম "invalid value";
    }

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
#যদি (NGX_HAVE_DLOPEN)
    শূন্য                *handle;
    বর্ণ               **names, **order;
    ngx_str_t           *মান, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    যদি (cf->cycle->modules_used) {
        প্রত্যাগম "is specified too late";
    }

    মান = cf->args->elts;

    file = মান[1];

    যদি (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    যদি (cln == রিক্ত) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    যদি (handle == রিক্ত) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        প্রত্যাগম NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    যদি (modules == রিক্ত) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &মান[1], "ngx_modules", ngx_dlerror());
        প্রত্যাগম NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    যদি (names == রিক্ত) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &মান[1], "ngx_module_names", ngx_dlerror());
        প্রত্যাগম NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    প্রতি (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        যদি (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            প্রত্যাগম NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    প্রত্যাগম NGX_CONF_OK;

#অন্যথা

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    প্রত্যাগম NGX_CONF_ERROR;

#endif
}


#যদি (NGX_HAVE_DLOPEN)

স্থির শূন্য
ngx_unload_module(শূন্য *data)
{
    শূন্য  *handle = data;

    যদি (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
