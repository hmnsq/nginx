
/*
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം size_t  ngx_data_sizes[] = {
    വലുപ്പം(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    വലുപ്പം(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    വലുപ്പം(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    വലുപ്പം(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    വലുപ്പം(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, offsetof(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    item->type = type;

    തിരികെ item;
}


ശൂന്യം
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    എങ്കിൽ (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        തിരികെ;
    }

    object = &obj->data.object;

    എങ്കിൽ (name) {
        item->name = *name;
    }

    എങ്കിൽ (object->next == ശൂന്യ) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    എങ്കിൽ (obj == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    decl = (ngx_data_decl_t *) data;

    ആകുമ്പോൾ (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        എങ്കിൽ (item == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    തിരികെ obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *മൂല്യം;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    എങ്കിൽ (obj == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    decl = (ngx_data_decl_t *) data;

    ആകുമ്പോൾ (decl->name.len) {

        എങ്കിൽ (fields) {
            മൂല്യം = fields->elts;

            വേണ്ടി (i = 0; i < fields->nelts; i++) {

                എങ്കിൽ (decl->name.len == മൂല്യം[i].len
                    && ngx_strncmp(decl->name.data, മൂല്യം[i].data,
                                   മൂല്യം[i].len)
                    == 0)
                {
                    പോകുക found;
                }
            }

            decl++;
            തുടരുക;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        എങ്കിൽ (item == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    തിരികെ obj;
}


ngx_data_item_t *
ngx_data_സംഖ്യ_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    item->data.integer = data;

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_str_t        *മൂല്യം, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    മൂല്യം = &item->data.string;

    src = (ngx_str_t *) data;

    മൂല്യം->len = src->len;

    മൂല്യം->data = ngx_pstrdup(pool, src);
    എങ്കിൽ (മൂല്യം->data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    item->data.boolean = data;

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    u_char      *p;
    ngx_tm_t     tm;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[വലുപ്പം("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &tm);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday,
                    tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    എങ്കിൽ (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    തിരികെ ngx_data_string_handler((uintptr_t) &src, pool, ശൂന്യ);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_int_t  മൂല്യം;

    മൂല്യം = *(ngx_int_t *) ((u_char *) ctx + data);

    തിരികെ ngx_data_സംഖ്യ_handler(മൂല്യം, pool, ശൂന്യ);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_str_t        *മൂല്യം, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    എങ്കിൽ (item == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    മൂല്യം = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    മൂല്യം->len = src->len;

    മൂല്യം->data = ngx_pstrdup(pool, src);
    എങ്കിൽ (മൂല്യം->data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, ശൂന്യം *ctx)
{
    ngx_uint_t  മൂല്യം;

    മൂല്യം = *(ngx_uint_t *) ((u_char *) ctx + data);

    തിരികെ ngx_data_boolean_handler(മൂല്യം, pool, ശൂന്യ);
}
