
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


ساكن فراغ ngx_show_نسخه_info(فراغ);
ساكن ngx_int_t ngx_جمع_کړه_inherited_sockets(ngx_دوره_t *دوره);
ساكن فراغ ngx_cleanup_environment(فراغ *معلومات);
ساكن فراغ ngx_cleanup_environment_variable(فراغ *معلومات);
ساكن ngx_int_t ngx_get_options(صحيح argc, حرف *ثابت *argv);
ساكن ngx_int_t ngx_process_options(ngx_دوره_t *دوره);
ساكن ngx_int_t ngx_save_argv(ngx_دوره_t *دوره, صحيح argc, حرف *ثابت *argv);
ساكن فراغ *ngx_core_module_create_conf(ngx_دوره_t *دوره);
ساكن حرف *ngx_core_module_init_conf(ngx_دوره_t *دوره, فراغ *conf);
ساكن حرف *ngx_سېټ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سېټ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سېټ_لومړیتوب(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سېټ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_سېټ_کارګر_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_بارول_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
#إذا (NGX_HAVE_DLOPEN)
ساكن فراغ ngx_unload_module(فراغ *معلومات);
#د_که_پای


ساكن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساكن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_FLAG,
      ngx_conf_سېټ_flag_خونه,
      0,
      offsetof(ngx_core_conf_t, daemon),
      عدم },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_FLAG,
      ngx_conf_سېټ_flag_خونه,
      0,
      offsetof(ngx_core_conf_t, master),
      عدم },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_msec_خونه,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      عدم },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_str_خونه,
      0,
      offsetof(ngx_core_conf_t, pid),
      عدم },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_str_خونه,
      0,
      offsetof(ngx_core_conf_t, قلف_file),
      عدم },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_سېټ_کارګر_processes,
      0,
      0,
      عدم },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_enum_خونه,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE12,
      ngx_سېټ_user,
      0,
      0,
      عدم },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_سېټ_لومړیتوب,
      0,
      0,
      عدم },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_1_نور,
      ngx_سېټ_cpu_affinity,
      0,
      0,
      عدم },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_عدد_خونه,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      عدم },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_off_خونه,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      عدم },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_msec_خونه,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      عدم },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_سېټ_str_خونه,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      عدم },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_سېټ_env,
      0,
      0,
      عدم },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_بارول_module,
      0,
      0,
      عدم },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    عدم,                                  /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    عدم,                                  /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_uint_t   ngx_show_help;
ساكن ngx_uint_t   ngx_show_نسخه;
ساكن ngx_uint_t   ngx_show_configure;
ساكن u_char      *ngx_مختاړی;
ساكن u_char      *ngx_error_log;
ساكن u_char      *ngx_conf_file;
ساكن u_char      *ngx_conf_params;
#إذا (NGX_CONTROL_API)
ساكن u_char      *ngx_control_addr;
#د_که_پای
ساكن حرف        *ngx_signal;


ساكن حرف **ngx_os_environ;


