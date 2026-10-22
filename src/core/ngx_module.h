
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_MODULE_H_INCLUDED_
#تعریف_کړه _NGX_MODULE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


#تعریف_کړه NGX_MODULE_UNSET_شاخص  (ngx_uint_t) -1


#تعریف_کړه NGX_MODULE_SIGNATURE_0                                                \
    ngx_ارزښت(NGX_PTR_اندازه) ","                                               \
    ngx_ارزښت(NGX_SIG_ATOMIC_T_اندازه) ","                                      \
    ngx_ارزښت(NGX_وخت_T_اندازه) ","

#إذا (NGX_HAVE_KQUEUE)
#تعریف_کړه NGX_MODULE_SIGNATURE_1   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_1   "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_2   "0"

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_3   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_3   "0"
#د_که_پای

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_4   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_4   "0"
#د_که_پای

#إذا (NGX_HAVE_EVENTFD)
#تعریف_کړه NGX_MODULE_SIGNATURE_5   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_5   "0"
#د_که_پای

#إذا (NGX_HAVE_EPOLL)
#تعریف_کړه NGX_MODULE_SIGNATURE_6   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_6   "0"
#د_که_پای

#إذا (NGX_HAVE_KEEPALIVE_TUNABLE)
#تعریف_کړه NGX_MODULE_SIGNATURE_7   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_7   "0"
#د_که_پای

#إذا (NGX_HAVE_INET6)
#تعریف_کړه NGX_MODULE_SIGNATURE_8   "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_8   "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_9   "1"
#تعریف_کړه NGX_MODULE_SIGNATURE_10  "1"

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#تعریف_کړه NGX_MODULE_SIGNATURE_11  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_11  "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_12  "1"

#إذا (NGX_HAVE_SETFIB)
#تعریف_کړه NGX_MODULE_SIGNATURE_13  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_13  "0"
#د_که_پای

#إذا (NGX_HAVE_TCP_FASTOPEN)
#تعریف_کړه NGX_MODULE_SIGNATURE_14  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_14  "0"
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کړه NGX_MODULE_SIGNATURE_15  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_15  "0"
#د_که_پای

#إذا (NGX_HAVE_VARIADIC_MACROS)
#تعریف_کړه NGX_MODULE_SIGNATURE_16  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_16  "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_17  "0"

#إذا (NGX_QUIC || NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_18  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_18  "0"
#د_که_پای

#إذا (NGX_HAVE_OPENAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_19  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_19  "0"
#د_که_پای

#إذا (NGX_HAVE_ATOMIC_OPS)
#تعریف_کړه NGX_MODULE_SIGNATURE_20  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_20  "0"
#د_که_پای

#إذا (NGX_HAVE_POSIX_SEM)
#تعریف_کړه NGX_MODULE_SIGNATURE_21  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_21  "0"
#د_که_پای

#إذا (NGX_تارونه || NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_22  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_22  "0"
#د_که_پای

#إذا (NGX_PCRE)
#تعریف_کړه NGX_MODULE_SIGNATURE_23  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_23  "0"
#د_که_پای

#إذا (NGX_HTTP_SSL || NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_24  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_24  "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_25  "1"

#إذا (NGX_HTTP_GZIP)
#تعریف_کړه NGX_MODULE_SIGNATURE_26  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_26  "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE_27  "1"

#إذا (NGX_HTTP_X_FORWARDED_FOR)
#تعریف_کړه NGX_MODULE_SIGNATURE_28  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_28  "0"
#د_که_پای

#إذا (NGX_HTTP_REALIP)
#تعریف_کړه NGX_MODULE_SIGNATURE_29  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_29  "0"
#د_که_پای

#إذا (NGX_HTTP_سرلیکونه)
#تعریف_کړه NGX_MODULE_SIGNATURE_30  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_30  "0"
#د_که_پای

#إذا (NGX_HTTP_DAV)
#تعریف_کړه NGX_MODULE_SIGNATURE_31  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_31  "0"
#د_که_پای

#إذا (NGX_HTTP_CACHE)
#تعریف_کړه NGX_MODULE_SIGNATURE_32  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_32  "0"
#د_که_پای

#إذا (NGX_HTTP_UPSTREAM_ZONE)
#تعریف_کړه NGX_MODULE_SIGNATURE_33  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_33  "0"
#د_که_پای

#إذا (NGX_COMPAT)
#تعریف_کړه NGX_MODULE_SIGNATURE_34  "1"
#وإلا
#تعریف_کړه NGX_MODULE_SIGNATURE_34  "0"
#د_که_پای

#تعریف_کړه NGX_MODULE_SIGNATURE                                                  \
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


#تعریف_کړه NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_شاخص, NGX_MODULE_UNSET_شاخص,                           \
    عدم, 0, 0, nginx_نسخه, NGX_MODULE_SIGNATURE

#تعریف_کړه NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


بنية ngx_module_s {
    ngx_uint_t            ctx_شاخص;
    ngx_uint_t            شاخص;

    حرف                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            نسخه;
    ثابت حرف           *signature;

    فراغ                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *لوګاریتم);

    ngx_int_t           (*init_module)(ngx_دوره_t *دوره);

    ngx_int_t           (*init_process)(ngx_دوره_t *دوره);
    ngx_int_t           (*init_thread)(ngx_دوره_t *دوره);
    فراغ                (*exit_thread)(ngx_دوره_t *دوره);
    فراغ                (*exit_process)(ngx_دوره_t *دوره);

    فراغ                (*exit_master)(ngx_دوره_t *دوره);

    uintptr_t             اضافي_hook0;
    uintptr_t             اضافي_hook1;
    uintptr_t             اضافي_hook2;
    uintptr_t             اضافي_hook3;
    uintptr_t             اضافي_hook4;
    uintptr_t             اضافي_hook5;
    uintptr_t             اضافي_hook6;
    uintptr_t             اضافي_hook7;
};


تعريفنوع بنية {
    ngx_str_t             name;
    فراغ               *(*create_conf)(ngx_دوره_t *دوره);
    حرف               *(*init_conf)(ngx_دوره_t *دوره, فراغ *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(فراغ);
ngx_int_t ngx_دوره_modules(ngx_دوره_t *دوره);
ngx_int_t ngx_init_modules(ngx_دوره_t *دوره);
ngx_int_t ngx_شمېر_modules(ngx_دوره_t *دوره, ngx_uint_t type);


ngx_int_t ngx_جمع_کړه_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, حرف **ترتیب);


خارجي ngx_module_t  *ngx_modules[];
خارجي ngx_uint_t     ngx_max_module;

خارجي حرف          *ngx_module_نومونه[];


#د_که_پای /* _NGX_MODULE_H_INCLUDED_ */
