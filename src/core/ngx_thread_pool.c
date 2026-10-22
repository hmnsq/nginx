
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_thread_pool.h>


प्रकारनिर्देश संरचना {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


प्रकारनिर्देश संरचना {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = रिक्त;                                                        \
    (q)->last = &(q)->first


संरचना ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                पङ्क्तिः;
};


स्थिर ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
स्थिर शून्य ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
स्थिर शून्य ngx_thread_pool_exit_handler(शून्य *data, ngx_log_t *log);

स्थिर शून्य *ngx_thread_pool_cycle(शून्य *data);
स्थिर शून्य ngx_thread_pool_handler(ngx_event_t *ev);

स्थिर वर्ण *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);

स्थिर शून्य *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
स्थिर वर्ण *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, शून्य *conf);

स्थिर ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
स्थिर शून्य ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


स्थिर ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      रिक्त },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_str_t  ngx_thread_pool_default = ngx_string("default");

स्थिर ngx_uint_t               ngx_thread_pool_task_id;
स्थिर ngx_atomic_t             ngx_thread_pool_done_lock;
स्थिर ngx_thread_pool_queue_t  ngx_thread_pool_done;


स्थिर ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    पूर्णाङ्क             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    सचे (ngx_notify == रिक्त) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        पटिनिवत्त NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    सचे (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (शून्य) ngx_thread_mutex_destroy(&tp->mtx, log);
        पटिनिवत्त NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    सचे (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        पटिनिवत्त NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    सचे (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        पटिनिवत्त NGX_ERROR;
    }

#सचे 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_राशिः_MIN);
    सचे (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        पटिनिवत्त NGX_ERROR;
    }
#endif

    अत्थाय (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        सचे (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            पटिनिवत्त NGX_ERROR;
        }
    }

    (शून्य) pthread_attr_destroy(&attr);

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    चञ्चल ngx_uint_t  lock;

    ngx_memzero(&task, परिमाण(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (शून्य *) &lock;

    अत्थाय (n = 0; n < tp->threads; n++) {
        lock = 1;

        सचे (ngx_thread_task_post(tp, &task) != NGX_OK) {
            पटिनिवत्त;
        }

        याव (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (शून्य) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (शून्य) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


स्थिर शून्य
ngx_thread_pool_exit_handler(शून्य *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(रिक्त);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, परिमाण(ngx_thread_task_t) + size);
    सचे (task == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    task->ctx = task + 1;

    पटिनिवत्त task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    सचे (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (tp->waiting >= tp->max_queue) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        पटिनिवत्त NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = रिक्त;

    सचे (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        पटिनिवत्त NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य *
ngx_thread_pool_cycle(शून्य *data)
{
    ngx_thread_pool_t *tp = data;

    पूर्णाङ्क                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#सचे 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, रिक्त);
    सचे (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        पटिनिवत्त रिक्त;
    }

    अत्थाय ( ;; ) {
        सचे (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            पटिनिवत्त रिक्त;
        }

        /* the number may become negative */
        tp->waiting--;

        याव (tp->queue.first == रिक्त) {
            सचे (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                पटिनिवत्त रिक्त;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        सचे (tp->queue.first == रिक्त) {
            tp->queue.last = &tp->queue.first;
        }

        सचे (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            पटिनिवत्त रिक्त;
        }

#सचे 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = रिक्त;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (शून्य) ngx_notify(ngx_thread_pool_handler);
    }
}


स्थिर शून्य
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = रिक्त;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    याव (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


स्थिर शून्य *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, परिमाण(ngx_thread_pool_conf_t));
    सचे (tcf == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    सचे (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       परिमाण(ngx_thread_pool_t *))
        != NGX_OK)
    {
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त tcf;
}


स्थिर वर्ण *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, शून्य *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    अत्थाय (i = 0; i < tcf->pools.nelts; i++) {

        सचे (tpp[i]->threads) {
            अनुवर्त;
        }

        सचे (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            अनुवर्त;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->पङ्क्तिः);

        पटिनिवत्त NGX_CONF_ERROR;
    }

    पटिनिवत्त NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t          *मानम्;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    मानम् = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &मानम्[1]);

    सचे (tp == रिक्त) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    सचे (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        पटिनिवत्त NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    अत्थाय (i = 2; i < cf->args->nelts; i++) {

        सचे (ngx_strncmp(मानम्[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(मानम्[i].data + 8, मानम्[i].len - 8);

            सचे (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &मानम्[i]);
                पटिनिवत्त NGX_CONF_ERROR;
            }

            अनुवर्त;
        }

        सचे (ngx_strncmp(मानम्[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(मानम्[i].data + 10, मानम्[i].len - 10);

            सचे (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &मानम्[i]);
                पटिनिवत्त NGX_CONF_ERROR;
            }

            अनुवर्त;
        }
    }

    सचे (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        पटिनिवत्त NGX_CONF_ERROR;
    }

    पटिनिवत्त NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    सचे (name == रिक्त) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    सचे (tp) {
        पटिनिवत्त tp;
    }

    tp = ngx_pcalloc(cf->pool, परिमाण(ngx_thread_pool_t));
    सचे (tp == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_प्रक्षिप(&tcf->pools);
    सचे (tpp == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    *tpp = tp;

    पटिनिवत्त tp;
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

    अत्थाय (i = 0; i < tcf->pools.nelts; i++) {

        सचे (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            पटिनिवत्त tpp[i];
        }
    }

    पटिनिवत्त रिक्त;
}


स्थिर ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    सचे (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        पटिनिवत्त NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    सचे (tcf == रिक्त) {
        पटिनिवत्त NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    अत्थाय (i = 0; i < tcf->pools.nelts; i++) {
        सचे (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            पटिनिवत्त NGX_ERROR;
        }
    }

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    सचे (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        पटिनिवत्त;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    सचे (tcf == रिक्त) {
        पटिनिवत्त;
    }

    tpp = tcf->pools.elts;

    अत्थाय (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
