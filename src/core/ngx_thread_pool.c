
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
    ngx_uint_t                पङ्क्ति;
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

    यदि (ngx_notify == रिक्त) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        फर्काऊ NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    यदि (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (शून्य) ngx_thread_mutex_destroy(&tp->mtx, log);
        फर्काऊ NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        फर्काऊ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        फर्काऊ NGX_ERROR;
    }

#यदि 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_थाक_MIN);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        फर्काऊ NGX_ERROR;
    }
#endif

    लागि (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        यदि (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            फर्काऊ NGX_ERROR;
        }
    }

    (शून्य) pthread_attr_destroy(&attr);

    फर्काऊ NGX_OK;
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

    लागि (n = 0; n < tp->threads; n++) {
        lock = 1;

        यदि (ngx_thread_task_post(tp, &task) != NGX_OK) {
            फर्काऊ;
        }

        जबसम्म (lock) {
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
ngx_thread_task_alloc(ngx_pool_t *pool, माप_प्रकार size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, परिमाण(ngx_thread_task_t) + size);
    यदि (task == रिक्त) {
        फर्काऊ रिक्त;
    }

    task->ctx = task + 1;

    फर्काऊ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    यदि (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        फर्काऊ NGX_ERROR;
    }

    यदि (tp->waiting >= tp->max_queue) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        फर्काऊ NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = रिक्त;

    यदि (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        फर्काऊ NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    फर्काऊ NGX_OK;
}


स्थिर शून्य *
ngx_thread_pool_cycle(शून्य *data)
{
    ngx_thread_pool_t *tp = data;

    पूर्णाङ्क                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#यदि 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, संक_अवैध);
    sigdelset(&set, संक_भ_त्रुटि);
    sigdelset(&set, संक_अंश);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, रिक्त);
    यदि (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        फर्काऊ रिक्त;
    }

    लागि ( ;; ) {
        यदि (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            फर्काऊ रिक्त;
        }

        /* the number may become negative */
        tp->waiting--;

        जबसम्म (tp->queue.first == रिक्त) {
            यदि (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (शून्य) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                फर्काऊ रिक्त;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        यदि (tp->queue.first == रिक्त) {
            tp->queue.last = &tp->queue.first;
        }

        यदि (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            फर्काऊ रिक्त;
        }

#यदि 0
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

    जबसम्म (task) {
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
    यदि (tcf == रिक्त) {
        फर्काऊ रिक्त;
    }

    यदि (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       परिमाण(ngx_thread_pool_t *))
        != NGX_OK)
    {
        फर्काऊ रिक्त;
    }

    फर्काऊ tcf;
}


स्थिर वर्ण *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, शून्य *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    लागि (i = 0; i < tcf->pools.nelts; i++) {

        यदि (tpp[i]->threads) {
            जारी;
        }

        यदि (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            जारी;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->पङ्क्ति);

        फर्काऊ NGX_CONF_ERROR;
    }

    फर्काऊ NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t          *मान;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    मान = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &मान[1]);

    यदि (tp == रिक्त) {
        फर्काऊ NGX_CONF_ERROR;
    }

    यदि (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        फर्काऊ NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    लागि (i = 2; i < cf->args->nelts; i++) {

        यदि (ngx_strncmp(मान[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(मान[i].data + 8, मान[i].len - 8);

            यदि (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &मान[i]);
                फर्काऊ NGX_CONF_ERROR;
            }

            जारी;
        }

        यदि (ngx_strncmp(मान[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(मान[i].data + 10, मान[i].len - 10);

            यदि (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &मान[i]);
                फर्काऊ NGX_CONF_ERROR;
            }

            जारी;
        }
    }

    यदि (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        फर्काऊ NGX_CONF_ERROR;
    }

    फर्काऊ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    यदि (name == रिक्त) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    यदि (tp) {
        फर्काऊ tp;
    }

    tp = ngx_pcalloc(cf->pool, परिमाण(ngx_thread_pool_t));
    यदि (tp == रिक्त) {
        फर्काऊ रिक्त;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->पङ्क्ति = cf->conf_file->पङ्क्ति;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_थप(&tcf->pools);
    यदि (tpp == रिक्त) {
        फर्काऊ रिक्त;
    }

    *tpp = tp;

    फर्काऊ tp;
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

    लागि (i = 0; i < tcf->pools.nelts; i++) {

        यदि (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            फर्काऊ tpp[i];
        }
    }

    फर्काऊ रिक्त;
}


स्थिर ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    यदि (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        फर्काऊ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    यदि (tcf == रिक्त) {
        फर्काऊ NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    लागि (i = 0; i < tcf->pools.nelts; i++) {
        यदि (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            फर्काऊ NGX_ERROR;
        }
    }

    फर्काऊ NGX_OK;
}


स्थिर शून्य
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    यदि (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        फर्काऊ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    यदि (tcf == रिक्त) {
        फर्काऊ;
    }

    tpp = tcf->pools.elts;

    लागि (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
