
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن اندازه_t  ngx_داده_sizes[] = {
    حجم(ngx_داده_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_داده_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_داده_قلم_t *
ngx_داده_new_قلم(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_pcalloc(pool, offsetof(ngx_داده_قلم_t, داده)
                             + ngx_داده_sizes[type]);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    قلم->type = type;

    أرجع قلم;
}


فراغ
ngx_داده_جمع_کن_قلم(ngx_داده_قلم_t *obj, ngx_str_t *name, ngx_داده_قلم_t *قلم)
{
    ngx_داده_obj_t  *object;

    إذا (obj->type != NGX_داده_OBJECT_TYPE && obj->type != NGX_داده_LIST_TYPE) {
        أرجع;
    }

    object = &obj->داده.object;

    إذا (name) {
        قلم->name = *name;
    }

    إذا (object->بعدی == عدم) {
        object->بعدی = &object->قلم;
    }

    *object->بعدی = قلم;
    object->بعدی = &قلم->بعدی;
}


ngx_داده_قلم_t *
ngx_داده_obj_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_داده_قلم_t  *obj, *قلم;
    ngx_داده_decl_t  *decl;

    obj = ngx_داده_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_داده_decl_t *) داده;

    طالما (decl->name.len) {

        قلم = decl->handler(decl->داده, pool, ctx);
        إذا (قلم == عدم) {
            أرجع عدم;
        }

        إذا (قلم != NGX_داده_DECLINE) {
            ngx_داده_جمع_کن_قلم(obj, &decl->name, قلم);
        }

        decl++;
    }

    أرجع obj;
}


ngx_داده_قلم_t *
ngx_داده_obj_fields_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *مقدار;
    ngx_uint_t        i;
    ngx_داده_قلم_t  *obj, *قلم;
    ngx_داده_decl_t  *decl;

    obj = ngx_داده_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_داده_decl_t *) داده;

    طالما (decl->name.len) {

        إذا (fields) {
            مقدار = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                إذا (decl->name.len == مقدار[i].len
                    && ngx_strncmp(decl->name.داده, مقدار[i].داده,
                                   مقدار[i].len)
                    == 0)
                {
                    اذهب یافت;
                }
            }

            decl++;
            استمر;
        }

یافت:

        قلم = decl->handler(decl->داده, pool, ctx);
        إذا (قلم == عدم) {
            أرجع عدم;
        }

        إذا (قلم != NGX_داده_DECLINE) {
            ngx_داده_جمع_کن_قلم(obj, &decl->name, قلم);
        }

        decl++;
    }

    أرجع obj;
}


ngx_داده_قلم_t *
ngx_داده_عدد_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_integer(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    قلم->داده.integer = داده;

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_string_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *مقدار, *src;
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_string(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    مقدار = &قلم->داده.string;

    src = (ngx_str_t *) داده;

    مقدار->len = src->len;

    مقدار->داده = ngx_pstrdup(pool, src);
    إذا (مقدار->داده == عدم) {
        أرجع عدم;
    }

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_boolean_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_boolean(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    قلم->داده.boolean = داده;

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_زمان_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_زمان_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_زمان_t *) داده;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    إذا (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.داده = iso8601;
    src.len = p - iso8601;

    أرجع ngx_داده_string_handler((uintptr_t) &src, pool, عدم);
}


ngx_داده_قلم_t *
ngx_داده_struct_int64_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_integer(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    قلم->داده.integer = *(int64_t *) ((u_char *) ctx + داده);

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_struct_int_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  مقدار;

    مقدار = *(ngx_int_t *) ((u_char *) ctx + داده);

    أرجع ngx_داده_عدد_handler(مقدار, pool, عدم);
}


ngx_داده_قلم_t *
ngx_داده_struct_atomic_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_integer(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    قلم->داده.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + داده);

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_struct_str_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *مقدار, *src;
    ngx_داده_قلم_t  *قلم;

    قلم = ngx_داده_new_string(pool);
    إذا (قلم == عدم) {
        أرجع عدم;
    }

    مقدار = &قلم->داده.string;

    src = (ngx_str_t *) ((u_char *) ctx + داده);

    مقدار->len = src->len;

    مقدار->داده = ngx_pstrdup(pool, src);
    إذا (مقدار->داده == عدم) {
        أرجع عدم;
    }

    أرجع قلم;
}


ngx_داده_قلم_t *
ngx_داده_struct_boolean_handler(uintptr_t داده, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  مقدار;

    مقدار = *(ngx_uint_t *) ((u_char *) ctx + داده);

    أرجع ngx_داده_boolean_handler(مقدار, pool, عدم);
}
