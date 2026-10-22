
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_CONF_FILE_H_INCLUDED_
#परिभाषय _NGX_CONF_FILE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#परिभाषय NGX_CONF_NOARGS      0x00000001
#परिभाषय NGX_CONF_TAKE1       0x00000002
#परिभाषय NGX_CONF_TAKE2       0x00000004
#परिभाषय NGX_CONF_TAKE3       0x00000008
#परिभाषय NGX_CONF_TAKE4       0x00000010
#परिभाषय NGX_CONF_TAKE5       0x00000020
#परिभाषय NGX_CONF_TAKE6       0x00000040
#परिभाषय NGX_CONF_TAKE7       0x00000080

#परिभाषय NGX_CONF_MAX_तर्काः    8

#परिभाषय NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#परिभाषय NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#परिभाषय NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#परिभाषय NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#परिभाषय NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#परिभाषय NGX_CONF_तर्काः_सङ्ख्या 0x000000ff
#परिभाषय NGX_CONF_खण्डम्       0x00000100
#परिभाषय NGX_CONF_FLAG        0x00000200
#परिभाषय NGX_CONF_ANY         0x00000400
#परिभाषय NGX_CONF_1_अधिकम्       0x00000800
#परिभाषय NGX_CONF_2_अधिकम्       0x00001000

#परिभाषय NGX_प्रत्यक्षम्_CONF      0x00010000

#परिभाषय NGX_MAIN_CONF        0x01000000
#परिभाषय NGX_ANY_CONF         0xFF000000



#परिभाषय NGX_CONF_UNSET       -1
#परिभाषय NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#परिभाषय NGX_CONF_UNSET_PTR   (शून्य *) -1
#परिभाषय NGX_CONF_UNSET_आकार  (आकार_t) -1
#परिभाषय NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#परिभाषय NGX_CONF_OK          रिक्त
#परिभाषय NGX_CONF_ERROR       (शून्य *) -1

#परिभाषय NGX_CONF_खण्डम्_START 1
#परिभाषय NGX_CONF_खण्डम्_समाप्तम्  2
#परिभाषय NGX_CONF_FILE_समाप्तम्   3

#परिभाषय NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#परिभाषय NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#परिभाषय NGX_MAX_CONF_ERRSTR  1024


संरचना ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    वर्ण               *(*समुच्चयः)(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
    ngx_uint_t            conf;
    ngx_uint_t            विस्थापनम्;
    शून्य                 *post;
};

#परिभाषय ngx_null_command  { ngx_null_string, 0, रिक्त, 0, 0, रिक्त }


संरचना ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    शून्य                (*flush)(ngx_open_file_t *file, ngx_log_t *लघुगणकः);
    शून्य                 *दत्त;
};


प्रकारनिर्देश संरचना {
    ngx_file_t            file;
    ngx_buf_t            *बफर;
    ngx_buf_t            *dump;
    ngx_uint_t            पङ्क्तिः;
} ngx_conf_file_t;


प्रकारनिर्देश संरचना {
    ngx_str_t             name;
    ngx_buf_t            *बफर;
} ngx_conf_dump_t;


प्रकारनिर्देश वर्ण *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, शून्य *conf);


संरचना ngx_conf_s {
    वर्ण                 *name;
    ngx_array_t          *तर्काः;

    ngx_आवृत्तिः_t          *आवृत्तिः;
    ngx_pool_t           *pool;
    ngx_pool_t           *अस्थायी_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *लघुगणकः;

    शून्य                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    शून्य                 *handler_conf;
};


प्रकारनिर्देश वर्ण *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    शून्य *दत्त, शून्य *conf);

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


#परिभाषय NGX_CONF_BITMASK_समुच्चयः  1

प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    ngx_uint_t                आवरणम्;
} ngx_conf_bitmask_t;



वर्ण * ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *दत्त);
वर्ण *ngx_conf_check_अङ्कः_bounds(ngx_conf_t *cf, शून्य *post, शून्य *दत्त);
वर्ण *ngx_conf_check_आकार_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *दत्त);


बाह्य ngx_conf_post_t  ngx_conf_आकार_nonzero_post;


#परिभाषय ngx_get_conf(conf_ctx, module)  conf_ctx[module.सूचकांक]



