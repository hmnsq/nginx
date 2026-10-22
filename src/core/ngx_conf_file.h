
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_CONF_FILE_H_INCLUDED_
#تعریف_کړه _NGX_CONF_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#تعریف_کړه NGX_CONF_NOARGS      0x00000001
#تعریف_کړه NGX_CONF_TAKE1       0x00000002
#تعریف_کړه NGX_CONF_TAKE2       0x00000004
#تعریف_کړه NGX_CONF_TAKE3       0x00000008
#تعریف_کړه NGX_CONF_TAKE4       0x00000010
#تعریف_کړه NGX_CONF_TAKE5       0x00000020
#تعریف_کړه NGX_CONF_TAKE6       0x00000040
#تعریف_کړه NGX_CONF_TAKE7       0x00000080

#تعریف_کړه NGX_CONF_MAX_دلیلونه    8

#تعریف_کړه NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#تعریف_کړه NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#تعریف_کړه NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#تعریف_کړه NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#تعریف_کړه NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#تعریف_کړه NGX_CONF_دلیلونه_شمېره 0x000000ff
#تعریف_کړه NGX_CONF_بلاک       0x00000100
#تعریف_کړه NGX_CONF_FLAG        0x00000200
#تعریف_کړه NGX_CONF_ANY         0x00000400
#تعریف_کړه NGX_CONF_1_نور       0x00000800
#تعریف_کړه NGX_CONF_2_نور       0x00001000

#تعریف_کړه NGX_مستقیم_CONF      0x00010000

#تعریف_کړه NGX_MAIN_CONF        0x01000000
#تعریف_کړه NGX_ANY_CONF         0xFF000000



#تعریف_کړه NGX_CONF_UNSET       -1
#تعریف_کړه NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#تعریف_کړه NGX_CONF_UNSET_PTR   (فراغ *) -1
#تعریف_کړه NGX_CONF_UNSET_اندازه  (اندازه_t) -1
#تعریف_کړه NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#تعریف_کړه NGX_CONF_OK          عدم
#تعریف_کړه NGX_CONF_ERROR       (فراغ *) -1

#تعریف_کړه NGX_CONF_بلاک_پیل 1
#تعریف_کړه NGX_CONF_بلاک_بشپړ  2
#تعریف_کړه NGX_CONF_FILE_بشپړ   3

#تعریف_کړه NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#تعریف_کړه NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#تعریف_کړه NGX_MAX_CONF_ERRSTR  1024


بنية ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*سېټ)(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            دننه_واټن;
    فراغ                 *post;
};

#تعریف_کړه ngx_null_command  { ngx_null_string, 0, عدم, 0, 0, عدم }


بنية ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    فراغ                (*flush)(ngx_open_file_t *file, ngx_log_t *لوګاریتم);
    فراغ                 *معلومات;
};


تعريفنوع بنية {
    ngx_file_t            file;
    ngx_buf_t            *بفر;
    ngx_buf_t            *dump;
    ngx_uint_t            کرښه;
} ngx_conf_file_t;


تعريفنوع بنية {
    ngx_str_t             name;
    ngx_buf_t            *بفر;
} ngx_conf_dump_t;


تعريفنوع حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, فراغ *conf);


بنية ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *دلیلونه;

    ngx_دوره_t          *دوره;
    ngx_pool_t           *pool;
    ngx_pool_t           *لنډمهاله_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *لوګاریتم;

    فراغ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    فراغ                 *handler_conf;
};


تعريفنوع حرف *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    فراغ *معلومات, فراغ *conf);

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
    ngx_int_t                 ټیټ;
    ngx_int_t                 لوړ;
} ngx_conf_عدد_bounds_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                ارزښت;
} ngx_conf_enum_t;


#تعریف_کړه NGX_CONF_BITMASK_سېټ  1

تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                ماسک;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);
حرف *ngx_conf_check_عدد_bounds(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);
حرف *ngx_conf_check_اندازه_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);


خارجي ngx_conf_post_t  ngx_conf_اندازه_nonzero_post;


#تعریف_کړه ngx_get_conf(conf_ctx, module)  conf_ctx[module.شاخص]



#تعریف_کړه ngx_conf_init_ارزښت(conf, افتراضي)                                   \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = افتراضي;                                                      \
    }

#تعریف_کړه ngx_conf_init_ptr_ارزښت(conf, افتراضي)                               \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = افتراضي;                                                      \
    }

#تعریف_کړه ngx_conf_init_uint_ارزښت(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کړه ngx_conf_init_اندازه_ارزښت(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_اندازه) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کړه ngx_conf_init_msec_ارزښت(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = افتراضي;                                                      \
    }

#تعریف_کړه ngx_conf_یوځای_ارزښت(conf, prev, افتراضي)                            \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کړه ngx_conf_یوځای_ptr_ارزښت(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? افتراضي : prev;                \
    }

#تعریف_کړه ngx_conf_یوځای_uint_ارزښت(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? افتراضي : prev;               \
    }

#تعریف_کړه ngx_conf_یوځای_msec_ارزښت(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? افتراضي : prev;               \
    }

#تعریف_کړه ngx_conf_یوځای_sec_ارزښت(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کړه ngx_conf_یوځای_اندازه_ارزښت(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_اندازه) {                                       \
        conf = (prev == NGX_CONF_UNSET_اندازه) ? افتراضي : prev;               \
    }

#تعریف_کړه ngx_conf_یوځای_off_ارزښت(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#تعریف_کړه ngx_conf_یوځای_str_ارزښت(conf, prev, افتراضي)                        \
    إذا (conf.معلومات == عدم) {                                                 \
        إذا (prev.معلومات) {                                                     \
            conf.len = prev.len;                                             \
            conf.معلومات = prev.معلومات;                                           \
        } وإلا {                                                             \
            conf.len = حجم(افتراضي) - 1;                                  \
            conf.معلومات = (u_char *) افتراضي;                                  \
        }                                                                    \
    }

#تعریف_کړه ngx_conf_یوځای_bufs_ارزښت(conf, prev, default_عدد, default_اندازه)     \
    إذا (conf.عدد == 0) {                                                     \
        إذا (prev.عدد) {                                                      \
            conf.عدد = prev.عدد;                                             \
            conf.اندازه = prev.اندازه;                                           \
        } وإلا {                                                             \
            conf.عدد = default_عدد;                                          \
            conf.اندازه = default_اندازه;                                        \
        }                                                                    \
    }

#تعریف_کړه ngx_conf_یوځای_bitmask_ارزښت(conf, prev, افتراضي)                    \
    إذا (conf == 0) {                                                         \
        conf = (prev == 0) ? افتراضي : prev;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


ngx_int_t ngx_conf_ډک_name(ngx_دوره_t *دوره, ngx_str_t *name,
    ngx_uint_t conf_مختاړی);
ngx_open_file_t *ngx_conf_open_file(ngx_دوره_t *دوره, ngx_str_t *name);
فراغ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_سېټ_flag_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_str_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_str_array_خونه(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
حرف *ngx_conf_سېټ_keyval_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_عدد_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_اندازه_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_off_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_msec_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_sec_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_bufs_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_enum_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سېټ_bitmask_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


#د_که_پای /* _NGX_CONF_FILE_H_INCLUDED_ */
