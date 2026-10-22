
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <nginx.h>


स्थिर शून्य ngx_show_संस्करणम्_info(शून्य);
स्थिर ngx_int_t ngx_सङ्कलय_inherited_sockets(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर शून्य ngx_cleanup_environment(शून्य *दत्त);
स्थिर शून्य ngx_cleanup_environment_variable(शून्य *दत्त);
स्थिर ngx_int_t ngx_get_options(पूर्णाङ्क argc, वर्ण *ध्रुव *argv);
स्थिर ngx_int_t ngx_process_options(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर ngx_int_t ngx_save_argv(ngx_आवृत्तिः_t *आवृत्तिः, पूर्णाङ्क argc, वर्ण *ध्रुव *argv);
स्थिर शून्य *ngx_core_module_create_conf(ngx_आवृत्तिः_t *आवृत्तिः);
स्थिर वर्ण *ngx_core_module_init_conf(ngx_आवृत्तिः_t *आवृत्तिः, शून्य *conf);
स्थिर वर्ण *ngx_समुच्चयः_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_समुच्चयः_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_समुच्चयः_प्राथमिकता(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_समुच्चयः_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर वर्ण *ngx_समुच्चयः_कार्यकर्ता_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर वर्ण *ngx_आरोपय_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
#यदि (NGX_HAVE_DLOPEN)
स्थिर शून्य ngx_unload_module(शून्य *दत्त);
#यदि_अन्तः


स्थिर ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


स्थिर ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_FLAG,
      ngx_conf_समुच्चयः_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      रिक्त },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_FLAG,
      ngx_conf_समुच्चयः_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      रिक्त },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      रिक्त },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      रिक्त },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_str_slot,
      0,
      offsetof(ngx_core_conf_t, तालकम्_file),
      रिक्त },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_समुच्चयः_कार्यकर्ता_processes,
      0,
      0,
      रिक्त },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_enum_slot,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE12,
      ngx_समुच्चयः_user,
      0,
      0,
      रिक्त },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_समुच्चयः_प्राथमिकता,
      0,
      0,
      रिक्त },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_1_अधिकम्,
      ngx_समुच्चयः_cpu_affinity,
      0,
      0,
      रिक्त },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_अङ्कः_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      रिक्त },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      रिक्त },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      रिक्त },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_conf_समुच्चयः_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      रिक्त },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_समुच्चयः_env,
      0,
      0,
      रिक्त },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_प्रत्यक्षम्_CONF|NGX_CONF_TAKE1,
      ngx_आरोपय_module,
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
स्थिर ngx_uint_t   ngx_show_संस्करणम्;
स्थिर ngx_uint_t   ngx_show_configure;
स्थिर u_char      *ngx_उपसर्गः;
स्थिर u_char      *ngx_error_log;
स्थिर u_char      *ngx_conf_file;
स्थिर u_char      *ngx_conf_params;
#यदि (NGX_CONTROL_API)
स्थिर u_char      *ngx_control_addr;
#यदि_अन्तः
स्थिर वर्ण        *ngx_signal;


स्थिर वर्ण **ngx_os_environ;


