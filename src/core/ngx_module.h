
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_MODULE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_MODULE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <nginx.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_UNSET_ਸੂਚਕਾਂਕ  (ngx_uint_t) -1


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_0                                                \
    ngx_ਮੁੱਲ(NGX_PTR_ਮਾਪ) ","                                               \
    ngx_ਮੁੱਲ(NGX_SIG_ATOMIC_T_ਮਾਪ) ","                                      \
    ngx_ਮੁੱਲ(NGX_ਸਮਾਂ_T_ਮਾਪ) ","

#ਜੇ (NGX_HAVE_KQUEUE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_1   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_1   "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_2   "0"

#ਜੇ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_3   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_3   "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_4   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_4   "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_EVENTFD)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_5   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_5   "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_EPOLL)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_6   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_6   "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_KEEPALIVE_TUNABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_7   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_7   "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_INET6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_8   "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_8   "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_9   "1"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_10  "1"

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_11  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_11  "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_12  "1"

#ਜੇ (NGX_HAVE_SETFIB)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_13  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_13  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_TCP_FASTOPEN)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_14  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_14  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_15  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_15  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_VARIADIC_MACROS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_16  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_16  "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_17  "0"

#ਜੇ (NGX_QUIC || NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_18  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_18  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_OPENAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_19  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_19  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_ATOMIC_OPS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_20  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_20  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_POSIX_SEM)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_21  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_21  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_ਧਾਗੇ || NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_22  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_22  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_PCRE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_23  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_23  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_SSL || NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_24  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_24  "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_25  "1"

#ਜੇ (NGX_HTTP_GZIP)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_26  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_26  "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_27  "1"

#ਜੇ (NGX_HTTP_X_FORWARDED_FOR)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_28  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_28  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_REALIP)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_29  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_29  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_ਸਿਰਲੇਖਾਂ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_30  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_30  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_DAV)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_31  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_31  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_CACHE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_32  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_32  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HTTP_UPSTREAM_ZONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_33  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_33  "0"
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_COMPAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_34  "1"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE_34  "0"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_SIGNATURE                                                  \
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


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_ਸੂਚਕਾਂਕ, NGX_MODULE_UNSET_ਸੂਚਕਾਂਕ,                           \
    ਨਿਰਾ, 0, 0, nginx_ਸੰਸਕਰਣ, NGX_MODULE_SIGNATURE

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


ਬਣਤਰ ngx_module_s {
    ngx_uint_t            ctx_ਸੂਚਕਾਂਕ;
    ngx_uint_t            ਸੂਚਕਾਂਕ;

    ਅੱਖਰ                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            ਸੰਸਕਰਣ;
    ਅਚਲ ਅੱਖਰ           *signature;

    ਖਾਲੀ                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *ਲਘੂਗਣਕ);

    ngx_int_t           (*init_module)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);

    ngx_int_t           (*init_process)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
    ngx_int_t           (*init_thread)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
    ਖਾਲੀ                (*exit_thread)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
    ਖਾਲੀ                (*exit_process)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);

    ਖਾਲੀ                (*exit_master)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);

    uintptr_t             ਵਾਧੂ_hook0;
    uintptr_t             ਵਾਧੂ_hook1;
    uintptr_t             ਵਾਧੂ_hook2;
    uintptr_t             ਵਾਧੂ_hook3;
    uintptr_t             ਵਾਧੂ_hook4;
    uintptr_t             ਵਾਧੂ_hook5;
    uintptr_t             ਵਾਧੂ_hook6;
    uintptr_t             ਵਾਧੂ_hook7;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t             name;
    ਖਾਲੀ               *(*create_conf)(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
    ਅੱਖਰ               *(*init_conf)(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(ਖਾਲੀ);
ngx_int_t ngx_ਚੱਕਰ_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_init_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_ਗਿਣਤੀ_modules(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uint_t type);


ngx_int_t ngx_ਜੋੜੋ_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, ਅੱਖਰ **ਕ੍ਰਮ);


ਬਾਹਰੀ ngx_module_t  *ngx_modules[];
ਬਾਹਰੀ ngx_uint_t     ngx_max_module;

ਬਾਹਰੀ ਅੱਖਰ          *ngx_module_ਨਾਂ[];


#ਜੇ_ਅੰਤ /* _NGX_MODULE_H_INCLUDED_ */
