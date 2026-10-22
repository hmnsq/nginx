
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <nginx.h>


ساکن خالی ngx_show_نسخہ_info(خالی);
ساکن ngx_int_t ngx_جوڑو_inherited_sockets(ngx_چکر_t *چکر);
ساکن خالی ngx_cleanup_environment(خالی *ڈیٹا);
ساکن خالی ngx_cleanup_environment_variable(خالی *ڈیٹا);
ساکن ngx_int_t ngx_get_options(صحیح argc, حرف *ثابت *argv);
ساکن ngx_int_t ngx_process_options(ngx_چکر_t *چکر);
ساکن ngx_int_t ngx_save_argv(ngx_چکر_t *چکر, صحیح argc, حرف *ثابت *argv);
ساکن خالی *ngx_core_module_create_conf(ngx_چکر_t *چکر);
ساکن حرف *ngx_core_module_init_conf(ngx_چکر_t *چکر, خالی *conf);
ساکن حرف *ngx_سیٹ_user(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
ساکن حرف *ngx_سیٹ_env(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
ساکن حرف *ngx_سیٹ_ترجیح(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
ساکن حرف *ngx_سیٹ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    خالی *conf);
ساکن حرف *ngx_سیٹ_کارکن_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    خالی *conf);
ساکن حرف *ngx_بھرو_module(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
#اگر (NGX_HAVE_DLOPEN)
ساکن خالی ngx_unload_module(خالی *ڈیٹا);
#اگر_ختم


ساکن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساکن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_FLAG,
      ngx_conf_سیٹ_flag_خانہ,
      0,
      offsetof(ngx_core_conf_t, daemon),
      تہی },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_FLAG,
      ngx_conf_سیٹ_flag_خانہ,
      0,
      offsetof(ngx_core_conf_t, master),
      تہی },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_msec_خانہ,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      تہی },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_خانہ,
      0,
      offsetof(ngx_core_conf_t, pid),
      تہی },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_خانہ,
      0,
      offsetof(ngx_core_conf_t, تالا_file),
      تہی },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_کارکن_processes,
      0,
      0,
      تہی },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_enum_خانہ,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE12,
      ngx_سیٹ_user,
      0,
      0,
      تہی },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_ترجیح,
      0,
      0,
      تہی },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_1_مزید,
      ngx_سیٹ_cpu_affinity,
      0,
      0,
      تہی },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_عدد_خانہ,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      تہی },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_off_خانہ,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      تہی },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_msec_خانہ,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      تہی },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_خانہ,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      تہی },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_env,
      0,
      0,
      تہی },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE1,
      ngx_بھرو_module,
      0,
      0,
      تہی },

      ngx_null_command
};


ساکن ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    تہی,                                  /* init master */
    تہی,                                  /* init module */
    تہی,                                  /* init process */
    تہی,                                  /* init thread */
    تہی,                                  /* exit thread */
    تہی,                                  /* exit process */
    تہی,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساکن ngx_uint_t   ngx_show_help;
ساکن ngx_uint_t   ngx_show_نسخہ;
ساکن ngx_uint_t   ngx_show_configure;
ساکن u_char      *ngx_سابقہ;
ساکن u_char      *ngx_error_log;
ساکن u_char      *ngx_conf_file;
ساکن u_char      *ngx_conf_params;
#اگر (NGX_CONTROL_API)
ساکن u_char      *ngx_control_addr;
#اگر_ختم
ساکن حرف        *ngx_signal;


ساکن حرف **ngx_os_environ;


