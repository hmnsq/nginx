
/*
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल माप_प्रकार  ngx_तथ्य_sizes[] = {
    माप(ngx_तथ्य_obj_t),            /* NGX_DATA_OBJECT_TYPE */
    माप(ngx_तथ्य_obj_t),            /* NGX_DATA_LIST_TYPE */
    माप(ngx_str_t),                 /* NGX_DATA_STRING_TYPE */
    माप(int64_t),                   /* NGX_DATA_INTEGER_TYPE */
    माप(ngx_uint_t),                /* NGX_DATA_BOOLEAN_TYPE */
    0,                                 /* NGX_DATA_NULL_TYPE */
};


ngx_तथ्य_item_t *
ngx_तथ्य_new_item(ngx_pool_t *pool, ngx_uint_t type)
{
    ngx_तथ्य_item_t  *item;

    item = ngx_pcalloc(pool, दुरत्व(ngx_तथ्य_item_t, तथ्य)
                             + ngx_तथ्य_sizes[type]);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    item->type = type;

    वापस item;
}


व्योम
ngx_तथ्य_add_item(ngx_तथ्य_item_t *obj, ngx_str_t *name, ngx_तथ्य_item_t *item)
{
    ngx_तथ्य_obj_t  *object;

    अगर (obj->type != NGX_तथ्य_OBJECT_TYPE && obj->type != NGX_तथ्य_LIST_TYPE) {
        वापस;
    }

    object = &obj->तथ्य.object;

    अगर (name) {
        item->name = *name;
    }

    अगर (object->अग्र == शून्य) {
        object->अग्र = &object->item;
    }

    *object->अग्र = item;
    object->अग्र = &item->अग्र;
}


ngx_तथ्य_item_t *
ngx_तथ्य_obj_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_तथ्य_item_t  *obj, *item;
    ngx_तथ्य_decl_t  *decl;

    obj = ngx_तथ्य_new_object(pool);
    अगर (obj == शून्य) {
        वापस शून्य;
    }

    decl = (ngx_तथ्य_decl_t *) तथ्य;

    जबतक (decl->name.len) {

        item = decl->handler(decl->तथ्य, pool, ctx);
        अगर (item == शून्य) {
            वापस शून्य;
        }

        अगर (item != NGX_तथ्य_DECLINE) {
            ngx_तथ्य_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    वापस obj;
}


ngx_तथ्य_item_t *
ngx_तथ्य_obj_fields_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx,
    ngx_array_t *fields)
{
    ngx_str_t        *मूल्य;
    ngx_uint_t        i;
    ngx_तथ्य_item_t  *obj, *item;
    ngx_तथ्य_decl_t  *decl;

    obj = ngx_तथ्य_new_object(pool);
    अगर (obj == शून्य) {
        वापस शून्य;
    }

    decl = (ngx_तथ्य_decl_t *) तथ्य;

    जबतक (decl->name.len) {

        अगर (fields) {
            मूल्य = fields->elts;

            क्रम (i = 0; i < fields->nelts; i++) {

                अगर (decl->name.len == मूल्य[i].len
                    && ngx_strncmp(decl->name.तथ्य, मूल्य[i].तथ्य,
                                   मूल्य[i].len)
                    == 0)
                {
                    जाओ found;
                }
            }

            decl++;
            जारी;
        }

found:

        item = decl->handler(decl->तथ्य, pool, ctx);
        अगर (item == शून्य) {
            वापस शून्य;
        }

        अगर (item != NGX_तथ्य_DECLINE) {
            ngx_तथ्य_add_item(obj, &decl->name, item);
        }

        decl++;
    }

    वापस obj;
}


ngx_तथ्य_item_t *
ngx_तथ्य_संख्या_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_integer(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    item->तथ्य.integer = तथ्य;

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_string_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_str_t        *मूल्य, *src;
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_string(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    मूल्य = &item->तथ्य.string;

    src = (ngx_str_t *) तथ्य;

    मूल्य->len = src->len;

    मूल्य->तथ्य = ngx_pstrdup(pool, src);
    अगर (मूल्य->तथ्य == शून्य) {
        वापस शून्य;
    }

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_boolean_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_boolean(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    item->तथ्य.boolean = तथ्य;

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_time_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    u_char      *p;
    ngx_tm_t     पंचांग;
    ngx_str_t    src;
    ngx_time_t  *tp;
    u_char       iso8601[माप("1974-01-19T13:00:00.000Z") - 1];

    tp = (ngx_time_t *) तथ्य;

    ngx_gmtime(tp->sec, &पंचांग);

    p = ngx_sprintf(iso8601, "%4d-%02d-%02dT%02d:%02d:%02d",
                    पंचांग.ngx_tm_year, पंचांग.ngx_tm_mon, पंचांग.ngx_tm_mday,
                    पंचांग.ngx_tm_hour, पंचांग.ngx_tm_min, पंचांग.ngx_tm_sec);

    अगर (tp->msec) {
        p = ngx_sprintf(p, ".%03ui", tp->msec);
    }

    *p++ = 'Z';

    src.तथ्य = iso8601;
    src.len = p - iso8601;

    वापस ngx_तथ्य_string_handler((uintptr_t) &src, pool, शून्य);
}


ngx_तथ्य_item_t *
ngx_तथ्य_struct_int64_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_integer(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    item->तथ्य.integer = *(int64_t *) ((u_char *) ctx + तथ्य);

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_struct_int_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_int_t  मूल्य;

    मूल्य = *(ngx_int_t *) ((u_char *) ctx + तथ्य);

    वापस ngx_तथ्य_संख्या_handler(मूल्य, pool, शून्य);
}


ngx_तथ्य_item_t *
ngx_तथ्य_struct_atomic_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_integer(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    item->तथ्य.integer = *(ngx_atomic_uint_t *) ((u_char *) ctx + तथ्य);

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_struct_str_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_str_t        *मूल्य, *src;
    ngx_तथ्य_item_t  *item;

    item = ngx_तथ्य_new_string(pool);
    अगर (item == शून्य) {
        वापस शून्य;
    }

    मूल्य = &item->तथ्य.string;

    src = (ngx_str_t *) ((u_char *) ctx + तथ्य);

    मूल्य->len = src->len;

    मूल्य->तथ्य = ngx_pstrdup(pool, src);
    अगर (मूल्य->तथ्य == शून्य) {
        वापस शून्य;
    }

    वापस item;
}


ngx_तथ्य_item_t *
ngx_तथ्य_struct_boolean_handler(uintptr_t तथ्य, ngx_pool_t *pool, व्योम *ctx)
{
    ngx_uint_t  मूल्य;

    मूल्य = *(ngx_uint_t *) ((u_char *) ctx + तथ्य);

    वापस ngx_तथ्य_boolean_handler(मूल्य, pool, शून्य);
}
