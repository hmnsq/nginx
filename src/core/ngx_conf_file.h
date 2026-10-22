
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_CONF_FILE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_CONF_FILE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_NOARGS      0x00000001
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE1       0x00000002
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE2       0x00000004
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE3       0x00000008
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE4       0x00000010
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE5       0x00000020
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE6       0x00000040
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE7       0x00000080

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_MAX_ವಾದಗಳು    8

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ವಾದಗಳು_ಸಂಖ್ಯೆ 0x000000ff
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ಖಂಡ       0x00000100
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_FLAG        0x00000200
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ANY         0x00000400
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_1_ಇನ್ನಷ್ಟು       0x00000800
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_2_ಇನ್ನಷ್ಟು       0x00001000

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ನೇರ_CONF      0x00010000

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAIN_CONF        0x01000000
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ANY_CONF         0xFF000000



#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_UNSET       -1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_UNSET_PTR   (ಶೂನ್ಯ *) -1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_UNSET_ಗಾತ್ರ  (ಗಾತ್ರ_t) -1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_OK          ಖಾಲಿ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ERROR       (ಶೂನ್ಯ *) -1

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ಖಂಡ_ಆರಂಭ 1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ಖಂಡ_ಮುಗಿದ  2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_FILE_ಮುಗಿದ   3

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_CONF_ERRSTR  1024


ರಚನೆ ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    ಅಕ್ಷರ               *(*ಗಣ)(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            ಸ್ಥಾನಾಂತರ;
    ಶೂನ್ಯ                 *post;
};

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_null_command  { ngx_null_string, 0, ಖಾಲಿ, 0, 0, ಖಾಲಿ }


ರಚನೆ ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    ಶೂನ್ಯ                (*flush)(ngx_open_file_t *file, ngx_log_t *ಲಘುಗಣಕ);
    ಶೂನ್ಯ                 *ದತ್ತಾಂಶ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_file_t            file;
    ngx_buf_t            *ಬಫರ್;
    ngx_buf_t            *dump;
    ngx_uint_t            ಸಾಲು;
} ngx_conf_file_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t             name;
    ngx_buf_t            *ಬಫರ್;
} ngx_conf_dump_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, ಶೂನ್ಯ *conf);


ರಚನೆ ngx_conf_s {
    ಅಕ್ಷರ                 *name;
    ngx_array_t          *ವಾದಗಳು;

    ngx_ಚಕ್ರ_t          *ಚಕ್ರ;
    ngx_pool_t           *pool;
    ngx_pool_t           *ತಾತ್ಕಾಲಿಕ_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *ಲಘುಗಣಕ;

    ಶೂನ್ಯ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    ಶೂನ್ಯ                 *handler_conf;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಶೂನ್ಯ *conf);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_conf_post_handler_pt  post_handler;
    ಅಕ್ಷರ                     *old_name;
    ಅಕ್ಷರ                     *new_name;
} ngx_conf_deprecated_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 ಕೆಳಗಿನ;
    ngx_int_t                 ಮೇಲಿನ;
} ngx_conf_ಅಂಕೆ_bounds_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                 name;
    ngx_uint_t                ಮೌಲ್ಯ;
} ngx_conf_enum_t;


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_BITMASK_ಗಣ  1

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                 name;
    ngx_uint_t                ಮುಖವಾಡ;
} ngx_conf_bitmask_t;



