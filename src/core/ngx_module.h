
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_MODULE_H_INCLUDED_
#تعریف_کرو _NGX_MODULE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <nginx.h>


#تعریف_کرو NGX_MODULE_UNSET_اشاریہ  (ngx_uint_t) -1


#تعریف_کرو NGX_MODULE_SIGNATURE_0                                                \
    ngx_قدر(NGX_PTR_سائز) ","                                               \
    ngx_قدر(NGX_SIG_ATOMIC_T_سائز) ","                                      \
    ngx_قدر(NGX_وقت_T_سائز) ","

#اگر (NGX_HAVE_KQUEUE)
#تعریف_کرو NGX_MODULE_SIGNATURE_1   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_1   "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_2   "0"

#اگر (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_3   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_3   "0"
#اگر_ختم

#اگر (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_4   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_4   "0"
#اگر_ختم

#اگر (NGX_HAVE_EVENTFD)
#تعریف_کرو NGX_MODULE_SIGNATURE_5   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_5   "0"
#اگر_ختم

#اگر (NGX_HAVE_EPOLL)
#تعریف_کرو NGX_MODULE_SIGNATURE_6   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_6   "0"
#اگر_ختم

#اگر (NGX_HAVE_KEEPALIVE_TUNABLE)
#تعریف_کرو NGX_MODULE_SIGNATURE_7   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_7   "0"
#اگر_ختم

#اگر (NGX_HAVE_INET6)
#تعریف_کرو NGX_MODULE_SIGNATURE_8   "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_8   "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_9   "1"
#تعریف_کرو NGX_MODULE_SIGNATURE_10  "1"

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#تعریف_کرو NGX_MODULE_SIGNATURE_11  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_11  "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_12  "1"

#اگر (NGX_HAVE_SETFIB)
#تعریف_کرو NGX_MODULE_SIGNATURE_13  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_13  "0"
#اگر_ختم

#اگر (NGX_HAVE_TCP_FASTOPEN)
#تعریف_کرو NGX_MODULE_SIGNATURE_14  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_14  "0"
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کرو NGX_MODULE_SIGNATURE_15  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_15  "0"
#اگر_ختم

#اگر (NGX_HAVE_VARIADIC_MACROS)
#تعریف_کرو NGX_MODULE_SIGNATURE_16  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_16  "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_17  "0"

#اگر (NGX_QUIC || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_18  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_18  "0"
#اگر_ختم

#اگر (NGX_HAVE_OPENAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_19  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_19  "0"
#اگر_ختم

#اگر (NGX_HAVE_ATOMIC_OPS)
#تعریف_کرو NGX_MODULE_SIGNATURE_20  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_20  "0"
#اگر_ختم

#اگر (NGX_HAVE_POSIX_SEM)
#تعریف_کرو NGX_MODULE_SIGNATURE_21  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_21  "0"
#اگر_ختم

#اگر (NGX_دھاگے || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_22  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_22  "0"
#اگر_ختم

#اگر (NGX_PCRE)
#تعریف_کرو NGX_MODULE_SIGNATURE_23  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_23  "0"
#اگر_ختم

#اگر (NGX_HTTP_SSL || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_24  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_24  "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_25  "1"

#اگر (NGX_HTTP_GZIP)
#تعریف_کرو NGX_MODULE_SIGNATURE_26  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_26  "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE_27  "1"

#اگر (NGX_HTTP_X_FORWARDED_FOR)
#تعریف_کرو NGX_MODULE_SIGNATURE_28  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_28  "0"
#اگر_ختم

#اگر (NGX_HTTP_REALIP)
#تعریف_کرو NGX_MODULE_SIGNATURE_29  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_29  "0"
#اگر_ختم

#اگر (NGX_HTTP_سرخیاں)
#تعریف_کرو NGX_MODULE_SIGNATURE_30  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_30  "0"
#اگر_ختم

#اگر (NGX_HTTP_DAV)
#تعریف_کرو NGX_MODULE_SIGNATURE_31  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_31  "0"
#اگر_ختم

#اگر (NGX_HTTP_CACHE)
#تعریف_کرو NGX_MODULE_SIGNATURE_32  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_32  "0"
#اگر_ختم

#اگر (NGX_HTTP_UPSTREAM_ZONE)
#تعریف_کرو NGX_MODULE_SIGNATURE_33  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_33  "0"
#اگر_ختم

#اگر (NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_34  "1"
#ورنہ
#تعریف_کرو NGX_MODULE_SIGNATURE_34  "0"
#اگر_ختم

#تعریف_کرو NGX_MODULE_SIGNATURE                                                  \
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


#تعریف_کرو NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_اشاریہ, NGX_MODULE_UNSET_اشاریہ,                           \
    تہی, 0, 0, nginx_نسخہ, NGX_MODULE_SIGNATURE

#تعریف_کرو NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


ساخت ngx_module_s {
    ngx_uint_t            ctx_اشاریہ;
    ngx_uint_t            اشاریہ;

    حرف                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            نسخہ;
    ثابت حرف           *signature;

    خالی                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *لوگارتھم);

    ngx_int_t           (*init_module)(ngx_چکر_t *چکر);

    ngx_int_t           (*init_process)(ngx_چکر_t *چکر);
    ngx_int_t           (*init_thread)(ngx_چکر_t *چکر);
    خالی                (*exit_thread)(ngx_چکر_t *چکر);
    خالی                (*exit_process)(ngx_چکر_t *چکر);

    خالی                (*exit_master)(ngx_چکر_t *چکر);

    uintptr_t             فالتو_hook0;
    uintptr_t             فالتو_hook1;
    uintptr_t             فالتو_hook2;
    uintptr_t             فالتو_hook3;
    uintptr_t             فالتو_hook4;
    uintptr_t             فالتو_hook5;
    uintptr_t             فالتو_hook6;
    uintptr_t             فالتو_hook7;
};


قسمتعریف ساخت {
    ngx_str_t             name;
    خالی               *(*create_conf)(ngx_چکر_t *چکر);
    حرف               *(*init_conf)(ngx_چکر_t *چکر, خالی *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(خالی);
ngx_int_t ngx_چکر_modules(ngx_چکر_t *چکر);
ngx_int_t ngx_init_modules(ngx_چکر_t *چکر);
ngx_int_t ngx_گنتی_modules(ngx_چکر_t *چکر, ngx_uint_t type);


ngx_int_t ngx_جوڑو_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, حرف **ترتیب);


خارجی ngx_module_t  *ngx_modules[];
خارجی ngx_uint_t     ngx_max_module;

خارجی حرف          *ngx_module_نام[];


#اگر_ختم /* _NGX_MODULE_H_INCLUDED_ */
