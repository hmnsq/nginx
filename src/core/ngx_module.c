
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


static ngx_uint_t ngx_module_index(ngx_cycle_t *cycle);
static ngx_uint_t ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t index);


ngx_uint_t         ngx_max_module;
static ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(void)
{
    ngx_uint_t  i;

    为 (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    返回 NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * sizeof(ngx_module_t *));
    如果 (cycle->modules == NULL) {
        返回 NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * sizeof(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    返回 NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    为 (i = 0; cycle->modules[i]; i++) {
        如果 (cycle->modules[i]->init_module) {
            如果 (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                返回 NGX_ERROR;
            }
        }
    }

    返回 NGX_OK;
}


ngx_int_t
ngx_count_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    为 (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        如果 (module->type != type) {
            continue;
        }

        如果 (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            如果 (module->ctx_index > max) {
                max = module->ctx_index;
            }

            如果 (module->ctx_index == next) {
                next++;
            }

            continue;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        如果 (module->ctx_index > max) {
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

    如果 (cycle->old_cycle && cycle->old_cycle->modules) {

        为 (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            如果 (module->type != type) {
                continue;
            }

            如果 (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    返回 max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    char **order)
{
    void               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    如果 (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        返回 NGX_ERROR;
    }

    如果 (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        返回 NGX_ERROR;
    }

    如果 (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        返回 NGX_ERROR;
    }

    为 (m = 0; cf->cycle->modules[m]; m++) {
        如果 (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            返回 NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    如果 (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        如果 (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            返回 NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    如果 (order) {
        为 (i = 0; order[i]; i++) {
            如果 (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                break;
            }
        }

        为 ( /* void */ ; order[i]; i++) {

#如果 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            为 (m = 0; m < before; m++) {
                如果 (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    break;
                }
            }
        }
    }

    /* put the module before modules[before] */

    如果 (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * sizeof(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    如果 (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        如果 (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            如果 (rv == NULL) {
                返回 NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    返回 NGX_OK;
}


static ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    为 (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        如果 (module->index == index) {
            index++;
            goto again;
        }
    }

    /* check previous cycle */

    如果 (cycle->old_cycle && cycle->old_cycle->modules) {

        为 (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            如果 (module->index == index) {
                index++;
                goto again;
            }
        }
    }

    返回 index;
}


static ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    为 (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        如果 (module->type != type) {
            continue;
        }

        如果 (module->ctx_index == index) {
            index++;
            goto again;
        }
    }

    /* check previous cycle */

    如果 (cycle->old_cycle && cycle->old_cycle->modules) {

        为 (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            如果 (module->type != type) {
                continue;
            }

            如果 (module->ctx_index == index) {
                index++;
                goto again;
            }
        }
    }

    返回 index;
}
