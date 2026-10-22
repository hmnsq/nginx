
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


ساكن فراغ ngx_show_نسخہ_info(فراغ);
ساكن ngx_int_t ngx_جوڑو_inherited_sockets(ngx_چکر_t *چکر);
ساكن فراغ ngx_cleanup_environment(فراغ *انکڑے);
ساكن فراغ ngx_cleanup_environment_variable(فراغ *انکڑے);
ساكن ngx_int_t ngx_get_options(صحيح argc, حرف *ثابت *argv);
ساكن ngx_int_t ngx_process_options(ngx_چکر_t *چکر);
ساكن ngx_int_t ngx_save_argv(ngx_چکر_t *چکر, صحيح argc, حرف *ثابت *argv);
ساكن فراغ *ngx_core_module_create_conf(ngx_چکر_t *چکر);
ساكن حرف *ngx_core_module_init_conf(ngx_چکر_t *چکر, فراغ *conf);
ساكن حرف *ngx_سیٹ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سیٹ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سیٹ_ترجیح(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سیٹ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_سیٹ_کامہ_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_لدو_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
#إذا (NGX_HAVE_DLOPEN)
ساكن فراغ ngx_unload_module(فراغ *انکڑے);
#جے_انت


ساكن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساكن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_FLAG,
      ngx_conf_سیٹ_flag_تھاں,
      0,
      offsetof(ngx_core_conf_t, daemon),
      عدم },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_FLAG,
      ngx_conf_سیٹ_flag_تھاں,
      0,
      offsetof(ngx_core_conf_t, master),
      عدم },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_msec_تھاں,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      عدم },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_تھاں,
      0,
      offsetof(ngx_core_conf_t, pid),
      عدم },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_تھاں,
      0,
      offsetof(ngx_core_conf_t, جندرا_file),
      عدم },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_کامہ_processes,
      0,
      0,
      عدم },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_enum_تھاں,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE12,
      ngx_سیٹ_user,
      0,
      0,
      عدم },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_ترجیح,
      0,
      0,
      عدم },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_1_ہور,
      ngx_سیٹ_cpu_affinity,
      0,
      0,
      عدم },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_عدد_تھاں,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      عدم },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_off_تھاں,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      عدم },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_msec_تھاں,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      عدم },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_conf_سیٹ_str_تھاں,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      عدم },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_سیٹ_env,
      0,
      0,
      عدم },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE1,
      ngx_لدو_module,
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
ساكن ngx_uint_t   ngx_show_نسخہ;
ساكن ngx_uint_t   ngx_show_configure;
ساكن u_char      *ngx_اگیتر;
ساكن u_char      *ngx_error_log;
ساكن u_char      *ngx_conf_file;
ساكن u_char      *ngx_conf_params;
#إذا (NGX_CONTROL_API)
ساكن u_char      *ngx_control_addr;
#جے_انت
ساكن حرف        *ngx_signal;


ساكن حرف **ngx_os_environ;


