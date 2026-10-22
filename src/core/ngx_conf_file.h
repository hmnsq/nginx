
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


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

#define NGX_CONF_ARGS_সংখ্যা 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (শূন্য *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          রিক্ত
#define NGX_CONF_ERROR       (শূন্য *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


সংরচনা ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    বর্ণ               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    শূন্য                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, রিক্ত, 0, 0, রিক্ত }


সংরচনা ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    শূন্য                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    শূন্য                 *data;
};


প্রকারনির্দেশ সংরচনা {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            সারি;
} ngx_conf_file_t;


প্রকারনির্দেশ সংরচনা {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


প্রকারনির্দেশ বর্ণ *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, শূন্য *conf);


সংরচনা ngx_conf_s {
    বর্ণ                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    শূন্য                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    শূন্য                 *handler_conf;
};


প্রকারনির্দেশ বর্ণ *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    শূন্য *data, শূন্য *conf);

প্রকারনির্দেশ সংরচনা {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


প্রকারনির্দেশ সংরচনা {
    ngx_conf_post_handler_pt  post_handler;
    বর্ণ                     *old_name;
    বর্ণ                     *new_name;
} ngx_conf_deprecated_t;


প্রকারনির্দেশ সংরচনা {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 নিচের;
    ngx_int_t                 উপরের;
} ngx_conf_অঙ্ক_bounds_t;


প্রকারনির্দেশ সংরচনা {
    ngx_str_t                 name;
    ngx_uint_t                মান;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

প্রকারনির্দেশ সংরচনা {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



বর্ণ * ngx_conf_deprecated(ngx_conf_t *cf, শূন্য *post, শূন্য *data);
বর্ণ *ngx_conf_check_অঙ্ক_bounds(ngx_conf_t *cf, শূন্য *post, শূন্য *data);
বর্ণ *ngx_conf_check_size_nonzero(ngx_conf_t *cf, শূন্য *post, শূন্য *data);


বাহ্য ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_মান(conf, স্বভাব)                                   \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = স্বভাব;                                                      \
    }

#define ngx_conf_init_ptr_মান(conf, স্বভাব)                               \
    যদি (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = স্বভাব;                                                      \
    }

#define ngx_conf_init_uint_মান(conf, স্বভাব)                              \
    যদি (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = স্বভাব;                                                      \
    }

#define ngx_conf_init_size_মান(conf, স্বভাব)                              \
    যদি (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = স্বভাব;                                                      \
    }

#define ngx_conf_init_msec_মান(conf, স্বভাব)                              \
    যদি (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = স্বভাব;                                                      \
    }

#define ngx_conf_merge_মান(conf, prev, স্বভাব)                            \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? স্বভাব : prev;                    \
    }

#define ngx_conf_merge_ptr_মান(conf, prev, স্বভাব)                        \
    যদি (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? স্বভাব : prev;                \
    }

#define ngx_conf_merge_uint_মান(conf, prev, স্বভাব)                       \
    যদি (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? স্বভাব : prev;               \
    }

#define ngx_conf_merge_msec_মান(conf, prev, স্বভাব)                       \
    যদি (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? স্বভাব : prev;               \
    }

#define ngx_conf_merge_sec_মান(conf, prev, স্বভাব)                        \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? স্বভাব : prev;                    \
    }

#define ngx_conf_merge_size_মান(conf, prev, স্বভাব)                       \
    যদি (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? স্বভাব : prev;               \
    }

#define ngx_conf_merge_off_মান(conf, prev, স্বভাব)                        \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? স্বভাব : prev;                    \
    }

#define ngx_conf_merge_str_মান(conf, prev, স্বভাব)                        \
    যদি (conf.data == রিক্ত) {                                                 \
        যদি (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } অন্যথা {                                                             \
            conf.len = পরিমাণ(স্বভাব) - 1;                                  \
            conf.data = (u_char *) স্বভাব;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_মান(conf, prev, default_অঙ্ক, default_size)     \
    যদি (conf.অঙ্ক == 0) {                                                     \
        যদি (prev.অঙ্ক) {                                                      \
            conf.অঙ্ক = prev.অঙ্ক;                                             \
            conf.size = prev.size;                                           \
        } অন্যথা {                                                             \
            conf.অঙ্ক = default_অঙ্ক;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_মান(conf, prev, স্বভাব)                    \
    যদি (conf == 0) {                                                         \
        conf = (prev == 0) ? স্বভাব : prev;                                 \
    }


বর্ণ *ngx_conf_param(ngx_conf_t *cf);
বর্ণ *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
বর্ণ *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
শূন্য ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ধ্রুব বর্ণ *fmt, ...);


বর্ণ *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    শূন্য *conf);
বর্ণ *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_অঙ্ক_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
বর্ণ *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
