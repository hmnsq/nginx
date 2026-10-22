
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


ساكن ngx_uint_t ngx_module_index(ngx_cycle_t *cycle);
ساكن ngx_uint_t ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t index);


ngx_uint_t         ngx_max_module;
ساكن ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(فراغ)
{
    ngx_uint_t  i;

    لكل (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    أرجع NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * حجم(ngx_module_t *));
    إذا (cycle->modules == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    أرجع NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    لكل (i = 0; cycle->modules[i]; i++) {
        إذا (cycle->modules[i]->init_module) {
            إذا (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                أرجع NGX_ERROR;
            }
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_كدي_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    لكل (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            إذا (module->ctx_index > max) {
                max = module->ctx_index;
            }

            إذا (module->ctx_index == next) {
                next++;
            }

            استمر;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        إذا (module->ctx_index > max) {
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

    إذا (cycle->old_cycle && cycle->old_cycle->modules) {

        لكل (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    أرجع max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **order)
{
    فراغ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    إذا (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        أرجع NGX_ERROR;
    }

    إذا (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        أرجع NGX_ERROR;
    }

    إذا (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        أرجع NGX_ERROR;
    }

    لكل (m = 0; cf->cycle->modules[m]; m++) {
        إذا (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            أرجع NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    إذا (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        إذا (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            أرجع NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    إذا (order) {
        لكل (i = 0; order[i]; i++) {
            إذا (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                اكسر;
            }
        }

        لكل ( /* void */ ; order[i]; i++) {

#إذا 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            لكل (m = 0; m < before; m++) {
                إذا (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    اكسر;
                }
            }
        }
    }

    /* put the module before modules[before] */

    إذا (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    إذا (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        إذا (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            إذا (rv == عدم) {
                أرجع NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    أرجع NGX_OK;
}


ساكن ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    لكل (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        إذا (module->index == index) {
            index++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (cycle->old_cycle && cycle->old_cycle->modules) {

        لكل (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            إذا (module->index == index) {
                index++;
                اذهب again;
            }
        }
    }

    أرجع index;
}


ساكن ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    لكل (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        إذا (module->type != type) {
            استمر;
        }

        إذا (module->ctx_index == index) {
            index++;
            اذهب again;
        }
    }

    /* check previous cycle */

    إذا (cycle->old_cycle && cycle->old_cycle->modules) {

        لكل (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            إذا (module->type != type) {
                استمر;
            }

            إذا (module->ctx_index == index) {
                index++;
                اذهب again;
            }
        }
    }

    أرجع index;
}
