
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ size_t  ngx_data_sizes[] = {
    ಪರಿಮಾಣ(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    ಪರಿಮಾಣ(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    ಪರಿಮಾಣ(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    ಪರಿಮಾಣ(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    ಪರಿಮಾಣ(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    item->type = type;

    ಪ್ರತ್ಯಾಗಮ item;
}


ಶೂನ್ಯ
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    ಯದಿ (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    object = &obj->data.object;

    ಯದಿ (name) {
        item->name = *name;
    }

    ಯದಿ (object->next == ರಿಕ್ತ) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ಯದಿ (obj == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    decl = (ngx_data_decl_t *) data;

    ಯಾವತ್ (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        ಯದಿ (item == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ಪ್ರತ್ಯಾಗಮ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ಯದಿ (obj == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    decl = (ngx_data_decl_t *) data;

    ಯಾವತ್ (decl->name.len) {

        ಯದಿ (fields) {
            ಮೌಲ್ಯ = fields->elts;

            ಪ್ರತಿ (i = 0; i < fields->nelts; i++) {

                ಯದಿ (decl->name.len == ಮೌಲ್ಯ[i].len
                    && ngx_strncmp(decl->name.data, ಮೌಲ್ಯ[i].data,
                                   ಮೌಲ್ಯ[i].len)
                    == 0)
                {
                    ಗಚ್ಛ found;
                }
            }

            decl++;
            ಅನುವರ್ತ;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        ಯದಿ (item == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ಪ್ರತ್ಯಾಗಮ obj;
}


ngx_data_item_t *
ngx_data_ಸಂಖ್ಯೆ_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    item->data.integer = data;

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_str_t        *ಮೌಲ್ಯ, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಮೌಲ್ಯ = &item->data.string;

    src = (ngx_str_t *) data;

    ಮೌಲ್ಯ->len = src->len;

    ಮೌಲ್ಯ->data = ngx_pstrdup(pool, src);
    ಯದಿ (ಮೌಲ್ಯ->data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    item->data.boolean = data;

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[ಪರಿಮಾಣ("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ಯದಿ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    ಪ್ರತ್ಯಾಗಮ ngx_data_string_handler((uintptr_t) &src, pool, ರಿಕ್ತ);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_int_t  ಮೌಲ್ಯ;

    ಮೌಲ್ಯ = *(ngx_int_t *) ((u_char *) ctx + data);

    ಪ್ರತ್ಯಾಗಮ ngx_data_ಸಂಖ್ಯೆ_handler(ಮೌಲ್ಯ, pool, ರಿಕ್ತ);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_str_t        *ಮೌಲ್ಯ, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ಯದಿ (item == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಮೌಲ್ಯ = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    ಮೌಲ್ಯ->len = src->len;

    ಮೌಲ್ಯ->data = ngx_pstrdup(pool, src);
    ಯದಿ (ಮೌಲ್ಯ->data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, ಶೂನ್ಯ *ctx)
{
    ngx_uint_t  ಮೌಲ್ಯ;

    ಮೌಲ್ಯ = *(ngx_uint_t *) ((u_char *) ctx + data);

    ಪ್ರತ್ಯಾಗಮ ngx_data_boolean_handler(ಮೌಲ್ಯ, pool, ರಿಕ್ತ);
}
