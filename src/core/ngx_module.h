
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MODULE_H_INCLUDED_
#define _NGX_MODULE_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <nginx.h>


#define NGX_MODULE_UNSET_INDEX  (ngx_uint_t) -1


#define NGX_MODULE_SIGNATURE_0                                                \
    ngx_કિંમત(NGX_PTR_SIZE) ","                                               \
    ngx_કિંમત(NGX_SIG_ATOMIC_T_SIZE) ","                                      \
    ngx_કિંમત(NGX_TIME_T_SIZE) ","

#જો (NGX_HAVE_KQUEUE)
#define NGX_MODULE_SIGNATURE_1   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_1   "0"
#endif

#define NGX_MODULE_SIGNATURE_2   "0"

#જો (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_3   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_3   "0"
#endif

#જો (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_4   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_4   "0"
#endif

#જો (NGX_HAVE_EVENTFD)
#define NGX_MODULE_SIGNATURE_5   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_5   "0"
#endif

#જો (NGX_HAVE_EPOLL)
#define NGX_MODULE_SIGNATURE_6   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_6   "0"
#endif

#જો (NGX_HAVE_KEEPALIVE_TUNABLE)
#define NGX_MODULE_SIGNATURE_7   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_7   "0"
#endif

#જો (NGX_HAVE_INET6)
#define NGX_MODULE_SIGNATURE_8   "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_8   "0"
#endif

#define NGX_MODULE_SIGNATURE_9   "1"
#define NGX_MODULE_SIGNATURE_10  "1"

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#define NGX_MODULE_SIGNATURE_11  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_11  "0"
#endif

#define NGX_MODULE_SIGNATURE_12  "1"

#જો (NGX_HAVE_SETFIB)
#define NGX_MODULE_SIGNATURE_13  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_13  "0"
#endif

#જો (NGX_HAVE_TCP_FASTOPEN)
#define NGX_MODULE_SIGNATURE_14  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_14  "0"
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
#define NGX_MODULE_SIGNATURE_15  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_15  "0"
#endif

#જો (NGX_HAVE_VARIADIC_MACROS)
#define NGX_MODULE_SIGNATURE_16  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_16  "0"
#endif

#define NGX_MODULE_SIGNATURE_17  "0"

#જો (NGX_QUIC || NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_18  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_18  "0"
#endif

#જો (NGX_HAVE_OPENAT)
#define NGX_MODULE_SIGNATURE_19  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_19  "0"
#endif

#જો (NGX_HAVE_ATOMIC_OPS)
#define NGX_MODULE_SIGNATURE_20  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_20  "0"
#endif

#જો (NGX_HAVE_POSIX_SEM)
#define NGX_MODULE_SIGNATURE_21  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_21  "0"
#endif

#જો (NGX_THREADS || NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_22  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_22  "0"
#endif

#જો (NGX_PCRE)
#define NGX_MODULE_SIGNATURE_23  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_23  "0"
#endif

#જો (NGX_HTTP_SSL || NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_24  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_24  "0"
#endif

#define NGX_MODULE_SIGNATURE_25  "1"

#જો (NGX_HTTP_GZIP)
#define NGX_MODULE_SIGNATURE_26  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_26  "0"
#endif

#define NGX_MODULE_SIGNATURE_27  "1"

#જો (NGX_HTTP_X_FORWARDED_FOR)
#define NGX_MODULE_SIGNATURE_28  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_28  "0"
#endif

#જો (NGX_HTTP_REALIP)
#define NGX_MODULE_SIGNATURE_29  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_29  "0"
#endif

#જો (NGX_HTTP_HEADERS)
#define NGX_MODULE_SIGNATURE_30  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_30  "0"
#endif

#જો (NGX_HTTP_DAV)
#define NGX_MODULE_SIGNATURE_31  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_31  "0"
#endif

#જો (NGX_HTTP_CACHE)
#define NGX_MODULE_SIGNATURE_32  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_32  "0"
#endif

#જો (NGX_HTTP_UPSTREAM_ZONE)
#define NGX_MODULE_SIGNATURE_33  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_33  "0"
#endif

#જો (NGX_COMPAT)
#define NGX_MODULE_SIGNATURE_34  "1"
#નહીંતર
#define NGX_MODULE_SIGNATURE_34  "0"
#endif

#define NGX_MODULE_SIGNATURE                                                  \
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


#define NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_INDEX, NGX_MODULE_UNSET_INDEX,                           \
    ખાલી, 0, 0, nginx_version, NGX_MODULE_SIGNATURE

#define NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


સંરચના ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index;

    અક્ષર                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            version;
    અચલ અક્ષર           *signature;

    શૂન્ય                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *log);

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    શૂન્ય                (*exit_thread)(ngx_cycle_t *cycle);
    શૂન્ય                (*exit_process)(ngx_cycle_t *cycle);

    શૂન્ય                (*exit_master)(ngx_cycle_t *cycle);

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t             name;
    શૂન્ય               *(*create_conf)(ngx_cycle_t *cycle);
    અક્ષર               *(*init_conf)(ngx_cycle_t *cycle, શૂન્ય *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(શૂન્ય);
ngx_int_t ngx_cycle_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_init_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_ગણતરી_modules(ngx_cycle_t *cycle, ngx_uint_t type);


ngx_int_t ngx_add_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, અક્ષર **order);


બાહ્ય ngx_module_t  *ngx_modules[];
બાહ્ય ngx_uint_t     ngx_max_module;

બાહ્ય અક્ષર          *ngx_module_names[];


#endif /* _NGX_MODULE_H_INCLUDED_ */
