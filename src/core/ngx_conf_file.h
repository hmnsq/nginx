
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_CONF_FILE_H_INCLUDED_
#تعریف_کرو _NGX_CONF_FILE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


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

#تعریف_کرو NGX_CONF_MAX_دلائل    8

#تعریف_کرو NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#تعریف_کرو NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#تعریف_کرو NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#تعریف_کرو NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#تعریف_کرو NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#تعریف_کرو NGX_CONF_دلائل_نمبر 0x000000ff
#تعریف_کرو NGX_CONF_بلاک       0x00000100
#تعریف_کرو NGX_CONF_FLAG        0x00000200
#تعریف_کرو NGX_CONF_ANY         0x00000400
#تعریف_کرو NGX_CONF_1_مزید       0x00000800
#تعریف_کرو NGX_CONF_2_مزید       0x00001000

#تعریف_کرو NGX_براہ_راست_CONF      0x00010000

#تعریف_کرو NGX_MAIN_CONF        0x01000000
#تعریف_کرو NGX_ANY_CONF         0xFF000000



#تعریف_کرو NGX_CONF_UNSET       -1
#تعریف_کرو NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#تعریف_کرو NGX_CONF_UNSET_PTR   (خالی *) -1
#تعریف_کرو NGX_CONF_UNSET_سائز  (سائز_t) -1
#تعریف_کرو NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#تعریف_کرو NGX_CONF_OK          تہی
#تعریف_کرو NGX_CONF_ERROR       (خالی *) -1

#تعریف_کرو NGX_CONF_بلاک_آغاز 1
#تعریف_کرو NGX_CONF_بلاک_مکمل  2
#تعریف_کرو NGX_CONF_FILE_مکمل   3

#تعریف_کرو NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#تعریف_کرو NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#تعریف_کرو NGX_MAX_CONF_ERRSTR  1024


ساخت ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*سیٹ)(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
    ngx_uint_t            conf;
    ngx_uint_t            انحراف;
    خالی                 *post;
};

#تعریف_کرو ngx_null_command  { ngx_null_string, 0, تہی, 0, 0, تہی }


ساخت ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    خالی                (*flush)(ngx_open_file_t *file, ngx_log_t *لوگارتھم);
    خالی                 *ڈیٹا;
};


قسمتعریف ساخت {
    ngx_file_t            file;
    ngx_buf_t            *بفر;
    ngx_buf_t            *dump;
    ngx_uint_t            سطر;
} ngx_conf_file_t;


قسمتعریف ساخت {
    ngx_str_t             name;
    ngx_buf_t            *بفر;
} ngx_conf_dump_t;


قسمتعریف حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, خالی *conf);


ساخت ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *دلائل;

    ngx_چکر_t          *چکر;
    ngx_pool_t           *pool;
    ngx_pool_t           *عارضی_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *لوگارتھم;

    خالی                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    خالی                 *handler_conf;
};


قسمتعریف حرف *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    خالی *ڈیٹا, خالی *conf);

قسمتعریف ساخت {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


قسمتعریف ساخت {
    ngx_conf_post_handler_pt  post_handler;
    حرف                     *old_name;
    حرف                     *new_name;
} ngx_conf_deprecated_t;


قسمتعریف ساخت {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 کم;
    ngx_int_t                 بلند;
} ngx_conf_عدد_bounds_t;


قسمتعریف ساخت {
    ngx_str_t                 name;
    ngx_uint_t                قدر;
} ngx_conf_enum_t;


#تعریف_کرو NGX_CONF_BITMASK_سیٹ  1

قسمتعریف ساخت {
    ngx_str_t                 name;
    ngx_uint_t                نقاب;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا);
حرف *ngx_conf_check_عدد_bounds(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا);
حرف *ngx_conf_check_سائز_nonzero(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا);


خارجی ngx_conf_post_t  ngx_conf_سائز_nonzero_post;


#تعریف_کرو ngx_get_conf(conf_ctx, module)  conf_ctx[module.اشاریہ]



#تعریف_کرو ngx_conf_init_قدر(conf, طےشدہ)                                   \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = طےشدہ;                                                      \
    }

