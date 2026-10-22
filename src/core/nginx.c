
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


ساكن فراغ ngx_show_نسخو_info(فراغ);
ساكن ngx_int_t ngx_جوڙيو_inherited_sockets(ngx_چڪر_t *چڪر);
ساكن فراغ ngx_cleanup_environment(فراغ *معلومات);
ساكن فراغ ngx_cleanup_environment_variable(فراغ *معلومات);
ساكن ngx_int_t ngx_get_options(صحيح argc, حرف *ثابت *argv);
ساكن ngx_int_t ngx_process_options(ngx_چڪر_t *چڪر);
ساكن ngx_int_t ngx_save_argv(ngx_چڪر_t *چڪر, صحيح argc, حرف *ثابت *argv);
ساكن فراغ *ngx_core_module_create_conf(ngx_چڪر_t *چڪر);
ساكن حرف *ngx_core_module_init_conf(ngx_چڪر_t *چڪر, فراغ *conf);
ساكن حرف *ngx_سيٽ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سيٽ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سيٽ_ترجيح(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_سيٽ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_سيٽ_ڪم_ڪندڙ_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_ڀريو_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
#إذا (NGX_HAVE_DLOPEN)
ساكن فراغ ngx_unload_module(فراغ *معلومات);
#جيڪڏهن_پورو


ساكن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساكن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_FLAG,
      ngx_conf_سيٽ_flag_جڳهه,
      0,
      offsetof(ngx_core_conf_t, daemon),
      عدم },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_FLAG,
      ngx_conf_سيٽ_flag_جڳهه,
      0,
      offsetof(ngx_core_conf_t, master),
      عدم },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_msec_جڳهه,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      عدم },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_str_جڳهه,
      0,
      offsetof(ngx_core_conf_t, pid),
      عدم },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_str_جڳهه,
      0,
      offsetof(ngx_core_conf_t, تالو_file),
      عدم },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_سيٽ_ڪم_ڪندڙ_processes,
      0,
      0,
      عدم },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_enum_جڳهه,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE12,
      ngx_سيٽ_user,
      0,
      0,
      عدم },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_سيٽ_ترجيح,
      0,
      0,
      عدم },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_1_وڌيڪ,
      ngx_سيٽ_cpu_affinity,
      0,
      0,
      عدم },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_انگ_جڳهه,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      عدم },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_off_جڳهه,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      عدم },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_msec_جڳهه,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      عدم },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_conf_سيٽ_str_جڳهه,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      عدم },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_سيٽ_env,
      0,
      0,
      عدم },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_سڌو_CONF|NGX_CONF_TAKE1,
      ngx_ڀريو_module,
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
ساكن ngx_uint_t   ngx_show_نسخو;
ساكن ngx_uint_t   ngx_show_configure;
ساكن u_char      *ngx_اڳياڙي;
ساكن u_char      *ngx_error_log;
ساكن u_char      *ngx_conf_file;
ساكن u_char      *ngx_conf_params;
#إذا (NGX_CONTROL_API)
ساكن u_char      *ngx_control_addr;
#جيڪڏهن_پورو
ساكن حرف        *ngx_signal;


ساكن حرف **ngx_os_environ;


