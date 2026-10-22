
/*
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર size_t  ngx_data_sizes[] = {
    કદ(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    કદ(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    કદ(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    કદ(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    કદ(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    item->type = type;

    પાછા item;
}


શૂન્ય
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    જો (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        પાછા;
    }

    object = &obj->data.object;

    જો (name) {
        item->name = *name;
    }

    જો (object->next == ખાલી) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    જો (obj == ખાલી) {
        પાછા ખાલી;
    }

    decl = (ngx_data_decl_t *) data;

    જ્યારે (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        જો (item == ખાલી) {
            પાછા ખાલી;
        }

        જો (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    પાછા obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *કિંમત;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    જો (obj == ખાલી) {
        પાછા ખાલી;
    }

    decl = (ngx_data_decl_t *) data;

    જ્યારે (decl->name.len) {

        જો (fields) {
            કિંમત = fields->elts;

            માટે (i = 0; i < fields->nelts; i++) {

                જો (decl->name.len == કિંમત[i].len
                    && ngx_strncmp(decl->name.data, કિંમત[i].data,
                                   કિંમત[i].len)
                    == 0)
                {
                    જાઓ found;
                }
            }

            decl++;
            ચાલુ;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        જો (item == ખાલી) {
            પાછા ખાલી;
        }

        જો (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    પાછા obj;
}


ngx_data_item_t *
ngx_data_સંખ્યા_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    item->data.integer = data;

    પાછા item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_str_t        *કિંમત, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    કિંમત = &item->data.string;

    src = (ngx_str_t *) data;

    કિંમત->len = src->len;

    કિંમત->data = ngx_pstrdup(pool, src);
    જો (કિંમત->data == ખાલી) {
        પાછા ખાલી;
    }

    પાછા item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    item->data.boolean = data;

    પાછા item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[કદ("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    જો (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    પાછા ngx_data_string_handler((uintptr_t) &src, pool, ખાલી);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    પાછા item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_int_t  કિંમત;

    કિંમત = *(ngx_int_t *) ((u_char *) ctx + data);

    પાછા ngx_data_સંખ્યા_handler(કિંમત, pool, ખાલી);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    પાછા item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_str_t        *કિંમત, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    જો (item == ખાલી) {
        પાછા ખાલી;
    }

    કિંમત = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    કિંમત->len = src->len;

    કિંમત->data = ngx_pstrdup(pool, src);
    જો (કિંમત->data == ખાલી) {
        પાછા ખાલી;
    }

    પાછા item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, શૂન્ય *ctx)
{
    ngx_uint_t  કિંમત;

    કિંમત = *(ngx_uint_t *) ((u_char *) ctx + data);

    પાછા ngx_data_boolean_handler(કિંમત, pool, ખાલી);
}
