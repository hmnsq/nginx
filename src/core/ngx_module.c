
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


স্থির ngx_uint_t ngx_module_index(ngx_cycle_t *cycle);
স্থির ngx_uint_t ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t index);


ngx_uint_t         ngx_max_module;
স্থির ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(শূন্য)
{
    ngx_uint_t  i;

    প্রতি (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * পরিমাণ(ngx_module_t *));
    যদি (cycle->modules == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * পরিমাণ(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    প্রতি (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->init_module) {
            যদি (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                প্রত্যাগম NGX_ERROR;
            }
        }
    }

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_গুনতি_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    প্রতি (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->type != type) {
            অনুবর্ত;
        }

        যদি (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            যদি (module->ctx_index > max) {
                max = module->ctx_index;
            }

            যদি (module->ctx_index == next) {
                next++;
            }

            অনুবর্ত;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        যদি (module->ctx_index > max) {
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

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        প্রতি (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->type != type) {
                অনুবর্ত;
            }

            যদি (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    প্রত্যাগম max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    বর্ণ **order)
{
    শূন্য               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    যদি (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        প্রত্যাগম NGX_ERROR;
    }

    যদি (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        প্রত্যাগম NGX_ERROR;
    }

    যদি (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        প্রত্যাগম NGX_ERROR;
    }

    প্রতি (m = 0; cf->cycle->modules[m]; m++) {
        যদি (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            প্রত্যাগম NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    যদি (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        যদি (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            প্রত্যাগম NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    যদি (order) {
        প্রতি (i = 0; order[i]; i++) {
            যদি (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                ভঙ্গ;
            }
        }

        প্রতি ( /* void */ ; order[i]; i++) {

#যদি 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            প্রতি (m = 0; m < before; m++) {
                যদি (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    ভঙ্গ;
                }
            }
        }
    }

    /* put the module before modules[before] */

    যদি (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * পরিমাণ(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    যদি (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        যদি (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            যদি (rv == রিক্ত) {
                প্রত্যাগম NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    প্রত্যাগম NGX_OK;
}


স্থির ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    প্রতি (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->index == index) {
            index++;
            গচ্ছ again;
        }
    }

    /* check previous cycle */

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        প্রতি (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->index == index) {
                index++;
                গচ্ছ again;
            }
        }
    }

    প্রত্যাগম index;
}


স্থির ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    প্রতি (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        যদি (module->type != type) {
            অনুবর্ত;
        }

        যদি (module->ctx_index == index) {
            index++;
            গচ্ছ again;
        }
    }

    /* check previous cycle */

    যদি (cycle->old_cycle && cycle->old_cycle->modules) {

        প্রতি (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            যদি (module->type != type) {
                অনুবর্ত;
            }

            যদি (module->ctx_index == index) {
                index++;
                গচ্ছ again;
            }
        }
    }

    প্রত্যাগম index;
}
