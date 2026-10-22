
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <nginx.h>


स्थिर शून्य ngx_show_version_info(शून्य);
स्थिर ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
स्थिर शून्य ngx_cleanup_environment(शून्य *data);
स्थिर शून्य ngx_cleanup_environment_variable(शून्य *data);
स्थिर ngx_int_t ngx_get_options(पूर्णांक argc, अक्षर *स्थिरांक *argv);
स्थिर ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
स्थिर ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, पूर्णांक argc, अक्षर *स्थिरांक *argv);
स्थिर शून्य *ngx_core_module_create_conf(ngx_cycle_t *cycle);
स्थिर अक्षर *ngx_core_module_init_conf(ngx_cycle_t *cycle, शून्य *conf);
स्थिर अक्षर *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर अक्षर *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर अक्षर *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर अक्षर *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर अक्षर *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर अक्षर *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
#जर (NGX_HAVE_DLOPEN)
स्थिर शून्य ngx_unload_module(शून्य *data);
#endif


स्थिर ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


स्थिर ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, daemon),
      रिक्त },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, master),
      रिक्त },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, timer_resolution),
      रिक्त },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, pid),
      रिक्त },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, lock_file),
      रिक्त },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      रिक्त },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      0,
      दुरत्व(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE12,
      ngx_set_user,
      0,
      0,
      रिक्त },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      रिक्त },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      रिक्त },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_अंक_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_nofile),
      रिक्त },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_core),
      रिक्त },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, shutdown_timeout),
      रिक्त },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, working_directory),
      रिक्त },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      रिक्त },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      रिक्त },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    रिक्त,                                  /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    रिक्त,                                  /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_uint_t   ngx_show_help;
स्थिर ngx_uint_t   ngx_show_version;
स्थिर ngx_uint_t   ngx_show_configure;
स्थिर u_char      *ngx_prefix;
स्थिर u_char      *ngx_error_log;
स्थिर u_char      *ngx_conf_file;
स्थिर u_char      *ngx_conf_params;
#जर (NGX_CONTROL_API)
स्थिर u_char      *ngx_control_addr;
#endif
स्थिर अक्षर        *ngx_signal;


स्थिर अक्षर **ngx_os_environ;


