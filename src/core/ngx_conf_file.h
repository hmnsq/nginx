
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_CONF_FILE_H_INCLUDED_
#وصف_ڪريو _NGX_CONF_FILE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#وصف_ڪريو NGX_CONF_NOARGS      0x00000001
#وصف_ڪريو NGX_CONF_TAKE1       0x00000002
#وصف_ڪريو NGX_CONF_TAKE2       0x00000004
#وصف_ڪريو NGX_CONF_TAKE3       0x00000008
#وصف_ڪريو NGX_CONF_TAKE4       0x00000010
#وصف_ڪريو NGX_CONF_TAKE5       0x00000020
#وصف_ڪريو NGX_CONF_TAKE6       0x00000040
#وصف_ڪريو NGX_CONF_TAKE7       0x00000080

#وصف_ڪريو NGX_CONF_MAX_دليلون    8

#وصف_ڪريو NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#وصف_ڪريو NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#وصف_ڪريو NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#وصف_ڪريو NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#وصف_ڪريو NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#وصف_ڪريو NGX_CONF_دليلون_عدد 0x000000ff
#وصف_ڪريو NGX_CONF_بلاڪ       0x00000100
#وصف_ڪريو NGX_CONF_FLAG        0x00000200
#وصف_ڪريو NGX_CONF_ANY         0x00000400
#وصف_ڪريو NGX_CONF_1_وڌيڪ       0x00000800
#وصف_ڪريو NGX_CONF_2_وڌيڪ       0x00001000

#وصف_ڪريو NGX_سڌو_CONF      0x00010000

#وصف_ڪريو NGX_MAIN_CONF        0x01000000
#وصف_ڪريو NGX_ANY_CONF         0xFF000000



#وصف_ڪريو NGX_CONF_UNSET       -1
#وصف_ڪريو NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#وصف_ڪريو NGX_CONF_UNSET_PTR   (فراغ *) -1
#وصف_ڪريو NGX_CONF_UNSET_سائيز  (سائيز_t) -1
#وصف_ڪريو NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#وصف_ڪريو NGX_CONF_OK          عدم
#وصف_ڪريو NGX_CONF_ERROR       (فراغ *) -1

#وصف_ڪريو NGX_CONF_بلاڪ_شروعات 1
#وصف_ڪريو NGX_CONF_بلاڪ_پورو  2
#وصف_ڪريو NGX_CONF_FILE_پورو   3

#وصف_ڪريو NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#وصف_ڪريو NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#وصف_ڪريو NGX_MAX_CONF_ERRSTR  1024


بنية ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    حرف               *(*سيٽ)(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            وٿي;
    فراغ                 *post;
};

#وصف_ڪريو ngx_null_command  { ngx_null_string, 0, عدم, 0, 0, عدم }


بنية ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    فراغ                (*flush)(ngx_open_file_t *file, ngx_log_t *لاگرٿم);
    فراغ                 *معلومات;
};


تعريفنوع بنية {
    ngx_file_t            file;
    ngx_buf_t            *بفر;
    ngx_buf_t            *dump;
    ngx_uint_t            سٽ;
} ngx_conf_file_t;


تعريفنوع بنية {
    ngx_str_t             name;
    ngx_buf_t            *بفر;
} ngx_conf_dump_t;


تعريفنوع حرف *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, فراغ *conf);


بنية ngx_conf_s {
    حرف                 *name;
    ngx_array_t          *دليلون;

    ngx_چڪر_t          *چڪر;
    ngx_pool_t           *pool;
    ngx_pool_t           *عارضي_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *لاگرٿم;

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
    ngx_int_t                 هيٺيون;
    ngx_int_t                 مٿاهون;
} ngx_conf_انگ_bounds_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                قدر;
} ngx_conf_enum_t;


#وصف_ڪريو NGX_CONF_BITMASK_سيٽ  1

تعريفنوع بنية {
    ngx_str_t                 name;
    ngx_uint_t                نقاب;
} ngx_conf_bitmask_t;



