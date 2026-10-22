
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONF_FILE_H_INCLUDED_
#define _NGX_CONF_FILE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


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

#define NGX_CONF_ARGS_संख्या 0x000000ff
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
#define NGX_CONF_UNSET_PTR   (शून्य *) -1
#define NGX_CONF_UNSET_SIZE  (माप_प्रकार) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          रिक्त
#define NGX_CONF_ERROR       (शून्य *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024


संरचना ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    अक्षर               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    शून्य                 *post;
};

#define ngx_null_command  { ngx_null_string, 0, रिक्त, 0, 0, रिक्त }


संरचना ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    शून्य                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    शून्य                 *data;
};


प्रकारव्याख्या संरचना {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            ओळ;
} ngx_conf_file_t;


प्रकारव्याख्या संरचना {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


प्रकारव्याख्या अक्षर *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, शून्य *conf);


संरचना ngx_conf_s {
    अक्षर                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    शून्य                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    शून्य                 *handler_conf;
};


प्रकारव्याख्या अक्षर *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    शून्य *data, शून्य *conf);

प्रकारव्याख्या संरचना {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


प्रकारव्याख्या संरचना {
    ngx_conf_post_handler_pt  post_handler;
    अक्षर                     *old_name;
    अक्षर                     *new_name;
} ngx_conf_deprecated_t;


प्रकारव्याख्या संरचना {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 खालचा;
    ngx_int_t                 वरचा;
} ngx_conf_अंक_bounds_t;


प्रकारव्याख्या संरचना {
    ngx_str_t                 name;
    ngx_uint_t                मूल्य;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

प्रकारव्याख्या संरचना {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



अक्षर * ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *data);
अक्षर *ngx_conf_check_अंक_bounds(ngx_conf_t *cf, शून्य *post, शून्य *data);
अक्षर *ngx_conf_check_size_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *data);


बाह्य ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_मूल्य(conf, पूर्वनिर्धारित)                                   \
    जर (conf == NGX_CONF_UNSET) {                                            \
        conf = पूर्वनिर्धारित;                                                      \
    }

#define ngx_conf_init_ptr_मूल्य(conf, पूर्वनिर्धारित)                               \
    जर (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = पूर्वनिर्धारित;                                                      \
    }

#define ngx_conf_init_uint_मूल्य(conf, पूर्वनिर्धारित)                              \
    जर (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = पूर्वनिर्धारित;                                                      \
    }

#define ngx_conf_init_size_मूल्य(conf, पूर्वनिर्धारित)                              \
    जर (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = पूर्वनिर्धारित;                                                      \
    }

#define ngx_conf_init_msec_मूल्य(conf, पूर्वनिर्धारित)                              \
    जर (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = पूर्वनिर्धारित;                                                      \
    }

#define ngx_conf_merge_मूल्य(conf, prev, पूर्वनिर्धारित)                            \
    जर (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? पूर्वनिर्धारित : prev;                    \
    }

#define ngx_conf_merge_ptr_मूल्य(conf, prev, पूर्वनिर्धारित)                        \
    जर (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? पूर्वनिर्धारित : prev;                \
    }

#define ngx_conf_merge_uint_मूल्य(conf, prev, पूर्वनिर्धारित)                       \
    जर (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? पूर्वनिर्धारित : prev;               \
    }

#define ngx_conf_merge_msec_मूल्य(conf, prev, पूर्वनिर्धारित)                       \
    जर (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? पूर्वनिर्धारित : prev;               \
    }

#define ngx_conf_merge_sec_मूल्य(conf, prev, पूर्वनिर्धारित)                        \
    जर (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? पूर्वनिर्धारित : prev;                    \
    }

#define ngx_conf_merge_size_मूल्य(conf, prev, पूर्वनिर्धारित)                       \
    जर (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? पूर्वनिर्धारित : prev;               \
    }

#define ngx_conf_merge_off_मूल्य(conf, prev, पूर्वनिर्धारित)                        \
    जर (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? पूर्वनिर्धारित : prev;                    \
    }

#define ngx_conf_merge_str_मूल्य(conf, prev, पूर्वनिर्धारित)                        \
    जर (conf.data == रिक्त) {                                                 \
        जर (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } अन्यथा {                                                             \
            conf.len = आकार(पूर्वनिर्धारित) - 1;                                  \
            conf.data = (u_char *) पूर्वनिर्धारित;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_मूल्य(conf, prev, default_अंक, default_size)     \
    जर (conf.अंक == 0) {                                                     \
        जर (prev.अंक) {                                                      \
            conf.अंक = prev.अंक;                                             \
            conf.size = prev.size;                                           \
        } अन्यथा {                                                             \
            conf.अंक = default_अंक;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_मूल्य(conf, prev, पूर्वनिर्धारित)                    \
    जर (conf == 0) {                                                         \
        conf = (prev == 0) ? पूर्वनिर्धारित : prev;                                 \
    }


अक्षर *ngx_conf_param(ngx_conf_t *cf);
अक्षर *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
अक्षर *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
शून्य ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, स्थिरांक अक्षर *fmt, ...);


अक्षर *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
अक्षर *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_अंक_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
अक्षर *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
