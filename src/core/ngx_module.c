
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#define NGX_MAX_DYNAMIC_MODULES  128


സ്ഥിതം ngx_uint_t ngx_module_index(ngx_cycle_t *cycle);
സ്ഥിതം ngx_uint_t ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t index);


ngx_uint_t         ngx_max_module;
സ്ഥിതം ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(ശൂന്യം)
{
    ngx_uint_t  i;

    വേണ്ടി (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * വലുപ്പം(ngx_module_t *));
    എങ്കിൽ (cycle->modules == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * വലുപ്പം(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    വേണ്ടി (i = 0; cycle->modules[i]; i++) {
        എങ്കിൽ (cycle->modules[i]->init_module) {
            എങ്കിൽ (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                തിരികെ NGX_ERROR;
            }
        }
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_എണ്ണം_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    വേണ്ടി (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        എങ്കിൽ (module->type != type) {
            തുടരുക;
        }

        എങ്കിൽ (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            എങ്കിൽ (module->ctx_index > max) {
                max = module->ctx_index;
            }

            എങ്കിൽ (module->ctx_index == next) {
                next++;
            }

            തുടരുക;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        എങ്കിൽ (module->ctx_index > max) {
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

    എങ്കിൽ (cycle->old_cycle && cycle->old_cycle->modules) {

        വേണ്ടി (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            എങ്കിൽ (module->type != type) {
                തുടരുക;
            }

            എങ്കിൽ (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    തിരികെ max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    അക്ഷരം **order)
{
    ശൂന്യം               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    എങ്കിൽ (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (m = 0; cf->cycle->modules[m]; m++) {
        എങ്കിൽ (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            തിരികെ NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    എങ്കിൽ (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        എങ്കിൽ (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            തിരികെ NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    എങ്കിൽ (order) {
        വേണ്ടി (i = 0; order[i]; i++) {
            എങ്കിൽ (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                നിർത്തുക;
            }
        }

        വേണ്ടി ( /* void */ ; order[i]; i++) {

#എങ്കിൽ 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            വേണ്ടി (m = 0; m < before; m++) {
                എങ്കിൽ (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    നിർത്തുക;
                }
            }
        }
    }

    /* put the module before modules[before] */

    എങ്കിൽ (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * വലുപ്പം(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    എങ്കിൽ (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        എങ്കിൽ (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            എങ്കിൽ (rv == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    വേണ്ടി (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        എങ്കിൽ (module->index == index) {
            index++;
            പോകുക again;
        }
    }

    /* check previous cycle */

    എങ്കിൽ (cycle->old_cycle && cycle->old_cycle->modules) {

        വേണ്ടി (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            എങ്കിൽ (module->index == index) {
                index++;
                പോകുക again;
            }
        }
    }

    തിരികെ index;
}


സ്ഥിതം ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    വേണ്ടി (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        എങ്കിൽ (module->type != type) {
            തുടരുക;
        }

        എങ്കിൽ (module->ctx_index == index) {
            index++;
            പോകുക again;
        }
    }

    /* check previous cycle */

    എങ്കിൽ (cycle->old_cycle && cycle->old_cycle->modules) {

        വേണ്ടി (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            എങ്കിൽ (module->type != type) {
                തുടരുക;
            }

            എങ്കിൽ (module->ctx_index == index) {
                index++;
                പോകുക again;
            }
        }
    }

    തിരികെ index;
}
