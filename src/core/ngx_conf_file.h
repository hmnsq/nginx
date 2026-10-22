
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


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

#define NGX_CONF_ARGS_સંખ્યા 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (શૂન્ય *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          ખાલી
#define NGX_CONF_ERROR       (શૂન્ય *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


સંરચના ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    અક્ષર               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    શૂન્ય                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, ખાલી, 0, 0, ખાલી }


સંરચના ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    શૂન્ય                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    શૂન્ય                 *data;
};


પ્રકારવ્યાખ્યા સંરચના {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            લીટી;
} ngx_conf_file_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


પ્રકારવ્યાખ્યા અક્ષર *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, શૂન્ય *conf);


સંરચના ngx_conf_s {
    અક્ષર                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    શૂન્ય                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    શૂન્ય                 *handler_conf;
};


પ્રકારવ્યાખ્યા અક્ષર *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    શૂન્ય *data, શૂન્ય *conf);

પ્રકારવ્યાખ્યા સંરચના {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_conf_post_handler_pt  post_handler;
    અક્ષર                     *old_name;
    અક્ષર                     *new_name;
} ngx_conf_deprecated_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 નીચલું;
    ngx_int_t                 ઉપલું;
} ngx_conf_અંક_bounds_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t                 name;
    ngx_uint_t                કિંમત;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



અક્ષર * ngx_conf_deprecated(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data);
અક્ષર *ngx_conf_check_અંક_bounds(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data);
અક્ષર *ngx_conf_check_size_nonzero(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data);


બાહ્ય ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_કિંમત(conf, મૂળભૂત)                                   \
    જો (conf == NGX_CONF_UNSET) {                                            \
        conf = મૂળભૂત;                                                      \
    }

#define ngx_conf_init_ptr_કિંમત(conf, મૂળભૂત)                               \
    જો (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = મૂળભૂત;                                                      \
    }

#define ngx_conf_init_uint_કિંમત(conf, મૂળભૂત)                              \
    જો (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = મૂળભૂત;                                                      \
    }

#define ngx_conf_init_size_કિંમત(conf, મૂળભૂત)                              \
    જો (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = મૂળભૂત;                                                      \
    }

#define ngx_conf_init_msec_કિંમત(conf, મૂળભૂત)                              \
    જો (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = મૂળભૂત;                                                      \
    }

#define ngx_conf_merge_કિંમત(conf, prev, મૂળભૂત)                            \
    જો (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? મૂળભૂત : prev;                    \
    }

#define ngx_conf_merge_ptr_કિંમત(conf, prev, મૂળભૂત)                        \
    જો (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? મૂળભૂત : prev;                \
    }

#define ngx_conf_merge_uint_કિંમત(conf, prev, મૂળભૂત)                       \
    જો (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? મૂળભૂત : prev;               \
    }

#define ngx_conf_merge_msec_કિંમત(conf, prev, મૂળભૂત)                       \
    જો (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? મૂળભૂત : prev;               \
    }

#define ngx_conf_merge_sec_કિંમત(conf, prev, મૂળભૂત)                        \
    જો (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? મૂળભૂત : prev;                    \
    }

#define ngx_conf_merge_size_કિંમત(conf, prev, મૂળભૂત)                       \
    જો (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? મૂળભૂત : prev;               \
    }

#define ngx_conf_merge_off_કિંમત(conf, prev, મૂળભૂત)                        \
    જો (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? મૂળભૂત : prev;                    \
    }

#define ngx_conf_merge_str_કિંમત(conf, prev, મૂળભૂત)                        \
    જો (conf.data == ખાલી) {                                                 \
        જો (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } નહીંતર {                                                             \
            conf.len = કદ(મૂળભૂત) - 1;                                  \
            conf.data = (u_char *) મૂળભૂત;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_કિંમત(conf, prev, default_અંક, default_size)     \
    જો (conf.અંક == 0) {                                                     \
        જો (prev.અંક) {                                                      \
            conf.અંક = prev.અંક;                                             \
            conf.size = prev.size;                                           \
        } નહીંતર {                                                             \
            conf.અંક = default_અંક;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_કિંમત(conf, prev, મૂળભૂત)                    \
    જો (conf == 0) {                                                         \
        conf = (prev == 0) ? મૂળભૂત : prev;                                 \
    }


અક્ષર *ngx_conf_param(ngx_conf_t *cf);
અક્ષર *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
અક્ષર *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
શૂન્ય ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, અચલ અક્ષર *fmt, ...);


અક્ષર *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    શૂન્ય *conf);
અક્ષર *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_અંક_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
અક્ષર *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
