
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


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

#define NGX_CONF_ARGS_எண் 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (வெறுமை *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          வெற்று
#define NGX_CONF_ERROR       (வெறுமை *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


கட்டமைப்பு ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    எழுத்து               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    வெறுமை                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, வெற்று, 0, 0, வெற்று }


கட்டமைப்பு ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    வெறுமை                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    வெறுமை                 *data;
};


வகைவரை கட்டமைப்பு {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            வரி;
} ngx_conf_file_t;


வகைவரை கட்டமைப்பு {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


வகைவரை எழுத்து *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, வெறுமை *conf);


கட்டமைப்பு ngx_conf_s {
    எழுத்து                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    வெறுமை                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    வெறுமை                 *handler_conf;
};


வகைவரை எழுத்து *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    வெறுமை *data, வெறுமை *conf);

வகைவரை கட்டமைப்பு {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


வகைவரை கட்டமைப்பு {
    ngx_conf_post_handler_pt  post_handler;
    எழுத்து                     *old_name;
    எழுத்து                     *new_name;
} ngx_conf_deprecated_t;


வகைவரை கட்டமைப்பு {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 கீழ்;
    ngx_int_t                 உயர்வு;
} ngx_conf_இலக்கம்_bounds_t;


வகைவரை கட்டமைப்பு {
    ngx_str_t                 name;
    ngx_uint_t                மதிப்பு;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

வகைவரை கட்டமைப்பு {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



எழுத்து * ngx_conf_deprecated(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data);
எழுத்து *ngx_conf_check_இலக்கம்_bounds(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data);
எழுத்து *ngx_conf_check_size_nonzero(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data);


வெளி ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_மதிப்பு(conf, இயல்பு)                                   \
    எனில் (conf == NGX_CONF_UNSET) {                                            \
        conf = இயல்பு;                                                      \
    }

#define ngx_conf_init_ptr_மதிப்பு(conf, இயல்பு)                               \
    எனில் (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = இயல்பு;                                                      \
    }

#define ngx_conf_init_uint_மதிப்பு(conf, இயல்பு)                              \
    எனில் (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = இயல்பு;                                                      \
    }

#define ngx_conf_init_size_மதிப்பு(conf, இயல்பு)                              \
    எனில் (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = இயல்பு;                                                      \
    }

#define ngx_conf_init_msec_மதிப்பு(conf, இயல்பு)                              \
    எனில் (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = இயல்பு;                                                      \
    }

#define ngx_conf_merge_மதிப்பு(conf, prev, இயல்பு)                            \
    எனில் (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? இயல்பு : prev;                    \
    }

#define ngx_conf_merge_ptr_மதிப்பு(conf, prev, இயல்பு)                        \
    எனில் (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? இயல்பு : prev;                \
    }

#define ngx_conf_merge_uint_மதிப்பு(conf, prev, இயல்பு)                       \
    எனில் (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? இயல்பு : prev;               \
    }

#define ngx_conf_merge_msec_மதிப்பு(conf, prev, இயல்பு)                       \
    எனில் (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? இயல்பு : prev;               \
    }

#define ngx_conf_merge_sec_மதிப்பு(conf, prev, இயல்பு)                        \
    எனில் (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? இயல்பு : prev;                    \
    }

#define ngx_conf_merge_size_மதிப்பு(conf, prev, இயல்பு)                       \
    எனில் (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? இயல்பு : prev;               \
    }

#define ngx_conf_merge_off_மதிப்பு(conf, prev, இயல்பு)                        \
    எனில் (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? இயல்பு : prev;                    \
    }

#define ngx_conf_merge_str_மதிப்பு(conf, prev, இயல்பு)                        \
    எனில் (conf.data == வெற்று) {                                                 \
        எனில் (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } இல்லையெனில் {                                                             \
            conf.len = அளவு(இயல்பு) - 1;                                  \
            conf.data = (u_char *) இயல்பு;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_மதிப்பு(conf, prev, default_இலக்கம், default_size)     \
    எனில் (conf.இலக்கம் == 0) {                                                     \
        எனில் (prev.இலக்கம்) {                                                      \
            conf.இலக்கம் = prev.இலக்கம்;                                             \
            conf.size = prev.size;                                           \
        } இல்லையெனில் {                                                             \
            conf.இலக்கம் = default_இலக்கம்;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_மதிப்பு(conf, prev, இயல்பு)                    \
    எனில் (conf == 0) {                                                         \
        conf = (prev == 0) ? இயல்பு : prev;                                 \
    }


எழுத்து *ngx_conf_param(ngx_conf_t *cf);
எழுத்து *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
எழுத்து *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
வெறுமை ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, மாறிலி எழுத்து *fmt, ...);


எழுத்து *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    வெறுமை *conf);
எழுத்து *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_இலக்கம்_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
எழுத்து *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
