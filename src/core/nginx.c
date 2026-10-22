
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


ساكن فراغ ngx_show_نسخه_info(فراغ);
ساكن ngx_int_t ngx_جمع_کن_inherited_sockets(ngx_چرخه_t *چرخه);
ساكن فراغ ngx_cleanup_environment(فراغ *داده);
ساكن فراغ ngx_cleanup_environment_variable(فراغ *داده);
ساكن ngx_int_t ngx_get_options(صحيح argc, حرف *ثابت *argv);
ساكن ngx_int_t ngx_process_options(ngx_چرخه_t *چرخه);
ساكن ngx_int_t ngx_save_argv(ngx_چرخه_t *چرخه, صحيح argc, حرف *ثابت *argv);
ساكن فراغ *ngx_core_module_create_conf(ngx_چرخه_t *چرخه);
ساكن حرف *ngx_core_module_init_conf(ngx_چرخه_t *چرخه, فراغ *conf);
ساكن حرف *ngx_مجموعه_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_مجموعه_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_مجموعه_اولویت(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_مجموعه_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_مجموعه_کارگر_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_بارگیری_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
#إذا (NGX_HAVE_DLOPEN)
ساكن فراغ ngx_unload_module(فراغ *داده);
#پایان_اگر


ساكن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساكن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_FLAG,
      ngx_conf_مجموعه_flag_جایگاه,
      0,
      offsetof(ngx_core_conf_t, daemon),
      عدم },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_FLAG,
      ngx_conf_مجموعه_flag_جایگاه,
      0,
      offsetof(ngx_core_conf_t, master),
      عدم },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_msec_جایگاه,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      عدم },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_str_جایگاه,
      0,
      offsetof(ngx_core_conf_t, pid),
      عدم },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_str_جایگاه,
      0,
      offsetof(ngx_core_conf_t, قفل_file),
      عدم },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_مجموعه_کارگر_processes,
      0,
      0,
      عدم },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_enum_جایگاه,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE12,
      ngx_مجموعه_user,
      0,
      0,
      عدم },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_مجموعه_اولویت,
      0,
      0,
      عدم },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_1_بیشتر,
      ngx_مجموعه_cpu_affinity,
      0,
      0,
      عدم },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_رقم_جایگاه,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      عدم },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_off_جایگاه,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      عدم },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_msec_جایگاه,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      عدم },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_conf_مجموعه_str_جایگاه,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      عدم },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_مجموعه_env,
      0,
      0,
      عدم },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE1,
      ngx_بارگیری_module,
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
ساكن u_char      *ngx_پیشوند;
ساكن u_char      *ngx_error_log;
ساكن u_char      *ngx_conf_file;
ساكن u_char      *ngx_conf_params;
#إذا (NGX_CONTROL_API)
ساكن u_char      *ngx_control_addr;
#پایان_اگر
ساكن حرف        *ngx_signal;


ساكن حرف **ngx_os_environ;


