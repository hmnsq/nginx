
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


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

#define NGX_CONF_MAX_وسائط    8

#define NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#define NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#define NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#define NGX_CONF_وسائط_رقم 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (فراغ *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          عدم
#define NGX_CONF_ERROR       (فراغ *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


بنية ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    فراغ                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, عدم, 0, 0, عدم }


بنية ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    فراغ                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    فراغ                 *data;
};


تعريفنوع بنية {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            سطر;
} ngx_conf_file_t;


تعريفنوع بنية {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


تعريفنوع حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, فراغ *conf);


بنية ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *وسائط;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    فراغ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    فراغ                 *handler_conf;
};


تعريفنوع حرف *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    فراغ *data, فراغ *conf);

تعريفنوع بنية {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


تعريفنوع بنية {
    ngx_conf_post_handler_pt  post_handler;
    حرف                     *old_name;
    حرف                     *new_name;
} ngx_conf_deprecated_t;


تعريفنوع بنية {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 أدنى;
    ngx_int_t                 عالي;
} ngx_conf_رقم_bounds_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                قيمة;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *data);
حرف *ngx_conf_check_رقم_bounds(ngx_conf_t *cf, فراغ *post, فراغ *data);
حرف *ngx_conf_check_size_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *data);


خارجي ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_قيمة(conf, افتراضي)                                   \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = افتراضي;                                                      \
    }

#define ngx_conf_init_ptr_قيمة(conf, افتراضي)                               \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = افتراضي;                                                      \
    }

#define ngx_conf_init_uint_قيمة(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = افتراضي;                                                      \
    }

#define ngx_conf_init_size_قيمة(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = افتراضي;                                                      \
    }

#define ngx_conf_init_msec_قيمة(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = افتراضي;                                                      \
    }

#define ngx_conf_merge_قيمة(conf, prev, افتراضي)                            \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#define ngx_conf_merge_ptr_قيمة(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? افتراضي : prev;                \
    }

#define ngx_conf_merge_uint_قيمة(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? افتراضي : prev;               \
    }

#define ngx_conf_merge_msec_قيمة(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? افتراضي : prev;               \
    }

#define ngx_conf_merge_sec_قيمة(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#define ngx_conf_merge_size_قيمة(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? افتراضي : prev;               \
    }

#define ngx_conf_merge_off_قيمة(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#define ngx_conf_merge_str_قيمة(conf, prev, افتراضي)                        \
    إذا (conf.data == عدم) {                                                 \
        إذا (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } وإلا {                                                             \
            conf.len = حجم(افتراضي) - 1;                                  \
            conf.data = (u_char *) افتراضي;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_قيمة(conf, prev, default_رقم, default_size)     \
    إذا (conf.رقم == 0) {                                                     \
        إذا (prev.رقم) {                                                      \
            conf.رقم = prev.رقم;                                             \
            conf.size = prev.size;                                           \
        } وإلا {                                                             \
            conf.رقم = default_رقم;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_قيمة(conf, prev, افتراضي)                    \
    إذا (conf == 0) {                                                         \
        conf = (prev == 0) ? افتراضي : prev;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
فراغ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
حرف *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_رقم_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
