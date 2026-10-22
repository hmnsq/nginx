
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_CONF_FILE_H_INCLUDED_
#تعریف_کن _NGX_CONF_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#تعریف_کن NGX_CONF_NOARGS      0x00000001
#تعریف_کن NGX_CONF_TAKE1       0x00000002
#تعریف_کن NGX_CONF_TAKE2       0x00000004
#تعریف_کن NGX_CONF_TAKE3       0x00000008
#تعریف_کن NGX_CONF_TAKE4       0x00000010
#تعریف_کن NGX_CONF_TAKE5       0x00000020
#تعریف_کن NGX_CONF_TAKE6       0x00000040
#تعریف_کن NGX_CONF_TAKE7       0x00000080

#تعریف_کن NGX_CONF_MAX_آرگومانها    8

#تعریف_کن NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#تعریف_کن NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#تعریف_کن NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#تعریف_کن NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#تعریف_کن NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#تعریف_کن NGX_CONF_آرگومانها_عدد 0x000000ff
#تعریف_کن NGX_CONF_بلوک       0x00000100
#تعریف_کن NGX_CONF_FLAG        0x00000200
#تعریف_کن NGX_CONF_ANY         0x00000400
#تعریف_کن NGX_CONF_1_بیشتر       0x00000800
#تعریف_کن NGX_CONF_2_بیشتر       0x00001000

#تعریف_کن NGX_مستقیم_CONF      0x00010000

#تعریف_کن NGX_MAIN_CONF        0x01000000
#تعریف_کن NGX_ANY_CONF         0xFF000000



#تعریف_کن NGX_CONF_UNSET       -1
#تعریف_کن NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#تعریف_کن NGX_CONF_UNSET_PTR   (فراغ *) -1
#تعریف_کن NGX_CONF_UNSET_اندازه  (اندازه_t) -1
#تعریف_کن NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#تعریف_کن NGX_CONF_OK          عدم
#تعریف_کن NGX_CONF_ERROR       (فراغ *) -1

#تعریف_کن NGX_CONF_بلوک_شروع 1
#تعریف_کن NGX_CONF_بلوک_انجامشده  2
#تعریف_کن NGX_CONF_FILE_انجامشده   3

#تعریف_کن NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#تعریف_کن NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#تعریف_کن NGX_MAX_CONF_ERRSTR  1024


بنية ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*مجموعه)(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            جابه_جایی;
    فراغ                 *post;
};

#تعریف_کن ngx_null_command  { ngx_null_string, 0, عدم, 0, 0, عدم }


بنية ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    فراغ                (*flush)(ngx_open_file_t *file, ngx_log_t *لگاریتم);
    فراغ                 *داده;
};


تعريفنوع بنية {
    ngx_file_t            file;
    ngx_buf_t            *میانگیر;
    ngx_buf_t            *dump;
    ngx_uint_t            سطر;
} ngx_conf_file_t;


تعريفنوع بنية {
    ngx_str_t             name;
    ngx_buf_t            *میانگیر;
} ngx_conf_dump_t;


تعريفنوع حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, فراغ *conf);


بنية ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *آرگومانها;

    ngx_چرخه_t          *چرخه;
    ngx_pool_t           *pool;
    ngx_pool_t           *موقت_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *لگاریتم;

    فراغ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    فراغ                 *handler_conf;
};


تعريفنوع حرف *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    فراغ *داده, فراغ *conf);

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
    ngx_int_t                 کم;
    ngx_int_t                 بالا;
} ngx_conf_رقم_bounds_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                مقدار;
} ngx_conf_enum_t;


#تعریف_کن NGX_CONF_BITMASK_مجموعه  1

تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                ماسک;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *داده);
حرف *ngx_conf_check_رقم_bounds(ngx_conf_t *cf, فراغ *post, فراغ *داده);
حرف *ngx_conf_check_اندازه_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *داده);


خارجي ngx_conf_post_t  ngx_conf_اندازه_nonzero_post;


#تعریف_کن ngx_get_conf(conf_ctx, module)  conf_ctx[module.شاخص]



#تعریف_کن ngx_conf_init_مقدار(conf, افتراضي)                                   \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = افتراضي;                                                      \
    }

#تعریف_کن ngx_conf_init_ptr_مقدار(conf, افتراضي)                               \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = افتراضي;                                                      \
    }

#تعریف_کن ngx_conf_init_uint_مقدار(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کن ngx_conf_init_اندازه_مقدار(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_اندازه) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کن ngx_conf_init_msec_مقدار(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کن ngx_conf_ادغام_مقدار(conf, prev, افتراضي)                            \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کن ngx_conf_ادغام_ptr_مقدار(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? افتراضي : prev;                \
    }

#تعریف_کن ngx_conf_ادغام_uint_مقدار(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? افتراضي : prev;               \
    }

#تعریف_کن ngx_conf_ادغام_msec_مقدار(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? افتراضي : prev;               \
    }

#تعریف_کن ngx_conf_ادغام_sec_مقدار(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کن ngx_conf_ادغام_اندازه_مقدار(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_اندازه) {                                       \
        conf = (prev == NGX_CONF_UNSET_اندازه) ? افتراضي : prev;               \
    }

#تعریف_کن ngx_conf_ادغام_off_مقدار(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کن ngx_conf_ادغام_str_مقدار(conf, prev, افتراضي)                        \
    إذا (conf.داده == عدم) {                                                 \
        إذا (prev.داده) {                                                     \
            conf.len = prev.len;                                             \
            conf.داده = prev.داده;                                           \
        } وإلا {                                                             \
            conf.len = حجم(افتراضي) - 1;                                  \
            conf.داده = (u_char *) افتراضي;                                  \
        }                                                                    \
    }

#تعریف_کن ngx_conf_ادغام_bufs_مقدار(conf, prev, default_رقم, default_اندازه)     \
    إذا (conf.رقم == 0) {                                                     \
        إذا (prev.رقم) {                                                      \
            conf.رقم = prev.رقم;                                             \
            conf.اندازه = prev.اندازه;                                           \
        } وإلا {                                                             \
            conf.رقم = default_رقم;                                          \
            conf.اندازه = default_اندازه;                                        \
        }                                                                    \
    }

#تعریف_کن ngx_conf_ادغام_bitmask_مقدار(conf, prev, افتراضي)                    \
    إذا (conf == 0) {                                                         \
        conf = (prev == 0) ? افتراضي : prev;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


ngx_int_t ngx_conf_پر_name(ngx_چرخه_t *چرخه, ngx_str_t *name,
    ngx_uint_t conf_پیشوند);
ngx_open_file_t *ngx_conf_open_file(ngx_چرخه_t *چرخه, ngx_str_t *name);
فراغ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_مجموعه_flag_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_str_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_str_array_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
حرف *ngx_conf_مجموعه_keyval_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_رقم_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_اندازه_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_off_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_msec_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_sec_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_bufs_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_enum_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_مجموعه_bitmask_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


#پایان_اگر /* _NGX_CONF_FILE_H_INCLUDED_ */
