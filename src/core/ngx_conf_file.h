
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CONF_FILE_H_INCLUDED_
#घोषणा _NGX_CONF_FILE_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#घोषणा NGX_CONF_NOARGS      0x00000001
#घोषणा NGX_CONF_TAKE1       0x00000002
#घोषणा NGX_CONF_TAKE2       0x00000004
#घोषणा NGX_CONF_TAKE3       0x00000008
#घोषणा NGX_CONF_TAKE4       0x00000010
#घोषणा NGX_CONF_TAKE5       0x00000020
#घोषणा NGX_CONF_TAKE6       0x00000040
#घोषणा NGX_CONF_TAKE7       0x00000080

#घोषणा NGX_CONF_MAX_ARGS    8

#घोषणा NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#घोषणा NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#घोषणा NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#घोषणा NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#घोषणा NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#घोषणा NGX_CONF_ARGS_संख्या 0x000000ff
#घोषणा NGX_CONF_BLOCK       0x00000100
#घोषणा NGX_CONF_FLAG        0x00000200
#घोषणा NGX_CONF_ANY         0x00000400
#घोषणा NGX_CONF_1MORE       0x00000800
#घोषणा NGX_CONF_2MORE       0x00001000

#घोषणा NGX_DIRECT_CONF      0x00010000

#घोषणा NGX_MAIN_CONF        0x01000000
#घोषणा NGX_ANY_CONF         0xFF000000



#घोषणा NGX_CONF_UNSET       -1
#घोषणा NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#घोषणा NGX_CONF_UNSET_PTR   (व्योम *) -1
#घोषणा NGX_CONF_UNSET_आकार  (माप_प्रकार) -1
#घोषणा NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#घोषणा NGX_CONF_OK          शून्य
#घोषणा NGX_CONF_ERROR       (व्योम *) -1

#घोषणा NGX_CONF_BLOCK_START 1
#घोषणा NGX_CONF_BLOCK_DONE  2
#घोषणा NGX_CONF_FILE_DONE   3

#घोषणा NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#घोषणा NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#घोषणा NGX_MAX_CONF_ERRSTR  1024


काष्ठा ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    अक्षर               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    व्योम                 *post;
};

#घोषणा ngx_null_command  { ngx_null_string, 0, शून्य, 0, 0, शून्य }


काष्ठा ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    व्योम                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    व्योम                 *तथ्य;
};


प्रकार काष्ठा {
    ngx_file_t            file;
    ngx_buf_t            *बफर;
    ngx_buf_t            *dump;
    ngx_uint_t            पंक्ति;
} ngx_conf_file_t;


प्रकार काष्ठा {
    ngx_str_t             name;
    ngx_buf_t            *बफर;
} ngx_conf_dump_t;


प्रकार अक्षर *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, व्योम *conf);


काष्ठा ngx_conf_s {
    अक्षर                 *name;
    ngx_array_t          *args;

    ngx_cycle_t          *cycle;
    ngx_pool_t           *pool;
    ngx_pool_t           *अस्थायी_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    व्योम                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    व्योम                 *handler_conf;
};


प्रकार अक्षर *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    व्योम *तथ्य, व्योम *conf);

