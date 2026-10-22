
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <nginx.h>


સ્થિર શૂન્ય ngx_show_version_info(શૂન્ય);
સ્થિર ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
સ્થિર શૂન્ય ngx_cleanup_environment(શૂન્ય *data);
સ્થિર શૂન્ય ngx_cleanup_environment_variable(શૂન્ય *data);
સ્થિર ngx_int_t ngx_get_options(પૂર્ણાંક argc, અક્ષર *અચલ *argv);
સ્થિર ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
સ્થિર ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, પૂર્ણાંક argc, અક્ષર *અચલ *argv);
સ્થિર શૂન્ય *ngx_core_module_create_conf(ngx_cycle_t *cycle);
સ્થિર અક્ષર *ngx_core_module_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf);
સ્થિર અક્ષર *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
સ્થિર અક્ષર *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
સ્થિર અક્ષર *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
સ્થિર અક્ષર *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    શૂન્ય *conf);
સ્થિર અક્ષર *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    શૂન્ય *conf);
સ્થિર અક્ષર *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
#જો (NGX_HAVE_DLOPEN)
સ્થિર શૂન્ય ngx_unload_module(શૂન્ય *data);
#endif


સ્થિર ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


સ્થિર ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ખાલી },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      ખાલી },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ખાલી },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      ખાલી },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      ખાલી },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      ખાલી },

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
      ખાલી },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      ખાલી },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      ખાલી },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_અંક_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ખાલી },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ખાલી },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ખાલી },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ખાલી },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      ખાલી },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      ખાલી },

      ngx_null_command
};


સ્થિર ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ખાલી,                                  /* init master */
    ખાલી,                                  /* init module */
    ખાલી,                                  /* init process */
    ખાલી,                                  /* init thread */
    ખાલી,                                  /* exit thread */
    ખાલી,                                  /* exit process */
    ખાલી,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


સ્થિર ngx_uint_t   ngx_show_help;
સ્થિર ngx_uint_t   ngx_show_version;
સ્થિર ngx_uint_t   ngx_show_configure;
સ્થિર u_char      *ngx_prefix;
સ્થિર u_char      *ngx_error_log;
સ્થિર u_char      *ngx_conf_file;
સ્થિર u_char      *ngx_conf_params;
#જો (NGX_CONTROL_API)
સ્થિર u_char      *ngx_control_addr;
#endif
સ્થિર અક્ષર        *ngx_signal;


સ્થિર અક્ષર **ngx_os_environ;