صحيح ngx_cdecl
اصلی(صحيح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *لگاریتم;
    ngx_uint_t        i;
    ngx_چرخه_t      *چرخه, init_چرخه;
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

    ngx_زمان_init();

#إذا (NGX_PCRE)
    ngx_regex_init();
#پایان_اگر

    ngx_pid = ngx_getpid();
    ngx_والد = ngx_getppid();

    لگاریتم = ngx_log_init(ngx_پیشوند, ngx_error_log);
    إذا (لگاریتم == عدم) {
        أرجع 1;
    }

    /* STUB */
#إذا (NGX_OPENSSL)
    ngx_ssl_init(لگاریتم);
#پایان_اگر

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_چرخه, حجم(ngx_چرخه_t));
    init_چرخه.لگاریتم = لگاریتم;
    ngx_چرخه = &init_چرخه;

    init_چرخه.pool = ngx_create_pool(1024, لگاریتم);
    إذا (init_چرخه.pool == عدم) {
        أرجع 1;
    }

    إذا (ngx_save_argv(&init_چرخه, argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_process_options(&init_چرخه) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_os_init(لگاریتم) != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    إذا (ngx_بررسی_چرخهای_32_جدول_init() != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    إذا (ngx_جمع_کن_inherited_sockets(&init_چرخه) != NGX_OK) {
        أرجع 1;
    }

#إذا (NGX_CONTROL_API)
    ngx_control_preinit();
#پایان_اگر

    إذا (ngx_preinit_modules() != NGX_OK) {
        أرجع 1;
    }

    چرخه = ngx_init_چرخه(&init_چرخه);
    إذا (چرخه == عدم) {
        إذا (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_چرخه.conf_file.داده);
        }

        أرجع 1;
    }

    إذا (ngx_test_config) {
        إذا (!ngx_quiet_نما) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           چرخه->conf_file.داده);
        }

        إذا (ngx_dump_config) {
            cd = چرخه->config_dump.elts;

            لكل (i = 0; i < چرخه->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.داده,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].میانگیر;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->آخرین - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        أرجع 0;
    }

    إذا (ngx_signal) {
        أرجع ngx_signal_process(چرخه, ngx_signal);
    }

    ngx_os_وضعیت(چرخه->لگاریتم);

    ngx_چرخه = چرخه;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    إذا (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#إذا !(NGX_WIN32)

    إذا (ngx_init_سیگنالها(چرخه->لگاریتم) != NGX_OK) {
        أرجع 1;
    }

    إذا (!ngx_inherited && ccf->daemon) {
        إذا (ngx_daemon(چرخه->لگاریتم) != NGX_OK) {
            أرجع 1;
        }

        ngx_daemonized = 1;
    }

    إذا (ngx_inherited) {
        ngx_daemonized = 1;
    }

#پایان_اگر

#إذا (NGX_CONTROL_API)
    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, چرخه->لگاریتم, 0,
                      "control API unavailable in single process mode");

    } وإلا {
        إذا (ngx_control_init(ngx_control_addr) != NGX_OK) {
            أرجع 1;
        }
    }
