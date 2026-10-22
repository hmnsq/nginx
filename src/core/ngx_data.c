
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ناپ_t  ngx_انکڑے_sizes[] = {
    حجم(ngx_انکڑے_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_انکڑے_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_انکڑے_شے_t *
ngx_انکڑے_new_شے(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_pcalloc(pool, offsetof(ngx_انکڑے_شے_t, انکڑے)
                             + ngx_انکڑے_sizes[type]);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    شے->type = type;

    أرجع شے;
}


فراغ
ngx_انکڑے_جوڑو_شے(ngx_انکڑے_شے_t *obj, ngx_str_t *name, ngx_انکڑے_شے_t *شے)
{
    ngx_انکڑے_obj_t  *object;

    إذا (obj->type != NGX_انکڑے_OBJECT_TYPE && obj->type != NGX_انکڑے_LIST_TYPE) {
        أرجع;
    }

    object = &obj->انکڑے.object;

    إذا (name) {
        شے->name = *name;
    }

    إذا (object->اگلا == عدم) {
        object->اگلا = &object->شے;
    }

    *object->اگلا = شے;
    object->اگلا = &شے->اگلا;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_obj_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_انکڑے_شے_t  *obj, *شے;
    ngx_انکڑے_decl_t  *decl;

    obj = ngx_انکڑے_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_انکڑے_decl_t *) انکڑے;

    طالما (decl->name.len) {

        شے = decl->handler(decl->انکڑے, pool, ctx);
        إذا (شے == عدم) {
            أرجع عدم;
        }

        إذا (شے != NGX_انکڑے_DECLINE) {
            ngx_انکڑے_جوڑو_شے(obj, &decl->name, شے);
        }

        decl++;
    }

    أرجع obj;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_obj_fields_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *قدر;
    ngx_uint_t        i;
    ngx_انکڑے_شے_t  *obj, *شے;
    ngx_انکڑے_decl_t  *decl;

    obj = ngx_انکڑے_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_انکڑے_decl_t *) انکڑے;

    طالما (decl->name.len) {

        إذا (fields) {
            قدر = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                إذا (decl->name.len == قدر[i].len
                    && ngx_strncmp(decl->name.انکڑے, قدر[i].انکڑے,
                                   قدر[i].len)
                    == 0)
                {
                    اذهب لبھیا;
                }
            }

            decl++;
            استمر;
        }

لبھیا:

        شے = decl->handler(decl->انکڑے, pool, ctx);
        إذا (شے == عدم) {
            أرجع عدم;
        }

        إذا (شے != NGX_انکڑے_DECLINE) {
            ngx_انکڑے_جوڑو_شے(obj, &decl->name, شے);
        }

        decl++;
    }

    أرجع obj;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_نمبر_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_integer(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    شے->انکڑے.integer = انکڑے;

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_string_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_string(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    قدر = &شے->انکڑے.string;

    src = (ngx_str_t *) انکڑے;

    قدر->len = src->len;

    قدر->انکڑے = ngx_pstrdup(pool, src);
    إذا (قدر->انکڑے == عدم) {
        أرجع عدم;
    }

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_boolean_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_boolean(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    شے->انکڑے.boolean = انکڑے;

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_ویلا_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_ویلا_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_ویلا_t *) انکڑے;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    إذا (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.انکڑے = iso8601;
    src.len = p - iso8601;

    أرجع ngx_انکڑے_string_handler((uintptr_t) &src, pool, عدم);
}


ngx_انکڑے_شے_t *
ngx_انکڑے_struct_int64_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_integer(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    شے->انکڑے.integer = *(int64_t *) ((u_char *) ctx + انکڑے);

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_struct_int_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  قدر;

    قدر = *(ngx_int_t *) ((u_char *) ctx + انکڑے);

    أرجع ngx_انکڑے_نمبر_handler(قدر, pool, عدم);
}


ngx_انکڑے_شے_t *
ngx_انکڑے_struct_atomic_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_integer(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    شے->انکڑے.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + انکڑے);

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_struct_str_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_انکڑے_شے_t  *شے;

    شے = ngx_انکڑے_new_string(pool);
    إذا (شے == عدم) {
        أرجع عدم;
    }

    قدر = &شے->انکڑے.string;

    src = (ngx_str_t *) ((u_char *) ctx + انکڑے);

    قدر->len = src->len;

    قدر->انکڑے = ngx_pstrdup(pool, src);
    إذا (قدر->انکڑے == عدم) {
        أرجع عدم;
    }

    أرجع شے;
}


ngx_انکڑے_شے_t *
ngx_انکڑے_struct_boolean_handler(uintptr_t انکڑے, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  قدر;

    قدر = *(ngx_uint_t *) ((u_char *) ctx + انکڑے);

    أرجع ngx_انکڑے_boolean_handler(قدر, pool, عدم);
}
