
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

    དོན་ལུ (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    ལོག NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * parimaa_na(ngx_module_t *));
    གལ་སྲིད (cycle->modules == NULL) {
        ལོག NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * parimaa_na(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    ལོག NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    དོན་ལུ (i = 0; cycle->modules[i]; i++) {
        གལ་སྲིད (cycle->modules[i]->init_module) {
            གལ་སྲིད (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                ལོག NGX_ERROR;
            }
        }
    }

    ལོག NGX_OK;
}


ngx_int_t
ngx_count_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    དོན་ལུ (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        གལ་སྲིད (module->type != type) {
            continue;
        }

        གལ་སྲིད (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            གལ་སྲིད (module->ctx_index > max) {
                max = module->ctx_index;
            }

            གལ་སྲིད (module->ctx_index == next) {
                next++;
            }

            continue;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        གལ་སྲིད (module->ctx_index > max) {
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

    གལ་སྲིད (cycle->old_cycle && cycle->old_cycle->modules) {

        དོན་ལུ (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            གལ་སྲིད (module->type != type) {
                continue;
            }

            གལ་སྲིད (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    ལོག max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    var_na **order)
{
    void               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    གལ་སྲིད (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        ལོག NGX_ERROR;
    }

    དོན་ལུ (m = 0; cf->cycle->modules[m]; m++) {
        གལ་སྲིད (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            ལོག NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    གལ་སྲིད (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        གལ་སྲིད (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            ལོག NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    གལ་སྲིད (order) {
        དོན་ལུ (i = 0; order[i]; i++) {
            གལ་སྲིད (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                bha_nga;
            }
        }

        དོན་ལུ ( /* void */ ; order[i]; i++) {

#གལ་སྲིད 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            དོན་ལུ (m = 0; m < before; m++) {
                གལ་སྲིད (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    bha_nga;
                }
            }
        }
    }

    /* put the module before modules[before] */

    གལ་སྲིད (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * parimaa_na(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    གལ་སྲིད (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        གལ་སྲིད (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            གལ་སྲིད (rv == NULL) {
                ལོག NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    ལོག NGX_OK;
}


static ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    དོན་ལུ (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        གལ་སྲིད (module->index == index) {
            index++;
            goto again;
        }
    }

    /* check previous cycle */

    གལ་སྲིད (cycle->old_cycle && cycle->old_cycle->modules) {

        དོན་ལུ (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            གལ་སྲིད (module->index == index) {
                index++;
                goto again;
            }
        }
    }

    ལོག index;
}


static ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    དོན་ལུ (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        གལ་སྲིད (module->type != type) {
            continue;
        }

        གལ་སྲིད (module->ctx_index == index) {
            index++;
            goto again;
        }
    }

    /* check previous cycle */

    གལ་སྲིད (cycle->old_cycle && cycle->old_cycle->modules) {

        དོན་ལུ (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            གལ་སྲིད (module->type != type) {
                continue;
            }

            གལ་སྲིད (module->ctx_index == index) {
                index++;
                goto again;
            }
        }
    }

    ལོག index;
}
