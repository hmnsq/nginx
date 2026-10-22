
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


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

#define NGX_CONF_ARGS_ಸಂಖ್ಯೆ 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (ಶೂನ್ಯ *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          ರಿಕ್ತ
#define NGX_CONF_ERROR       (ಶೂನ್ಯ *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


ಸಂರಚನಾ ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    ವರ್ಣ               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    ಶೂನ್ಯ                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, ರಿಕ್ತ, 0, 0, ರಿಕ್ತ }


ಸಂರಚನಾ ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    ಶೂನ್ಯ                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    ಶೂನ್ಯ                 *data;
};


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            ಸಾಲು;
} ngx_conf_file_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ವರ್ಣ *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, ಶೂನ್ಯ *conf);


ಸಂರಚನಾ ngx_conf_s {
    ವರ್ಣ                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    ಶೂನ್ಯ                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    ಶೂನ್ಯ                 *handler_conf;
};


ಪ್ರಕಾರನಿರ್ದೇಶ ವರ್ಣ *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    ಶೂನ್ಯ *data, ಶೂನ್ಯ *conf);

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_conf_post_handler_pt  post_handler;
    ವರ್ಣ                     *old_name;
    ವರ್ಣ                     *new_name;
} ngx_conf_deprecated_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 ಕೆಳಗಿನ;
    ngx_int_t                 ಮೇಲಿನ;
} ngx_conf_ಅಂಕೆ_bounds_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                 name;
    ngx_uint_t                ಮೌಲ್ಯ;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



ವರ್ಣ * ngx_conf_deprecated(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data);
ವರ್ಣ *ngx_conf_check_ಅಂಕೆ_bounds(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data);
ವರ್ಣ *ngx_conf_check_size_nonzero(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data);


ಬಾಹ್ಯ ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_ಮೌಲ್ಯ(conf, ಸ್ವಭಾವ)                                   \
    ಯದಿ (conf == NGX_CONF_UNSET) {                                            \
        conf = ಸ್ವಭಾವ;                                                      \
    }

#define ngx_conf_init_ptr_ಮೌಲ್ಯ(conf, ಸ್ವಭಾವ)                               \
    ಯದಿ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = ಸ್ವಭಾವ;                                                      \
    }

#define ngx_conf_init_uint_ಮೌಲ್ಯ(conf, ಸ್ವಭಾವ)                              \
    ಯದಿ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = ಸ್ವಭಾವ;                                                      \
    }

#define ngx_conf_init_size_ಮೌಲ್ಯ(conf, ಸ್ವಭಾವ)                              \
    ಯದಿ (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = ಸ್ವಭಾವ;                                                      \
    }

#define ngx_conf_init_msec_ಮೌಲ್ಯ(conf, ಸ್ವಭಾವ)                              \
    ಯದಿ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = ಸ್ವಭಾವ;                                                      \
    }

#define ngx_conf_merge_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                            \
    ಯದಿ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಸ್ವಭಾವ : prev;                    \
    }

#define ngx_conf_merge_ptr_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                        \
    ಯದಿ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? ಸ್ವಭಾವ : prev;                \
    }

#define ngx_conf_merge_uint_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                       \
    ಯದಿ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? ಸ್ವಭಾವ : prev;               \
    }

#define ngx_conf_merge_msec_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                       \
    ಯದಿ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? ಸ್ವಭಾವ : prev;               \
    }

#define ngx_conf_merge_sec_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                        \
    ಯದಿ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಸ್ವಭಾವ : prev;                    \
    }

#define ngx_conf_merge_size_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                       \
    ಯದಿ (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? ಸ್ವಭಾವ : prev;               \
    }

#define ngx_conf_merge_off_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                        \
    ಯದಿ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? ಸ್ವಭಾವ : prev;                    \
    }

#define ngx_conf_merge_str_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                        \
    ಯದಿ (conf.data == ರಿಕ್ತ) {                                                 \
        ಯದಿ (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } ಅನ್ಯಥಾ {                                                             \
            conf.len = ಪರಿಮಾಣ(ಸ್ವಭಾವ) - 1;                                  \
            conf.data = (u_char *) ಸ್ವಭಾವ;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_ಮೌಲ್ಯ(conf, prev, default_ಅಂಕೆ, default_size)     \
    ಯದಿ (conf.ಅಂಕೆ == 0) {                                                     \
        ಯದಿ (prev.ಅಂಕೆ) {                                                      \
            conf.ಅಂಕೆ = prev.ಅಂಕೆ;                                             \
            conf.size = prev.size;                                           \
        } ಅನ್ಯಥಾ {                                                             \
            conf.ಅಂಕೆ = default_ಅಂಕೆ;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_ಮೌಲ್ಯ(conf, prev, ಸ್ವಭಾವ)                    \
    ಯದಿ (conf == 0) {                                                         \
        conf = (prev == 0) ? ಸ್ವಭಾವ : prev;                                 \
    }


ವರ್ಣ *ngx_conf_param(ngx_conf_t *cf);
ವರ್ಣ *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
ವರ್ಣ *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
ಶೂನ್ಯ ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ಧ್ರುವ ವರ್ಣ *fmt, ...);


ವರ್ಣ *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_ಅಂಕೆ_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ವರ್ಣ *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
