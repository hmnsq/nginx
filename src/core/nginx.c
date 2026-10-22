
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
static ngx_int_t ngx_get_options(정수 argc, char *const *argv);
static ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
static ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, 정수 argc, char *const *argv);
static void *ngx_core_module_create_conf(ngx_cycle_t *cycle);
static char *ngx_core_module_init_conf(ngx_cycle_t *cycle, void *conf);
static char *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
#만약 (NGX_HAVE_DLOPEN)
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
#만약 (NGX_CONTROL_API)
static u_char      *ngx_control_addr;
#endif
static char        *ngx_signal;


static char **ngx_os_environ;


정수 ngx_cdecl
main(정수 argc, char *const *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    만약 (ngx_strerror_init() != NGX_OK) {
        반환 1;
    }

    만약 (ngx_get_options(argc, argv) != NGX_OK) {
        반환 1;
    }

    만약 (ngx_show_version) {
        ngx_show_version_info();

        만약 (!ngx_test_config) {
            반환 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#만약 (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    만약 (log == NULL) {
        반환 1;
    }

    /* STUB */
#만약 (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    만약 (init_cycle.pool == NULL) {
        반환 1;
    }

    만약 (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        반환 1;
    }

    만약 (ngx_process_options(&init_cycle) != NGX_OK) {
        반환 1;
    }

    만약 (ngx_os_init(log) != NGX_OK) {
        반환 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    만약 (ngx_crc32_table_init() != NGX_OK) {
        반환 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    만약 (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        반환 1;
    }

#만약 (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    만약 (ngx_preinit_modules() != NGX_OK) {
        반환 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    만약 (cycle == NULL) {
        만약 (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        반환 1;
    }

    만약 (ngx_test_config) {
        만약 (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        만약 (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            위해 (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (void) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (void) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        반환 0;
    }

    만약 (ngx_signal) {
        반환 ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    만약 (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#만약 !(NGX_WIN32)

    만약 (ngx_init_signals(cycle->log) != NGX_OK) {
        반환 1;
    }

    만약 (!ngx_inherited && ccf->daemon) {
        만약 (ngx_daemon(cycle->log) != NGX_OK) {
            반환 1;
        }

        ngx_daemonized = 1;
    }

    만약 (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#만약 (NGX_CONTROL_API)
    만약 (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } 아니면 {
        만약 (ngx_control_init(ngx_control_addr) != NGX_OK) {
            반환 1;
        }
    }
#endif

    만약 (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        반환 1;
    }

    만약 (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        반환 1;
    }

    만약 (log->file->fd != ngx_stderr) {
        만약 (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    만약 (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } 아니면 {
        ngx_master_process_cycle(cycle);
    }

    반환 0;
}


static void
ngx_show_version_info(void)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    만약 (ngx_show_help) {
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
#만약 (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#아니면
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#아니면
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    만약 (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#만약 (NGX_SSL)
        만약 (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } 아니면 {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((char *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#아니면
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

    만약 (inherited == NULL) {
        반환 NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    만약 (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       sizeof(ngx_listening_t))
        != NGX_OK)
    {
        반환 NGX_ERROR;
    }

    위해 (p = inherited, v = p; *p; p++) {
        만약 (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            만약 (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                break;
            }

            v = p + 1;

            ls = ngx_array_push(&cycle->listening);
            만약 (ls == NULL) {
                반환 NGX_ERROR;
            }

            ngx_memzero(ls, sizeof(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    만약 (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    반환 ngx_set_inherited_sockets(cycle);
}


char **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    char                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    만약 (last == NULL && ccf->environment) {
        반환 ccf->environment;
    }

    var = ccf->env.elts;

    위해 (i = 0; i < ccf->env.nelts; i++) {
        만약 (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            goto tz_found;
        }
    }

    var = ngx_array_push(&ccf->env);
    만약 (var == NULL) {
        반환 NULL;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    위해 (i = 0; i < ccf->env.nelts; i++) {

        만약 (var[i].data[var[i].len] == '=') {
            n++;
            continue;
        }

        위해 (p = ngx_os_environ; *p; p++) {

            만약 (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                break;
            }
        }
    }

    만약 (last) {
        env = ngx_alloc((*last + n + 1) * sizeof(char *), cycle->log);
        만약 (env == NULL) {
            반환 NULL;
        }

        *last = n;

    } 아니면 {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        만약 (cln == NULL) {
            반환 NULL;
        }

        env = ngx_alloc((n + 1) * sizeof(char *), cycle->log);
        만약 (env == NULL) {
            반환 NULL;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    위해 (i = 0; i < ccf->env.nelts; i++) {

        만약 (var[i].data[var[i].len] == '=') {

            만약 (last) {
                env[n++] = (char *) var[i].data;
                continue;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            만약 (cln == NULL) {
                반환 NULL;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            만약 (str == NULL) {
                반환 NULL;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            continue;
        }

        위해 (p = ngx_os_environ; *p; p++) {

            만약 (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                break;
            }
        }
    }

    env[n] = NULL;

    만약 (last == NULL) {
        ccf->environment = env;
        environ = env;
    }

    반환 env;
}


static void
ngx_cleanup_environment(void *data)
{
    char  **env = data;

    만약 (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        반환;
    }

    ngx_free(env);
}


static void
ngx_cleanup_environment_variable(void *data)
{
    char  *var = data;

    char  **p;

    위해 (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        만약 (*p == var) {
            반환;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv)
{
    char             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, sizeof(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    만약 (env == NULL) {
        반환 NGX_INVALID_PID;
    }

    var = ngx_alloc(sizeof(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    만약 (var == NULL) {
        ngx_free(env);
        반환 NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", sizeof(NGINX_VAR));

    ls = cycle->listening.elts;
    위해 (i = 0; i < cycle->listening.nelts; i++) {
        만약 (ls[i].ignore) {
            continue;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#만약 (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#만약 (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    만약 (env[n] != NULL) {
        n++;
    }
#endif

    env[n] = NULL;

#만약 (NGX_DEBUG)
    {
    char  **e;
    위해 (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (char *const *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    만약 (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        반환 NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    만약 (pid == NGX_INVALID_PID) {
        만약 (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    반환 pid;
}


static ngx_int_t
ngx_get_options(정수 argc, char *const *argv)
{
    u_char     *p;
    ngx_int_t   i;

    위해 (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        만약 (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            반환 NGX_ERROR;
        }

        동안 (*p) {

            switch (*p++) {

            case '?':
            case 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                break;

            case 'v':
                ngx_show_version = 1;
                break;

            case 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                break;

            case 't':
                ngx_test_config = 1;
                break;

            case 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                break;

            case 'q':
                ngx_quiet_mode = 1;
                break;

            case 'p':
                만약 (*p) {
                    ngx_prefix = p;
                    goto next;
                }

                만약 (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                반환 NGX_ERROR;

            case 'e':
                만약 (*p) {
                    ngx_error_log = p;

                } 아니면 만약 (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } 아니면 {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    반환 NGX_ERROR;
                }

                만약 (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                goto next;

            case 'c':
                만약 (*p) {
                    ngx_conf_file = p;
                    goto next;
                }

                만약 (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                반환 NGX_ERROR;

            case 'g':
                만약 (*p) {
                    ngx_conf_params = p;
                    goto next;
                }

                만약 (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                반환 NGX_ERROR;

            case 's':
                만약 (*p) {
                    ngx_signal = (char *) p;

                } 아니면 만약 (argv[++i]) {
                    ngx_signal = argv[i];

                } 아니면 {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    반환 NGX_ERROR;
                }

                만약 (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    goto next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                반환 NGX_ERROR;

#만약 (NGX_CONTROL_API)
            case 'l':
                ngx_control_api_enabled = 1;

                만약 (*p) {
                    ngx_control_addr = p;
                    goto next;
                }

                만약 (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                반환 NGX_ERROR;
#endif

            default:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                반환 NGX_ERROR;
            }
        }

    next:

        continue;
    }

    반환 NGX_OK;
}


static ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, 정수 argc, char *const *argv)
{
#만약 (NGX_FREEBSD)

    ngx_os_argv = (char **) argv;
    ngx_argc = argc;
    ngx_argv = (char **) argv;

#아니면
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (char **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * sizeof(char *), cycle->log);
    만약 (ngx_argv == NULL) {
        반환 NGX_ERROR;
    }

    위해 (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        만약 (ngx_argv[i] == NULL) {
            반환 NGX_ERROR;
        }

        (void) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = NULL;

#endif

    ngx_os_environ = environ;

    반환 NGX_OK;
}


static ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    만약 (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        만약 (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            만약 (p == NULL) {
                반환 NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } 아니면 {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        만약 (p == NULL) {
            반환 NGX_ERROR;
        }

        만약 (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            반환 NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#아니면

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#아니면
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    만약 (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } 아니면 {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    만약 (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        반환 NGX_ERROR;
    }

    위해 (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        만약 (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            break;
        }
    }

    만약 (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } 아니면 {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    만약 (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    만약 (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    반환 NGX_OK;
}


static void *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, sizeof(ngx_core_conf_t));
    만약 (ccf == NULL) {
        반환 NULL;
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

    만약 (ngx_array_init(&ccf->env, cycle->pool, 1, sizeof(ngx_str_t))
        != NGX_OK)
    {
        반환 NULL;
    }

    반환 ccf;
}


static char *
ngx_core_module_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_value(ccf->daemon, 1);
    ngx_conf_init_value(ccf->master, 1);
    ngx_conf_init_msec_value(ccf->timer_resolution, 0);
    ngx_conf_init_msec_value(ccf->shutdown_timeout, 0);

    ngx_conf_init_value(ccf->worker_processes, 1);
    ngx_conf_init_value(ccf->debug_points, 0);

#만약 (NGX_HAVE_CPU_AFFINITY)

    만약 (!ccf->cpu_affinity_auto
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


    만약 (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    만약 (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        반환 NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + sizeof(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    만약 (ccf->oldpid.data == NULL) {
        반환 NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, sizeof(NGX_OLDPID_EXT));


    만약 (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        만약 (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            반환 NGX_CONF_ERROR;
        }

        만약 (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            반환 NGX_CONF_ERROR;
        }

        만약 (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            반환 NGX_CONF_ERROR;
        }
    }


#만약 !(NGX_WIN32)

    만약 (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        struct group   *grp;
        struct passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        만약 (pwd == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            반환 NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        만약 (grp == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            반환 NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    만약 (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    만약 (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        반환 NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    만약 (lock_file.len) {
        lock_file.len--;

        만약 (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += sizeof(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        만약 (cycle->lock_file.data == NULL) {
            반환 NGX_CONF_ERROR;
        }

    } 아니면 {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + sizeof(".accept"));
        만약 (cycle->lock_file.data == NULL) {
            반환 NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", sizeof(".accept"));
    }
    }

#endif

    반환 NGX_CONF_OK;
}


static char *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#만약 (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    반환 NGX_CONF_OK;

#아니면

    ngx_core_conf_t  *ccf = conf;

    char             *group;
    struct passwd    *pwd;
    struct group     *grp;
    ngx_str_t        *value;

    만약 (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        반환 "is duplicate";
    }

    만약 (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        반환 NGX_CONF_OK;
    }

    value = cf->args->elts;

    ccf->username = (char *) value[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((const char *) value[1].data);
    만약 (pwd == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", value[1].data);
        반환 NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (char *) ((cf->args->nelts == 2) ? value[1].data : value[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    만약 (grp == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        반환 NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    반환 NGX_CONF_OK;

#endif
}


static char *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *value, *var;
    ngx_uint_t   i;

    var = ngx_array_push(&ccf->env);
    만약 (var == NULL) {
        반환 NGX_CONF_ERROR;
    }

    value = cf->args->elts;
    *var = value[1];

    위해 (i = 0; i < value[1].len; i++) {

        만약 (value[1].data[i] == '=') {

            var->len = i;

            반환 NGX_CONF_OK;
        }
    }

    반환 NGX_CONF_OK;
}


static char *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *value;
    ngx_uint_t        n, minus;

    만약 (ccf->priority != 0) {
        반환 "is duplicate";
    }

    value = cf->args->elts;

    만약 (value[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } 아니면 만약 (value[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } 아니면 {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&value[1].data[n], value[1].len - n);
    만약 (ccf->priority == NGX_ERROR) {
        반환 "invalid number";
    }

    만약 (minus) {
        ccf->priority = -ccf->priority;
    }

    반환 NGX_CONF_OK;
}


static char *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#만약 (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *value;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    만약 (ccf->cpu_affinity) {
        반환 "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * sizeof(ngx_cpuset_t));
    만약 (mask == NULL) {
        반환 NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    value = cf->args->elts;

    만약 (ngx_strcmp(value[1].data, "auto") == 0) {

        만약 (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            반환 NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        위해 (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } 아니면 {
        n = 1;
    }

    위해 ( /* void */ ; n < cf->args->nelts; n++) {

        만약 (value[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            반환 NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        위해 (p = value[n].data + value[n].len - 1;
             p >= value[n].data;
             p--)
        {
            ch = *p;

            만약 (ch == ' ') {
                continue;
            }

            i++;

            만약 (ch == '0') {
                continue;
            }

            만약 (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                continue;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            반환 NGX_CONF_ERROR;
        }
    }

#아니면

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    반환 NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#만약 (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    static ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    만약 (ccf->cpu_affinity == NULL) {
        반환 NULL;
    }

    만약 (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        위해 (i = 0, j = n; /* void */ ; i++) {

            만약 (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                break;
            }

            만약 (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                반환 NULL;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        반환 &result;
    }

    만약 (ccf->cpu_affinity_n > n) {
        반환 &ccf->cpu_affinity[n];
    }

    반환 &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#아니면

    반환 NULL;

#endif
}


static char *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_str_t        *value;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    만약 (ccf->worker_processes != NGX_CONF_UNSET) {
        반환 "is duplicate";
    }

    value = cf->args->elts;

    만약 (ngx_strcmp(value[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        반환 NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(value[1].data, value[1].len);

    만약 (ccf->worker_processes == NGX_ERROR) {
        반환 "invalid value";
    }

    반환 NGX_CONF_OK;
}


static char *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
#만약 (NGX_HAVE_DLOPEN)
    void                *handle;
    char               **names, **order;
    ngx_str_t           *value, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    만약 (cf->cycle->modules_used) {
        반환 "is specified too late";
    }

    value = cf->args->elts;

    file = value[1];

    만약 (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        반환 NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    만약 (cln == NULL) {
        반환 NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    만약 (handle == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        반환 NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    만약 (modules == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_modules", ngx_dlerror());
        반환 NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    만약 (names == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_module_names", ngx_dlerror());
        반환 NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    위해 (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        만약 (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            반환 NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    반환 NGX_CONF_OK;

#아니면

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    반환 NGX_CONF_ERROR;

#endif
}


#만약 (NGX_HAVE_DLOPEN)

static void
ngx_unload_module(void *data)
{
    void  *handle = data;

    만약 (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
