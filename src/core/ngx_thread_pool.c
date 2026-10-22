
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_thread_pool.h>


قسمتعریف ساخت {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


قسمتعریف ساخت {
    ngx_thread_task_t        *پہلا;
    ngx_thread_task_t       **آخری;
} ngx_thread_pool_قطار_t;

#تعریف_کرو ngx_thread_pool_قطار_init(q)                                         \
    (q)->پہلا = تہی;                                                        \
    (q)->آخری = &(q)->پہلا


ساخت ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_قطار_t   قطار;
    ngx_int_t                 انتظار;
    ngx_thread_cond_t         cond;

    ngx_log_t                *لوگارتھم;

    ngx_str_t                 name;
    ngx_uint_t                دھاگے;
    ngx_int_t                 max_قطار;

    u_char                   *file;
    ngx_uint_t                سطر;
};


ساکن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوگارتھم,
    ngx_pool_t *pool);
ساکن خالی ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساکن خالی ngx_thread_pool_exit_handler(خالی *ڈیٹا, ngx_log_t *لوگارتھم);

ساکن خالی *ngx_thread_pool_چکر(خالی *ڈیٹا);
ساکن خالی ngx_thread_pool_handler(ngx_event_t *ev);

ساکن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);

ساکن خالی *ngx_thread_pool_create_conf(ngx_چکر_t *چکر);
ساکن حرف *ngx_thread_pool_init_conf(ngx_چکر_t *چکر, خالی *conf);

ساکن ngx_int_t ngx_thread_pool_init_کارکن(ngx_چکر_t *چکر);
ساکن خالی ngx_thread_pool_exit_کارکن(ngx_چکر_t *چکر);


ساکن ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      تہی },

      ngx_null_command
};


ساکن ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    تہی,                                  /* init master */
    تہی,                                  /* init module */
    ngx_thread_pool_init_کارکن,           /* init process */
    تہی,                                  /* init thread */
    تہی,                                  /* exit thread */
    ngx_thread_pool_exit_کارکن,           /* exit process */
    تہی,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساکن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساکن ngx_uint_t               ngx_thread_pool_task_id;
ساکن ngx_atomic_t             ngx_thread_pool_مکمل_تالا;
ساکن ngx_thread_pool_قطار_t  ngx_thread_pool_مکمل;


ساکن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *لوگارتھم, ngx_pool_t *pool)
{
    صحیح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    اگر (ngx_notify == تہی) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, 0,
               "the configured event method cannot be used with thread pools");
        واپس NGX_ERROR;
    }

    ngx_thread_pool_قطار_init(&tp->قطار);

    اگر (ngx_thread_mutex_create(&tp->mtx, لوگارتھم) != NGX_OK) {
        واپس NGX_ERROR;
    }

    اگر (ngx_thread_cond_create(&tp->cond, لوگارتھم) != NGX_OK) {
        (خالی) ngx_thread_mutex_destroy(&tp->mtx, لوگارتھم);
        واپس NGX_ERROR;
    }

    tp->لوگارتھم = لوگارتھم;

    err = pthread_attr_init(&attr);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_init() failed");
        واپس NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_setdetachstate() failed");
        واپس NGX_ERROR;
    }

#اگر 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ڈھیر_MIN);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                      "pthread_attr_setstacksize() failed");
        واپس NGX_ERROR;
    }
#اگر_ختم

    برائے (n = 0; n < tp->دھاگے; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_چکر, tp);
        اگر (err) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, err,
                          "pthread_create() failed");
            واپس NGX_ERROR;
        }
    }

    (خالی) pthread_attr_destroy(&attr);

    واپس NGX_OK;
}


ساکن خالی
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    متلون ngx_uint_t  تالا;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (خالی *) &تالا;

    برائے (n = 0; n < tp->دھاگے; n++) {
        تالا = 1;

        اگر (ngx_thread_task_post(tp, &task) != NGX_OK) {
            واپس;
        }

        جب (تالا) {
            ngx_sched_yield();
        }

        task.event.فعال = 0;
    }

    (خالی) ngx_thread_cond_destroy(&tp->cond, tp->لوگارتھم);

    (خالی) ngx_thread_mutex_destroy(&tp->mtx, tp->لوگارتھم);
}


