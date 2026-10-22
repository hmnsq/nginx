
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا size_t  ngx_data_sizes[] = {
    اندازه(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    اندازه(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    اندازه(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    اندازه(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    اندازه(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    item->type = type;

    بازگشت item;
}


تهی
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    اگر (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        بازگشت;
    }

    object = &obj->data.object;

    اگر (name) {
        item->name = *name;
    }

    اگر (object->next == پوچ) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    اگر (obj == پوچ) {
        بازگشت پوچ;
    }

    decl = (ngx_data_decl_t *) data;

    وقتی (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        اگر (item == پوچ) {
            بازگشت پوچ;
        }

        اگر (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    بازگشت obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *مقدار;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    اگر (obj == پوچ) {
        بازگشت پوچ;
    }

    decl = (ngx_data_decl_t *) data;

    وقتی (decl->name.len) {

        اگر (fields) {
            مقدار = fields->elts;

            برای (i = 0; i < fields->nelts; i++) {

                اگر (decl->name.len == مقدار[i].len
                    && ngx_strncmp(decl->name.data, مقدار[i].data,
                                   مقدار[i].len)
                    == 0)
                {
                    برو found;
                }
            }

            decl++;
            ادامه;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        اگر (item == پوچ) {
            بازگشت پوچ;
        }

        اگر (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    بازگشت obj;
}


ngx_data_item_t *
ngx_data_عدد_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    item->data.integer = data;

    بازگشت item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_str_t        *مقدار, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    مقدار = &item->data.string;

    src = (ngx_str_t *) data;

    مقدار->len = src->len;

    مقدار->data = ngx_pstrdup(pool, src);
    اگر (مقدار->data == پوچ) {
        بازگشت پوچ;
    }

    بازگشت item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    item->data.boolean = data;

    بازگشت item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[اندازه("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    اگر (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    بازگشت ngx_data_string_handler((uintptr_t) &src, pool, پوچ);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    بازگشت item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_int_t  مقدار;

    مقدار = *(ngx_int_t *) ((u_char *) ctx + data);

    بازگشت ngx_data_عدد_handler(مقدار, pool, پوچ);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    بازگشت item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_str_t        *مقدار, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    اگر (item == پوچ) {
        بازگشت پوچ;
    }

    مقدار = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    مقدار->len = src->len;

    مقدار->data = ngx_pstrdup(pool, src);
    اگر (مقدار->data == پوچ) {
        بازگشت پوچ;
    }

    بازگشت item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, تهی *ctx)
{
    ngx_uint_t  مقدار;

    مقدار = *(ngx_uint_t *) ((u_char *) ctx + data);

    بازگشت ngx_data_boolean_handler(مقدار, pool, پوچ);
}
