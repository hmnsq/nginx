
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن سائز_t  ngx_ڈیٹا_sizes[] = {
    حجم(ngx_ڈیٹا_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_ڈیٹا_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_new_شے(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_pcalloc(pool, offsetof(ngx_ڈیٹا_شے_t, ڈیٹا)
                             + ngx_ڈیٹا_sizes[type]);
    اگر (شے == تہی) {
        واپس تہی;
    }

    شے->type = type;

    واپس شے;
}


خالی
ngx_ڈیٹا_جوڑو_شے(ngx_ڈیٹا_شے_t *obj, ngx_str_t *name, ngx_ڈیٹا_شے_t *شے)
{
    ngx_ڈیٹا_obj_t  *object;

    اگر (obj->type != NGX_ڈیٹا_OBJECT_TYPE && obj->type != NGX_ڈیٹا_LIST_TYPE) {
        واپس;
    }

    object = &obj->ڈیٹا.object;

    اگر (name) {
        شے->name = *name;
    }

    اگر (object->اگلا == تہی) {
        object->اگلا = &object->شے;
    }

    *object->اگلا = شے;
    object->اگلا = &شے->اگلا;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_obj_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_ڈیٹا_شے_t  *obj, *شے;
    ngx_ڈیٹا_decl_t  *decl;

    obj = ngx_ڈیٹا_new_object(pool);
    اگر (obj == تہی) {
        واپس تہی;
    }

    decl = (ngx_ڈیٹا_decl_t *) ڈیٹا;

    جب (decl->name.len) {

        شے = decl->handler(decl->ڈیٹا, pool, ctx);
        اگر (شے == تہی) {
            واپس تہی;
        }

        اگر (شے != NGX_ڈیٹا_DECLINE) {
            ngx_ڈیٹا_جوڑو_شے(obj, &decl->name, شے);
        }

        decl++;
    }

    واپس obj;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_obj_fields_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *قدر;
    ngx_uint_t        i;
    ngx_ڈیٹا_شے_t  *obj, *شے;
    ngx_ڈیٹا_decl_t  *decl;

    obj = ngx_ڈیٹا_new_object(pool);
    اگر (obj == تہی) {
        واپس تہی;
    }

    decl = (ngx_ڈیٹا_decl_t *) ڈیٹا;

    جب (decl->name.len) {

        اگر (fields) {
            قدر = fields->elts;

            برائے (i = 0; i < fields->nelts; i++) {

                اگر (decl->name.len == قدر[i].len
                    && ngx_strncmp(decl->name.ڈیٹا, قدر[i].ڈیٹا,
                                   قدر[i].len)
                    == 0)
                {
                    جاؤ ملا;
                }
            }

            decl++;
            جاری;
        }

ملا:

        شے = decl->handler(decl->ڈیٹا, pool, ctx);
        اگر (شے == تہی) {
            واپس تہی;
        }

        اگر (شے != NGX_ڈیٹا_DECLINE) {
            ngx_ڈیٹا_جوڑو_شے(obj, &decl->name, شے);
        }

        decl++;
    }

    واپس obj;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_نمبر_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_integer(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    شے->ڈیٹا.integer = ڈیٹا;

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_string_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_string(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    قدر = &شے->ڈیٹا.string;

    src = (ngx_str_t *) ڈیٹا;

    قدر->len = src->len;

    قدر->ڈیٹا = ngx_pstrdup(pool, src);
    اگر (قدر->ڈیٹا == تہی) {
        واپس تہی;
    }

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_boolean_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_boolean(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    شے->ڈیٹا.boolean = ڈیٹا;

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_وقت_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_وقت_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_وقت_t *) ڈیٹا;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    اگر (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.ڈیٹا = iso8601;
    src.len = p - iso8601;

    واپس ngx_ڈیٹا_string_handler((uintptr_t) &src, pool, تہی);
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_struct_int64_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_integer(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    شے->ڈیٹا.integer = *(int64_t *) ((u_char *) ctx + ڈیٹا);

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_struct_int_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_int_t  قدر;

    قدر = *(ngx_int_t *) ((u_char *) ctx + ڈیٹا);

    واپس ngx_ڈیٹا_نمبر_handler(قدر, pool, تہی);
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_struct_atomic_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_integer(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    شے->ڈیٹا.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + ڈیٹا);

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_struct_str_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_str_t        *قدر, *src;
    ngx_ڈیٹا_شے_t  *شے;

    شے = ngx_ڈیٹا_new_string(pool);
    اگر (شے == تہی) {
        واپس تہی;
    }

    قدر = &شے->ڈیٹا.string;

    src = (ngx_str_t *) ((u_char *) ctx + ڈیٹا);

    قدر->len = src->len;

    قدر->ڈیٹا = ngx_pstrdup(pool, src);
    اگر (قدر->ڈیٹا == تہی) {
        واپس تہی;
    }

    واپس شے;
}


ngx_ڈیٹا_شے_t *
ngx_ڈیٹا_struct_boolean_handler(uintptr_t ڈیٹا, ngx_pool_t *pool, خالی *ctx)
{
    ngx_uint_t  قدر;

    قدر = *(ngx_uint_t *) ((u_char *) ctx + ڈیٹا);

    واپس ngx_ڈیٹا_boolean_handler(قدر, pool, تہی);
}
