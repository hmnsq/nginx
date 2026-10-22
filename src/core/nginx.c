
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <nginx.h>


స్థిర శూన్య ngx_show_సంస్కరణ_info(శూన్య);
స్థిర ngx_int_t ngx_కూడు_inherited_sockets(ngx_చక్రం_t *చక్రం);
స్థిర శూన్య ngx_cleanup_environment(శూన్య *దత్తాంశం);
స్థిర శూన్య ngx_cleanup_environment_variable(శూన్య *దత్తాంశం);
స్థిర ngx_int_t ngx_get_options(పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv);
స్థిర ngx_int_t ngx_process_options(ngx_చక్రం_t *చక్రం);
స్థిర ngx_int_t ngx_save_argv(ngx_చక్రం_t *చక్రం, పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv);
స్థిర శూన్య *ngx_core_module_create_conf(ngx_చక్రం_t *చక్రం);
స్థిర అక్షరం *ngx_core_module_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf);
స్థిర అక్షరం *ngx_సమితి_user(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
స్థిర అక్షరం *ngx_సమితి_env(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
స్థిర అక్షరం *ngx_సమితి_ప్రాధాన్యత(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
స్థిర అక్షరం *ngx_సమితి_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    శూన్య *conf);
స్థిర అక్షరం *ngx_సమితి_కార్మికుడు_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    శూన్య *conf);
స్థిర అక్షరం *ngx_ఎక్కించు_module(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
#ఒకవేళ (NGX_HAVE_DLOPEN)
స్థిర శూన్య ngx_unload_module(శూన్య *దత్తాంశం);
#అయితే_ముగింపు


స్థిర ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


స్థిర ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_FLAG,
      ngx_conf_సమితి_flag_స్థానం,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ఖాళీ },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_FLAG,
      ngx_conf_సమితి_flag_స్థానం,
      0,
      offsetof(ngx_core_conf_t, master),
      ఖాళీ },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_msec_స్థానం,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ఖాళీ },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_str_స్థానం,
      0,
      offsetof(ngx_core_conf_t, pid),
      ఖాళీ },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_str_స్థానం,
      0,
      offsetof(ngx_core_conf_t, తాళం_file),
      ఖాళీ },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_సమితి_కార్మికుడు_processes,
      0,
      0,
      ఖాళీ },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_enum_స్థానం,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE12,
      ngx_సమితి_user,
      0,
      0,
      ఖాళీ },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_సమితి_ప్రాధాన్యత,
      0,
      0,
      ఖాళీ },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_1_మరిన్ని,
      ngx_సమితి_cpu_affinity,
      0,
      0,
      ఖాళీ },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_అంకె_స్థానం,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ఖాళీ },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_off_స్థానం,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ఖాళీ },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_msec_స్థానం,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ఖాళీ },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_conf_సమితి_str_స్థానం,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ఖాళీ },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_సమితి_env,
      0,
      0,
      ఖాళీ },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_TAKE1,
      ngx_ఎక్కించు_module,
      0,
      0,
      ఖాళీ },

      ngx_null_command
};


స్థిర ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ఖాళీ,                                  /* init master */
    ఖాళీ,                                  /* init module */
    ఖాళీ,                                  /* init process */
    ఖాళీ,                                  /* init thread */
    ఖాళీ,                                  /* exit thread */
    ఖాళీ,                                  /* exit process */
    ఖాళీ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


స్థిర ngx_uint_t   ngx_show_help;
స్థిర ngx_uint_t   ngx_show_సంస్కరణ;
స్థిర ngx_uint_t   ngx_show_configure;
స్థిర u_char      *ngx_ఉపసర్గ;
స్థిర u_char      *ngx_error_log;
స్థిర u_char      *ngx_conf_file;
స్థిర u_char      *ngx_conf_params;
#ఒకవేళ (NGX_CONTROL_API)
స్థిర u_char      *ngx_control_addr;
#అయితే_ముగింపు
స్థిర అక్షరం        *ngx_signal;


స్థిర అక్షరం **ngx_os_environ;


