
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
    ngx_thread_task_t        *پہلا;
    ngx_thread_task_t       **آخری;
} ngx_thread_pool_قطار_t;

#تعریف_کرو ngx_thread_pool_قطار_init(q)                                         \
    (q)->پہلا = عدم;                                                        \
    (q)->آخری = &(q)->پہلا


بنية ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_قطار_t   قطار;
    ngx_int_t                 اڈیک;
    ngx_thread_cond_t         cond;

    ngx_log_t                *لوگارتھم;

    ngx_str_t                 name;
    ngx_uint_t                دھاگے;
    ngx_int_t                 max_قطار;

    u_char                   *file;
    ngx_uint_t                سطر;
};


ساكن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوگارتھم,
    ngx_pool_t *pool);
ساكن فراغ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساكن فراغ ngx_thread_pool_exit_handler(فراغ *انکڑے, ngx_log_t *لوگارتھم);

ساكن فراغ *ngx_thread_pool_چکر(فراغ *انکڑے);
ساكن فراغ ngx_thread_pool_handler(ngx_event_t *ev);

ساكن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);

ساكن فراغ *ngx_thread_pool_create_conf(ngx_چکر_t *چکر);
ساكن حرف *ngx_thread_pool_init_conf(ngx_چکر_t *چکر, فراغ *conf);

ساكن ngx_int_t ngx_thread_pool_init_کامہ(ngx_چکر_t *چکر);
ساكن فراغ ngx_thread_pool_exit_کامہ(ngx_چکر_t *چکر);


ساكن ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_سدھا_CONF|NGX_CONF_TAKE23,
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
    ngx_thread_pool_init_کامہ,           /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_thread_pool_exit_کامہ,           /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساكن ngx_uint_t               ngx_thread_pool_task_id;
ساكن ngx_atomic_t             ngx_thread_pool_پورا_جندرا;
ساكن ngx_thread_pool_قطار_t  ngx_thread_pool_پورا;


ساكن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوگارتھم, ngx_pool_t *pool)
{
    صحيح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    إذا (ngx_notify == عدم) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, 0,
               "the configured event method cannot be used with thread pools");
        أرجع NGX_ERROR;
    }

    ngx_thread_pool_قطار_init(&tp->قطار);

    إذا (ngx_thread_mutex_create(&tp->mtx, لوگارتھم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_cond_create(&tp->cond, لوگارتھم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_destroy(&tp->mtx, لوگارتھم);
        أرجع NGX_ERROR;
    }

    tp->لوگارتھم = لوگارتھم;

    err = pthread_attr_init(&attr);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_init() failed");
        أرجع NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_setdetachstate() failed");
        أرجع NGX_ERROR;
    }

#إذا 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ڈھیر_MIN);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_setstacksize() failed");
        أرجع NGX_ERROR;
    }
#جے_انت

    لكل (n = 0; n < tp->دھاگے; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_چکر, tp);
        إذا (err) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
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
    متطاير ngx_uint_t  جندرا;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (فراغ *) &جندرا;

    لكل (n = 0; n < tp->دھاگے; n++) {
        جندرا = 1;

        إذا (ngx_thread_task_post(tp, &task) != NGX_OK) {
            أرجع;
        }

        طالما (جندرا) {
            ngx_sched_yield();
        }

        task.event.سرگرم = 0;
    }

    (فراغ) ngx_thread_cond_destroy(&tp->cond, tp->لوگارتھم);

    (فراغ) ngx_thread_mutex_destroy(&tp->mtx, tp->لوگارتھم);
}


ساكن فراغ
ngx_thread_pool_exit_handler(فراغ *انکڑے, ngx_log_t *لوگارتھم)
{
    ngx_uint_t *جندرا = انکڑے;

    *جندرا = 0;

    pthread_exit(عدم);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, ناپ_t ناپ)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, حجم(ngx_thread_task_t) + ناپ);
    إذا (task == عدم) {
        أرجع عدم;
    }

    task->ctx = task + 1;

    أرجع task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    إذا (task->event.سرگرم) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوگارتھم, 0,
                      "task #%ui already active", task->id);
        أرجع NGX_ERROR;
    }

    إذا (ngx_thread_mutex_جندرا(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    إذا (tp->اڈیک >= tp->max_قطار) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);

        ngx_log_error(NGX_LOG_ERR, tp->لوگارتھم, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->اڈیک);
        أرجع NGX_ERROR;
    }

    task->event.سرگرم = 1;

    task->id = ngx_thread_pool_task_id++;
    task->اگلا = عدم;

    إذا (ngx_thread_cond_signal(&tp->cond, tp->لوگارتھم) != NGX_OK) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);
        أرجع NGX_ERROR;
    }

    *tp->قطار.آخری = task;
    tp->قطار.آخری = &task->اگلا;

    tp->اڈیک++;

    (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_thread_pool_چکر(فراغ *انکڑے)
{
    ngx_thread_pool_t *tp = انکڑے;

    صحيح                 err;
    sigset_t            سیٹ;
    ngx_thread_task_t  *task;

#إذا 0
    ngx_ویلا_update();
#جے_انت

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&سیٹ);

    sigdelset(&سیٹ, SIGILL);
    sigdelset(&سیٹ, SIGFPE);
    sigdelset(&سیٹ, SIGSEGV);
    sigdelset(&سیٹ, SIGBUS);

    err = pthread_sigmask(SIG_بلاک, &سیٹ, عدم);
    إذا (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوگارتھم, err, "pthread_sigmask() failed");
        أرجع عدم;
    }

    لكل ( ;; ) {
        إذا (ngx_thread_mutex_جندرا(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
            أرجع عدم;
        }

        /* the number may become negative */
        tp->اڈیک--;

        طالما (tp->قطار.پہلا == عدم) {
            إذا (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->لوگارتھم)
                != NGX_OK)
            {
                (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);
                أرجع عدم;
            }
        }

        task = tp->قطار.پہلا;
        tp->قطار.پہلا = task->اگلا;

        إذا (tp->قطار.پہلا == عدم) {
            tp->قطار.آخری = &tp->قطار.پہلا;
        }

        إذا (ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
            أرجع عدم;
        }

#إذا 0
        ngx_ویلا_update();
#جے_انت

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->لوگارتھم);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->اگلا = عدم;

        ngx_spinlock(&ngx_thread_pool_پورا_جندرا, 1, 2048);

        *ngx_thread_pool_پورا.آخری = task;
        ngx_thread_pool_پورا.آخری = &task->اگلا;

        ngx_حافظہ_barrier();

        ngx_unlock(&ngx_thread_pool_پورا_جندرا);

        (فراغ) ngx_notify(ngx_thread_pool_handler);
    }
}


