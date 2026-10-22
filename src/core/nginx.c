
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <nginx.h>


static void ngx_show_version_info(void);
static ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
static void ngx_cleanup_environment(void *data);
static void ngx_cleanup_environment_variable(void *data);
static ngx_int_t ngx_get_options(ចំនួនគត់ argc, var_na *const *argv);
static ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
static ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, ចំនួនគត់ argc, var_na *const *argv);
static void *ngx_core_module_create_conf(ngx_cycle_t *cycle);
static var_na *ngx_core_module_init_conf(ngx_cycle_t *cycle, void *conf);
static var_na *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static var_na *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static var_na *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static var_na *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static var_na *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static var_na *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
#ប្រសិនបើ (NGX_HAVE_DLOPEN)
static void ngx_unload_module(void *data);
#endif


static ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


static ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      NULL },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      NULL },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      NULL },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      NULL },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      NULL },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      NULL },

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
      NULL },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      NULL },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      NULL },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      NULL },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      NULL },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      NULL },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      NULL },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      NULL },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_uint_t   ngx_show_help;
static ngx_uint_t   ngx_show_version;
static ngx_uint_t   ngx_show_configure;
static u_char      *ngx_prefix;
static u_char      *ngx_error_log;
static u_char      *ngx_conf_file;
static u_char      *ngx_conf_params;
#ប្រសិនបើ (NGX_CONTROL_API)
static u_char      *ngx_control_addr;
#endif
static var_na        *ngx_signal;


static var_na **ngx_os_environ;


