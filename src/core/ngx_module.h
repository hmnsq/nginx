
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_MODULE_H_INCLUDED_
#परिभाषय _NGX_MODULE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <nginx.h>


#परिभाषय NGX_MODULE_UNSET_सूचकांक  (ngx_uint_t) -1


#परिभाषय NGX_MODULE_SIGNATURE_0                                                \
    ngx_मानम्(NGX_PTR_आकार) ","                                               \
    ngx_मानम्(NGX_SIG_ATOMIC_T_आकार) ","                                      \
    ngx_मानम्(NGX_TIME_T_आकार) ","

#यदि (NGX_HAVE_KQUEUE)
#परिभाषय NGX_MODULE_SIGNATURE_1   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_1   "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_2   "0"

#यदि (NGX_HAVE_FILE_AIO || NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_3   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_3   "0"
#यदि_अन्तः

#यदि (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_4   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_4   "0"
#यदि_अन्तः

#यदि (NGX_HAVE_EVENTFD)
#परिभाषय NGX_MODULE_SIGNATURE_5   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_5   "0"
#यदि_अन्तः

#यदि (NGX_HAVE_EPOLL)
#परिभाषय NGX_MODULE_SIGNATURE_6   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_6   "0"
#यदि_अन्तः

#यदि (NGX_HAVE_KEEPALIVE_TUNABLE)
#परिभाषय NGX_MODULE_SIGNATURE_7   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_7   "0"
#यदि_अन्तः

#यदि (NGX_HAVE_INET6)
#परिभाषय NGX_MODULE_SIGNATURE_8   "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_8   "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_9   "1"
#परिभाषय NGX_MODULE_SIGNATURE_10  "1"

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
#परिभाषय NGX_MODULE_SIGNATURE_11  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_11  "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_12  "1"

#यदि (NGX_HAVE_SETFIB)
#परिभाषय NGX_MODULE_SIGNATURE_13  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_13  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_TCP_FASTOPEN)
#परिभाषय NGX_MODULE_SIGNATURE_14  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_14  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)
#परिभाषय NGX_MODULE_SIGNATURE_15  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_15  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_VARIADIC_MACROS)
#परिभाषय NGX_MODULE_SIGNATURE_16  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_16  "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_17  "0"

#यदि (NGX_QUIC || NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_18  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_18  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_OPENAT)
#परिभाषय NGX_MODULE_SIGNATURE_19  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_19  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_ATOMIC_OPS)
#परिभाषय NGX_MODULE_SIGNATURE_20  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_20  "0"
#यदि_अन्तः

#यदि (NGX_HAVE_POSIX_SEM)
#परिभाषय NGX_MODULE_SIGNATURE_21  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_21  "0"
#यदि_अन्तः

#यदि (NGX_तन्तवः || NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_22  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_22  "0"
#यदि_अन्तः

#यदि (NGX_PCRE)
#परिभाषय NGX_MODULE_SIGNATURE_23  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_23  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_SSL || NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_24  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_24  "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_25  "1"

#यदि (NGX_HTTP_GZIP)
#परिभाषय NGX_MODULE_SIGNATURE_26  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_26  "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE_27  "1"

#यदि (NGX_HTTP_X_FORWARDED_FOR)
#परिभाषय NGX_MODULE_SIGNATURE_28  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_28  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_REALIP)
#परिभाषय NGX_MODULE_SIGNATURE_29  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_29  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_शीर्षकाणि)
#परिभाषय NGX_MODULE_SIGNATURE_30  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_30  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_DAV)
#परिभाषय NGX_MODULE_SIGNATURE_31  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_31  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_CACHE)
#परिभाषय NGX_MODULE_SIGNATURE_32  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_32  "0"
#यदि_अन्तः

#यदि (NGX_HTTP_UPSTREAM_ZONE)
#परिभाषय NGX_MODULE_SIGNATURE_33  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_33  "0"
#यदि_अन्तः

#यदि (NGX_COMPAT)
#परिभाषय NGX_MODULE_SIGNATURE_34  "1"
#अन्यथा
#परिभाषय NGX_MODULE_SIGNATURE_34  "0"
#यदि_अन्तः

#परिभाषय NGX_MODULE_SIGNATURE                                                  \
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


#परिभाषय NGX_MODULE_V1                                                         \
    NGX_MODULE_UNSET_सूचकांक, NGX_MODULE_UNSET_सूचकांक,                           \
    रिक्त, 0, 0, nginx_संस्करणम्, NGX_MODULE_SIGNATURE

#परिभाषय NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


संरचना ngx_module_s {
    ngx_uint_t            ctx_सूचकांक;
    ngx_uint_t            सूचकांक;

    वर्ण                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            संस्करणम्;
    ध्रुव वर्ण           *signature;

    शून्य                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *लघुगणकः);

    ngx_int_t           (*init_module)(ngx_आवृत्तिः_t *आवृत्तिः);

    ngx_int_t           (*init_process)(ngx_आवृत्तिः_t *आवृत्तिः);
    ngx_int_t           (*init_thread)(ngx_आवृत्तिः_t *आवृत्तिः);
    शून्य                (*exit_thread)(ngx_आवृत्तिः_t *आवृत्तिः);
    शून्य                (*exit_process)(ngx_आवृत्तिः_t *आवृत्तिः);

    शून्य                (*exit_master)(ngx_आवृत्तिः_t *आवृत्तिः);

    uintptr_t             अतिरिक्तः_hook0;
    uintptr_t             अतिरिक्तः_hook1;
    uintptr_t             अतिरिक्तः_hook2;
    uintptr_t             अतिरिक्तः_hook3;
    uintptr_t             अतिरिक्तः_hook4;
    uintptr_t             अतिरिक्तः_hook5;
    uintptr_t             अतिरिक्तः_hook6;
    uintptr_t             अतिरिक्तः_hook7;
};


प्रकारनिर्देश संरचना {
    ngx_str_t             name;
    शून्य               *(*create_conf)(ngx_आवृत्तिः_t *आवृत्तिः);
    वर्ण               *(*init_conf)(ngx_आवृत्तिः_t *आवृत्तिः, शून्य *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(शून्य);
ngx_int_t ngx_आवृत्तिः_modules(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_init_modules(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_गणनम्_modules(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uint_t type);


ngx_int_t ngx_सङ्कलय_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, वर्ण **क्रमः);


बाह्य ngx_module_t  *ngx_modules[];
बाह्य ngx_uint_t     ngx_max_module;

बाह्य वर्ण          *ngx_module_नामानि[];


#यदि_अन्तः /* _NGX_MODULE_H_INCLUDED_ */
