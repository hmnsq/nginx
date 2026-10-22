
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_MODULE_H_INCLUDED_
#घोषणा _NGX_MODULE_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <nginx.h>


#घोषणा NGX_MODULE_UNSET_सूचकांक  (ngx_uint_t) -1


#घोषणा NGX_MODULE_SIGNATURE_0                                                \
    ngx_मूल्य(NGX_PTR_आकार) ","                                               \
    ngx_मूल्य(NGX_SIG_ATOMIC_T_आकार) ","                                      \
    ngx_मूल्य(NGX_TIME_T_आकार) ","

#अगर (NGX_HAVE_KQUEUE)
#घोषणा NGX_MODULE_SIGNATURE_1   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_1   "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_2   "0"

#अगर (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_3   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_3   "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_4   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_4   "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_EVENTFD)
#घोषणा NGX_MODULE_SIGNATURE_5   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_5   "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_EPOLL)
#घोषणा NGX_MODULE_SIGNATURE_6   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_6   "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_KEEPALIVE_TUNABLE)
#घोषणा NGX_MODULE_SIGNATURE_7   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_7   "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_INET6)
#घोषणा NGX_MODULE_SIGNATURE_8   "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_8   "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_9   "1"
#घोषणा NGX_MODULE_SIGNATURE_10  "1"

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#घोषणा NGX_MODULE_SIGNATURE_11  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_11  "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_12  "1"

#अगर (NGX_HAVE_SETFIB)
#घोषणा NGX_MODULE_SIGNATURE_13  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_13  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_TCP_FASTOPEN)
#घोषणा NGX_MODULE_SIGNATURE_14  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_14  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
#घोषणा NGX_MODULE_SIGNATURE_15  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_15  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_VARIADIC_MACROS)
#घोषणा NGX_MODULE_SIGNATURE_16  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_16  "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_17  "0"

#अगर (NGX_QUIC || NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_18  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_18  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_OPENAT)
#घोषणा NGX_MODULE_SIGNATURE_19  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_19  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_ATOMIC_OPS)
#घोषणा NGX_MODULE_SIGNATURE_20  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_20  "0"
#पूर्ण_अगर

#अगर (NGX_HAVE_POSIX_SEM)
#घोषणा NGX_MODULE_SIGNATURE_21  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_21  "0"
#पूर्ण_अगर

#अगर (NGX_THREADS || NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_22  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_22  "0"
#पूर्ण_अगर

#अगर (NGX_PCRE)
#घोषणा NGX_MODULE_SIGNATURE_23  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_23  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_SSL || NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_24  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_24  "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_25  "1"

#अगर (NGX_HTTP_GZIP)
#घोषणा NGX_MODULE_SIGNATURE_26  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_26  "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE_27  "1"

#अगर (NGX_HTTP_X_FORWARDED_FOR)
#घोषणा NGX_MODULE_SIGNATURE_28  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_28  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_REALIP)
#घोषणा NGX_MODULE_SIGNATURE_29  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_29  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_HEADERS)
#घोषणा NGX_MODULE_SIGNATURE_30  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_30  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_DAV)
#घोषणा NGX_MODULE_SIGNATURE_31  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_31  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_CACHE)
#घोषणा NGX_MODULE_SIGNATURE_32  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_32  "0"
#पूर्ण_अगर

#अगर (NGX_HTTP_UPSTREAM_ZONE)
#घोषणा NGX_MODULE_SIGNATURE_33  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_33  "0"
#पूर्ण_अगर

#अगर (NGX_COMPAT)
#घोषणा NGX_MODULE_SIGNATURE_34  "1"
#अन्यथा
#घोषणा NGX_MODULE_SIGNATURE_34  "0"
#पूर्ण_अगर

#घोषणा NGX_MODULE_SIGNATURE                                                  \
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


#घोषणा NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_सूचकांक, NGX_MODULE_UNSET_सूचकांक,                           \
    शून्य, 0, 0, nginx_version, NGX_MODULE_SIGNATURE

#घोषणा NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


काष्ठा ngx_module_s {
    ngx_uint_t            ctx_सूचकांक;
    ngx_uint_t            सूचकांक;

    अक्षर                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            version;
    स्थिर अक्षर           *signature;

    व्योम                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *log);

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    व्योम                (*exit_thread)(ngx_cycle_t *cycle);
    व्योम                (*exit_process)(ngx_cycle_t *cycle);

    व्योम                (*exit_master)(ngx_cycle_t *cycle);

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};


प्रकार काष्ठा {
    ngx_str_t             name;
    व्योम               *(*create_conf)(ngx_cycle_t *cycle);
    अक्षर               *(*init_conf)(ngx_cycle_t *cycle, व्योम *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(व्योम);
ngx_int_t ngx_cycle_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_init_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_गिनती_modules(ngx_cycle_t *cycle, ngx_uint_t type);


ngx_int_t ngx_add_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, अक्षर **order);


बाह्य ngx_module_t  *ngx_modules[];
बाह्य ngx_uint_t     ngx_max_module;

बाह्य अक्षर          *ngx_module_names[];


#पूर्ण_अगर /* _NGX_MODULE_H_INCLUDED_ */
