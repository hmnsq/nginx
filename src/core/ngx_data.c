
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن سائيز_t  ngx_معلومات_sizes[] = {
    حجم(ngx_معلومات_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_معلومات_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_معلومات_شي_t *
ngx_معلومات_new_شي(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_معلومات_شي_t  *شي;

    شي = ngx_pcalloc(pool, offsetof(ngx_معلومات_شي_t, معلومات)
                             + ngx_معلومات_sizes[type]);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    شي->type = type;

    أرجع شي;
}


فراغ
ngx_معلومات_جوڙيو_شي(ngx_معلومات_شي_t *obj, ngx_str_t *name, ngx_معلومات_شي_t *شي)
{
    ngx_معلومات_obj_t  *object;

    إذا (obj->type != NGX_معلومات_OBJECT_TYPE && obj->type != NGX_معلومات_LIST_TYPE) {
        أرجع;
    }

    object = &obj->معلومات.object;

    إذا (name) {
        شي->name = *name;
    }

    إذا (object->ايندڙ == عدم) {
        object->ايندڙ = &object->شي;
    }

    *object->ايندڙ = شي;
    object->ايندڙ = &شي->ايندڙ;
}


ngx_معلومات_شي_t *
ngx_معلومات_obj_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_شي_t  *obj, *شي;
    ngx_معلومات_decl_t  *decl;

    obj = ngx_معلومات_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_معلومات_decl_t *) معلومات;

    طالما (decl->name.len) {

        شي = decl->handler(decl->معلومات, pool, ctx);
        إذا (شي == عدم) {
            أرجع عدم;
        }

        إذا (شي != NGX_معلومات_DECLINE) {
            ngx_معلومات_جوڙيو_شي(obj, &decl->name, شي);
        }

        decl++;
    }

    أرجع obj;
}


ngx_معلومات_شي_t *
ngx_معلومات_obj_fields_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *قدر;
    ngx_uint_t        i;
    ngx_معلومات_شي_t  *obj, *شي;
    ngx_معلومات_decl_t  *decl;

    obj = ngx_معلومات_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_معلومات_decl_t *) معلومات;

    طالما (decl->name.len) {

        إذا (fields) {
            قدر = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                إذا (decl->name.len == قدر[i].len
                    && ngx_strncmp(decl->name.معلومات, قدر[i].معلومات,
                                   قدر[i].len)
                    == 0)
                {
                    اذهب مليو;
                }
            }

            decl++;
            استمر;
        }

مليو:

        شي = decl->handler(decl->معلومات, pool, ctx);
        إذا (شي == عدم) {
            أرجع عدم;
        }

        إذا (شي != NGX_معلومات_DECLINE) {
            ngx_معلومات_جوڙيو_شي(obj, &decl->name, شي);
        }

        decl++;
    }

    أرجع obj;
}


ngx_معلومات_شي_t *
ngx_معلومات_عدد_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_integer(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    شي->معلومات.integer = معلومات;

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_string_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_string(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    قدر = &شي->معلومات.string;

    src = (ngx_str_t *) معلومات;

    قدر->len = src->len;

    قدر->معلومات = ngx_pstrdup(pool, src);
    إذا (قدر->معلومات == عدم) {
        أرجع عدم;
    }

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_boolean(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    شي->معلومات.boolean = معلومات;

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_وقت_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_وقت_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_وقت_t *) معلومات;

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


ngx_معلومات_شي_t *
ngx_معلومات_struct_int64_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_integer(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    شي->معلومات.integer = *(int64_t *) ((u_char *) ctx + معلومات);

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_struct_int_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  قدر;

    قدر = *(ngx_int_t *) ((u_char *) ctx + معلومات);

    أرجع ngx_معلومات_عدد_handler(قدر, pool, عدم);
}


ngx_معلومات_شي_t *
ngx_معلومات_struct_atomic_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_integer(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    شي->معلومات.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + معلومات);

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_struct_str_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_معلومات_شي_t  *شي;

    شي = ngx_معلومات_new_string(pool);
    إذا (شي == عدم) {
        أرجع عدم;
    }

    قدر = &شي->معلومات.string;

    src = (ngx_str_t *) ((u_char *) ctx + معلومات);

    قدر->len = src->len;

    قدر->معلومات = ngx_pstrdup(pool, src);
    إذا (قدر->معلومات == عدم) {
        أرجع عدم;
    }

    أرجع شي;
}


ngx_معلومات_شي_t *
ngx_معلومات_struct_boolean_handler(uintptr_t معلومات, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  قدر;

    قدر = *(ngx_uint_t *) ((u_char *) ctx + معلومات);

    أرجع ngx_معلومات_boolean_handler(قدر, pool, عدم);
}
