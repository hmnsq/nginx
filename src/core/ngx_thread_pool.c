
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
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = عدم;                                                        \
    (q)->last = &(q)->first


بنية ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                line;
};


ساكن ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
ساكن فراغ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ساكن فراغ ngx_thread_pool_exit_handler(فراغ *data, ngx_log_t *log);

ساكن فراغ *ngx_thread_pool_cycle(فراغ *data);
ساكن فراغ ngx_thread_pool_handler(ngx_event_t *ev);

ساكن حرف *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);

ساكن فراغ *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
ساكن حرف *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, فراغ *conf);

ساكن ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
ساكن فراغ ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


ساكن ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
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
    ngx_thread_pool_init_worker,           /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ساكن ngx_uint_t               ngx_thread_pool_task_id;
ساكن ngx_atomic_t             ngx_thread_pool_done_lock;
ساكن ngx_thread_pool_queue_t  ngx_thread_pool_done;


ساكن ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    صحيح             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    אם (ngx_notify == عدم) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        החזר NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    אם (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        החזר NGX_ERROR;
    }

    אם (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (فراغ) ngx_thread_mutex_destroy(&tp->mtx, log);
        החזר NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    אם (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        החזר NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    אם (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        החזר NGX_ERROR;
    }

#אם 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
    אם (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        החזר NGX_ERROR;
    }
#endif

    עבור (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        אם (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            החזר NGX_ERROR;
        }
    }

    (فراغ) pthread_attr_destroy(&attr);

    החזר NGX_OK;
}


ساكن فراغ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    متطاير ngx_uint_t  lock;

    ngx_memzero(&task, حجم(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (فراغ *) &lock;

    עבור (n = 0; n < tp->threads; n++) {
        lock = 1;

        אם (ngx_thread_task_post(tp, &task) != NGX_OK) {
            החזר;
        }

        כלעוד (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (فراغ) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (فراغ) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


ساكن فراغ
ngx_thread_pool_exit_handler(فراغ *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(عدم);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, حجم(ngx_thread_task_t) + size);
    אם (task == عدم) {
        החזר عدم;
    }

    task->ctx = task + 1;

    החזר task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    אם (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        החזר NGX_ERROR;
    }

    אם (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        החזר NGX_ERROR;
    }

    אם (tp->waiting >= tp->max_queue) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        החזר NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = عدم;

    אם (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        החזר NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    החזר NGX_OK;
}


ساكن فراغ *
ngx_thread_pool_cycle(فراغ *data)
{
    ngx_thread_pool_t *tp = data;

    صحيح                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#אם 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, عدم);
    אם (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        החזר عدم;
    }

    עבור ( ;; ) {
        אם (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            החזר عدم;
        }

        /* the number may become negative */
        tp->waiting--;

        כלעוד (tp->queue.first == عدم) {
            אם (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (فراغ) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                החזר عدم;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        אם (tp->queue.first == عدم) {
            tp->queue.last = &tp->queue.first;
        }

        אם (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            החזר عدم;
        }

#אם 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = عدم;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (فراغ) ngx_notify(ngx_thread_pool_handler);
    }
}


ساكن فراغ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = عدم;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    כלעוד (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


ساكن فراغ *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, حجم(ngx_thread_pool_conf_t));
    אם (tcf == عدم) {
        החזר عدم;
    }

    אם (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       حجم(ngx_thread_pool_t *))
        != NGX_OK)
    {
        החזר عدم;
    }

    החזר tcf;
}


ساكن حرف *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, فراغ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    עבור (i = 0; i < tcf->pools.nelts; i++) {

        אם (tpp[i]->threads) {
            استمر;
        }

        אם (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            استمر;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->line);

        החזר NGX_CONF_ERROR;
    }

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t          *value;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    value = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &value[1]);

    אם (tp == عدم) {
        החזר NGX_CONF_ERROR;
    }

    אם (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        החזר NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    עבור (i = 2; i < cf->args->nelts; i++) {

        אם (ngx_strncmp(value[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(value[i].data + 8, value[i].len - 8);

            אם (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &value[i]);
                החזר NGX_CONF_ERROR;
            }

            استمر;
        }

        אם (ngx_strncmp(value[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(value[i].data + 10, value[i].len - 10);

            אם (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &value[i]);
                החזר NGX_CONF_ERROR;
            }

            استمر;
        }
    }

    אם (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        החזר NGX_CONF_ERROR;
    }

    החזר NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    אם (name == عدم) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    אם (tp) {
        החזר tp;
    }

    tp = ngx_pcalloc(cf->pool, حجم(ngx_thread_pool_t));
    אם (tp == عدم) {
        החזר عدم;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->line = cf->conf_file->line;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_push(&tcf->pools);
    אם (tpp == عدم) {
        החזר عدم;
    }

    *tpp = tp;

    החזר tp;
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

    עבור (i = 0; i < tcf->pools.nelts; i++) {

        אם (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            החזר tpp[i];
        }
    }

    החזר عدم;
}


ساكن ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    אם (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        החזר NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    אם (tcf == عدم) {
        החזר NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    עבור (i = 0; i < tcf->pools.nelts; i++) {
        אם (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            החזר NGX_ERROR;
        }
    }

    החזר NGX_OK;
}


ساكن فراغ
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    אם (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        החזר;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    אם (tcf == عدم) {
        החזר;
    }

    tpp = tcf->pools.elts;

    עבור (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
