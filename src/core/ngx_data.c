
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن اندازه_t  ngx_معلومات_sizes[] = {
    حجم(ngx_معلومات_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_معلومات_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_معلومات_توکی_t *
ngx_معلومات_new_توکی(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_pcalloc(pool, offsetof(ngx_معلومات_توکی_t, معلومات)
                             + ngx_معلومات_sizes[type]);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    توکی->type = type;

    أرجع توکی;
}


فراغ
ngx_معلومات_جمع_کړه_توکی(ngx_معلومات_توکی_t *obj, ngx_str_t *name, ngx_معلومات_توکی_t *توکی)
{
    ngx_معلومات_obj_t  *object;

    إذا (obj->type != NGX_معلومات_OBJECT_TYPE && obj->type != NGX_معلومات_LIST_TYPE) {
        أرجع;
    }

    object = &obj->معلومات.object;

    إذا (name) {
        توکی->name = *name;
    }

    إذا (object->بل == عدم) {
        object->بل = &object->توکی;
    }

    *object->بل = توکی;
    object->بل = &توکی->بل;
}


ngx_معلومات_توکی_t *
ngx_معلومات_obj_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_توکی_t  *obj, *توکی;
    ngx_معلومات_decl_t  *decl;

    obj = ngx_معلومات_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_معلومات_decl_t *) معلومات;

    طالما (decl->name.len) {

        توکی = decl->handler(decl->معلومات, pool, ctx);
        إذا (توکی == عدم) {
            أرجع عدم;
        }

        إذا (توکی != NGX_معلومات_DECLINE) {
            ngx_معلومات_جمع_کړه_توکی(obj, &decl->name, توکی);
        }

        decl++;
    }

    أرجع obj;
}


ngx_معلومات_توکی_t *
ngx_معلومات_obj_fields_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *ارزښت;
    ngx_uint_t        i;
    ngx_معلومات_توکی_t  *obj, *توکی;
    ngx_معلومات_decl_t  *decl;

    obj = ngx_معلومات_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_معلومات_decl_t *) معلومات;

    طالما (decl->name.len) {

        إذا (fields) {
            ارزښت = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                إذا (decl->name.len == ارزښت[i].len
                    && ngx_strncmp(decl->name.معلومات, ارزښت[i].معلومات,
                                   ارزښت[i].len)
                    == 0)
                {
                    اذهب وموندل;
                }
            }

            decl++;
            استمر;
        }

وموندل:

        توکی = decl->handler(decl->معلومات, pool, ctx);
        إذا (توکی == عدم) {
            أرجع عدم;
        }

        إذا (توکی != NGX_معلومات_DECLINE) {
            ngx_معلومات_جمع_کړه_توکی(obj, &decl->name, توکی);
        }

        decl++;
    }

    أرجع obj;
}


ngx_معلومات_توکی_t *
ngx_معلومات_شمېره_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_integer(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    توکی->معلومات.integer = معلومات;

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_string_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *ارزښت, *src;
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_string(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    ارزښت = &توکی->معلومات.string;

    src = (ngx_str_t *) معلومات;

    ارزښت->len = src->len;

    ارزښت->معلومات = ngx_pstrdup(pool, src);
    إذا (ارزښت->معلومات == عدم) {
        أرجع عدم;
    }

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_boolean(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    توکی->معلومات.boolean = معلومات;

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_وخت_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_وخت_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_وخت_t *) معلومات;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    إذا (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.معلومات = iso8601;
    src.len = p - iso8601;

    أرجع ngx_معلومات_string_handler((uintptr_t) &src, pool, عدم);
}


ngx_معلومات_توکی_t *
ngx_معلومات_struct_int64_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_integer(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    توکی->معلومات.integer = *(int64_t *) ((u_char *) ctx + معلومات);

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_struct_int_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  ارزښت;

    ارزښت = *(ngx_int_t *) ((u_char *) ctx + معلومات);

    أرجع ngx_معلومات_شمېره_handler(ارزښت, pool, عدم);
}


ngx_معلومات_توکی_t *
ngx_معلومات_struct_atomic_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_integer(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    توکی->معلومات.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + معلومات);

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_struct_str_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *ارزښت, *src;
    ngx_معلومات_توکی_t  *توکی;

    توکی = ngx_معلومات_new_string(pool);
    إذا (توکی == عدم) {
        أرجع عدم;
    }

    ارزښت = &توکی->معلومات.string;

    src = (ngx_str_t *) ((u_char *) ctx + معلومات);

    ارزښت->len = src->len;

    ارزښت->معلومات = ngx_pstrdup(pool, src);
    إذا (ارزښت->معلومات == عدم) {
        أرجع عدم;
    }

    أرجع توکی;
}


ngx_معلومات_توکی_t *
ngx_معلومات_struct_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  ارزښت;

    ارزښت = *(ngx_uint_t *) ((u_char *) ctx + معلومات);

    أرجع ngx_معلومات_boolean_handler(ارزښت, pool, عدم);
}
