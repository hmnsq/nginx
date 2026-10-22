
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_thread_pool.h>


تعريفنوع بنية {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


تعريفنوع بنية {
    ngx_thread_task_t        *لومړی;
    ngx_thread_task_t       **وروستی;
} ngx_thread_pool_کتار_t;

#تعریف_کړه ngx_thread_pool_کتار_init(q)                                         \
    (q)->لومړی = عدم;                                                        \
    (q)->وروستی = &(q)->لومړی


بنية ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_کتار_t   کتار;
    ngx_int_t                 انتظار;
    ngx_thread_cond_t         cond;

    ngx_log_t                *لوګاریتم;

    ngx_str_t                 name;
    ngx_uint_t                تارونه;
    ngx_int_t                 max_کتار;

    u_char                   *file;
    ngx_uint_t                کرښه;
};


ساكن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوګاریتم,
    ngx_pool_t *pool);
ساكن فراغ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساكن فراغ ngx_thread_pool_exit_handler(فراغ *معلومات, ngx_log_t *لوګاریتم);

ساكن فراغ *ngx_thread_pool_دوره(فراغ *معلومات);
ساكن فراغ ngx_thread_pool_handler(ngx_event_t *ev);

ساكن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);

ساكن فراغ *ngx_thread_pool_create_conf(ngx_دوره_t *دوره);
ساكن حرف *ngx_thread_pool_init_conf(ngx_دوره_t *دوره, فراغ *conf);

ساكن ngx_int_t ngx_thread_pool_init_کارګر(ngx_دوره_t *دوره);
ساكن فراغ ngx_thread_pool_exit_کارګر(ngx_دوره_t *دوره);


ساكن ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      عدم },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    ngx_thread_pool_init_کارګر,           /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_thread_pool_exit_کارګر,           /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساكن ngx_uint_t               ngx_thread_pool_task_id;
ساكن ngx_atomic_t             ngx_thread_pool_بشپړ_قلف;
ساكن ngx_thread_pool_کتار_t  ngx_thread_pool_بشپړ;


ساكن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوګاریتم, ngx_pool_t *pool)
{
    صحيح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    إذا (ngx_notify == عدم) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, 0,
               "the configured event method cannot be used with thread pools");
        أرجع NGX_ERROR;
    }

    ngx_thread_pool_کتار_init(&tp->کتار);

    إذا (ngx_thread_mutex_create(&tp->mtx, لوګاریتم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_cond_create(&tp->cond, لوګاریتم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_destroy(&tp->mtx, لوګاریتم);
        أرجع NGX_ERROR;
    }

    tp->لوګاریتم = لوګاریتم;

    err = pthread_attr_init(&attr);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, err,
                      "pthread_attr_init() failed");
        أرجع NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, err,
                      "pthread_attr_setdetachstate() failed");
        أرجع NGX_ERROR;
    }

#إذا 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ډېری_MIN);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, err,
                      "pthread_attr_setstacksize() failed");
        أرجع NGX_ERROR;
    }
#د_که_پای

    لكل (n = 0; n < tp->تارونه; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_دوره, tp);
        إذا (err) {
            ngx_log_error(NGX_LOG_ALERT, لوګاریتم, err,
                          "pthread_create() failed");
            أرجع NGX_ERROR;
        }
    }

    (فراغ) pthread_attr_destroy(&attr);

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    متطاير ngx_uint_t  قلف;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (فراغ *) &قلف;

    لكل (n = 0; n < tp->تارونه; n++) {
        قلف = 1;

        إذا (ngx_thread_task_post(tp, &task) != NGX_OK) {
            أرجع;
        }

        طالما (قلف) {
            ngx_sched_yield();
        }

        task.event.فعال = 0;
    }

    (فراغ) ngx_thread_cond_destroy(&tp->cond, tp->لوګاریتم);

    (فراغ) ngx_thread_mutex_destroy(&tp->mtx, tp->لوګاریتم);
}


ساكن فراغ
ngx_thread_pool_exit_handler(فراغ *معلومات, ngx_log_t *لوګاریتم)
{
    ngx_uint_t *قلف = معلومات;

    *قلف = 0;

    pthread_exit(عدم);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, اندازه_t اندازه)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, حجم(ngx_thread_task_t) + اندازه);
    إذا (task == عدم) {
        أرجع عدم;
    }

    task->ctx = task + 1;

    أرجع task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    إذا (task->event.فعال) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوګاریتم, 0,
                      "task #%ui already active", task->id);
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_mutex_قلف(&tp->mtx, tp->لوګاریتم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (tp->انتظار >= tp->max_کتار) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوګاریتم);

        ngx_log_error(NGX_LOG_ERR, tp->لوګاریتم, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->انتظار);
        أرجع NGX_ERROR;
    }

    task->event.فعال = 1;

    task->id = ngx_thread_pool_task_id++;
    task->بل = عدم;

    إذا (ngx_thread_cond_signal(&tp->cond, tp->لوګاریتم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوګاریتم);
        أرجع NGX_ERROR;
    }

    *tp->کتار.وروستی = task;
    tp->کتار.وروستی = &task->بل;

    tp->انتظار++;

    (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوګاریتم);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوګاریتم, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_thread_pool_دوره(فراغ *معلومات)
{
    ngx_thread_pool_t *tp = معلومات;

    صحيح                 err;
    sigset_t            سېټ;
    ngx_thread_task_t  *task;

#إذا 0
    ngx_وخت_update();
#د_که_پای

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->لوګاریتم, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&سېټ);

    sigdelset(&سېټ, SIGILL);
    sigdelset(&سېټ, SIGFPE);
    sigdelset(&سېټ, SIGSEGV);
    sigdelset(&سېټ, SIGBUS);

    err = pthread_sigmask(SIG_بلاک, &سېټ, عدم);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوګاریتم, err, "pthread_sigmask() failed");
        أرجع عدم;
    }

    لكل ( ;; ) {
        إذا (ngx_thread_mutex_قلف(&tp->mtx, tp->لوګاریتم) != NGX_OK) {
            أرجع عدم;
        }

        /* the number may become negative */
        tp->انتظار--;

        طالما (tp->کتار.لومړی == عدم) {
            إذا (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->لوګاریتم)
                != NGX_OK)
            {
                (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوګاریتم);
                أرجع عدم;
            }
        }

        task = tp->کتار.لومړی;
        tp->کتار.لومړی = task->بل;

        إذا (tp->کتار.لومړی == عدم) {
            tp->کتار.وروستی = &tp->کتار.لومړی;
        }

        إذا (ngx_thread_mutex_unlock(&tp->mtx, tp->لوګاریتم) != NGX_OK) {
            أرجع عدم;
        }

