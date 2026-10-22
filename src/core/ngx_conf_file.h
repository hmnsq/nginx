
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


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

#define NGX_CONF_ARGS_عدد 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (تهی *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          پوچ
#define NGX_CONF_ERROR       (تهی *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


ساختار ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    نویسه               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    تهی                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, پوچ, 0, 0, پوچ }


ساختار ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    تهی                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    تهی                 *data;
};


تعریفنوع ساختار {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            سطر;
} ngx_conf_file_t;


تعریفنوع ساختار {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


تعریفنوع نویسه *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, تهی *conf);


ساختار ngx_conf_s {
    نویسه                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    تهی                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    تهی                 *handler_conf;
};


تعریفنوع نویسه *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    تهی *data, تهی *conf);

تعریفنوع ساختار {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


تعریفنوع ساختار {
    ngx_conf_post_handler_pt  post_handler;
    نویسه                     *old_name;
    نویسه                     *new_name;
} ngx_conf_deprecated_t;


تعریفنوع ساختار {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 کم;
    ngx_int_t                 بالا;
} ngx_conf_رقم_bounds_t;


تعریفنوع ساختار {
    ngx_str_t                 name;
    ngx_uint_t                مقدار;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

تعریفنوع ساختار {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



نویسه * ngx_conf_deprecated(ngx_conf_t *cf, تهی *post, تهی *data);
نویسه *ngx_conf_check_رقم_bounds(ngx_conf_t *cf, تهی *post, تهی *data);
نویسه *ngx_conf_check_size_nonzero(ngx_conf_t *cf, تهی *post, تهی *data);


بیرونی ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_مقدار(conf, پیشفرض)                                   \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = پیشفرض;                                                      \
    }

#define ngx_conf_init_ptr_مقدار(conf, پیشفرض)                               \
    اگر (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = پیشفرض;                                                      \
    }

#define ngx_conf_init_uint_مقدار(conf, پیشفرض)                              \
    اگر (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = پیشفرض;                                                      \
    }

#define ngx_conf_init_size_مقدار(conf, پیشفرض)                              \
    اگر (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = پیشفرض;                                                      \
    }

#define ngx_conf_init_msec_مقدار(conf, پیشفرض)                              \
    اگر (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = پیشفرض;                                                      \
    }

#define ngx_conf_merge_مقدار(conf, prev, پیشفرض)                            \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? پیشفرض : prev;                    \
    }

#define ngx_conf_merge_ptr_مقدار(conf, prev, پیشفرض)                        \
    اگر (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? پیشفرض : prev;                \
    }

#define ngx_conf_merge_uint_مقدار(conf, prev, پیشفرض)                       \
    اگر (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? پیشفرض : prev;               \
    }

#define ngx_conf_merge_msec_مقدار(conf, prev, پیشفرض)                       \
    اگر (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? پیشفرض : prev;               \
    }

#define ngx_conf_merge_sec_مقدار(conf, prev, پیشفرض)                        \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? پیشفرض : prev;                    \
    }

#define ngx_conf_merge_size_مقدار(conf, prev, پیشفرض)                       \
    اگر (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? پیشفرض : prev;               \
    }

#define ngx_conf_merge_off_مقدار(conf, prev, پیشفرض)                        \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? پیشفرض : prev;                    \
    }

#define ngx_conf_merge_str_مقدار(conf, prev, پیشفرض)                        \
    اگر (conf.data == پوچ) {                                                 \
        اگر (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } وگرنه {                                                             \
            conf.len = اندازه(پیشفرض) - 1;                                  \
            conf.data = (u_char *) پیشفرض;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_مقدار(conf, prev, default_رقم, default_size)     \
    اگر (conf.رقم == 0) {                                                     \
        اگر (prev.رقم) {                                                      \
            conf.رقم = prev.رقم;                                             \
            conf.size = prev.size;                                           \
        } وگرنه {                                                             \
            conf.رقم = default_رقم;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_مقدار(conf, prev, پیشفرض)                    \
    اگر (conf == 0) {                                                         \
        conf = (prev == 0) ? پیشفرض : prev;                                 \
    }


نویسه *ngx_conf_param(ngx_conf_t *cf);
نویسه *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
نویسه *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
تهی ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت نویسه *fmt, ...);


نویسه *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    تهی *conf);
نویسه *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_رقم_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
نویسه *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
