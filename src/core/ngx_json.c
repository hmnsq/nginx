
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ssize_t   (*ngx_json_درازی_pt)(ngx_داده_قلم_t *قلم);
تعريفنوع u_char   *(*ngx_json_encode_pt)(u_char *p, ngx_داده_قلم_t *قلم);


تعريفنوع بنية {
    ngx_json_درازی_pt        درازی;
    ngx_json_encode_pt        encode;
} ngx_json_encode_t;


ساكن ssize_t ngx_json_obj_درازی(ngx_داده_قلم_t *قلم);
ساكن ssize_t ngx_json_list_درازی(ngx_داده_قلم_t *قلم);
ساكن ssize_t ngx_json_string_درازی(ngx_داده_قلم_t *قلم);
ساكن ssize_t ngx_json_integer_درازی(ngx_داده_قلم_t *قلم);
ساكن ssize_t ngx_json_boolean_درازی(ngx_داده_قلم_t *قلم);
ساكن ssize_t ngx_json_null_درازی(ngx_داده_قلم_t *قلم);

ساكن u_char *ngx_json_obj_encode(u_char *p, ngx_داده_قلم_t *قلم);
ساكن u_char *ngx_json_list_encode(u_char *p, ngx_داده_قلم_t *قلم);
ساكن u_char *ngx_json_string_encode(u_char *p, ngx_داده_قلم_t *قلم);
ساكن u_char *ngx_json_integer_encode(u_char *p, ngx_داده_قلم_t *قلم);
ساكن u_char *ngx_json_boolean_encode(u_char *p, ngx_داده_قلم_t *قلم);
ساكن u_char *ngx_json_null_encode(u_char *p, ngx_داده_قلم_t *قلم);


ساكن ngx_json_encode_t  ngx_json_encode[] = {

    /* NGX_DATA_OBJECT_TYPE */
    { ngx_json_obj_درازی,
      ngx_json_obj_encode },

    /* NGX_DATA_LIST_TYPE */
    { ngx_json_list_درازی,
      ngx_json_list_encode },

    /* NGX_DATA_STRING_TYPE */
    { ngx_json_string_درازی,
      ngx_json_string_encode },

    /* NGX_DATA_INTEGER_TYPE */
    { ngx_json_integer_درازی,
      ngx_json_integer_encode },

    /* NGX_DATA_BOOLEAN_TYPE */
    { ngx_json_boolean_درازی,
      ngx_json_boolean_encode },

    /* NGX_DATA_NULL_TYPE */
    { ngx_json_null_درازی,
      ngx_json_null_encode },
};


ngx_buf_t *
ngx_json_render(ngx_pool_t *pool, ngx_داده_قلم_t *قلم)
{
    اندازه_t      درازی;
    ngx_buf_t  *buf;

    درازی = ngx_json_encode[قلم->type].درازی(قلم);
    إذا (درازی == (اندازه_t) NGX_ERROR) {
        أرجع عدم;
    }

    buf = ngx_create_موقت_buf(pool, درازی);
    إذا (buf == عدم) {
        أرجع عدم;
    }

    buf->آخرین = ngx_json_encode[قلم->type].encode(buf->آخرین, قلم);

    أرجع buf;
}