#إذا 0
        ngx_وخت_update();
#د_که_پای

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوګاریتم, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->لوګاریتم);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوګاریتم, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->بل = عدم;

        ngx_spinlock(&ngx_thread_pool_بشپړ_قلف, 1, 2048);

        *ngx_thread_pool_بشپړ.وروستی = task;
        ngx_thread_pool_بشپړ.وروستی = &task->بل;

        ngx_حافظه_barrier();

        ngx_unlock(&ngx_thread_pool_بشپړ_قلف);

        (فراغ) ngx_notify(ngx_thread_pool_handler);
    }
}


ساكن فراغ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->لوګاریتم, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_بشپړ_قلف, 1, 2048);

    task = ngx_thread_pool_بشپړ.لومړی;
    ngx_thread_pool_بشپړ.لومړی = عدم;
    ngx_thread_pool_بشپړ.وروستی = &ngx_thread_pool_بشپړ.لومړی;

    ngx_حافظه_barrier();

    ngx_unlock(&ngx_thread_pool_بشپړ_قلف);

    طالما (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوګاریتم, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->بل;

        event->complete = 1;
        event->فعال = 0;

        event->handler(event);
    }
}


ساكن فراغ *
ngx_thread_pool_create_conf(ngx_دوره_t *دوره)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(دوره->pool, حجم(ngx_thread_pool_conf_t));
    إذا (tcf == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(&tcf->pools, دوره->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع tcf;
}


ساكن حرف *
ngx_thread_pool_init_conf(ngx_دوره_t *دوره, فراغ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->تارونه) {
            استمر;
        }

        إذا (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.معلومات, ngx_thread_pool_default.معلومات,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->تارونه = 32;
            tpp[i]->max_کتار = 65536;
            استمر;
        }

        ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->کرښه);

        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t          *ارزښت;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    ارزښت = cf->دلیلونه->elts;

    tp = ngx_thread_pool_جمع_کړه(cf, &ارزښت[1]);

    إذا (tp == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (tp->تارونه) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        أرجع NGX_CONF_ERROR;
    }

    tp->max_کتار = 65536;

    لكل (i = 2; i < cf->دلیلونه->nelts; i++) {

        إذا (ngx_strncmp(ارزښت[i].معلومات, "threads=", 8) == 0) {

            tp->تارونه = ngx_atoi(ارزښت[i].معلومات + 8, ارزښت[i].len - 8);

            إذا (tp->تارونه == (ngx_uint_t) NGX_ERROR || tp->تارونه == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &ارزښت[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }

        إذا (ngx_strncmp(ارزښت[i].معلومات, "max_queue=", 10) == 0) {

            tp->max_کتار = ngx_atoi(ارزښت[i].معلومات + 10, ارزښت[i].len - 10);

            إذا (tp->max_کتار == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &ارزښت[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }
    }

    إذا (tp->تارونه == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_جمع_کړه(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    إذا (name == عدم) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->دوره, name);

    إذا (tp) {
        أرجع tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    إذا (tp == عدم) {
        أرجع عدم;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.معلومات;
    tp->کرښه = cf->conf_file->کرښه;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->دوره->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ټېل(&tcf->pools);
    إذا (tpp == عدم) {
        أرجع عدم;
    }

    *tpp = tp;

    أرجع tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_دوره_t *دوره, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(دوره->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.معلومات, name->معلومات, name->len) == 0)
        {
            أرجع tpp[i];
        }
    }

    أرجع عدم;
}


ساكن ngx_int_t
ngx_thread_pool_init_کارګر(ngx_دوره_t *دوره)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کارګر
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(دوره->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع NGX_OK;
    }

    ngx_thread_pool_کتار_init(&ngx_thread_pool_بشپړ);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        إذا (ngx_thread_pool_init(tpp[i], دوره->لوګاریتم, دوره->pool) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_exit_کارګر(ngx_دوره_t *دوره)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کارګر
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(دوره->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع;
    }

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
