
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <nginx.h>


अटल व्योम ngx_show_version_info(व्योम);
अटल ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
अटल व्योम ngx_cleanup_environment(व्योम *तथ्य);
अटल व्योम ngx_cleanup_environment_variable(व्योम *तथ्य);
अटल ngx_int_t ngx_get_options(पूर्णांक argc, अक्षर *स्थिर *argv);
अटल ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
अटल ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, पूर्णांक argc, अक्षर *स्थिर *argv);
अटल व्योम *ngx_core_module_create_conf(ngx_cycle_t *cycle);
अटल अक्षर *ngx_core_module_init_conf(ngx_cycle_t *cycle, व्योम *conf);
अटल अक्षर *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अटल अक्षर *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अटल अक्षर *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अटल अक्षर *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    व्योम *conf);
अटल अक्षर *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    व्योम *conf);
अटल अक्षर *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
#अगर (NGX_HAVE_DLOPEN)
अटल व्योम ngx_unload_module(व्योम *तथ्य);
#पूर्ण_अगर


अटल ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


अटल ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, daemon),
      शून्य },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, master),
      शून्य },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, timer_resolution),
      शून्य },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, pid),
      शून्य },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, lock_file),
      शून्य },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      शून्य },

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
      शून्य },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      शून्य },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      शून्य },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_अंक_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_nofile),
      शून्य },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_core),
      शून्य },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, shutdown_timeout),
      शून्य },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, working_directory),
      शून्य },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      शून्य },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      शून्य },

      ngx_null_command
};


अटल ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    शून्य,                                  /* init master */
    शून्य,                                  /* init module */
    शून्य,                                  /* init process */
    शून्य,                                  /* init thread */
    शून्य,                                  /* exit thread */
    शून्य,                                  /* exit process */
    शून्य,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


अटल ngx_uint_t   ngx_show_help;
अटल ngx_uint_t   ngx_show_version;
अटल ngx_uint_t   ngx_show_configure;
अटल u_char      *ngx_prefix;
अटल u_char      *ngx_error_log;
अटल u_char      *ngx_conf_file;
अटल u_char      *ngx_conf_params;
#अगर (NGX_CONTROL_API)
अटल u_char      *ngx_control_addr;
#पूर्ण_अगर
अटल अक्षर        *ngx_signal;


अटल अक्षर **ngx_os_environ;


