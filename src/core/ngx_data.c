
/*
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర కొలత_t  ngx_దత్తాంశం_sizes[] = {
    పరిమాణం(ngx_దత్తాంశం_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    పరిమాణం(ngx_దత్తాంశం_obj_t),            /* NGX_DATA_LIST_TYPE */
    పరిమాణం(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    పరిమాణం(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    పరిమాణం(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_new_అంశం(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_pcalloc(pool, offsetof(ngx_దత్తాంశం_అంశం_t, దత్తాంశం)
                             + ngx_దత్తాంశం_sizes[type]);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అంశం->type = type;

    తిరిగి అంశం;
}


శూన్య
ngx_దత్తాంశం_కూడు_అంశం(ngx_దత్తాంశం_అంశం_t *obj, ngx_str_t *name, ngx_దత్తాంశం_అంశం_t *అంశం)
{
    ngx_దత్తాంశం_obj_t  *object;

    ఒకవేళ (obj->type != NGX_దత్తాంశం_OBJECT_TYPE && obj->type != NGX_దత్తాంశం_LIST_TYPE) {
        తిరిగి;
    }

    object = &obj->దత్తాంశం.object;

    ఒకవేళ (name) {
        అంశం->name = *name;
    }

    ఒకవేళ (object->తదుపరి == ఖాళీ) {
        object->తదుపరి = &object->అంశం;
    }

    *object->తదుపరి = అంశం;
    object->తదుపరి = &అంశం->తదుపరి;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_obj_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_దత్తాంశం_అంశం_t  *obj, *అంశం;
    ngx_దత్తాంశం_decl_t  *decl;

    obj = ngx_దత్తాంశం_new_object(pool);
    ఒకవేళ (obj == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    decl = (ngx_దత్తాంశం_decl_t *) దత్తాంశం;

    అయితే (decl->name.len) {

        అంశం = decl->handler(decl->దత్తాంశం, pool, ctx);
        ఒకవేళ (అంశం == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (అంశం != NGX_దత్తాంశం_DECLINE) {
            ngx_దత్తాంశం_కూడు_అంశం(obj, &decl->name, అంశం);
        }

        decl++;
    }

    తిరిగి obj;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_obj_fields_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *విలువ;
    ngx_uint_t        i;
    ngx_దత్తాంశం_అంశం_t  *obj, *అంశం;
    ngx_దత్తాంశం_decl_t  *decl;

    obj = ngx_దత్తాంశం_new_object(pool);
    ఒకవేళ (obj == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    decl = (ngx_దత్తాంశం_decl_t *) దత్తాంశం;

    అయితే (decl->name.len) {

        ఒకవేళ (fields) {
            విలువ = fields->elts;

            కొరకు (i = 0; i < fields->nelts; i++) {

                ఒకవేళ (decl->name.len == విలువ[i].len
                    && ngx_strncmp(decl->name.దత్తాంశం, విలువ[i].దత్తాంశం,
                                   విలువ[i].len)
                    == 0)
                {
                    వెళ్ళు దొరికింది;
                }
            }

            decl++;
            కొనసాగు;
        }

దొరికింది:

        అంశం = decl->handler(decl->దత్తాంశం, pool, ctx);
        ఒకవేళ (అంశం == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (అంశం != NGX_దత్తాంశం_DECLINE) {
            ngx_దత్తాంశం_కూడు_అంశం(obj, &decl->name, అంశం);
        }

        decl++;
    }

    తిరిగి obj;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_సంఖ్య_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_integer(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అంశం->దత్తాంశం.integer = దత్తాంశం;

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_string_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_str_t        *విలువ, *src;
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_string(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    విలువ = &అంశం->దత్తాంశం.string;

    src = (ngx_str_t *) దత్తాంశం;

    విలువ->len = src->len;

    విలువ->దత్తాంశం = ngx_pstrdup(pool, src);
    ఒకవేళ (విలువ->దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_boolean_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_boolean(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అంశం->దత్తాంశం.boolean = దత్తాంశం;

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_సమయం_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_సమయం_t  *tp;
    u_char       iso8601[పరిమాణం("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_సమయం_t *) దత్తాంశం;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ఒకవేళ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.దత్తాంశం = iso8601;
    src.len = p - iso8601;

    తిరిగి ngx_దత్తాంశం_string_handler((uintptr_t) &src, pool, ఖాళీ);
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_struct_int64_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_integer(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అంశం->దత్తాంశం.integer = *(int64_t *) ((u_char *) ctx + దత్తాంశం);

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_struct_int_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_int_t  విలువ;

    విలువ = *(ngx_int_t *) ((u_char *) ctx + దత్తాంశం);

    తిరిగి ngx_దత్తాంశం_సంఖ్య_handler(విలువ, pool, ఖాళీ);
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_struct_atomic_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_integer(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అంశం->దత్తాంశం.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + దత్తాంశం);

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_struct_str_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_str_t        *విలువ, *src;
    ngx_దత్తాంశం_అంశం_t  *అంశం;

    అంశం = ngx_దత్తాంశం_new_string(pool);
    ఒకవేళ (అంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    విలువ = &అంశం->దత్తాంశం.string;

    src = (ngx_str_t *) ((u_char *) ctx + దత్తాంశం);

    విలువ->len = src->len;

    విలువ->దత్తాంశం = ngx_pstrdup(pool, src);
    ఒకవేళ (విలువ->దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    తిరిగి అంశం;
}


ngx_దత్తాంశం_అంశం_t *
ngx_దత్తాంశం_struct_boolean_handler(uintptr_t దత్తాంశం, ngx_pool_t *pool, శూన్య *ctx)
{
    ngx_uint_t  విలువ;

    విలువ = *(ngx_uint_t *) ((u_char *) ctx + దత్తాంశం);

    తిరిగి ngx_దత్తాంశం_boolean_handler(విలువ, pool, ఖాళీ);
}