حرف * ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);
حرف *ngx_conf_check_انگ_bounds(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);
حرف *ngx_conf_check_سائيز_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *معلومات);


خارجي ngx_conf_post_t  ngx_conf_سائيز_nonzero_post;


#وصف_ڪريو ngx_get_conf(conf_ctx, module)  conf_ctx[module.اشاريو]



#وصف_ڪريو ngx_conf_init_قدر(conf, افتراضي)                                   \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = افتراضي;                                                      \
    }

#وصف_ڪريو ngx_conf_init_ptr_قدر(conf, افتراضي)                               \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = افتراضي;                                                      \
    }

#وصف_ڪريو ngx_conf_init_uint_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = افتراضي;                                                      \
    }

#وصف_ڪريو ngx_conf_init_سائيز_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_سائيز) {                                       \
        conf = افتراضي;                                                      \
    }

#وصف_ڪريو ngx_conf_init_msec_قدر(conf, افتراضي)                              \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = افتراضي;                                                      \
    }

#وصف_ڪريو ngx_conf_ملايو_قدر(conf, prev, افتراضي)                            \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#وصف_ڪريو ngx_conf_ملايو_ptr_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? افتراضي : prev;                \
    }

#وصف_ڪريو ngx_conf_ملايو_uint_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? افتراضي : prev;               \
    }

#وصف_ڪريو ngx_conf_ملايو_msec_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? افتراضي : prev;               \
    }

#وصف_ڪريو ngx_conf_ملايو_sec_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#وصف_ڪريو ngx_conf_ملايو_سائيز_قدر(conf, prev, افتراضي)                       \
    إذا (conf == NGX_CONF_UNSET_سائيز) {                                       \
        conf = (prev == NGX_CONF_UNSET_سائيز) ? افتراضي : prev;               \
    }

#وصف_ڪريو ngx_conf_ملايو_off_قدر(conf, prev, افتراضي)                        \
    إذا (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? افتراضي : prev;                    \
    }

#وصف_ڪريو ngx_conf_ملايو_str_قدر(conf, prev, افتراضي)                        \
    إذا (conf.معلومات == عدم) {                                                 \
        إذا (prev.معلومات) {                                                     \
            conf.len = prev.len;                                             \
            conf.معلومات = prev.معلومات;                                           \
        } وإلا {                                                             \
            conf.len = حجم(افتراضي) - 1;                                  \
            conf.معلومات = (u_char *) افتراضي;                                  \
        }                                                                    \
    }

#وصف_ڪريو ngx_conf_ملايو_bufs_قدر(conf, prev, default_انگ, default_سائيز)     \
    إذا (conf.انگ == 0) {                                                     \
        إذا (prev.انگ) {                                                      \
            conf.انگ = prev.انگ;                                             \
            conf.سائيز = prev.سائيز;                                           \
        } وإلا {                                                             \
            conf.انگ = default_انگ;                                          \
            conf.سائيز = default_سائيز;                                        \
        }                                                                    \
    }

#وصف_ڪريو ngx_conf_ملايو_bitmask_قدر(conf, prev, افتراضي)                    \
    إذا (conf == 0) {                                                         \
        conf = (prev == 0) ? افتراضي : prev;                                 \
    }


حرف *ngx_conf_param(ngx_conf_t *cf);
حرف *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
حرف *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


ngx_int_t ngx_conf_ڀريل_name(ngx_چڪر_t *چڪر, ngx_str_t *name,
    ngx_uint_t conf_اڳياڙي);
ngx_open_file_t *ngx_conf_open_file(ngx_چڪر_t *چڪر, ngx_str_t *name);
فراغ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ثابت حرف *fmt, ...);


حرف *ngx_conf_سيٽ_flag_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_str_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_str_array_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
حرف *ngx_conf_سيٽ_keyval_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_انگ_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_سائيز_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_off_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_msec_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_sec_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_bufs_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_enum_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
حرف *ngx_conf_سيٽ_bitmask_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);


#جيڪڏهن_پورو /* _NGX_CONF_FILE_H_INCLUDED_ */
