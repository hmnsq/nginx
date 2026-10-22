
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_MODULE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_MODULE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <nginx.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_UNSET_ಸೂಚ್ಯಂಕ  (ngx_uint_t) -1


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_0                                                \
    ngx_ಮೌಲ್ಯ(NGX_PTR_ಗಾತ್ರ) ","                                               \
    ngx_ಮೌಲ್ಯ(NGX_SIG_ATOMIC_T_ಗಾತ್ರ) ","                                      \
    ngx_ಮೌಲ್ಯ(NGX_ಸಮಯ_T_ಗಾತ್ರ) ","

#ಆದರೆ (NGX_HAVE_KQUEUE)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_1   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_1   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_2   "0"

#ಆದರೆ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_3   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_3   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_4   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_4   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_EVENTFD)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_5   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_5   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_EPOLL)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_6   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_6   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_KEEPALIVE_TUNABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_7   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_7   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_INET6)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_8   "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_8   "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_9   "1"
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_10  "1"

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_11  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_11  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_12  "1"

#ಆದರೆ (NGX_HAVE_SETFIB)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_13  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_13  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_TCP_FASTOPEN)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_14  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_14  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_15  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_15  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_VARIADIC_MACROS)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_16  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_16  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_17  "0"

#ಆದರೆ (NGX_QUIC || NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_18  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_18  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_OPENAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_19  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_19  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_20  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_20  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_POSIX_SEM)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_21  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_21  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_ಎಳೆಗಳು || NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_22  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_22  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_PCRE)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_23  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_23  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_SSL || NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_24  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_24  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_25  "1"

#ಆದರೆ (NGX_HTTP_GZIP)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_26  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_26  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_27  "1"

#ಆದರೆ (NGX_HTTP_X_FORWARDED_FOR)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_28  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_28  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_REALIP)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_29  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_29  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_ಶೀರ್ಷಿಕೆಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_30  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_30  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_DAV)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_31  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_31  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_CACHE)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_32  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_32  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HTTP_UPSTREAM_ZONE)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_33  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_33  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_COMPAT)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_34  "1"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE_34  "0"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_SIGNATURE                                                  \
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


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_ಸೂಚ್ಯಂಕ, NGX_MODULE_UNSET_ಸೂಚ್ಯಂಕ,                           \
    ಖಾಲಿ, 0, 0, nginx_ಆವೃತ್ತಿ, NGX_MODULE_SIGNATURE

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


ರಚನೆ ngx_module_s {
    ngx_uint_t            ctx_ಸೂಚ್ಯಂಕ;
    ngx_uint_t            ಸೂಚ್ಯಂಕ;

    ಅಕ್ಷರ                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            ಆವೃತ್ತಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ           *signature;

    ಶೂನ್ಯ                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *ಲಘುಗಣಕ);

    ngx_int_t           (*init_module)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);

    ngx_int_t           (*init_process)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
    ngx_int_t           (*init_thread)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
    ಶೂನ್ಯ                (*exit_thread)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
    ಶೂನ್ಯ                (*exit_process)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);

    ಶೂನ್ಯ                (*exit_master)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);

    uintptr_t             ಹೆಚ್ಚುವರಿ_hook0;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook1;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook2;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook3;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook4;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook5;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook6;
    uintptr_t             ಹೆಚ್ಚುವರಿ_hook7;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t             name;
    ಶೂನ್ಯ               *(*create_conf)(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
    ಅಕ್ಷರ               *(*init_conf)(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(ಶೂನ್ಯ);
ngx_int_t ngx_ಚಕ್ರ_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_init_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_ಎಣಿಕೆ_modules(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uint_t type);


ngx_int_t ngx_ಕೂಡಿಸು_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, ಅಕ್ಷರ **ಕ್ರಮ);


ಬಾಹ್ಯ ngx_module_t  *ngx_modules[];
ಬಾಹ್ಯ ngx_uint_t     ngx_max_module;

ಬಾಹ್ಯ ಅಕ್ಷರ          *ngx_module_ಹೆಸರುಗಳು[];


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_MODULE_H_INCLUDED_ */
