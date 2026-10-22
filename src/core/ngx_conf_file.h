
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_CONF_FILE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_CONF_FILE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_NOARGS      0x00000001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE1       0x00000002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE2       0x00000004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE3       0x00000008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE4       0x00000010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE5       0x00000020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE6       0x00000040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE7       0x00000080

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_MAX_ਦਲੀਲਾਂ    8

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ਦਲੀਲਾਂ_ਸੰਖਿਆ 0x000000ff
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ਖੰਡ       0x00000100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_FLAG        0x00000200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ANY         0x00000400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_1_ਹੋਰ       0x00000800
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_2_ਹੋਰ       0x00001000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਸਿੱਧਾ_CONF      0x00010000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAIN_CONF        0x01000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ANY_CONF         0xFF000000



#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_UNSET       -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_UNSET_PTR   (ਖਾਲੀ *) -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_UNSET_ਮਾਪ  (ਮਾਪ_t) -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_OK          ਨਿਰਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ERROR       (ਖਾਲੀ *) -1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ਖੰਡ_ਪੂਰਾ  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_FILE_ਪੂਰਾ   3

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_CONF_ERRSTR  1024


ਬਣਤਰ ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    ਅੱਖਰ               *(*ਸੈੱਟ)(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            ਵਿੱਥ;
    ਖਾਲੀ                 *post;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_null_command  { ngx_null_string, 0, ਨਿਰਾ, 0, 0, ਨਿਰਾ }


ਬਣਤਰ ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    ਖਾਲੀ                (*flush)(ngx_open_file_t *file, ngx_log_t *ਲਘੂਗਣਕ);
    ਖਾਲੀ                 *ਅੰਕੜੇ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_file_t            file;
    ngx_buf_t            *ਬਫਰ;
    ngx_buf_t            *dump;
    ngx_uint_t            ਸਤਰ;
} ngx_conf_file_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t             name;
    ngx_buf_t            *ਬਫਰ;
} ngx_conf_dump_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, ਖਾਲੀ *conf);


ਬਣਤਰ ngx_conf_s {
    ਅੱਖਰ                 *name;
    ngx_array_t          *ਦਲੀਲਾਂ;

    ngx_ਚੱਕਰ_t          *ਚੱਕਰ;
    ngx_pool_t           *pool;
    ngx_pool_t           *ਆਰਜ਼ੀ_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *ਲਘੂਗਣਕ;

    ਖਾਲੀ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    ਖਾਲੀ                 *handler_conf;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    ਖਾਲੀ *ਅੰਕੜੇ, ਖਾਲੀ *conf);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_conf_post_handler_pt  post_handler;
    ਅੱਖਰ                     *old_name;
    ਅੱਖਰ                     *new_name;
} ngx_conf_deprecated_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 ਨੀਵਾਂ;
    ngx_int_t                 ਉੱਚਾ;
} ngx_conf_ਅੰਕ_bounds_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                 name;
    ngx_uint_t                ਮੁੱਲ;
} ngx_conf_enum_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_BITMASK_ਸੈੱਟ  1

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                 name;
    ngx_uint_t                ਨਕਾਬ;
} ngx_conf_bitmask_t;



ਅੱਖਰ * ngx_conf_deprecated(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ);
ਅੱਖਰ *ngx_conf_check_ਅੰਕ_bounds(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ);
ਅੱਖਰ *ngx_conf_check_ਮਾਪ_nonzero(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ);


ਬਾਹਰੀ ngx_conf_post_t  ngx_conf_ਮਾਪ_nonzero_post;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_get_conf(conf_ctx, module)  conf_ctx[module.ਸੂਚਕਾਂਕ]