प्रकार काष्ठा {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


प्रकार काष्ठा {
    ngx_conf_post_handler_pt  post_handler;
    अक्षर                     *old_name;
    अक्षर                     *new_name;
} ngx_conf_deprecated_t;


प्रकार काष्ठा {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 निचला;
    ngx_int_t                 ऊपरी;
} ngx_conf_अंक_bounds_t;


प्रकार काष्ठा {
    ngx_str_t                 name;
    ngx_uint_t                मूल्य;
} ngx_conf_enum_t;


#घोषणा NGX_CONF_BITMASK_SET  1

प्रकार काष्ठा {
    ngx_str_t                 name;
    ngx_uint_t                mask;
} ngx_conf_bitmask_t;



अक्षर * ngx_conf_deprecated(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य);
अक्षर *ngx_conf_check_अंक_bounds(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य);
अक्षर *ngx_conf_check_आकार_nonzero(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य);


बाह्य ngx_conf_post_t  ngx_conf_आकार_nonzero_post;


#घोषणा ngx_get_conf(conf_ctx, module)  conf_ctx[module.सूचकांक]



#घोषणा ngx_conf_init_मूल्य(conf, शेष)                                   \
    अगर (conf == NGX_CONF_UNSET) {                                            \
        conf = शेष;                                                      \
    }

#घोषणा ngx_conf_init_ptr_मूल्य(conf, शेष)                               \
    अगर (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = शेष;                                                      \
    }

#घोषणा ngx_conf_init_uint_मूल्य(conf, शेष)                              \
    अगर (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = शेष;                                                      \
    }

#घोषणा ngx_conf_init_आकार_मूल्य(conf, शेष)                              \
    अगर (conf == NGX_CONF_UNSET_आकार) {                                       \
        conf = शेष;                                                      \
    }

#घोषणा ngx_conf_init_msec_मूल्य(conf, शेष)                              \
    अगर (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = शेष;                                                      \
    }

#घोषणा ngx_conf_merge_मूल्य(conf, पूर्व, शेष)                            \
    अगर (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? शेष : पूर्व;                    \
    }

#घोषणा ngx_conf_merge_ptr_मूल्य(conf, पूर्व, शेष)                        \
    अगर (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (पूर्व == NGX_CONF_UNSET_PTR) ? शेष : पूर्व;                \
    }

#घोषणा ngx_conf_merge_uint_मूल्य(conf, पूर्व, शेष)                       \
    अगर (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_UINT) ? शेष : पूर्व;               \
    }

#घोषणा ngx_conf_merge_msec_मूल्य(conf, पूर्व, शेष)                       \
    अगर (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_MSEC) ? शेष : पूर्व;               \
    }

#घोषणा ngx_conf_merge_sec_मूल्य(conf, पूर्व, शेष)                        \
    अगर (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? शेष : पूर्व;                    \
    }

#घोषणा ngx_conf_merge_आकार_मूल्य(conf, पूर्व, शेष)                       \
    अगर (conf == NGX_CONF_UNSET_आकार) {                                       \
        conf = (पूर्व == NGX_CONF_UNSET_आकार) ? शेष : पूर्व;               \
    }

#घोषणा ngx_conf_merge_off_मूल्य(conf, पूर्व, शेष)                        \
    अगर (conf == NGX_CONF_UNSET) {                                            \
        conf = (पूर्व == NGX_CONF_UNSET) ? शेष : पूर्व;                    \
    }

#घोषणा ngx_conf_merge_str_मूल्य(conf, पूर्व, शेष)                        \
    अगर (conf.तथ्य == शून्य) {                                                 \
        अगर (पूर्व.तथ्य) {                                                     \
            conf.len = पूर्व.len;                                             \
            conf.तथ्य = पूर्व.तथ्य;                                           \
        } अन्यथा {                                                             \
            conf.len = माप(शेष) - 1;                                  \
            conf.तथ्य = (u_char *) शेष;                                  \
        }                                                                    \
    }

#घोषणा ngx_conf_merge_bufs_मूल्य(conf, पूर्व, default_अंक, default_आकार)     \
    अगर (conf.अंक == 0) {                                                     \
        अगर (पूर्व.अंक) {                                                      \
            conf.अंक = पूर्व.अंक;                                             \
            conf.आकार = पूर्व.आकार;                                           \
        } अन्यथा {                                                             \
            conf.अंक = default_अंक;                                          \
            conf.आकार = default_आकार;                                        \
        }                                                                    \
    }

#घोषणा ngx_conf_merge_bitmask_मूल्य(conf, पूर्व, शेष)                    \
    अगर (conf == 0) {                                                         \
        conf = (पूर्व == 0) ? शेष : पूर्व;                                 \
    }


अक्षर *ngx_conf_param(ngx_conf_t *cf);
अक्षर *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
अक्षर *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
व्योम ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, स्थिर अक्षर *fmt, ...);


अक्षर *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    व्योम *conf);
अक्षर *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_अंक_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_आकार_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अक्षर *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);


#पूर्ण_अगर /* _NGX_CONF_FILE_H_INCLUDED_ */