#تعریف_کرو ngx_conf_init_ptr_قدر(conf, طےشدہ)                               \
    اگر (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = طےشدہ;                                                      \
    }

#تعریف_کرو ngx_conf_init_uint_قدر(conf, طےشدہ)                              \
    اگر (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = طےشدہ;                                                      \
    }

#تعریف_کرو ngx_conf_init_سائز_قدر(conf, طےشدہ)                              \
    اگر (conf == NGX_CONF_UNSET_سائز) {                                       \
        conf = طےشدہ;                                                      \
    }

#تعریف_کرو ngx_conf_init_msec_قدر(conf, طےشدہ)                              \
    اگر (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = طےشدہ;                                                      \
    }

#تعریف_کرو ngx_conf_ملاؤ_قدر(conf, پچھلا, طےشدہ)                            \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (پچھلا == NGX_CONF_UNSET) ? طےشدہ : پچھلا;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_ptr_قدر(conf, پچھلا, طےشدہ)                        \
    اگر (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (پچھلا == NGX_CONF_UNSET_PTR) ? طےشدہ : پچھلا;                \
    }

#تعریف_کرو ngx_conf_ملاؤ_uint_قدر(conf, پچھلا, طےشدہ)                       \
    اگر (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (پچھلا == NGX_CONF_UNSET_UINT) ? طےشدہ : پچھلا;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_msec_قدر(conf, پچھلا, طےشدہ)                       \
    اگر (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (پچھلا == NGX_CONF_UNSET_MSEC) ? طےشدہ : پچھلا;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_sec_قدر(conf, پچھلا, طےشدہ)                        \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (پچھلا == NGX_CONF_UNSET) ? طےشدہ : پچھلا;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_سائز_قدر(conf, پچھلا, طےشدہ)                       \
    اگر (conf == NGX_CONF_UNSET_سائز) {                                       \
        conf = (پچھلا == NGX_CONF_UNSET_سائز) ? طےشدہ : پچھلا;               \
    }

#تعریف_کرو ngx_conf_ملاؤ_off_قدر(conf, پچھلا, طےشدہ)                        \
    اگر (conf == NGX_CONF_UNSET) {                                            \
        conf = (پچھلا == NGX_CONF_UNSET) ? طےشدہ : پچھلا;                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_str_قدر(conf, پچھلا, طےشدہ)                        \
    اگر (conf.ڈیٹا == تہی) {                                                 \
        اگر (پچھلا.ڈیٹا) {                                                     \
            conf.len = پچھلا.len;                                             \
            conf.ڈیٹا = پچھلا.ڈیٹا;                                           \
        } ورنہ {                                                             \
            conf.len = حجم(طےشدہ) - 1;                                  \
            conf.ڈیٹا = (u_char *) طےشدہ;                                  \
        }                                                                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_bufs_قدر(conf, پچھلا, default_عدد, default_سائز)     \
    اگر (conf.عدد == 0) {                                                     \
        اگر (پچھلا.عدد) {                                                      \
            conf.عدد = پچھلا.عدد;                                             \
            conf.سائز = پچھلا.سائز;                                           \
        } ورنہ {                                                             \
            conf.عدد = default_عدد;                                          \
            conf.سائز = default_سائز;                                        \
        }                                                                    \
    }

#تعریف_کرو ngx_conf_ملاؤ_bitmask_قدر(conf, پچھلا, طےشدہ)                    \
    اگر (conf == 0) {                                                         \
        conf = (پچھلا == 0) ? طےشدہ : پچھلا;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);


ngx_int_t ngx_conf_بھرا_name(ngx_چکر_t *چکر, ngx_str_t *name,
    ngx_uint_t conf_سابقہ);
ngx_open_file_t *ngx_conf_open_file(ngx_چکر_t *چکر, ngx_str_t *name);
خالی ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_سیٹ_flag_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_str_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_str_array_خانہ(ngx_conf_t *cf, ngx_command_t *cmd,
    خالی *conf);
حرف *ngx_conf_سیٹ_keyval_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_عدد_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_سائز_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_off_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_msec_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_sec_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_bufs_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_enum_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
حرف *ngx_conf_سیٹ_bitmask_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);


#اگر_ختم /* _NGX_CONF_FILE_H_INCLUDED_ */
