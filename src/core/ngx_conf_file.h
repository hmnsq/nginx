
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


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

#define NGX_CONF_ARGS_ସଂଖ୍ୟା 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (ଶୂନ୍ୟ *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          ଖାଲି
#define NGX_CONF_ERROR       (ଶୂନ୍ୟ *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


ସଂରଚନା ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    ଅକ୍ଷର               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    ଶୂନ୍ୟ                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, ଖାଲି, 0, 0, ଖାଲି }


ସଂରଚନା ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    ଶୂନ୍ୟ                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    ଶୂନ୍ୟ                 *data;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            ଧାଡ଼ି;
} ngx_conf_file_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅକ୍ଷର *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, ଶୂନ୍ୟ *conf);


ସଂରଚନା ngx_conf_s {
    ଅକ୍ଷର                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    ଶୂନ୍ୟ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    ଶୂନ୍ୟ                 *handler_conf;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅକ୍ଷର *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    ଶୂନ୍ୟ *data, ଶୂନ୍ୟ *conf);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_conf_post_handler_pt  post_handler;
    ଅକ୍ଷର                     *old_name;
    ଅକ୍ଷର                     *new_name;
} ngx_conf_deprecated_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 ତଳ;
    ngx_int_t                 ଉପର;
} ngx_conf_ଅଙ୍କ_bounds_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t                 name;
    ngx_uint_t                ମୂଲ୍ୟ;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



ଅକ୍ଷର * ngx_conf_deprecated(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data);
ଅକ୍ଷର *ngx_conf_check_ଅଙ୍କ_bounds(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data);
ଅକ୍ଷର *ngx_conf_check_size_nonzero(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data);


ବାହ୍ୟ ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_ମୂଲ୍ୟ(conf, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                                   \
    ଯଦି (conf == NGX_CONF_UNSET) {                                            \
        conf = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                                      \
    }

#define ngx_conf_init_ptr_ମୂଲ୍ୟ(conf, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                               \
    ଯଦି (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                                      \
    }

#define ngx_conf_init_uint_ମୂଲ୍ୟ(conf, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                              \
    ଯଦି (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                                      \
    }

#define ngx_conf_init_size_ମୂଲ୍ୟ(conf, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                              \
    ଯଦି (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                                      \
    }

#define ngx_conf_init_msec_ମୂଲ୍ୟ(conf, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                              \
    ଯଦି (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                                      \
    }

#define ngx_conf_merge_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                            \
    ଯଦି (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;                    \
    }

#define ngx_conf_merge_ptr_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                        \
    ଯଦି (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;                \
    }

#define ngx_conf_merge_uint_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                       \
    ଯଦି (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;               \
    }

#define ngx_conf_merge_msec_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                       \
    ଯଦି (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;               \
    }

#define ngx_conf_merge_sec_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                        \
    ଯଦି (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;                    \
    }

#define ngx_conf_merge_size_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                       \
    ଯଦି (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;               \
    }

#define ngx_conf_merge_off_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                        \
    ଯଦି (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;                    \
    }

#define ngx_conf_merge_str_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                        \
    ଯଦି (conf.data == ଖାଲି) {                                                 \
        ଯଦି (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } ଅନ୍ୟଥା {                                                             \
            conf.len = ଆକାର(ପୂର୍ବନିର୍ଦ୍ଧାରିତ) - 1;                                  \
            conf.data = (u_char *) ପୂର୍ବନିର୍ଦ୍ଧାରିତ;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_ମୂଲ୍ୟ(conf, prev, default_ଅଙ୍କ, default_size)     \
    ଯଦି (conf.ଅଙ୍କ == 0) {                                                     \
        ଯଦି (prev.ଅଙ୍କ) {                                                      \
            conf.ଅଙ୍କ = prev.ଅଙ୍କ;                                             \
            conf.size = prev.size;                                           \
        } ଅନ୍ୟଥା {                                                             \
            conf.ଅଙ୍କ = default_ଅଙ୍କ;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_ମୂଲ୍ୟ(conf, prev, ପୂର୍ବନିର୍ଦ୍ଧାରିତ)                    \
    ଯଦି (conf == 0) {                                                         \
        conf = (prev == 0) ? ପୂର୍ବନିର୍ଦ୍ଧାରିତ : prev;                                 \
    }


ଅକ୍ଷର *ngx_conf_param(ngx_conf_t *cf);
ଅକ୍ଷର *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
ଅକ୍ଷର *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
ଶୂନ୍ୟ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...);


ଅକ୍ଷର *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_ଅଙ୍କ_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ଅକ୍ଷର *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