صحيح ngx_cdecl
اصلي(صحيح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *لوګاریتم;
    ngx_uint_t        i;
    ngx_دوره_t      *دوره, init_دوره;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    إذا (ngx_strerror_init() != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_get_options(argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_show_نسخه) {
        ngx_show_نسخه_info();

        إذا (!ngx_test_config) {
            أرجع 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_وخت_init();

#إذا (NGX_PCRE)
    ngx_regex_init();
#د_که_پای

    ngx_pid = ngx_getpid();
    ngx_پلار = ngx_getppid();

    لوګاریتم = ngx_log_init(ngx_مختاړی, ngx_error_log);
    إذا (لوګاریتم == عدم) {
        أرجع 1;
    }

    /* STUB */
#إذا (NGX_OPENSSL)
    ngx_ssl_init(لوګاریتم);
#د_که_پای

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_دوره, حجم(ngx_دوره_t));
    init_دوره.لوګاریتم = لوګاریتم;
    ngx_دوره = &init_دوره;

    init_دوره.pool = ngx_create_pool(1024, لوګاریتم);
    إذا (init_دوره.pool == عدم) {
        أرجع 1;
    }

    إذا (ngx_save_argv(&init_دوره, argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_process_options(&init_دوره) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_os_init(لوګاریتم) != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    إذا (ngx_دوراني_ازموینه_32_جدول_init() != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    إذا (ngx_جمع_کړه_inherited_sockets(&init_دوره) != NGX_OK) {
        أرجع 1;
    }

#إذا (NGX_CONTROL_API)
    ngx_control_preinit();
#د_که_پای

    إذا (ngx_preinit_modules() != NGX_OK) {
        أرجع 1;
    }

    دوره = ngx_init_دوره(&init_دوره);
    إذا (دوره == عدم) {
        إذا (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_دوره.conf_file.معلومات);
        }

        أرجع 1;
    }

    إذا (ngx_test_config) {
        إذا (!ngx_quiet_ډېرپېښ) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           دوره->conf_file.معلومات);
        }

        إذا (ngx_dump_config) {
            cd = دوره->config_dump.elts;

            لكل (i = 0; i < دوره->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.معلومات,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].بفر;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->وروستی - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        أرجع 0;
    }

    إذا (ngx_signal) {
        أرجع ngx_signal_process(دوره, ngx_signal);
    }

    ngx_os_حالت(دوره->لوګاریتم);

    ngx_دوره = دوره;

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    إذا (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#إذا !(NGX_WIN32)

    إذا (ngx_init_سیګنالونه(دوره->لوګاریتم) != NGX_OK) {
        أرجع 1;
    }

    إذا (!ngx_inherited && ccf->daemon) {
        إذا (ngx_daemon(دوره->لوګاریتم) != NGX_OK) {
            أرجع 1;
        }

        ngx_daemonized = 1;
    }

    إذا (ngx_inherited) {
        ngx_daemonized = 1;
    }

#د_که_پای

#إذا (NGX_CONTROL_API)
    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, دوره->لوګاریتم, 0,
                      "control API unavailable in single process mode");

    } وإلا {
        إذا (ngx_control_init(ngx_control_addr) != NGX_OK) {
            أرجع 1;
        }
    }
#د_که_پای

    إذا (ngx_create_pidfile(&ccf->pid, دوره->لوګاریتم) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_log_redirect_stderr(دوره) != NGX_OK) {
        أرجع 1;
    }

    إذا (لوګاریتم->file->fd != ngx_stderr) {
        إذا (ngx_close_file(لوګاریتم->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_دوره(دوره);

    } وإلا {
        ngx_master_process_دوره(دوره);
    }

    أرجع 0;
}


ساكن فراغ
ngx_show_نسخه_info(فراغ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    إذا (ngx_show_help) {
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
#إذا (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#د_که_پای
#که_تعریف_شوی NGX_مختاړی
            "  -p prefix     : set prefix path (default: " NGX_مختاړی ")"
                               NGX_LINEFEED
#وإلا
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#د_که_پای
            "  -e filename   : set error log file (default: "
#که_تعریف_شوی NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وإلا
                               NGX_ERROR_LOG_لار ")" NGX_LINEFEED
#د_که_پای
            "  -c filename   : set configuration file (default: " NGX_CONF_لار
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    إذا (ngx_show_configure) {

#که_تعریف_شوی NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#د_که_پای

#إذا (NGX_SSL)
        إذا (ngx_strcmp(ngx_ssl_نسخه(), OPENSSL_نسخه_متن) == 0) {
            ngx_write_stderr("built with " OPENSSL_نسخه_متن NGX_LINEFEED);
        } وإلا {
            ngx_write_stderr("built with " OPENSSL_نسخه_متن
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_نسخه());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#که_تعریف_شوی SSL_CTRL_سېټ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وإلا
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#د_که_پای
#د_که_پای

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساكن ngx_int_t
ngx_جمع_کړه_inherited_sockets(ngx_دوره_t *دوره)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    إذا (inherited == عدم) {
        أرجع NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, دوره->لوګاریتم, 0,
                  "using inherited sockets from \"%s\"", inherited);

    إذا (ngx_array_init(&دوره->listening, دوره->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (p = inherited, v = p; *p; p++) {
        إذا (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            إذا (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_ټېل(&دوره->listening);
            إذا (ls == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    إذا (v != p) {
        ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    أرجع ngx_سېټ_inherited_sockets(دوره);
}


حرف **
ngx_سېټ_environment(ngx_دوره_t *دوره, ngx_uint_t *وروستی)
{
    حرف                **p, **env, *str;
    اندازه_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    إذا (وروستی == عدم && ccf->environment) {
        أرجع ccf->environment;
    }

    var = ccf->env.elts;

    لكل (i = 0; i < ccf->env.nelts; i++) {
        إذا (ngx_strcmp(var[i].معلومات, "TZ") == 0
            || ngx_strncmp(var[i].معلومات, "TZ=", 3) == 0)
        {
            اذهب tz_وموندل;
        }
    }

    var = ngx_array_ټېل(&ccf->env);
    إذا (var == عدم) {
        أرجع عدم;
    }

    var->len = 2;
    var->معلومات = (u_char *) "TZ";

    var = ccf->env.elts;

tz_وموندل:

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].معلومات[var[i].len] == '=') {
            n++;
            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].معلومات, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                اكسر;
            }
        }
    }

    إذا (وروستی) {
        env = ngx_alloc((*وروستی + n + 1) * حجم(حرف *), دوره->لوګاریتم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        *وروستی = n;

    } وإلا {
        cln = ngx_pool_cleanup_جمع_کړه(دوره->pool, 0);
        إذا (cln == عدم) {
            أرجع عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), دوره->لوګاریتم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->معلومات = env;
    }

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].معلومات[var[i].len] == '=') {

            إذا (وروستی) {
                env[n++] = (حرف *) var[i].معلومات;
                استمر;
            }

            cln = ngx_pool_cleanup_جمع_کړه(دوره->pool, 0);
            إذا (cln == عدم) {
                أرجع عدم;
            }

            len = ngx_strlen(var[i].معلومات) + 1;

            str = ngx_alloc(len, دوره->لوګاریتم);
            إذا (str == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(str, var[i].معلومات, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->معلومات = str;

            env[n++] = str;

            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].معلومات, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                اكسر;
            }
        }
    }

    env[n] = عدم;

    إذا (وروستی == عدم) {
        ccf->environment = env;
        environ = env;
    }

    أرجع env;
}


ساكن فراغ
ngx_cleanup_environment(فراغ *معلومات)
{
    حرف  **env = معلومات;

    إذا (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        أرجع;
    }

    ngx_free(env);
}


ساكن فراغ
ngx_cleanup_environment_variable(فراغ *معلومات)
{
    حرف  *var = معلومات;

    حرف  **p;

    لكل (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        إذا (*p == var) {
            أرجع;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_دوګونی(ngx_دوره_t *دوره, حرف *ثابت *argv)
{
    حرف             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, حجم(ngx_exec_ctx_t));

    ctx.لار = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_سېټ_environment(دوره, &n);
    إذا (env == عدم) {
        أرجع NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + دوره->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    دوره->لوګاریتم);
    إذا (var == عدم) {
        ngx_free(env);
        أرجع NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = دوره->listening.elts;
    لكل (i = 0; i < دوره->listening.nelts; i++) {
        إذا (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#إذا (NGX_SETPROCTITLE_کاروي_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#د_که_پای

#إذا (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    إذا (env[n] != عدم) {
        n++;
    }
#د_که_پای

    env[n] = عدم;

#إذا (NGX_DEBUG)
    {
    حرف  **e;
    لكل (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, دوره->لوګاریتم, 0, "env: %s", *e);
    }
    }
#د_که_پای

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    إذا (ngx_rename_file(ccf->pid.معلومات, ccf->oldpid.معلومات) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.معلومات, ccf->oldpid.معلومات, argv[0]);

        ngx_free(env);
        ngx_free(var);

        أرجع NGX_INVALID_PID;
    }

    pid = ngx_execute(دوره, &ctx);

    إذا (pid == NGX_INVALID_PID) {
        إذا (ngx_rename_file(ccf->oldpid.معلومات, ccf->pid.معلومات)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.معلومات, ccf->pid.معلومات, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    أرجع pid;
}


ساكن ngx_int_t
ngx_get_options(صحيح argc, حرف *ثابت *argv)
{
    u_char     *p;
    ngx_int_t   i;

    لكل (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        إذا (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            أرجع NGX_ERROR;
        }

        طالما (*p) {

            بدل (*p++) {

            حالة '?':
            حالة 'h':
                ngx_show_نسخه = 1;
                ngx_show_help = 1;
                اكسر;

            حالة 'v':
                ngx_show_نسخه = 1;
                اكسر;

            حالة 'V':
                ngx_show_نسخه = 1;
                ngx_show_configure = 1;
                اكسر;

            حالة 't':
                ngx_test_config = 1;
                اكسر;

            حالة 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                اكسر;

            حالة 'q':
                ngx_quiet_ډېرپېښ = 1;
                اكسر;

            حالة 'p':
                إذا (*p) {
                    ngx_مختاړی = p;
                    اذهب بل;
                }

                إذا (argv[++i]) {
                    ngx_مختاړی = (u_char *) argv[i];
                    اذهب بل;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                أرجع NGX_ERROR;

            حالة 'e':
                إذا (*p) {
                    ngx_error_log = p;

                } وإلا إذا (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } وإلا {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    أرجع NGX_ERROR;
                }

                إذا (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                اذهب بل;

            حالة 'c':
                إذا (*p) {
                    ngx_conf_file = p;
                    اذهب بل;
                }

                إذا (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب بل;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                أرجع NGX_ERROR;

            حالة 'g':
                إذا (*p) {
                    ngx_conf_params = p;
                    اذهب بل;
                }

                إذا (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب بل;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                أرجع NGX_ERROR;

            حالة 's':
                إذا (*p) {
                    ngx_signal = (حرف *) p;

                } وإلا إذا (argv[++i]) {
                    ngx_signal = argv[i];

                } وإلا {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    أرجع NGX_ERROR;
                }

                إذا (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    اذهب بل;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                أرجع NGX_ERROR;

#إذا (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                إذا (*p) {
                    ngx_control_addr = p;
                    اذهب بل;
                }

                إذا (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب بل;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                أرجع NGX_ERROR;
#د_که_پای

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                أرجع NGX_ERROR;
            }
        }

    بل:

        استمر;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_دوره_t *دوره, صحيح argc, حرف *ثابت *argv)
{
#إذا (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#وإلا
    اندازه_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), دوره->لوګاریتم);
    إذا (ngx_argv == عدم) {
        أرجع NGX_ERROR;
    }

    لكل (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, دوره->لوګاریتم);
        إذا (ngx_argv[i] == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#د_که_پای

    ngx_os_environ = environ;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_دوره_t *دوره)
{
    u_char  *p;
    اندازه_t   len;

    إذا (ngx_مختاړی) {
        len = ngx_strlen(ngx_مختاړی);
        p = ngx_مختاړی;

        إذا (len && !ngx_لار_separator(p[len - 1])) {
            p = ngx_pnalloc(دوره->pool, len + 1);
            إذا (p == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(p, ngx_مختاړی, len);
            p[len++] = '/';
        }

        دوره->conf_مختاړی.len = len;
        دوره->conf_مختاړی.معلومات = p;
        دوره->مختاړی.len = len;
        دوره->مختاړی.معلومات = p;

    } وإلا {

#که_نه_تعریف_شوی NGX_مختاړی

        p = ngx_pnalloc(دوره->pool, NGX_MAX_لار);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        إذا (ngx_getcwd(p, NGX_MAX_لار) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            أرجع NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        دوره->conf_مختاړی.len = len;
        دوره->conf_مختاړی.معلومات = p;
        دوره->مختاړی.len = len;
        دوره->مختاړی.معلومات = p;

#وإلا

#که_تعریف_شوی NGX_CONF_مختاړی
        ngx_str_سېټ(&دوره->conf_مختاړی, NGX_CONF_مختاړی);
#وإلا
        ngx_str_سېټ(&دوره->conf_مختاړی, NGX_مختاړی);
#د_که_پای
        ngx_str_سېټ(&دوره->مختاړی, NGX_مختاړی);

#د_که_پای
    }

    إذا (ngx_conf_file) {
        دوره->conf_file.len = ngx_strlen(ngx_conf_file);
        دوره->conf_file.معلومات = ngx_conf_file;

    } وإلا {
        ngx_str_سېټ(&دوره->conf_file, NGX_CONF_لار);
    }

    إذا (ngx_conf_ډک_name(دوره, &دوره->conf_file, 0) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    لكل (p = دوره->conf_file.معلومات + دوره->conf_file.len - 1;
         p > دوره->conf_file.معلومات;
         p--)
    {
        إذا (ngx_لار_separator(*p)) {
            دوره->conf_مختاړی.len = p - دوره->conf_file.معلومات + 1;
            دوره->conf_مختاړی.معلومات = دوره->conf_file.معلومات;
            اكسر;
        }
    }

    إذا (ngx_error_log) {
        دوره->error_log.len = ngx_strlen(ngx_error_log);
        دوره->error_log.معلومات = ngx_error_log;

    } وإلا {
        ngx_str_سېټ(&دوره->error_log, NGX_ERROR_LOG_لار);
    }

    إذا (ngx_conf_params) {
        دوره->conf_param.len = ngx_strlen(ngx_conf_params);
        دوره->conf_param.معلومات = ngx_conf_params;
    }

    إذا (ngx_test_config) {
        دوره->لوګاریتم->log_level = NGX_LOG_INFO;
    }

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_دوره_t *دوره)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(دوره->pool, حجم(ngx_core_conf_t));
    إذا (ccf == عدم) {
        أرجع عدم;
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

    ccf->کارګر_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->ډله = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    إذا (ngx_array_init(&ccf->env, دوره->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_دوره_t *دوره, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_ارزښت(ccf->daemon, 1);
    ngx_conf_init_ارزښت(ccf->master, 1);
    ngx_conf_init_msec_ارزښت(ccf->timer_resolution, 0);
    ngx_conf_init_msec_ارزښت(ccf->shutdown_timeout, 0);

    ngx_conf_init_ارزښت(ccf->کارګر_processes, 1);
    ngx_conf_init_ارزښت(ccf->debug_points, 0);

#إذا (NGX_HAVE_CPU_AFFINITY)

    إذا (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->کارګر_processes)
    {
        ngx_log_error(NGX_LOG_WARN, دوره->لوګاریتم, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#د_که_پای


    إذا (ccf->pid.len == 0) {
        ngx_str_سېټ(&ccf->pid, NGX_PID_لار);
    }

    إذا (ngx_conf_ډک_name(دوره, &ccf->pid, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.معلومات = ngx_pnalloc(دوره->pool, ccf->oldpid.len);
    إذا (ccf->oldpid.معلومات == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.معلومات, ccf->pid.معلومات, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    إذا (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        إذا (ngx_conf_ډک_name(دوره, &ccf->working_directory, 0) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_file_info(ccf->working_directory.معلومات, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.معلومات);
            أرجع NGX_CONF_ERROR;
        }

        إذا (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.معلومات);
            أرجع NGX_CONF_ERROR;
        }
    }


#إذا !(NGX_WIN32)

    إذا (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية ډله   *grp;
        بنية passwd  *pwd;

        ngx_سېټ_errno(0);
        pwd = getpwnam(NGX_USER);
        إذا (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_سېټ_errno(0);
        grp = getgrnam(NGX_ډله);
        إذا (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          "getgrnam(\"" NGX_ډله "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->ډله = grp->gr_gid;
    }


    إذا (ccf->قلف_file.len == 0) {
        ngx_str_سېټ(&ccf->قلف_file, NGX_قلف_لار);
    }

    إذا (ngx_conf_ډک_name(دوره, &ccf->قلف_file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    {
    ngx_str_t  قلف_file;

    قلف_file = دوره->old_دوره->قلف_file;

    إذا (قلف_file.len) {
        قلف_file.len--;

        إذا (ccf->قلف_file.len != قلف_file.len
            || ngx_strncmp(ccf->قلف_file.معلومات, قلف_file.معلومات, قلف_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        دوره->قلف_file.len = قلف_file.len + 1;
        قلف_file.len += حجم(".accept");

        دوره->قلف_file.معلومات = ngx_pstrdup(دوره->pool, &قلف_file);
        إذا (دوره->قلف_file.معلومات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا {
        دوره->قلف_file.len = ccf->قلف_file.len + 1;
        دوره->قلف_file.معلومات = ngx_pnalloc(دوره->pool,
                                      ccf->قلف_file.len + حجم(".accept"));
        إذا (دوره->قلف_file.معلومات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(دوره->قلف_file.معلومات, ccf->قلف_file.معلومات,
                              ccf->قلف_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#د_که_پای

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سېټ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    أرجع NGX_CONF_OK;

#وإلا

    ngx_core_conf_t  *ccf = conf;

    حرف             *ډله;
    بنية passwd    *pwd;
    بنية ډله     *grp;
    ngx_str_t        *ارزښت;

    إذا (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    إذا (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        أرجع NGX_CONF_OK;
    }

    ارزښت = cf->دلیلونه->elts;

    ccf->username = (حرف *) ارزښت[1].معلومات;

    ngx_سېټ_errno(0);
    pwd = getpwnam((ثابت حرف *) ارزښت[1].معلومات);
    إذا (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", ارزښت[1].معلومات);
        أرجع NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    ډله = (حرف *) ((cf->دلیلونه->nelts == 2) ? ارزښت[1].معلومات : ارزښت[2].معلومات);

    ngx_سېټ_errno(0);
    grp = getgrnam(ډله);
    إذا (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", ډله);
        أرجع NGX_CONF_ERROR;
    }

    ccf->ډله = grp->gr_gid;

    أرجع NGX_CONF_OK;

#د_که_پای
}


ساكن حرف *
ngx_سېټ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *ارزښت, *var;
    ngx_uint_t   i;

    var = ngx_array_ټېل(&ccf->env);
    إذا (var == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ارزښت = cf->دلیلونه->elts;
    *var = ارزښت[1];

    لكل (i = 0; i < ارزښت[1].len; i++) {

        إذا (ارزښت[1].معلومات[i] == '=') {

            var->len = i;

            أرجع NGX_CONF_OK;
        }
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سېټ_لومړیتوب(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *ارزښت;
    ngx_uint_t        n, minus;

    إذا (ccf->لومړیتوب != 0) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    إذا (ارزښت[1].معلومات[0] == '-') {
        n = 1;
        minus = 1;

    } وإلا إذا (ارزښت[1].معلومات[0] == '+') {
        n = 1;
        minus = 0;

    } وإلا {
        n = 0;
        minus = 0;
    }

    ccf->لومړیتوب = ngx_atoi(&ارزښت[1].معلومات[n], ارزښت[1].len - n);
    إذا (ccf->لومړیتوب == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (minus) {
        ccf->لومړیتوب = -ccf->لومړیتوب;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سېټ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *ارزښت;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *ماسک;

    إذا (ccf->cpu_affinity) {
        أرجع "is duplicate";
    }

    ماسک = ngx_palloc(cf->pool, (cf->دلیلونه->nelts - 1) * حجم(ngx_cpuset_t));
    إذا (ماسک == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->دلیلونه->nelts - 1;
    ccf->cpu_affinity = ماسک;

    ارزښت = cf->دلیلونه->elts;

    إذا (ngx_strcmp(ارزښت[1].معلومات, "auto") == 0) {

        إذا (cf->دلیلونه->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            أرجع NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&ماسک[0]);
        لكل (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_سېټ(i, &ماسک[0]);
        }

        n = 2;

    } وإلا {
        n = 1;
    }

    لكل ( /* void */ ; n < cf->دلیلونه->nelts; n++) {

        إذا (ارزښت[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            أرجع NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&ماسک[n - 1]);

        لكل (p = ارزښت[n].معلومات + ارزښت[n].len - 1;
             p >= ارزښت[n].معلومات;
             p--)
        {
            ch = *p;

            إذا (ch == ' ') {
                استمر;
            }

            i++;

            إذا (ch == '0') {
                استمر;
            }

            إذا (ch == '1') {
                CPU_سېټ(i - 1, &ماسک[n - 1]);
                استمر;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            أرجع NGX_CONF_ERROR;
        }
    }

#وإلا

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#د_که_پای

    أرجع NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *ماسک;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  پایله;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_دوره->conf_ctx,
                                           ngx_core_module);

    إذا (ccf->cpu_affinity == عدم) {
        أرجع عدم;
    }

    إذا (ccf->cpu_affinity_auto) {
        ماسک = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        لكل (i = 0, j = n; /* void */ ; i++) {

            إذا (CPU_ISSET(i % CPU_SETSIZE, ماسک) && j-- == 0) {
                اكسر;
            }

            إذا (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                أرجع عدم;
            }

            /* void */
        }

        CPU_ZERO(&پایله);
        CPU_سېټ(i % CPU_SETSIZE, &پایله);

        أرجع &پایله;
    }

    إذا (ccf->cpu_affinity_n > n) {
        أرجع &ccf->cpu_affinity[n];
    }

    أرجع &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وإلا

    أرجع عدم;

#د_که_پای
}


ساكن حرف *
ngx_سېټ_کارګر_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *ارزښت;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    إذا (ccf->کارګر_processes != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    إذا (ngx_strcmp(ارزښت[1].معلومات, "auto") == 0) {
        ccf->کارګر_processes = ngx_ncpu;
        أرجع NGX_CONF_OK;
    }

    ccf->کارګر_processes = ngx_atoi(ارزښت[1].معلومات, ارزښت[1].len);

    إذا (ccf->کارګر_processes == NGX_ERROR) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_بارول_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **نومونه, **ترتیب;
    ngx_str_t           *ارزښت, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    إذا (cf->دوره->modules_کارول_شوی) {
        أرجع "is specified too late";
    }

    ارزښت = cf->دلیلونه->elts;

    file = ارزښت[1];

    إذا (ngx_conf_ډک_name(cf->دوره, &file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_جمع_کړه(cf->دوره->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.معلومات);
    إذا (handle == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.معلومات, ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->معلومات = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    إذا (modules == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ارزښت[1], "ngx_modules", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    نومونه = ngx_dlsym(handle, "ngx_module_names");
    إذا (نومونه == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ارزښت[1], "ngx_module_names", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    ترتیب = ngx_dlsym(handle, "ngx_module_order");

    لكل (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = نومونه[i];

        إذا (ngx_جمع_کړه_module(cf, &file, module, ترتیب) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لوګاریتم, 0, "module: %s i:%ui",
                       module->name, module->شاخص);
    }

    أرجع NGX_CONF_OK;

#وإلا

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    أرجع NGX_CONF_ERROR;

#د_که_پای
}


#إذا (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *معلومات)
{
    فراغ  *handle = معلومات;

    إذا (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#د_که_پای
