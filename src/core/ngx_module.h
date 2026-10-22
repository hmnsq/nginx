
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_MODULE_H_INCLUDED_
#وصف_ڪريو _NGX_MODULE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


#وصف_ڪريو NGX_MODULE_UNSET_اشاريو  (ngx_uint_t) -1


#وصف_ڪريو NGX_MODULE_SIGNATURE_0                                                \
    ngx_قدر(NGX_PTR_سائيز) ","                                               \
    ngx_قدر(NGX_SIG_ATOMIC_T_سائيز) ","                                      \
    ngx_قدر(NGX_وقت_T_سائيز) ","

#إذا (NGX_HAVE_KQUEUE)
#وصف_ڪريو NGX_MODULE_SIGNATURE_1   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_1   "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_2   "0"

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_3   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_3   "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_4   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_4   "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_EVENTFD)
#وصف_ڪريو NGX_MODULE_SIGNATURE_5   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_5   "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_EPOLL)
#وصف_ڪريو NGX_MODULE_SIGNATURE_6   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_6   "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_KEEPALIVE_TUNABLE)
#وصف_ڪريو NGX_MODULE_SIGNATURE_7   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_7   "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_INET6)
#وصف_ڪريو NGX_MODULE_SIGNATURE_8   "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_8   "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_9   "1"
#وصف_ڪريو NGX_MODULE_SIGNATURE_10  "1"

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#وصف_ڪريو NGX_MODULE_SIGNATURE_11  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_11  "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_12  "1"

#إذا (NGX_HAVE_SETFIB)
#وصف_ڪريو NGX_MODULE_SIGNATURE_13  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_13  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_TCP_FASTOPEN)
#وصف_ڪريو NGX_MODULE_SIGNATURE_14  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_14  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)
#وصف_ڪريو NGX_MODULE_SIGNATURE_15  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_15  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_VARIADIC_MACROS)
#وصف_ڪريو NGX_MODULE_SIGNATURE_16  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_16  "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_17  "0"

#إذا (NGX_QUIC || NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_18  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_18  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_OPENAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_19  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_19  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_ATOMIC_OPS)
#وصف_ڪريو NGX_MODULE_SIGNATURE_20  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_20  "0"
#جيڪڏهن_پورو

#إذا (NGX_HAVE_POSIX_SEM)
#وصف_ڪريو NGX_MODULE_SIGNATURE_21  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_21  "0"
#جيڪڏهن_پورو

#إذا (NGX_ڌاڳا || NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_22  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_22  "0"
#جيڪڏهن_پورو

#إذا (NGX_PCRE)
#وصف_ڪريو NGX_MODULE_SIGNATURE_23  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_23  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_SSL || NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_24  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_24  "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_25  "1"

#إذا (NGX_HTTP_GZIP)
#وصف_ڪريو NGX_MODULE_SIGNATURE_26  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_26  "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE_27  "1"

#إذا (NGX_HTTP_X_FORWARDED_FOR)
#وصف_ڪريو NGX_MODULE_SIGNATURE_28  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_28  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_REALIP)
#وصف_ڪريو NGX_MODULE_SIGNATURE_29  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_29  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_سرا)
#وصف_ڪريو NGX_MODULE_SIGNATURE_30  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_30  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_DAV)
#وصف_ڪريو NGX_MODULE_SIGNATURE_31  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_31  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_CACHE)
#وصف_ڪريو NGX_MODULE_SIGNATURE_32  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_32  "0"
#جيڪڏهن_پورو

#إذا (NGX_HTTP_UPSTREAM_ZONE)
#وصف_ڪريو NGX_MODULE_SIGNATURE_33  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_33  "0"
#جيڪڏهن_پورو

#إذا (NGX_COMPAT)
#وصف_ڪريو NGX_MODULE_SIGNATURE_34  "1"
#وإلا
#وصف_ڪريو NGX_MODULE_SIGNATURE_34  "0"
#جيڪڏهن_پورو

#وصف_ڪريو NGX_MODULE_SIGNATURE                                                  \
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


#وصف_ڪريو NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_اشاريو, NGX_MODULE_UNSET_اشاريو,                           \
    عدم, 0, 0, nginx_نسخو, NGX_MODULE_SIGNATURE

#وصف_ڪريو NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


بنية ngx_module_s {
    ngx_uint_t            ctx_اشاريو;
    ngx_uint_t            اشاريو;

    حرف                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            نسخو;
    ثابت حرف           *signature;

    فراغ                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *لاگرٿم);

    ngx_int_t           (*init_module)(ngx_چڪر_t *چڪر);

    ngx_int_t           (*init_process)(ngx_چڪر_t *چڪر);
    ngx_int_t           (*init_thread)(ngx_چڪر_t *چڪر);
    فراغ                (*exit_thread)(ngx_چڪر_t *چڪر);
    فراغ                (*exit_process)(ngx_چڪر_t *چڪر);

    فراغ                (*exit_master)(ngx_چڪر_t *چڪر);

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
    فراغ               *(*create_conf)(ngx_چڪر_t *چڪر);
    حرف               *(*init_conf)(ngx_چڪر_t *چڪر, فراغ *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(فراغ);
ngx_int_t ngx_چڪر_modules(ngx_چڪر_t *چڪر);
ngx_int_t ngx_init_modules(ngx_چڪر_t *چڪر);
ngx_int_t ngx_ڳڻپ_modules(ngx_چڪر_t *چڪر, ngx_uint_t type);


ngx_int_t ngx_جوڙيو_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, حرف **ترتيب);


خارجي ngx_module_t  *ngx_modules[];
خارجي ngx_uint_t     ngx_max_module;

خارجي حرف          *ngx_module_نالا[];


#جيڪڏهن_پورو /* _NGX_MODULE_H_INCLUDED_ */
