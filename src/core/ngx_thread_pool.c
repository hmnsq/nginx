
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
    ngx_thread_task_t        *اولی;
    ngx_thread_task_t       **آخرین;
} ngx_thread_pool_صف_t;

#تعریف_کن ngx_thread_pool_صف_init(q)                                         \
    (q)->اولی = عدم;                                                        \
    (q)->آخرین = &(q)->اولی


بنية ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_صف_t   صف;
    ngx_int_t                 انتظار;
    ngx_thread_cond_t         cond;

    ngx_log_t                *لگاریتم;

    ngx_str_t                 name;
    ngx_uint_t                نخها;
    ngx_int_t                 max_صف;

    u_char                   *file;
    ngx_uint_t                سطر;
};


ساكن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لگاریتم,
    ngx_pool_t *pool);
ساكن فراغ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساكن فراغ ngx_thread_pool_exit_handler(فراغ *داده, ngx_log_t *لگاریتم);

ساكن فراغ *ngx_thread_pool_چرخه(فراغ *داده);
ساكن فراغ ngx_thread_pool_handler(ngx_event_t *ev);

ساكن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);

ساكن فراغ *ngx_thread_pool_create_conf(ngx_چرخه_t *چرخه);
ساكن حرف *ngx_thread_pool_init_conf(ngx_چرخه_t *چرخه, فراغ *conf);

ساكن ngx_int_t ngx_thread_pool_init_کارگر(ngx_چرخه_t *چرخه);
ساكن فراغ ngx_thread_pool_exit_کارگر(ngx_چرخه_t *چرخه);


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
    ngx_thread_pool_init_کارگر,           /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_thread_pool_exit_کارگر,           /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساكن ngx_uint_t               ngx_thread_pool_task_id;
ساكن ngx_atomic_t             ngx_thread_pool_انجامشده_قفل;
ساكن ngx_thread_pool_صف_t  ngx_thread_pool_انجامشده;


ساكن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لگاریتم, ngx_pool_t *pool)
{
    صحيح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    إذا (ngx_notify == عدم) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, 0,
               "the configured event method cannot be used with thread pools");
        أرجع NGX_ERROR;
    }

    ngx_thread_pool_صف_init(&tp->صف);

    إذا (ngx_thread_mutex_create(&tp->mtx, لگاریتم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_cond_create(&tp->cond, لگاریتم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_destroy(&tp->mtx, لگاریتم);
        أرجع NGX_ERROR;
    }

    tp->لگاریتم = لگاریتم;

    err = pthread_attr_init(&attr);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, err,
                      "pthread_attr_init() failed");
        أرجع NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, err,
                      "pthread_attr_setdetachstate() failed");
        أرجع NGX_ERROR;
    }

#إذا 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_پشته_MIN);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, err,
                      "pthread_attr_setstacksize() failed");
        أرجع NGX_ERROR;
    }
#پایان_اگر

    لكل (n = 0; n < tp->نخها; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_چرخه, tp);
        إذا (err) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, err,
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
    متطاير ngx_uint_t  قفل;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (فراغ *) &قفل;

    لكل (n = 0; n < tp->نخها; n++) {
        قفل = 1;

        إذا (ngx_thread_task_post(tp, &task) != NGX_OK) {
            أرجع;
        }

        طالما (قفل) {
            ngx_sched_yield();
        }

        task.event.فعال = 0;
    }

    (فراغ) ngx_thread_cond_destroy(&tp->cond, tp->لگاریتم);

    (فراغ) ngx_thread_mutex_destroy(&tp->mtx, tp->لگاریتم);
}


ساكن فراغ
ngx_thread_pool_exit_handler(فراغ *داده, ngx_log_t *لگاریتم)
{
    ngx_uint_t *قفل = داده;

    *قفل = 0;

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
        ngx_log_error(NGX_LOG_ALERT, tp->لگاریتم, 0,
                      "task #%ui already active", task->id);
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_mutex_قفل(&tp->mtx, tp->لگاریتم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (tp->انتظار >= tp->max_صف) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لگاریتم);

        ngx_log_error(NGX_LOG_ERR, tp->لگاریتم, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->انتظار);
        أرجع NGX_ERROR;
    }

    task->event.فعال = 1;

    task->id = ngx_thread_pool_task_id++;
    task->بعدی = عدم;

    إذا (ngx_thread_cond_signal(&tp->cond, tp->لگاریتم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لگاریتم);
        أرجع NGX_ERROR;
    }

    *tp->صف.آخرین = task;
    tp->صف.آخرین = &task->بعدی;

    tp->انتظار++;

    (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لگاریتم);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لگاریتم, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_thread_pool_چرخه(فراغ *داده)
{
    ngx_thread_pool_t *tp = داده;

    صحيح                 err;
    sigset_t            مجموعه;
    ngx_thread_task_t  *task;

#إذا 0
    ngx_زمان_update();
#پایان_اگر

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->لگاریتم, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&مجموعه);

    sigdelset(&مجموعه, SIGILL);
    sigdelset(&مجموعه, SIGFPE);
    sigdelset(&مجموعه, SIGSEGV);
    sigdelset(&مجموعه, SIGBUS);

    err = pthread_sigmask(SIG_بلوک, &مجموعه, عدم);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->لگاریتم, err, "pthread_sigmask() failed");
        أرجع عدم;
    }

    لكل ( ;; ) {
        إذا (ngx_thread_mutex_قفل(&tp->mtx, tp->لگاریتم) != NGX_OK) {
            أرجع عدم;
        }

        /* the number may become negative */
        tp->انتظار--;

        طالما (tp->صف.اولی == عدم) {
            إذا (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->لگاریتم)
                != NGX_OK)
            {
                (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لگاریتم);
                أرجع عدم;
            }
        }

        task = tp->صف.اولی;
        tp->صف.اولی = task->بعدی;

        إذا (tp->صف.اولی == عدم) {
            tp->صف.آخرین = &tp->صف.اولی;
        }

        إذا (ngx_thread_mutex_unlock(&tp->mtx, tp->لگاریتم) != NGX_OK) {
            أرجع عدم;
        }