ساكن ssize_t
ngx_json_obj_درازی(ngx_داده_قلم_t *قلم)
{
    اندازه_t            len, مجموع;
    ngx_داده_obj_t   *obj;
    ngx_داده_قلم_t  *i;

    obj = &قلم->داده.object;
    مجموع = حجم("{}") - 1;
    i = obj->قلم;

    إذا (i) {
        لكل ( ;; ) {
            إذا (i->name.len > NGX_MAX_اندازه_T_مقدار - (حجم("\"\":") - 1)) {
                أرجع NGX_ERROR;
            }

            len = حجم("\"\":") - 1 + i->name.len;

            إذا (len > NGX_MAX_اندازه_T_مقدار - مجموع) {
                أرجع NGX_ERROR;
            }

            مجموع += len;

            len = ngx_escape_json(عدم, i->name.داده, i->name.len);

            إذا (len > NGX_MAX_اندازه_T_مقدار - مجموع) {
                أرجع NGX_ERROR;
            }

            مجموع += len;

            len = ngx_json_encode[i->type].درازی(i);
            إذا (len == (اندازه_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_اندازه_T_مقدار - مجموع) {
                أرجع NGX_ERROR;
            }

            مجموع += len;

            i = i->بعدی;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (مجموع > NGX_MAX_اندازه_T_مقدار - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            مجموع += حجم(",") - 1;
        }
    }

    أرجع مجموع;
}


ساكن u_char *
ngx_json_obj_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    ngx_داده_obj_t   *obj;
    ngx_داده_قلم_t  *i;

    obj = &قلم->داده.object;
    i = obj->قلم;

    *p++ = '{';

    إذا (i) {
        لكل ( ;; ) {
            *p++ = '"';

            p = (u_char *) ngx_escape_json(p, i->name.داده, i->name.len);

            *p++ = '"';
            *p++ = ':';

            p = ngx_json_encode[i->type].encode(p, i);

            i = i->بعدی;

            إذا (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = '}';

    أرجع p;
}


ساكن ssize_t
ngx_json_list_درازی(ngx_داده_قلم_t *قلم)
{
    اندازه_t            len, مجموع;
    ngx_داده_obj_t   *obj;
    ngx_داده_قلم_t  *i;

    obj = &قلم->داده.object;
    مجموع = حجم("[]") - 1;
    i = obj->قلم;

    إذا (i) {
        لكل ( ;; ) {
            len = ngx_json_encode[i->type].درازی(i);
            إذا (len == (اندازه_t) NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (len > NGX_MAX_اندازه_T_مقدار - مجموع) {
                أرجع NGX_ERROR;
            }

            مجموع += len;

            i = i->بعدی;

            إذا (i == عدم) {
                اكسر;
            }

            إذا (مجموع > NGX_MAX_اندازه_T_مقدار - (حجم(",") - 1)) {
                أرجع NGX_ERROR;
            }

            مجموع += حجم(",") - 1;
        }
    }

    أرجع مجموع;
}


ساكن u_char *
ngx_json_list_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    ngx_داده_obj_t   *obj;
    ngx_داده_قلم_t  *i;

    obj = &قلم->داده.object;
    i = obj->قلم;

    *p++ = '[';

    إذا (i) {
        لكل ( ;; ) {
            p = ngx_json_encode[i->type].encode(p, i);

            i = i->بعدی;

            إذا (i == عدم) {
                اكسر;
            }

            *p++ = ',';
        }
    }

    *p++ = ']';

    أرجع p;
}


ساكن ssize_t
ngx_json_string_درازی(ngx_داده_قلم_t *قلم)
{
    اندازه_t      len, مجموع;
    ngx_str_t  *str;

    str = &قلم->داده.string;

    إذا (str->len > NGX_MAX_اندازه_T_مقدار - (حجم("\"\"") - 1)) {
        أرجع NGX_ERROR;
    }

    مجموع = حجم("\"\"") - 1 + str->len;

    len = ngx_escape_json(عدم, str->داده, str->len);

    إذا (len > NGX_MAX_اندازه_T_مقدار - مجموع) {
        أرجع NGX_ERROR;
    }

    أرجع مجموع + len;
}


ساكن u_char *
ngx_json_string_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    ngx_str_t  *str;

    str = &قلم->داده.string;

    *p++ = '"';

    p = (u_char *) ngx_escape_json(p, str->داده, str->len);

    *p++ = '"';

    أرجع p;
}


ساكن ssize_t
ngx_json_integer_درازی(ngx_داده_قلم_t *قلم)
{
    أرجع NGX_INT64_LEN;
}


ساكن u_char *
ngx_json_integer_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    أرجع ngx_sprintf(p, "%L", قلم->داده.integer);
}


ساكن ssize_t
ngx_json_boolean_درازی(ngx_داده_قلم_t *قلم)
{
    أرجع حجم("false") - 1;
}


ساكن u_char *
ngx_json_boolean_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    أرجع قلم->داده.boolean ? ngx_cpymem(p, "true", 4)
                              : ngx_cpymem(p, "false", 5);
}


ساكن ssize_t
ngx_json_null_درازی(ngx_داده_قلم_t *قلم)
{
    أرجع حجم("null") - 1;
}


ساكن u_char *
ngx_json_null_encode(u_char *p, ngx_داده_قلم_t *قلم)
{
    أرجع ngx_cpymem(p, "null", 4);
}