पूर्णांक ngx_cdecl
मुख्य(पूर्णांक argc, अक्षर *स्थिर *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    अगर (ngx_strerror_init() != NGX_OK) {
        वापस 1;
    }

    अगर (ngx_get_options(argc, argv) != NGX_OK) {
        वापस 1;
    }

    अगर (ngx_show_version) {
        ngx_show_version_info();

        अगर (!ngx_test_config) {
            वापस 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#अगर (NGX_PCRE)
    ngx_regex_init();
#पूर्ण_अगर

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    अगर (log == शून्य) {
        वापस 1;
    }

    /* STUB */
#अगर (NGX_OPENSSL)
    ngx_ssl_init(log);
#पूर्ण_अगर

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, माप(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    अगर (init_cycle.pool == शून्य) {
        वापस 1;
    }

    अगर (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        वापस 1;
    }

    अगर (ngx_process_options(&init_cycle) != NGX_OK) {
        वापस 1;
    }

    अगर (ngx_os_init(log) != NGX_OK) {
        वापस 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    अगर (ngx_crc32_table_init() != NGX_OK) {
        वापस 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    अगर (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        वापस 1;
    }

#अगर (NGX_CONTROL_API)
    ngx_control_preinit();
#पूर्ण_अगर

    अगर (ngx_preinit_modules() != NGX_OK) {
        वापस 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    अगर (cycle == शून्य) {
        अगर (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.तथ्य);
        }

        वापस 1;
    }

    अगर (ngx_test_config) {
        अगर (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.तथ्य);
        }

        अगर (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            क्रम (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (व्योम) ngx_write_fd(ngx_stdout, cd[i].name.तथ्य,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].बफर;

                (व्योम) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        वापस 0;
    }

    अगर (ngx_signal) {
        वापस ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    अगर (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#अगर !(NGX_WIN32)

    अगर (ngx_init_signals(cycle->log) != NGX_OK) {
        वापस 1;
    }

    अगर (!ngx_inherited && ccf->daemon) {
        अगर (ngx_daemon(cycle->log) != NGX_OK) {
            वापस 1;
        }

        ngx_daemonized = 1;
    }

    अगर (ngx_inherited) {
        ngx_daemonized = 1;
    }

#पूर्ण_अगर

#अगर (NGX_CONTROL_API)
    अगर (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } अन्यथा {
        अगर (ngx_control_init(ngx_control_addr) != NGX_OK) {
            वापस 1;
        }
    }
#पूर्ण_अगर

    अगर (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        वापस 1;
    }

    अगर (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        वापस 1;
    }

    अगर (log->file->fd != ngx_stderr) {
        अगर (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    अगर (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } अन्यथा {
        ngx_master_process_cycle(cycle);
    }

    वापस 0;
}


अटल व्योम
ngx_show_version_info(व्योम)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    अगर (ngx_show_help) {
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
#अगर (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#पूर्ण_अगर
#अगर_घोषित NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#अन्यथा
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#पूर्ण_अगर
            "  -e filename   : set error log file (default: "
#अगर_घोषित NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#अन्यथा
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#पूर्ण_अगर
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    अगर (ngx_show_configure) {

#अगर_घोषित NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#पूर्ण_अगर

#अगर (NGX_SSL)
        अगर (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } अन्यथा {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((अक्षर *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#अगर_घोषित SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#अन्यथा
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#पूर्ण_अगर
#पूर्ण_अगर

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


अटल ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) दो_पर्या(NGINX_VAR);

    अगर (inherited == शून्य) {
        वापस NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    अगर (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       माप(ngx_listening_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    क्रम (p = inherited, v = p; *p; p++) {
        अगर (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            अगर (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                अवरोध;
            }

            v = p + 1;

            ls = ngx_array_धकेलो(&cycle->listening);
            अगर (ls == शून्य) {
                वापस NGX_ERROR;
            }

            ngx_memzero(ls, माप(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    अगर (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    वापस ngx_set_inherited_sockets(cycle);
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

    अगर (last == शून्य && ccf->environment) {
        वापस ccf->environment;
    }

    var = ccf->env.elts;

    क्रम (i = 0; i < ccf->env.nelts; i++) {
        अगर (ngx_strcmp(var[i].तथ्य, "TZ") == 0
            || ngx_strncmp(var[i].तथ्य, "TZ=", 3) == 0)
        {
            जाओ tz_found;
        }
    }

    var = ngx_array_धकेलो(&ccf->env);
    अगर (var == शून्य) {
        वापस शून्य;
    }

    var->len = 2;
    var->तथ्य = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    क्रम (i = 0; i < ccf->env.nelts; i++) {

        अगर (var[i].तथ्य[var[i].len] == '=') {
            n++;
            जारी;
        }

        क्रम (p = ngx_os_environ; *p; p++) {

            अगर (ngx_strncmp(*p, var[i].तथ्य, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                अवरोध;
            }
        }
    }

    अगर (last) {
        env = ngx_alloc((*last + n + 1) * माप(अक्षर *), cycle->log);
        अगर (env == शून्य) {
            वापस शून्य;
        }

        *last = n;

    } अन्यथा {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        अगर (cln == शून्य) {
            वापस शून्य;
        }

        env = ngx_alloc((n + 1) * माप(अक्षर *), cycle->log);
        अगर (env == शून्य) {
            वापस शून्य;
        }

        cln->handler = ngx_cleanup_environment;
        cln->तथ्य = env;
    }

    n = 0;

    क्रम (i = 0; i < ccf->env.nelts; i++) {

        अगर (var[i].तथ्य[var[i].len] == '=') {

            अगर (last) {
                env[n++] = (अक्षर *) var[i].तथ्य;
                जारी;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            अगर (cln == शून्य) {
                वापस शून्य;
            }

            len = ngx_strlen(var[i].तथ्य) + 1;

            str = ngx_alloc(len, cycle->log);
            अगर (str == शून्य) {
                वापस शून्य;
            }

            ngx_memcpy(str, var[i].तथ्य, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->तथ्य = str;

            env[n++] = str;

            जारी;
        }

        क्रम (p = ngx_os_environ; *p; p++) {

            अगर (ngx_strncmp(*p, var[i].तथ्य, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                अवरोध;
            }
        }
    }

    env[n] = शून्य;

    अगर (last == शून्य) {
        ccf->environment = env;
        environ = env;
    }

    वापस env;
}


अटल व्योम
ngx_cleanup_environment(व्योम *तथ्य)
{
    अक्षर  **env = तथ्य;

    अगर (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        वापस;
    }

    ngx_free(env);
}


अटल व्योम
ngx_cleanup_environment_variable(व्योम *तथ्य)
{
    अक्षर  *var = तथ्य;

    अक्षर  **p;

    क्रम (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        अगर (*p == var) {
            वापस;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_द्विआधारी(ngx_cycle_t *cycle, अक्षर *स्थिर *argv)
{
    अक्षर             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, माप(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    अगर (env == शून्य) {
        वापस NGX_INVALID_PID;
    }

    var = ngx_alloc(माप(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    अगर (var == शून्य) {
        ngx_free(env);
        वापस NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", माप(NGINX_VAR));

    ls = cycle->listening.elts;
    क्रम (i = 0; i < cycle->listening.nelts; i++) {
        अगर (ls[i].ignore) {
            जारी;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#अगर (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#पूर्ण_अगर

#अगर (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    अगर (env[n] != शून्य) {
        n++;
    }
#पूर्ण_अगर

    env[n] = शून्य;

#अगर (NGX_DEBUG)
    {
    अक्षर  **e;
    क्रम (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#पूर्ण_अगर

    ctx.envp = (अक्षर *स्थिर *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    अगर (ngx_rename_file(ccf->pid.तथ्य, ccf->oldpid.तथ्य) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.तथ्य, ccf->oldpid.तथ्य, argv[0]);

        ngx_free(env);
        ngx_free(var);

        वापस NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    अगर (pid == NGX_INVALID_PID) {
        अगर (ngx_rename_file(ccf->oldpid.तथ्य, ccf->pid.तथ्य)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.तथ्य, ccf->pid.तथ्य, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    वापस pid;
}


अटल ngx_int_t
ngx_get_options(पूर्णांक argc, अक्षर *स्थिर *argv)
{
    u_char     *p;
    ngx_int_t   i;

    क्रम (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        अगर (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            वापस NGX_ERROR;
        }

        जबतक (*p) {

            चयन (*p++) {

            हाल '?':
            हाल 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                अवरोध;

            हाल 'v':
                ngx_show_version = 1;
                अवरोध;

            हाल 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                अवरोध;

            हाल 't':
                ngx_test_config = 1;
                अवरोध;

            हाल 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                अवरोध;

            हाल 'q':
                ngx_quiet_mode = 1;
                अवरोध;

            हाल 'p':
                अगर (*p) {
                    ngx_prefix = p;
                    जाओ अग्र;
                }

                अगर (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    जाओ अग्र;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                वापस NGX_ERROR;

            हाल 'e':
                अगर (*p) {
                    ngx_error_log = p;

                } अन्यथा अगर (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    वापस NGX_ERROR;
                }

                अगर (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                जाओ अग्र;

            हाल 'c':
                अगर (*p) {
                    ngx_conf_file = p;
                    जाओ अग्र;
                }

                अगर (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    जाओ अग्र;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                वापस NGX_ERROR;

            हाल 'g':
                अगर (*p) {
                    ngx_conf_params = p;
                    जाओ अग्र;
                }

                अगर (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    जाओ अग्र;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                वापस NGX_ERROR;

            हाल 's':
                अगर (*p) {
                    ngx_signal = (अक्षर *) p;

                } अन्यथा अगर (argv[++i]) {
                    ngx_signal = argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    वापस NGX_ERROR;
                }

                अगर (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    जाओ अग्र;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                वापस NGX_ERROR;

#अगर (NGX_CONTROL_API)
            हाल 'l':
                ngx_control_api_enabled = 1;

                अगर (*p) {
                    ngx_control_addr = p;
                    जाओ अग्र;
                }

                अगर (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    जाओ अग्र;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                वापस NGX_ERROR;
#पूर्ण_अगर

            शेष:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                वापस NGX_ERROR;
            }
        }

    अग्र:

        जारी;
    }

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, पूर्णांक argc, अक्षर *स्थिर *argv)
{
#अगर (NGX_FREEBSD)

    ngx_os_argv = (अक्षर **) argv;
    ngx_argc = argc;
    ngx_argv = (अक्षर **) argv;

#अन्यथा
    माप_प्रकार     len;
    ngx_int_t  i;

    ngx_os_argv = (अक्षर **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * माप(अक्षर *), cycle->log);
    अगर (ngx_argv == शून्य) {
        वापस NGX_ERROR;
    }

    क्रम (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        अगर (ngx_argv[i] == शून्य) {
            वापस NGX_ERROR;
        }

        (व्योम) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = शून्य;

#पूर्ण_अगर

    ngx_os_environ = environ;

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    माप_प्रकार   len;

    अगर (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        अगर (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            अगर (p == शून्य) {
                वापस NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.तथ्य = p;
        cycle->prefix.len = len;
        cycle->prefix.तथ्य = p;

    } अन्यथा {

#अगर_अघोषित NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        अगर (p == शून्य) {
            वापस NGX_ERROR;
        }

        अगर (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            वापस NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.तथ्य = p;
        cycle->prefix.len = len;
        cycle->prefix.तथ्य = p;

#अन्यथा

#अगर_घोषित NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#अन्यथा
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#पूर्ण_अगर
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#पूर्ण_अगर
    }

    अगर (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.तथ्य = ngx_conf_file;

    } अन्यथा {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    अगर (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        वापस NGX_ERROR;
    }

    क्रम (p = cycle->conf_file.तथ्य + cycle->conf_file.len - 1;
         p > cycle->conf_file.तथ्य;
         p--)
    {
        अगर (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.तथ्य + 1;
            cycle->conf_prefix.तथ्य = cycle->conf_file.तथ्य;
            अवरोध;
        }
    }

    अगर (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.तथ्य = ngx_error_log;

    } अन्यथा {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    अगर (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.तथ्य = ngx_conf_params;
    }

    अगर (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    वापस NGX_OK;
}


अटल व्योम *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, माप(ngx_core_conf_t));
    अगर (ccf == शून्य) {
        वापस शून्य;
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

    अगर (ngx_array_init(&ccf->env, cycle->pool, 1, माप(ngx_str_t))
        != NGX_OK)
    {
        वापस शून्य;
    }

    वापस ccf;
}


अटल अक्षर *
ngx_core_module_init_conf(ngx_cycle_t *cycle, व्योम *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_मूल्य(ccf->daemon, 1);
    ngx_conf_init_मूल्य(ccf->master, 1);
    ngx_conf_init_msec_मूल्य(ccf->timer_resolution, 0);
    ngx_conf_init_msec_मूल्य(ccf->shutdown_timeout, 0);

    ngx_conf_init_मूल्य(ccf->worker_processes, 1);
    ngx_conf_init_मूल्य(ccf->debug_points, 0);

#अगर (NGX_HAVE_CPU_AFFINITY)

    अगर (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->worker_processes)
    {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#पूर्ण_अगर


    अगर (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    अगर (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + माप(NGX_OLDPID_EXT);

    ccf->oldpid.तथ्य = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    अगर (ccf->oldpid.तथ्य == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.तथ्य, ccf->pid.तथ्य, ccf->pid.len),
               NGX_OLDPID_EXT, माप(NGX_OLDPID_EXT));


    अगर (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        अगर (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            वापस NGX_CONF_ERROR;
        }

        अगर (ngx_file_info(ccf->working_directory.तथ्य, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.तथ्य);
            वापस NGX_CONF_ERROR;
        }

        अगर (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.तथ्य);
            वापस NGX_CONF_ERROR;
        }
    }


#अगर !(NGX_WIN32)

    अगर (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        काष्ठा group   *grp;
        काष्ठा passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        अगर (pwd == शून्य) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            वापस NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        अगर (grp == शून्य) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            वापस NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    अगर (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    अगर (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    अगर (lock_file.len) {
        lock_file.len--;

        अगर (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.तथ्य, lock_file.तथ्य, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += माप(".accept");

        cycle->lock_file.तथ्य = ngx_pstrdup(cycle->pool, &lock_file);
        अगर (cycle->lock_file.तथ्य == शून्य) {
            वापस NGX_CONF_ERROR;
        }

    } अन्यथा {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.तथ्य = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + माप(".accept"));
        अगर (cycle->lock_file.तथ्य == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.तथ्य, ccf->lock_file.तथ्य,
                              ccf->lock_file.len),
                   ".accept", माप(".accept"));
    }
    }

#पूर्ण_अगर

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
#अगर (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    वापस NGX_CONF_OK;

#अन्यथा

    ngx_core_conf_t  *ccf = conf;

    अक्षर             *group;
    काष्ठा passwd    *pwd;
    काष्ठा group     *grp;
    ngx_str_t        *मूल्य;

    अगर (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        वापस "is duplicate";
    }

    अगर (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        वापस NGX_CONF_OK;
    }

    मूल्य = cf->args->elts;

    ccf->username = (अक्षर *) मूल्य[1].तथ्य;

    ngx_set_errno(0);
    pwd = getpwnam((स्थिर अक्षर *) मूल्य[1].तथ्य);
    अगर (pwd == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", मूल्य[1].तथ्य);
        वापस NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (अक्षर *) ((cf->args->nelts == 2) ? मूल्य[1].तथ्य : मूल्य[2].तथ्य);

    ngx_set_errno(0);
    grp = getgrnam(group);
    अगर (grp == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        वापस NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    वापस NGX_CONF_OK;

#पूर्ण_अगर
}


अटल अक्षर *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *मूल्य, *var;
    ngx_uint_t   i;

    var = ngx_array_धकेलो(&ccf->env);
    अगर (var == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;
    *var = मूल्य[1];

    क्रम (i = 0; i < मूल्य[1].len; i++) {

        अगर (मूल्य[1].तथ्य[i] == '=') {

            var->len = i;

            वापस NGX_CONF_OK;
        }
    }

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *मूल्य;
    ngx_uint_t        n, minus;

    अगर (ccf->priority != 0) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    अगर (मूल्य[1].तथ्य[0] == '-') {
        n = 1;
        minus = 1;

    } अन्यथा अगर (मूल्य[1].तथ्य[0] == '+') {
        n = 1;
        minus = 0;

    } अन्यथा {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&मूल्य[1].तथ्य[n], मूल्य[1].len - n);
    अगर (ccf->priority == NGX_ERROR) {
        वापस "invalid number";
    }

    अगर (minus) {
        ccf->priority = -ccf->priority;
    }

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
#अगर (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *मूल्य;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    अगर (ccf->cpu_affinity) {
        वापस "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * माप(ngx_cpuset_t));
    अगर (mask == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    मूल्य = cf->args->elts;

    अगर (ngx_strcmp(मूल्य[1].तथ्य, "auto") == 0) {

        अगर (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            वापस NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        क्रम (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } अन्यथा {
        n = 1;
    }

    क्रम ( /* void */ ; n < cf->args->nelts; n++) {

        अगर (मूल्य[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            वापस NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        क्रम (p = मूल्य[n].तथ्य + मूल्य[n].len - 1;
             p >= मूल्य[n].तथ्य;
             p--)
        {
            ch = *p;

            अगर (ch == ' ') {
                जारी;
            }

            i++;

            अगर (ch == '0') {
                जारी;
            }

            अगर (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                जारी;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            वापस NGX_CONF_ERROR;
        }
    }

#अन्यथा

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#पूर्ण_अगर

    वापस NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#अगर (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    अटल ngx_cpuset_t  परिणाम;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    अगर (ccf->cpu_affinity == शून्य) {
        वापस शून्य;
    }

    अगर (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        क्रम (i = 0, j = n; /* void */ ; i++) {

            अगर (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                अवरोध;
            }

            अगर (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                वापस शून्य;
            }

            /* void */
        }

        CPU_ZERO(&परिणाम);
        CPU_SET(i % CPU_SETSIZE, &परिणाम);

        वापस &परिणाम;
    }

    अगर (ccf->cpu_affinity_n > n) {
        वापस &ccf->cpu_affinity[n];
    }

    वापस &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#अन्यथा

    वापस शून्य;

#पूर्ण_अगर
}


अटल अक्षर *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    ngx_str_t        *मूल्य;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    अगर (ccf->worker_processes != NGX_CONF_UNSET) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    अगर (ngx_strcmp(मूल्य[1].तथ्य, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        वापस NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(मूल्य[1].तथ्य, मूल्य[1].len);

    अगर (ccf->worker_processes == NGX_ERROR) {
        वापस "invalid value";
    }

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
#अगर (NGX_HAVE_DLOPEN)
    व्योम                *handle;
    अक्षर               **names, **order;
    ngx_str_t           *मूल्य, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    अगर (cf->cycle->modules_used) {
        वापस "is specified too late";
    }

    मूल्य = cf->args->elts;

    file = मूल्य[1];

    अगर (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    अगर (cln == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.तथ्य);
    अगर (handle == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.तथ्य, ngx_dlerror());
        वापस NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->तथ्य = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    अगर (modules == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_modules", ngx_dlerror());
        वापस NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    अगर (names == शून्य) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_module_names", ngx_dlerror());
        वापस NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    क्रम (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        अगर (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            वापस NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->सूचकांक);
    }

    वापस NGX_CONF_OK;

#अन्यथा

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    वापस NGX_CONF_ERROR;

#पूर्ण_अगर
}


#अगर (NGX_HAVE_DLOPEN)

अटल व्योम
ngx_unload_module(व्योम *तथ्य)
{
    व्योम  *handle = तथ्य;

    अगर (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#पूर्ण_अगर
