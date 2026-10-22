
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर माप_प्रकार  ngx_data_sizes[] = {
    आकार(ngx_data_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    आकार(ngx_data_obj_t),            /* NGX_DATA_LIST_TYPE */
    आकार(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    आकार(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    आकार(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_data_item_t *
ngx_data_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_data_item_t  *item;

    item = ngx_pcalloc(pool, दुरत्व(ngx_data_item_t, data)
                             + ngx_data_sizes[type]);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    item->type = type;

    परत item;
}


शून्य
ngx_data_add_item(ngx_data_item_t *obj, ngx_str_t *name, ngx_data_item_t *item)
{
    ngx_data_obj_t  *object;

    जर (obj->type != NGX_DATA_OBJECT_TYPE && obj->type != NGX_DATA_LIST_TYPE) {
        परत;
    }

    object = &obj->data.object;

    जर (name) {
        item->name = *name;
    }

    जर (object->next == रिक्त) {
        object->next = &object->item;
    }

    *object->next = item;
    object->next = &item->next;
}


ngx_data_item_t *
ngx_data_obj_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    जर (obj == रिक्त) {
        परत रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    पर्यंत (decl->name.len) {

        item = decl->handler(decl->data, pool, ctx);
        जर (item == रिक्त) {
            परत रिक्त;
        }

        जर (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    परत obj;
}


ngx_data_item_t *
ngx_data_obj_fields_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *मूल्य;
    ngx_uint_t        i;
    ngx_data_item_t  *obj, *item;
    ngx_data_decl_t  *decl;

    obj = ngx_data_new_object(pool);
    जर (obj == रिक्त) {
        परत रिक्त;
    }

    decl = (ngx_data_decl_t *) data;

    पर्यंत (decl->name.len) {

        जर (fields) {
            मूल्य = fields->elts;

            साठी (i = 0; i < fields->nelts; i++) {

                जर (decl->name.len == मूल्य[i].len
                    && ngx_strncmp(decl->name.data, मूल्य[i].data,
                                   मूल्य[i].len)
                    == 0)
                {
                    जा found;
                }
            }

            decl++;
            सुरूठेवा;
        }

found:

        item = decl->handler(decl->data, pool, ctx);
        जर (item == रिक्त) {
            परत रिक्त;
        }

        जर (item != NGX_DATA_DECLINE) {
            ngx_data_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    परत obj;
}


ngx_data_item_t *
ngx_data_संख्या_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    item->data.integer = data;

    परत item;
}


ngx_data_item_t *
ngx_data_string_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मूल्य, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    मूल्य = &item->data.string;

    src = (ngx_str_t *) data;

    मूल्य->len = src->len;

    मूल्य->data = ngx_pstrdup(pool, src);
    जर (मूल्य->data == रिक्त) {
        परत रिक्त;
    }

    परत item;
}


ngx_data_item_t *
ngx_data_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_boolean(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    item->data.boolean = data;

    परत item;
}


ngx_data_item_t *
ngx_data_time_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    u_char      *p;
    ngx_tm_t     पंचांग;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[आकार("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) data;

    ngx_gmtime(tp->sec, &पंचांग);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    पंचांग.ngx_tm_year, पंचांग.ngx_tm_mon, पंचांग.ngx_tm_mday,
                    पंचांग.ngx_tm_hour, पंचांग.ngx_tm_min, पंचांग.ngx_tm_sec);

    जर (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.data = iso8601;
    src.len = p - iso8601;

    परत ngx_data_string_handler((uintptr_t) &src, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_int64_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    item->data.integer = *(int64_t *) ((u_char *) ctx + data);

    परत item;
}


ngx_data_item_t *
ngx_data_struct_int_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_int_t  मूल्य;

    मूल्य = *(ngx_int_t *) ((u_char *) ctx + data);

    परत ngx_data_संख्या_handler(मूल्य, pool, रिक्त);
}


ngx_data_item_t *
ngx_data_struct_atomic_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_data_item_t  *item;

    item = ngx_data_new_integer(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    item->data.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + data);

    परत item;
}


ngx_data_item_t *
ngx_data_struct_str_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_str_t        *मूल्य, *src;
    ngx_data_item_t  *item;

    item = ngx_data_new_string(pool);
    जर (item == रिक्त) {
        परत रिक्त;
    }

    मूल्य = &item->data.string;

    src = (ngx_str_t *) ((u_char *) ctx + data);

    मूल्य->len = src->len;

    मूल्य->data = ngx_pstrdup(pool, src);
    जर (मूल्य->data == रिक्त) {
        परत रिक्त;
    }

    परत item;
}


ngx_data_item_t *
ngx_data_struct_boolean_handler(uintptr_t data, ngx_pool_t *pool, शून्य *ctx)
{
    ngx_uint_t  मूल्य;

    मूल्य = *(ngx_uint_t *) ((u_char *) ctx + data);

    परत ngx_data_boolean_handler(मूल्य, pool, रिक्त);
}