ಅಕ್ಷರ * ngx_conf_deprecated(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಅಕ್ಷರ *ngx_conf_check_ಅಂಕೆ_bounds(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಅಕ್ಷರ *ngx_conf_check_ಗಾತ್ರ_nonzero(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ);


ಬಾಹ್ಯ ngx_conf_post_t  ngx_conf_ಗಾತ್ರ_nonzero_post;


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_get_conf(conf_ctx, module)  conf_ctx[module.ಸೂಚ್ಯಂಕ]



#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_init_ಮೌಲ್ಯ(conf, ಪೂರ್ವನಿಯೋಜಿತ)                                   \
    ಆದರೆ (conf == NGX_CONF_UNSET) {                                            \
        conf = ಪೂರ್ವನಿಯೋಜಿತ;                                                      \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_init_ptr_ಮೌಲ್ಯ(conf, ಪೂರ್ವನಿಯೋಜಿತ)                               \
    ಆದರೆ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = ಪೂರ್ವನಿಯೋಜಿತ;                                                      \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_init_uint_ಮೌಲ್ಯ(conf, ಪೂರ್ವನಿಯೋಜಿತ)                              \
    ಆದರೆ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = ಪೂರ್ವನಿಯೋಜಿತ;                                                      \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_init_ಗಾತ್ರ_ಮೌಲ್ಯ(conf, ಪೂರ್ವನಿಯೋಜಿತ)                              \
    ಆದರೆ (conf == NGX_CONF_UNSET_ಗಾತ್ರ) {                                       \
        conf = ಪೂರ್ವನಿಯೋಜಿತ;                                                      \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_init_msec_ಮೌಲ್ಯ(conf, ಪೂರ್ವನಿಯೋಜಿತ)                              \
    ಆದರೆ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = ಪೂರ್ವನಿಯೋಜಿತ;                                                      \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                            \
    ಆದರೆ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;                    \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_ptr_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                        \
    ಆದರೆ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;                \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_uint_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                       \
    ಆದರೆ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;               \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_msec_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                       \
    ಆದರೆ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;               \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_sec_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                        \
    ಆದರೆ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;                    \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_ಗಾತ್ರ_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                       \
    ಆದರೆ (conf == NGX_CONF_UNSET_ಗಾತ್ರ) {                                       \
        conf = (prev == NGX_CONF_UNSET_ಗಾತ್ರ) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;               \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_off_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                        \
    ಆದರೆ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;                    \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_str_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                        \
    ಆದರೆ (conf.ದತ್ತಾಂಶ == ಖಾಲಿ) {                                                 \
        ಆದರೆ (prev.ದತ್ತಾಂಶ) {                                                     \
            conf.len = prev.len;                                             \
            conf.ದತ್ತಾಂಶ = prev.ದತ್ತಾಂಶ;                                           \
        } ಇಲ್ಲದಿದ್ದರೆ {                                                             \
            conf.len = ಗಾತ್ರ(ಪೂರ್ವನಿಯೋಜಿತ) - 1;                                  \
            conf.ದತ್ತಾಂಶ = (u_char *) ಪೂರ್ವನಿಯೋಜಿತ;                                  \
        }                                                                    \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_bufs_ಮೌಲ್ಯ(conf, prev, default_ಅಂಕೆ, default_ಗಾತ್ರ)     \
    ಆದರೆ (conf.ಅಂಕೆ == 0) {                                                     \
        ಆದರೆ (prev.ಅಂಕೆ) {                                                      \
            conf.ಅಂಕೆ = prev.ಅಂಕೆ;                                             \
            conf.ಗಾತ್ರ_2 = prev.ಗಾತ್ರ_2;                                           \
        } ಇಲ್ಲದಿದ್ದರೆ {                                                             \
            conf.ಅಂಕೆ = default_ಅಂಕೆ;                                          \
            conf.ಗಾತ್ರ_2 = default_ಗಾತ್ರ;                                        \
        }                                                                    \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_conf_ವಿಲೀನ_bitmask_ಮೌಲ್ಯ(conf, prev, ಪೂರ್ವನಿಯೋಜಿತ)                    \
    ಆದರೆ (conf == 0) {                                                         \
        conf = (prev == 0) ? ಪೂರ್ವನಿಯೋಜಿತ : prev;                                 \
    }


ಅಕ್ಷರ *ngx_conf_param(ngx_conf_t *cf);
ಅಕ್ಷರ *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
ಅಕ್ಷರ *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


ngx_int_t ngx_conf_ತುಂಬಿದ_name(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name,
    ngx_uint_t conf_ಪೂರ್ವಪ್ರತ್ಯಯ);
ngx_open_file_t *ngx_conf_open_file(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name);
ಶೂನ್ಯ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);


ಅಕ್ಷರ *ngx_conf_ಗಣ_flag_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_str_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_str_array_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd,
    ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_keyval_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_ಅಂಕೆ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_ಗಾತ್ರ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_off_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_msec_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_sec_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_bufs_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_enum_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಅಕ್ಷರ *ngx_conf_ಗಣ_bitmask_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_CONF_FILE_H_INCLUDED_ */
