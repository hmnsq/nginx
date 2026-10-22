
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن size_t  ngx_data_sizes[] = {
    حجم(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    حجم(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    حجم(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    حجم(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    حجم(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    item->type = type;

    ܦܢܐ item;
}


فراغ
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    ܐܢ (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        ܦܢܐ;
    }

    object = &obj->data.object;

    ܐܢ (name) {
        item->name = *name;
    }

    ܐܢ (object->next == عدم) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ܐܢ (obj == عدم) {
        ܦܢܐ عدم;
    }

    decl = (ngx_data_decl_t *) data;

    ܟܡܐ (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        ܐܢ (item == عدم) {
            ܦܢܐ عدم;
        }

        ܐܢ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ܦܢܐ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *value;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ܐܢ (obj == عدم) {
        ܦܢܐ عدم;
    }

    decl = (ngx_data_decl_t *) data;

    ܟܡܐ (decl->name.len) {

        ܐܢ (fields) {
            value = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                ܐܢ (decl->name.len == value[i].len
                    && ngx_strncmp(decl->name.data, value[i].data,
                                   value[i].len)
                    == 0)
                {
                    اذهب found;
                }
            }

            decl++;
            استمر;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        ܐܢ (item == عدم) {
            ܦܢܐ عدم;
        }

        ܐܢ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ܦܢܐ obj;
}


ngx_data_item_t *
ngx_data_number_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    item->data.integer = data;

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *value, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    value = &item->data.string;

    src = (ngx_str_t *) data;

    value->len = src->len;

    value->data = ngx_pstrdup(pool, src);
    ܐܢ (value->data == عدم) {
        ܦܢܐ عدم;
    }

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    item->data.boolean = data;

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[حجم("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ܐܢ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    ܦܢܐ ngx_data_string_handler((uintptr_t) &src, pool, عدم);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  value;

    value = *(ngx_int_t *) ((u_char *) ctx + data);

    ܦܢܐ ngx_data_number_handler(value, pool, عدم);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *value, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ܐܢ (item == عدم) {
        ܦܢܐ عدم;
    }

    value = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    value->len = src->len;

    value->data = ngx_pstrdup(pool, src);
    ܐܢ (value->data == عدم) {
        ܦܢܐ عدم;
    }

    ܦܢܐ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  value;

    value = *(ngx_uint_t *) ((u_char *) ctx + data);

    ܦܢܐ ngx_data_boolean_handler(value, pool, عدم);
}
