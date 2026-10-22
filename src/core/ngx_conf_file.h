
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_CONF_FILE_H_INCLUDED_
#నిర్వచించు _NGX_CONF_FILE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#నిర్వచించు NGX_CONF_NOARGS      0x00000001
#నిర్వచించు NGX_CONF_TAKE1       0x00000002
#నిర్వచించు NGX_CONF_TAKE2       0x00000004
#నిర్వచించు NGX_CONF_TAKE3       0x00000008
#నిర్వచించు NGX_CONF_TAKE4       0x00000010
#నిర్వచించు NGX_CONF_TAKE5       0x00000020
#నిర్వచించు NGX_CONF_TAKE6       0x00000040
#నిర్వచించు NGX_CONF_TAKE7       0x00000080

#నిర్వచించు NGX_CONF_MAX_వాదనలు    8

#నిర్వచించు NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#నిర్వచించు NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#నిర్వచించు NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#నిర్వచించు NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#నిర్వచించు NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#నిర్వచించు NGX_CONF_వాదనలు_సంఖ్య 0x000000ff
#నిర్వచించు NGX_CONF_ఖండం       0x00000100
#నిర్వచించు NGX_CONF_FLAG        0x00000200
#నిర్వచించు NGX_CONF_ANY         0x00000400
#నిర్వచించు NGX_CONF_1_మరిన్ని       0x00000800
#నిర్వచించు NGX_CONF_2_మరిన్ని       0x00001000

#నిర్వచించు NGX_ప్రత్యక్ష_CONF      0x00010000

#నిర్వచించు NGX_MAIN_CONF        0x01000000
#నిర్వచించు NGX_ANY_CONF         0xFF000000



#నిర్వచించు NGX_CONF_UNSET       -1
#నిర్వచించు NGX_CONF_UNSET_UINT  (ngx_uint_t) -1
#నిర్వచించు NGX_CONF_UNSET_PTR   (శూన్య *) -1
#నిర్వచించు NGX_CONF_UNSET_కొలత  (కొలత_t) -1
#నిర్వచించు NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#నిర్వచించు NGX_CONF_OK          ఖాళీ
#నిర్వచించు NGX_CONF_ERROR       (శూన్య *) -1

#నిర్వచించు NGX_CONF_ఖండం_ప్రారంభం 1
#నిర్వచించు NGX_CONF_ఖండం_పూర్తి  2
#నిర్వచించు NGX_CONF_FILE_పూర్తి   3

#నిర్వచించు NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#నిర్వచించు NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#నిర్వచించు NGX_MAX_CONF_ERRSTR  1024


నిర్మాణం ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    అక్షరం               *(*సమితి)(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
    ngx_uint_t            conf;
    ngx_uint_t            స్థానభ్రంశం;
    శూన్య                 *post;
};

#నిర్వచించు ngx_null_command  { ngx_null_string, 0, ఖాళీ, 0, 0, ఖాళీ }


నిర్మాణం ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    శూన్య                (*flush)(ngx_open_file_t *file, ngx_log_t *సంవర్గమానం);
    శూన్య                 *దత్తాంశం;
};


రకనిర్వచనం నిర్మాణం {
    ngx_file_t            file;
    ngx_buf_t            *బఫరు;
    ngx_buf_t            *dump;
    ngx_uint_t            పంక్తి;
} ngx_conf_file_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t             name;
    ngx_buf_t            *బఫరు;
} ngx_conf_dump_t;


రకనిర్వచనం అక్షరం *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, శూన్య *conf);


నిర్మాణం ngx_conf_s {
    అక్షరం                 *name;
    ngx_array_t          *వాదనలు;

    ngx_చక్రం_t          *చక్రం;
    ngx_pool_t           *pool;
    ngx_pool_t           *తాత్కాలిక_pool;
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *సంవర్గమానం;

    శూన్య                 *ctx;
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    శూన్య                 *handler_conf;
};


రకనిర్వచనం అక్షరం *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    శూన్య *దత్తాంశం, శూన్య *conf);

రకనిర్వచనం నిర్మాణం {
    ngx_conf_post_handler_pt  post_handler;
} ngx_conf_post_t;


రకనిర్వచనం నిర్మాణం {
    ngx_conf_post_handler_pt  post_handler;
    అక్షరం                     *old_name;
    అక్షరం                     *new_name;
} ngx_conf_deprecated_t;


రకనిర్వచనం నిర్మాణం {
    ngx_conf_post_handler_pt  post_handler;
    ngx_int_t                 దిగువ;
    ngx_int_t                 ఎగువ;
} ngx_conf_అంకె_bounds_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                 name;
    ngx_uint_t                విలువ;
} ngx_conf_enum_t;


#నిర్వచించు NGX_CONF_BITMASK_సమితి  1

రకనిర్వచనం నిర్మాణం {
    ngx_str_t                 name;
    ngx_uint_t                ముసుగు;
} ngx_conf_bitmask_t;



అక్షరం * ngx_conf_deprecated(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం);
అక్షరం *ngx_conf_check_అంకె_bounds(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం);
అక్షరం *ngx_conf_check_కొలత_nonzero(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం);


