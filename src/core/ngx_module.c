
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_MAX_DYNAMIC_MODULES  128


अटल ngx_uint_t ngx_module_सूचकांक(ngx_cycle_t *cycle);
अटल ngx_uint_t ngx_module_ctx_सूचकांक(ngx_cycle_t *cycle, ngx_uint_t type,
    ngx_uint_t सूचकांक);


ngx_uint_t         ngx_max_module;
अटल ngx_uint_t  ngx_modules_n;


ngx_int_t
ngx_preinit_modules(व्योम)
{
    ngx_uint_t  i;

    क्रम (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->सूचकांक = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    वापस NGX_OK;
}


ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * माप(ngx_module_t *));
    अगर (cycle->modules == शून्य) {
        वापस NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * माप(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    वापस NGX_OK;
}


ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    क्रम (i = 0; cycle->modules[i]; i++) {
        अगर (cycle->modules[i]->init_module) {
            अगर (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                वापस NGX_ERROR;
            }
        }
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_गिनती_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, अग्र, max;
    ngx_module_t  *module;

    अग्र = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    क्रम (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        अगर (module->type != type) {
            जारी;
        }

        अगर (module->ctx_सूचकांक != NGX_MODULE_UNSET_सूचकांक) {

            /* if ctx_index was assigned, preserve it */

            अगर (module->ctx_सूचकांक > max) {
                max = module->ctx_सूचकांक;
            }

            अगर (module->ctx_सूचकांक == अग्र) {
                अग्र++;
            }

            जारी;
        }

        /* search for some free index */

        module->ctx_सूचकांक = ngx_module_ctx_सूचकांक(cycle, type, अग्र);

        अगर (module->ctx_सूचकांक > max) {
            max = module->ctx_सूचकांक;
        }

        अग्र = module->ctx_सूचकांक + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    अगर (cycle->old_cycle && cycle->old_cycle->modules) {

        क्रम (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            अगर (module->type != type) {
                जारी;
            }

            अगर (module->ctx_सूचकांक > max) {
                max = module->ctx_सूचकांक;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    वापस max + 1;
}


ngx_int_t
ngx_add_module(ngx_conf_t *cf, ngx_str_t *file, ngx_module_t *module,
    अक्षर **order)
{
    व्योम               *rv;
    ngx_uint_t          i, m, before;
    ngx_core_module_t  *core_module;

    अगर (cf->cycle->modules_n >= ngx_max_module) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "too many modules loaded");
        वापस NGX_ERROR;
    }

    अगर (module->version != nginx_version) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" version %ui instead of %ui",
                           file, module->version, (ngx_uint_t) nginx_version);
        वापस NGX_ERROR;
    }

    अगर (ngx_strcmp(module->signature, NGX_MODULE_SIGNATURE) != 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "module \"%V\" is not binary compatible",
                           file);
        वापस NGX_ERROR;
    }

    क्रम (m = 0; cf->cycle->modules[m]; m++) {
        अगर (ngx_strcmp(cf->cycle->modules[m]->name, module->name) == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "module \"%s\" is already loaded",
                               module->name);
            वापस NGX_ERROR;
        }
    }

    /*
     * if the module wasn't previously loaded, assign an index
     */

    अगर (module->सूचकांक == NGX_MODULE_UNSET_सूचकांक) {
        module->सूचकांक = ngx_module_सूचकांक(cf->cycle);

        अगर (module->सूचकांक >= ngx_max_module) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "too many modules loaded");
            वापस NGX_ERROR;
        }
    }

    /*
     * put the module into the cycle->modules array
     */

    before = cf->cycle->modules_n;

    अगर (order) {
        क्रम (i = 0; order[i]; i++) {
            अगर (ngx_strcmp(order[i], module->name) == 0) {
                i++;
                अवरोध;
            }
        }

        क्रम ( /* void */ ; order[i]; i++) {

#अगर 0
            ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0,
                           "module: %s before %s",
                           module->name, order[i]);
#पूर्ण_अगर

            क्रम (m = 0; m < before; m++) {
                अगर (ngx_strcmp(cf->cycle->modules[m]->name, order[i]) == 0) {

                    ngx_log_debug3(NGX_LOG_DEBUG_CORE, cf->log, 0,
                                   "module: %s before %s:%i",
                                   module->name, order[i], m);

                    before = m;
                    अवरोध;
                }
            }
        }
    }

    /* put the module before modules[before] */

    अगर (before != cf->cycle->modules_n) {
        ngx_memmove(&cf->cycle->modules[before + 1],
                    &cf->cycle->modules[before],
                    (cf->cycle->modules_n - before) * माप(ngx_module_t *));
    }

    cf->cycle->modules[before] = module;
    cf->cycle->modules_n++;

    अगर (module->type == NGX_CORE_MODULE) {

        /*
         * we are smart enough to initialize core modules;
         * other modules are expected to be loaded before
         * initialization - e.g., http modules must be loaded
         * before http{} block
         */

        core_module = module->ctx;

        अगर (core_module->create_conf) {
            rv = core_module->create_conf(cf->cycle);
            अगर (rv == शून्य) {
                वापस NGX_ERROR;
            }

            cf->cycle->conf_ctx[module->सूचकांक] = rv;
        }
    }

    वापस NGX_OK;
}


अटल ngx_uint_t
ngx_module_सूचकांक(ngx_cycle_t *cycle)
{
    ngx_uint_t     i, सूचकांक;
    ngx_module_t  *module;

    सूचकांक = 0;

again:

    /* find an unused index */

    क्रम (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        अगर (module->सूचकांक == सूचकांक) {
            सूचकांक++;
            जाओ again;
        }
    }

    /* check previous cycle */

    अगर (cycle->old_cycle && cycle->old_cycle->modules) {

        क्रम (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            अगर (module->सूचकांक == सूचकांक) {
                सूचकांक++;
                जाओ again;
            }
        }
    }

    वापस सूचकांक;
}


अटल ngx_uint_t
ngx_module_ctx_सूचकांक(ngx_cycle_t *cycle, ngx_uint_t type, ngx_uint_t सूचकांक)
{
    ngx_uint_t     i;
    ngx_module_t  *module;

again:

    /* find an unused ctx_index */

    क्रम (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        अगर (module->type != type) {
            जारी;
        }

        अगर (module->ctx_सूचकांक == सूचकांक) {
            सूचकांक++;
            जाओ again;
        }
    }

    /* check previous cycle */

    अगर (cycle->old_cycle && cycle->old_cycle->modules) {

        क्रम (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            अगर (module->type != type) {
                जारी;
            }

            अगर (module->ctx_सूचकांक == सूचकांक) {
                सूचकांक++;
                जाओ again;
            }
        }
    }

    वापस सूचकांक;
}
