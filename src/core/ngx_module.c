
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

    अत्थाय (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    पटिनिवत्त NGX_OK;
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
    सचे (cycle->modules == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * परिमाण(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    अत्थाय (i = 0; cycle->modules[i]; i++) {
        सचे (cycle->modules[i]->init_module) {
            सचे (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                पटिनिवत्त NGX_ERROR;
            }
        }
    }

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_गणनम्_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    अत्थाय (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        सचे (module->type != type) {
            अनुवर्त;
        }

        सचे (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            सचे (module->ctx_index > max) {
                max = module->ctx_index;
            }

            सचे (module->ctx_index == next) {
                next++;
            }

            अनुवर्त;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        सचे (module->ctx_index > max) {
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

    सचे (cycle->old_cycle && cycle->old_cycle->modules) {

        अत्थाय (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            सचे (module->type != type) {
                अनुवर्त;
            }

            सचे (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    पटिनिवत्त max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    वर्ण **order)
{
    शून्य               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    सचे (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        पटिनिवत्त NGX_ERROR;
    }

    सचे (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        पटिनिवत्त NGX_ERROR;
    }

    अत्थाय (m = 0; cf->cycle->modules[m]; m++) {
        सचे (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            पटिनिवत्त NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    सचे (module->index == NGX_MODULE_UNSET_INDEX) {
        module->index = ngx_module_index(cf->cycle);

        सचे (module->index >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            पटिनिवत्त NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    सचे (order) {
        अत्थाय (i = 0; order[i]; i++) {
            सचे (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                भङ्ग;
            }
        }

        अत्थाय ( /* void */ ; order[i]; i++) {

#सचे 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#endif

            अत्थाय (m = 0; m < before; m++) {
                सचे (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    भङ्ग;
                }
            }
        }
    }

    /* put the module before modules[before] */

    सचे (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * परिमाण(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    सचे (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        सचे (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            सचे (rv == रिक्त) {
                पटिनिवत्त NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->index] = rv;
        }
    }

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_uint_t
ngx_module_index(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, index;
    ngx_module_t  *module;

    index = 0;

again:

    /* find an unused index */

    अत्थाय (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        सचे (module->index == index) {
            index++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    सचे (cycle->old_cycle && cycle->old_cycle->modules) {

        अत्थाय (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            सचे (module->index == index) {
                index++;
                गच्छ again;
            }
        }
    }

    पटिनिवत्त index;
}


स्थिर ngx_uint_t
ngx_module_ctx_index(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t index)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    अत्थाय (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        सचे (module->type != type) {
            अनुवर्त;
        }

        सचे (module->ctx_index == index) {
            index++;
            गच्छ again;
        }
    }

    /* check previous cycle */

    सचे (cycle->old_cycle && cycle->old_cycle->modules) {

        अत्थाय (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            सचे (module->type != type) {
                अनुवर्त;
            }

            सचे (module->ctx_index == index) {
                index++;
                गच्छ again;
            }
        }
    }

    पटिनिवत्त index;
}
