
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_MODULE_H_INCLUDED_
#تعریف_کرو _NGX_MODULE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


#تعریف_کرو NGX_MODULE_UNSET_اشاریہ  (ngx_uint_t) -1


#تعریف_کرو NGX_MODULE_SIGNATURE_0                                                \
    ngx_قدر(NGX_PTR_ناپ) ","                                               \
    ngx_قدر(NGX_SIG_ATOMIC_T_ناپ) ","                                      \
    ngx_قدر(NGX_ویلا_T_ناپ) ","

#إذا (NGX_HAVE_KQUEUE)
#تعریف_کرو NGX_MODULE_SIGNATURE_1   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_1   "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_2   "0"

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_3   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_3   "0"
#جے_انت

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_4   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_4   "0"
#جے_انت

#إذا (NGX_HAVE_EVENTFD)
#تعریف_کرو NGX_MODULE_SIGNATURE_5   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_5   "0"
#جے_انت

#إذا (NGX_HAVE_EPOLL)
#تعریف_کرو NGX_MODULE_SIGNATURE_6   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_6   "0"
#جے_انت

#إذا (NGX_HAVE_KEEPALIVE_TUNABLE)
#تعریف_کرو NGX_MODULE_SIGNATURE_7   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_7   "0"
#جے_انت

#إذا (NGX_HAVE_INET6)
#تعریف_کرو NGX_MODULE_SIGNATURE_8   "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_8   "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_9   "1"
#تعریف_کرو NGX_MODULE_SIGNATURE_10  "1"

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#تعریف_کرو NGX_MODULE_SIGNATURE_11  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_11  "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_12  "1"

#إذا (NGX_HAVE_SETFIB)
#تعریف_کرو NGX_MODULE_SIGNATURE_13  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_13  "0"
#جے_انت

#إذا (NGX_HAVE_TCP_FASTOPEN)
#تعریف_کرو NGX_MODULE_SIGNATURE_14  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_14  "0"
#جے_انت

#إذا (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کرو NGX_MODULE_SIGNATURE_15  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_15  "0"
#جے_انت

#إذا (NGX_HAVE_VARIADIC_MACROS)
#تعریف_کرو NGX_MODULE_SIGNATURE_16  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_16  "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_17  "0"

#إذا (NGX_QUIC || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_18  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_18  "0"
#جے_انت

#إذا (NGX_HAVE_OPENAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_19  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_19  "0"
#جے_انت

#إذا (NGX_HAVE_ATOMIC_OPS)
#تعریف_کرو NGX_MODULE_SIGNATURE_20  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_20  "0"
#جے_انت

#إذا (NGX_HAVE_POSIX_SEM)
#تعریف_کرو NGX_MODULE_SIGNATURE_21  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_21  "0"
#جے_انت

#إذا (NGX_دھاگے || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_22  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_22  "0"
#جے_انت

#إذا (NGX_PCRE)
#تعریف_کرو NGX_MODULE_SIGNATURE_23  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_23  "0"
#جے_انت

#إذا (NGX_HTTP_SSL || NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_24  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_24  "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_25  "1"

#إذا (NGX_HTTP_GZIP)
#تعریف_کرو NGX_MODULE_SIGNATURE_26  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_26  "0"
#جے_انت

#تعریف_کرو NGX_MODULE_SIGNATURE_27  "1"

#إذا (NGX_HTTP_X_FORWARDED_FOR)
#تعریف_کرو NGX_MODULE_SIGNATURE_28  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_28  "0"
#جے_انت

#إذا (NGX_HTTP_REALIP)
#تعریف_کرو NGX_MODULE_SIGNATURE_29  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_29  "0"
#جے_انت

#إذا (NGX_HTTP_سرخیاں)
#تعریف_کرو NGX_MODULE_SIGNATURE_30  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_30  "0"
#جے_انت

#إذا (NGX_HTTP_DAV)
#تعریف_کرو NGX_MODULE_SIGNATURE_31  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_31  "0"
#جے_انت

#إذا (NGX_HTTP_CACHE)
#تعریف_کرو NGX_MODULE_SIGNATURE_32  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_32  "0"
#جے_انت

#إذا (NGX_HTTP_UPSTREAM_ZONE)
#تعریف_کرو NGX_MODULE_SIGNATURE_33  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_33  "0"
#جے_انت

#إذا (NGX_COMPAT)
#تعریف_کرو NGX_MODULE_SIGNATURE_34  "1"
#وإلا
#تعریف_کرو NGX_MODULE_SIGNATURE_34  "0"
#جے_انت

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
    عدم, 0, 0, nginx_نسخہ, NGX_MODULE_SIGNATURE

#تعریف_کرو NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


بنية ngx_module_s {
    ngx_uint_t            ctx_اشاریہ;
    ngx_uint_t            اشاریہ;

    حرف                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            نسخہ;
    ثابت حرف           *signature;

    فراغ                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *لوگارتھم);

    ngx_int_t           (*init_module)(ngx_چکر_t *چکر);

    ngx_int_t           (*init_process)(ngx_چکر_t *چکر);
    ngx_int_t           (*init_thread)(ngx_چکر_t *چکر);
    فراغ                (*exit_thread)(ngx_چکر_t *چکر);
    فراغ                (*exit_process)(ngx_چکر_t *چکر);

    فراغ                (*exit_master)(ngx_چکر_t *چکر);

    uintptr_t             وادھو_hook0;
    uintptr_t             وادھو_hook1;
    uintptr_t             وادھو_hook2;
    uintptr_t             وادھو_hook3;
    uintptr_t             وادھو_hook4;
    uintptr_t             وادھو_hook5;
    uintptr_t             وادھو_hook6;
    uintptr_t             وادھو_hook7;
};


تعريفنوع بنية {
    ngx_str_t             name;
    فراغ               *(*create_conf)(ngx_چکر_t *چکر);
    حرف               *(*init_conf)(ngx_چکر_t *چکر, فراغ *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(فراغ);
ngx_int_t ngx_چکر_modules(ngx_چکر_t *چکر);
ngx_int_t ngx_init_modules(ngx_چکر_t *چکر);
ngx_int_t ngx_گنتی_modules(ngx_چکر_t *چکر, ngx_uint_t type);


ngx_int_t ngx_جوڑو_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, حرف **ترتیب);


خارجي ngx_module_t  *ngx_modules[];
خارجي ngx_uint_t     ngx_max_module;

خارجي حرف          *ngx_module_ناں[];


#جے_انت /* _NGX_MODULE_H_INCLUDED_ */