బాహ్య ngx_conf_post_t  ngx_conf_కొలత_nonzero_post;


#నిర్వచించు ngx_get_conf(conf_ctx, module)  conf_ctx[module.సూచీ]



#నిర్వచించు ngx_conf_init_విలువ(conf, అప్రమేయ)                                   \
    ఒకవేళ (conf == NGX_CONF_UNSET) {                                            \
        conf = అప్రమేయ;                                                      \
    }

#నిర్వచించు ngx_conf_init_ptr_విలువ(conf, అప్రమేయ)                               \
    ఒకవేళ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = అప్రమేయ;                                                      \
    }

#నిర్వచించు ngx_conf_init_uint_విలువ(conf, అప్రమేయ)                              \
    ఒకవేళ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = అప్రమేయ;                                                      \
    }

#నిర్వచించు ngx_conf_init_కొలత_విలువ(conf, అప్రమేయ)                              \
    ఒకవేళ (conf == NGX_CONF_UNSET_కొలత) {                                       \
        conf = అప్రమేయ;                                                      \
    }

#నిర్వచించు ngx_conf_init_msec_విలువ(conf, అప్రమేయ)                              \
    ఒకవేళ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = అప్రమేయ;                                                      \
    }

#నిర్వచించు ngx_conf_విలీనం_విలువ(conf, prev, అప్రమేయ)                            \
    ఒకవేళ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? అప్రమేయ : prev;                    \
    }

#నిర్వచించు ngx_conf_విలీనం_ptr_విలువ(conf, prev, అప్రమేయ)                        \
    ఒకవేళ (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? అప్రమేయ : prev;                \
    }

#నిర్వచించు ngx_conf_విలీనం_uint_విలువ(conf, prev, అప్రమేయ)                       \
    ఒకవేళ (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? అప్రమేయ : prev;               \
    }

#నిర్వచించు ngx_conf_విలీనం_msec_విలువ(conf, prev, అప్రమేయ)                       \
    ఒకవేళ (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? అప్రమేయ : prev;               \
    }

#నిర్వచించు ngx_conf_విలీనం_sec_విలువ(conf, prev, అప్రమేయ)                        \
    ఒకవేళ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? అప్రమేయ : prev;                    \
    }

#నిర్వచించు ngx_conf_విలీనం_కొలత_విలువ(conf, prev, అప్రమేయ)                       \
    ఒకవేళ (conf == NGX_CONF_UNSET_కొలత) {                                       \
        conf = (prev == NGX_CONF_UNSET_కొలత) ? అప్రమేయ : prev;               \
    }

#నిర్వచించు ngx_conf_విలీనం_off_విలువ(conf, prev, అప్రమేయ)                        \
    ఒకవేళ (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? అప్రమేయ : prev;                    \
    }

#నిర్వచించు ngx_conf_విలీనం_str_విలువ(conf, prev, అప్రమేయ)                        \
    ఒకవేళ (conf.దత్తాంశం == ఖాళీ) {                                                 \
        ఒకవేళ (prev.దత్తాంశం) {                                                     \
            conf.len = prev.len;                                             \
            conf.దత్తాంశం = prev.దత్తాంశం;                                           \
        } లేకపోతే {                                                             \
            conf.len = పరిమాణం(అప్రమేయ) - 1;                                  \
            conf.దత్తాంశం = (u_char *) అప్రమేయ;                                  \
        }                                                                    \
    }

#నిర్వచించు ngx_conf_విలీనం_bufs_విలువ(conf, prev, default_అంకె, default_కొలత)     \
    ఒకవేళ (conf.అంకె == 0) {                                                     \
        ఒకవేళ (prev.అంకె) {                                                      \
            conf.అంకె = prev.అంకె;                                             \
            conf.కొలత = prev.కొలత;                                           \
        } లేకపోతే {                                                             \
            conf.అంకె = default_అంకె;                                          \
            conf.కొలత = default_కొలత;                                        \
        }                                                                    \
    }

#నిర్వచించు ngx_conf_విలీనం_bitmask_విలువ(conf, prev, అప్రమేయ)                    \
    ఒకవేళ (conf == 0) {                                                         \
        conf = (prev == 0) ? అప్రమేయ : prev;                                 \
    }


అక్షరం *ngx_conf_param(ngx_conf_t *cf);
అక్షరం *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
అక్షరం *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);


ngx_int_t ngx_conf_నిండిన_name(ngx_చక్రం_t *చక్రం, ngx_str_t *name,
    ngx_uint_t conf_ఉపసర్గ);
ngx_open_file_t *ngx_conf_open_file(ngx_చక్రం_t *చక్రం, ngx_str_t *name);
శూన్య ngx_cdecl ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...);


అక్షరం *ngx_conf_సమితి_flag_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_str_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_str_array_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd,
    శూన్య *conf);
అక్షరం *ngx_conf_సమితి_keyval_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_అంకె_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_కొలత_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_off_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_msec_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_sec_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_bufs_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_enum_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
అక్షరం *ngx_conf_సమితి_bitmask_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);


#అయితే_ముగింపు /* _NGX_CONF_FILE_H_INCLUDED_ */