పూర్ణాంకం ngx_cdecl
ప్రధాన(పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *సంవర్గమానం;
    ngx_uint_t        i;
    ngx_చక్రం_t      *చక్రం, init_చక్రం;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    ఒకవేళ (ngx_strerror_init() != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_get_options(argc, argv) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_show_సంస్కరణ) {
        ngx_show_సంస్కరణ_info();

        ఒకవేళ (!ngx_test_config) {
            తిరిగి 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_సమయం_init();

#ఒకవేళ (NGX_PCRE)
    ngx_regex_init();
#అయితే_ముగింపు

    ngx_pid = ngx_getpid();
    ngx_మాతృ = ngx_getppid();

    సంవర్గమానం = ngx_log_init(ngx_ఉపసర్గ, ngx_error_log);
    ఒకవేళ (సంవర్గమానం == ఖాళీ) {
        తిరిగి 1;
    }

    /* STUB */
#ఒకవేళ (NGX_OPENSSL)
    ngx_ssl_init(సంవర్గమానం);
#అయితే_ముగింపు

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_చక్రం, పరిమాణం(ngx_చక్రం_t));
    init_చక్రం.సంవర్గమానం = సంవర్గమానం;
    ngx_చక్రం = &init_చక్రం;

    init_చక్రం.pool = ngx_create_pool(1024, సంవర్గమానం);
    ఒకవేళ (init_చక్రం.pool == ఖాళీ) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_save_argv(&init_చక్రం, argc, argv) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_process_options(&init_చక్రం) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_os_init(సంవర్గమానం) != NGX_OK) {
        తిరిగి 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    ఒకవేళ (ngx_చక్రీయ_తనిఖీ_32_పట్టిక_init() != NGX_OK) {
        తిరిగి 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    ఒకవేళ (ngx_కూడు_inherited_sockets(&init_చక్రం) != NGX_OK) {
        తిరిగి 1;
    }

#ఒకవేళ (NGX_CONTROL_API)
    ngx_control_preinit();
#అయితే_ముగింపు

    ఒకవేళ (ngx_preinit_modules() != NGX_OK) {
        తిరిగి 1;
    }

    చక్రం = ngx_init_చక్రం(&init_చక్రం);
    ఒకవేళ (చక్రం == ఖాళీ) {
        ఒకవేళ (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_చక్రం.conf_file.దత్తాంశం);
        }

        తిరిగి 1;
    }

    ఒకవేళ (ngx_test_config) {
        ఒకవేళ (!ngx_quiet_బహుళకం) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           చక్రం->conf_file.దత్తాంశం);
        }

        ఒకవేళ (ngx_dump_config) {
            cd = చక్రం->config_dump.elts;

            కొరకు (i = 0; i < చక్రం->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (శూన్య) ngx_write_fd(ngx_stdout, cd[i].name.దత్తాంశం,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].బఫరు;

                (శూన్య) ngx_write_fd(ngx_stdout, b->pos, b->చివరి - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        తిరిగి 0;
    }

    ఒకవేళ (ngx_signal) {
        తిరిగి ngx_signal_process(చక్రం, ngx_signal);
    }

    ngx_os_స్థితి(చక్రం->సంవర్గమానం);

    ngx_చక్రం = చక్రం;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ఒకవేళ (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#ఒకవేళ !(NGX_WIN32)

    ఒకవేళ (ngx_init_సంకేతాలు(చక్రం->సంవర్గమానం) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (!ngx_inherited && ccf->daemon) {
        ఒకవేళ (ngx_daemon(చక్రం->సంవర్గమానం) != NGX_OK) {
            తిరిగి 1;
        }

        ngx_daemonized = 1;
    }

    ఒకవేళ (ngx_inherited) {
        ngx_daemonized = 1;
    }

#అయితే_ముగింపు

#ఒకవేళ (NGX_CONTROL_API)
    ఒకవేళ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, చక్రం->సంవర్గమానం, 0,
                      "control API unavailable in single process mode");

    } లేకపోతే {
        ఒకవేళ (ngx_control_init(ngx_control_addr) != NGX_OK) {
            తిరిగి 1;
        }
    }
#అయితే_ముగింపు

    ఒకవేళ (ngx_create_pidfile(&ccf->pid, చక్రం->సంవర్గమానం) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (ngx_log_redirect_stderr(చక్రం) != NGX_OK) {
        తిరిగి 1;
    }

    ఒకవేళ (సంవర్గమానం->file->fd != ngx_stderr) {
        ఒకవేళ (ngx_close_file(సంవర్గమానం->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    ఒకవేళ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_చక్రం(చక్రం);

    } లేకపోతే {
        ngx_master_process_చక్రం(చక్రం);
    }

    తిరిగి 0;
}


స్థిర శూన్య
ngx_show_సంస్కరణ_info(శూన్య)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    ఒకవేళ (ngx_show_help) {
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
#ఒకవేళ (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#అయితే_ముగింపు
#నిర్వచితమైతే NGX_ఉపసర్గ
            "  -p prefix     : set prefix path (default: " NGX_ఉపసర్గ ")"
                               NGX_LINEFEED
#లేకపోతే
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#అయితే_ముగింపు
            "  -e filename   : set error log file (default: "
#నిర్వచితమైతే NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#లేకపోతే
                               NGX_ERROR_LOG_మార్గం ")" NGX_LINEFEED
#అయితే_ముగింపు
            "  -c filename   : set configuration file (default: " NGX_CONF_మార్గం
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    ఒకవేళ (ngx_show_configure) {

#నిర్వచితమైతే NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#అయితే_ముగింపు

#ఒకవేళ (NGX_SSL)
        ఒకవేళ (ngx_strcmp(ngx_ssl_సంస్కరణ(), OPENSSL_సంస్కరణ_పాఠ్యం) == 0) {
            ngx_write_stderr("built with " OPENSSL_సంస్కరణ_పాఠ్యం NGX_LINEFEED);
        } లేకపోతే {
            ngx_write_stderr("built with " OPENSSL_సంస్కరణ_పాఠ్యం
                             " (running with ");
            ngx_write_stderr((అక్షరం *) (uintptr_t) ngx_ssl_సంస్కరణ());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#నిర్వచితమైతే SSL_CTRL_సమితి_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#లేకపోతే
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#అయితే_ముగింపు
#అయితే_ముగింపు

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


స్థిర ngx_int_t
ngx_కూడు_inherited_sockets(ngx_చక్రం_t *చక్రం)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    ఒకవేళ (inherited == ఖాళీ) {
        తిరిగి NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, చక్రం->సంవర్గమానం, 0,
                  "using inherited sockets from \"%s\"", inherited);

    ఒకవేళ (ngx_array_init(&చక్రం->listening, చక్రం->pool, 10,
                       పరిమాణం(ngx_listening_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    కొరకు (p = inherited, v = p; *p; p++) {
        ఒకవేళ (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            ఒకవేళ (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                ఆపు;
            }

            v = p + 1;

            ls = ngx_array_నెట్టు(&చక్రం->listening);
            ఒకవేళ (ls == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            ngx_memzero(ls, పరిమాణం(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    ఒకవేళ (v != p) {
        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    తిరిగి ngx_సమితి_inherited_sockets(చక్రం);
}


అక్షరం **
ngx_సమితి_environment(ngx_చక్రం_t *చక్రం, ngx_uint_t *చివరి)
{
    అక్షరం                **p, **env, *str;
    కొలత_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ఒకవేళ (చివరి == ఖాళీ && ccf->environment) {
        తిరిగి ccf->environment;
    }

    var = ccf->env.elts;

    కొరకు (i = 0; i < ccf->env.nelts; i++) {
        ఒకవేళ (ngx_strcmp(var[i].దత్తాంశం, "TZ") == 0
            || ngx_strncmp(var[i].దత్తాంశం, "TZ=", 3) == 0)
        {
            వెళ్ళు tz_దొరికింది;
        }
    }

    var = ngx_array_నెట్టు(&ccf->env);
    ఒకవేళ (var == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    var->len = 2;
    var->దత్తాంశం = (u_char *) "TZ";

    var = ccf->env.elts;

tz_దొరికింది:

    n = 0;

    కొరకు (i = 0; i < ccf->env.nelts; i++) {

        ఒకవేళ (var[i].దత్తాంశం[var[i].len] == '=') {
            n++;
            కొనసాగు;
        }

        కొరకు (p = ngx_os_environ; *p; p++) {

            ఒకవేళ (ngx_strncmp(*p, var[i].దత్తాంశం, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                ఆపు;
            }
        }
    }

    ఒకవేళ (చివరి) {
        env = ngx_alloc((*చివరి + n + 1) * పరిమాణం(అక్షరం *), చక్రం->సంవర్గమానం);
        ఒకవేళ (env == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        *చివరి = n;

    } లేకపోతే {
        cln = ngx_pool_cleanup_కూడు(చక్రం->pool, 0);
        ఒకవేళ (cln == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        env = ngx_alloc((n + 1) * పరిమాణం(అక్షరం *), చక్రం->సంవర్గమానం);
        ఒకవేళ (env == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        cln->handler = ngx_cleanup_environment;
        cln->దత్తాంశం = env;
    }

    n = 0;

    కొరకు (i = 0; i < ccf->env.nelts; i++) {

        ఒకవేళ (var[i].దత్తాంశం[var[i].len] == '=') {

            ఒకవేళ (చివరి) {
                env[n++] = (అక్షరం *) var[i].దత్తాంశం;
                కొనసాగు;
            }

            cln = ngx_pool_cleanup_కూడు(చక్రం->pool, 0);
            ఒకవేళ (cln == ఖాళీ) {
                తిరిగి ఖాళీ;
            }

            len = ngx_strlen(var[i].దత్తాంశం) + 1;

            str = ngx_alloc(len, చక్రం->సంవర్గమానం);
            ఒకవేళ (str == ఖాళీ) {
                తిరిగి ఖాళీ;
            }

            ngx_memcpy(str, var[i].దత్తాంశం, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->దత్తాంశం = str;

            env[n++] = str;

            కొనసాగు;
        }

        కొరకు (p = ngx_os_environ; *p; p++) {

            ఒకవేళ (ngx_strncmp(*p, var[i].దత్తాంశం, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                ఆపు;
            }
        }
    }

    env[n] = ఖాళీ;

    ఒకవేళ (చివరి == ఖాళీ) {
        ccf->environment = env;
        environ = env;
    }

    తిరిగి env;
}


స్థిర శూన్య
ngx_cleanup_environment(శూన్య *దత్తాంశం)
{
    అక్షరం  **env = దత్తాంశం;

    ఒకవేళ (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        తిరిగి;
    }

    ngx_free(env);
}


స్థిర శూన్య
ngx_cleanup_environment_variable(శూన్య *దత్తాంశం)
{
    అక్షరం  *var = దత్తాంశం;

    అక్షరం  **p;

    కొరకు (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        ఒకవేళ (*p == var) {
            తిరిగి;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ద్విమాన(ngx_చక్రం_t *చక్రం, అక్షరం *స్థిరరాశి *argv)
{
    అక్షరం             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, పరిమాణం(ngx_exec_ctx_t));

    ctx.మార్గం = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_సమితి_environment(చక్రం, &n);
    ఒకవేళ (env == ఖాళీ) {
        తిరిగి NGX_INVALID_PID;
    }

    var = ngx_alloc(పరిమాణం(NGINX_VAR)
                    + చక్రం->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    చక్రం->సంవర్గమానం);
    ఒకవేళ (var == ఖాళీ) {
        ngx_free(env);
        తిరిగి NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", పరిమాణం(NGINX_VAR));

    ls = చక్రం->listening.elts;
    కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {
        ఒకవేళ (ls[i].ignore) {
            కొనసాగు;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#ఒకవేళ (NGX_SETPROCTITLE_ఉపయోగిస్తుంది_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#అయితే_ముగింపు

#ఒకవేళ (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    ఒకవేళ (env[n] != ఖాళీ) {
        n++;
    }
#అయితే_ముగింపు

    env[n] = ఖాళీ;

#ఒకవేళ (NGX_DEBUG)
    {
    అక్షరం  **e;
    కొరకు (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, చక్రం->సంవర్గమానం, 0, "env: %s", *e);
    }
    }
#అయితే_ముగింపు

    ctx.envp = (అక్షరం *స్థిరరాశి *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ఒకవేళ (ngx_rename_file(ccf->pid.దత్తాంశం, ccf->oldpid.దత్తాంశం) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.దత్తాంశం, ccf->oldpid.దత్తాంశం, argv[0]);

        ngx_free(env);
        ngx_free(var);

        తిరిగి NGX_INVALID_PID;
    }

    pid = ngx_execute(చక్రం, &ctx);

    ఒకవేళ (pid == NGX_INVALID_PID) {
        ఒకవేళ (ngx_rename_file(ccf->oldpid.దత్తాంశం, ccf->pid.దత్తాంశం)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.దత్తాంశం, ccf->pid.దత్తాంశం, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    తిరిగి pid;
}


స్థిర ngx_int_t
ngx_get_options(పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv)
{
    u_char     *p;
    ngx_int_t   i;

    కొరకు (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        ఒకవేళ (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            తిరిగి NGX_ERROR;
        }

        అయితే (*p) {

            ఎంపిక (*p++) {

            స్థితి '?':
            స్థితి 'h':
                ngx_show_సంస్కరణ = 1;
                ngx_show_help = 1;
                ఆపు;

            స్థితి 'v':
                ngx_show_సంస్కరణ = 1;
                ఆపు;

            స్థితి 'V':
                ngx_show_సంస్కరణ = 1;
                ngx_show_configure = 1;
                ఆపు;

            స్థితి 't':
                ngx_test_config = 1;
                ఆపు;

            స్థితి 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                ఆపు;

            స్థితి 'q':
                ngx_quiet_బహుళకం = 1;
                ఆపు;

            స్థితి 'p':
                ఒకవేళ (*p) {
                    ngx_ఉపసర్గ = p;
                    వెళ్ళు తదుపరి;
                }

                ఒకవేళ (argv[++i]) {
                    ngx_ఉపసర్గ = (u_char *) argv[i];
                    వెళ్ళు తదుపరి;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                తిరిగి NGX_ERROR;

            స్థితి 'e':
                ఒకవేళ (*p) {
                    ngx_error_log = p;

                } లేకపోతే ఒకవేళ (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } లేకపోతే {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                వెళ్ళు తదుపరి;

            స్థితి 'c':
                ఒకవేళ (*p) {
                    ngx_conf_file = p;
                    వెళ్ళు తదుపరి;
                }

                ఒకవేళ (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    వెళ్ళు తదుపరి;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                తిరిగి NGX_ERROR;

            స్థితి 'g':
                ఒకవేళ (*p) {
                    ngx_conf_params = p;
                    వెళ్ళు తదుపరి;
                }

                ఒకవేళ (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    వెళ్ళు తదుపరి;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                తిరిగి NGX_ERROR;

            స్థితి 's':
                ఒకవేళ (*p) {
                    ngx_signal = (అక్షరం *) p;

                } లేకపోతే ఒకవేళ (argv[++i]) {
                    ngx_signal = argv[i];

                } లేకపోతే {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    వెళ్ళు తదుపరి;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                తిరిగి NGX_ERROR;

#ఒకవేళ (NGX_CONTROL_API)
            స్థితి 'l':
                ngx_control_api_enabled = 1;

                ఒకవేళ (*p) {
                    ngx_control_addr = p;
                    వెళ్ళు తదుపరి;
                }

                ఒకవేళ (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    వెళ్ళు తదుపరి;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                తిరిగి NGX_ERROR;
#అయితే_ముగింపు

            అప్రమేయ:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                తిరిగి NGX_ERROR;
            }
        }

    తదుపరి:

        కొనసాగు;
    }

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_save_argv(ngx_చక్రం_t *చక్రం, పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv)
{
#ఒకవేళ (NGX_FREEBSD)

    ngx_os_argv = (అక్షరం **) argv;
    ngx_argc = argc;
    ngx_argv = (అక్షరం **) argv;

#లేకపోతే
    కొలత_t     len;
    ngx_int_t  i;

    ngx_os_argv = (అక్షరం **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * పరిమాణం(అక్షరం *), చక్రం->సంవర్గమానం);
    ఒకవేళ (ngx_argv == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    కొరకు (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, చక్రం->సంవర్గమానం);
        ఒకవేళ (ngx_argv[i] == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        (శూన్య) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ఖాళీ;

#అయితే_ముగింపు

    ngx_os_environ = environ;

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_process_options(ngx_చక్రం_t *చక్రం)
{
    u_char  *p;
    కొలత_t   len;

    ఒకవేళ (ngx_ఉపసర్గ) {
        len = ngx_strlen(ngx_ఉపసర్గ);
        p = ngx_ఉపసర్గ;

        ఒకవేళ (len && !ngx_మార్గం_separator(p[len - 1])) {
            p = ngx_pnalloc(చక్రం->pool, len + 1);
            ఒకవేళ (p == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            ngx_memcpy(p, ngx_ఉపసర్గ, len);
            p[len++] = '/';
        }

        చక్రం->conf_ఉపసర్గ.len = len;
        చక్రం->conf_ఉపసర్గ.దత్తాంశం = p;
        చక్రం->ఉపసర్గ.len = len;
        చక్రం->ఉపసర్గ.దత్తాంశం = p;

    } లేకపోతే {

#నిర్వచితం_కాకపోతే NGX_ఉపసర్గ

        p = ngx_pnalloc(చక్రం->pool, NGX_MAX_మార్గం);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (ngx_getcwd(p, NGX_MAX_మార్గం) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            తిరిగి NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        చక్రం->conf_ఉపసర్గ.len = len;
        చక్రం->conf_ఉపసర్గ.దత్తాంశం = p;
        చక్రం->ఉపసర్గ.len = len;
        చక్రం->ఉపసర్గ.దత్తాంశం = p;

#లేకపోతే

#నిర్వచితమైతే NGX_CONF_ఉపసర్గ
        ngx_str_సమితి(&చక్రం->conf_ఉపసర్గ, NGX_CONF_ఉపసర్గ);
#లేకపోతే
        ngx_str_సమితి(&చక్రం->conf_ఉపసర్గ, NGX_ఉపసర్గ);
#అయితే_ముగింపు
        ngx_str_సమితి(&చక్రం->ఉపసర్గ, NGX_ఉపసర్గ);

#అయితే_ముగింపు
    }

    ఒకవేళ (ngx_conf_file) {
        చక్రం->conf_file.len = ngx_strlen(ngx_conf_file);
        చక్రం->conf_file.దత్తాంశం = ngx_conf_file;

    } లేకపోతే {
        ngx_str_సమితి(&చక్రం->conf_file, NGX_CONF_మార్గం);
    }

    ఒకవేళ (ngx_conf_నిండిన_name(చక్రం, &చక్రం->conf_file, 0) != NGX_OK) {
        తిరిగి NGX_ERROR;
    }

    కొరకు (p = చక్రం->conf_file.దత్తాంశం + చక్రం->conf_file.len - 1;
         p > చక్రం->conf_file.దత్తాంశం;
         p--)
    {
        ఒకవేళ (ngx_మార్గం_separator(*p)) {
            చక్రం->conf_ఉపసర్గ.len = p - చక్రం->conf_file.దత్తాంశం + 1;
            చక్రం->conf_ఉపసర్గ.దత్తాంశం = చక్రం->conf_file.దత్తాంశం;
            ఆపు;
        }
    }

    ఒకవేళ (ngx_error_log) {
        చక్రం->error_log.len = ngx_strlen(ngx_error_log);
        చక్రం->error_log.దత్తాంశం = ngx_error_log;

    } లేకపోతే {
        ngx_str_సమితి(&చక్రం->error_log, NGX_ERROR_LOG_మార్గం);
    }

    ఒకవేళ (ngx_conf_params) {
        చక్రం->conf_param.len = ngx_strlen(ngx_conf_params);
        చక్రం->conf_param.దత్తాంశం = ngx_conf_params;
    }

    ఒకవేళ (ngx_test_config) {
        చక్రం->సంవర్గమానం->log_level = NGX_LOG_INFO;
    }

    తిరిగి NGX_OK;
}


స్థిర శూన్య *
ngx_core_module_create_conf(ngx_చక్రం_t *చక్రం)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(చక్రం->pool, పరిమాణం(ngx_core_conf_t));
    ఒకవేళ (ccf == ఖాళీ) {
        తిరిగి ఖాళీ;
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

    ccf->కార్మికుడు_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->సమూహం = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    ఒకవేళ (ngx_array_init(&ccf->env, చక్రం->pool, 1, పరిమాణం(ngx_str_t))
        != NGX_OK)
    {
        తిరిగి ఖాళీ;
    }

    తిరిగి ccf;
}


స్థిర అక్షరం *
ngx_core_module_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_విలువ(ccf->daemon, 1);
    ngx_conf_init_విలువ(ccf->master, 1);
    ngx_conf_init_msec_విలువ(ccf->timer_resolution, 0);
    ngx_conf_init_msec_విలువ(ccf->shutdown_timeout, 0);

    ngx_conf_init_విలువ(ccf->కార్మికుడు_processes, 1);
    ngx_conf_init_విలువ(ccf->debug_points, 0);

#ఒకవేళ (NGX_HAVE_CPU_AFFINITY)

    ఒకవేళ (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->కార్మికుడు_processes)
    {
        ngx_log_error(NGX_LOG_WARN, చక్రం->సంవర్గమానం, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#అయితే_ముగింపు


    ఒకవేళ (ccf->pid.len == 0) {
        ngx_str_సమితి(&ccf->pid, NGX_PID_మార్గం);
    }

    ఒకవేళ (ngx_conf_నిండిన_name(చక్రం, &ccf->pid, 0) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + పరిమాణం(NGX_OLDPID_EXT);

    ccf->oldpid.దత్తాంశం = ngx_pnalloc(చక్రం->pool, ccf->oldpid.len);
    ఒకవేళ (ccf->oldpid.దత్తాంశం == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.దత్తాంశం, ccf->pid.దత్తాంశం, ccf->pid.len),
               NGX_OLDPID_EXT, పరిమాణం(NGX_OLDPID_EXT));


    ఒకవేళ (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        ఒకవేళ (ngx_conf_నిండిన_name(చక్రం, &ccf->working_directory, 0) != NGX_OK) {
            తిరిగి NGX_CONF_ERROR;
        }

        ఒకవేళ (ngx_file_info(ccf->working_directory.దత్తాంశం, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.దత్తాంశం);
            తిరిగి NGX_CONF_ERROR;
        }

        ఒకవేళ (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.దత్తాంశం);
            తిరిగి NGX_CONF_ERROR;
        }
    }


#ఒకవేళ !(NGX_WIN32)

    ఒకవేళ (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        నిర్మాణం సమూహం   *grp;
        నిర్మాణం passwd  *pwd;

        ngx_సమితి_errno(0);
        pwd = getpwnam(NGX_USER);
        ఒకవేళ (pwd == ఖాళీ) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            తిరిగి NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_సమితి_errno(0);
        grp = getgrnam(NGX_సమూహం);
        ఒకవేళ (grp == ఖాళీ) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          "getgrnam(\"" NGX_సమూహం "\") failed");
            తిరిగి NGX_CONF_ERROR;
        }

        ccf->సమూహం = grp->gr_gid;
    }


    ఒకవేళ (ccf->తాళం_file.len == 0) {
        ngx_str_సమితి(&ccf->తాళం_file, NGX_తాళం_మార్గం);
    }

    ఒకవేళ (ngx_conf_నిండిన_name(చక్రం, &ccf->తాళం_file, 0) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    {
    ngx_str_t  తాళం_file;

    తాళం_file = చక్రం->old_చక్రం->తాళం_file;

    ఒకవేళ (తాళం_file.len) {
        తాళం_file.len--;

        ఒకవేళ (ccf->తాళం_file.len != తాళం_file.len
            || ngx_strncmp(ccf->తాళం_file.దత్తాంశం, తాళం_file.దత్తాంశం, తాళం_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        చక్రం->తాళం_file.len = తాళం_file.len + 1;
        తాళం_file.len += పరిమాణం(".accept");

        చక్రం->తాళం_file.దత్తాంశం = ngx_pstrdup(చక్రం->pool, &తాళం_file);
        ఒకవేళ (చక్రం->తాళం_file.దత్తాంశం == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

    } లేకపోతే {
        చక్రం->తాళం_file.len = ccf->తాళం_file.len + 1;
        చక్రం->తాళం_file.దత్తాంశం = ngx_pnalloc(చక్రం->pool,
                                      ccf->తాళం_file.len + పరిమాణం(".accept"));
        ఒకవేళ (చక్రం->తాళం_file.దత్తాంశం == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(చక్రం->తాళం_file.దత్తాంశం, ccf->తాళం_file.దత్తాంశం,
                              ccf->తాళం_file.len),
                   ".accept", పరిమాణం(".accept"));
    }
    }

#అయితే_ముగింపు

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_సమితి_user(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
#ఒకవేళ (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    తిరిగి NGX_CONF_OK;

#లేకపోతే

    ngx_core_conf_t  *ccf = conf;

    అక్షరం             *సమూహం;
    నిర్మాణం passwd    *pwd;
    నిర్మాణం సమూహం     *grp;
    ngx_str_t        *విలువ;

    ఒకవేళ (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        తిరిగి "is duplicate";
    }

    ఒకవేళ (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        తిరిగి NGX_CONF_OK;
    }

    విలువ = cf->వాదనలు->elts;

    ccf->username = (అక్షరం *) విలువ[1].దత్తాంశం;

    ngx_సమితి_errno(0);
    pwd = getpwnam((స్థిరరాశి అక్షరం *) విలువ[1].దత్తాంశం);
    ఒకవేళ (pwd == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", విలువ[1].దత్తాంశం);
        తిరిగి NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    సమూహం = (అక్షరం *) ((cf->వాదనలు->nelts == 2) ? విలువ[1].దత్తాంశం : విలువ[2].దత్తాంశం);

    ngx_సమితి_errno(0);
    grp = getgrnam(సమూహం);
    ఒకవేళ (grp == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", సమూహం);
        తిరిగి NGX_CONF_ERROR;
    }

    ccf->సమూహం = grp->gr_gid;

    తిరిగి NGX_CONF_OK;

#అయితే_ముగింపు
}


స్థిర అక్షరం *
ngx_సమితి_env(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *విలువ, *var;
    ngx_uint_t   i;

    var = ngx_array_నెట్టు(&ccf->env);
    ఒకవేళ (var == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    విలువ = cf->వాదనలు->elts;
    *var = విలువ[1];

    కొరకు (i = 0; i < విలువ[1].len; i++) {

        ఒకవేళ (విలువ[1].దత్తాంశం[i] == '=') {

            var->len = i;

            తిరిగి NGX_CONF_OK;
        }
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_సమితి_ప్రాధాన్యత(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *విలువ;
    ngx_uint_t        n, minus;

    ఒకవేళ (ccf->ప్రాధాన్యత != 0) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    ఒకవేళ (విలువ[1].దత్తాంశం[0] == '-') {
        n = 1;
        minus = 1;

    } లేకపోతే ఒకవేళ (విలువ[1].దత్తాంశం[0] == '+') {
        n = 1;
        minus = 0;

    } లేకపోతే {
        n = 0;
        minus = 0;
    }

    ccf->ప్రాధాన్యత = ngx_atoi(&విలువ[1].దత్తాంశం[n], విలువ[1].len - n);
    ఒకవేళ (ccf->ప్రాధాన్యత == NGX_ERROR) {
        తిరిగి "invalid number";
    }

    ఒకవేళ (minus) {
        ccf->ప్రాధాన్యత = -ccf->ప్రాధాన్యత;
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_సమితి_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
#ఒకవేళ (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *విలువ;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *ముసుగు;

    ఒకవేళ (ccf->cpu_affinity) {
        తిరిగి "is duplicate";
    }

    ముసుగు = ngx_palloc(cf->pool, (cf->వాదనలు->nelts - 1) * పరిమాణం(ngx_cpuset_t));
    ఒకవేళ (ముసుగు == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->వాదనలు->nelts - 1;
    ccf->cpu_affinity = ముసుగు;

    విలువ = cf->వాదనలు->elts;

    ఒకవేళ (ngx_strcmp(విలువ[1].దత్తాంశం, "auto") == 0) {

        ఒకవేళ (cf->వాదనలు->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            తిరిగి NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&ముసుగు[0]);
        కొరకు (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_సమితి(i, &ముసుగు[0]);
        }

        n = 2;

    } లేకపోతే {
        n = 1;
    }

    కొరకు ( /* void */ ; n < cf->వాదనలు->nelts; n++) {

        ఒకవేళ (విలువ[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            తిరిగి NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&ముసుగు[n - 1]);

        కొరకు (p = విలువ[n].దత్తాంశం + విలువ[n].len - 1;
             p >= విలువ[n].దత్తాంశం;
             p--)
        {
            ch = *p;

            ఒకవేళ (ch == ' ') {
                కొనసాగు;
            }

            i++;

            ఒకవేళ (ch == '0') {
                కొనసాగు;
            }

            ఒకవేళ (ch == '1') {
                CPU_సమితి(i - 1, &ముసుగు[n - 1]);
                కొనసాగు;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            తిరిగి NGX_CONF_ERROR;
        }
    }

#లేకపోతే

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#అయితే_ముగింపు

    తిరిగి NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#ఒకవేళ (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *ముసుగు;
    ngx_core_conf_t  *ccf;

    స్థిర ngx_cpuset_t  ఫలితం;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_చక్రం->conf_ctx,
                                           ngx_core_module);

    ఒకవేళ (ccf->cpu_affinity == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ccf->cpu_affinity_auto) {
        ముసుగు = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        కొరకు (i = 0, j = n; /* void */ ; i++) {

            ఒకవేళ (CPU_ISSET(i % CPU_SETSIZE, ముసుగు) && j-- == 0) {
                ఆపు;
            }

            ఒకవేళ (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                తిరిగి ఖాళీ;
            }

            /* void */
        }

        CPU_ZERO(&ఫలితం);
        CPU_సమితి(i % CPU_SETSIZE, &ఫలితం);

        తిరిగి &ఫలితం;
    }

    ఒకవేళ (ccf->cpu_affinity_n > n) {
        తిరిగి &ccf->cpu_affinity[n];
    }

    తిరిగి &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#లేకపోతే

    తిరిగి ఖాళీ;

#అయితే_ముగింపు
}


స్థిర అక్షరం *
ngx_సమితి_కార్మికుడు_processes(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    ngx_str_t        *విలువ;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    ఒకవేళ (ccf->కార్మికుడు_processes != NGX_CONF_UNSET) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    ఒకవేళ (ngx_strcmp(విలువ[1].దత్తాంశం, "auto") == 0) {
        ccf->కార్మికుడు_processes = ngx_ncpu;
        తిరిగి NGX_CONF_OK;
    }

    ccf->కార్మికుడు_processes = ngx_atoi(విలువ[1].దత్తాంశం, విలువ[1].len);

    ఒకవేళ (ccf->కార్మికుడు_processes == NGX_ERROR) {
        తిరిగి "invalid value";
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_ఎక్కించు_module(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
#ఒకవేళ (NGX_HAVE_DLOPEN)
    శూన్య                *handle;
    అక్షరం               **పేర్లు, **క్రమం;
    ngx_str_t           *విలువ, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    ఒకవేళ (cf->చక్రం->modules_వాడిన) {
        తిరిగి "is specified too late";
    }

    విలువ = cf->వాదనలు->elts;

    file = విలువ[1];

    ఒకవేళ (ngx_conf_నిండిన_name(cf->చక్రం, &file, 0) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_కూడు(cf->చక్రం->pool, 0);
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.దత్తాంశం);
    ఒకవేళ (handle == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.దత్తాంశం, ngx_dlerror());
        తిరిగి NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->దత్తాంశం = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    ఒకవేళ (modules == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &విలువ[1], "ngx_modules", ngx_dlerror());
        తిరిగి NGX_CONF_ERROR;
    }

    పేర్లు = ngx_dlsym(handle, "ngx_module_names");
    ఒకవేళ (పేర్లు == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &విలువ[1], "ngx_module_names", ngx_dlerror());
        తిరిగి NGX_CONF_ERROR;
    }

    క్రమం = ngx_dlsym(handle, "ngx_module_order");

    కొరకు (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = పేర్లు[i];

        ఒకవేళ (ngx_కూడు_module(cf, &file, module, క్రమం) != NGX_OK) {
            తిరిగి NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0, "module: %s i:%ui",
                       module->name, module->సూచీ);
    }

    తిరిగి NGX_CONF_OK;

#లేకపోతే

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    తిరిగి NGX_CONF_ERROR;

#అయితే_ముగింపు
}


#ఒకవేళ (NGX_HAVE_DLOPEN)

స్థిర శూన్య
ngx_unload_module(శూన్య *దత్తాంశం)
{
    శూన్య  *handle = దత్తాంశం;

    ఒకవేళ (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#అయితే_ముగింపు