صحيح ngx_cdecl
مکھ(صحيح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *لوگارتھم;
    ngx_uint_t        i;
    ngx_چکر_t      *چکر, init_چکر;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    إذا (ngx_strerror_init() != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_get_options(argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_show_نسخہ) {
        ngx_show_نسخہ_info();

        إذا (!ngx_test_config) {
            أرجع 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_ویلا_init();

#إذا (NGX_PCRE)
    ngx_regex_init();
#جے_انت

    ngx_pid = ngx_getpid();
    ngx_ماپا = ngx_getppid();

    لوگارتھم = ngx_log_init(ngx_اگیتر, ngx_error_log);
    إذا (لوگارتھم == عدم) {
        أرجع 1;
    }

    /* STUB */
#إذا (NGX_OPENSSL)
    ngx_ssl_init(لوگارتھم);
#جے_انت

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_چکر, حجم(ngx_چکر_t));
    init_چکر.لوگارتھم = لوگارتھم;
    ngx_چکر = &init_چکر;

    init_چکر.pool = ngx_create_pool(1024, لوگارتھم);
    إذا (init_چکر.pool == عدم) {
        أرجع 1;
    }

    إذا (ngx_save_argv(&init_چکر, argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_process_options(&init_چکر) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_os_init(لوگارتھم) != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    إذا (ngx_چکری_جانچ_32_جدول_init() != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    إذا (ngx_جوڑو_inherited_sockets(&init_چکر) != NGX_OK) {
        أرجع 1;
    }

#إذا (NGX_CONTROL_API)
    ngx_control_preinit();
#جے_انت

    إذا (ngx_preinit_modules() != NGX_OK) {
        أرجع 1;
    }

    چکر = ngx_init_چکر(&init_چکر);
    إذا (چکر == عدم) {
        إذا (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_چکر.conf_file.انکڑے);
        }

        أرجع 1;
    }

    إذا (ngx_test_config) {
        إذا (!ngx_quiet_منوال) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           چکر->conf_file.انکڑے);
        }

        إذا (ngx_dump_config) {
            cd = چکر->config_dump.elts;

            لكل (i = 0; i < چکر->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.انکڑے,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].بفر;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->آخری - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        أرجع 0;
    }

    إذا (ngx_signal) {
        أرجع ngx_signal_process(چکر, ngx_signal);
    }

    ngx_os_حالت(چکر->لوگارتھم);

    ngx_چکر = چکر;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    إذا (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#إذا !(NGX_WIN32)

    إذا (ngx_init_اشارے(چکر->لوگارتھم) != NGX_OK) {
        أرجع 1;
    }

    إذا (!ngx_inherited && ccf->daemon) {
        إذا (ngx_daemon(چکر->لوگارتھم) != NGX_OK) {
            أرجع 1;
        }

        ngx_daemonized = 1;
    }

    إذا (ngx_inherited) {
        ngx_daemonized = 1;
    }

#جے_انت

#إذا (NGX_CONTROL_API)
    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                      "control API unavailable in single process mode");

    } وإلا {
        إذا (ngx_control_init(ngx_control_addr) != NGX_OK) {
            أرجع 1;
        }
    }
#جے_انت

    إذا (ngx_create_pidfile(&ccf->pid, چکر->لوگارتھم) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_log_redirect_stderr(چکر) != NGX_OK) {
        أرجع 1;
    }

    إذا (لوگارتھم->file->fd != ngx_stderr) {
        إذا (ngx_close_file(لوگارتھم->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_چکر(چکر);

    } وإلا {
        ngx_master_process_چکر(چکر);
    }

    أرجع 0;
}


ساكن فراغ
ngx_show_نسخہ_info(فراغ)
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
#جے_انت
#جے_تعریف_شدہ NGX_اگیتر
            "  -p prefix     : set prefix path (default: " NGX_اگیتر ")"
                               NGX_LINEFEED
#وإلا
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#جے_انت
            "  -e filename   : set error log file (default: "
#جے_تعریف_شدہ NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وإلا
                               NGX_ERROR_LOG_رستہ ")" NGX_LINEFEED
#جے_انت
            "  -c filename   : set configuration file (default: " NGX_CONF_رستہ
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    إذا (ngx_show_configure) {

#جے_تعریف_شدہ NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#جے_انت

#إذا (NGX_SSL)
        إذا (ngx_strcmp(ngx_ssl_نسخہ(), OPENSSL_نسخہ_متن) == 0) {
            ngx_write_stderr("built with " OPENSSL_نسخہ_متن NGX_LINEFEED);
        } وإلا {
            ngx_write_stderr("built with " OPENSSL_نسخہ_متن
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_نسخہ());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#جے_تعریف_شدہ SSL_CTRL_سیٹ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وإلا
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#جے_انت
#جے_انت

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساكن ngx_int_t
ngx_جوڑو_inherited_sockets(ngx_چکر_t *چکر)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    إذا (inherited == عدم) {
        أرجع NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, 0,
                  "using inherited sockets from \"%s\"", inherited);

    إذا (ngx_array_init(&چکر->listening, چکر->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (p = inherited, v = p; *p; p++) {
        إذا (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            إذا (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_ڈالو(&چکر->listening);
            إذا (ls == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    إذا (v != p) {
        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    أرجع ngx_سیٹ_inherited_sockets(چکر);
}


حرف **
ngx_سیٹ_environment(ngx_چکر_t *چکر, ngx_uint_t *آخری)
{
    حرف                **p, **env, *str;
    ناپ_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    إذا (آخری == عدم && ccf->environment) {
        أرجع ccf->environment;
    }

    var = ccf->env.elts;

    لكل (i = 0; i < ccf->env.nelts; i++) {
        إذا (ngx_strcmp(var[i].انکڑے, "TZ") == 0
            || ngx_strncmp(var[i].انکڑے, "TZ=", 3) == 0)
        {
            اذهب tz_لبھیا;
        }
    }

    var = ngx_array_ڈالو(&ccf->env);
    إذا (var == عدم) {
        أرجع عدم;
    }

    var->len = 2;
    var->انکڑے = (u_char *) "TZ";

    var = ccf->env.elts;

tz_لبھیا:

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].انکڑے[var[i].len] == '=') {
            n++;
            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].انکڑے, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                اكسر;
            }
        }
    }

    إذا (آخری) {
        env = ngx_alloc((*آخری + n + 1) * حجم(حرف *), چکر->لوگارتھم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        *آخری = n;

    } وإلا {
        cln = ngx_pool_cleanup_جوڑو(چکر->pool, 0);
        إذا (cln == عدم) {
            أرجع عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), چکر->لوگارتھم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->انکڑے = env;
    }

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].انکڑے[var[i].len] == '=') {

            إذا (آخری) {
                env[n++] = (حرف *) var[i].انکڑے;
                استمر;
            }

            cln = ngx_pool_cleanup_جوڑو(چکر->pool, 0);
            إذا (cln == عدم) {
                أرجع عدم;
            }

            len = ngx_strlen(var[i].انکڑے) + 1;

            str = ngx_alloc(len, چکر->لوگارتھم);
            إذا (str == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(str, var[i].انکڑے, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->انکڑے = str;

            env[n++] = str;

            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].انکڑے, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                اكسر;
            }
        }
    }

    env[n] = عدم;

    إذا (آخری == عدم) {
        ccf->environment = env;
        environ = env;
    }

    أرجع env;
}


ساكن فراغ
ngx_cleanup_environment(فراغ *انکڑے)
{
    حرف  **env = انکڑے;

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
ngx_cleanup_environment_variable(فراغ *انکڑے)
{
    حرف  *var = انکڑے;

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

    ctx.رستہ = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_سیٹ_environment(چکر, &n);
    إذا (env == عدم) {
        أرجع NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + چکر->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    چکر->لوگارتھم);
    إذا (var == عدم) {
        ngx_free(env);
        أرجع NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = چکر->listening.elts;
    لكل (i = 0; i < چکر->listening.nelts; i++) {
        إذا (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#إذا (NGX_SETPROCTITLE_ورتدا_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#جے_انت

#إذا (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    إذا (env[n] != عدم) {
        n++;
    }
#جے_انت

    env[n] = عدم;

#إذا (NGX_DEBUG)
    {
    حرف  **e;
    لكل (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, چکر->لوگارتھم, 0, "env: %s", *e);
    }
    }
#جے_انت

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    إذا (ngx_rename_file(ccf->pid.انکڑے, ccf->oldpid.انکڑے) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.انکڑے, ccf->oldpid.انکڑے, argv[0]);

        ngx_free(env);
        ngx_free(var);

        أرجع NGX_INVALID_PID;
    }

    pid = ngx_execute(چکر, &ctx);

    إذا (pid == NGX_INVALID_PID) {
        إذا (ngx_rename_file(ccf->oldpid.انکڑے, ccf->pid.انکڑے)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.انکڑے, ccf->pid.انکڑے, argv[0]);
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
                ngx_show_نسخہ = 1;
                ngx_show_help = 1;
                اكسر;

            حالة 'v':
                ngx_show_نسخہ = 1;
                اكسر;

            حالة 'V':
                ngx_show_نسخہ = 1;
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
                ngx_quiet_منوال = 1;
                اكسر;

            حالة 'p':
                إذا (*p) {
                    ngx_اگیتر = p;
                    اذهب اگلا;
                }

                إذا (argv[++i]) {
                    ngx_اگیتر = (u_char *) argv[i];
                    اذهب اگلا;
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

                اذهب اگلا;

            حالة 'c':
                إذا (*p) {
                    ngx_conf_file = p;
                    اذهب اگلا;
                }

                إذا (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب اگلا;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                أرجع NGX_ERROR;

            حالة 'g':
                إذا (*p) {
                    ngx_conf_params = p;
                    اذهب اگلا;
                }

                إذا (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب اگلا;
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
                    اذهب اگلا;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                أرجع NGX_ERROR;

#إذا (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                إذا (*p) {
                    ngx_control_addr = p;
                    اذهب اگلا;
                }

                إذا (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب اگلا;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                أرجع NGX_ERROR;
#جے_انت

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                أرجع NGX_ERROR;
            }
        }

    اگلا:

        استمر;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_چکر_t *چکر, صحيح argc, حرف *ثابت *argv)
{
#إذا (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#وإلا
    ناپ_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), چکر->لوگارتھم);
    إذا (ngx_argv == عدم) {
        أرجع NGX_ERROR;
    }

    لكل (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, چکر->لوگارتھم);
        إذا (ngx_argv[i] == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#جے_انت

    ngx_os_environ = environ;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_چکر_t *چکر)
{
    u_char  *p;
    ناپ_t   len;

    إذا (ngx_اگیتر) {
        len = ngx_strlen(ngx_اگیتر);
        p = ngx_اگیتر;

        إذا (len && !ngx_رستہ_separator(p[len - 1])) {
            p = ngx_pnalloc(چکر->pool, len + 1);
            إذا (p == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(p, ngx_اگیتر, len);
            p[len++] = '/';
        }

        چکر->conf_اگیتر.len = len;
        چکر->conf_اگیتر.انکڑے = p;
        چکر->اگیتر.len = len;
        چکر->اگیتر.انکڑے = p;

    } وإلا {

#جے_بنا_تعریف NGX_اگیتر

        p = ngx_pnalloc(چکر->pool, NGX_MAX_رستہ);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        إذا (ngx_getcwd(p, NGX_MAX_رستہ) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            أرجع NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        چکر->conf_اگیتر.len = len;
        چکر->conf_اگیتر.انکڑے = p;
        چکر->اگیتر.len = len;
        چکر->اگیتر.انکڑے = p;

#وإلا

#جے_تعریف_شدہ NGX_CONF_اگیتر
        ngx_str_سیٹ(&چکر->conf_اگیتر, NGX_CONF_اگیتر);
#وإلا
        ngx_str_سیٹ(&چکر->conf_اگیتر, NGX_اگیتر);
#جے_انت
        ngx_str_سیٹ(&چکر->اگیتر, NGX_اگیتر);

#جے_انت
    }

    إذا (ngx_conf_file) {
        چکر->conf_file.len = ngx_strlen(ngx_conf_file);
        چکر->conf_file.انکڑے = ngx_conf_file;

    } وإلا {
        ngx_str_سیٹ(&چکر->conf_file, NGX_CONF_رستہ);
    }

    إذا (ngx_conf_بھریا_name(چکر, &چکر->conf_file, 0) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    لكل (p = چکر->conf_file.انکڑے + چکر->conf_file.len - 1;
         p > چکر->conf_file.انکڑے;
         p--)
    {
        إذا (ngx_رستہ_separator(*p)) {
            چکر->conf_اگیتر.len = p - چکر->conf_file.انکڑے + 1;
            چکر->conf_اگیتر.انکڑے = چکر->conf_file.انکڑے;
            اكسر;
        }
    }

    إذا (ngx_error_log) {
        چکر->error_log.len = ngx_strlen(ngx_error_log);
        چکر->error_log.انکڑے = ngx_error_log;

    } وإلا {
        ngx_str_سیٹ(&چکر->error_log, NGX_ERROR_LOG_رستہ);
    }

    إذا (ngx_conf_params) {
        چکر->conf_param.len = ngx_strlen(ngx_conf_params);
        چکر->conf_param.انکڑے = ngx_conf_params;
    }

    إذا (ngx_test_config) {
        چکر->لوگارتھم->log_level = NGX_LOG_INFO;
    }

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_چکر_t *چکر)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(چکر->pool, حجم(ngx_core_conf_t));
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

    ccf->کامہ_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->گروہ = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    إذا (ngx_array_init(&ccf->env, چکر->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_چکر_t *چکر, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_قدر(ccf->daemon, 1);
    ngx_conf_init_قدر(ccf->master, 1);
    ngx_conf_init_msec_قدر(ccf->timer_resolution, 0);
    ngx_conf_init_msec_قدر(ccf->shutdown_timeout, 0);

    ngx_conf_init_قدر(ccf->کامہ_processes, 1);
    ngx_conf_init_قدر(ccf->debug_points, 0);

#إذا (NGX_HAVE_CPU_AFFINITY)

    إذا (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->کامہ_processes)
    {
        ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#جے_انت


    إذا (ccf->pid.len == 0) {
        ngx_str_سیٹ(&ccf->pid, NGX_PID_رستہ);
    }

    إذا (ngx_conf_بھریا_name(چکر, &ccf->pid, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.انکڑے = ngx_pnalloc(چکر->pool, ccf->oldpid.len);
    إذا (ccf->oldpid.انکڑے == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.انکڑے, ccf->pid.انکڑے, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    إذا (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        إذا (ngx_conf_بھریا_name(چکر, &ccf->working_directory, 0) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_file_info(ccf->working_directory.انکڑے, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.انکڑے);
            أرجع NGX_CONF_ERROR;
        }

        إذا (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.انکڑے);
            أرجع NGX_CONF_ERROR;
        }
    }


#إذا !(NGX_WIN32)

    إذا (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية گروہ   *grp;
        بنية passwd  *pwd;

        ngx_سیٹ_errno(0);
        pwd = getpwnam(NGX_USER);
        إذا (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_سیٹ_errno(0);
        grp = getgrnam(NGX_گروہ);
        إذا (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "getgrnam(\"" NGX_گروہ "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->گروہ = grp->gr_gid;
    }


    إذا (ccf->جندرا_file.len == 0) {
        ngx_str_سیٹ(&ccf->جندرا_file, NGX_جندرا_رستہ);
    }

    إذا (ngx_conf_بھریا_name(چکر, &ccf->جندرا_file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    {
    ngx_str_t  جندرا_file;

    جندرا_file = چکر->old_چکر->جندرا_file;

    إذا (جندرا_file.len) {
        جندرا_file.len--;

        إذا (ccf->جندرا_file.len != جندرا_file.len
            || ngx_strncmp(ccf->جندرا_file.انکڑے, جندرا_file.انکڑے, جندرا_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        چکر->جندرا_file.len = جندرا_file.len + 1;
        جندرا_file.len += حجم(".accept");

        چکر->جندرا_file.انکڑے = ngx_pstrdup(چکر->pool, &جندرا_file);
        إذا (چکر->جندرا_file.انکڑے == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا {
        چکر->جندرا_file.len = ccf->جندرا_file.len + 1;
        چکر->جندرا_file.انکڑے = ngx_pnalloc(چکر->pool,
                                      ccf->جندرا_file.len + حجم(".accept"));
        إذا (چکر->جندرا_file.انکڑے == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(چکر->جندرا_file.انکڑے, ccf->جندرا_file.انکڑے,
                              ccf->جندرا_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#جے_انت

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سیٹ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    أرجع NGX_CONF_OK;

#وإلا

    ngx_core_conf_t  *ccf = conf;

    حرف             *گروہ;
    بنية passwd    *pwd;
    بنية گروہ     *grp;
    ngx_str_t        *قدر;

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

    قدر = cf->دلیلاں->elts;

    ccf->username = (حرف *) قدر[1].انکڑے;

    ngx_سیٹ_errno(0);
    pwd = getpwnam((ثابت حرف *) قدر[1].انکڑے);
    إذا (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", قدر[1].انکڑے);
        أرجع NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    گروہ = (حرف *) ((cf->دلیلاں->nelts == 2) ? قدر[1].انکڑے : قدر[2].انکڑے);

    ngx_سیٹ_errno(0);
    grp = getgrnam(گروہ);
    إذا (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", گروہ);
        أرجع NGX_CONF_ERROR;
    }

    ccf->گروہ = grp->gr_gid;

    أرجع NGX_CONF_OK;

#جے_انت
}


ساكن حرف *
ngx_سیٹ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *قدر, *var;
    ngx_uint_t   i;

    var = ngx_array_ڈالو(&ccf->env);
    إذا (var == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دلیلاں->elts;
    *var = قدر[1];

    لكل (i = 0; i < قدر[1].len; i++) {

        إذا (قدر[1].انکڑے[i] == '=') {

            var->len = i;

            أرجع NGX_CONF_OK;
        }
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سیٹ_ترجیح(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *قدر;
    ngx_uint_t        n, minus;

    إذا (ccf->ترجیح != 0) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    إذا (قدر[1].انکڑے[0] == '-') {
        n = 1;
        minus = 1;

    } وإلا إذا (قدر[1].انکڑے[0] == '+') {
        n = 1;
        minus = 0;

    } وإلا {
        n = 0;
        minus = 0;
    }

    ccf->ترجیح = ngx_atoi(&قدر[1].انکڑے[n], قدر[1].len - n);
    إذا (ccf->ترجیح == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (minus) {
        ccf->ترجیح = -ccf->ترجیح;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سیٹ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *قدر;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *نقاب;

    إذا (ccf->cpu_affinity) {
        أرجع "is duplicate";
    }

    نقاب = ngx_palloc(cf->pool, (cf->دلیلاں->nelts - 1) * حجم(ngx_cpuset_t));
    إذا (نقاب == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->دلیلاں->nelts - 1;
    ccf->cpu_affinity = نقاب;

    قدر = cf->دلیلاں->elts;

    إذا (ngx_strcmp(قدر[1].انکڑے, "auto") == 0) {

        إذا (cf->دلیلاں->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            أرجع NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&نقاب[0]);
        لكل (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_سیٹ(i, &نقاب[0]);
        }

        n = 2;

    } وإلا {
        n = 1;
    }

    لكل ( /* void */ ; n < cf->دلیلاں->nelts; n++) {

        إذا (قدر[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            أرجع NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&نقاب[n - 1]);

        لكل (p = قدر[n].انکڑے + قدر[n].len - 1;
             p >= قدر[n].انکڑے;
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
                CPU_سیٹ(i - 1, &نقاب[n - 1]);
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
#جے_انت

    أرجع NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *نقاب;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  نتیجہ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_چکر->conf_ctx,
                                           ngx_core_module);

    إذا (ccf->cpu_affinity == عدم) {
        أرجع عدم;
    }

    إذا (ccf->cpu_affinity_auto) {
        نقاب = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        لكل (i = 0, j = n; /* void */ ; i++) {

            إذا (CPU_ISSET(i % CPU_SETSIZE, نقاب) && j-- == 0) {
                اكسر;
            }

            إذا (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                أرجع عدم;
            }

            /* void */
        }

        CPU_ZERO(&نتیجہ);
        CPU_سیٹ(i % CPU_SETSIZE, &نتیجہ);

        أرجع &نتیجہ;
    }

    إذا (ccf->cpu_affinity_n > n) {
        أرجع &ccf->cpu_affinity[n];
    }

    أرجع &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وإلا

    أرجع عدم;

#جے_انت
}


ساكن حرف *
ngx_سیٹ_کامہ_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *قدر;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    إذا (ccf->کامہ_processes != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    إذا (ngx_strcmp(قدر[1].انکڑے, "auto") == 0) {
        ccf->کامہ_processes = ngx_ncpu;
        أرجع NGX_CONF_OK;
    }

    ccf->کامہ_processes = ngx_atoi(قدر[1].انکڑے, قدر[1].len);

    إذا (ccf->کامہ_processes == NGX_ERROR) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_لدو_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **ناں, **ترتیب;
    ngx_str_t           *قدر, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    إذا (cf->چکر->modules_ورتیا) {
        أرجع "is specified too late";
    }

    قدر = cf->دلیلاں->elts;

    file = قدر[1];

    إذا (ngx_conf_بھریا_name(cf->چکر, &file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_جوڑو(cf->چکر->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.انکڑے);
    إذا (handle == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.انکڑے, ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->انکڑے = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    إذا (modules == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_modules", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    ناں = ngx_dlsym(handle, "ngx_module_names");
    إذا (ناں == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_module_names", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    ترتیب = ngx_dlsym(handle, "ngx_module_order");

    لكل (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = ناں[i];

        إذا (ngx_جوڑو_module(cf, &file, module, ترتیب) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "module: %s i:%ui",
                       module->name, module->اشاریہ);
    }

    أرجع NGX_CONF_OK;

#وإلا

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    أرجع NGX_CONF_ERROR;

#جے_انت
}


#إذا (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *انکڑے)
{
    فراغ  *handle = انکڑے;

    إذا (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#جے_انت
