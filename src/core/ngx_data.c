
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static size_t  ngx_data_sizes[] = {
    parimaa_na(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    parimaa_na(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    parimaa_na(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    parimaa_na(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    parimaa_na(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    item->type = type;

    ត្រឡប់ item;
}


void
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    ប្រសិនបើ (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        ត្រឡប់;
    }

    object = &obj->data.object;

    ប្រសិនបើ (name) {
        item->name = *name;
    }

    ប្រសិនបើ (object->next == NULL) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ប្រសិនបើ (obj == NULL) {
        ត្រឡប់ NULL;
    }

    decl = (ngx_data_decl_t *) data;

    ខណៈពេល (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        ប្រសិនបើ (item == NULL) {
            ត្រឡប់ NULL;
        }

        ប្រសិនបើ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ត្រឡប់ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, void *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *value;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ប្រសិនបើ (obj == NULL) {
        ត្រឡប់ NULL;
    }

    decl = (ngx_data_decl_t *) data;

    ខណៈពេល (decl->name.len) {

        ប្រសិនបើ (fields) {
            value = fields->elts;

            សម្រាប់ (i = 0; i < fields->nelts; i++) {

                ប្រសិនបើ (decl->name.len == value[i].len
                    && ngx_strncmp(decl->name.data, value[i].data,
                                   value[i].len)
                    == 0)
                {
                    goto found;
                }
            }

            decl++;
            continue;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        ប្រសិនបើ (item == NULL) {
            ត្រឡប់ NULL;
        }

        ប្រសិនបើ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ត្រឡប់ obj;
}


ngx_data_item_t *
ngx_data_number_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    item->data.integer = data;

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_str_t        *value, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    value = &item->data.string;

    src = (ngx_str_t *) data;

    value->len = src->len;

    value->data = ngx_pstrdup(pool, src);
    ប្រសិនបើ (value->data == NULL) {
        ត្រឡប់ NULL;
    }

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    item->data.boolean = data;

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[parimaa_na("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ប្រសិនបើ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    ត្រឡប់ ngx_data_string_handler((uintptr_t) &src, pool, NULL);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_int_t  value;

    value = *(ngx_int_t *) ((u_char *) ctx + data);

    ត្រឡប់ ngx_data_number_handler(value, pool, NULL);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_str_t        *value, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ប្រសិនបើ (item == NULL) {
        ត្រឡប់ NULL;
    }

    value = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    value->len = src->len;

    value->data = ngx_pstrdup(pool, src);
    ប្រសិនបើ (value->data == NULL) {
        ត្រឡប់ NULL;
    }

    ត្រឡប់ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, void *ctx)
{
    ngx_uint_t  value;

    value = *(ngx_uint_t *) ((u_char *) ctx + data);

    ត្រឡប់ ngx_data_boolean_handler(value, pool, NULL);
}
