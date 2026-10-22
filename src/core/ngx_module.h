
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_MODULE_H_INCLUDED_
#నిర్వచించు _NGX_MODULE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <nginx.h>


#నిర్వచించు NGX_MODULE_UNSET_సూచీ  (ngx_uint_t) -1


#నిర్వచించు NGX_MODULE_SIGNATURE_0                                                \
    ngx_విలువ(NGX_PTR_కొలత) ","                                               \
    ngx_విలువ(NGX_SIG_ATOMIC_T_కొలత) ","                                      \
    ngx_విలువ(NGX_సమయం_T_కొలత) ","

#ఒకవేళ (NGX_HAVE_KQUEUE)
#నిర్వచించు NGX_MODULE_SIGNATURE_1   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_1   "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_2   "0"

#ఒకవేళ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_3   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_3   "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_4   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_4   "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_EVENTFD)
#నిర్వచించు NGX_MODULE_SIGNATURE_5   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_5   "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_EPOLL)
#నిర్వచించు NGX_MODULE_SIGNATURE_6   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_6   "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_KEEPALIVE_TUNABLE)
#నిర్వచించు NGX_MODULE_SIGNATURE_7   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_7   "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_INET6)
#నిర్వచించు NGX_MODULE_SIGNATURE_8   "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_8   "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_9   "1"
#నిర్వచించు NGX_MODULE_SIGNATURE_10  "1"

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#నిర్వచించు NGX_MODULE_SIGNATURE_11  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_11  "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_12  "1"

#ఒకవేళ (NGX_HAVE_SETFIB)
#నిర్వచించు NGX_MODULE_SIGNATURE_13  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_13  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_TCP_FASTOPEN)
#నిర్వచించు NGX_MODULE_SIGNATURE_14  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_14  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
#నిర్వచించు NGX_MODULE_SIGNATURE_15  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_15  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_VARIADIC_MACROS)
#నిర్వచించు NGX_MODULE_SIGNATURE_16  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_16  "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_17  "0"

#ఒకవేళ (NGX_QUIC || NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_18  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_18  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_OPENAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_19  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_19  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)
#నిర్వచించు NGX_MODULE_SIGNATURE_20  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_20  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_POSIX_SEM)
#నిర్వచించు NGX_MODULE_SIGNATURE_21  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_21  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_దారాలు || NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_22  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_22  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_PCRE)
#నిర్వచించు NGX_MODULE_SIGNATURE_23  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_23  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_SSL || NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_24  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_24  "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_25  "1"

#ఒకవేళ (NGX_HTTP_GZIP)
#నిర్వచించు NGX_MODULE_SIGNATURE_26  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_26  "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE_27  "1"

#ఒకవేళ (NGX_HTTP_X_FORWARDED_FOR)
#నిర్వచించు NGX_MODULE_SIGNATURE_28  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_28  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_REALIP)
#నిర్వచించు NGX_MODULE_SIGNATURE_29  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_29  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_శీర్షికలు)
#నిర్వచించు NGX_MODULE_SIGNATURE_30  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_30  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_DAV)
#నిర్వచించు NGX_MODULE_SIGNATURE_31  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_31  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_CACHE)
#నిర్వచించు NGX_MODULE_SIGNATURE_32  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_32  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_HTTP_UPSTREAM_ZONE)
#నిర్వచించు NGX_MODULE_SIGNATURE_33  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_33  "0"
#అయితే_ముగింపు

#ఒకవేళ (NGX_COMPAT)
#నిర్వచించు NGX_MODULE_SIGNATURE_34  "1"
#లేకపోతే
#నిర్వచించు NGX_MODULE_SIGNATURE_34  "0"
#అయితే_ముగింపు

