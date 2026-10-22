
/*
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର size_t  ngx_data_sizes[] = {
    ଆକାର(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    ଆକାର(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    ଆକାର(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    ଆକାର(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    ଆକାର(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    item->type = type;

    ଫେରନ୍ତୁ item;
}


ଶୂନ୍ୟ
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    ଯଦି (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        ଫେରନ୍ତୁ;
    }

    object = &obj->data.object;

    ଯଦି (name) {
        item->name = *name;
    }

    ଯଦି (object->next == ଖାଲି) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ଯଦି (obj == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    decl = (ngx_data_decl_t *) data;

    ଯେତେବେଳେ (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        ଯଦି (item == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ଫେରନ୍ତୁ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    ଯଦି (obj == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    decl = (ngx_data_decl_t *) data;

    ଯେତେବେଳେ (decl->name.len) {

        ଯଦି (fields) {
            ମୂଲ୍ୟ = fields->elts;

            ପାଇଁ (i = 0; i < fields->nelts; i++) {

                ଯଦି (decl->name.len == ମୂଲ୍ୟ[i].len
                    && ngx_strncmp(decl->name.data, ମୂଲ୍ୟ[i].data,
                                   ମୂଲ୍ୟ[i].len)
                    == 0)
                {
                    ଯାଅ found;
                }
            }

            decl++;
            ଜାରି;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        ଯଦି (item == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    ଫେରନ୍ତୁ obj;
}


ngx_data_item_t *
ngx_data_ସଂଖ୍ୟା_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    item->data.integer = data;

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_str_t        *ମୂଲ୍ୟ, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ମୂଲ୍ୟ = &item->data.string;

    src = (ngx_str_t *) data;

    ମୂଲ୍ୟ->len = src->len;

    ମୂଲ୍ୟ->data = ngx_pstrdup(pool, src);
    ଯଦି (ମୂଲ୍ୟ->data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    item->data.boolean = data;

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[ଆକାର("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ଯଦି (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    ଫେରନ୍ତୁ ngx_data_string_handler((uintptr_t) &src, pool, ଖାଲି);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_int_t  ମୂଲ୍ୟ;

    ମୂଲ୍ୟ = *(ngx_int_t *) ((u_char *) ctx + data);

    ଫେରନ୍ତୁ ngx_data_ସଂଖ୍ୟା_handler(ମୂଲ୍ୟ, pool, ଖାଲି);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_str_t        *ମୂଲ୍ୟ, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    ଯଦି (item == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ମୂଲ୍ୟ = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    ମୂଲ୍ୟ->len = src->len;

    ମୂଲ୍ୟ->data = ngx_pstrdup(pool, src);
    ଯଦି (ମୂଲ୍ୟ->data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, ଶୂନ୍ୟ *ctx)
{
    ngx_uint_t  ମୂଲ୍ୟ;

    ମୂଲ୍ୟ = *(ngx_uint_t *) ((u_char *) ctx + data);

    ଫେରନ୍ତୁ ngx_data_boolean_handler(ମୂଲ୍ୟ, pool, ଖାଲି);
}