#إذا 0
        ngx_زمان_update();
#پایان_اگر

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لگاریتم, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->لگاریتم);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لگاریتم, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->بعدی = عدم;

        ngx_spinlock(&ngx_thread_pool_انجامشده_قفل, 1, 2048);

        *ngx_thread_pool_انجامشده.آخرین = task;
        ngx_thread_pool_انجامشده.آخرین = &task->بعدی;

        ngx_حافظه_barrier();

        ngx_unlock(&ngx_thread_pool_انجامشده_قفل);

        (فراغ) ngx_notify(ngx_thread_pool_handler);
    }
}


ساكن فراغ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->لگاریتم, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_انجامشده_قفل, 1, 2048);

    task = ngx_thread_pool_انجامشده.اولی;
    ngx_thread_pool_انجامشده.اولی = عدم;
    ngx_thread_pool_انجامشده.آخرین = &ngx_thread_pool_انجامشده.اولی;

    ngx_حافظه_barrier();

    ngx_unlock(&ngx_thread_pool_انجامشده_قفل);

    طالما (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لگاریتم, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->بعدی;

        event->complete = 1;
        event->فعال = 0;

        event->handler(event);
    }
}


ساكن فراغ *
ngx_thread_pool_create_conf(ngx_چرخه_t *چرخه)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(چرخه->pool, حجم(ngx_thread_pool_conf_t));
    إذا (tcf == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(&tcf->pools, چرخه->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع tcf;
}


ساكن حرف *
ngx_thread_pool_init_conf(ngx_چرخه_t *چرخه, فراغ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->نخها) {
            استمر;
        }

        إذا (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.داده, ngx_thread_pool_default.داده,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->نخها = 32;
            tpp[i]->max_صف = 65536;
            استمر;
        }

        ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->سطر);

        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t          *مقدار;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    مقدار = cf->آرگومانها->elts;

    tp = ngx_thread_pool_جمع_کن(cf, &مقدار[1]);

    إذا (tp == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (tp->نخها) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        أرجع NGX_CONF_ERROR;
    }

    tp->max_صف = 65536;

    لكل (i = 2; i < cf->آرگومانها->nelts; i++) {

        إذا (ngx_strncmp(مقدار[i].داده, "threads=", 8) == 0) {

            tp->نخها = ngx_atoi(مقدار[i].داده + 8, مقدار[i].len - 8);

            إذا (tp->نخها == (ngx_uint_t) NGX_ERROR || tp->نخها == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &مقدار[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }

        إذا (ngx_strncmp(مقدار[i].داده, "max_queue=", 10) == 0) {

            tp->max_صف = ngx_atoi(مقدار[i].داده + 10, مقدار[i].len - 10);

            إذا (tp->max_صف == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &مقدار[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }
    }

    إذا (tp->نخها == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_جمع_کن(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    إذا (name == عدم) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->چرخه, name);

    إذا (tp) {
        أرجع tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    إذا (tp == عدم) {
        أرجع عدم;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.داده;
    tp->سطر = cf->conf_file->سطر;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->چرخه->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_افزودن(&tcf->pools);
    إذا (tpp == عدم) {
        أرجع عدم;
    }

    *tpp = tp;

    أرجع tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_چرخه_t *چرخه, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چرخه->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.داده, name->داده, name->len) == 0)
        {
            أرجع tpp[i];
        }
    }

    أرجع عدم;
}


ساكن ngx_int_t
ngx_thread_pool_init_کارگر(ngx_چرخه_t *چرخه)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کارگر
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چرخه->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع NGX_OK;
    }

    ngx_thread_pool_صف_init(&ngx_thread_pool_انجامشده);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        إذا (ngx_thread_pool_init(tpp[i], چرخه->لگاریتم, چرخه->pool) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_exit_کارگر(ngx_چرخه_t *چرخه)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کارگر
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چرخه->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع;
    }

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