#परिभाषय ngx_conf_init_मानम्(conf, स्वभाव)                                   \
    यदि (conf == NGX_CONF_UNSET) {                                            \
        conf = स्वभाव;                                                      \
    }

#परिभाषय ngx_conf_init_ptr_मानम्(conf, स्वभाव)                               \
    यदि (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = स्वभाव;                                                      \
    }

#परिभाषय ngx_conf_init_uint_मानम्(conf, स्वभाव)                              \
    यदि (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = स्वभाव;                                                      \
    }

#परिभाषय ngx_conf_init_आकार_मानम्(conf, स्वभाव)                              \
    यदि (conf == NGX_CONF_UNSET_आकार) {                                       \
        conf = स्वभाव;                                                      \
    }

#परिभाषय ngx_conf_init_msec_मानम्(conf, स्वभाव)                              \
    यदि (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = स्वभाव;                                                      \
    }

#परिभाषय ngx_conf_merge_मानम्(conf, पूर्व, स्वभाव)                            \
    यदि (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? स्वभाव : पूर्व;                    \
    }

#परिभाषय ngx_conf_merge_ptr_मानम्(conf, पूर्व, स्वभाव)                        \
    यदि (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (पूर्व == NGX_CONF_UNSET_PTR) ? स्वभाव : पूर्व;                \
    }

#परिभाषय ngx_conf_merge_uint_मानम्(conf, पूर्व, स्वभाव)                       \
    यदि (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_UINT) ? स्वभाव : पूर्व;               \
    }

#परिभाषय ngx_conf_merge_msec_मानम्(conf, पूर्व, स्वभाव)                       \
    यदि (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_MSEC) ? स्वभाव : पूर्व;               \
    }

#परिभाषय ngx_conf_merge_sec_मानम्(conf, पूर्व, स्वभाव)                        \
    यदि (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? स्वभाव : पूर्व;                    \
    }

#परिभाषय ngx_conf_merge_आकार_मानम्(conf, पूर्व, स्वभाव)                       \
    यदि (conf == NGX_CONF_UNSET_आकार) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_आकार) ? स्वभाव : पूर्व;               \
    }

#परिभाषय ngx_conf_merge_off_मानम्(conf, पूर्व, स्वभाव)                        \
    यदि (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? स्वभाव : पूर्व;                    \
    }

#परिभाषय ngx_conf_merge_str_मानम्(conf, पूर्व, स्वभाव)                        \
    यदि (conf.दत्त == रिक्त) {                                                 \
        यदि (पूर्व.दत्त) {                                                     \
            conf.len = पूर्व.len;                                             \
            conf.दत्त = पूर्व.दत्त;                                           \
        } अन्यथा {                                                             \
            conf.len = परिमाण(स्वभाव) - 1;                                  \
            conf.दत्त = (u_char *) स्वभाव;                                  \
        }                                                                    \
    }

#परिभाषय ngx_conf_merge_bufs_मानम्(conf, पूर्व, default_अङ्कः, default_आकार)     \
    यदि (conf.अङ्कः == 0) {                                                     \
        यदि (पूर्व.अङ्कः) {                                                      \
            conf.अङ्कः = पूर्व.अङ्कः;                                             \
            conf.आकार = पूर्व.आकार;                                           \
        } अन्यथा {                                                             \
            conf.अङ्कः = default_अङ्कः;                                          \
            conf.आकार = default_आकार;                                        \
        }                                                                    \
    }

#परिभाषय ngx_conf_merge_bitmask_मानम्(conf, पूर्व, स्वभाव)                    \
    यदि (conf == 0) {                                                         \
        conf = (पूर्व == 0) ? स्वभाव : पूर्व;                                 \
    }


वर्ण *ngx_conf_param(ngx_conf_t *cf);
वर्ण *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
वर्ण *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


ngx_int_t ngx_conf_पूर्णम्_name(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name,
    ngx_uint_t conf_उपसर्गः);
ngx_open_file_t *ngx_conf_open_file(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name);
शून्य ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, ध्रुव वर्ण *fmt, ...);


वर्ण *ngx_conf_समुच्चयः_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_अङ्कः_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_आकार_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
वर्ण *ngx_conf_समुच्चयः_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);


#यदि_अन्तः /* _NGX_CONF_FILE_H_INCLUDED_ */
