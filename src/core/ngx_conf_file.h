
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#define NGX_CONF_NOARGS      0x00000001
#define NGX_CONF_TAKE1       0x00000002
#define NGX_CONF_TAKE2       0x00000004
#define NGX_CONF_TAKE3       0x00000008
#define NGX_CONF_TAKE4       0x00000010
#define NGX_CONF_TAKE5       0x00000020
#define NGX_CONF_TAKE6       0x00000040
#define NGX_CONF_TAKE7       0x00000080

#define NGX_CONF_MAX_ARGS    8

#define NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#define NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#define NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#define NGX_CONF_ARGS_സംഖ്യ 0x000000ff
#define NGX_CONF_BLOCK       0x00000100
#define NGX_CONF_FLAG        0x00000200
#define NGX_CONF_ANY         0x00000400
#define NGX_CONF_1MORE       0x00000800
#define NGX_CONF_2MORE       0x00001000

#define NGX_DIRECT_CONF      0x00010000

#define NGX_MAIN_CONF        0x01000000
#define NGX_ANY_CONF         0xFF000000



#define NGX_CONF_UNSET       -1
#define NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#define NGX_CONF_UNSET_PTR   (ശൂന്യം *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          ശൂന്യ
#define NGX_CONF_ERROR       (ശൂന്യം *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


ഘടന ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    അക്ഷരം               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    ശൂന്യം                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, ശൂന്യ, 0, 0, ശൂന്യ }


ഘടന ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    ശൂന്യം                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    ശൂന്യം                 *data;
};


തരനിർവചനം ഘടന {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            വരി;
} ngx_conf_file_t;


തരനിർവചനം ഘടന {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


തരനിർവചനം അക്ഷരം *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, ശൂന്യം *conf);


ഘടന ngx_conf_s {
    അക്ഷരം                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    ശൂന്യം                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    ശൂന്യം                 *handler_conf;
};


തരനിർവചനം അക്ഷരം *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    ശൂന്യം *data, ശൂന്യം *conf);

തരനിർവചനം ഘടന {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


തരനിർവചനം ഘടന {
    ngx_conf_post_handler_pt  post_handler;
    അക്ഷരം                     *old_name;
    അക്ഷരം                     *new_name;
} ngx_conf_deprecated_t;


തരനിർവചനം ഘടന {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 താഴെയുള്ള;
    ngx_int_t                 മുകളിലെ;
} ngx_conf_അക്കം_bounds_t;


തരനിർവചനം ഘടന {
    ngx_str_t                 name;
    ngx_uint_t                മൂല്യം;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

തരനിർവചനം ഘടന {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



അക്ഷരം * ngx_conf_deprecated(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data);
അക്ഷരം *ngx_conf_check_അക്കം_bounds(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data);
അക്ഷരം *ngx_conf_check_size_nonzero(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data);


ബാഹ്യ ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_മൂല്യം(conf, സ്ഥിരസ്ഥിതി)                                   \
    എങ്കിൽ (conf == NGX_CONF_UNSET) {                                            \
        conf = സ്ഥിരസ്ഥിതി;                                                      \
    }

#define ngx_conf_init_ptr_മൂല്യം(conf, സ്ഥിരസ്ഥിതി)                               \
    എങ്കിൽ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = സ്ഥിരസ്ഥിതി;                                                      \
    }

#define ngx_conf_init_uint_മൂല്യം(conf, സ്ഥിരസ്ഥിതി)                              \
    എങ്കിൽ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = സ്ഥിരസ്ഥിതി;                                                      \
    }

#define ngx_conf_init_size_മൂല്യം(conf, സ്ഥിരസ്ഥിതി)                              \
    എങ്കിൽ (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = സ്ഥിരസ്ഥിതി;                                                      \
    }

#define ngx_conf_init_msec_മൂല്യം(conf, സ്ഥിരസ്ഥിതി)                              \
    എങ്കിൽ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = സ്ഥിരസ്ഥിതി;                                                      \
    }

#define ngx_conf_merge_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                            \
    എങ്കിൽ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? സ്ഥിരസ്ഥിതി : prev;                    \
    }

#define ngx_conf_merge_ptr_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                        \
    എങ്കിൽ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? സ്ഥിരസ്ഥിതി : prev;                \
    }

#define ngx_conf_merge_uint_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                       \
    എങ്കിൽ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? സ്ഥിരസ്ഥിതി : prev;               \
    }

#define ngx_conf_merge_msec_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                       \
    എങ്കിൽ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? സ്ഥിരസ്ഥിതി : prev;               \
    }

#define ngx_conf_merge_sec_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                        \
    എങ്കിൽ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? സ്ഥിരസ്ഥിതി : prev;                    \
    }

#define ngx_conf_merge_size_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                       \
    എങ്കിൽ (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? സ്ഥിരസ്ഥിതി : prev;               \
    }

#define ngx_conf_merge_off_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                        \
    എങ്കിൽ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? സ്ഥിരസ്ഥിതി : prev;                    \
    }

#define ngx_conf_merge_str_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                        \
    എങ്കിൽ (conf.data == ശൂന്യ) {                                                 \
        എങ്കിൽ (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } അല്ലെങ്കിൽ {                                                             \
            conf.len = വലുപ്പം(സ്ഥിരസ്ഥിതി) - 1;                                  \
            conf.data = (u_char *) സ്ഥിരസ്ഥിതി;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_മൂല്യം(conf, prev, default_അക്കം, default_size)     \
    എങ്കിൽ (conf.അക്കം == 0) {                                                     \
        എങ്കിൽ (prev.അക്കം) {                                                      \
            conf.അക്കം = prev.അക്കം;                                             \
            conf.size = prev.size;                                           \
        } അല്ലെങ്കിൽ {                                                             \
            conf.അക്കം = default_അക്കം;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_മൂല്യം(conf, prev, സ്ഥിരസ്ഥിതി)                    \
    എങ്കിൽ (conf == 0) {                                                         \
        conf = (prev == 0) ? സ്ഥിരസ്ഥിതി : prev;                                 \
    }


അക്ഷരം *ngx_conf_param(ngx_conf_t *cf);
അക്ഷരം *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
അക്ഷരം *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
ശൂന്യം ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, സ്ഥിരം അക്ഷരം *fmt, ...);


അക്ഷരം *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_അക്കം_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
അക്ഷരം *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