#پایان_اگر

    إذا (ngx_create_pidfile(&ccf->pid, چرخه->لگاریتم) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_log_redirect_stderr(چرخه) != NGX_OK) {
        أرجع 1;
    }

    إذا (لگاریتم->file->fd != ngx_stderr) {
        إذا (ngx_close_file(لگاریتم->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_چرخه(چرخه);

    } وإلا {
        ngx_master_process_چرخه(چرخه);
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
#پایان_اگر
#اگر_تعریف_شده NGX_پیشوند
            "  -p prefix     : set prefix path (default: " NGX_پیشوند ")"
                               NGX_LINEFEED
#وإلا
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#پایان_اگر
            "  -e filename   : set error log file (default: "
#اگر_تعریف_شده NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وإلا
                               NGX_ERROR_LOG_مسیر ")" NGX_LINEFEED
#پایان_اگر
            "  -c filename   : set configuration file (default: " NGX_CONF_مسیر
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    إذا (ngx_show_configure) {

#اگر_تعریف_شده NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#پایان_اگر

#إذا (NGX_SSL)
        إذا (ngx_strcmp(ngx_ssl_نسخه(), OPENSSL_نسخه_متن) == 0) {
            ngx_write_stderr("built with " OPENSSL_نسخه_متن NGX_LINEFEED);
        } وإلا {
            ngx_write_stderr("built with " OPENSSL_نسخه_متن
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_نسخه());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#اگر_تعریف_شده SSL_CTRL_مجموعه_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وإلا
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#پایان_اگر
#پایان_اگر

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساكن ngx_int_t
ngx_جمع_کن_inherited_sockets(ngx_چرخه_t *چرخه)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    إذا (inherited == عدم) {
        أرجع NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, چرخه->لگاریتم, 0,
                  "using inherited sockets from \"%s\"", inherited);

    إذا (ngx_array_init(&چرخه->listening, چرخه->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (p = inherited, v = p; *p; p++) {
        إذا (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            إذا (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_افزودن(&چرخه->listening);
            إذا (ls == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    إذا (v != p) {
        ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    أرجع ngx_مجموعه_inherited_sockets(چرخه);
}


حرف **
ngx_مجموعه_environment(ngx_چرخه_t *چرخه, ngx_uint_t *آخرین)
{
    حرف                **p, **env, *str;
    اندازه_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    إذا (آخرین == عدم && ccf->environment) {
        أرجع ccf->environment;
    }

    var = ccf->env.elts;

    لكل (i = 0; i < ccf->env.nelts; i++) {
        إذا (ngx_strcmp(var[i].داده, "TZ") == 0
            || ngx_strncmp(var[i].داده, "TZ=", 3) == 0)
        {
            اذهب tz_یافت;
        }
    }

    var = ngx_array_افزودن(&ccf->env);
    إذا (var == عدم) {
        أرجع عدم;
    }

    var->len = 2;
    var->داده = (u_char *) "TZ";

    var = ccf->env.elts;

tz_یافت:

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].داده[var[i].len] == '=') {
            n++;
            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].داده, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                اكسر;
            }
        }
    }

    إذا (آخرین) {
        env = ngx_alloc((*آخرین + n + 1) * حجم(حرف *), چرخه->لگاریتم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        *آخرین = n;

    } وإلا {
        cln = ngx_pool_cleanup_جمع_کن(چرخه->pool, 0);
        إذا (cln == عدم) {
            أرجع عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), چرخه->لگاریتم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->داده = env;
    }

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].داده[var[i].len] == '=') {

            إذا (آخرین) {
                env[n++] = (حرف *) var[i].داده;
                استمر;
            }

            cln = ngx_pool_cleanup_جمع_کن(چرخه->pool, 0);
            إذا (cln == عدم) {
                أرجع عدم;
            }

            len = ngx_strlen(var[i].داده) + 1;

            str = ngx_alloc(len, چرخه->لگاریتم);
            إذا (str == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(str, var[i].داده, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->داده = str;

            env[n++] = str;

            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].داده, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                اكسر;
            }
        }
    }

    env[n] = عدم;

    إذا (آخرین == عدم) {
        ccf->environment = env;
        environ = env;
    }

    أرجع env;
}


ساكن فراغ
ngx_cleanup_environment(فراغ *داده)
{
    حرف  **env = داده;

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
ngx_cleanup_environment_variable(فراغ *داده)
{
    حرف  *var = داده;

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
ngx_exec_new_دودویی(ngx_چرخه_t *چرخه, حرف *ثابت *argv)
{
    حرف             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, حجم(ngx_exec_ctx_t));

    ctx.مسیر = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_مجموعه_environment(چرخه, &n);
    إذا (env == عدم) {
        أرجع NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + چرخه->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    چرخه->لگاریتم);
    إذا (var == عدم) {
        ngx_free(env);
        أرجع NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = چرخه->listening.elts;
    لكل (i = 0; i < چرخه->listening.nelts; i++) {
        إذا (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#إذا (NGX_SETPROCTITLE_استفاده_میکند_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#پایان_اگر

#إذا (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    إذا (env[n] != عدم) {
        n++;
    }
#پایان_اگر

    env[n] = عدم;

#إذا (NGX_DEBUG)
    {
    حرف  **e;
    لكل (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, چرخه->لگاریتم, 0, "env: %s", *e);
    }
    }
#پایان_اگر

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    إذا (ngx_rename_file(ccf->pid.داده, ccf->oldpid.داده) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.داده, ccf->oldpid.داده, argv[0]);

        ngx_free(env);
        ngx_free(var);

        أرجع NGX_INVALID_PID;
    }

    pid = ngx_execute(چرخه, &ctx);

    إذا (pid == NGX_INVALID_PID) {
        إذا (ngx_rename_file(ccf->oldpid.داده, ccf->pid.داده)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.داده, ccf->pid.داده, argv[0]);
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
                ngx_quiet_نما = 1;
                اكسر;

            حالة 'p':
                إذا (*p) {
                    ngx_پیشوند = p;
                    اذهب بعدی;
                }

                إذا (argv[++i]) {
                    ngx_پیشوند = (u_char *) argv[i];
                    اذهب بعدی;
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

                اذهب بعدی;

            حالة 'c':
                إذا (*p) {
                    ngx_conf_file = p;
                    اذهب بعدی;
                }

                إذا (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب بعدی;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                أرجع NGX_ERROR;

            حالة 'g':
                إذا (*p) {
                    ngx_conf_params = p;
                    اذهب بعدی;
                }

                إذا (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب بعدی;
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
                    اذهب بعدی;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                أرجع NGX_ERROR;

#إذا (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                إذا (*p) {
                    ngx_control_addr = p;
                    اذهب بعدی;
                }

                إذا (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب بعدی;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                أرجع NGX_ERROR;
#پایان_اگر

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                أرجع NGX_ERROR;
            }
        }

    بعدی:

        استمر;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_چرخه_t *چرخه, صحيح argc, حرف *ثابت *argv)
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

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), چرخه->لگاریتم);
    إذا (ngx_argv == عدم) {
        أرجع NGX_ERROR;
    }

    لكل (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, چرخه->لگاریتم);
        إذا (ngx_argv[i] == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#پایان_اگر

    ngx_os_environ = environ;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_چرخه_t *چرخه)
{
    u_char  *p;
    اندازه_t   len;

    إذا (ngx_پیشوند) {
        len = ngx_strlen(ngx_پیشوند);
        p = ngx_پیشوند;

        إذا (len && !ngx_مسیر_separator(p[len - 1])) {
            p = ngx_pnalloc(چرخه->pool, len + 1);
            إذا (p == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(p, ngx_پیشوند, len);
            p[len++] = '/';
        }

        چرخه->conf_پیشوند.len = len;
        چرخه->conf_پیشوند.داده = p;
        چرخه->پیشوند.len = len;
        چرخه->پیشوند.داده = p;

    } وإلا {

#اگر_تعریف_نشده NGX_پیشوند

        p = ngx_pnalloc(چرخه->pool, NGX_MAX_مسیر);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        إذا (ngx_getcwd(p, NGX_MAX_مسیر) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            أرجع NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        چرخه->conf_پیشوند.len = len;
        چرخه->conf_پیشوند.داده = p;
        چرخه->پیشوند.len = len;
        چرخه->پیشوند.داده = p;

#وإلا

#اگر_تعریف_شده NGX_CONF_پیشوند
        ngx_str_مجموعه(&چرخه->conf_پیشوند, NGX_CONF_پیشوند);
#وإلا
        ngx_str_مجموعه(&چرخه->conf_پیشوند, NGX_پیشوند);
#پایان_اگر
        ngx_str_مجموعه(&چرخه->پیشوند, NGX_پیشوند);

#پایان_اگر
    }

    إذا (ngx_conf_file) {
        چرخه->conf_file.len = ngx_strlen(ngx_conf_file);
        چرخه->conf_file.داده = ngx_conf_file;

    } وإلا {
        ngx_str_مجموعه(&چرخه->conf_file, NGX_CONF_مسیر);
    }

    إذا (ngx_conf_پر_name(چرخه, &چرخه->conf_file, 0) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    لكل (p = چرخه->conf_file.داده + چرخه->conf_file.len - 1;
         p > چرخه->conf_file.داده;
         p--)
    {
        إذا (ngx_مسیر_separator(*p)) {
            چرخه->conf_پیشوند.len = p - چرخه->conf_file.داده + 1;
            چرخه->conf_پیشوند.داده = چرخه->conf_file.داده;
            اكسر;
        }
    }

    إذا (ngx_error_log) {
        چرخه->error_log.len = ngx_strlen(ngx_error_log);
        چرخه->error_log.داده = ngx_error_log;

    } وإلا {
        ngx_str_مجموعه(&چرخه->error_log, NGX_ERROR_LOG_مسیر);
    }

    إذا (ngx_conf_params) {
        چرخه->conf_param.len = ngx_strlen(ngx_conf_params);
        چرخه->conf_param.داده = ngx_conf_params;
    }

    إذا (ngx_test_config) {
        چرخه->لگاریتم->log_level = NGX_LOG_INFO;
    }

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_چرخه_t *چرخه)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(چرخه->pool, حجم(ngx_core_conf_t));
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

    ccf->کارگر_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->گروه = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    إذا (ngx_array_init(&ccf->env, چرخه->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_چرخه_t *چرخه, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_مقدار(ccf->daemon, 1);
    ngx_conf_init_مقدار(ccf->master, 1);
    ngx_conf_init_msec_مقدار(ccf->timer_resolution, 0);
    ngx_conf_init_msec_مقدار(ccf->shutdown_timeout, 0);

    ngx_conf_init_مقدار(ccf->کارگر_processes, 1);
    ngx_conf_init_مقدار(ccf->debug_points, 0);

#إذا (NGX_HAVE_CPU_AFFINITY)

    إذا (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->کارگر_processes)
    {
        ngx_log_error(NGX_LOG_WARN, چرخه->لگاریتم, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#پایان_اگر


    إذا (ccf->pid.len == 0) {
        ngx_str_مجموعه(&ccf->pid, NGX_PID_مسیر);
    }

    إذا (ngx_conf_پر_name(چرخه, &ccf->pid, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.داده = ngx_pnalloc(چرخه->pool, ccf->oldpid.len);
    إذا (ccf->oldpid.داده == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.داده, ccf->pid.داده, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    إذا (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        إذا (ngx_conf_پر_name(چرخه, &ccf->working_directory, 0) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_file_info(ccf->working_directory.داده, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.داده);
            أرجع NGX_CONF_ERROR;
        }

        إذا (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.داده);
            أرجع NGX_CONF_ERROR;
        }
    }


#إذا !(NGX_WIN32)

    إذا (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية گروه   *grp;
        بنية passwd  *pwd;

        ngx_مجموعه_errno(0);
        pwd = getpwnam(NGX_USER);
        إذا (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_مجموعه_errno(0);
        grp = getgrnam(NGX_گروه);
        إذا (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          "getgrnam(\"" NGX_گروه "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->گروه = grp->gr_gid;
    }


    إذا (ccf->قفل_file.len == 0) {
        ngx_str_مجموعه(&ccf->قفل_file, NGX_قفل_مسیر);
    }

    إذا (ngx_conf_پر_name(چرخه, &ccf->قفل_file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    {
    ngx_str_t  قفل_file;

    قفل_file = چرخه->old_چرخه->قفل_file;

    إذا (قفل_file.len) {
        قفل_file.len--;

        إذا (ccf->قفل_file.len != قفل_file.len
            || ngx_strncmp(ccf->قفل_file.داده, قفل_file.داده, قفل_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        چرخه->قفل_file.len = قفل_file.len + 1;
        قفل_file.len += حجم(".accept");

        چرخه->قفل_file.داده = ngx_pstrdup(چرخه->pool, &قفل_file);
        إذا (چرخه->قفل_file.داده == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا {
        چرخه->قفل_file.len = ccf->قفل_file.len + 1;
        چرخه->قفل_file.داده = ngx_pnalloc(چرخه->pool,
                                      ccf->قفل_file.len + حجم(".accept"));
        إذا (چرخه->قفل_file.داده == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(چرخه->قفل_file.داده, ccf->قفل_file.داده,
                              ccf->قفل_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#پایان_اگر

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_مجموعه_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    أرجع NGX_CONF_OK;

#وإلا

    ngx_core_conf_t  *ccf = conf;

    حرف             *گروه;
    بنية passwd    *pwd;
    بنية گروه     *grp;
    ngx_str_t        *مقدار;

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

    مقدار = cf->آرگومانها->elts;

    ccf->username = (حرف *) مقدار[1].داده;

    ngx_مجموعه_errno(0);
    pwd = getpwnam((ثابت حرف *) مقدار[1].داده);
    إذا (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", مقدار[1].داده);
        أرجع NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    گروه = (حرف *) ((cf->آرگومانها->nelts == 2) ? مقدار[1].داده : مقدار[2].داده);

    ngx_مجموعه_errno(0);
    grp = getgrnam(گروه);
    إذا (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", گروه);
        أرجع NGX_CONF_ERROR;
    }

    ccf->گروه = grp->gr_gid;

    أرجع NGX_CONF_OK;

#پایان_اگر
}


ساكن حرف *
ngx_مجموعه_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *مقدار, *var;
    ngx_uint_t   i;

    var = ngx_array_افزودن(&ccf->env);
    إذا (var == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    مقدار = cf->آرگومانها->elts;
    *var = مقدار[1];

    لكل (i = 0; i < مقدار[1].len; i++) {

        إذا (مقدار[1].داده[i] == '=') {

            var->len = i;

            أرجع NGX_CONF_OK;
        }
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_مجموعه_اولویت(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *مقدار;
    ngx_uint_t        n, minus;

    إذا (ccf->اولویت != 0) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    إذا (مقدار[1].داده[0] == '-') {
        n = 1;
        minus = 1;

    } وإلا إذا (مقدار[1].داده[0] == '+') {
        n = 1;
        minus = 0;

    } وإلا {
        n = 0;
        minus = 0;
    }

    ccf->اولویت = ngx_atoi(&مقدار[1].داده[n], مقدار[1].len - n);
    إذا (ccf->اولویت == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (minus) {
        ccf->اولویت = -ccf->اولویت;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_مجموعه_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *مقدار;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *ماسک;

    إذا (ccf->cpu_affinity) {
        أرجع "is duplicate";
    }

    ماسک = ngx_palloc(cf->pool, (cf->آرگومانها->nelts - 1) * حجم(ngx_cpuset_t));
    إذا (ماسک == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->آرگومانها->nelts - 1;
    ccf->cpu_affinity = ماسک;

    مقدار = cf->آرگومانها->elts;

    إذا (ngx_strcmp(مقدار[1].داده, "auto") == 0) {

        إذا (cf->آرگومانها->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            أرجع NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&ماسک[0]);
        لكل (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_مجموعه(i, &ماسک[0]);
        }

        n = 2;

    } وإلا {
        n = 1;
    }

    لكل ( /* void */ ; n < cf->آرگومانها->nelts; n++) {

        إذا (مقدار[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            أرجع NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&ماسک[n - 1]);

        لكل (p = مقدار[n].داده + مقدار[n].len - 1;
             p >= مقدار[n].داده;
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
                CPU_مجموعه(i - 1, &ماسک[n - 1]);
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
#پایان_اگر

    أرجع NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *ماسک;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  نتیجه;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_چرخه->conf_ctx,
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

        CPU_ZERO(&نتیجه);
        CPU_مجموعه(i % CPU_SETSIZE, &نتیجه);

        أرجع &نتیجه;
    }

    إذا (ccf->cpu_affinity_n > n) {
        أرجع &ccf->cpu_affinity[n];
    }

    أرجع &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وإلا

    أرجع عدم;

#پایان_اگر
}


ساكن حرف *
ngx_مجموعه_کارگر_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *مقدار;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    إذا (ccf->کارگر_processes != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    إذا (ngx_strcmp(مقدار[1].داده, "auto") == 0) {
        ccf->کارگر_processes = ngx_ncpu;
        أرجع NGX_CONF_OK;
    }

    ccf->کارگر_processes = ngx_atoi(مقدار[1].داده, مقدار[1].len);

    إذا (ccf->کارگر_processes == NGX_ERROR) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_بارگیری_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **نامها, **ترتیب;
    ngx_str_t           *مقدار, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    إذا (cf->چرخه->modules_استفاده_شده) {
        أرجع "is specified too late";
    }

    مقدار = cf->آرگومانها->elts;

    file = مقدار[1];

    إذا (ngx_conf_پر_name(cf->چرخه, &file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_جمع_کن(cf->چرخه->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.داده);
    إذا (handle == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.داده, ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->داده = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    إذا (modules == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &مقدار[1], "ngx_modules", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    نامها = ngx_dlsym(handle, "ngx_module_names");
    إذا (نامها == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &مقدار[1], "ngx_module_names", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    ترتیب = ngx_dlsym(handle, "ngx_module_order");

    لكل (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = نامها[i];

        إذا (ngx_جمع_کن_module(cf, &file, module, ترتیب) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0, "module: %s i:%ui",
                       module->name, module->شاخص);
    }

    أرجع NGX_CONF_OK;

#وإلا

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    أرجع NGX_CONF_ERROR;

#پایان_اگر
}


#إذا (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *داده)
{
    فراغ  *handle = داده;

    إذا (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#پایان_اگر
