
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_thread_pool.h>


تعریفنوع ساختار {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


تعریفنوع ساختار {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = پوچ;                                                        \
    (q)->last = &(q)->first


ساختار ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                سطر;
};


ایستا ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
ایستا تهی ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ایستا تهی ngx_thread_pool_exit_handler(تهی *data, ngx_log_t *log);

ایستا تهی *ngx_thread_pool_cycle(تهی *data);
ایستا تهی ngx_thread_pool_handler(ngx_event_t *ev);

ایستا نویسه *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);

ایستا تهی *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
ایستا نویسه *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, تهی *conf);

ایستا ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
ایستا تهی ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


ایستا ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      پوچ },

      ngx_null_command
};


ایستا ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    پوچ,                                  /* init master */
    پوچ,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    پوچ,                                  /* init thread */
    پوچ,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    پوچ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ایستا ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ایستا ngx_uint_t               ngx_thread_pool_task_id;
ایستا ngx_atomic_t             ngx_thread_pool_done_lock;
ایستا ngx_thread_pool_queue_t  ngx_thread_pool_done;


ایستا ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    صحیح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    اگر (ngx_notify == پوچ) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        بازگشت NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    اگر (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        بازگشت NGX_ERROR;
    }

    اگر (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (تهی) ngx_thread_mutex_destroy(&tp->mtx, log);
        بازگشت NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        بازگشت NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        بازگشت NGX_ERROR;
    }

#اگر 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_پشته_MIN);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        بازگشت NGX_ERROR;
    }
#endif

    برای (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        اگر (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            بازگشت NGX_ERROR;
        }
    }

    (تهی) pthread_attr_destroy(&attr);

    بازگشت NGX_OK;
}


ایستا تهی
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    فرار ngx_uint_t  lock;

    ngx_memzero(&task, اندازه(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (تهی *) &lock;

    برای (n = 0; n < tp->threads; n++) {
        lock = 1;

        اگر (ngx_thread_task_post(tp, &task) != NGX_OK) {
            بازگشت;
        }

        وقتی (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (تهی) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (تهی) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


ایستا تهی
ngx_thread_pool_exit_handler(تهی *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(پوچ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, اندازه(ngx_thread_task_t) + size);
    اگر (task == پوچ) {
        بازگشت پوچ;
    }

    task->ctx = task + 1;

    بازگشت task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    اگر (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        بازگشت NGX_ERROR;
    }

    اگر (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        بازگشت NGX_ERROR;
    }

    اگر (tp->waiting >= tp->max_queue) {
        (تهی) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        بازگشت NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = پوچ;

    اگر (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (تهی) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        بازگشت NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (تهی) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    بازگشت NGX_OK;
}


ایستا تهی *
ngx_thread_pool_cycle(تهی *data)
{
    ngx_thread_pool_t *tp = data;

    صحیح                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#اگر 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, پوچ);
    اگر (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        بازگشت پوچ;
    }

    برای ( ;; ) {
        اگر (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            بازگشت پوچ;
        }

        /* the number may become negative */
        tp->waiting--;

        وقتی (tp->queue.first == پوچ) {
            اگر (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (تهی) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                بازگشت پوچ;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        اگر (tp->queue.first == پوچ) {
            tp->queue.last = &tp->queue.first;
        }

        اگر (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            بازگشت پوچ;
        }

#اگر 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = پوچ;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (تهی) ngx_notify(ngx_thread_pool_handler);
    }
}


ایستا تهی
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = پوچ;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    وقتی (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


ایستا تهی *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, اندازه(ngx_thread_pool_conf_t));
    اگر (tcf == پوچ) {
        بازگشت پوچ;
    }

    اگر (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       اندازه(ngx_thread_pool_t *))
        != NGX_OK)
    {
        بازگشت پوچ;
    }

    بازگشت tcf;
}


ایستا نویسه *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, تهی *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    برای (i = 0; i < tcf->pools.nelts; i++) {

        اگر (tpp[i]->threads) {
            ادامه;
        }

        اگر (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            ادامه;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->سطر);

        بازگشت NGX_CONF_ERROR;
    }

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    ngx_str_t          *مقدار;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    مقدار = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &مقدار[1]);

    اگر (tp == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    اگر (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        بازگشت NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    برای (i = 2; i < cf->args->nelts; i++) {

        اگر (ngx_strncmp(مقدار[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(مقدار[i].data + 8, مقدار[i].len - 8);

            اگر (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &مقدار[i]);
                بازگشت NGX_CONF_ERROR;
            }

            ادامه;
        }

        اگر (ngx_strncmp(مقدار[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(مقدار[i].data + 10, مقدار[i].len - 10);

            اگر (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &مقدار[i]);
                بازگشت NGX_CONF_ERROR;
            }

            ادامه;
        }
    }

    اگر (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        بازگشت NGX_CONF_ERROR;
    }

    بازگشت NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    اگر (name == پوچ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    اگر (tp) {
        بازگشت tp;
    }

    tp = ngx_pcalloc(cf->pool, اندازه(ngx_thread_pool_t));
    اگر (tp == پوچ) {
        بازگشت پوچ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->سطر = cf->conf_file->سطر;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_افزودن(&tcf->pools);
    اگر (tpp == پوچ) {
        بازگشت پوچ;
    }

    *tpp = tp;

    بازگشت tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    برای (i = 0; i < tcf->pools.nelts; i++) {

        اگر (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            بازگشت tpp[i];
        }
    }

    بازگشت پوچ;
}


ایستا ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    اگر (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        بازگشت NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    اگر (tcf == پوچ) {
        بازگشت NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    برای (i = 0; i < tcf->pools.nelts; i++) {
        اگر (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            بازگشت NGX_ERROR;
        }
    }

    بازگشت NGX_OK;
}


ایستا تهی
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    اگر (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        بازگشت;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    اگر (tcf == پوچ) {
        بازگشت;
    }

    tpp = tcf->pools.elts;

    برای (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
