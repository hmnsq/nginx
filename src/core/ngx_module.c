
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

    עבור (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    החזר NGX_OK;
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
    אם (cycle->modules == عدم) {
        החזר NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * حجم(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    החזר NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    עבור (i = 0; cycle->modules[i]; i++) {
        אם (cycle->modules[i]->init_module) {
            אם (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                החזר NGX_ERROR;
            }
        }
    }

    החזר NGX_OK;
}


ngx_int_t
ngx_count_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    עבור (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        אם (module->type != type) {
            استمر;
        }

        אם (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            אם (module->ctx_index > max) {
                max = module->ctx_index;
            }

            אם (module->ctx_index == next) {
                next++;
            }

            استمر;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        אם (module->ctx_index > max) {
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

    אם (cycle->old_cycle && cycle->old_cycle->modules) {

        עבור (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            אם (module->type != type) {
                استمر;
            }

            אם (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    החזר max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    حرف **order)
{
    فراغ               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    אם (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        החזר NGX_ERROR;
    }

    אם (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        החזר NGX_ERROR;
    }

    אם (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        החזר NGX_ERROR;
    }

    עבור (m = 0; cf->cycle->modules[m]; m++) {
        אם (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            החזר NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    אם (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        אם (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            החזר NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    אם (order) {
        עבור (i = 0; order[i]; i++) {
            אם (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                اكسر;
            }
        }

        עבור ( /* void */ ; order[i]; i++) {

#אם 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            עבור (m = 0; m < before; m++) {
                אם (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

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

    אם (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * حجم(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    אם (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        אם (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            אם (rv == عدم) {
                החזר NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    החזר NGX_OK;
}


ساكن ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    עבור (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        אם (module->index == index) {
            index++;
            اذهب again;
        }
    }

    /* check previous cycle */

    אם (cycle->old_cycle && cycle->old_cycle->modules) {

        עבור (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            אם (module->index == index) {
                index++;
                اذهب again;
            }
        }
    }

    החזר index;
}


ساكن ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    עבור (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        אם (module->type != type) {
            استمر;
        }

        אם (module->ctx_index == index) {
            index++;
            اذهب again;
        }
    }

    /* check previous cycle */

    אם (cycle->old_cycle && cycle->old_cycle->modules) {

        עבור (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            אם (module->type != type) {
                استمر;
            }

            אם (module->ctx_index == index) {
                index++;
                اذهب again;
            }
        }
    }

    החזר index;
}