#నిర్వచించు NGX_MODULE_SIGNATURE                                                  \
    NGX_MODULE_SIGNATURE_0 NGX_MODULE_SIGNATURE_1 NGX_MODULE_SIGNATURE_2      \
    NGX_MODULE_SIGNATURE_3 NGX_MODULE_SIGNATURE_4 NGX_MODULE_SIGNATURE_5      \
    NGX_MODULE_SIGNATURE_6 NGX_MODULE_SIGNATURE_7 NGX_MODULE_SIGNATURE_8      \
    NGX_MODULE_SIGNATURE_9 NGX_MODULE_SIGNATURE_10 NGX_MODULE_SIGNATURE_11    \
    NGX_MODULE_SIGNATURE_12 NGX_MODULE_SIGNATURE_13 NGX_MODULE_SIGNATURE_14   \
    NGX_MODULE_SIGNATURE_15 NGX_MODULE_SIGNATURE_16 NGX_MODULE_SIGNATURE_17   \
    NGX_MODULE_SIGNATURE_18 NGX_MODULE_SIGNATURE_19 NGX_MODULE_SIGNATURE_20   \
    NGX_MODULE_SIGNATURE_21 NGX_MODULE_SIGNATURE_22 NGX_MODULE_SIGNATURE_23   \
    NGX_MODULE_SIGNATURE_24 NGX_MODULE_SIGNATURE_25 NGX_MODULE_SIGNATURE_26   \
    NGX_MODULE_SIGNATURE_27 NGX_MODULE_SIGNATURE_28 NGX_MODULE_SIGNATURE_29   \
    NGX_MODULE_SIGNATURE_30 NGX_MODULE_SIGNATURE_31 NGX_MODULE_SIGNATURE_32   \
    NGX_MODULE_SIGNATURE_33 NGX_MODULE_SIGNATURE_34


#నిర్వచించు NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_సూచీ, NGX_MODULE_UNSET_సూచీ,                           \
    ఖాళీ, 0, 0, nginx_సంస్కరణ, NGX_MODULE_SIGNATURE

#నిర్వచించు NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


నిర్మాణం ngx_module_s {
    ngx_uint_t            ctx_సూచీ;
    ngx_uint_t            సూచీ;

    అక్షరం                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            సంస్కరణ;
    స్థిరరాశి అక్షరం           *signature;

    శూన్య                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *సంవర్గమానం);

    ngx_int_t           (*init_module)(ngx_చక్రం_t *చక్రం);

    ngx_int_t           (*init_process)(ngx_చక్రం_t *చక్రం);
    ngx_int_t           (*init_thread)(ngx_చక్రం_t *చక్రం);
    శూన్య                (*exit_thread)(ngx_చక్రం_t *చక్రం);
    శూన్య                (*exit_process)(ngx_చక్రం_t *చక్రం);

    శూన్య                (*exit_master)(ngx_చక్రం_t *చక్రం);

    uintptr_t             అదనపు_hook0;
    uintptr_t             అదనపు_hook1;
    uintptr_t             అదనపు_hook2;
    uintptr_t             అదనపు_hook3;
    uintptr_t             అదనపు_hook4;
    uintptr_t             అదనపు_hook5;
    uintptr_t             అదనపు_hook6;
    uintptr_t             అదనపు_hook7;
};


రకనిర్వచనం నిర్మాణం {
    ngx_str_t             name;
    శూన్య               *(*create_conf)(ngx_చక్రం_t *చక్రం);
    అక్షరం               *(*init_conf)(ngx_చక్రం_t *చక్రం, శూన్య *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(శూన్య);
ngx_int_t ngx_చక్రం_modules(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_init_modules(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_లెక్క_modules(ngx_చక్రం_t *చక్రం, ngx_uint_t type);


ngx_int_t ngx_కూడు_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, అక్షరం **క్రమం);


బాహ్య ngx_module_t  *ngx_modules[];
బాహ్య ngx_uint_t     ngx_max_module;

బాహ్య అక్షరం          *ngx_module_పేర్లు[];


#అయితే_ముగింపు /* _NGX_MODULE_H_INCLUDED_ */
