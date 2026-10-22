
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির size_t  ngx_data_sizes[] = {
    পরিমাণ(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    পরিমাণ(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    পরিমাণ(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    পরিমাণ(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    পরিমাণ(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    item->type = type;

    প্রত্যাগম item;
}


শূন্য
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    যদি (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        প্রত্যাগম;
    }

    object = &obj->data.object;

    যদি (name) {
        item->name = *name;
    }

    যদি (object->next == রিক্ত) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    যদি (obj == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    decl = (ngx_data_decl_t *) data;

    যাবত্ (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        যদি (item == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        যদি (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    প্রত্যাগম obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *মান;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    যদি (obj == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    decl = (ngx_data_decl_t *) data;

    যাবত্ (decl->name.len) {

        যদি (fields) {
            মান = fields->elts;

            প্রতি (i = 0; i < fields->nelts; i++) {

                যদি (decl->name.len == মান[i].len
                    && ngx_strncmp(decl->name.data, মান[i].data,
                                   মান[i].len)
                    == 0)
                {
                    গচ্ছ found;
                }
            }

            decl++;
            অনুবর্ত;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        যদি (item == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

        যদি (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    প্রত্যাগম obj;
}


ngx_data_item_t *
ngx_data_সংখ্যা_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    item->data.integer = data;

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_str_t        *মান, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    মান = &item->data.string;

    src = (ngx_str_t *) data;

    মান->len = src->len;

    মান->data = ngx_pstrdup(pool, src);
    যদি (মান->data == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    item->data.boolean = data;

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[পরিমাণ("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    যদি (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    প্রত্যাগম ngx_data_string_handler((uintptr_t) &src, pool, রিক্ত);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_int_t  মান;

    মান = *(ngx_int_t *) ((u_char *) ctx + data);

    প্রত্যাগম ngx_data_সংখ্যা_handler(মান, pool, রিক্ত);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_str_t        *মান, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    যদি (item == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    মান = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    মান->len = src->len;

    মান->data = ngx_pstrdup(pool, src);
    যদি (মান->data == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, শূন্য *ctx)
{
    ngx_uint_t  মান;

    মান = *(ngx_uint_t *) ((u_char *) ctx + data);

    প্রত্যাগম ngx_data_boolean_handler(মান, pool, রিক্ত);
}
