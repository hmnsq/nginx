
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर आकार_t  ngx_दत्त_sizes[] = {
    परिमाण(ngx_दत्त_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    परिमाण(ngx_दत्त_obj_t),            /* NGX_DATA_LIST_TYPE */
    परिमाण(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    परिमाण(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    परिमाण(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_दत्त_item_t *
ngx_दत्त_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_दत्त_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_दत्त_item_t, दत्त)
                             + ngx_दत्त_sizes[type]);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    item->type = type;

    प्रत्यागम item;
}


शून्य
ngx_दत्त_सङ्कलय_item(ngx_दत्त_item_t *obj, ngx_str_t *name, ngx_दत्त_item_t *item)
{
    ngx_दत्त_obj_t  *object;

    यदि (obj->type != NGX_दत्त_OBJECT_TYPE && obj->type != NGX_दत्त_LIST_TYPE) {
        प्रत्यागम;
    }

    object = &obj->दत्त.object;

    यदि (name) {
        item->name = *name;
    }

    यदि (object->अग्र == रिक्त) {
        object->अग्र = &object->item;
    }

    *object->अग्र = item;
    object->अग्र = &item->अग्र;
}


ngx_दत्त_item_t *
ngx_दत्त_obj_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_दत्त_item_t  *obj, *item;
    ngx_दत्त_decl_t  *decl;

    obj = ngx_दत्त_new_object(pool);
    यदि (obj == रिक्त) {
        प्रत्यागम रिक्त;
    }

    decl = (ngx_दत्त_decl_t *) दत्त;

    यावत् (decl->name.len) {

        item = decl->handler(decl->दत्त, pool, ctx);
        यदि (item == रिक्त) {
            प्रत्यागम रिक्त;
        }

        यदि (item != NGX_दत्त_DECLINE) {
            ngx_दत्त_सङ्कलय_item(obj, &decl->name, item);
        }

        decl++;
    }

    प्रत्यागम obj;
}


ngx_दत्त_item_t *
ngx_दत्त_obj_fields_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *मानम्;
    ngx_uint_t        i;
    ngx_दत्त_item_t  *obj, *item;
    ngx_दत्त_decl_t  *decl;

    obj = ngx_दत्त_new_object(pool);
    यदि (obj == रिक्त) {
        प्रत्यागम रिक्त;
    }

    decl = (ngx_दत्त_decl_t *) दत्त;

    यावत् (decl->name.len) {

        यदि (fields) {
            मानम् = fields->elts;

            प्रति (i = 0; i < fields->nelts; i++) {

                यदि (decl->name.len == मानम्[i].len
                    && ngx_strncmp(decl->name.दत्त, मानम्[i].दत्त,
                                   मानम्[i].len)
                    == 0)
                {
                    गच्छ प्राप्तम्;
                }
            }

            decl++;
            अनुवर्त;
        }

प्राप्तम्:

        item = decl->handler(decl->दत्त, pool, ctx);
        यदि (item == रिक्त) {
            प्रत्यागम रिक्त;
        }

        यदि (item != NGX_दत्त_DECLINE) {
            ngx_दत्त_सङ्कलय_item(obj, &decl->name, item);
        }

        decl++;
    }

    प्रत्यागम obj;
}


ngx_दत्त_item_t *
ngx_दत्त_सङ्ख्या_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_integer(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    item->दत्त.integer = दत्त;

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_string_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मानम्, *src;
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_string(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    मानम् = &item->दत्त.string;

    src = (ngx_str_t *) दत्त;

    मानम्->len = src->len;

    मानम्->दत्त = ngx_pstrdup(pool, src);
    यदि (मानम्->दत्त == रिक्त) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_boolean_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_boolean(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    item->दत्त.boolean = दत्त;

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_time_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[परिमाण("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) दत्त;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    यदि (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.दत्त = iso8601;
    src.len = p - iso8601;

    प्रत्यागम ngx_दत्त_string_handler((uintptr_t) &src, pool, रिक्त);
}


ngx_दत्त_item_t *
ngx_दत्त_struct_int64_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_integer(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    item->दत्त.integer = *(int64_t *) ((u_char *) ctx + दत्त);

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_struct_int_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_int_t  मानम्;

    मानम् = *(ngx_int_t *) ((u_char *) ctx + दत्त);

    प्रत्यागम ngx_दत्त_सङ्ख्या_handler(मानम्, pool, रिक्त);
}


ngx_दत्त_item_t *
ngx_दत्त_struct_atomic_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_integer(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    item->दत्त.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + दत्त);

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_struct_str_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मानम्, *src;
    ngx_दत्त_item_t  *item;

    item = ngx_दत्त_new_string(pool);
    यदि (item == रिक्त) {
        प्रत्यागम रिक्त;
    }

    मानम् = &item->दत्त.string;

    src = (ngx_str_t *) ((u_char *) ctx + दत्त);

    मानम्->len = src->len;

    मानम्->दत्त = ngx_pstrdup(pool, src);
    यदि (मानम्->दत्त == रिक्त) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम item;
}


ngx_दत्त_item_t *
ngx_दत्त_struct_boolean_handler(uintptr_t दत्त, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_uint_t  मानम्;

    मानम् = *(ngx_uint_t *) ((u_char *) ctx + दत्त);

    प्रत्यागम ngx_दत्त_boolean_handler(मानम्, pool, रिक्त);
}