ساکن خالی
ngx_thread_pool_exit_handler(خالی *ڈیٹا, ngx_log_t *لوگارتھم)
{
    ngx_uint_t *تالا = ڈیٹا;

    *تالا = 0;

    pthread_exit(تہی);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, سائز_t سائز)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, حجم(ngx_thread_task_t) + سائز);
    اگر (task == تہی) {
        واپس تہی;
    }

    task->ctx = task + 1;

    واپس task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    اگر (task->event.فعال) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوگارتھم, 0,
                      "task #%ui already active", task->id);
        واپس NGX_ERROR;
    }

    اگر (ngx_thread_mutex_تالا(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
        واپس NGX_ERROR;
    }

    اگر (tp->انتظار >= tp->max_قطار) {
        (خالی) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);

        ngx_log_error(NGX_LOG_ERR, tp->لوگارتھم, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->انتظار);
        واپس NGX_ERROR;
    }

    task->event.فعال = 1;

    task->id = ngx_thread_pool_task_id++;
    task->اگلا = تہی;

    اگر (ngx_thread_cond_signal(&tp->cond, tp->لوگارتھم) != NGX_OK) {
        (خالی) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);
        واپس NGX_ERROR;
    }

    *tp->قطار.آخری = task;
    tp->قطار.آخری = &task->اگلا;

    tp->انتظار++;

    (خالی) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    واپس NGX_OK;
}


ساکن خالی *
ngx_thread_pool_چکر(خالی *ڈیٹا)
{
    ngx_thread_pool_t *tp = ڈیٹا;

    صحیح                 err;
    sigset_t            سیٹ;
    ngx_thread_task_t  *task;

#اگر 0
    ngx_وقت_update();
#اگر_ختم

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&سیٹ);

    sigdelset(&سیٹ, SIGILL);
    sigdelset(&سیٹ, SIGFPE);
    sigdelset(&سیٹ, SIGSEGV);
    sigdelset(&سیٹ, SIGBUS);

    err = pthread_sigmask(SIG_بلاک, &سیٹ, تہی);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->لوگارتھم, err, "pthread_sigmask() failed");
        واپس تہی;
    }

    برائے ( ;; ) {
        اگر (ngx_thread_mutex_تالا(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
            واپس تہی;
        }

        /* the number may become negative */
        tp->انتظار--;

        جب (tp->قطار.پہلا == تہی) {
            اگر (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->لوگارتھم)
                != NGX_OK)
            {
                (خالی) ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم);
                واپس تہی;
            }
        }

        task = tp->قطار.پہلا;
        tp->قطار.پہلا = task->اگلا;

        اگر (tp->قطار.پہلا == تہی) {
            tp->قطار.آخری = &tp->قطار.پہلا;
        }

        اگر (ngx_thread_mutex_unlock(&tp->mtx, tp->لوگارتھم) != NGX_OK) {
            واپس تہی;
        }

#اگر 0
        ngx_وقت_update();
#اگر_ختم

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->لوگارتھم);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->لوگارتھم, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->اگلا = تہی;

        ngx_spinlock(&ngx_thread_pool_مکمل_تالا, 1, 2048);

        *ngx_thread_pool_مکمل.آخری = task;
        ngx_thread_pool_مکمل.آخری = &task->اگلا;

        ngx_حافظہ_barrier();

        ngx_unlock(&ngx_thread_pool_مکمل_تالا);

        (خالی) ngx_notify(ngx_thread_pool_handler);
    }
}


ساکن خالی
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_مکمل_تالا, 1, 2048);

    task = ngx_thread_pool_مکمل.پہلا;
    ngx_thread_pool_مکمل.پہلا = تہی;
    ngx_thread_pool_مکمل.آخری = &ngx_thread_pool_مکمل.پہلا;

    ngx_حافظہ_barrier();

    ngx_unlock(&ngx_thread_pool_مکمل_تالا);

    جب (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->اگلا;

        event->complete = 1;
        event->فعال = 0;

        event->handler(event);
    }
}


