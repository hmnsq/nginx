
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ಗಾತ್ರ_t  ngx_ದತ್ತಾಂಶ_sizes[] = {
    ಗಾತ್ರ(ngx_ದತ್ತಾಂಶ_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    ಗಾತ್ರ(ngx_ದತ್ತಾಂಶ_obj_t),            /* NGX_DATA_LIST_TYPE */
    ಗಾತ್ರ(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    ಗಾತ್ರ(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    ಗಾತ್ರ(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_new_ಅಂಶ(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_pcalloc(pool, offsetof(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t, ದತ್ತಾಂಶ)
                             + ngx_ದತ್ತಾಂಶ_sizes[type]);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಂಶ->type = type;

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ಶೂನ್ಯ
ngx_ದತ್ತಾಂಶ_ಕೂಡಿಸು_ಅಂಶ(ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *obj, ngx_str_t *name, ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *ಅಂಶ)
{
    ngx_ದತ್ತಾಂಶ_obj_t  *object;

    ಆದರೆ (obj->type != NGX_ದತ್ತಾಂಶ_OBJECT_TYPE && obj->type != NGX_ದತ್ತಾಂಶ_LIST_TYPE) {
        ಹಿಂತಿರುಗಿ;
    }

    object = &obj->ದತ್ತಾಂಶ.object;

    ಆದರೆ (name) {
        ಅಂಶ->name = *name;
    }

    ಆದರೆ (object->ಮುಂದಿನ == ಖಾಲಿ) {
        object->ಮುಂದಿನ = &object->ಅಂಶ;
    }

    *object->ಮುಂದಿನ = ಅಂಶ;
    object->ಮುಂದಿನ = &ಅಂಶ->ಮುಂದಿನ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_obj_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *obj, *ಅಂಶ;
    ngx_ದತ್ತಾಂಶ_decl_t  *decl;

    obj = ngx_ದತ್ತಾಂಶ_new_object(pool);
    ಆದರೆ (obj == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    decl = (ngx_ದತ್ತಾಂಶ_decl_t *) ದತ್ತಾಂಶ;

    ಆಗ (decl->name.len) {

        ಅಂಶ = decl->handler(decl->ದತ್ತಾಂಶ, pool, ctx);
        ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (ಅಂಶ != NGX_ದತ್ತಾಂಶ_DECLINE) {
            ngx_ದತ್ತಾಂಶ_ಕೂಡಿಸು_ಅಂಶ(obj, &decl->name, ಅಂಶ);
        }

        decl++;
    }

    ಹಿಂತಿರುಗಿ obj;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_obj_fields_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_uint_t        i;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *obj, *ಅಂಶ;
    ngx_ದತ್ತಾಂಶ_decl_t  *decl;

    obj = ngx_ದತ್ತಾಂಶ_new_object(pool);
    ಆದರೆ (obj == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    decl = (ngx_ದತ್ತಾಂಶ_decl_t *) ದತ್ತಾಂಶ;

    ಆಗ (decl->name.len) {

        ಆದರೆ (fields) {
            ಮೌಲ್ಯ = fields->elts;

            ಗಾಗಿ (i = 0; i < fields->nelts; i++) {

                ಆದರೆ (decl->name.len == ಮೌಲ್ಯ[i].len
                    && ngx_strncmp(decl->name.ದತ್ತಾಂಶ, ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ,
                                   ಮೌಲ್ಯ[i].len)
                    == 0)
                {
                    ಹೋಗು ಸಿಕ್ಕಿತು;
                }
            }

            decl++;
            ಮುಂದುವರಿ;
        }

ಸಿಕ್ಕಿತು:

        ಅಂಶ = decl->handler(decl->ದತ್ತಾಂಶ, pool, ctx);
        ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (ಅಂಶ != NGX_ದತ್ತಾಂಶ_DECLINE) {
            ngx_ದತ್ತಾಂಶ_ಕೂಡಿಸು_ಅಂಶ(obj, &decl->name, ಅಂಶ);
        }

        decl++;
    }

    ಹಿಂತಿರುಗಿ obj;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_ಸಂಖ್ಯೆ_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_integer(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಂಶ->ದತ್ತಾಂಶ.integer = ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_string_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_str_t        *ಮೌಲ್ಯ, *src;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_string(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಮೌಲ್ಯ = &ಅಂಶ->ದತ್ತಾಂಶ.string;

    src = (ngx_str_t *) ದತ್ತಾಂಶ;

    ಮೌಲ್ಯ->len = src->len;

    ಮೌಲ್ಯ->ದತ್ತಾಂಶ = ngx_pstrdup(pool, src);
    ಆದರೆ (ಮೌಲ್ಯ->ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_boolean_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_boolean(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಂಶ->ದತ್ತಾಂಶ.boolean = ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_ಸಮಯ_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_ಸಮಯ_t  *tp;
    u_char       iso8601[ಗಾತ್ರ("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_ಸಮಯ_t *) ದತ್ತಾಂಶ;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ಆದರೆ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.ದತ್ತಾಂಶ = iso8601;
    src.len = p - iso8601;

    ಹಿಂತಿರುಗಿ ngx_ದತ್ತಾಂಶ_string_handler((uintptr_t) &src, pool, ಖಾಲಿ);
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_struct_int64_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_integer(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಂಶ->ದತ್ತಾಂಶ.integer = *(int64_t *) ((u_char *) ctx + ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_struct_int_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_int_t  ಮೌಲ್ಯ;

    ಮೌಲ್ಯ = *(ngx_int_t *) ((u_char *) ctx + ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ ngx_ದತ್ತಾಂಶ_ಸಂಖ್ಯೆ_handler(ಮೌಲ್ಯ, pool, ಖಾಲಿ);
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_struct_atomic_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_integer(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಂಶ->ದತ್ತಾಂಶ.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_struct_str_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_str_t        *ಮೌಲ್ಯ, *src;
    ngx_ದತ್ತಾಂಶ_ಅಂಶ_t  *ಅಂಶ;

    ಅಂಶ = ngx_ದತ್ತಾಂಶ_new_string(pool);
    ಆದರೆ (ಅಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಮೌಲ್ಯ = &ಅಂಶ->ದತ್ತಾಂಶ.string;

    src = (ngx_str_t *) ((u_char *) ctx + ದತ್ತಾಂಶ);

    ಮೌಲ್ಯ->len = src->len;

    ಮೌಲ್ಯ->ದತ್ತಾಂಶ = ngx_pstrdup(pool, src);
    ಆದರೆ (ಮೌಲ್ಯ->ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ಅಂಶ;
}


ngx_ದತ್ತಾಂಶ_ಅಂಶ_t *
ngx_ದತ್ತಾಂಶ_struct_boolean_handler(uintptr_t ದತ್ತಾಂಶ, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_uint_t  ಮೌಲ್ಯ;

    ಮೌಲ್ಯ = *(ngx_uint_t *) ((u_char *) ctx + ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ ngx_ದತ್ತಾಂಶ_boolean_handler(ಮೌಲ್ಯ, pool, ಖಾಲಿ);
}
