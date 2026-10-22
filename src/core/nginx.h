
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGINX_H_INCLUDED_
#تعریف_کرو _NGINX_H_INCLUDED_


#تعریف_کرو nginx_نسخہ      1031009
#تعریف_کرو NGINX_نسخہ      "1.31.9"
#تعریف_کرو NGINX_VER          "nginx/" NGINX_نسخہ

#اگر_تعریف_شدہ NGX_BUILD
#تعریف_کرو NGINX_VER_BUILD    NGINX_VER " (" NGX_BUILD ")"
#ورنہ
#تعریف_کرو NGINX_VER_BUILD    NGINX_VER
#اگر_ختم

#تعریف_کرو NGINX_VAR          "NGINX"
#تعریف_کرو NGX_OLDPID_EXT     ".oldbin"


#اگر_ختم /* _NGINX_H_INCLUDED_ */
