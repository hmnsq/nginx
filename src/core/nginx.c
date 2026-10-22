
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <nginx.h>


ಸ್ಥಿರ ಶೂನ್ಯ ngx_show_ಆವೃತ್ತಿ_info(ಶೂನ್ಯ);
ಸ್ಥಿರ ngx_int_t ngx_ಕೂಡಿಸು_inherited_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_cleanup_environment(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_cleanup_environment_variable(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ngx_int_t ngx_get_options(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv);
ಸ್ಥಿರ ngx_int_t ngx_process_options(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ngx_int_t ngx_save_argv(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_core_module_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_core_module_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ಗಣ_user(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ಗಣ_env(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ಗಣ_ಆದ್ಯತೆ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ಗಣ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ಗಣ_ಕೆಲಸಗಾರ_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_ತುಂಬು_module(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
#ಆದರೆ (NGX_HAVE_DLOPEN)
ಸ್ಥಿರ ಶೂನ್ಯ ngx_unload_module(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ಸ್ಥಿರ ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_FLAG,
      ngx_conf_ಗಣ_flag_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ಖಾಲಿ },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_FLAG,
      ngx_conf_ಗಣ_flag_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, master),
      ಖಾಲಿ },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_msec_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ಖಾಲಿ },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_str_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, pid),
      ಖಾಲಿ },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_str_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, ಬೀಗ_file),
      ಖಾಲಿ },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_ಗಣ_ಕೆಲಸಗಾರ_processes,
      0,
      0,
      ಖಾಲಿ },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_enum_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE12,
      ngx_ಗಣ_user,
      0,
      0,
      ಖಾಲಿ },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_ಗಣ_ಆದ್ಯತೆ,
      0,
      0,
      ಖಾಲಿ },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_1_ಇನ್ನಷ್ಟು,
      ngx_ಗಣ_cpu_affinity,
      0,
      0,
      ಖಾಲಿ },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_ಅಂಕೆ_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ಖಾಲಿ },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_off_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ಖಾಲಿ },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_msec_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ಖಾಲಿ },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_conf_ಗಣ_str_ಸ್ಥಾನ,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ಖಾಲಿ },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_ಗಣ_env,
      0,
      0,
      ಖಾಲಿ },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE1,
      ngx_ತುಂಬು_module,
      0,
      0,
      ಖಾಲಿ },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ಖಾಲಿ,                                  /* init master */
    ಖಾಲಿ,                                  /* init module */
    ಖಾಲಿ,                                  /* init process */
    ಖಾಲಿ,                                  /* init thread */
    ಖಾಲಿ,                                  /* exit thread */
    ಖಾಲಿ,                                  /* exit process */
    ಖಾಲಿ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_uint_t   ngx_show_help;
ಸ್ಥಿರ ngx_uint_t   ngx_show_ಆವೃತ್ತಿ;
ಸ್ಥಿರ ngx_uint_t   ngx_show_configure;
ಸ್ಥಿರ u_char      *ngx_ಪೂರ್ವಪ್ರತ್ಯಯ;
ಸ್ಥಿರ u_char      *ngx_error_log;
ಸ್ಥಿರ u_char      *ngx_conf_file;
ಸ್ಥಿರ u_char      *ngx_conf_params;
#ಆದರೆ (NGX_CONTROL_API)
ಸ್ಥಿರ u_char      *ngx_control_addr;
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಅಕ್ಷರ        *ngx_signal;


ಸ್ಥಿರ ಅಕ್ಷರ **ngx_os_environ;