صحيح ngx_cdecl
مکيه(صحيح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *لاگرٿم;
    ngx_uint_t        i;
    ngx_چڪر_t      *چڪر, init_چڪر;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    إذا (ngx_strerror_init() != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_get_options(argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_show_نسخو) {
        ngx_show_نسخو_info();

        إذا (!ngx_test_config) {
            أرجع 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_وقت_init();

#إذا (NGX_PCRE)
    ngx_regex_init();
#جيڪڏهن_پورو

    ngx_pid = ngx_getpid();
    ngx_والد = ngx_getppid();

    لاگرٿم = ngx_log_init(ngx_اڳياڙي, ngx_error_log);
    إذا (لاگرٿم == عدم) {
        أرجع 1;
    }

    /* STUB */
#إذا (NGX_OPENSSL)
    ngx_ssl_init(لاگرٿم);
#جيڪڏهن_پورو

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_چڪر, حجم(ngx_چڪر_t));
    init_چڪر.لاگرٿم = لاگرٿم;
    ngx_چڪر = &init_چڪر;

    init_چڪر.pool = ngx_create_pool(1024, لاگرٿم);
    إذا (init_چڪر.pool == عدم) {
        أرجع 1;
    }

    إذا (ngx_save_argv(&init_چڪر, argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_process_options(&init_چڪر) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_os_init(لاگرٿم) != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    إذا (ngx_چڪري_جاچ_32_جدول_init() != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    إذا (ngx_جوڙيو_inherited_sockets(&init_چڪر) != NGX_OK) {
        أرجع 1;
    }

#إذا (NGX_CONTROL_API)
    ngx_control_preinit();
#جيڪڏهن_پورو

    إذا (ngx_preinit_modules() != NGX_OK) {
        أرجع 1;
    }

    چڪر = ngx_init_چڪر(&init_چڪر);
    إذا (چڪر == عدم) {
        إذا (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_چڪر.conf_file.معلومات);
        }

        أرجع 1;
    }

    إذا (ngx_test_config) {
        إذا (!ngx_quiet_گهڻائي) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           چڪر->conf_file.معلومات);
        }

        إذا (ngx_dump_config) {
            cd = چڪر->config_dump.elts;

            لكل (i = 0; i < چڪر->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.معلومات,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].بفر;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->آخري - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        أرجع 0;
    }

    إذا (ngx_signal) {
        أرجع ngx_signal_process(چڪر, ngx_signal);
    }

    ngx_os_حالت(چڪر->لاگرٿم);

    ngx_چڪر = چڪر;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    إذا (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#إذا !(NGX_WIN32)

    إذا (ngx_init_سگنل(چڪر->لاگرٿم) != NGX_OK) {
        أرجع 1;
    }

    إذا (!ngx_inherited && ccf->daemon) {
        إذا (ngx_daemon(چڪر->لاگرٿم) != NGX_OK) {
            أرجع 1;
        }

        ngx_daemonized = 1;
    }

    إذا (ngx_inherited) {
        ngx_daemonized = 1;
    }

#جيڪڏهن_پورو

#إذا (NGX_CONTROL_API)
    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, چڪر->لاگرٿم, 0,
                      "control API unavailable in single process mode");

    } وإلا {
        إذا (ngx_control_init(ngx_control_addr) != NGX_OK) {
            أرجع 1;
        }
    }
#جيڪڏهن_پورو

    إذا (ngx_create_pidfile(&ccf->pid, چڪر->لاگرٿم) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_log_redirect_stderr(چڪر) != NGX_OK) {
        أرجع 1;
    }

    إذا (لاگرٿم->file->fd != ngx_stderr) {
        إذا (ngx_close_file(لاگرٿم->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_چڪر(چڪر);

    } وإلا {
        ngx_master_process_چڪر(چڪر);
    }

    أرجع 0;
}


ساكن فراغ
ngx_show_نسخو_info(فراغ)
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
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل NGX_اڳياڙي
            "  -p prefix     : set prefix path (default: " NGX_اڳياڙي ")"
                               NGX_LINEFEED
#وإلا
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#جيڪڏهن_پورو
            "  -e filename   : set error log file (default: "
#جيڪڏهن_وصف_ٿيل NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وإلا
                               NGX_ERROR_LOG_رستو ")" NGX_LINEFEED
#جيڪڏهن_پورو
            "  -c filename   : set configuration file (default: " NGX_CONF_رستو
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    إذا (ngx_show_configure) {

#جيڪڏهن_وصف_ٿيل NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#جيڪڏهن_پورو

#إذا (NGX_SSL)
        إذا (ngx_strcmp(ngx_ssl_نسخو(), OPENSSL_نسخو_متن) == 0) {
            ngx_write_stderr("built with " OPENSSL_نسخو_متن NGX_LINEFEED);
        } وإلا {
            ngx_write_stderr("built with " OPENSSL_نسخو_متن
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_نسخو());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#جيڪڏهن_وصف_ٿيل SSL_CTRL_سيٽ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وإلا
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#جيڪڏهن_پورو
#جيڪڏهن_پورو

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساكن ngx_int_t
ngx_جوڙيو_inherited_sockets(ngx_چڪر_t *چڪر)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    إذا (inherited == عدم) {
        أرجع NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, چڪر->لاگرٿم, 0,
                  "using inherited sockets from \"%s\"", inherited);

    إذا (ngx_array_init(&چڪر->listening, چڪر->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (p = inherited, v = p; *p; p++) {
        إذا (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            إذا (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_ڌڪو(&چڪر->listening);
            إذا (ls == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    إذا (v != p) {
        ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    أرجع ngx_سيٽ_inherited_sockets(چڪر);
}


حرف **
ngx_سيٽ_environment(ngx_چڪر_t *چڪر, ngx_uint_t *آخري)
{
    حرف                **p, **env, *str;
    سائيز_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    إذا (آخري == عدم && ccf->environment) {
        أرجع ccf->environment;
    }

    var = ccf->env.elts;

    لكل (i = 0; i < ccf->env.nelts; i++) {
        إذا (ngx_strcmp(var[i].معلومات, "TZ") == 0
            || ngx_strncmp(var[i].معلومات, "TZ=", 3) == 0)
        {
            اذهب tz_مليو;
        }
    }

    var = ngx_array_ڌڪو(&ccf->env);
    إذا (var == عدم) {
        أرجع عدم;
    }

    var->len = 2;
    var->معلومات = (u_char *) "TZ";

    var = ccf->env.elts;

tz_مليو:

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

    إذا (آخري) {
        env = ngx_alloc((*آخري + n + 1) * حجم(حرف *), چڪر->لاگرٿم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        *آخري = n;

    } وإلا {
        cln = ngx_pool_cleanup_جوڙيو(چڪر->pool, 0);
        إذا (cln == عدم) {
            أرجع عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), چڪر->لاگرٿم);
        إذا (env == عدم) {
            أرجع عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->معلومات = env;
    }

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].معلومات[var[i].len] == '=') {

            إذا (آخري) {
                env[n++] = (حرف *) var[i].معلومات;
                استمر;
            }

            cln = ngx_pool_cleanup_جوڙيو(چڪر->pool, 0);
            إذا (cln == عدم) {
                أرجع عدم;
            }

            len = ngx_strlen(var[i].معلومات) + 1;

            str = ngx_alloc(len, چڪر->لاگرٿم);
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

    إذا (آخري == عدم) {
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
ngx_exec_new_ٻٽو(ngx_چڪر_t *چڪر, حرف *ثابت *argv)
{
    حرف             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, حجم(ngx_exec_ctx_t));

    ctx.رستو = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_سيٽ_environment(چڪر, &n);
    إذا (env == عدم) {
        أرجع NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + چڪر->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    چڪر->لاگرٿم);
    إذا (var == عدم) {
        ngx_free(env);
        أرجع NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = چڪر->listening.elts;
    لكل (i = 0; i < چڪر->listening.nelts; i++) {
        إذا (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#إذا (NGX_SETPROCTITLE_استعمال_ڪري_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#جيڪڏهن_پورو

#إذا (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    إذا (env[n] != عدم) {
        n++;
    }
#جيڪڏهن_پورو

    env[n] = عدم;

#إذا (NGX_DEBUG)
    {
    حرف  **e;
    لكل (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, چڪر->لاگرٿم, 0, "env: %s", *e);
    }
    }
#جيڪڏهن_پورو

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    إذا (ngx_rename_file(ccf->pid.معلومات, ccf->oldpid.معلومات) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.معلومات, ccf->oldpid.معلومات, argv[0]);

        ngx_free(env);
        ngx_free(var);

        أرجع NGX_INVALID_PID;
    }

    pid = ngx_execute(چڪر, &ctx);

    إذا (pid == NGX_INVALID_PID) {
        إذا (ngx_rename_file(ccf->oldpid.معلومات, ccf->pid.معلومات)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
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
                ngx_show_نسخو = 1;
                ngx_show_help = 1;
                اكسر;

            حالة 'v':
                ngx_show_نسخو = 1;
                اكسر;

            حالة 'V':
                ngx_show_نسخو = 1;
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
                ngx_quiet_گهڻائي = 1;
                اكسر;

            حالة 'p':
                إذا (*p) {
                    ngx_اڳياڙي = p;
                    اذهب ايندڙ;
                }

                إذا (argv[++i]) {
                    ngx_اڳياڙي = (u_char *) argv[i];
                    اذهب ايندڙ;
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

                اذهب ايندڙ;

            حالة 'c':
                إذا (*p) {
                    ngx_conf_file = p;
                    اذهب ايندڙ;
                }

                إذا (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب ايندڙ;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                أرجع NGX_ERROR;

            حالة 'g':
                إذا (*p) {
                    ngx_conf_params = p;
                    اذهب ايندڙ;
                }

                إذا (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب ايندڙ;
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
                    اذهب ايندڙ;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                أرجع NGX_ERROR;

#إذا (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                إذا (*p) {
                    ngx_control_addr = p;
                    اذهب ايندڙ;
                }

                إذا (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب ايندڙ;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                أرجع NGX_ERROR;
#جيڪڏهن_پورو

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                أرجع NGX_ERROR;
            }
        }

    ايندڙ:

        استمر;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_چڪر_t *چڪر, صحيح argc, حرف *ثابت *argv)
{
#إذا (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#وإلا
    سائيز_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), چڪر->لاگرٿم);
    إذا (ngx_argv == عدم) {
        أرجع NGX_ERROR;
    }

    لكل (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, چڪر->لاگرٿم);
        إذا (ngx_argv[i] == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#جيڪڏهن_پورو

    ngx_os_environ = environ;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_چڪر_t *چڪر)
{
    u_char  *p;
    سائيز_t   len;

    إذا (ngx_اڳياڙي) {
        len = ngx_strlen(ngx_اڳياڙي);
        p = ngx_اڳياڙي;

        إذا (len && !ngx_رستو_separator(p[len - 1])) {
            p = ngx_pnalloc(چڪر->pool, len + 1);
            إذا (p == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(p, ngx_اڳياڙي, len);
            p[len++] = '/';
        }

        چڪر->conf_اڳياڙي.len = len;
        چڪر->conf_اڳياڙي.معلومات = p;
        چڪر->اڳياڙي.len = len;
        چڪر->اڳياڙي.معلومات = p;

    } وإلا {

#جيڪڏهن_وصف_نه_ٿيل NGX_اڳياڙي

        p = ngx_pnalloc(چڪر->pool, NGX_MAX_رستو);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        إذا (ngx_getcwd(p, NGX_MAX_رستو) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            أرجع NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        چڪر->conf_اڳياڙي.len = len;
        چڪر->conf_اڳياڙي.معلومات = p;
        چڪر->اڳياڙي.len = len;
        چڪر->اڳياڙي.معلومات = p;

#وإلا

#جيڪڏهن_وصف_ٿيل NGX_CONF_اڳياڙي
        ngx_str_سيٽ(&چڪر->conf_اڳياڙي, NGX_CONF_اڳياڙي);
#وإلا
        ngx_str_سيٽ(&چڪر->conf_اڳياڙي, NGX_اڳياڙي);
#جيڪڏهن_پورو
        ngx_str_سيٽ(&چڪر->اڳياڙي, NGX_اڳياڙي);

#جيڪڏهن_پورو
    }

    إذا (ngx_conf_file) {
        چڪر->conf_file.len = ngx_strlen(ngx_conf_file);
        چڪر->conf_file.معلومات = ngx_conf_file;

    } وإلا {
        ngx_str_سيٽ(&چڪر->conf_file, NGX_CONF_رستو);
    }

    إذا (ngx_conf_ڀريل_name(چڪر, &چڪر->conf_file, 0) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    لكل (p = چڪر->conf_file.معلومات + چڪر->conf_file.len - 1;
         p > چڪر->conf_file.معلومات;
         p--)
    {
        إذا (ngx_رستو_separator(*p)) {
            چڪر->conf_اڳياڙي.len = p - چڪر->conf_file.معلومات + 1;
            چڪر->conf_اڳياڙي.معلومات = چڪر->conf_file.معلومات;
            اكسر;
        }
    }

    إذا (ngx_error_log) {
        چڪر->error_log.len = ngx_strlen(ngx_error_log);
        چڪر->error_log.معلومات = ngx_error_log;

    } وإلا {
        ngx_str_سيٽ(&چڪر->error_log, NGX_ERROR_LOG_رستو);
    }

    إذا (ngx_conf_params) {
        چڪر->conf_param.len = ngx_strlen(ngx_conf_params);
        چڪر->conf_param.معلومات = ngx_conf_params;
    }

    إذا (ngx_test_config) {
        چڪر->لاگرٿم->log_level = NGX_LOG_INFO;
    }

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_چڪر_t *چڪر)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(چڪر->pool, حجم(ngx_core_conf_t));
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

    ccf->ڪم_ڪندڙ_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->گروهه = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    إذا (ngx_array_init(&ccf->env, چڪر->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_چڪر_t *چڪر, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_قدر(ccf->daemon, 1);
    ngx_conf_init_قدر(ccf->master, 1);
    ngx_conf_init_msec_قدر(ccf->timer_resolution, 0);
    ngx_conf_init_msec_قدر(ccf->shutdown_timeout, 0);

    ngx_conf_init_قدر(ccf->ڪم_ڪندڙ_processes, 1);
    ngx_conf_init_قدر(ccf->debug_points, 0);

#إذا (NGX_HAVE_CPU_AFFINITY)

    إذا (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->ڪم_ڪندڙ_processes)
    {
        ngx_log_error(NGX_LOG_WARN, چڪر->لاگرٿم, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#جيڪڏهن_پورو


    إذا (ccf->pid.len == 0) {
        ngx_str_سيٽ(&ccf->pid, NGX_PID_رستو);
    }

    إذا (ngx_conf_ڀريل_name(چڪر, &ccf->pid, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.معلومات = ngx_pnalloc(چڪر->pool, ccf->oldpid.len);
    إذا (ccf->oldpid.معلومات == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.معلومات, ccf->pid.معلومات, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    إذا (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        إذا (ngx_conf_ڀريل_name(چڪر, &ccf->working_directory, 0) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_file_info(ccf->working_directory.معلومات, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.معلومات);
            أرجع NGX_CONF_ERROR;
        }

        إذا (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.معلومات);
            أرجع NGX_CONF_ERROR;
        }
    }


#إذا !(NGX_WIN32)

    إذا (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية گروهه   *grp;
        بنية passwd  *pwd;

        ngx_سيٽ_errno(0);
        pwd = getpwnam(NGX_USER);
        إذا (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_سيٽ_errno(0);
        grp = getgrnam(NGX_گروهه);
        إذا (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          "getgrnam(\"" NGX_گروهه "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->گروهه = grp->gr_gid;
    }


    إذا (ccf->تالو_file.len == 0) {
        ngx_str_سيٽ(&ccf->تالو_file, NGX_تالو_رستو);
    }

    إذا (ngx_conf_ڀريل_name(چڪر, &ccf->تالو_file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    {
    ngx_str_t  تالو_file;

    تالو_file = چڪر->old_چڪر->تالو_file;

    إذا (تالو_file.len) {
        تالو_file.len--;

        إذا (ccf->تالو_file.len != تالو_file.len
            || ngx_strncmp(ccf->تالو_file.معلومات, تالو_file.معلومات, تالو_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        چڪر->تالو_file.len = تالو_file.len + 1;
        تالو_file.len += حجم(".accept");

        چڪر->تالو_file.معلومات = ngx_pstrdup(چڪر->pool, &تالو_file);
        إذا (چڪر->تالو_file.معلومات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا {
        چڪر->تالو_file.len = ccf->تالو_file.len + 1;
        چڪر->تالو_file.معلومات = ngx_pnalloc(چڪر->pool,
                                      ccf->تالو_file.len + حجم(".accept"));
        إذا (چڪر->تالو_file.معلومات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(چڪر->تالو_file.معلومات, ccf->تالو_file.معلومات,
                              ccf->تالو_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#جيڪڏهن_پورو

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سيٽ_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    أرجع NGX_CONF_OK;

#وإلا

    ngx_core_conf_t  *ccf = conf;

    حرف             *گروهه;
    بنية passwd    *pwd;
    بنية گروهه     *grp;
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

    قدر = cf->دليلون->elts;

    ccf->username = (حرف *) قدر[1].معلومات;

    ngx_سيٽ_errno(0);
    pwd = getpwnam((ثابت حرف *) قدر[1].معلومات);
    إذا (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", قدر[1].معلومات);
        أرجع NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    گروهه = (حرف *) ((cf->دليلون->nelts == 2) ? قدر[1].معلومات : قدر[2].معلومات);

    ngx_سيٽ_errno(0);
    grp = getgrnam(گروهه);
    إذا (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", گروهه);
        أرجع NGX_CONF_ERROR;
    }

    ccf->گروهه = grp->gr_gid;

    أرجع NGX_CONF_OK;

#جيڪڏهن_پورو
}


ساكن حرف *
ngx_سيٽ_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *قدر, *var;
    ngx_uint_t   i;

    var = ngx_array_ڌڪو(&ccf->env);
    إذا (var == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دليلون->elts;
    *var = قدر[1];

    لكل (i = 0; i < قدر[1].len; i++) {

        إذا (قدر[1].معلومات[i] == '=') {

            var->len = i;

            أرجع NGX_CONF_OK;
        }
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سيٽ_ترجيح(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *قدر;
    ngx_uint_t        n, minus;

    إذا (ccf->ترجيح != 0) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    إذا (قدر[1].معلومات[0] == '-') {
        n = 1;
        minus = 1;

    } وإلا إذا (قدر[1].معلومات[0] == '+') {
        n = 1;
        minus = 0;

    } وإلا {
        n = 0;
        minus = 0;
    }

    ccf->ترجيح = ngx_atoi(&قدر[1].معلومات[n], قدر[1].len - n);
    إذا (ccf->ترجيح == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (minus) {
        ccf->ترجيح = -ccf->ترجيح;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_سيٽ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
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

    نقاب = ngx_palloc(cf->pool, (cf->دليلون->nelts - 1) * حجم(ngx_cpuset_t));
    إذا (نقاب == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->دليلون->nelts - 1;
    ccf->cpu_affinity = نقاب;

    قدر = cf->دليلون->elts;

    إذا (ngx_strcmp(قدر[1].معلومات, "auto") == 0) {

        إذا (cf->دليلون->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            أرجع NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&نقاب[0]);
        لكل (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_سيٽ(i, &نقاب[0]);
        }

        n = 2;

    } وإلا {
        n = 1;
    }

    لكل ( /* void */ ; n < cf->دليلون->nelts; n++) {

        إذا (قدر[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            أرجع NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&نقاب[n - 1]);

        لكل (p = قدر[n].معلومات + قدر[n].len - 1;
             p >= قدر[n].معلومات;
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
                CPU_سيٽ(i - 1, &نقاب[n - 1]);
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
#جيڪڏهن_پورو

    أرجع NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *نقاب;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  نتيجو;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_چڪر->conf_ctx,
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

        CPU_ZERO(&نتيجو);
        CPU_سيٽ(i % CPU_SETSIZE, &نتيجو);

        أرجع &نتيجو;
    }

    إذا (ccf->cpu_affinity_n > n) {
        أرجع &ccf->cpu_affinity[n];
    }

    أرجع &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وإلا

    أرجع عدم;

#جيڪڏهن_پورو
}


ساكن حرف *
ngx_سيٽ_ڪم_ڪندڙ_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *قدر;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    إذا (ccf->ڪم_ڪندڙ_processes != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    إذا (ngx_strcmp(قدر[1].معلومات, "auto") == 0) {
        ccf->ڪم_ڪندڙ_processes = ngx_ncpu;
        أرجع NGX_CONF_OK;
    }

    ccf->ڪم_ڪندڙ_processes = ngx_atoi(قدر[1].معلومات, قدر[1].len);

    إذا (ccf->ڪم_ڪندڙ_processes == NGX_ERROR) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_ڀريو_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **نالا, **ترتيب;
    ngx_str_t           *قدر, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    إذا (cf->چڪر->modules_استعمال_ٿيل) {
        أرجع "is specified too late";
    }

    قدر = cf->دليلون->elts;

    file = قدر[1];

    إذا (ngx_conf_ڀريل_name(cf->چڪر, &file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_جوڙيو(cf->چڪر->pool, 0);
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
                           &قدر[1], "ngx_modules", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    نالا = ngx_dlsym(handle, "ngx_module_names");
    إذا (نالا == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_module_names", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    ترتيب = ngx_dlsym(handle, "ngx_module_order");

    لكل (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = نالا[i];

        إذا (ngx_جوڙيو_module(cf, &file, module, ترتيب) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0, "module: %s i:%ui",
                       module->name, module->اشاريو);
    }

    أرجع NGX_CONF_OK;

#وإلا

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    أرجع NGX_CONF_ERROR;

#جيڪڏهن_پورو
}


#إذا (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *معلومات)
{
    فراغ  *handle = معلومات;

    إذا (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#جيڪڏهن_پورو
