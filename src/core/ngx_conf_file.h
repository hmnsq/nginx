
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
#define NGX_CONF_UNSET_আকার  (আকার_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          ফাঁকা
#define NGX_CONF_ERROR       (শূন্য *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


কাঠামো ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    অক্ষর               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    শূন্য                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, ফাঁকা, 0, 0, ফাঁকা }


কাঠামো ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    শূন্য                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    শূন্য                 *তথ্য;
};


প্রকারলেখ কাঠামো {
    ngx_file_t            file;
    ngx_buf_t            *বাফার;
    ngx_buf_t            *dump;
    ngx_uint_t            সারি;
} ngx_conf_file_t;


প্রকারলেখ কাঠামো {
    ngx_str_t             name;
    ngx_buf_t            *বাফার;
} ngx_conf_dump_t;


প্রকারলেখ অক্ষর *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, শূন্য *conf);


কাঠামো ngx_conf_s {
    অক্ষর                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *অস্থায়ী_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    শূন্য                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    শূন্য                 *handler_conf;
};


প্রকারলেখ অক্ষর *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    শূন্য *তথ্য, শূন্য *conf);

প্রকারলেখ কাঠামো {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


প্রকারলেখ কাঠামো {
    ngx_conf_post_handler_pt  post_handler;
    অক্ষর                     *old_name;
    অক্ষর                     *new_name;
} ngx_conf_deprecated_t;


প্রকারলেখ কাঠামো {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 নিচের;
    ngx_int_t                 উপরের;
} ngx_conf_অঙ্ক_bounds_t;


প্রকারলেখ কাঠামো {
    ngx_str_t                 name;
    ngx_uint_t                মান;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

প্রকারলেখ কাঠামো {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



অক্ষর * ngx_conf_deprecated(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য);
অক্ষর *ngx_conf_check_অঙ্ক_bounds(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য);
অক্ষর *ngx_conf_check_আকার_nonzero(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য);


বাহ্যিক ngx_conf_post_t  ngx_conf_আকার_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.সূচক]



#define ngx_conf_init_মান(conf, পূর্বনির্ধারিত)                                   \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = পূর্বনির্ধারিত;                                                      \
    }

#define ngx_conf_init_ptr_মান(conf, পূর্বনির্ধারিত)                               \
    যদি (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = পূর্বনির্ধারিত;                                                      \
    }

#define ngx_conf_init_uint_মান(conf, পূর্বনির্ধারিত)                              \
    যদি (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = পূর্বনির্ধারিত;                                                      \
    }

#define ngx_conf_init_আকার_মান(conf, পূর্বনির্ধারিত)                              \
    যদি (conf == NGX_CONF_UNSET_আকার) {                                       \
        conf = পূর্বনির্ধারিত;                                                      \
    }

#define ngx_conf_init_msec_মান(conf, পূর্বনির্ধারিত)                              \
    যদি (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = পূর্বনির্ধারিত;                                                      \
    }

#define ngx_conf_merge_মান(conf, পূর্ব, পূর্বনির্ধারিত)                            \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (পূর্ব == NGX_CONF_UNSET) ? পূর্বনির্ধারিত : পূর্ব;                    \
    }

#define ngx_conf_merge_ptr_মান(conf, পূর্ব, পূর্বনির্ধারিত)                        \
    যদি (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (পূর্ব == NGX_CONF_UNSET_PTR) ? পূর্বনির্ধারিত : পূর্ব;                \
    }

#define ngx_conf_merge_uint_মান(conf, পূর্ব, পূর্বনির্ধারিত)                       \
    যদি (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (পূর্ব == NGX_CONF_UNSET_UINT) ? পূর্বনির্ধারিত : পূর্ব;               \
    }

#define ngx_conf_merge_msec_মান(conf, পূর্ব, পূর্বনির্ধারিত)                       \
    যদি (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (পূর্ব == NGX_CONF_UNSET_MSEC) ? পূর্বনির্ধারিত : পূর্ব;               \
    }

#define ngx_conf_merge_sec_মান(conf, পূর্ব, পূর্বনির্ধারিত)                        \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (পূর্ব == NGX_CONF_UNSET) ? পূর্বনির্ধারিত : পূর্ব;                    \
    }

#define ngx_conf_merge_আকার_মান(conf, পূর্ব, পূর্বনির্ধারিত)                       \
    যদি (conf == NGX_CONF_UNSET_আকার) {                                       \
        conf = (পূর্ব == NGX_CONF_UNSET_আকার) ? পূর্বনির্ধারিত : পূর্ব;               \
    }

#define ngx_conf_merge_off_মান(conf, পূর্ব, পূর্বনির্ধারিত)                        \
    যদি (conf == NGX_CONF_UNSET) {                                            \
        conf = (পূর্ব == NGX_CONF_UNSET) ? পূর্বনির্ধারিত : পূর্ব;                    \
    }

#define ngx_conf_merge_str_মান(conf, পূর্ব, পূর্বনির্ধারিত)                        \
    যদি (conf.তথ্য == ফাঁকা) {                                                 \
        যদি (পূর্ব.তথ্য) {                                                     \
            conf.len = পূর্ব.len;                                             \
            conf.তথ্য = পূর্ব.তথ্য;                                           \
        } অন্যথা {                                                             \
            conf.len = আকার(পূর্বনির্ধারিত) - 1;                                  \
            conf.তথ্য = (u_char *) পূর্বনির্ধারিত;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_মান(conf, পূর্ব, default_অঙ্ক, default_আকার)     \
    যদি (conf.অঙ্ক == 0) {                                                     \
        যদি (পূর্ব.অঙ্ক) {                                                      \
            conf.অঙ্ক = পূর্ব.অঙ্ক;                                             \
            conf.আকার_2 = পূর্ব.আকার_2;                                           \
        } অন্যথা {                                                             \
            conf.অঙ্ক = default_অঙ্ক;                                          \
            conf.আকার_2 = default_আকার;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_মান(conf, পূর্ব, পূর্বনির্ধারিত)                    \
    যদি (conf == 0) {                                                         \
        conf = (পূর্ব == 0) ? পূর্বনির্ধারিত : পূর্ব;                                 \
    }


অক্ষর *ngx_conf_param(ngx_conf_t *cf);
অক্ষর *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
অক্ষর *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
শূন্য ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ধ্রুবক অক্ষর *fmt, ...);


অক্ষর *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    শূন্য *conf);
অক্ষর *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_অঙ্ক_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_আকার_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
অক্ষর *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
