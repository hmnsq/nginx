
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

#define NGX_CONF_ARGS_सङ्ख्या 0x000000ff
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
#define NGX_CONF_UNSET_SIZE  (size_t) -1
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
    वर्ण               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
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


प्रकारनिर्देश संरचना {
    ngx_file_t            file;
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    ngx_uint_t            पङ्क्तिः;
} ngx_conf_file_t;


प्रकारनिर्देश संरचना {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;


प्रकारनिर्देश वर्ण *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, शून्य *conf);


संरचना ngx_conf_s {
    वर्ण                 *name;
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


प्रकारनिर्देश वर्ण *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    शून्य *data, शून्य *conf);

प्रकारनिर्देश संरचना {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


प्रकारनिर्देश संरचना {
    ngx_conf_post_handler_pt  post_handler;
    वर्ण                     *old_name;
    वर्ण                     *new_name;
} ngx_conf_deprecated_t;


प्रकारनिर्देश संरचना {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 नीचः;
    ngx_int_t                 उच्चः;
} ngx_conf_अङ्कः_bounds_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    ngx_uint_t                मानम्;
} ngx_conf_enum_t;


#define NGX_CONF_BITMASK_SET  1

प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



वर्ण * ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *data);
वर्ण *ngx_conf_check_अङ्कः_bounds(ngx_conf_t *cf, शून्य *post, शून्य *data);
वर्ण *ngx_conf_check_size_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *data);


बाह्य ngx_conf_post_t  ngx_conf_size_nonzero_post;


#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_मानम्(conf, स्वभाव)                                   \
    सचे (conf == NGX_CONF_UNSET) {                                            \
        conf = स्वभाव;                                                      \
    }

#define ngx_conf_init_ptr_मानम्(conf, स्वभाव)                               \
    सचे (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = स्वभाव;                                                      \
    }

#define ngx_conf_init_uint_मानम्(conf, स्वभाव)                              \
    सचे (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = स्वभाव;                                                      \
    }

#define ngx_conf_init_size_मानम्(conf, स्वभाव)                              \
    सचे (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = स्वभाव;                                                      \
    }

#define ngx_conf_init_msec_मानम्(conf, स्वभाव)                              \
    सचे (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = स्वभाव;                                                      \
    }

#define ngx_conf_merge_मानम्(conf, prev, स्वभाव)                            \
    सचे (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? स्वभाव : prev;                    \
    }

#define ngx_conf_merge_ptr_मानम्(conf, prev, स्वभाव)                        \
    सचे (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? स्वभाव : prev;                \
    }

#define ngx_conf_merge_uint_मानम्(conf, prev, स्वभाव)                       \
    सचे (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? स्वभाव : prev;               \
    }

#define ngx_conf_merge_msec_मानम्(conf, prev, स्वभाव)                       \
    सचे (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? स्वभाव : prev;               \
    }

#define ngx_conf_merge_sec_मानम्(conf, prev, स्वभाव)                        \
    सचे (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? स्वभाव : prev;                    \
    }

#define ngx_conf_merge_size_मानम्(conf, prev, स्वभाव)                       \
    सचे (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? स्वभाव : prev;               \
    }

#define ngx_conf_merge_off_मानम्(conf, prev, स्वभाव)                        \
    सचे (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? स्वभाव : prev;                    \
    }

#define ngx_conf_merge_str_मानम्(conf, prev, स्वभाव)                        \
    सचे (conf.data == रिक्त) {                                                 \
        सचे (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } अञ्ञथा {                                                             \
            conf.len = परिमाण(स्वभाव) - 1;                                  \
            conf.data = (u_char *) स्वभाव;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_मानम्(conf, prev, default_अङ्कः, default_size)     \
    सचे (conf.अङ्कः == 0) {                                                     \
        सचे (prev.अङ्कः) {                                                      \
            conf.अङ्कः = prev.अङ्कः;                                             \
            conf.size = prev.size;                                           \
        } अञ्ञथा {                                                             \
            conf.अङ्कः = default_अङ्कः;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_मानम्(conf, prev, स्वभाव)                    \
    सचे (conf == 0) {                                                         \
        conf = (prev == 0) ? स्वभाव : prev;                                 \
    }


वर्ण *ngx_conf_param(ngx_conf_t *cf);
वर्ण *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
वर्ण *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
शून्य ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ध्रुव वर्ण *fmt, ...);


वर्ण *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
वर्ण *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_अङ्कः_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


#endif /* _NGX_CONF_FILE_H_INCLUDED_ */