#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_init_ਮੁੱਲ(conf, ਮੂਲ)                                   \
    ਜੇ (conf == NGX_CONF_UNSET) {                                            \
        conf = ਮੂਲ;                                                      \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_init_ptr_ਮੁੱਲ(conf, ਮੂਲ)                               \
    ਜੇ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = ਮੂਲ;                                                      \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_init_uint_ਮੁੱਲ(conf, ਮੂਲ)                              \
    ਜੇ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = ਮੂਲ;                                                      \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_init_ਮਾਪ_ਮੁੱਲ(conf, ਮੂਲ)                              \
    ਜੇ (conf == NGX_CONF_UNSET_ਮਾਪ) {                                       \
        conf = ਮੂਲ;                                                      \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_init_msec_ਮੁੱਲ(conf, ਮੂਲ)                              \
    ਜੇ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = ਮੂਲ;                                                      \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_ਮੁੱਲ(conf, prev, ਮੂਲ)                            \
    ਜੇ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ਮੂਲ : prev;                    \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_ptr_ਮੁੱਲ(conf, prev, ਮੂਲ)                        \
    ਜੇ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? ਮੂਲ : prev;                \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_uint_ਮੁੱਲ(conf, prev, ਮੂਲ)                       \
    ਜੇ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? ਮੂਲ : prev;               \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_msec_ਮੁੱਲ(conf, prev, ਮੂਲ)                       \
    ਜੇ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? ਮੂਲ : prev;               \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_sec_ਮੁੱਲ(conf, prev, ਮੂਲ)                        \
    ਜੇ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ਮੂਲ : prev;                    \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_ਮਾਪ_ਮੁੱਲ(conf, prev, ਮੂਲ)                       \
    ਜੇ (conf == NGX_CONF_UNSET_ਮਾਪ) {                                       \
        conf = (prev == NGX_CONF_UNSET_ਮਾਪ) ? ਮੂਲ : prev;               \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_off_ਮੁੱਲ(conf, prev, ਮੂਲ)                        \
    ਜੇ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ਮੂਲ : prev;                    \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_str_ਮੁੱਲ(conf, prev, ਮੂਲ)                        \
    ਜੇ (conf.ਅੰਕੜੇ == ਨਿਰਾ) {                                                 \
        ਜੇ (prev.ਅੰਕੜੇ) {                                                     \
            conf.len = prev.len;                                             \
            conf.ਅੰਕੜੇ = prev.ਅੰਕੜੇ;                                           \
        } ਨਹੀਂਤਾਂ {                                                             \
            conf.len = ਆਕਾਰ(ਮੂਲ) - 1;                                  \
            conf.ਅੰਕੜੇ = (u_char *) ਮੂਲ;                                  \
        }                                                                    \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_bufs_ਮੁੱਲ(conf, prev, default_ਅੰਕ, default_ਮਾਪ)     \
    ਜੇ (conf.ਅੰਕ == 0) {                                                     \
        ਜੇ (prev.ਅੰਕ) {                                                      \
            conf.ਅੰਕ = prev.ਅੰਕ;                                             \
            conf.ਮਾਪ = prev.ਮਾਪ;                                           \
        } ਨਹੀਂਤਾਂ {                                                             \
            conf.ਅੰਕ = default_ਅੰਕ;                                          \
            conf.ਮਾਪ = default_ਮਾਪ;                                        \
        }                                                                    \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_conf_ਮਿਲਾਓ_bitmask_ਮੁੱਲ(conf, prev, ਮੂਲ)                    \
    ਜੇ (conf == 0) {                                                         \
        conf = (prev == 0) ? ਮੂਲ : prev;                                 \
    }


ਅੱਖਰ *ngx_conf_param(ngx_conf_t *cf);
ਅੱਖਰ *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
ਅੱਖਰ *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);


ngx_int_t ngx_conf_ਭਰਿਆ_name(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name,
    ngx_uint_t conf_ਅਗੇਤਰ);
ngx_open_file_t *ngx_conf_open_file(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name);
ਖਾਲੀ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...);


ਅੱਖਰ *ngx_conf_ਸੈੱਟ_flag_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_str_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_str_array_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd,
    ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_keyval_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_ਅੰਕ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_ਮਾਪ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_off_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_msec_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_sec_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_bufs_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_enum_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਅੱਖਰ *ngx_conf_ਸੈੱਟ_bitmask_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);


#ਜੇ_ਅੰਤ /* _NGX_CONF_FILE_H_INCLUDED_ */