صحیح ngx_cdecl
مین(صحیح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *لوگارتھم;
    ngx_uint_t        i;
    ngx_چکر_t      *چکر, init_چکر;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    اگر (ngx_strerror_init() != NGX_OK) {
        واپس 1;
    }

    اگر (ngx_get_options(argc, argv) != NGX_OK) {
        واپس 1;
    }

    اگر (ngx_show_نسخہ) {
        ngx_show_نسخہ_info();

        اگر (!ngx_test_config) {
            واپس 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_وقت_init();

#اگر (NGX_PCRE)
    ngx_regex_init();
#اگر_ختم

    ngx_pid = ngx_getpid();
    ngx_والد = ngx_getppid();

    لوگارتھم = ngx_log_init(ngx_سابقہ, ngx_error_log);
    اگر (لوگارتھم == تہی) {
        واپس 1;
    }

    /* STUB */
#اگر (NGX_OPENSSL)
    ngx_ssl_init(لوگارتھم);
#اگر_ختم

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_چکر, حجم(ngx_چکر_t));
    init_چکر.لوگارتھم = لوگارتھم;
    ngx_چکر = &init_چکر;

    init_چکر.pool = ngx_create_pool(1024, لوگارتھم);
    اگر (init_چکر.pool == تہی) {
        واپس 1;
    }

    اگر (ngx_save_argv(&init_چکر, argc, argv) != NGX_OK) {
        واپس 1;
    }

    اگر (ngx_process_options(&init_چکر) != NGX_OK) {
        واپس 1;
    }

    اگر (ngx_os_init(لوگارتھم) != NGX_OK) {
        واپس 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    اگر (ngx_چکری_جانچ_32_جدول_init() != NGX_OK) {
        واپس 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    اگر (ngx_جوڑو_inherited_sockets(&init_چکر) != NGX_OK) {
        واپس 1;
    }

#اگر (NGX_CONTROL_API)
    ngx_control_preinit();
#اگر_ختم

    اگر (ngx_preinit_modules() != NGX_OK) {
        واپس 1;
    }

    چکر = ngx_init_چکر(&init_چکر);
    اگر (چکر == تہی) {
        اگر (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_چکر.conf_file.ڈیٹا);
        }

        واپس 1;
    }

    اگر (ngx_test_config) {
        اگر (!ngx_quiet_منوال) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           چکر->conf_file.ڈیٹا);
        }

        اگر (ngx_dump_config) {
            cd = چکر->config_dump.elts;

            برائے (i = 0; i < چکر->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (خالی) ngx_write_fd(ngx_stdout, cd[i].name.ڈیٹا,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].بفر;

                (خالی) ngx_write_fd(ngx_stdout, b->pos, b->آخری - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        واپس 0;
    }

    اگر (ngx_signal) {
        واپس ngx_signal_process(چکر, ngx_signal);
    }

    ngx_os_حالت(چکر->لوگارتھم);

    ngx_چکر = چکر;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    اگر (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#اگر !(NGX_WIN32)

    اگر (ngx_init_اشارات(چکر->لوگارتھم) != NGX_OK) {
        واپس 1;
    }

    اگر (!ngx_inherited && ccf->daemon) {
        اگر (ngx_daemon(چکر->لوگارتھم) != NGX_OK) {
            واپس 1;
        }

        ngx_daemonized = 1;
    }

    اگر (ngx_inherited) {
        ngx_daemonized = 1;
    }

#اگر_ختم

#اگر (NGX_CONTROL_API)
    اگر (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                      "control API unavailable in single process mode");

    } ورنہ {
        اگر (ngx_control_init(ngx_control_addr) != NGX_OK) {
            واپس 1;
        }
    }
#اگر_ختم

    اگر (ngx_create_pidfile(&ccf->pid, چکر->لوگارتھم) != NGX_OK) {
        واپس 1;
    }

    اگر (ngx_log_redirect_stderr(چکر) != NGX_OK) {
        واپس 1;
    }

    اگر (لوگارتھم->file->fd != ngx_stderr) {
        اگر (ngx_close_file(لوگارتھم->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    اگر (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_چکر(چکر);

    } ورنہ {
        ngx_master_process_چکر(چکر);
    }

    واپس 0;
}


ساکن خالی
ngx_show_نسخہ_info(خالی)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    اگر (ngx_show_help) {
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
#اگر (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#اگر_ختم
#اگر_تعریف_شدہ NGX_سابقہ
            "  -p prefix     : set prefix path (default: " NGX_سابقہ ")"
                               NGX_LINEFEED
#ورنہ
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#اگر_ختم
            "  -e filename   : set error log file (default: "
#اگر_تعریف_شدہ NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#ورنہ
                               NGX_ERROR_LOG_راستہ ")" NGX_LINEFEED
#اگر_ختم
            "  -c filename   : set configuration file (default: " NGX_CONF_راستہ
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    اگر (ngx_show_configure) {

#اگر_تعریف_شدہ NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#اگر_ختم

#اگر (NGX_SSL)
        اگر (ngx_strcmp(ngx_ssl_نسخہ(), OPENSSL_نسخہ_متن) == 0) {
            ngx_write_stderr("built with " OPENSSL_نسخہ_متن NGX_LINEFEED);
        } ورنہ {
            ngx_write_stderr("built with " OPENSSL_نسخہ_متن
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_نسخہ());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#اگر_تعریف_شدہ SSL_CTRL_سیٹ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#ورنہ
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#اگر_ختم
#اگر_ختم

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساکن ngx_int_t
ngx_جوڑو_inherited_sockets(ngx_چکر_t *چکر)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    اگر (inherited == تہی) {
        واپس NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, 0,
                  "using inherited sockets from \"%s\"", inherited);

    اگر (ngx_array_init(&چکر->listening, چکر->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    برائے (p = inherited, v = p; *p; p++) {
        اگر (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            اگر (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                توڑو;
            }

            v = p + 1;

            ls = ngx_array_ڈالو(&چکر->listening);
            اگر (ls == تہی) {
                واپس NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    اگر (v != p) {
        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    واپس ngx_سیٹ_inherited_sockets(چکر);
}


حرف **
ngx_سیٹ_environment(ngx_چکر_t *چکر, ngx_uint_t *آخری)
{
    حرف                **p, **env, *str;
    سائز_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    اگر (آخری == تہی && ccf->environment) {
        واپس ccf->environment;
    }

    var = ccf->env.elts;

    برائے (i = 0; i < ccf->env.nelts; i++) {
        اگر (ngx_strcmp(var[i].ڈیٹا, "TZ") == 0
            || ngx_strncmp(var[i].ڈیٹا, "TZ=", 3) == 0)
        {
            جاؤ tz_ملا;
        }
    }

    var = ngx_array_ڈالو(&ccf->env);
    اگر (var == تہی) {
        واپس تہی;
    }

    var->len = 2;
    var->ڈیٹا = (u_char *) "TZ";

    var = ccf->env.elts;

tz_ملا:

    n = 0;

    برائے (i = 0; i < ccf->env.nelts; i++) {

        اگر (var[i].ڈیٹا[var[i].len] == '=') {
            n++;
            جاری;
        }

        برائے (p = ngx_os_environ; *p; p++) {

            اگر (ngx_strncmp(*p, var[i].ڈیٹا, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                توڑو;
            }
        }
    }

    اگر (آخری) {
        env = ngx_alloc((*آخری + n + 1) * حجم(حرف *), چکر->لوگارتھم);
        اگر (env == تہی) {
            واپس تہی;
        }

        *آخری = n;

    } ورنہ {
        cln = ngx_pool_cleanup_جوڑو(چکر->pool, 0);
        اگر (cln == تہی) {
            واپس تہی;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), چکر->لوگارتھم);
        اگر (env == تہی) {
            واپس تہی;
        }

        cln->handler = ngx_cleanup_environment;
        cln->ڈیٹا = env;
    }

    n = 0;

    برائے (i = 0; i < ccf->env.nelts; i++) {

        اگر (var[i].ڈیٹا[var[i].len] == '=') {

            اگر (آخری) {
                env[n++] = (حرف *) var[i].ڈیٹا;
                جاری;
            }

            cln = ngx_pool_cleanup_جوڑو(چکر->pool, 0);
            اگر (cln == تہی) {
                واپس تہی;
            }

            len = ngx_strlen(var[i].ڈیٹا) + 1;

            str = ngx_alloc(len, چکر->لوگارتھم);
            اگر (str == تہی) {
                واپس تہی;
            }

            ngx_memcpy(str, var[i].ڈیٹا, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->ڈیٹا = str;

            env[n++] = str;

            جاری;
        }

        برائے (p = ngx_os_environ; *p; p++) {

            اگر (ngx_strncmp(*p, var[i].ڈیٹا, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                توڑو;
            }
        }
    }

    env[n] = تہی;

    اگر (آخری == تہی) {
        ccf->environment = env;
        environ = env;
    }

    واپس env;
}


ساکن خالی
ngx_cleanup_environment(خالی *ڈیٹا)
{
    حرف  **env = ڈیٹا;

    اگر (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        واپس;
    }

    ngx_free(env);
}


ساکن خالی
ngx_cleanup_environment_variable(خالی *ڈیٹا)
{
    حرف  *var = ڈیٹا;

    حرف  **p;

    برائے (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        اگر (*p == var) {
            واپس;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ثنائی(ngx_چکر_t *چکر, حرف *ثابت *argv)
{
    حرف             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, حجم(ngx_exec_ctx_t));

    ctx.راستہ = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_سیٹ_environment(چکر, &n);
    اگر (env == تہی) {
        واپس NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + چکر->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    چکر->لوگارتھم);
    اگر (var == تہی) {
        ngx_free(env);
        واپس NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = چکر->listening.elts;
    برائے (i = 0; i < چکر->listening.nelts; i++) {
        اگر (ls[i].ignore) {
            جاری;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#اگر (NGX_SETPROCTITLE_استعمال_کرتا_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#اگر_ختم

#اگر (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    اگر (env[n] != تہی) {
        n++;
    }
#اگر_ختم

    env[n] = تہی;

#اگر (NGX_DEBUG)
    {
    حرف  **e;
    برائے (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, چکر->لوگارتھم, 0, "env: %s", *e);
    }
    }
#اگر_ختم

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    اگر (ngx_rename_file(ccf->pid.ڈیٹا, ccf->oldpid.ڈیٹا) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.ڈیٹا, ccf->oldpid.ڈیٹا, argv[0]);

        ngx_free(env);
        ngx_free(var);

        واپس NGX_INVALID_PID;
    }

    pid = ngx_execute(چکر, &ctx);

    اگر (pid == NGX_INVALID_PID) {
        اگر (ngx_rename_file(ccf->oldpid.ڈیٹا, ccf->pid.ڈیٹا)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.ڈیٹا, ccf->pid.ڈیٹا, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    واپس pid;
}


ساکن ngx_int_t
ngx_get_options(صحیح argc, حرف *ثابت *argv)
{
    u_char     *p;
    ngx_int_t   i;

    برائے (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        اگر (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            واپس NGX_ERROR;
        }

        جب (*p) {

            انتخاب (*p++) {

            حالت '?':
            حالت 'h':
                ngx_show_نسخہ = 1;
                ngx_show_help = 1;
                توڑو;

            حالت 'v':
                ngx_show_نسخہ = 1;
                توڑو;

            حالت 'V':
                ngx_show_نسخہ = 1;
                ngx_show_configure = 1;
                توڑو;

            حالت 't':
                ngx_test_config = 1;
                توڑو;

            حالت 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                توڑو;

            حالت 'q':
                ngx_quiet_منوال = 1;
                توڑو;

            حالت 'p':
                اگر (*p) {
                    ngx_سابقہ = p;
                    جاؤ اگلا;
                }

                اگر (argv[++i]) {
                    ngx_سابقہ = (u_char *) argv[i];
                    جاؤ اگلا;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                واپس NGX_ERROR;

            حالت 'e':
                اگر (*p) {
                    ngx_error_log = p;

                } ورنہ اگر (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } ورنہ {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    واپس NGX_ERROR;
                }

                اگر (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                جاؤ اگلا;

            حالت 'c':
                اگر (*p) {
                    ngx_conf_file = p;
                    جاؤ اگلا;
                }

                اگر (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    جاؤ اگلا;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                واپس NGX_ERROR;

            حالت 'g':
                اگر (*p) {
                    ngx_conf_params = p;
                    جاؤ اگلا;
                }

                اگر (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    جاؤ اگلا;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                واپس NGX_ERROR;

            حالت 's':
                اگر (*p) {
                    ngx_signal = (حرف *) p;

                } ورنہ اگر (argv[++i]) {
                    ngx_signal = argv[i];

                } ورنہ {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    واپس NGX_ERROR;
                }

                اگر (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    جاؤ اگلا;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                واپس NGX_ERROR;

#اگر (NGX_CONTROL_API)
            حالت 'l':
                ngx_control_api_enabled = 1;

                اگر (*p) {
                    ngx_control_addr = p;
                    جاؤ اگلا;
                }

                اگر (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    جاؤ اگلا;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                واپس NGX_ERROR;
#اگر_ختم

            طےشدہ:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                واپس NGX_ERROR;
            }
        }

    اگلا:

        جاری;
    }

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_save_argv(ngx_چکر_t *چکر, صحیح argc, حرف *ثابت *argv)
{
#اگر (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#ورنہ
    سائز_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), چکر->لوگارتھم);
    اگر (ngx_argv == تہی) {
        واپس NGX_ERROR;
    }

    برائے (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, چکر->لوگارتھم);
        اگر (ngx_argv[i] == تہی) {
            واپس NGX_ERROR;
        }

        (خالی) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = تہی;

#اگر_ختم

    ngx_os_environ = environ;

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_process_options(ngx_چکر_t *چکر)
{
    u_char  *p;
    سائز_t   len;

    اگر (ngx_سابقہ) {
        len = ngx_strlen(ngx_سابقہ);
        p = ngx_سابقہ;

        اگر (len && !ngx_راستہ_separator(p[len - 1])) {
            p = ngx_pnalloc(چکر->pool, len + 1);
            اگر (p == تہی) {
                واپس NGX_ERROR;
            }

            ngx_memcpy(p, ngx_سابقہ, len);
            p[len++] = '/';
        }

        چکر->conf_سابقہ.len = len;
        چکر->conf_سابقہ.ڈیٹا = p;
        چکر->سابقہ.len = len;
        چکر->سابقہ.ڈیٹا = p;

    } ورنہ {

#اگر_غیر_تعریف_شدہ NGX_سابقہ

        p = ngx_pnalloc(چکر->pool, NGX_MAX_راستہ);
        اگر (p == تہی) {
            واپس NGX_ERROR;
        }

        اگر (ngx_getcwd(p, NGX_MAX_راستہ) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            واپس NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        چکر->conf_سابقہ.len = len;
        چکر->conf_سابقہ.ڈیٹا = p;
        چکر->سابقہ.len = len;
        چکر->سابقہ.ڈیٹا = p;

#ورنہ

#اگر_تعریف_شدہ NGX_CONF_سابقہ
        ngx_str_سیٹ(&چکر->conf_سابقہ, NGX_CONF_سابقہ);
#ورنہ
        ngx_str_سیٹ(&چکر->conf_سابقہ, NGX_سابقہ);
#اگر_ختم
        ngx_str_سیٹ(&چکر->سابقہ, NGX_سابقہ);

#اگر_ختم
    }

    اگر (ngx_conf_file) {
        چکر->conf_file.len = ngx_strlen(ngx_conf_file);
        چکر->conf_file.ڈیٹا = ngx_conf_file;

    } ورنہ {
        ngx_str_سیٹ(&چکر->conf_file, NGX_CONF_راستہ);
    }

    اگر (ngx_conf_بھرا_name(چکر, &چکر->conf_file, 0) != NGX_OK) {
        واپس NGX_ERROR;
    }

    برائے (p = چکر->conf_file.ڈیٹا + چکر->conf_file.len - 1;
         p > چکر->conf_file.ڈیٹا;
         p--)
    {
        اگر (ngx_راستہ_separator(*p)) {
            چکر->conf_سابقہ.len = p - چکر->conf_file.ڈیٹا + 1;
            چکر->conf_سابقہ.ڈیٹا = چکر->conf_file.ڈیٹا;
            توڑو;
        }
    }

    اگر (ngx_error_log) {
        چکر->error_log.len = ngx_strlen(ngx_error_log);
        چکر->error_log.ڈیٹا = ngx_error_log;

    } ورنہ {
        ngx_str_سیٹ(&چکر->error_log, NGX_ERROR_LOG_راستہ);
    }

    اگر (ngx_conf_params) {
        چکر->conf_param.len = ngx_strlen(ngx_conf_params);
        چکر->conf_param.ڈیٹا = ngx_conf_params;
    }

    اگر (ngx_test_config) {
        چکر->لوگارتھم->log_level = NGX_LOG_INFO;
    }

    واپس NGX_OK;
}


ساکن خالی *
ngx_core_module_create_conf(ngx_چکر_t *چکر)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(چکر->pool, حجم(ngx_core_conf_t));
    اگر (ccf == تہی) {
        واپس تہی;
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

    ccf->کارکن_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->گروہ = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    اگر (ngx_array_init(&ccf->env, چکر->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        واپس تہی;
    }

    واپس ccf;
}


ساکن حرف *
ngx_core_module_init_conf(ngx_چکر_t *چکر, خالی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_قدر(ccf->daemon, 1);
    ngx_conf_init_قدر(ccf->master, 1);
    ngx_conf_init_msec_قدر(ccf->timer_resolution, 0);
    ngx_conf_init_msec_قدر(ccf->shutdown_timeout, 0);

    ngx_conf_init_قدر(ccf->کارکن_processes, 1);
    ngx_conf_init_قدر(ccf->debug_points, 0);

#اگر (NGX_HAVE_CPU_AFFINITY)

    اگر (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->کارکن_processes)
    {
        ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#اگر_ختم


    اگر (ccf->pid.len == 0) {
        ngx_str_سیٹ(&ccf->pid, NGX_PID_راستہ);
    }

    اگر (ngx_conf_بھرا_name(چکر, &ccf->pid, 0) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.ڈیٹا = ngx_pnalloc(چکر->pool, ccf->oldpid.len);
    اگر (ccf->oldpid.ڈیٹا == تہی) {
        واپس NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.ڈیٹا, ccf->pid.ڈیٹا, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    اگر (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        اگر (ngx_conf_بھرا_name(چکر, &ccf->working_directory, 0) != NGX_OK) {
            واپس NGX_CONF_ERROR;
        }

        اگر (ngx_file_info(ccf->working_directory.ڈیٹا, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.ڈیٹا);
            واپس NGX_CONF_ERROR;
        }

        اگر (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.ڈیٹا);
            واپس NGX_CONF_ERROR;
        }
    }


#اگر !(NGX_WIN32)

    اگر (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ساخت گروہ   *grp;
        ساخت passwd  *pwd;

        ngx_سیٹ_errno(0);
        pwd = getpwnam(NGX_USER);
        اگر (pwd == تہی) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            واپس NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_سیٹ_errno(0);
        grp = getgrnam(NGX_گروہ);
        اگر (grp == تہی) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "getgrnam(\"" NGX_گروہ "\") failed");
            واپس NGX_CONF_ERROR;
        }

        ccf->گروہ = grp->gr_gid;
    }


    اگر (ccf->تالا_file.len == 0) {
        ngx_str_سیٹ(&ccf->تالا_file, NGX_تالا_راستہ);
    }

    اگر (ngx_conf_بھرا_name(چکر, &ccf->تالا_file, 0) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    {
    ngx_str_t  تالا_file;

    تالا_file = چکر->old_چکر->تالا_file;

    اگر (تالا_file.len) {
        تالا_file.len--;

        اگر (ccf->تالا_file.len != تالا_file.len
            || ngx_strncmp(ccf->تالا_file.ڈیٹا, تالا_file.ڈیٹا, تالا_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        چکر->تالا_file.len = تالا_file.len + 1;
        تالا_file.len += حجم(".accept");

        چکر->تالا_file.ڈیٹا = ngx_pstrdup(چکر->pool, &تالا_file);
        اگر (چکر->تالا_file.ڈیٹا == تہی) {
            واپس NGX_CONF_ERROR;
        }

    } ورنہ {
        چکر->تالا_file.len = ccf->تالا_file.len + 1;
        چکر->تالا_file.ڈیٹا = ngx_pnalloc(چکر->pool,
                                      ccf->تالا_file.len + حجم(".accept"));
        اگر (چکر->تالا_file.ڈیٹا == تہی) {
            واپس NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(چکر->تالا_file.ڈیٹا, ccf->تالا_file.ڈیٹا,
                              ccf->تالا_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#اگر_ختم

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_سیٹ_user(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
#اگر (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    واپس NGX_CONF_OK;

#ورنہ

    ngx_core_conf_t  *ccf = conf;

    حرف             *گروہ;
    ساخت passwd    *pwd;
    ساخت گروہ     *grp;
    ngx_str_t        *قدر;

    اگر (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        واپس "is duplicate";
    }

    اگر (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        واپس NGX_CONF_OK;
    }

    قدر = cf->دلائل->elts;

    ccf->username = (حرف *) قدر[1].ڈیٹا;

    ngx_سیٹ_errno(0);
    pwd = getpwnam((ثابت حرف *) قدر[1].ڈیٹا);
    اگر (pwd == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", قدر[1].ڈیٹا);
        واپس NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    گروہ = (حرف *) ((cf->دلائل->nelts == 2) ? قدر[1].ڈیٹا : قدر[2].ڈیٹا);

    ngx_سیٹ_errno(0);
    grp = getgrnam(گروہ);
    اگر (grp == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", گروہ);
        واپس NGX_CONF_ERROR;
    }

    ccf->گروہ = grp->gr_gid;

    واپس NGX_CONF_OK;

#اگر_ختم
}


ساکن حرف *
ngx_سیٹ_env(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *قدر, *var;
    ngx_uint_t   i;

    var = ngx_array_ڈالو(&ccf->env);
    اگر (var == تہی) {
        واپس NGX_CONF_ERROR;
    }

    قدر = cf->دلائل->elts;
    *var = قدر[1];

    برائے (i = 0; i < قدر[1].len; i++) {

        اگر (قدر[1].ڈیٹا[i] == '=') {

            var->len = i;

            واپس NGX_CONF_OK;
        }
    }

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_سیٹ_ترجیح(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *قدر;
    ngx_uint_t        n, minus;

    اگر (ccf->ترجیح != 0) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    اگر (قدر[1].ڈیٹا[0] == '-') {
        n = 1;
        minus = 1;

    } ورنہ اگر (قدر[1].ڈیٹا[0] == '+') {
        n = 1;
        minus = 0;

    } ورنہ {
        n = 0;
        minus = 0;
    }

    ccf->ترجیح = ngx_atoi(&قدر[1].ڈیٹا[n], قدر[1].len - n);
    اگر (ccf->ترجیح == NGX_ERROR) {
        واپس "invalid number";
    }

    اگر (minus) {
        ccf->ترجیح = -ccf->ترجیح;
    }

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_سیٹ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
#اگر (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *قدر;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *نقاب;

    اگر (ccf->cpu_affinity) {
        واپس "is duplicate";
    }

    نقاب = ngx_palloc(cf->pool, (cf->دلائل->nelts - 1) * حجم(ngx_cpuset_t));
    اگر (نقاب == تہی) {
        واپس NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->دلائل->nelts - 1;
    ccf->cpu_affinity = نقاب;

    قدر = cf->دلائل->elts;

    اگر (ngx_strcmp(قدر[1].ڈیٹا, "auto") == 0) {

        اگر (cf->دلائل->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            واپس NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&نقاب[0]);
        برائے (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_سیٹ(i, &نقاب[0]);
        }

        n = 2;

    } ورنہ {
        n = 1;
    }

    برائے ( /* void */ ; n < cf->دلائل->nelts; n++) {

        اگر (قدر[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            واپس NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&نقاب[n - 1]);

        برائے (p = قدر[n].ڈیٹا + قدر[n].len - 1;
             p >= قدر[n].ڈیٹا;
             p--)
        {
            ch = *p;

            اگر (ch == ' ') {
                جاری;
            }

            i++;

            اگر (ch == '0') {
                جاری;
            }

            اگر (ch == '1') {
                CPU_سیٹ(i - 1, &نقاب[n - 1]);
                جاری;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            واپس NGX_CONF_ERROR;
        }
    }

#ورنہ

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#اگر_ختم

    واپس NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#اگر (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *نقاب;
    ngx_core_conf_t  *ccf;

    ساکن ngx_cpuset_t  نتیجہ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_چکر->conf_ctx,
                                           ngx_core_module);

    اگر (ccf->cpu_affinity == تہی) {
        واپس تہی;
    }

    اگر (ccf->cpu_affinity_auto) {
        نقاب = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        برائے (i = 0, j = n; /* void */ ; i++) {

            اگر (CPU_ISSET(i % CPU_SETSIZE, نقاب) && j-- == 0) {
                توڑو;
            }

            اگر (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                واپس تہی;
            }

            /* void */
        }

        CPU_ZERO(&نتیجہ);
        CPU_سیٹ(i % CPU_SETSIZE, &نتیجہ);

        واپس &نتیجہ;
    }

    اگر (ccf->cpu_affinity_n > n) {
        واپس &ccf->cpu_affinity[n];
    }

    واپس &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#ورنہ

    واپس تہی;

#اگر_ختم
}


ساکن حرف *
ngx_سیٹ_کارکن_processes(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    ngx_str_t        *قدر;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    اگر (ccf->کارکن_processes != NGX_CONF_UNSET) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    اگر (ngx_strcmp(قدر[1].ڈیٹا, "auto") == 0) {
        ccf->کارکن_processes = ngx_ncpu;
        واپس NGX_CONF_OK;
    }

    ccf->کارکن_processes = ngx_atoi(قدر[1].ڈیٹا, قدر[1].len);

    اگر (ccf->کارکن_processes == NGX_ERROR) {
        واپس "invalid value";
    }

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_بھرو_module(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
#اگر (NGX_HAVE_DLOPEN)
    خالی                *handle;
    حرف               **نام, **ترتیب;
    ngx_str_t           *قدر, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    اگر (cf->چکر->modules_مستعمل) {
        واپس "is specified too late";
    }

    قدر = cf->دلائل->elts;

    file = قدر[1];

    اگر (ngx_conf_بھرا_name(cf->چکر, &file, 0) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_جوڑو(cf->چکر->pool, 0);
    اگر (cln == تہی) {
        واپس NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.ڈیٹا);
    اگر (handle == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.ڈیٹا, ngx_dlerror());
        واپس NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->ڈیٹا = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    اگر (modules == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_modules", ngx_dlerror());
        واپس NGX_CONF_ERROR;
    }

    نام = ngx_dlsym(handle, "ngx_module_names");
    اگر (نام == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_module_names", ngx_dlerror());
        واپس NGX_CONF_ERROR;
    }

    ترتیب = ngx_dlsym(handle, "ngx_module_order");

    برائے (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = نام[i];

        اگر (ngx_جوڑو_module(cf, &file, module, ترتیب) != NGX_OK) {
            واپس NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "module: %s i:%ui",
                       module->name, module->اشاریہ);
    }

    واپس NGX_CONF_OK;

#ورنہ

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    واپس NGX_CONF_ERROR;

#اگر_ختم
}


#اگر (NGX_HAVE_DLOPEN)

ساکن خالی
ngx_unload_module(خالی *ڈیٹا)
{
    خالی  *handle = ڈیٹا;

    اگر (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#اگر_ختم
