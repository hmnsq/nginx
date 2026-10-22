
/*
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான size_t  ngx_data_sizes[] = {
    அளவு(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    அளவு(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    அளவு(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    அளவு(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    அளவு(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    item->type = type;

    திரும்பு item;
}


வெறுமை
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    எனில் (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        திரும்பு;
    }

    object = &obj->data.object;

    எனில் (name) {
        item->name = *name;
    }

    எனில் (object->next == வெற்று) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    எனில் (obj == வெற்று) {
        திரும்பு வெற்று;
    }

    decl = (ngx_data_decl_t *) data;

    போது (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        எனில் (item == வெற்று) {
            திரும்பு வெற்று;
        }

        எனில் (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    திரும்பு obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *மதிப்பு;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    எனில் (obj == வெற்று) {
        திரும்பு வெற்று;
    }

    decl = (ngx_data_decl_t *) data;

    போது (decl->name.len) {

        எனில் (fields) {
            மதிப்பு = fields->elts;

            க்கு (i = 0; i < fields->nelts; i++) {

                எனில் (decl->name.len == மதிப்பு[i].len
                    && ngx_strncmp(decl->name.data, மதிப்பு[i].data,
                                   மதிப்பு[i].len)
                    == 0)
                {
                    செல் found;
                }
            }

            decl++;
            தொடர்;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        எனில் (item == வெற்று) {
            திரும்பு வெற்று;
        }

        எனில் (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    திரும்பு obj;
}


ngx_data_item_t *
ngx_data_எண்_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    item->data.integer = data;

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_str_t        *மதிப்பு, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    மதிப்பு = &item->data.string;

    src = (ngx_str_t *) data;

    மதிப்பு->len = src->len;

    மதிப்பு->data = ngx_pstrdup(pool, src);
    எனில் (மதிப்பு->data == வெற்று) {
        திரும்பு வெற்று;
    }

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    item->data.boolean = data;

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[அளவு("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    எனில் (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    திரும்பு ngx_data_string_handler((uintptr_t) &src, pool, வெற்று);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_int_t  மதிப்பு;

    மதிப்பு = *(ngx_int_t *) ((u_char *) ctx + data);

    திரும்பு ngx_data_எண்_handler(மதிப்பு, pool, வெற்று);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_str_t        *மதிப்பு, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    எனில் (item == வெற்று) {
        திரும்பு வெற்று;
    }

    மதிப்பு = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    மதிப்பு->len = src->len;

    மதிப்பு->data = ngx_pstrdup(pool, src);
    எனில் (மதிப்பு->data == வெற்று) {
        திரும்பு வெற்று;
    }

    திரும்பு item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, வெறுமை *ctx)
{
    ngx_uint_t  மதிப்பு;

    மதிப்பு = *(ngx_uint_t *) ((u_char *) ctx + data);

    திரும்பு ngx_data_boolean_handler(மதிப்பு, pool, வெற்று);
}