ساكن فراغ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_پورا_جندرا, 1, 2048);

    task = ngx_thread_pool_پورا.پہلا;
    ngx_thread_pool_پورا.پہلا = عدم;
    ngx_thread_pool_پورا.آخری = &ngx_thread_pool_پورا.پہلا;

    ngx_حافظہ_barrier();

    ngx_unlock(&ngx_thread_pool_پورا_جندرا);

    طالما (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->اگلا;

        event->complete = 1;
        event->سرگرم = 0;

        event->handler(event);
    }
}


ساكن فراغ *
ngx_thread_pool_create_conf(ngx_چکر_t *چکر)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(چکر->pool, حجم(ngx_thread_pool_conf_t));
    إذا (tcf == عدم) {
        أرجع عدم;
    }

    إذا (ngx_array_init(&tcf->pools, چکر->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع tcf;
}


ساكن حرف *
ngx_thread_pool_init_conf(ngx_چکر_t *چکر, فراغ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->دھاگے) {
            استمر;
        }

        إذا (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.انکڑے, ngx_thread_pool_default.انکڑے,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->دھاگے = 32;
            tpp[i]->max_قطار = 65536;
            استمر;
        }

        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->سطر);

        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t          *قدر;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    قدر = cf->دلیلاں->elts;

    tp = ngx_thread_pool_جوڑو(cf, &قدر[1]);

    إذا (tp == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (tp->دھاگے) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        أرجع NGX_CONF_ERROR;
    }

    tp->max_قطار = 65536;

    لكل (i = 2; i < cf->دلیلاں->nelts; i++) {

        إذا (ngx_strncmp(قدر[i].انکڑے, "threads=", 8) == 0) {

            tp->دھاگے = ngx_atoi(قدر[i].انکڑے + 8, قدر[i].len - 8);

            إذا (tp->دھاگے == (ngx_uint_t) NGX_ERROR || tp->دھاگے == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &قدر[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }

        إذا (ngx_strncmp(قدر[i].انکڑے, "max_queue=", 10) == 0) {

            tp->max_قطار = ngx_atoi(قدر[i].انکڑے + 10, قدر[i].len - 10);

            إذا (tp->max_قطار == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &قدر[i]);
                أرجع NGX_CONF_ERROR;
            }

            استمر;
        }
    }

    إذا (tp->دھاگے == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_جوڑو(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    إذا (name == عدم) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->چکر, name);

    إذا (tp) {
        أرجع tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    إذا (tp == عدم) {
        أرجع عدم;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.انکڑے;
    tp->سطر = cf->conf_file->سطر;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ڈالو(&tcf->pools);
    إذا (tpp == عدم) {
        أرجع عدم;
    }

    *tpp = tp;

    أرجع tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_چکر_t *چکر, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {

        إذا (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.انکڑے, name->انکڑے, name->len) == 0)
        {
            أرجع tpp[i];
        }
    }

    أرجع عدم;
}


ساكن ngx_int_t
ngx_thread_pool_init_کامہ(ngx_چکر_t *چکر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کامہ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع NGX_OK;
    }

    ngx_thread_pool_قطار_init(&ngx_thread_pool_پورا);

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        إذا (ngx_thread_pool_init(tpp[i], چکر->لوگارتھم, چکر->pool) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ساكن فراغ
ngx_thread_pool_exit_کامہ(ngx_چکر_t *چکر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    إذا (ngx_process != NGX_PROCESS_کامہ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        أرجع;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    إذا (tcf == عدم) {
        أرجع;
    }

    tpp = tcf->pools.elts;

    لكل (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