ساکن خالی *
ngx_thread_pool_create_conf(ngx_چکر_t *چکر)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(چکر->pool, حجم(ngx_thread_pool_conf_t));
    اگر (tcf == تہی) {
        واپس تہی;
    }

    اگر (ngx_array_init(&tcf->pools, چکر->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        واپس تہی;
    }

    واپس tcf;
}


ساکن حرف *
ngx_thread_pool_init_conf(ngx_چکر_t *چکر, خالی *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    برائے (i = 0; i < tcf->pools.nelts; i++) {

        اگر (tpp[i]->دھاگے) {
            جاری;
        }

        اگر (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.ڈیٹا, ngx_thread_pool_default.ڈیٹا,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->دھاگے = 32;
            tpp[i]->max_قطار = 65536;
            جاری;
        }

        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->سطر);

        واپس NGX_CONF_ERROR;
    }

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    ngx_str_t          *قدر;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    قدر = cf->دلائل->elts;

    tp = ngx_thread_pool_جوڑو(cf, &قدر[1]);

    اگر (tp == تہی) {
        واپس NGX_CONF_ERROR;
    }

    اگر (tp->دھاگے) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        واپس NGX_CONF_ERROR;
    }

    tp->max_قطار = 65536;

    برائے (i = 2; i < cf->دلائل->nelts; i++) {

        اگر (ngx_strncmp(قدر[i].ڈیٹا, "threads=", 8) == 0) {

            tp->دھاگے = ngx_atoi(قدر[i].ڈیٹا + 8, قدر[i].len - 8);

            اگر (tp->دھاگے == (ngx_uint_t) NGX_ERROR || tp->دھاگے == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &قدر[i]);
                واپس NGX_CONF_ERROR;
            }

            جاری;
        }

        اگر (ngx_strncmp(قدر[i].ڈیٹا, "max_queue=", 10) == 0) {

            tp->max_قطار = ngx_atoi(قدر[i].ڈیٹا + 10, قدر[i].len - 10);

            اگر (tp->max_قطار == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &قدر[i]);
                واپس NGX_CONF_ERROR;
            }

            جاری;
        }
    }

    اگر (tp->دھاگے == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        واپس NGX_CONF_ERROR;
    }

    واپس NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_جوڑو(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    اگر (name == تہی) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->چکر, name);

    اگر (tp) {
        واپس tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    اگر (tp == تہی) {
        واپس تہی;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.ڈیٹا;
    tp->سطر = cf->conf_file->سطر;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ڈالو(&tcf->pools);
    اگر (tpp == تہی) {
        واپس تہی;
    }

    *tpp = tp;

    واپس tp;
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

    برائے (i = 0; i < tcf->pools.nelts; i++) {

        اگر (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.ڈیٹا, name->ڈیٹا, name->len) == 0)
        {
            واپس tpp[i];
        }
    }

    واپس تہی;
}


ساکن ngx_int_t
ngx_thread_pool_init_کارکن(ngx_چکر_t *چکر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    اگر (ngx_process != NGX_PROCESS_کارکن
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        واپس NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    اگر (tcf == تہی) {
        واپس NGX_OK;
    }

    ngx_thread_pool_قطار_init(&ngx_thread_pool_مکمل);

    tpp = tcf->pools.elts;

    برائے (i = 0; i < tcf->pools.nelts; i++) {
        اگر (ngx_thread_pool_init(tpp[i], چکر->لوگارتھم, چکر->pool) != NGX_OK) {
            واپس NGX_ERROR;
        }
    }

    واپس NGX_OK;
}


ساکن خالی
ngx_thread_pool_exit_کارکن(ngx_چکر_t *چکر)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    اگر (ngx_process != NGX_PROCESS_کارکن
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        واپس;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(چکر->conf_ctx,
                                                  ngx_thread_pool_module);

    اگر (tcf == تہی) {
        واپس;
    }

    tpp = tcf->pools.elts;

    برائے (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
