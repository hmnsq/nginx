
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
    إذا (item == عدم) {
        أرجع عدم;
    }

    item->type = type;

    أرجع item;
}


فراغ
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    إذا (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        أرجع;
    }

    object = &obj->data.object;

    إذا (name) {
        item->name = *name;
    }

    إذا (object->next == عدم) {
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
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_data_decl_t *) data;

    طالما (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        إذا (item == عدم) {
            أرجع عدم;
        }

        إذا (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    أرجع obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *درج;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    إذا (obj == عدم) {
        أرجع عدم;
    }

    decl = (ngx_data_decl_t *) data;

    طالما (decl->name.len) {

        إذا (fields) {
            درج = fields->elts;

            لكل (i = 0; i < fields->nelts; i++) {

                إذا (decl->name.len == درج[i].len
                    && ngx_strncmp(decl->name.data, درج[i].data,
                                   درج[i].len)
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
        إذا (item == عدم) {
            أرجع عدم;
        }

        إذا (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    أرجع obj;
}


ngx_data_item_t *
ngx_data_لمب_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    item->data.integer = data;

    أرجع item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *درج, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    درج = &item->data.string;

    src = (ngx_str_t *) data;

    درج->len = src->len;

    درج->data = ngx_pstrdup(pool, src);
    إذا (درج->data == عدم) {
        أرجع عدم;
    }

    أرجع item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    item->data.boolean = data;

    أرجع item;
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

    إذا (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    أرجع ngx_data_string_handler((uintptr_t) &src, pool, عدم);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    أرجع item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_int_t  درج;

    درج = *(ngx_int_t *) ((u_char *) ctx + data);

    أرجع ngx_data_لمب_handler(درج, pool, عدم);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    أرجع item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_str_t        *درج, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    إذا (item == عدم) {
        أرجع عدم;
    }

    درج = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    درج->len = src->len;

    درج->data = ngx_pstrdup(pool, src);
    إذا (درج->data == عدم) {
        أرجع عدم;
    }

    أرجع item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, فراغ *ctx)
{
    ngx_uint_t  درج;

    درج = *(ngx_uint_t *) ((u_char *) ctx + data);

    أرجع ngx_data_boolean_handler(درج, pool, عدم);
}
