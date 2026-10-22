
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


स्थिर ngx_uint_t ngx_module_index(ngx_cycle_t *cycle);
स्थिर ngx_uint_t ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t index);


ngx_uint_t         ngx_max_module;
स्थिर ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(शून्य)
{
    ngx_uint_t  i;

    लागि (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * परिमाण(ngx_module_t *));
    यदि (cycle->modules == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * परिमाण(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    लागि (i = 0; cycle->modules[i]; i++) {
        यदि (cycle->modules[i]->init_module) {
            यदि (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                फर्काऊ NGX_ERROR;
            }
        }
    }

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_गन्ती_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    लागि (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        यदि (module->type != type) {
            जारी;
        }

        यदि (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            यदि (module->ctx_index > max) {
                max = module->ctx_index;
            }

            यदि (module->ctx_index == next) {
                next++;
            }

            जारी;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        यदि (module->ctx_index > max) {
            max = module->ctx_index;
        }

        next = module->ctx_index + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    यदि (cycle->old_cycle && cycle->old_cycle->modules) {

        लागि (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            यदि (module->type != type) {
                जारी;
            }

            यदि (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    फर्काऊ max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    वर्ण **order)
{
    शून्य               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    यदि (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        फर्काऊ NGX_ERROR;
    }

    यदि (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        फर्काऊ NGX_ERROR;
    }

    लागि (m = 0; cf->cycle->modules[m]; m++) {
        यदि (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            फर्काऊ NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    यदि (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        यदि (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            फर्काऊ NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    यदि (order) {
        लागि (i = 0; order[i]; i++) {
            यदि (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                रोक;
            }
        }

        लागि ( /* void */ ; order[i]; i++) {

#यदि 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            लागि (m = 0; m < before; m++) {
                यदि (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    रोक;
                }
            }
        }
    }

    /* put the module before modules[before] */

    यदि (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * परिमाण(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    यदि (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        यदि (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            यदि (rv == रिक्त) {
                फर्काऊ NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    फर्काऊ NGX_OK;
}


स्थिर ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    लागि (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        यदि (module->index == index) {
            index++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    यदि (cycle->old_cycle && cycle->old_cycle->modules) {

        लागि (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            यदि (module->index == index) {
                index++;
                गच्छ again;
            }
        }
    }

    फर्काऊ index;
}


स्थिर ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    लागि (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        यदि (module->type != type) {
            जारी;
        }

        यदि (module->ctx_index == index) {
            index++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    यदि (cycle->old_cycle && cycle->old_cycle->modules) {

        लागि (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            यदि (module->type != type) {
                जारी;
            }

            यदि (module->ctx_index == index) {
                index++;
                गच्छ again;
            }
        }
    }

    फर्काऊ index;
}