પૂર્ણાંક ngx_cdecl
મુખ્ય(પૂર્ણાંક argc, અક્ષર *અચલ *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    જો (ngx_strerror_init() != NGX_OK) {
        પાછા 1;
    }

    જો (ngx_get_options(argc, argv) != NGX_OK) {
        પાછા 1;
    }

    જો (ngx_show_version) {
        ngx_show_version_info();

        જો (!ngx_test_config) {
            પાછા 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#જો (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    જો (log == ખાલી) {
        પાછા 1;
    }

    /* STUB */
#જો (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, કદ(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    જો (init_cycle.pool == ખાલી) {
        પાછા 1;
    }

    જો (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        પાછા 1;
    }

    જો (ngx_process_options(&init_cycle) != NGX_OK) {
        પાછા 1;
    }

    જો (ngx_os_init(log) != NGX_OK) {
        પાછા 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    જો (ngx_crc32_table_init() != NGX_OK) {
        પાછા 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    જો (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        પાછા 1;
    }

#જો (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    જો (ngx_preinit_modules() != NGX_OK) {
        પાછા 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    જો (cycle == ખાલી) {
        જો (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        પાછા 1;
    }

    જો (ngx_test_config) {
        જો (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        જો (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            માટે (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (શૂન્ય) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (શૂન્ય) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        પાછા 0;
    }

    જો (ngx_signal) {
        પાછા ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    જો (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#જો !(NGX_WIN32)

    જો (ngx_init_signals(cycle->log) != NGX_OK) {
        પાછા 1;
    }

    જો (!ngx_inherited && ccf->daemon) {
        જો (ngx_daemon(cycle->log) != NGX_OK) {
            પાછા 1;
        }

        ngx_daemonized = 1;
    }

    જો (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#જો (NGX_CONTROL_API)
    જો (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } નહીંતર {
        જો (ngx_control_init(ngx_control_addr) != NGX_OK) {
            પાછા 1;
        }
    }
#endif

    જો (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        પાછા 1;
    }

    જો (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        પાછા 1;
    }

    જો (log->file->fd != ngx_stderr) {
        જો (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    જો (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } નહીંતર {
        ngx_master_process_cycle(cycle);
    }

    પાછા 0;
}


સ્થિર શૂન્ય
ngx_show_version_info(શૂન્ય)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    જો (ngx_show_help) {
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
#જો (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#નહીંતર
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#નહીંતર
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    જો (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#જો (NGX_SSL)
        જો (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } નહીંતર {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((અક્ષર *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#નહીંતર
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


સ્થિર ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    જો (inherited == ખાલી) {
        પાછા NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    જો (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       કદ(ngx_listening_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    માટે (p = inherited, v = p; *p; p++) {
        જો (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            જો (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                તોડો;
            }

            v = p + 1;

            ls = ngx_array_ધકેલો(&cycle->listening);
            જો (ls == ખાલી) {
                પાછા NGX_ERROR;
            }

            ngx_memzero(ls, કદ(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    જો (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    પાછા ngx_set_inherited_sockets(cycle);
}


અક્ષર **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    અક્ષર                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    જો (last == ખાલી && ccf->environment) {
        પાછા ccf->environment;
    }

    var = ccf->env.elts;

    માટે (i = 0; i < ccf->env.nelts; i++) {
        જો (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            જાઓ tz_found;
        }
    }

    var = ngx_array_ધકેલો(&ccf->env);
    જો (var == ખાલી) {
        પાછા ખાલી;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    માટે (i = 0; i < ccf->env.nelts; i++) {

        જો (var[i].data[var[i].len] == '=') {
            n++;
            ચાલુ;
        }

        માટે (p = ngx_os_environ; *p; p++) {

            જો (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                તોડો;
            }
        }
    }

    જો (last) {
        env = ngx_alloc((*last + n + 1) * કદ(અક્ષર *), cycle->log);
        જો (env == ખાલી) {
            પાછા ખાલી;
        }

        *last = n;

    } નહીંતર {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        જો (cln == ખાલી) {
            પાછા ખાલી;
        }

        env = ngx_alloc((n + 1) * કદ(અક્ષર *), cycle->log);
        જો (env == ખાલી) {
            પાછા ખાલી;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    માટે (i = 0; i < ccf->env.nelts; i++) {

        જો (var[i].data[var[i].len] == '=') {

            જો (last) {
                env[n++] = (અક્ષર *) var[i].data;
                ચાલુ;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            જો (cln == ખાલી) {
                પાછા ખાલી;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            જો (str == ખાલી) {
                પાછા ખાલી;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            ચાલુ;
        }

        માટે (p = ngx_os_environ; *p; p++) {

            જો (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                તોડો;
            }
        }
    }

    env[n] = ખાલી;

    જો (last == ખાલી) {
        ccf->environment = env;
        environ = env;
    }

    પાછા env;
}


સ્થિર શૂન્ય
ngx_cleanup_environment(શૂન્ય *data)
{
    અક્ષર  **env = data;

    જો (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        પાછા;
    }

    ngx_free(env);
}


સ્થિર શૂન્ય
ngx_cleanup_environment_variable(શૂન્ય *data)
{
    અક્ષર  *var = data;

    અક્ષર  **p;

    માટે (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        જો (*p == var) {
            પાછા;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_દ્વિઅંકી(ngx_cycle_t *cycle, અક્ષર *અચલ *argv)
{
    અક્ષર             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, કદ(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    જો (env == ખાલી) {
        પાછા NGX_INVALID_PID;
    }

    var = ngx_alloc(કદ(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    જો (var == ખાલી) {
        ngx_free(env);
        પાછા NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", કદ(NGINX_VAR));

    ls = cycle->listening.elts;
    માટે (i = 0; i < cycle->listening.nelts; i++) {
        જો (ls[i].ignore) {
            ચાલુ;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#જો (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#જો (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    જો (env[n] != ખાલી) {
        n++;
    }
#endif

    env[n] = ખાલી;

#જો (NGX_DEBUG)
    {
    અક્ષર  **e;
    માટે (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (અક્ષર *અચલ *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    જો (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        પાછા NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    જો (pid == NGX_INVALID_PID) {
        જો (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    પાછા pid;
}


સ્થિર ngx_int_t
ngx_get_options(પૂર્ણાંક argc, અક્ષર *અચલ *argv)
{
    u_char     *p;
    ngx_int_t   i;

    માટે (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        જો (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            પાછા NGX_ERROR;
        }

        જ્યારે (*p) {

            પસંદગી (*p++) {

            સ્થિતિ '?':
            સ્થિતિ 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                તોડો;

            સ્થિતિ 'v':
                ngx_show_version = 1;
                તોડો;

            સ્થિતિ 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                તોડો;

            સ્થિતિ 't':
                ngx_test_config = 1;
                તોડો;

            સ્થિતિ 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                તોડો;

            સ્થિતિ 'q':
                ngx_quiet_mode = 1;
                તોડો;

            સ્થિતિ 'p':
                જો (*p) {
                    ngx_prefix = p;
                    જાઓ next;
                }

                જો (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    જાઓ next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                પાછા NGX_ERROR;

            સ્થિતિ 'e':
                જો (*p) {
                    ngx_error_log = p;

                } નહીંતર જો (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } નહીંતર {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    પાછા NGX_ERROR;
                }

                જો (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                જાઓ next;

            સ્થિતિ 'c':
                જો (*p) {
                    ngx_conf_file = p;
                    જાઓ next;
                }

                જો (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    જાઓ next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                પાછા NGX_ERROR;

            સ્થિતિ 'g':
                જો (*p) {
                    ngx_conf_params = p;
                    જાઓ next;
                }

                જો (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    જાઓ next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                પાછા NGX_ERROR;

            સ્થિતિ 's':
                જો (*p) {
                    ngx_signal = (અક્ષર *) p;

                } નહીંતર જો (argv[++i]) {
                    ngx_signal = argv[i];

                } નહીંતર {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    પાછા NGX_ERROR;
                }

                જો (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    જાઓ next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                પાછા NGX_ERROR;

#જો (NGX_CONTROL_API)
            સ્થિતિ 'l':
                ngx_control_api_enabled = 1;

                જો (*p) {
                    ngx_control_addr = p;
                    જાઓ next;
                }

                જો (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    જાઓ next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                પાછા NGX_ERROR;
#endif

            મૂળભૂત:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                પાછા NGX_ERROR;
            }
        }

    next:

        ચાલુ;
    }

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, પૂર્ણાંક argc, અક્ષર *અચલ *argv)
{
#જો (NGX_FREEBSD)

    ngx_os_argv = (અક્ષર **) argv;
    ngx_argc = argc;
    ngx_argv = (અક્ષર **) argv;

#નહીંતર
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (અક્ષર **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * કદ(અક્ષર *), cycle->log);
    જો (ngx_argv == ખાલી) {
        પાછા NGX_ERROR;
    }

    માટે (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        જો (ngx_argv[i] == ખાલી) {
            પાછા NGX_ERROR;
        }

        (શૂન્ય) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ખાલી;

#endif

    ngx_os_environ = environ;

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    જો (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        જો (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            જો (p == ખાલી) {
                પાછા NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } નહીંતર {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        જો (p == ખાલી) {
            પાછા NGX_ERROR;
        }

        જો (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            પાછા NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#નહીંતર

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#નહીંતર
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    જો (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } નહીંતર {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    જો (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        પાછા NGX_ERROR;
    }

    માટે (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        જો (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            તોડો;
        }
    }

    જો (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } નહીંતર {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    જો (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    જો (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    પાછા NGX_OK;
}


સ્થિર શૂન્ય *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, કદ(ngx_core_conf_t));
    જો (ccf == ખાલી) {
        પાછા ખાલી;
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

    જો (ngx_array_init(&ccf->env, cycle->pool, 1, કદ(ngx_str_t))
        != NGX_OK)
    {
        પાછા ખાલી;
    }

    પાછા ccf;
}


સ્થિર અક્ષર *
ngx_core_module_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_કિંમત(ccf->daemon, 1);
    ngx_conf_init_કિંમત(ccf->master, 1);
    ngx_conf_init_msec_કિંમત(ccf->timer_resolution, 0);
    ngx_conf_init_msec_કિંમત(ccf->shutdown_timeout, 0);

    ngx_conf_init_કિંમત(ccf->worker_processes, 1);
    ngx_conf_init_કિંમત(ccf->debug_points, 0);

#જો (NGX_HAVE_CPU_AFFINITY)

    જો (!ccf->cpu_affinity_auto
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


    જો (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    જો (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + કદ(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    જો (ccf->oldpid.data == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, કદ(NGX_OLDPID_EXT));


    જો (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        જો (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            પાછા NGX_CONF_ERROR;
        }

        જો (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            પાછા NGX_CONF_ERROR;
        }

        જો (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            પાછા NGX_CONF_ERROR;
        }
    }


#જો !(NGX_WIN32)

    જો (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        સંરચના group   *grp;
        સંરચના passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        જો (pwd == ખાલી) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            પાછા NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        જો (grp == ખાલી) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            પાછા NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    જો (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    જો (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    જો (lock_file.len) {
        lock_file.len--;

        જો (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += કદ(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        જો (cycle->lock_file.data == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

    } નહીંતર {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + કદ(".accept"));
        જો (cycle->lock_file.data == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", કદ(".accept"));
    }
    }

#endif

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
#જો (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    પાછા NGX_CONF_OK;

#નહીંતર

    ngx_core_conf_t  *ccf = conf;

    અક્ષર             *group;
    સંરચના passwd    *pwd;
    સંરચના group     *grp;
    ngx_str_t        *કિંમત;

    જો (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        પાછા "is duplicate";
    }

    જો (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        પાછા NGX_CONF_OK;
    }

    કિંમત = cf->args->elts;

    ccf->username = (અક્ષર *) કિંમત[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((અચલ અક્ષર *) કિંમત[1].data);
    જો (pwd == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", કિંમત[1].data);
        પાછા NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (અક્ષર *) ((cf->args->nelts == 2) ? કિંમત[1].data : કિંમત[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    જો (grp == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        પાછા NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    પાછા NGX_CONF_OK;

#endif
}


સ્થિર અક્ષર *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *કિંમત, *var;
    ngx_uint_t   i;

    var = ngx_array_ધકેલો(&ccf->env);
    જો (var == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    કિંમત = cf->args->elts;
    *var = કિંમત[1];

    માટે (i = 0; i < કિંમત[1].len; i++) {

        જો (કિંમત[1].data[i] == '=') {

            var->len = i;

            પાછા NGX_CONF_OK;
        }
    }

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *કિંમત;
    ngx_uint_t        n, minus;

    જો (ccf->priority != 0) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    જો (કિંમત[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } નહીંતર જો (કિંમત[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } નહીંતર {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&કિંમત[1].data[n], કિંમત[1].len - n);
    જો (ccf->priority == NGX_ERROR) {
        પાછા "invalid number";
    }

    જો (minus) {
        ccf->priority = -ccf->priority;
    }

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
#જો (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *કિંમત;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    જો (ccf->cpu_affinity) {
        પાછા "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * કદ(ngx_cpuset_t));
    જો (mask == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    કિંમત = cf->args->elts;

    જો (ngx_strcmp(કિંમત[1].data, "auto") == 0) {

        જો (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            પાછા NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        માટે (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } નહીંતર {
        n = 1;
    }

    માટે ( /* void */ ; n < cf->args->nelts; n++) {

        જો (કિંમત[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            પાછા NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        માટે (p = કિંમત[n].data + કિંમત[n].len - 1;
             p >= કિંમત[n].data;
             p--)
        {
            ch = *p;

            જો (ch == ' ') {
                ચાલુ;
            }

            i++;

            જો (ch == '0') {
                ચાલુ;
            }

            જો (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                ચાલુ;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            પાછા NGX_CONF_ERROR;
        }
    }

#નહીંતર

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    પાછા NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#જો (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    સ્થિર ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    જો (ccf->cpu_affinity == ખાલી) {
        પાછા ખાલી;
    }

    જો (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        માટે (i = 0, j = n; /* void */ ; i++) {

            જો (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                તોડો;
            }

            જો (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                પાછા ખાલી;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        પાછા &result;
    }

    જો (ccf->cpu_affinity_n > n) {
        પાછા &ccf->cpu_affinity[n];
    }

    પાછા &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#નહીંતર

    પાછા ખાલી;

#endif
}


સ્થિર અક્ષર *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    ngx_str_t        *કિંમત;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    જો (ccf->worker_processes != NGX_CONF_UNSET) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    જો (ngx_strcmp(કિંમત[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        પાછા NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(કિંમત[1].data, કિંમત[1].len);

    જો (ccf->worker_processes == NGX_ERROR) {
        પાછા "invalid value";
    }

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
#જો (NGX_HAVE_DLOPEN)
    શૂન્ય                *handle;
    અક્ષર               **names, **order;
    ngx_str_t           *કિંમત, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    જો (cf->cycle->modules_used) {
        પાછા "is specified too late";
    }

    કિંમત = cf->args->elts;

    file = કિંમત[1];

    જો (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    જો (cln == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    જો (handle == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        પાછા NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    જો (modules == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &કિંમત[1], "ngx_modules", ngx_dlerror());
        પાછા NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    જો (names == ખાલી) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &કિંમત[1], "ngx_module_names", ngx_dlerror());
        પાછા NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    માટે (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        જો (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            પાછા NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    પાછા NGX_CONF_OK;

#નહીંતર

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    પાછા NGX_CONF_ERROR;

#endif
}


#જો (NGX_HAVE_DLOPEN)

સ્થિર શૂન્ય
ngx_unload_module(શૂન્ય *data)
{
    શૂન્ય  *handle = data;

    જો (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
