
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGINX_H_INCLUDED_
#تعریف_کن _NGINX_H_INCLUDED_


#تعریف_کن nginx_نسخه      1031009
#تعریف_کن NGINX_نسخه      "1.31.9"
#تعریف_کن NGINX_VER          "nginx/" NGINX_نسخه

#اگر_تعریف_شده NGX_BUILD
#تعریف_کن NGINX_VER_BUILD    NGINX_VER " (" NGX_BUILD ")"
#وإلا
#تعریف_کن NGINX_VER_BUILD    NGINX_VER
#پایان_اگر

#تعریف_کن NGINX_VAR          "NGINX"
#تعریف_کن NGX_OLDPID_EXT     ".oldbin"


#پایان_اگر /* _NGINX_H_INCLUDED_ */