पूर्णांक ngx_cdecl
main(पूर्णांक argc, अक्षर *स्थिरांक *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    जर (ngx_strerror_init() != NGX_OK) {
        परत 1;
    }

    जर (ngx_get_options(argc, argv) != NGX_OK) {
        परत 1;
    }

    जर (ngx_show_version) {
        ngx_show_version_info();

        जर (!ngx_test_config) {
            परत 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#जर (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    जर (log == रिक्त) {
        परत 1;
    }

    /* STUB */
#जर (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, आकार(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    जर (init_cycle.pool == रिक्त) {
        परत 1;
    }

    जर (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        परत 1;
    }

    जर (ngx_process_options(&init_cycle) != NGX_OK) {
        परत 1;
    }

    जर (ngx_os_init(log) != NGX_OK) {
        परत 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    जर (ngx_crc32_table_init() != NGX_OK) {
        परत 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    जर (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        परत 1;
    }

#जर (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    जर (ngx_preinit_modules() != NGX_OK) {
        परत 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    जर (cycle == रिक्त) {
        जर (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        परत 1;
    }

    जर (ngx_test_config) {
        जर (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        जर (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            साठी (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (शून्य) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (शून्य) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        परत 0;
    }

    जर (ngx_signal) {
        परत ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    जर (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#जर !(NGX_WIN32)

    जर (ngx_init_signals(cycle->log) != NGX_OK) {
        परत 1;
    }

    जर (!ngx_inherited && ccf->daemon) {
        जर (ngx_daemon(cycle->log) != NGX_OK) {
            परत 1;
        }

        ngx_daemonized = 1;
    }

    जर (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#जर (NGX_CONTROL_API)
    जर (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } अन्यथा {
        जर (ngx_control_init(ngx_control_addr) != NGX_OK) {
            परत 1;
        }
    }
#endif

    जर (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        परत 1;
    }

    जर (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        परत 1;
    }

    जर (log->file->fd != ngx_stderr) {
        जर (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    जर (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } अन्यथा {
        ngx_master_process_cycle(cycle);
    }

    परत 0;
}


स्थिर शून्य
ngx_show_version_info(शून्य)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    जर (ngx_show_help) {
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
#जर (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#अन्यथा
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#अन्यथा
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    जर (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#जर (NGX_SSL)
        जर (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } अन्यथा {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((अक्षर *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#अन्यथा
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


स्थिर ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) दो_पर्या(NGINX_VAR);

    जर (inherited == रिक्त) {
        परत NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    जर (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       आकार(ngx_listening_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    साठी (p = inherited, v = p; *p; p++) {
        जर (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            जर (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                थांबा;
            }

            v = p + 1;

            ls = ngx_array_ढकला(&cycle->listening);
            जर (ls == रिक्त) {
                परत NGX_ERROR;
            }

            ngx_memzero(ls, आकार(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    जर (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    परत ngx_set_inherited_sockets(cycle);
}


अक्षर **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    अक्षर                **p, **env, *str;
    माप_प्रकार                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    जर (last == रिक्त && ccf->environment) {
        परत ccf->environment;
    }

    var = ccf->env.elts;

    साठी (i = 0; i < ccf->env.nelts; i++) {
        जर (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            जा tz_found;
        }
    }

    var = ngx_array_ढकला(&ccf->env);
    जर (var == रिक्त) {
        परत रिक्त;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    साठी (i = 0; i < ccf->env.nelts; i++) {

        जर (var[i].data[var[i].len] == '=') {
            n++;
            सुरूठेवा;
        }

        साठी (p = ngx_os_environ; *p; p++) {

            जर (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                थांबा;
            }
        }
    }

    जर (last) {
        env = ngx_alloc((*last + n + 1) * आकार(अक्षर *), cycle->log);
        जर (env == रिक्त) {
            परत रिक्त;
        }

        *last = n;

    } अन्यथा {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        जर (cln == रिक्त) {
            परत रिक्त;
        }

        env = ngx_alloc((n + 1) * आकार(अक्षर *), cycle->log);
        जर (env == रिक्त) {
            परत रिक्त;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    साठी (i = 0; i < ccf->env.nelts; i++) {

        जर (var[i].data[var[i].len] == '=') {

            जर (last) {
                env[n++] = (अक्षर *) var[i].data;
                सुरूठेवा;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            जर (cln == रिक्त) {
                परत रिक्त;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            जर (str == रिक्त) {
                परत रिक्त;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            सुरूठेवा;
        }

        साठी (p = ngx_os_environ; *p; p++) {

            जर (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                थांबा;
            }
        }
    }

    env[n] = रिक्त;

    जर (last == रिक्त) {
        ccf->environment = env;
        environ = env;
    }

    परत env;
}


स्थिर शून्य
ngx_cleanup_environment(शून्य *data)
{
    अक्षर  **env = data;

    जर (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        परत;
    }

    ngx_free(env);
}


स्थिर शून्य
ngx_cleanup_environment_variable(शून्य *data)
{
    अक्षर  *var = data;

    अक्षर  **p;

    साठी (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        जर (*p == var) {
            परत;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_द्विमान(ngx_cycle_t *cycle, अक्षर *स्थिरांक *argv)
{
    अक्षर             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, आकार(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    जर (env == रिक्त) {
        परत NGX_INVALID_PID;
    }

    var = ngx_alloc(आकार(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    जर (var == रिक्त) {
        ngx_free(env);
        परत NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", आकार(NGINX_VAR));

    ls = cycle->listening.elts;
    साठी (i = 0; i < cycle->listening.nelts; i++) {
        जर (ls[i].ignore) {
            सुरूठेवा;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#जर (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#जर (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    जर (env[n] != रिक्त) {
        n++;
    }
#endif

    env[n] = रिक्त;

#जर (NGX_DEBUG)
    {
    अक्षर  **e;
    साठी (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (अक्षर *स्थिरांक *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    जर (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        परत NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    जर (pid == NGX_INVALID_PID) {
        जर (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    परत pid;
}


स्थिर ngx_int_t
ngx_get_options(पूर्णांक argc, अक्षर *स्थिरांक *argv)
{
    u_char     *p;
    ngx_int_t   i;

    साठी (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        जर (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            परत NGX_ERROR;
        }

        पर्यंत (*p) {

            निवड (*p++) {

            स्थिती '?':
            स्थिती 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                थांबा;

            स्थिती 'v':
                ngx_show_version = 1;
                थांबा;

            स्थिती 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                थांबा;

            स्थिती 't':
                ngx_test_config = 1;
                थांबा;

            स्थिती 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                थांबा;

            स्थिती 'q':
                ngx_quiet_mode = 1;
                थांबा;

            स्थिती 'p':
                जर (*p) {
                    ngx_prefix = p;
                    जा next;
                }

                जर (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    जा next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                परत NGX_ERROR;

            स्थिती 'e':
                जर (*p) {
                    ngx_error_log = p;

                } अन्यथा जर (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    परत NGX_ERROR;
                }

                जर (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                जा next;

            स्थिती 'c':
                जर (*p) {
                    ngx_conf_file = p;
                    जा next;
                }

                जर (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    जा next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                परत NGX_ERROR;

            स्थिती 'g':
                जर (*p) {
                    ngx_conf_params = p;
                    जा next;
                }

                जर (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    जा next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                परत NGX_ERROR;

            स्थिती 's':
                जर (*p) {
                    ngx_signal = (अक्षर *) p;

                } अन्यथा जर (argv[++i]) {
                    ngx_signal = argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    परत NGX_ERROR;
                }

                जर (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    जा next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                परत NGX_ERROR;

#जर (NGX_CONTROL_API)
            स्थिती 'l':
                ngx_control_api_enabled = 1;

                जर (*p) {
                    ngx_control_addr = p;
                    जा next;
                }

                जर (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    जा next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                परत NGX_ERROR;
#endif

            पूर्वनिर्धारित:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                परत NGX_ERROR;
            }
        }

    next:

        सुरूठेवा;
    }

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, पूर्णांक argc, अक्षर *स्थिरांक *argv)
{
#जर (NGX_FREEBSD)

    ngx_os_argv = (अक्षर **) argv;
    ngx_argc = argc;
    ngx_argv = (अक्षर **) argv;

#अन्यथा
    माप_प्रकार     len;
    ngx_int_t  i;

    ngx_os_argv = (अक्षर **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * आकार(अक्षर *), cycle->log);
    जर (ngx_argv == रिक्त) {
        परत NGX_ERROR;
    }

    साठी (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        जर (ngx_argv[i] == रिक्त) {
            परत NGX_ERROR;
        }

        (शून्य) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = रिक्त;

#endif

    ngx_os_environ = environ;

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    माप_प्रकार   len;

    जर (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        जर (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            जर (p == रिक्त) {
                परत NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } अन्यथा {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        जर (p == रिक्त) {
            परत NGX_ERROR;
        }

        जर (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            परत NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#अन्यथा

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#अन्यथा
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    जर (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } अन्यथा {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    जर (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        परत NGX_ERROR;
    }

    साठी (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        जर (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            थांबा;
        }
    }

    जर (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } अन्यथा {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    जर (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    जर (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    परत NGX_OK;
}


स्थिर शून्य *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, आकार(ngx_core_conf_t));
    जर (ccf == रिक्त) {
        परत रिक्त;
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

    जर (ngx_array_init(&ccf->env, cycle->pool, 1, आकार(ngx_str_t))
        != NGX_OK)
    {
        परत रिक्त;
    }

    परत ccf;
}


स्थिर अक्षर *
ngx_core_module_init_conf(ngx_cycle_t *cycle, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_मूल्य(ccf->daemon, 1);
    ngx_conf_init_मूल्य(ccf->master, 1);
    ngx_conf_init_msec_मूल्य(ccf->timer_resolution, 0);
    ngx_conf_init_msec_मूल्य(ccf->shutdown_timeout, 0);

    ngx_conf_init_मूल्य(ccf->worker_processes, 1);
    ngx_conf_init_मूल्य(ccf->debug_points, 0);

#जर (NGX_HAVE_CPU_AFFINITY)

    जर (!ccf->cpu_affinity_auto
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


    जर (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    जर (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        परत NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + आकार(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    जर (ccf->oldpid.data == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, आकार(NGX_OLDPID_EXT));


    जर (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        जर (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            परत NGX_CONF_ERROR;
        }

        जर (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            परत NGX_CONF_ERROR;
        }

        जर (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            परत NGX_CONF_ERROR;
        }
    }


#जर !(NGX_WIN32)

    जर (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        संरचना group   *grp;
        संरचना passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        जर (pwd == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            परत NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        जर (grp == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            परत NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    जर (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    जर (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        परत NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    जर (lock_file.len) {
        lock_file.len--;

        जर (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += आकार(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        जर (cycle->lock_file.data == रिक्त) {
            परत NGX_CONF_ERROR;
        }

    } अन्यथा {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + आकार(".accept"));
        जर (cycle->lock_file.data == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", आकार(".accept"));
    }
    }

#endif

    परत NGX_CONF_OK;
}


स्थिर अक्षर *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#जर (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    परत NGX_CONF_OK;

#अन्यथा

    ngx_core_conf_t  *ccf = conf;

    अक्षर             *group;
    संरचना passwd    *pwd;
    संरचना group     *grp;
    ngx_str_t        *मूल्य;

    जर (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        परत "is duplicate";
    }

    जर (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        परत NGX_CONF_OK;
    }

    मूल्य = cf->args->elts;

    ccf->username = (अक्षर *) मूल्य[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((स्थिरांक अक्षर *) मूल्य[1].data);
    जर (pwd == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", मूल्य[1].data);
        परत NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (अक्षर *) ((cf->args->nelts == 2) ? मूल्य[1].data : मूल्य[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    जर (grp == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        परत NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    परत NGX_CONF_OK;

#endif
}


स्थिर अक्षर *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *मूल्य, *var;
    ngx_uint_t   i;

    var = ngx_array_ढकला(&ccf->env);
    जर (var == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;
    *var = मूल्य[1];

    साठी (i = 0; i < मूल्य[1].len; i++) {

        जर (मूल्य[1].data[i] == '=') {

            var->len = i;

            परत NGX_CONF_OK;
        }
    }

    परत NGX_CONF_OK;
}


स्थिर अक्षर *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *मूल्य;
    ngx_uint_t        n, minus;

    जर (ccf->priority != 0) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    जर (मूल्य[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } अन्यथा जर (मूल्य[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } अन्यथा {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&मूल्य[1].data[n], मूल्य[1].len - n);
    जर (ccf->priority == NGX_ERROR) {
        परत "invalid number";
    }

    जर (minus) {
        ccf->priority = -ccf->priority;
    }

    परत NGX_CONF_OK;
}


स्थिर अक्षर *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#जर (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *मूल्य;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    जर (ccf->cpu_affinity) {
        परत "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * आकार(ngx_cpuset_t));
    जर (mask == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    मूल्य = cf->args->elts;

    जर (ngx_strcmp(मूल्य[1].data, "auto") == 0) {

        जर (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            परत NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        साठी (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } अन्यथा {
        n = 1;
    }

    साठी ( /* void */ ; n < cf->args->nelts; n++) {

        जर (मूल्य[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            परत NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        साठी (p = मूल्य[n].data + मूल्य[n].len - 1;
             p >= मूल्य[n].data;
             p--)
        {
            ch = *p;

            जर (ch == ' ') {
                सुरूठेवा;
            }

            i++;

            जर (ch == '0') {
                सुरूठेवा;
            }

            जर (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                सुरूठेवा;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            परत NGX_CONF_ERROR;
        }
    }

#अन्यथा

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    परत NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#जर (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    स्थिर ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    जर (ccf->cpu_affinity == रिक्त) {
        परत रिक्त;
    }

    जर (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        साठी (i = 0, j = n; /* void */ ; i++) {

            जर (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                थांबा;
            }

            जर (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                परत रिक्त;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        परत &result;
    }

    जर (ccf->cpu_affinity_n > n) {
        परत &ccf->cpu_affinity[n];
    }

    परत &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#अन्यथा

    परत रिक्त;

#endif
}


स्थिर अक्षर *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t        *मूल्य;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    जर (ccf->worker_processes != NGX_CONF_UNSET) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    जर (ngx_strcmp(मूल्य[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        परत NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(मूल्य[1].data, मूल्य[1].len);

    जर (ccf->worker_processes == NGX_ERROR) {
        परत "invalid value";
    }

    परत NGX_CONF_OK;
}


स्थिर अक्षर *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#जर (NGX_HAVE_DLOPEN)
    शून्य                *handle;
    अक्षर               **names, **order;
    ngx_str_t           *मूल्य, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    जर (cf->cycle->modules_used) {
        परत "is specified too late";
    }

    मूल्य = cf->args->elts;

    file = मूल्य[1];

    जर (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        परत NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    जर (cln == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    जर (handle == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        परत NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    जर (modules == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_modules", ngx_dlerror());
        परत NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    जर (names == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_module_names", ngx_dlerror());
        परत NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    साठी (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        जर (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            परत NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    परत NGX_CONF_OK;

#अन्यथा

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    परत NGX_CONF_ERROR;

#endif
}


#जर (NGX_HAVE_DLOPEN)

स्थिर शून्य
ngx_unload_module(शून्य *data)
{
    शून्य  *handle = data;

    जर (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