ಪೂರ್ಣಾಂಕ ngx_cdecl
ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *ಲಘುಗಣಕ;
    ngx_uint_t        i;
    ngx_ಚಕ್ರ_t      *ಚಕ್ರ, init_ಚಕ್ರ;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    ಆದರೆ (ngx_strerror_init() != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_get_options(argc, argv) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_show_ಆವೃತ್ತಿ) {
        ngx_show_ಆವೃತ್ತಿ_info();

        ಆದರೆ (!ngx_test_config) {
            ಹಿಂತಿರುಗಿ 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_ಸಮಯ_init();

#ಆದರೆ (NGX_PCRE)
    ngx_regex_init();
#ಆದರೆ_ಅಂತ್ಯ

    ngx_pid = ngx_getpid();
    ngx_ಪೋಷಕ = ngx_getppid();

    ಲಘುಗಣಕ = ngx_log_init(ngx_ಪೂರ್ವಪ್ರತ್ಯಯ, ngx_error_log);
    ಆದರೆ (ಲಘುಗಣಕ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ 1;
    }

    /* STUB */
#ಆದರೆ (NGX_OPENSSL)
    ngx_ssl_init(ಲಘುಗಣಕ);
#ಆದರೆ_ಅಂತ್ಯ

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_ಚಕ್ರ, ಗಾತ್ರ(ngx_ಚಕ್ರ_t));
    init_ಚಕ್ರ.ಲಘುಗಣಕ = ಲಘುಗಣಕ;
    ngx_ಚಕ್ರ = &init_ಚಕ್ರ;

    init_ಚಕ್ರ.pool = ngx_create_pool(1024, ಲಘುಗಣಕ);
    ಆದರೆ (init_ಚಕ್ರ.pool == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_save_argv(&init_ಚಕ್ರ, argc, argv) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_process_options(&init_ಚಕ್ರ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_os_init(ಲಘುಗಣಕ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    ಆದರೆ (ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_ಕೋಷ್ಟಕ_init() != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    ಆದರೆ (ngx_ಕೂಡಿಸು_inherited_sockets(&init_ಚಕ್ರ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

#ಆದರೆ (NGX_CONTROL_API)
    ngx_control_preinit();
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_preinit_modules() != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಚಕ್ರ = ngx_init_ಚಕ್ರ(&init_ಚಕ್ರ);
    ಆದರೆ (ಚಕ್ರ == ಖಾಲಿ) {
        ಆದರೆ (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_ಚಕ್ರ.conf_file.ದತ್ತಾಂಶ);
        }

        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_test_config) {
        ಆದರೆ (!ngx_quiet_ಬಹುಲಕ) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ);
        }

        ಆದರೆ (ngx_dump_config) {
            cd = ಚಕ್ರ->config_dump.elts;

            ಗಾಗಿ (i = 0; i < ಚಕ್ರ->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (ಶೂನ್ಯ) ngx_write_fd(ngx_stdout, cd[i].name.ದತ್ತಾಂಶ,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].ಬಫರ್;

                (ಶೂನ್ಯ) ngx_write_fd(ngx_stdout, b->pos, b->ಕೊನೆಯ - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        ಹಿಂತಿರುಗಿ 0;
    }

    ಆದರೆ (ngx_signal) {
        ಹಿಂತಿರುಗಿ ngx_signal_process(ಚಕ್ರ, ngx_signal);
    }

    ngx_os_ಸ್ಥಿತಿ(ಚಕ್ರ->ಲಘುಗಣಕ);

    ngx_ಚಕ್ರ = ಚಕ್ರ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಆದರೆ (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#ಆದರೆ !(NGX_WIN32)

    ಆದರೆ (ngx_init_ಸಂಕೇತಗಳು(ಚಕ್ರ->ಲಘುಗಣಕ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (!ngx_inherited && ccf->daemon) {
        ಆದರೆ (ngx_daemon(ಚಕ್ರ->ಲಘುಗಣಕ) != NGX_OK) {
            ಹಿಂತಿರುಗಿ 1;
        }

        ngx_daemonized = 1;
    }

    ಆದರೆ (ngx_inherited) {
        ngx_daemonized = 1;
    }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_CONTROL_API)
    ಆದರೆ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "control API unavailable in single process mode");

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ngx_control_init(ngx_control_addr) != NGX_OK) {
            ಹಿಂತಿರುಗಿ 1;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_create_pidfile(&ccf->pid, ಚಕ್ರ->ಲಘುಗಣಕ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ngx_log_redirect_stderr(ಚಕ್ರ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ (ಲಘುಗಣಕ->file->fd != ngx_stderr) {
        ಆದರೆ (ngx_close_file(ಲಘುಗಣಕ->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    ಆದರೆ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_ಚಕ್ರ(ಚಕ್ರ);

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_master_process_ಚಕ್ರ(ಚಕ್ರ);
    }

    ಹಿಂತಿರುಗಿ 0;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_show_ಆವೃತ್ತಿ_info(ಶೂನ್ಯ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    ಆದರೆ (ngx_show_help) {
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
#ಆದರೆ (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NGX_ಪೂರ್ವಪ್ರತ್ಯಯ
            "  -p prefix     : set prefix path (default: " NGX_ಪೂರ್ವಪ್ರತ್ಯಯ ")"
                               NGX_LINEFEED
#ಇಲ್ಲದಿದ್ದರೆ
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#ಆದರೆ_ಅಂತ್ಯ
            "  -e filename   : set error log file (default: "
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#ಇಲ್ಲದಿದ್ದರೆ
                               NGX_ERROR_LOG_ಮಾರ್ಗ ")" NGX_LINEFEED
#ಆದರೆ_ಅಂತ್ಯ
            "  -c filename   : set configuration file (default: " NGX_CONF_ಮಾರ್ಗ
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    ಆದರೆ (ngx_show_configure) {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_SSL)
        ಆದರೆ (ngx_strcmp(ngx_ssl_ಆವೃತ್ತಿ(), OPENSSL_ಆವೃತ್ತಿ_ಪಠ್ಯ) == 0) {
            ngx_write_stderr("built with " OPENSSL_ಆವೃತ್ತಿ_ಪಠ್ಯ NGX_LINEFEED);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_write_stderr("built with " OPENSSL_ಆವೃತ್ತಿ_ಪಠ್ಯ
                             " (running with ");
            ngx_write_stderr((ಅಕ್ಷರ *) (uintptr_t) ngx_ssl_ಆವೃತ್ತಿ());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SSL_CTRL_ಗಣ_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#ಇಲ್ಲದಿದ್ದರೆ
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ಸ್ಥಿರ ngx_int_t
ngx_ಕೂಡಿಸು_inherited_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    ಆದರೆ (inherited == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                  "using inherited sockets from \"%s\"", inherited);

    ಆದರೆ (ngx_array_init(&ಚಕ್ರ->listening, ಚಕ್ರ->pool, 10,
                       ಗಾತ್ರ(ngx_listening_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (p = inherited, v = p; *p; p++) {
        ಆದರೆ (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            ಆದರೆ (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                ನಿಲ್ಲಿಸು;
            }

            v = p + 1;

            ls = ngx_array_ತಳ್ಳು(&ಚಕ್ರ->listening);
            ಆದರೆ (ls == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ngx_memzero(ls, ಗಾತ್ರ(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    ಆದರೆ (v != p) {
        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    ಹಿಂತಿರುಗಿ ngx_ಗಣ_inherited_sockets(ಚಕ್ರ);
}


ಅಕ್ಷರ **
ngx_ಗಣ_environment(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t *ಕೊನೆಯ)
{
    ಅಕ್ಷರ                **p, **env, *str;
    ಗಾತ್ರ_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಆದರೆ (ಕೊನೆಯ == ಖಾಲಿ && ccf->environment) {
        ಹಿಂತಿರುಗಿ ccf->environment;
    }

    var = ccf->env.elts;

    ಗಾಗಿ (i = 0; i < ccf->env.nelts; i++) {
        ಆದರೆ (ngx_strcmp(var[i].ದತ್ತಾಂಶ, "TZ") == 0
            || ngx_strncmp(var[i].ದತ್ತಾಂಶ, "TZ=", 3) == 0)
        {
            ಹೋಗು tz_ಸಿಕ್ಕಿತು;
        }
    }

    var = ngx_array_ತಳ್ಳು(&ccf->env);
    ಆದರೆ (var == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    var->len = 2;
    var->ದತ್ತಾಂಶ = (u_char *) "TZ";

    var = ccf->env.elts;

tz_ಸಿಕ್ಕಿತು:

    n = 0;

    ಗಾಗಿ (i = 0; i < ccf->env.nelts; i++) {

        ಆದರೆ (var[i].ದತ್ತಾಂಶ[var[i].len] == '=') {
            n++;
            ಮುಂದುವರಿ;
        }

        ಗಾಗಿ (p = ngx_os_environ; *p; p++) {

            ಆದರೆ (ngx_strncmp(*p, var[i].ದತ್ತಾಂಶ, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                ನಿಲ್ಲಿಸು;
            }
        }
    }

    ಆದರೆ (ಕೊನೆಯ) {
        env = ngx_alloc((*ಕೊನೆಯ + n + 1) * ಗಾತ್ರ(ಅಕ್ಷರ *), ಚಕ್ರ->ಲಘುಗಣಕ);
        ಆದರೆ (env == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        *ಕೊನೆಯ = n;

    } ಇಲ್ಲದಿದ್ದರೆ {
        cln = ngx_pool_cleanup_ಕೂಡಿಸು(ಚಕ್ರ->pool, 0);
        ಆದರೆ (cln == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        env = ngx_alloc((n + 1) * ಗಾತ್ರ(ಅಕ್ಷರ *), ಚಕ್ರ->ಲಘುಗಣಕ);
        ಆದರೆ (env == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        cln->handler = ngx_cleanup_environment;
        cln->ದತ್ತಾಂಶ = env;
    }

    n = 0;

    ಗಾಗಿ (i = 0; i < ccf->env.nelts; i++) {

        ಆದರೆ (var[i].ದತ್ತಾಂಶ[var[i].len] == '=') {

            ಆದರೆ (ಕೊನೆಯ) {
                env[n++] = (ಅಕ್ಷರ *) var[i].ದತ್ತಾಂಶ;
                ಮುಂದುವರಿ;
            }

            cln = ngx_pool_cleanup_ಕೂಡಿಸು(ಚಕ್ರ->pool, 0);
            ಆದರೆ (cln == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            len = ngx_strlen(var[i].ದತ್ತಾಂಶ) + 1;

            str = ngx_alloc(len, ಚಕ್ರ->ಲಘುಗಣಕ);
            ಆದರೆ (str == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ngx_memcpy(str, var[i].ದತ್ತಾಂಶ, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->ದತ್ತಾಂಶ = str;

            env[n++] = str;

            ಮುಂದುವರಿ;
        }

        ಗಾಗಿ (p = ngx_os_environ; *p; p++) {

            ಆದರೆ (ngx_strncmp(*p, var[i].ದತ್ತಾಂಶ, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                ನಿಲ್ಲಿಸು;
            }
        }
    }

    env[n] = ಖಾಲಿ;

    ಆದರೆ (ಕೊನೆಯ == ಖಾಲಿ) {
        ccf->environment = env;
        environ = env;
    }

    ಹಿಂತಿರುಗಿ env;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_cleanup_environment(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಅಕ್ಷರ  **env = ದತ್ತಾಂಶ;

    ಆದರೆ (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        ಹಿಂತಿರುಗಿ;
    }

    ngx_free(env);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_cleanup_environment_variable(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಅಕ್ಷರ  *var = ದತ್ತಾಂಶ;

    ಅಕ್ಷರ  **p;

    ಗಾಗಿ (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        ಆದರೆ (*p == var) {
            ಹಿಂತಿರುಗಿ;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ದ್ವಿಮಾನ(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
    ಅಕ್ಷರ             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, ಗಾತ್ರ(ngx_exec_ctx_t));

    ctx.ಮಾರ್ಗ = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_ಗಣ_environment(ಚಕ್ರ, &n);
    ಆದರೆ (env == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_INVALID_PID;
    }

    var = ngx_alloc(ಗಾತ್ರ(NGINX_VAR)
                    + ಚಕ್ರ->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    ಚಕ್ರ->ಲಘುಗಣಕ);
    ಆದರೆ (var == ಖಾಲಿ) {
        ngx_free(env);
        ಹಿಂತಿರುಗಿ NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", ಗಾತ್ರ(NGINX_VAR));

    ls = ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {
        ಆದರೆ (ls[i].ignore) {
            ಮುಂದುವರಿ;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#ಆದರೆ (NGX_SETPROCTITLE_ಬಳಸುತ್ತದೆ_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    ಆದರೆ (env[n] != ಖಾಲಿ) {
        n++;
    }
#ಆದರೆ_ಅಂತ್ಯ

    env[n] = ಖಾಲಿ;

#ಆದರೆ (NGX_DEBUG)
    {
    ಅಕ್ಷರ  **e;
    ಗಾಗಿ (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಚಕ್ರ->ಲಘುಗಣಕ, 0, "env: %s", *e);
    }
    }
#ಆದರೆ_ಅಂತ್ಯ

    ctx.envp = (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಆದರೆ (ngx_rename_file(ccf->pid.ದತ್ತಾಂಶ, ccf->oldpid.ದತ್ತಾಂಶ) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.ದತ್ತಾಂಶ, ccf->oldpid.ದತ್ತಾಂಶ, argv[0]);

        ngx_free(env);
        ngx_free(var);

        ಹಿಂತಿರುಗಿ NGX_INVALID_PID;
    }

    pid = ngx_execute(ಚಕ್ರ, &ctx);

    ಆದರೆ (pid == NGX_INVALID_PID) {
        ಆದರೆ (ngx_rename_file(ccf->oldpid.ದತ್ತಾಂಶ, ccf->pid.ದತ್ತಾಂಶ)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.ದತ್ತಾಂಶ, ccf->pid.ದತ್ತಾಂಶ, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    ಹಿಂತಿರುಗಿ pid;
}


ಸ್ಥಿರ ngx_int_t
ngx_get_options(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
    u_char     *p;
    ngx_int_t   i;

    ಗಾಗಿ (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        ಆದರೆ (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆಗ (*p) {

            ಆಯ್ಕೆ (*p++) {

            ಸ್ಥಿತಿ '?':
            ಸ್ಥಿತಿ 'h':
                ngx_show_ಆವೃತ್ತಿ = 1;
                ngx_show_help = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'v':
                ngx_show_ಆವೃತ್ತಿ = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'V':
                ngx_show_ಆವೃತ್ತಿ = 1;
                ngx_show_configure = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 't':
                ngx_test_config = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'q':
                ngx_quiet_ಬಹುಲಕ = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'p':
                ಆದರೆ (*p) {
                    ngx_ಪೂರ್ವಪ್ರತ್ಯಯ = p;
                    ಹೋಗು ಮುಂದಿನ;
                }

                ಆದರೆ (argv[++i]) {
                    ngx_ಪೂರ್ವಪ್ರತ್ಯಯ = (u_char *) argv[i];
                    ಹೋಗು ಮುಂದಿನ;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                ಹಿಂತಿರುಗಿ NGX_ERROR;

            ಸ್ಥಿತಿ 'e':
                ಆದರೆ (*p) {
                    ngx_error_log = p;

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } ಇಲ್ಲದಿದ್ದರೆ {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                ಹೋಗು ಮುಂದಿನ;

            ಸ್ಥಿತಿ 'c':
                ಆದರೆ (*p) {
                    ngx_conf_file = p;
                    ಹೋಗು ಮುಂದಿನ;
                }

                ಆದರೆ (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    ಹೋಗು ಮುಂದಿನ;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                ಹಿಂತಿರುಗಿ NGX_ERROR;

            ಸ್ಥಿತಿ 'g':
                ಆದರೆ (*p) {
                    ngx_conf_params = p;
                    ಹೋಗು ಮುಂದಿನ;
                }

                ಆದರೆ (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    ಹೋಗು ಮುಂದಿನ;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                ಹಿಂತಿರುಗಿ NGX_ERROR;

            ಸ್ಥಿತಿ 's':
                ಆದರೆ (*p) {
                    ngx_signal = (ಅಕ್ಷರ *) p;

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argv[++i]) {
                    ngx_signal = argv[i];

                } ಇಲ್ಲದಿದ್ದರೆ {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    ಹೋಗು ಮುಂದಿನ;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                ಹಿಂತಿರುಗಿ NGX_ERROR;

#ಆದರೆ (NGX_CONTROL_API)
            ಸ್ಥಿತಿ 'l':
                ngx_control_api_enabled = 1;

                ಆದರೆ (*p) {
                    ngx_control_addr = p;
                    ಹೋಗು ಮುಂದಿನ;
                }

                ಆದರೆ (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    ಹೋಗು ಮುಂದಿನ;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                ಹಿಂತಿರುಗಿ NGX_ERROR;
#ಆದರೆ_ಅಂತ್ಯ

            ಪೂರ್ವನಿಯೋಜಿತ:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

    ಮುಂದಿನ:

        ಮುಂದುವರಿ;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_save_argv(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
#ಆದರೆ (NGX_FREEBSD)

    ngx_os_argv = (ಅಕ್ಷರ **) argv;
    ngx_argc = argc;
    ngx_argv = (ಅಕ್ಷರ **) argv;

#ಇಲ್ಲದಿದ್ದರೆ
    ಗಾತ್ರ_t     len;
    ngx_int_t  i;

    ngx_os_argv = (ಅಕ್ಷರ **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * ಗಾತ್ರ(ಅಕ್ಷರ *), ಚಕ್ರ->ಲಘುಗಣಕ);
    ಆದರೆ (ngx_argv == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, ಚಕ್ರ->ಲಘುಗಣಕ);
        ಆದರೆ (ngx_argv[i] == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ಖಾಲಿ;

#ಆದರೆ_ಅಂತ್ಯ

    ngx_os_environ = environ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_process_options(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    u_char  *p;
    ಗಾತ್ರ_t   len;

    ಆದರೆ (ngx_ಪೂರ್ವಪ್ರತ್ಯಯ) {
        len = ngx_strlen(ngx_ಪೂರ್ವಪ್ರತ್ಯಯ);
        p = ngx_ಪೂರ್ವಪ್ರತ್ಯಯ;

        ಆದರೆ (len && !ngx_ಮಾರ್ಗ_separator(p[len - 1])) {
            p = ngx_pnalloc(ಚಕ್ರ->pool, len + 1);
            ಆದರೆ (p == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ngx_memcpy(p, ngx_ಪೂರ್ವಪ್ರತ್ಯಯ, len);
            p[len++] = '/';
        }

        ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.len = len;
        ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = p;
        ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.len = len;
        ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = p;

    } ಇಲ್ಲದಿದ್ದರೆ {

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ಪೂರ್ವಪ್ರತ್ಯಯ

        p = ngx_pnalloc(ಚಕ್ರ->pool, NGX_MAX_ಮಾರ್ಗ);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ngx_getcwd(p, NGX_MAX_ಮಾರ್ಗ) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.len = len;
        ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = p;
        ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.len = len;
        ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = p;

#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NGX_CONF_ಪೂರ್ವಪ್ರತ್ಯಯ
        ngx_str_ಗಣ(&ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ, NGX_CONF_ಪೂರ್ವಪ್ರತ್ಯಯ);
#ಇಲ್ಲದಿದ್ದರೆ
        ngx_str_ಗಣ(&ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ, NGX_ಪೂರ್ವಪ್ರತ್ಯಯ);
#ಆದರೆ_ಅಂತ್ಯ
        ngx_str_ಗಣ(&ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ, NGX_ಪೂರ್ವಪ್ರತ್ಯಯ);

#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ (ngx_conf_file) {
        ಚಕ್ರ->conf_file.len = ngx_strlen(ngx_conf_file);
        ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ = ngx_conf_file;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_str_ಗಣ(&ಚಕ್ರ->conf_file, NGX_CONF_ಮಾರ್ಗ);
    }

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(ಚಕ್ರ, &ಚಕ್ರ->conf_file, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (p = ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ + ಚಕ್ರ->conf_file.len - 1;
         p > ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ;
         p--)
    {
        ಆದರೆ (ngx_ಮಾರ್ಗ_separator(*p)) {
            ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.len = p - ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ + 1;
            ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (ngx_error_log) {
        ಚಕ್ರ->error_log.len = ngx_strlen(ngx_error_log);
        ಚಕ್ರ->error_log.ದತ್ತಾಂಶ = ngx_error_log;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_str_ಗಣ(&ಚಕ್ರ->error_log, NGX_ERROR_LOG_ಮಾರ್ಗ);
    }

    ಆದರೆ (ngx_conf_params) {
        ಚಕ್ರ->conf_param.len = ngx_strlen(ngx_conf_params);
        ಚಕ್ರ->conf_param.ದತ್ತಾಂಶ = ngx_conf_params;
    }

    ಆದರೆ (ngx_test_config) {
        ಚಕ್ರ->ಲಘುಗಣಕ->log_level = NGX_LOG_INFO;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_core_module_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(ಚಕ್ರ->pool, ಗಾತ್ರ(ngx_core_conf_t));
    ಆದರೆ (ccf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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

    ccf->ಕೆಲಸಗಾರ_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->ಗುಂಪು = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    ಆದರೆ (ngx_array_init(&ccf->env, ಚಕ್ರ->pool, 1, ಗಾತ್ರ(ngx_str_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ccf;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_core_module_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_ಮೌಲ್ಯ(ccf->daemon, 1);
    ngx_conf_init_ಮೌಲ್ಯ(ccf->master, 1);
    ngx_conf_init_msec_ಮೌಲ್ಯ(ccf->timer_resolution, 0);
    ngx_conf_init_msec_ಮೌಲ್ಯ(ccf->shutdown_timeout, 0);

    ngx_conf_init_ಮೌಲ್ಯ(ccf->ಕೆಲಸಗಾರ_processes, 1);
    ngx_conf_init_ಮೌಲ್ಯ(ccf->debug_points, 0);

#ಆದರೆ (NGX_HAVE_CPU_AFFINITY)

    ಆದರೆ (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->ಕೆಲಸಗಾರ_processes)
    {
        ngx_log_error(NGX_LOG_WARN, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#ಆದರೆ_ಅಂತ್ಯ


    ಆದರೆ (ccf->pid.len == 0) {
        ngx_str_ಗಣ(&ccf->pid, NGX_PID_ಮಾರ್ಗ);
    }

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(ಚಕ್ರ, &ccf->pid, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + ಗಾತ್ರ(NGX_OLDPID_EXT);

    ccf->oldpid.ದತ್ತಾಂಶ = ngx_pnalloc(ಚಕ್ರ->pool, ccf->oldpid.len);
    ಆದರೆ (ccf->oldpid.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.ದತ್ತಾಂಶ, ccf->pid.ದತ್ತಾಂಶ, ccf->pid.len),
               NGX_OLDPID_EXT, ಗಾತ್ರ(NGX_OLDPID_EXT));


    ಆದರೆ (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(ಚಕ್ರ, &ccf->working_directory, 0) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ಆದರೆ (ngx_file_info(ccf->working_directory.ದತ್ತಾಂಶ, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ಆದರೆ (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }


#ಆದರೆ !(NGX_WIN32)

    ಆದರೆ (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ರಚನೆ ಗುಂಪು   *grp;
        ರಚನೆ passwd  *pwd;

        ngx_ಗಣ_errno(0);
        pwd = getpwnam(NGX_USER);
        ಆದರೆ (pwd == ಖಾಲಿ) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_ಗಣ_errno(0);
        grp = getgrnam(NGX_ಗುಂಪು);
        ಆದರೆ (grp == ಖಾಲಿ) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          "getgrnam(\"" NGX_ಗುಂಪು "\") failed");
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ccf->ಗುಂಪು = grp->gr_gid;
    }


    ಆದರೆ (ccf->ಬೀಗ_file.len == 0) {
        ngx_str_ಗಣ(&ccf->ಬೀಗ_file, NGX_ಬೀಗ_ಮಾರ್ಗ);
    }

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(ಚಕ್ರ, &ccf->ಬೀಗ_file, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    {
    ngx_str_t  ಬೀಗ_file;

    ಬೀಗ_file = ಚಕ್ರ->old_ಚಕ್ರ->ಬೀಗ_file;

    ಆದರೆ (ಬೀಗ_file.len) {
        ಬೀಗ_file.len--;

        ಆದರೆ (ccf->ಬೀಗ_file.len != ಬೀಗ_file.len
            || ngx_strncmp(ccf->ಬೀಗ_file.ದತ್ತಾಂಶ, ಬೀಗ_file.ದತ್ತಾಂಶ, ಬೀಗ_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        ಚಕ್ರ->ಬೀಗ_file.len = ಬೀಗ_file.len + 1;
        ಬೀಗ_file.len += ಗಾತ್ರ(".accept");

        ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ = ngx_pstrdup(ಚಕ್ರ->pool, &ಬೀಗ_file);
        ಆದರೆ (ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಚಕ್ರ->ಬೀಗ_file.len = ccf->ಬೀಗ_file.len + 1;
        ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ = ngx_pnalloc(ಚಕ್ರ->pool,
                                      ccf->ಬೀಗ_file.len + ಗಾತ್ರ(".accept"));
        ಆದರೆ (ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ, ccf->ಬೀಗ_file.ದತ್ತಾಂಶ,
                              ccf->ಬೀಗ_file.len),
                   ".accept", ಗಾತ್ರ(".accept"));
    }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ಗಣ_user(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
#ಆದರೆ (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;

#ಇಲ್ಲದಿದ್ದರೆ

    ngx_core_conf_t  *ccf = conf;

    ಅಕ್ಷರ             *ಗುಂಪು;
    ರಚನೆ passwd    *pwd;
    ರಚನೆ ಗುಂಪು     *grp;
    ngx_str_t        *ಮೌಲ್ಯ;

    ಆದರೆ (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಆದರೆ (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ccf->username = (ಅಕ್ಷರ *) ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ;

    ngx_ಗಣ_errno(0);
    pwd = getpwnam((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ);
    ಆದರೆ (pwd == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    ಗುಂಪು = (ಅಕ್ಷರ *) ((cf->ವಾದಗಳು->nelts == 2) ? ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ : ಮೌಲ್ಯ[2].ದತ್ತಾಂಶ);

    ngx_ಗಣ_errno(0);
    grp = getgrnam(ಗುಂಪು);
    ಆದರೆ (grp == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", ಗುಂಪು);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ccf->ಗುಂಪು = grp->gr_gid;

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;

#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ಗಣ_env(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *ಮೌಲ್ಯ, *var;
    ngx_uint_t   i;

    var = ngx_array_ತಳ್ಳು(&ccf->env);
    ಆದರೆ (var == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;
    *var = ಮೌಲ್ಯ[1];

    ಗಾಗಿ (i = 0; i < ಮೌಲ್ಯ[1].len; i++) {

        ಆದರೆ (ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ[i] == '=') {

            var->len = i;

            ಹಿಂತಿರುಗಿ NGX_CONF_OK;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ಗಣ_ಆದ್ಯತೆ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_uint_t        n, minus;

    ಆದರೆ (ccf->ಆದ್ಯತೆ != 0) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಆದರೆ (ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ[0] == '-') {
        n = 1;
        minus = 1;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ[0] == '+') {
        n = 1;
        minus = 0;

    } ಇಲ್ಲದಿದ್ದರೆ {
        n = 0;
        minus = 0;
    }

    ccf->ಆದ್ಯತೆ = ngx_atoi(&ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ[n], ಮೌಲ್ಯ[1].len - n);
    ಆದರೆ (ccf->ಆದ್ಯತೆ == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid number";
    }

    ಆದರೆ (minus) {
        ccf->ಆದ್ಯತೆ = -ccf->ಆದ್ಯತೆ;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ಗಣ_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
#ಆದರೆ (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *ಮುಖವಾಡ;

    ಆದರೆ (ccf->cpu_affinity) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮುಖವಾಡ = ngx_palloc(cf->pool, (cf->ವಾದಗಳು->nelts - 1) * ಗಾತ್ರ(ngx_cpuset_t));
    ಆದರೆ (ಮುಖವಾಡ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->ವಾದಗಳು->nelts - 1;
    ccf->cpu_affinity = ಮುಖವಾಡ;

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಆದರೆ (ngx_strcmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, "auto") == 0) {

        ಆದರೆ (cf->ವಾದಗಳು->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&ಮುಖವಾಡ[0]);
        ಗಾಗಿ (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_ಗಣ(i, &ಮುಖವಾಡ[0]);
        }

        n = 2;

    } ಇಲ್ಲದಿದ್ದರೆ {
        n = 1;
    }

    ಗಾಗಿ ( /* void */ ; n < cf->ವಾದಗಳು->nelts; n++) {

        ಆದರೆ (ಮೌಲ್ಯ[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&ಮುಖವಾಡ[n - 1]);

        ಗಾಗಿ (p = ಮೌಲ್ಯ[n].ದತ್ತಾಂಶ + ಮೌಲ್ಯ[n].len - 1;
             p >= ಮೌಲ್ಯ[n].ದತ್ತಾಂಶ;
             p--)
        {
            ch = *p;

            ಆದರೆ (ch == ' ') {
                ಮುಂದುವರಿ;
            }

            i++;

            ಆದರೆ (ch == '0') {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ch == '1') {
                CPU_ಗಣ(i - 1, &ಮುಖವಾಡ[n - 1]);
                ಮುಂದುವರಿ;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

#ಇಲ್ಲದಿದ್ದರೆ

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#ಆದರೆ (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *ಮುಖವಾಡ;
    ngx_core_conf_t  *ccf;

    ಸ್ಥಿರ ngx_cpuset_t  ಫಲಿತಾಂಶ;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_ಚಕ್ರ->conf_ctx,
                                           ngx_core_module);

    ಆದರೆ (ccf->cpu_affinity == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ccf->cpu_affinity_auto) {
        ಮುಖವಾಡ = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        ಗಾಗಿ (i = 0, j = n; /* void */ ; i++) {

            ಆದರೆ (CPU_ISSET(i % CPU_SETSIZE, ಮುಖವಾಡ) && j-- == 0) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            /* void */
        }

        CPU_ZERO(&ಫಲಿತಾಂಶ);
        CPU_ಗಣ(i % CPU_SETSIZE, &ಫಲಿತಾಂಶ);

        ಹಿಂತಿರುಗಿ &ಫಲಿತಾಂಶ;
    }

    ಆದರೆ (ccf->cpu_affinity_n > n) {
        ಹಿಂತಿರುಗಿ &ccf->cpu_affinity[n];
    }

    ಹಿಂತಿರುಗಿ &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#ಇಲ್ಲದಿದ್ದರೆ

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ಗಣ_ಕೆಲಸಗಾರ_processes(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    ಆದರೆ (ccf->ಕೆಲಸಗಾರ_processes != NGX_CONF_UNSET) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಆದರೆ (ngx_strcmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, "auto") == 0) {
        ccf->ಕೆಲಸಗಾರ_processes = ngx_ncpu;
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ccf->ಕೆಲಸಗಾರ_processes = ngx_atoi(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, ಮೌಲ್ಯ[1].len);

    ಆದರೆ (ccf->ಕೆಲಸಗಾರ_processes == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_ತುಂಬು_module(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
#ಆದರೆ (NGX_HAVE_DLOPEN)
    ಶೂನ್ಯ                *handle;
    ಅಕ್ಷರ               **ಹೆಸರುಗಳು, **ಕ್ರಮ;
    ngx_str_t           *ಮೌಲ್ಯ, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    ಆದರೆ (cf->ಚಕ್ರ->modules_ಬಳಸಿದ) {
        ಹಿಂತಿರುಗಿ "is specified too late";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    file = ಮೌಲ್ಯ[1];

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(cf->ಚಕ್ರ, &file, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(cf->ಚಕ್ರ->pool, 0);
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.ದತ್ತಾಂಶ);
    ಆದರೆ (handle == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.ದತ್ತಾಂಶ, ngx_dlerror());
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->ದತ್ತಾಂಶ = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    ಆದರೆ (modules == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ಮೌಲ್ಯ[1], "ngx_modules", ngx_dlerror());
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹೆಸರುಗಳು = ngx_dlsym(handle, "ngx_module_names");
    ಆದರೆ (ಹೆಸರುಗಳು == ಖಾಲಿ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &ಮೌಲ್ಯ[1], "ngx_module_names", ngx_dlerror());
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಕ್ರಮ = ngx_dlsym(handle, "ngx_module_order");

    ಗಾಗಿ (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = ಹೆಸರುಗಳು[i];

        ಆದರೆ (ngx_ಕೂಡಿಸು_module(cf, &file, module, ಕ್ರಮ) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0, "module: %s i:%ui",
                       module->name, module->ಸೂಚ್ಯಂಕ);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;

#ಇಲ್ಲದಿದ್ದರೆ

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;

#ಆದರೆ_ಅಂತ್ಯ
}


#ಆದರೆ (NGX_HAVE_DLOPEN)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_unload_module(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಶೂನ್ಯ  *handle = ದತ್ತಾಂಶ;

    ಆದರೆ (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#ಆದರೆ_ಅಂತ್ಯ