पूर्णाङ्क ngx_cdecl
मुख्यम्(पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *लघुगणकः;
    ngx_uint_t        i;
    ngx_आवृत्तिः_t      *आवृत्तिः, init_आवृत्तिः;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    यदि (ngx_strerror_init() != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_get_options(argc, argv) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_show_संस्करणम्) {
        ngx_show_संस्करणम्_info();

        यदि (!ngx_test_config) {
            प्रत्यागम 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#यदि (NGX_PCRE)
    ngx_regex_init();
#यदि_अन्तः

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    लघुगणकः = ngx_log_init(ngx_उपसर्गः, ngx_error_log);
    यदि (लघुगणकः == रिक्त) {
        प्रत्यागम 1;
    }

    /* STUB */
#यदि (NGX_OPENSSL)
    ngx_ssl_init(लघुगणकः);
#यदि_अन्तः

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_आवृत्तिः, परिमाण(ngx_आवृत्तिः_t));
    init_आवृत्तिः.लघुगणकः = लघुगणकः;
    ngx_आवृत्तिः = &init_आवृत्तिः;

    init_आवृत्तिः.pool = ngx_create_pool(1024, लघुगणकः);
    यदि (init_आवृत्तिः.pool == रिक्त) {
        प्रत्यागम 1;
    }

    यदि (ngx_save_argv(&init_आवृत्तिः, argc, argv) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_process_options(&init_आवृत्तिः) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_os_init(लघुगणकः) != NGX_OK) {
        प्रत्यागम 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    यदि (ngx_चक्रीयपरीक्षा_32_सारणी_init() != NGX_OK) {
        प्रत्यागम 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    यदि (ngx_सङ्कलय_inherited_sockets(&init_आवृत्तिः) != NGX_OK) {
        प्रत्यागम 1;
    }

#यदि (NGX_CONTROL_API)
    ngx_control_preinit();
#यदि_अन्तः

    यदि (ngx_preinit_modules() != NGX_OK) {
        प्रत्यागम 1;
    }

    आवृत्तिः = ngx_init_आवृत्तिः(&init_आवृत्तिः);
    यदि (आवृत्तिः == रिक्त) {
        यदि (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_आवृत्तिः.conf_file.दत्त);
        }

        प्रत्यागम 1;
    }

    यदि (ngx_test_config) {
        यदि (!ngx_quiet_बहुलकम्) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           आवृत्तिः->conf_file.दत्त);
        }

        यदि (ngx_dump_config) {
            cd = आवृत्तिः->config_dump.elts;

            प्रति (i = 0; i < आवृत्तिः->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (शून्य) ngx_write_fd(ngx_stdout, cd[i].name.दत्त,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].बफर;

                (शून्य) ngx_write_fd(ngx_stdout, b->pos, b->चरमः - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        प्रत्यागम 0;
    }

    यदि (ngx_signal) {
        प्रत्यागम ngx_signal_process(आवृत्तिः, ngx_signal);
    }

    ngx_os_स्थितिः(आवृत्तिः->लघुगणकः);

    ngx_आवृत्तिः = आवृत्तिः;

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    यदि (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#यदि !(NGX_WIN32)

    यदि (ngx_init_सङ्केताः(आवृत्तिः->लघुगणकः) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (!ngx_inherited && ccf->daemon) {
        यदि (ngx_daemon(आवृत्तिः->लघुगणकः) != NGX_OK) {
            प्रत्यागम 1;
        }

        ngx_daemonized = 1;
    }

    यदि (ngx_inherited) {
        ngx_daemonized = 1;
    }

#यदि_अन्तः

#यदि (NGX_CONTROL_API)
    यदि (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, आवृत्तिः->लघुगणकः, 0,
                      "control API unavailable in single process mode");

    } अन्यथा {
        यदि (ngx_control_init(ngx_control_addr) != NGX_OK) {
            प्रत्यागम 1;
        }
    }
#यदि_अन्तः

    यदि (ngx_create_pidfile(&ccf->pid, आवृत्तिः->लघुगणकः) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_log_redirect_stderr(आवृत्तिः) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (लघुगणकः->file->fd != ngx_stderr) {
        यदि (ngx_close_file(लघुगणकः->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    यदि (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_आवृत्तिः(आवृत्तिः);

    } अन्यथा {
        ngx_master_process_आवृत्तिः(आवृत्तिः);
    }

    प्रत्यागम 0;
}


स्थिर शून्य
ngx_show_संस्करणम्_info(शून्य)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    यदि (ngx_show_help) {
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
#यदि (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#यदि_अन्तः
#यदि_परिभाषितम् NGX_उपसर्गः
            "  -p prefix     : set prefix path (default: " NGX_उपसर्गः ")"
                               NGX_LINEFEED
#अन्यथा
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#यदि_अन्तः
            "  -e filename   : set error log file (default: "
#यदि_परिभाषितम् NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#अन्यथा
                               NGX_ERROR_LOG_मार्गः ")" NGX_LINEFEED
#यदि_अन्तः
            "  -c filename   : set configuration file (default: " NGX_CONF_मार्गः
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    यदि (ngx_show_configure) {

#यदि_परिभाषितम् NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#यदि_अन्तः

#यदि (NGX_SSL)
        यदि (ngx_strcmp(ngx_ssl_संस्करणम्(), OPENSSL_संस्करणम्_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_संस्करणम्_TEXT NGX_LINEFEED);
        } अन्यथा {
            ngx_write_stderr("built with " OPENSSL_संस्करणम्_TEXT
                             " (running with ");
            ngx_write_stderr((वर्ण *) (uintptr_t) ngx_ssl_संस्करणम्());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#यदि_परिभाषितम् SSL_CTRL_समुच्चयः_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#अन्यथा
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#यदि_अन्तः
#यदि_अन्तः

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


स्थिर ngx_int_t
ngx_सङ्कलय_inherited_sockets(ngx_आवृत्तिः_t *आवृत्तिः)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    यदि (inherited == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, आवृत्तिः->लघुगणकः, 0,
                  "using inherited sockets from \"%s\"", inherited);

    यदि (ngx_array_init(&आवृत्तिः->listening, आवृत्तिः->pool, 10,
                       परिमाण(ngx_listening_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (p = inherited, v = p; *p; p++) {
        यदि (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            यदि (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                भङ्ग;
            }

            v = p + 1;

            ls = ngx_array_प्रक्षिप(&आवृत्तिः->listening);
            यदि (ls == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memzero(ls, परिमाण(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    यदि (v != p) {
        ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    प्रत्यागम ngx_समुच्चयः_inherited_sockets(आवृत्तिः);
}


वर्ण **
ngx_समुच्चयः_environment(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t *चरमः)
{
    वर्ण                **p, **env, *str;
    आकार_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    यदि (चरमः == रिक्त && ccf->environment) {
        प्रत्यागम ccf->environment;
    }

    var = ccf->env.elts;

    प्रति (i = 0; i < ccf->env.nelts; i++) {
        यदि (ngx_strcmp(var[i].दत्त, "TZ") == 0
            || ngx_strncmp(var[i].दत्त, "TZ=", 3) == 0)
        {
            गच्छ tz_प्राप्तम्;
        }
    }

    var = ngx_array_प्रक्षिप(&ccf->env);
    यदि (var == रिक्त) {
        प्रत्यागम रिक्त;
    }

    var->len = 2;
    var->दत्त = (u_char *) "TZ";

    var = ccf->env.elts;

tz_प्राप्तम्:

    n = 0;

    प्रति (i = 0; i < ccf->env.nelts; i++) {

        यदि (var[i].दत्त[var[i].len] == '=') {
            n++;
            अनुवर्त;
        }

        प्रति (p = ngx_os_environ; *p; p++) {

            यदि (ngx_strncmp(*p, var[i].दत्त, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                भङ्ग;
            }
        }
    }

    यदि (चरमः) {
        env = ngx_alloc((*चरमः + n + 1) * परिमाण(वर्ण *), आवृत्तिः->लघुगणकः);
        यदि (env == रिक्त) {
            प्रत्यागम रिक्त;
        }

        *चरमः = n;

    } अन्यथा {
        cln = ngx_pool_cleanup_सङ्कलय(आवृत्तिः->pool, 0);
        यदि (cln == रिक्त) {
            प्रत्यागम रिक्त;
        }

        env = ngx_alloc((n + 1) * परिमाण(वर्ण *), आवृत्तिः->लघुगणकः);
        यदि (env == रिक्त) {
            प्रत्यागम रिक्त;
        }

        cln->handler = ngx_cleanup_environment;
        cln->दत्त = env;
    }

    n = 0;

    प्रति (i = 0; i < ccf->env.nelts; i++) {

        यदि (var[i].दत्त[var[i].len] == '=') {

            यदि (चरमः) {
                env[n++] = (वर्ण *) var[i].दत्त;
                अनुवर्त;
            }

            cln = ngx_pool_cleanup_सङ्कलय(आवृत्तिः->pool, 0);
            यदि (cln == रिक्त) {
                प्रत्यागम रिक्त;
            }

            len = ngx_strlen(var[i].दत्त) + 1;

            str = ngx_alloc(len, आवृत्तिः->लघुगणकः);
            यदि (str == रिक्त) {
                प्रत्यागम रिक्त;
            }

            ngx_memcpy(str, var[i].दत्त, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->दत्त = str;

            env[n++] = str;

            अनुवर्त;
        }

        प्रति (p = ngx_os_environ; *p; p++) {

            यदि (ngx_strncmp(*p, var[i].दत्त, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                भङ्ग;
            }
        }
    }

    env[n] = रिक्त;

    यदि (चरमः == रिक्त) {
        ccf->environment = env;
        environ = env;
    }

    प्रत्यागम env;
}


स्थिर शून्य
ngx_cleanup_environment(शून्य *दत्त)
{
    वर्ण  **env = दत्त;

    यदि (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        प्रत्यागम;
    }

    ngx_free(env);
}


स्थिर शून्य
ngx_cleanup_environment_variable(शून्य *दत्त)
{
    वर्ण  *var = दत्त;

    वर्ण  **p;

    प्रति (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        यदि (*p == var) {
            प्रत्यागम;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_द्विमान(ngx_आवृत्तिः_t *आवृत्तिः, वर्ण *ध्रुव *argv)
{
    वर्ण             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, परिमाण(ngx_exec_ctx_t));

    ctx.मार्गः = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_समुच्चयः_environment(आवृत्तिः, &n);
    यदि (env == रिक्त) {
        प्रत्यागम NGX_INVALID_PID;
    }

    var = ngx_alloc(परिमाण(NGINX_VAR)
                    + आवृत्तिः->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    आवृत्तिः->लघुगणकः);
    यदि (var == रिक्त) {
        ngx_free(env);
        प्रत्यागम NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", परिमाण(NGINX_VAR));

    ls = आवृत्तिः->listening.elts;
    प्रति (i = 0; i < आवृत्तिः->listening.nelts; i++) {
        यदि (ls[i].ignore) {
            अनुवर्त;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#यदि (NGX_SETPROCTITLE_उपयुङ्क्ते_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#यदि_अन्तः

#यदि (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    यदि (env[n] != रिक्त) {
        n++;
    }
#यदि_अन्तः

    env[n] = रिक्त;

#यदि (NGX_DEBUG)
    {
    वर्ण  **e;
    प्रति (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, आवृत्तिः->लघुगणकः, 0, "env: %s", *e);
    }
    }
#यदि_अन्तः

    ctx.envp = (वर्ण *ध्रुव *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    यदि (ngx_rename_file(ccf->pid.दत्त, ccf->oldpid.दत्त) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.दत्त, ccf->oldpid.दत्त, argv[0]);

        ngx_free(env);
        ngx_free(var);

        प्रत्यागम NGX_INVALID_PID;
    }

    pid = ngx_execute(आवृत्तिः, &ctx);

    यदि (pid == NGX_INVALID_PID) {
        यदि (ngx_rename_file(ccf->oldpid.दत्त, ccf->pid.दत्त)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.दत्त, ccf->pid.दत्त, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    प्रत्यागम pid;
}


स्थिर ngx_int_t
ngx_get_options(पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
    u_char     *p;
    ngx_int_t   i;

    प्रति (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        यदि (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            प्रत्यागम NGX_ERROR;
        }

        यावत् (*p) {

            विकल्प (*p++) {

            पक्ष '?':
            पक्ष 'h':
                ngx_show_संस्करणम् = 1;
                ngx_show_help = 1;
                भङ्ग;

            पक्ष 'v':
                ngx_show_संस्करणम् = 1;
                भङ्ग;

            पक्ष 'V':
                ngx_show_संस्करणम् = 1;
                ngx_show_configure = 1;
                भङ्ग;

            पक्ष 't':
                ngx_test_config = 1;
                भङ्ग;

            पक्ष 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                भङ्ग;

            पक्ष 'q':
                ngx_quiet_बहुलकम् = 1;
                भङ्ग;

            पक्ष 'p':
                यदि (*p) {
                    ngx_उपसर्गः = p;
                    गच्छ अग्र;
                }

                यदि (argv[++i]) {
                    ngx_उपसर्गः = (u_char *) argv[i];
                    गच्छ अग्र;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                प्रत्यागम NGX_ERROR;

            पक्ष 'e':
                यदि (*p) {
                    ngx_error_log = p;

                } अन्यथा यदि (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                गच्छ अग्र;

            पक्ष 'c':
                यदि (*p) {
                    ngx_conf_file = p;
                    गच्छ अग्र;
                }

                यदि (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    गच्छ अग्र;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                प्रत्यागम NGX_ERROR;

            पक्ष 'g':
                यदि (*p) {
                    ngx_conf_params = p;
                    गच्छ अग्र;
                }

                यदि (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    गच्छ अग्र;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                प्रत्यागम NGX_ERROR;

            पक्ष 's':
                यदि (*p) {
                    ngx_signal = (वर्ण *) p;

                } अन्यथा यदि (argv[++i]) {
                    ngx_signal = argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    गच्छ अग्र;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                प्रत्यागम NGX_ERROR;

#यदि (NGX_CONTROL_API)
            पक्ष 'l':
                ngx_control_api_enabled = 1;

                यदि (*p) {
                    ngx_control_addr = p;
                    गच्छ अग्र;
                }

                यदि (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    गच्छ अग्र;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                प्रत्यागम NGX_ERROR;
#यदि_अन्तः

            स्वभाव:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                प्रत्यागम NGX_ERROR;
            }
        }

    अग्र:

        अनुवर्त;
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_save_argv(ngx_आवृत्तिः_t *आवृत्तिः, पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
#यदि (NGX_FREEBSD)

    ngx_os_argv = (वर्ण **) argv;
    ngx_argc = argc;
    ngx_argv = (वर्ण **) argv;

#अन्यथा
    आकार_t     len;
    ngx_int_t  i;

    ngx_os_argv = (वर्ण **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * परिमाण(वर्ण *), आवृत्तिः->लघुगणकः);
    यदि (ngx_argv == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, आवृत्तिः->लघुगणकः);
        यदि (ngx_argv[i] == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = रिक्त;

#यदि_अन्तः

    ngx_os_environ = environ;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_process_options(ngx_आवृत्तिः_t *आवृत्तिः)
{
    u_char  *p;
    आकार_t   len;

    यदि (ngx_उपसर्गः) {
        len = ngx_strlen(ngx_उपसर्गः);
        p = ngx_उपसर्गः;

        यदि (len && !ngx_मार्गः_separator(p[len - 1])) {
            p = ngx_pnalloc(आवृत्तिः->pool, len + 1);
            यदि (p == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memcpy(p, ngx_उपसर्गः, len);
            p[len++] = '/';
        }

        आवृत्तिः->conf_उपसर्गः.len = len;
        आवृत्तिः->conf_उपसर्गः.दत्त = p;
        आवृत्तिः->उपसर्गः.len = len;
        आवृत्तिः->उपसर्गः.दत्त = p;

    } अन्यथा {

#यदि_अपरिभाषितम् NGX_उपसर्गः

        p = ngx_pnalloc(आवृत्तिः->pool, NGX_MAX_मार्गः);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (ngx_getcwd(p, NGX_MAX_मार्गः) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            प्रत्यागम NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        आवृत्तिः->conf_उपसर्गः.len = len;
        आवृत्तिः->conf_उपसर्गः.दत्त = p;
        आवृत्तिः->उपसर्गः.len = len;
        आवृत्तिः->उपसर्गः.दत्त = p;

#अन्यथा

#यदि_परिभाषितम् NGX_CONF_उपसर्गः
        ngx_str_समुच्चयः(&आवृत्तिः->conf_उपसर्गः, NGX_CONF_उपसर्गः);
#अन्यथा
        ngx_str_समुच्चयः(&आवृत्तिः->conf_उपसर्गः, NGX_उपसर्गः);
#यदि_अन्तः
        ngx_str_समुच्चयः(&आवृत्तिः->उपसर्गः, NGX_उपसर्गः);

#यदि_अन्तः
    }

    यदि (ngx_conf_file) {
        आवृत्तिः->conf_file.len = ngx_strlen(ngx_conf_file);
        आवृत्तिः->conf_file.दत्त = ngx_conf_file;

    } अन्यथा {
        ngx_str_समुच्चयः(&आवृत्तिः->conf_file, NGX_CONF_मार्गः);
    }

    यदि (ngx_conf_पूर्णम्_name(आवृत्तिः, &आवृत्तिः->conf_file, 0) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (p = आवृत्तिः->conf_file.दत्त + आवृत्तिः->conf_file.len - 1;
         p > आवृत्तिः->conf_file.दत्त;
         p--)
    {
        यदि (ngx_मार्गः_separator(*p)) {
            आवृत्तिः->conf_उपसर्गः.len = p - आवृत्तिः->conf_file.दत्त + 1;
            आवृत्तिः->conf_उपसर्गः.दत्त = आवृत्तिः->conf_file.दत्त;
            भङ्ग;
        }
    }

    यदि (ngx_error_log) {
        आवृत्तिः->error_log.len = ngx_strlen(ngx_error_log);
        आवृत्तिः->error_log.दत्त = ngx_error_log;

    } अन्यथा {
        ngx_str_समुच्चयः(&आवृत्तिः->error_log, NGX_ERROR_LOG_मार्गः);
    }

    यदि (ngx_conf_params) {
        आवृत्तिः->conf_param.len = ngx_strlen(ngx_conf_params);
        आवृत्तिः->conf_param.दत्त = ngx_conf_params;
    }

    यदि (ngx_test_config) {
        आवृत्तिः->लघुगणकः->log_level = NGX_LOG_INFO;
    }

    प्रत्यागम NGX_OK;
}


स्थिर शून्य *
ngx_core_module_create_conf(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(आवृत्तिः->pool, परिमाण(ngx_core_conf_t));
    यदि (ccf == रिक्त) {
        प्रत्यागम रिक्त;
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

    ccf->कार्यकर्ता_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->गणः = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    यदि (ngx_array_init(&ccf->env, आवृत्तिः->pool, 1, परिमाण(ngx_str_t))
        != NGX_OK)
    {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम ccf;
}


स्थिर वर्ण *
ngx_core_module_init_conf(ngx_आवृत्तिः_t *आवृत्तिः, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_मानम्(ccf->daemon, 1);
    ngx_conf_init_मानम्(ccf->master, 1);
    ngx_conf_init_msec_मानम्(ccf->timer_resolution, 0);
    ngx_conf_init_msec_मानम्(ccf->shutdown_timeout, 0);

    ngx_conf_init_मानम्(ccf->कार्यकर्ता_processes, 1);
    ngx_conf_init_मानम्(ccf->debug_points, 0);

#यदि (NGX_HAVE_CPU_AFFINITY)

    यदि (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->कार्यकर्ता_processes)
    {
        ngx_log_error(NGX_LOG_WARN, आवृत्तिः->लघुगणकः, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#यदि_अन्तः


    यदि (ccf->pid.len == 0) {
        ngx_str_समुच्चयः(&ccf->pid, NGX_PID_मार्गः);
    }

    यदि (ngx_conf_पूर्णम्_name(आवृत्तिः, &ccf->pid, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + परिमाण(NGX_OLDPID_EXT);

    ccf->oldpid.दत्त = ngx_pnalloc(आवृत्तिः->pool, ccf->oldpid.len);
    यदि (ccf->oldpid.दत्त == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.दत्त, ccf->pid.दत्त, ccf->pid.len),
               NGX_OLDPID_EXT, परिमाण(NGX_OLDPID_EXT));


    यदि (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        यदि (ngx_conf_पूर्णम्_name(आवृत्तिः, &ccf->working_directory, 0) != NGX_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (ngx_file_info(ccf->working_directory.दत्त, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.दत्त);
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.दत्त);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }


#यदि !(NGX_WIN32)

    यदि (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        संरचना गणः   *grp;
        संरचना passwd  *pwd;

        ngx_समुच्चयः_errno(0);
        pwd = getpwnam(NGX_USER);
        यदि (pwd == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_समुच्चयः_errno(0);
        grp = getgrnam(NGX_गणः);
        यदि (grp == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          "getgrnam(\"" NGX_गणः "\") failed");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->गणः = grp->gr_gid;
    }


    यदि (ccf->तालकम्_file.len == 0) {
        ngx_str_समुच्चयः(&ccf->तालकम्_file, NGX_तालकम्_मार्गः);
    }

    यदि (ngx_conf_पूर्णम्_name(आवृत्तिः, &ccf->तालकम्_file, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    {
    ngx_str_t  तालकम्_file;

    तालकम्_file = आवृत्तिः->old_आवृत्तिः->तालकम्_file;

    यदि (तालकम्_file.len) {
        तालकम्_file.len--;

        यदि (ccf->तालकम्_file.len != तालकम्_file.len
            || ngx_strncmp(ccf->तालकम्_file.दत्त, तालकम्_file.दत्त, तालकम्_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        आवृत्तिः->तालकम्_file.len = तालकम्_file.len + 1;
        तालकम्_file.len += परिमाण(".accept");

        आवृत्तिः->तालकम्_file.दत्त = ngx_pstrdup(आवृत्तिः->pool, &तालकम्_file);
        यदि (आवृत्तिः->तालकम्_file.दत्त == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

    } अन्यथा {
        आवृत्तिः->तालकम्_file.len = ccf->तालकम्_file.len + 1;
        आवृत्तिः->तालकम्_file.दत्त = ngx_pnalloc(आवृत्तिः->pool,
                                      ccf->तालकम्_file.len + परिमाण(".accept"));
        यदि (आवृत्तिः->तालकम्_file.दत्त == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(आवृत्तिः->तालकम्_file.दत्त, ccf->तालकम्_file.दत्त,
                              ccf->तालकम्_file.len),
                   ".accept", परिमाण(".accept"));
    }
    }

#यदि_अन्तः

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_समुच्चयः_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    प्रत्यागम NGX_CONF_OK;

#अन्यथा

    ngx_core_conf_t  *ccf = conf;

    वर्ण             *गणः;
    संरचना passwd    *pwd;
    संरचना गणः     *grp;
    ngx_str_t        *मानम्;

    यदि (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    यदि (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        प्रत्यागम NGX_CONF_OK;
    }

    मानम् = cf->तर्काः->elts;

    ccf->username = (वर्ण *) मानम्[1].दत्त;

    ngx_समुच्चयः_errno(0);
    pwd = getpwnam((ध्रुव वर्ण *) मानम्[1].दत्त);
    यदि (pwd == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", मानम्[1].दत्त);
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    गणः = (वर्ण *) ((cf->तर्काः->nelts == 2) ? मानम्[1].दत्त : मानम्[2].दत्त);

    ngx_समुच्चयः_errno(0);
    grp = getgrnam(गणः);
    यदि (grp == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", गणः);
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->गणः = grp->gr_gid;

    प्रत्यागम NGX_CONF_OK;

#यदि_अन्तः
}


स्थिर वर्ण *
ngx_समुच्चयः_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *मानम्, *var;
    ngx_uint_t   i;

    var = ngx_array_प्रक्षिप(&ccf->env);
    यदि (var == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मानम् = cf->तर्काः->elts;
    *var = मानम्[1];

    प्रति (i = 0; i < मानम्[1].len; i++) {

        यदि (मानम्[1].दत्त[i] == '=') {

            var->len = i;

            प्रत्यागम NGX_CONF_OK;
        }
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_समुच्चयः_प्राथमिकता(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *मानम्;
    ngx_uint_t        n, minus;

    यदि (ccf->प्राथमिकता != 0) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    यदि (मानम्[1].दत्त[0] == '-') {
        n = 1;
        minus = 1;

    } अन्यथा यदि (मानम्[1].दत्त[0] == '+') {
        n = 1;
        minus = 0;

    } अन्यथा {
        n = 0;
        minus = 0;
    }

    ccf->प्राथमिकता = ngx_atoi(&मानम्[1].दत्त[n], मानम्[1].len - n);
    यदि (ccf->प्राथमिकता == NGX_ERROR) {
        प्रत्यागम "invalid number";
    }

    यदि (minus) {
        ccf->प्राथमिकता = -ccf->प्राथमिकता;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_समुच्चयः_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *मानम्;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *आवरणम्;

    यदि (ccf->cpu_affinity) {
        प्रत्यागम "is duplicate";
    }

    आवरणम् = ngx_palloc(cf->pool, (cf->तर्काः->nelts - 1) * परिमाण(ngx_cpuset_t));
    यदि (आवरणम् == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->तर्काः->nelts - 1;
    ccf->cpu_affinity = आवरणम्;

    मानम् = cf->तर्काः->elts;

    यदि (ngx_strcmp(मानम्[1].दत्त, "auto") == 0) {

        यदि (cf->तर्काः->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&आवरणम्[0]);
        प्रति (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_समुच्चयः(i, &आवरणम्[0]);
        }

        n = 2;

    } अन्यथा {
        n = 1;
    }

    प्रति ( /* void */ ; n < cf->तर्काः->nelts; n++) {

        यदि (मानम्[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            प्रत्यागम NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&आवरणम्[n - 1]);

        प्रति (p = मानम्[n].दत्त + मानम्[n].len - 1;
             p >= मानम्[n].दत्त;
             p--)
        {
            ch = *p;

            यदि (ch == ' ') {
                अनुवर्त;
            }

            i++;

            यदि (ch == '0') {
                अनुवर्त;
            }

            यदि (ch == '1') {
                CPU_समुच्चयः(i - 1, &आवरणम्[n - 1]);
                अनुवर्त;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

#अन्यथा

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#यदि_अन्तः

    प्रत्यागम NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#यदि (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *आवरणम्;
    ngx_core_conf_t  *ccf;

    स्थिर ngx_cpuset_t  परिणाम;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_आवृत्तिः->conf_ctx,
                                           ngx_core_module);

    यदि (ccf->cpu_affinity == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ccf->cpu_affinity_auto) {
        आवरणम् = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        प्रति (i = 0, j = n; /* void */ ; i++) {

            यदि (CPU_ISSET(i % CPU_SETSIZE, आवरणम्) && j-- == 0) {
                भङ्ग;
            }

            यदि (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                प्रत्यागम रिक्त;
            }

            /* void */
        }

        CPU_ZERO(&परिणाम);
        CPU_समुच्चयः(i % CPU_SETSIZE, &परिणाम);

        प्रत्यागम &परिणाम;
    }

    यदि (ccf->cpu_affinity_n > n) {
        प्रत्यागम &ccf->cpu_affinity[n];
    }

    प्रत्यागम &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#अन्यथा

    प्रत्यागम रिक्त;

#यदि_अन्तः
}


स्थिर वर्ण *
ngx_समुच्चयः_कार्यकर्ता_processes(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t        *मानम्;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    यदि (ccf->कार्यकर्ता_processes != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    यदि (ngx_strcmp(मानम्[1].दत्त, "auto") == 0) {
        ccf->कार्यकर्ता_processes = ngx_ncpu;
        प्रत्यागम NGX_CONF_OK;
    }

    ccf->कार्यकर्ता_processes = ngx_atoi(मानम्[1].दत्त, मानम्[1].len);

    यदि (ccf->कार्यकर्ता_processes == NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_आरोपय_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_HAVE_DLOPEN)
    शून्य                *handle;
    वर्ण               **नामानि, **क्रमः;
    ngx_str_t           *मानम्, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    यदि (cf->आवृत्तिः->modules_प्रयुक्तम्) {
        प्रत्यागम "is specified too late";
    }

    मानम् = cf->तर्काः->elts;

    file = मानम्[1];

    यदि (ngx_conf_पूर्णम्_name(cf->आवृत्तिः, &file, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_सङ्कलय(cf->आवृत्तिः->pool, 0);
    यदि (cln == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.दत्त);
    यदि (handle == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.दत्त, ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->दत्त = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    यदि (modules == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मानम्[1], "ngx_modules", ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    नामानि = ngx_dlsym(handle, "ngx_module_names");
    यदि (नामानि == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मानम्[1], "ngx_module_names", ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    क्रमः = ngx_dlsym(handle, "ngx_module_order");

    प्रति (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = नामानि[i];

        यदि (ngx_सङ्कलय_module(cf, &file, module, क्रमः) != NGX_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0, "module: %s i:%ui",
                       module->name, module->सूचकांक);
    }

    प्रत्यागम NGX_CONF_OK;

#अन्यथा

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    प्रत्यागम NGX_CONF_ERROR;

#यदि_अन्तः
}


#यदि (NGX_HAVE_DLOPEN)

स्थिर शून्य
ngx_unload_module(शून्य *दत्त)
{
    शून्य  *handle = दत्त;

    यदि (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#यदि_अन्तः
