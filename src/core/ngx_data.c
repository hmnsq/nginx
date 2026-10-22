
/*
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ਮਾਪ_t  ngx_ਅੰਕੜੇ_sizes[] = {
    ਆਕਾਰ(ngx_ਅੰਕੜੇ_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    ਆਕਾਰ(ngx_ਅੰਕੜੇ_obj_t),            /* NGX_DATA_LIST_TYPE */
    ਆਕਾਰ(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    ਆਕਾਰ(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    ਆਕਾਰ(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_new_ਵਸਤੂ(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_pcalloc(pool, offsetof(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t, ਅੰਕੜੇ)
                             + ngx_ਅੰਕੜੇ_sizes[type]);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਸਤੂ->type = type;

    ਵਾਪਸ ਵਸਤੂ;
}


ਖਾਲੀ
ngx_ਅੰਕੜੇ_ਜੋੜੋ_ਵਸਤੂ(ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *obj, ngx_str_t *name, ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *ਵਸਤੂ)
{
    ngx_ਅੰਕੜੇ_obj_t  *object;

    ਜੇ (obj->type != NGX_ਅੰਕੜੇ_OBJECT_TYPE && obj->type != NGX_ਅੰਕੜੇ_LIST_TYPE) {
        ਵਾਪਸ;
    }

    object = &obj->ਅੰਕੜੇ.object;

    ਜੇ (name) {
        ਵਸਤੂ->name = *name;
    }

    ਜੇ (object->ਅਗਲਾ == ਨਿਰਾ) {
        object->ਅਗਲਾ = &object->ਵਸਤੂ;
    }

    *object->ਅਗਲਾ = ਵਸਤੂ;
    object->ਅਗਲਾ = &ਵਸਤੂ->ਅਗਲਾ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_obj_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *obj, *ਵਸਤੂ;
    ngx_ਅੰਕੜੇ_decl_t  *decl;

    obj = ngx_ਅੰਕੜੇ_new_object(pool);
    ਜੇ (obj == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    decl = (ngx_ਅੰਕੜੇ_decl_t *) ਅੰਕੜੇ;

    ਜਦੋਂ (decl->name.len) {

        ਵਸਤੂ = decl->handler(decl->ਅੰਕੜੇ, pool, ctx);
        ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (ਵਸਤੂ != NGX_ਅੰਕੜੇ_DECLINE) {
            ngx_ਅੰਕੜੇ_ਜੋੜੋ_ਵਸਤੂ(obj, &decl->name, ਵਸਤੂ);
        }

        decl++;
    }

    ਵਾਪਸ obj;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_obj_fields_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *ਮੁੱਲ;
    ngx_uint_t        i;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *obj, *ਵਸਤੂ;
    ngx_ਅੰਕੜੇ_decl_t  *decl;

    obj = ngx_ਅੰਕੜੇ_new_object(pool);
    ਜੇ (obj == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    decl = (ngx_ਅੰਕੜੇ_decl_t *) ਅੰਕੜੇ;

    ਜਦੋਂ (decl->name.len) {

        ਜੇ (fields) {
            ਮੁੱਲ = fields->elts;

            ਲਈ (i = 0; i < fields->nelts; i++) {

                ਜੇ (decl->name.len == ਮੁੱਲ[i].len
                    && ngx_strncmp(decl->name.ਅੰਕੜੇ, ਮੁੱਲ[i].ਅੰਕੜੇ,
                                   ਮੁੱਲ[i].len)
                    == 0)
                {
                    ਜਾਓ ਮਿਲਿਆ;
                }
            }

            decl++;
            ਜਾਰੀ;
        }

ਮਿਲਿਆ:

        ਵਸਤੂ = decl->handler(decl->ਅੰਕੜੇ, pool, ctx);
        ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (ਵਸਤੂ != NGX_ਅੰਕੜੇ_DECLINE) {
            ngx_ਅੰਕੜੇ_ਜੋੜੋ_ਵਸਤੂ(obj, &decl->name, ਵਸਤੂ);
        }

        decl++;
    }

    ਵਾਪਸ obj;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_ਸੰਖਿਆ_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_integer(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਸਤੂ->ਅੰਕੜੇ.integer = ਅੰਕੜੇ;

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_string_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_str_t        *ਮੁੱਲ, *src;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_string(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਮੁੱਲ = &ਵਸਤੂ->ਅੰਕੜੇ.string;

    src = (ngx_str_t *) ਅੰਕੜੇ;

    ਮੁੱਲ->len = src->len;

    ਮੁੱਲ->ਅੰਕੜੇ = ngx_pstrdup(pool, src);
    ਜੇ (ਮੁੱਲ->ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_boolean_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_boolean(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਸਤੂ->ਅੰਕੜੇ.boolean = ਅੰਕੜੇ;

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_ਸਮਾਂ_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_ਸਮਾਂ_t  *tp;
    u_char       iso8601[ਆਕਾਰ("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_ਸਮਾਂ_t *) ਅੰਕੜੇ;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ਜੇ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.ਅੰਕੜੇ = iso8601;
    src.len = p - iso8601;

    ਵਾਪਸ ngx_ਅੰਕੜੇ_string_handler((uintptr_t) &src, pool, ਨਿਰਾ);
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_struct_int64_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_integer(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਸਤੂ->ਅੰਕੜੇ.integer = *(int64_t *) ((u_char *) ctx + ਅੰਕੜੇ);

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_struct_int_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_int_t  ਮੁੱਲ;

    ਮੁੱਲ = *(ngx_int_t *) ((u_char *) ctx + ਅੰਕੜੇ);

    ਵਾਪਸ ngx_ਅੰਕੜੇ_ਸੰਖਿਆ_handler(ਮੁੱਲ, pool, ਨਿਰਾ);
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_struct_atomic_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_integer(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਸਤੂ->ਅੰਕੜੇ.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + ਅੰਕੜੇ);

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_struct_str_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_str_t        *ਮੁੱਲ, *src;
    ngx_ਅੰਕੜੇ_ਵਸਤੂ_t  *ਵਸਤੂ;

    ਵਸਤੂ = ngx_ਅੰਕੜੇ_new_string(pool);
    ਜੇ (ਵਸਤੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਮੁੱਲ = &ਵਸਤੂ->ਅੰਕੜੇ.string;

    src = (ngx_str_t *) ((u_char *) ctx + ਅੰਕੜੇ);

    ਮੁੱਲ->len = src->len;

    ਮੁੱਲ->ਅੰਕੜੇ = ngx_pstrdup(pool, src);
    ਜੇ (ਮੁੱਲ->ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ਵਸਤੂ;
}


ngx_ਅੰਕੜੇ_ਵਸਤੂ_t *
ngx_ਅੰਕੜੇ_struct_boolean_handler(uintptr_t ਅੰਕੜੇ, ngx_pool_t *pool, ਖਾਲੀ *ctx)
{
    ngx_uint_t  ਮੁੱਲ;

    ਮੁੱਲ = *(ngx_uint_t *) ((u_char *) ctx + ਅੰਕੜੇ);

    ਵਾਪਸ ngx_ਅੰਕੜੇ_boolean_handler(ਮੁੱਲ, pool, ਨਿਰਾ);
}
