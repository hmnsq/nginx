
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_CONF_FILE_H_INCLUDED_
#تعریف_کرو _NGX_CONF_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#تعریف_کرو NGX_CONF_NOARGS      0x00000001
#تعریف_کرو NGX_CONF_TAKE1       0x00000002
#تعریف_کرو NGX_CONF_TAKE2       0x00000004
#تعریف_کرو NGX_CONF_TAKE3       0x00000008
#تعریف_کرو NGX_CONF_TAKE4       0x00000010
#تعریف_کرو NGX_CONF_TAKE5       0x00000020
#تعریف_کرو NGX_CONF_TAKE6       0x00000040
#تعریف_کرو NGX_CONF_TAKE7       0x00000080

#تعریف_کرو NGX_CONF_MAX_دلیلاں    8

#تعریف_کرو NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#تعریف_کرو NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#تعریف_کرو NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#تعریف_کرو NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#تعریف_کرو NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#تعریف_کرو NGX_CONF_دلیلاں_نمبر 0x000000ff
#تعریف_کرو NGX_CONF_بلاک       0x00000100
#تعریف_کرو NGX_CONF_FLAG        0x00000200
#تعریف_کرو NGX_CONF_ANY         0x00000400
#تعریف_کرو NGX_CONF_1_ہور       0x00000800
#تعریف_کرو NGX_CONF_2_ہور       0x00001000

#تعریف_کرو NGX_سدھا_CONF      0x00010000

#تعریف_کرو NGX_MAIN_CONF        0x01000000
#تعریف_کرو NGX_ANY_CONF         0xFF000000



#تعریف_کرو NGX_CONF_UNSET       -1
#تعریف_کرو NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#تعریف_کرو NGX_CONF_UNSET_PTR   (فراغ *) -1
#تعریف_کرو NGX_CONF_UNSET_ناپ  (ناپ_t) -1
#تعریف_کرو NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#تعریف_کرو NGX_CONF_OK          عدم
#تعریف_کرو NGX_CONF_ERROR       (فراغ *) -1

#تعریف_کرو NGX_CONF_بلاک_شروع 1
#تعریف_کرو NGX_CONF_بلاک_پورا  2
#تعریف_کرو NGX_CONF_FILE_پورا   3

#تعریف_کرو NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#تعریف_کرو NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#تعریف_کرو NGX_MAX_CONF_ERRSTR  1024


بنية ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*سیٹ)(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            وتھ;
    فراغ                 *post;
};

#تعریف_کرو ngx_null_command  { ngx_null_string, 0, عدم, 0, 0, عدم }


بنية ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    فراغ                (*flush)(ngx_open_file_t *file, ngx_log_t *لوگارتھم);
    فراغ                 *انکڑے;
};


تعريفنوع بنية {
    ngx_file_t            file;
    ngx_buf_t            *بفر;
    ngx_buf_t            *dump;
    ngx_uint_t            سطر;
} ngx_conf_file_t;


تعريفنوع بنية {
    ngx_str_t             name;
    ngx_buf_t            *بفر;
} ngx_conf_dump_t;


تعريفنوع حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, فراغ *conf);


بنية ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *دلیلاں;

    ngx_چکر_t          *چکر;
    ngx_pool_t           *pool;
    ngx_pool_t           *عارضی_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *لوگارتھم;

    فراغ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    فراغ                 *handler_conf;
};


تعريفنوع حرف *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    فراغ *انکڑے, فراغ *conf);

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
    ngx_int_t                 بلند;
} ngx_conf_عدد_bounds_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                قدر;
} ngx_conf_enum_t;


#تعریف_کرو NGX_CONF_BITMASK_سیٹ  1

تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                نقاب;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے);
حرف *ngx_conf_check_عدد_bounds(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے);
حرف *ngx_conf_check_ناپ_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے);


خارجي ngx_conf_post_t  ngx_conf_ناپ_nonzero_post;


#تعریف_کرو ngx_get_conf(conf_ctx, module)  conf_ctx[module.اشاریہ]



#تعریف_کرو ngx_conf_init_قدر(conf, افتراضي)                                   \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = افتراضي;                                                      \
    }

#تعریف_کرو ngx_conf_init_ptr_قدر(conf, افتراضي)                               \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = افتراضي;                                                      \
    }

#تعریف_کرو ngx_conf_init_uint_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کرو ngx_conf_init_ناپ_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_ناپ) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کرو ngx_conf_init_msec_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کرو ngx_conf_ملاؤ_قدر(conf, prev, افتراضي)                            \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_ptr_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? افتراضي : prev;                \
    }

#تعریف_کرو ngx_conf_ملاؤ_uint_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? افتراضي : prev;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_msec_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? افتراضي : prev;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_sec_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_ناپ_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_ناپ) {                                       \
        conf = (prev == NGX_CONF_UNSET_ناپ) ? افتراضي : prev;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_off_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_str_قدر(conf, prev, افتراضي)                        \
    إذا (conf.انکڑے == عدم) {                                                 \
        إذا (prev.انکڑے) {                                                     \
            conf.len = prev.len;                                             \
            conf.انکڑے = prev.انکڑے;                                           \
        } وإلا {                                                             \
            conf.len = حجم(افتراضي) - 1;                                  \
            conf.انکڑے = (u_char *) افتراضي;                                  \
        }                                                                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_bufs_قدر(conf, prev, default_عدد, default_ناپ)     \
    إذا (conf.عدد == 0) {                                                     \
        إذا (prev.عدد) {                                                      \
            conf.عدد = prev.عدد;                                             \
            conf.ناپ = prev.ناپ;                                           \
        } وإلا {                                                             \
            conf.عدد = default_عدد;                                          \
            conf.ناپ = default_ناپ;                                        \
        }                                                                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_bitmask_قدر(conf, prev, افتراضي)                    \
    إذا (conf == 0) {                                                         \
        conf = (prev == 0) ? افتراضي : prev;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


ngx_int_t ngx_conf_بھریا_name(ngx_چکر_t *چکر, ngx_str_t *name,
    ngx_uint_t conf_اگیتر);
ngx_open_file_t *ngx_conf_open_file(ngx_چکر_t *چکر, ngx_str_t *name);
فراغ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_سیٹ_flag_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_str_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_str_array_تھاں(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
حرف *ngx_conf_سیٹ_keyval_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_عدد_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_ناپ_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_off_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_msec_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_sec_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_bufs_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_enum_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سیٹ_bitmask_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


#جے_انت /* _NGX_CONF_FILE_H_INCLUDED_ */