ចំនួនគត់ ngx_cdecl
main(ចំនួនគត់ argc, var_na *const *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    ប្រសិនបើ (ngx_strerror_init() != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_get_options(argc, argv) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_show_version) {
        ngx_show_version_info();

        ប្រសិនបើ (!ngx_test_config) {
            ត្រឡប់ 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#ប្រសិនបើ (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    ប្រសិនបើ (log == NULL) {
        ត្រឡប់ 1;
    }

    /* STUB */
#ប្រសិនបើ (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, parimaa_na(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    ប្រសិនបើ (init_cycle.pool == NULL) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_process_options(&init_cycle) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_os_init(log) != NGX_OK) {
        ត្រឡប់ 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    ប្រសិនបើ (ngx_crc32_table_init() != NGX_OK) {
        ត្រឡប់ 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    ប្រសិនបើ (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        ត្រឡប់ 1;
    }

#ប្រសិនបើ (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    ប្រសិនបើ (ngx_preinit_modules() != NGX_OK) {
        ត្រឡប់ 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    ប្រសិនបើ (cycle == NULL) {
        ប្រសិនបើ (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_test_config) {
        ប្រសិនបើ (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        ប្រសិនបើ (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            សម្រាប់ (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (void) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (void) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        ត្រឡប់ 0;
    }

    ប្រសិនបើ (ngx_signal) {
        ត្រឡប់ ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ប្រសិនបើ (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#ប្រសិនបើ !(NGX_WIN32)

    ប្រសិនបើ (ngx_init_signals(cycle->log) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (!ngx_inherited && ccf->daemon) {
        ប្រសិនបើ (ngx_daemon(cycle->log) != NGX_OK) {
            ត្រឡប់ 1;
        }

        ngx_daemonized = 1;
    }

    ប្រសិនបើ (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#ប្រសិនបើ (NGX_CONTROL_API)
    ប្រសិនបើ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } បើមិនដូច្នេះ {
        ប្រសិនបើ (ngx_control_init(ngx_control_addr) != NGX_OK) {
            ត្រឡប់ 1;
        }
    }
#endif

    ប្រសិនបើ (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        ត្រឡប់ 1;
    }

    ប្រសិនបើ (log->file->fd != ngx_stderr) {
        ប្រសិនបើ (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    ប្រសិនបើ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } បើមិនដូច្នេះ {
        ngx_master_process_cycle(cycle);
    }

    ត្រឡប់ 0;
}


static void
ngx_show_version_info(void)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    ប្រសិនបើ (ngx_show_help) {
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
#ប្រសិនបើ (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#បើមិនដូច្នេះ
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#បើមិនដូច្នេះ
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    ប្រសិនបើ (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#ប្រសិនបើ (NGX_SSL)
        ប្រសិនបើ (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } បើមិនដូច្នេះ {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((var_na *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#បើមិនដូច្នេះ
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


static ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    ប្រសិនបើ (inherited == NULL) {
        ត្រឡប់ NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    ប្រសិនបើ (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       parimaa_na(ngx_listening_t))
        != NGX_OK)
    {
        ត្រឡប់ NGX_ERROR;
    }

    សម្រាប់ (p = inherited, v = p; *p; p++) {
        ប្រសិនបើ (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            ប្រសិនបើ (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                bha_nga;
            }

            v = p + 1;

            ls = ngx_array_push(&cycle->listening);
            ប្រសិនបើ (ls == NULL) {
                ត្រឡប់ NGX_ERROR;
            }

            ngx_memzero(ls, parimaa_na(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    ប្រសិនបើ (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    ត្រឡប់ ngx_set_inherited_sockets(cycle);
}


var_na **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    var_na                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ប្រសិនបើ (last == NULL && ccf->environment) {
        ត្រឡប់ ccf->environment;
    }

    var = ccf->env.elts;

    សម្រាប់ (i = 0; i < ccf->env.nelts; i++) {
        ប្រសិនបើ (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            goto tz_found;
        }
    }

    var = ngx_array_push(&ccf->env);
    ប្រសិនបើ (var == NULL) {
        ត្រឡប់ NULL;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    សម្រាប់ (i = 0; i < ccf->env.nelts; i++) {

        ប្រសិនបើ (var[i].data[var[i].len] == '=') {
            n++;
            continue;
        }

        សម្រាប់ (p = ngx_os_environ; *p; p++) {

            ប្រសិនបើ (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                bha_nga;
            }
        }
    }

    ប្រសិនបើ (last) {
        env = ngx_alloc((*last + n + 1) * parimaa_na(var_na *), cycle->log);
        ប្រសិនបើ (env == NULL) {
            ត្រឡប់ NULL;
        }

        *last = n;

    } បើមិនដូច្នេះ {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        ប្រសិនបើ (cln == NULL) {
            ត្រឡប់ NULL;
        }

        env = ngx_alloc((n + 1) * parimaa_na(var_na *), cycle->log);
        ប្រសិនបើ (env == NULL) {
            ត្រឡប់ NULL;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    សម្រាប់ (i = 0; i < ccf->env.nelts; i++) {

        ប្រសិនបើ (var[i].data[var[i].len] == '=') {

            ប្រសិនបើ (last) {
                env[n++] = (var_na *) var[i].data;
                continue;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            ប្រសិនបើ (cln == NULL) {
                ត្រឡប់ NULL;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            ប្រសិនបើ (str == NULL) {
                ត្រឡប់ NULL;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            continue;
        }

        សម្រាប់ (p = ngx_os_environ; *p; p++) {

            ប្រសិនបើ (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                bha_nga;
            }
        }
    }

    env[n] = NULL;

    ប្រសិនបើ (last == NULL) {
        ccf->environment = env;
        environ = env;
    }

    ត្រឡប់ env;
}


static void
ngx_cleanup_environment(void *data)
{
    var_na  **env = data;

    ប្រសិនបើ (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        ត្រឡប់;
    }

    ngx_free(env);
}


static void
ngx_cleanup_environment_variable(void *data)
{
    var_na  *var = data;

    var_na  **p;

    សម្រាប់ (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        ប្រសិនបើ (*p == var) {
            ត្រឡប់;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_binary(ngx_cycle_t *cycle, var_na *const *argv)
{
    var_na             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, parimaa_na(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    ប្រសិនបើ (env == NULL) {
        ត្រឡប់ NGX_INVALID_PID;
    }

    var = ngx_alloc(parimaa_na(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    ប្រសិនបើ (var == NULL) {
        ngx_free(env);
        ត្រឡប់ NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", parimaa_na(NGINX_VAR));

    ls = cycle->listening.elts;
    សម្រាប់ (i = 0; i < cycle->listening.nelts; i++) {
        ប្រសិនបើ (ls[i].ignore) {
            continue;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#ប្រសិនបើ (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#ប្រសិនបើ (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    ប្រសិនបើ (env[n] != NULL) {
        n++;
    }
#endif

    env[n] = NULL;

#ប្រសិនបើ (NGX_DEBUG)
    {
    var_na  **e;
    សម្រាប់ (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (var_na *const *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ប្រសិនបើ (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        ត្រឡប់ NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    ប្រសិនបើ (pid == NGX_INVALID_PID) {
        ប្រសិនបើ (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    ត្រឡប់ pid;
}


static ngx_int_t
ngx_get_options(ចំនួនគត់ argc, var_na *const *argv)
{
    u_char     *p;
    ngx_int_t   i;

    សម្រាប់ (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        ប្រសិនបើ (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            ត្រឡប់ NGX_ERROR;
        }

        ខណៈពេល (*p) {

            switch (*p++) {

            pak_sa '?':
            pak_sa 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                bha_nga;

            pak_sa 'v':
                ngx_show_version = 1;
                bha_nga;

            pak_sa 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                bha_nga;

            pak_sa 't':
                ngx_test_config = 1;
                bha_nga;

            pak_sa 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                bha_nga;

            pak_sa 'q':
                ngx_quiet_mode = 1;
                bha_nga;

            pak_sa 'p':
                ប្រសិនបើ (*p) {
                    ngx_prefix = p;
                    goto next;
                }

                ប្រសិនបើ (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                ត្រឡប់ NGX_ERROR;

            pak_sa 'e':
                ប្រសិនបើ (*p) {
                    ngx_error_log = p;

                } បើមិនដូច្នេះ ប្រសិនបើ (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } បើមិនដូច្នេះ {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    ត្រឡប់ NGX_ERROR;
                }

                ប្រសិនបើ (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                goto next;

            pak_sa 'c':
                ប្រសិនបើ (*p) {
                    ngx_conf_file = p;
                    goto next;
                }

                ប្រសិនបើ (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                ត្រឡប់ NGX_ERROR;

            pak_sa 'g':
                ប្រសិនបើ (*p) {
                    ngx_conf_params = p;
                    goto next;
                }

                ប្រសិនបើ (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                ត្រឡប់ NGX_ERROR;

            pak_sa 's':
                ប្រសិនបើ (*p) {
                    ngx_signal = (var_na *) p;

                } បើមិនដូច្នេះ ប្រសិនបើ (argv[++i]) {
                    ngx_signal = argv[i];

                } បើមិនដូច្នេះ {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    ត្រឡប់ NGX_ERROR;
                }

                ប្រសិនបើ (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    goto next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                ត្រឡប់ NGX_ERROR;

#ប្រសិនបើ (NGX_CONTROL_API)
            pak_sa 'l':
                ngx_control_api_enabled = 1;

                ប្រសិនបើ (*p) {
                    ngx_control_addr = p;
                    goto next;
                }

                ប្រសិនបើ (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                ត្រឡប់ NGX_ERROR;
#endif

            default:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                ត្រឡប់ NGX_ERROR;
            }
        }

    next:

        continue;
    }

    ត្រឡប់ NGX_OK;
}


static ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, ចំនួនគត់ argc, var_na *const *argv)
{
#ប្រសិនបើ (NGX_FREEBSD)

    ngx_os_argv = (var_na **) argv;
    ngx_argc = argc;
    ngx_argv = (var_na **) argv;

#បើមិនដូច្នេះ
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (var_na **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * parimaa_na(var_na *), cycle->log);
    ប្រសិនបើ (ngx_argv == NULL) {
        ត្រឡប់ NGX_ERROR;
    }

    សម្រាប់ (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        ប្រសិនបើ (ngx_argv[i] == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

        (void) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = NULL;

#endif

    ngx_os_environ = environ;

    ត្រឡប់ NGX_OK;
}


static ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    ប្រសិនបើ (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        ប្រសិនបើ (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            ប្រសិនបើ (p == NULL) {
                ត្រឡប់ NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } បើមិនដូច្នេះ {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        ប្រសិនបើ (p == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

        ប្រសិនបើ (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            ត្រឡប់ NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#បើមិនដូច្នេះ

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#បើមិនដូច្នេះ
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    ប្រសិនបើ (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } បើមិនដូច្នេះ {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    ប្រសិនបើ (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        ត្រឡប់ NGX_ERROR;
    }

    សម្រាប់ (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        ប្រសិនបើ (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            bha_nga;
        }
    }

    ប្រសិនបើ (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } បើមិនដូច្នេះ {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    ប្រសិនបើ (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    ប្រសិនបើ (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    ត្រឡប់ NGX_OK;
}


static void *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, parimaa_na(ngx_core_conf_t));
    ប្រសិនបើ (ccf == NULL) {
        ត្រឡប់ NULL;
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

    ប្រសិនបើ (ngx_array_init(&ccf->env, cycle->pool, 1, parimaa_na(ngx_str_t))
        != NGX_OK)
    {
        ត្រឡប់ NULL;
    }

    ត្រឡប់ ccf;
}


static var_na *
ngx_core_module_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_value(ccf->daemon, 1);
    ngx_conf_init_value(ccf->master, 1);
    ngx_conf_init_msec_value(ccf->timer_resolution, 0);
    ngx_conf_init_msec_value(ccf->shutdown_timeout, 0);

    ngx_conf_init_value(ccf->worker_processes, 1);
    ngx_conf_init_value(ccf->debug_points, 0);

#ប្រសិនបើ (NGX_HAVE_CPU_AFFINITY)

    ប្រសិនបើ (!ccf->cpu_affinity_auto
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


    ប្រសិនបើ (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    ប្រសិនបើ (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + parimaa_na(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    ប្រសិនបើ (ccf->oldpid.data == NULL) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, parimaa_na(NGX_OLDPID_EXT));


    ប្រសិនបើ (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        ប្រសិនបើ (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ប្រសិនបើ (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ប្រសិនបើ (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            ត្រឡប់ NGX_CONF_ERROR;
        }
    }


#ប្រសិនបើ !(NGX_WIN32)

    ប្រសិនបើ (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        sa_nracanaa group   *grp;
        sa_nracanaa passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        ប្រសិនបើ (pwd == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        ប្រសិនបើ (grp == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    ប្រសិនបើ (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    ប្រសិនបើ (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    ប្រសិនបើ (lock_file.len) {
        lock_file.len--;

        ប្រសិនបើ (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += parimaa_na(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        ប្រសិនបើ (cycle->lock_file.data == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

    } បើមិនដូច្នេះ {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + parimaa_na(".accept"));
        ប្រសិនបើ (cycle->lock_file.data == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", parimaa_na(".accept"));
    }
    }

#endif

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#ប្រសិនបើ (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    ត្រឡប់ NGX_CONF_OK;

#បើមិនដូច្នេះ

    ngx_core_conf_t  *ccf = conf;

    var_na             *group;
    sa_nracanaa passwd    *pwd;
    sa_nracanaa group     *grp;
    ngx_str_t        *value;

    ប្រសិនបើ (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        ត្រឡប់ "is duplicate";
    }

    ប្រសិនបើ (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        ត្រឡប់ NGX_CONF_OK;
    }

    value = cf->args->elts;

    ccf->username = (var_na *) value[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((const var_na *) value[1].data);
    ប្រសិនបើ (pwd == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", value[1].data);
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (var_na *) ((cf->args->nelts == 2) ? value[1].data : value[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    ប្រសិនបើ (grp == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    ត្រឡប់ NGX_CONF_OK;

#endif
}


static var_na *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *value, *var;
    ngx_uint_t   i;

    var = ngx_array_push(&ccf->env);
    ប្រសិនបើ (var == NULL) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    value = cf->args->elts;
    *var = value[1];

    សម្រាប់ (i = 0; i < value[1].len; i++) {

        ប្រសិនបើ (value[1].data[i] == '=') {

            var->len = i;

            ត្រឡប់ NGX_CONF_OK;
        }
    }

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *value;
    ngx_uint_t        n, minus;

    ប្រសិនបើ (ccf->priority != 0) {
        ត្រឡប់ "is duplicate";
    }

    value = cf->args->elts;

    ប្រសិនបើ (value[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } បើមិនដូច្នេះ ប្រសិនបើ (value[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } បើមិនដូច្នេះ {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&value[1].data[n], value[1].len - n);
    ប្រសិនបើ (ccf->priority == NGX_ERROR) {
        ត្រឡប់ "invalid number";
    }

    ប្រសិនបើ (minus) {
        ccf->priority = -ccf->priority;
    }

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#ប្រសិនបើ (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *value;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    ប្រសិនបើ (ccf->cpu_affinity) {
        ត្រឡប់ "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * parimaa_na(ngx_cpuset_t));
    ប្រសិនបើ (mask == NULL) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    value = cf->args->elts;

    ប្រសិនបើ (ngx_strcmp(value[1].data, "auto") == 0) {

        ប្រសិនបើ (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        សម្រាប់ (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } បើមិនដូច្នេះ {
        n = 1;
    }

    សម្រាប់ ( /* void */ ; n < cf->args->nelts; n++) {

        ប្រសិនបើ (value[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            ត្រឡប់ NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        សម្រាប់ (p = value[n].data + value[n].len - 1;
             p >= value[n].data;
             p--)
        {
            ch = *p;

            ប្រសិនបើ (ch == ' ') {
                continue;
            }

            i++;

            ប្រសិនបើ (ch == '0') {
                continue;
            }

            ប្រសិនបើ (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                continue;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            ត្រឡប់ NGX_CONF_ERROR;
        }
    }

#បើមិនដូច្នេះ

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    ត្រឡប់ NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#ប្រសិនបើ (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    static ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    ប្រសិនបើ (ccf->cpu_affinity == NULL) {
        ត្រឡប់ NULL;
    }

    ប្រសិនបើ (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        សម្រាប់ (i = 0, j = n; /* void */ ; i++) {

            ប្រសិនបើ (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                bha_nga;
            }

            ប្រសិនបើ (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                ត្រឡប់ NULL;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        ត្រឡប់ &result;
    }

    ប្រសិនបើ (ccf->cpu_affinity_n > n) {
        ត្រឡប់ &ccf->cpu_affinity[n];
    }

    ត្រឡប់ &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#បើមិនដូច្នេះ

    ត្រឡប់ NULL;

#endif
}


static var_na *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_str_t        *value;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    ប្រសិនបើ (ccf->worker_processes != NGX_CONF_UNSET) {
        ត្រឡប់ "is duplicate";
    }

    value = cf->args->elts;

    ប្រសិនបើ (ngx_strcmp(value[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        ត្រឡប់ NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(value[1].data, value[1].len);

    ប្រសិនបើ (ccf->worker_processes == NGX_ERROR) {
        ត្រឡប់ "invalid value";
    }

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#ប្រសិនបើ (NGX_HAVE_DLOPEN)
    void                *handle;
    var_na               **names, **order;
    ngx_str_t           *value, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    ប្រសិនបើ (cf->cycle->modules_used) {
        ត្រឡប់ "is specified too late";
    }

    value = cf->args->elts;

    file = value[1];

    ប្រសិនបើ (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    ប្រសិនបើ (cln == NULL) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    ប្រសិនបើ (handle == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        ត្រឡប់ NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    ប្រសិនបើ (modules == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_modules", ngx_dlerror());
        ត្រឡប់ NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    ប្រសិនបើ (names == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_module_names", ngx_dlerror());
        ត្រឡប់ NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    សម្រាប់ (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        ប្រសិនបើ (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    ត្រឡប់ NGX_CONF_OK;

#បើមិនដូច្នេះ

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    ត្រឡប់ NGX_CONF_ERROR;

#endif
}


#ប្រសិនបើ (NGX_HAVE_DLOPEN)

static void
ngx_unload_module(void *data)
{
    void  *handle = data;

    ប្រសិនបើ (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
