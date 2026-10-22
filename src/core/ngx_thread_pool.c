
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_thread_pool.h>


প্রকারনির্দেশ সংরচনা {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


প্রকারনির্দেশ সংরচনা {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = রিক্ত;                                                        \
    (q)->last = &(q)->first


সংরচনা ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_log_t                *log;

    ngx_str_t                 name;
    ngx_uint_t                threads;
    ngx_int_t                 max_queue;

    u_char                   *file;
    ngx_uint_t                সারি;
};


স্থির ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log,
    ngx_pool_t *pool);
স্থির শূন্য ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
স্থির শূন্য ngx_thread_pool_exit_handler(শূন্য *data, ngx_log_t *log);

স্থির শূন্য *ngx_thread_pool_cycle(শূন্য *data);
স্থির শূন্য ngx_thread_pool_handler(ngx_event_t *ev);

স্থির বর্ণ *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);

স্থির শূন্য *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
স্থির বর্ণ *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, শূন্য *conf);

স্থির ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
স্থির শূন্য ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


স্থির ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      রিক্ত },

      ngx_null_command
};


স্থির ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    রিক্ত,                                  /* init master */
    রিক্ত,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    রিক্ত,                                  /* init thread */
    রিক্ত,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    রিক্ত,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_str_t  ngx_thread_pool_default = ngx_string("default");

স্থির ngx_uint_t               ngx_thread_pool_task_id;
স্থির ngx_atomic_t             ngx_thread_pool_done_lock;
স্থির ngx_thread_pool_queue_t  ngx_thread_pool_done;


স্থির ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    পূর্ণাঙ্ক             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    যদি (ngx_notify == রিক্ত) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        প্রত্যাগম NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    যদি (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (শূন্য) ngx_thread_mutex_destroy(&tp->mtx, log);
        প্রত্যাগম NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        প্রত্যাগম NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        প্রত্যাগম NGX_ERROR;
    }

#যদি 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_স্তূপ_MIN);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        প্রত্যাগম NGX_ERROR;
    }
#endif

    প্রতি (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        যদি (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            প্রত্যাগম NGX_ERROR;
        }
    }

    (শূন্য) pthread_attr_destroy(&attr);

    প্রত্যাগম NGX_OK;
}


স্থির শূন্য
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    চঞ্চল ngx_uint_t  lock;

    ngx_memzero(&task, পরিমাণ(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (শূন্য *) &lock;

    প্রতি (n = 0; n < tp->threads; n++) {
        lock = 1;

        যদি (ngx_thread_task_post(tp, &task) != NGX_OK) {
            প্রত্যাগম;
        }

        যাবত্ (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (শূন্য) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (শূন্য) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


স্থির শূন্য
ngx_thread_pool_exit_handler(শূন্য *data, ngx_log_t *log)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(রিক্ত);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, পরিমাণ(ngx_thread_task_t) + size);
    যদি (task == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    task->ctx = task + 1;

    প্রত্যাগম task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    যদি (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        প্রত্যাগম NGX_ERROR;
    }

    যদি (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (tp->waiting >= tp->max_queue) {
        (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        প্রত্যাগম NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->next = রিক্ত;

    যদি (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        প্রত্যাগম NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    প্রত্যাগম NGX_OK;
}


স্থির শূন্য *
ngx_thread_pool_cycle(শূন্য *data)
{
    ngx_thread_pool_t *tp = data;

    পূর্ণাঙ্ক                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

#যদি 0
    ngx_time_update();
#endif

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, রিক্ত);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        প্রত্যাগম রিক্ত;
    }

    প্রতি ( ;; ) {
        যদি (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            প্রত্যাগম রিক্ত;
        }

        /* the number may become negative */
        tp->waiting--;

        যাবত্ (tp->queue.first == রিক্ত) {
            যদি (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                প্রত্যাগম রিক্ত;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        যদি (tp->queue.first == রিক্ত) {
            tp->queue.last = &tp->queue.first;
        }

        যদি (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            প্রত্যাগম রিক্ত;
        }

#যদি 0
        ngx_time_update();
#endif

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->log);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->next = রিক্ত;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_memory_barrier();

        ngx_unlock(&ngx_thread_pool_done_lock);

        (শূন্য) ngx_notify(ngx_thread_pool_handler);
    }
}


স্থির শূন্য
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->log, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = রিক্ত;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    যাবত্ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->next;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


স্থির শূন্য *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, পরিমাণ(ngx_thread_pool_conf_t));
    যদি (tcf == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    যদি (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       পরিমাণ(ngx_thread_pool_t *))
        != NGX_OK)
    {
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম tcf;
}


স্থির বর্ণ *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, শূন্য *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    প্রতি (i = 0; i < tcf->pools.nelts; i++) {

        যদি (tpp[i]->threads) {
            অনুবর্ত;
        }

        যদি (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.data, ngx_thread_pool_default.data,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            অনুবর্ত;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->সারি);

        প্রত্যাগম NGX_CONF_ERROR;
    }

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_str_t          *মান;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    মান = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &মান[1]);

    যদি (tp == রিক্ত) {
        প্রত্যাগম NGX_CONF_ERROR;
    }

    যদি (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        প্রত্যাগম NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    প্রতি (i = 2; i < cf->args->nelts; i++) {

        যদি (ngx_strncmp(মান[i].data, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(মান[i].data + 8, মান[i].len - 8);

            যদি (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &মান[i]);
                প্রত্যাগম NGX_CONF_ERROR;
            }

            অনুবর্ত;
        }

        যদি (ngx_strncmp(মান[i].data, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(মান[i].data + 10, মান[i].len - 10);

            যদি (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &মান[i]);
                প্রত্যাগম NGX_CONF_ERROR;
            }

            অনুবর্ত;
        }
    }

    যদি (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        প্রত্যাগম NGX_CONF_ERROR;
    }

    প্রত্যাগম NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    যদি (name == রিক্ত) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    যদি (tp) {
        প্রত্যাগম tp;
    }

    tp = ngx_pcalloc(cf->pool, পরিমাণ(ngx_thread_pool_t));
    যদি (tp == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.data;
    tp->সারি = cf->conf_file->সারি;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ঠেলো(&tcf->pools);
    যদি (tpp == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    *tpp = tp;

    প্রত্যাগম tp;
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

    প্রতি (i = 0; i < tcf->pools.nelts; i++) {

        যদি (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.data, name->data, name->len) == 0)
        {
            প্রত্যাগম tpp[i];
        }
    }

    প্রত্যাগম রিক্ত;
}


স্থির ngx_int_t
ngx_thread_pool_init_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    যদি (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        প্রত্যাগম NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    যদি (tcf == রিক্ত) {
        প্রত্যাগম NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    প্রতি (i = 0; i < tcf->pools.nelts; i++) {
        যদি (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            প্রত্যাগম NGX_ERROR;
        }
    }

    প্রত্যাগম NGX_OK;
}


স্থির শূন্য
ngx_thread_pool_exit_worker(ngx_cycle_t *cycle)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    যদি (ngx_process != NGX_PROCESS_WORKER
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        প্রত্যাগম;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    যদি (tcf == রিক্ত) {
        প্রত্যাগম;
    }

    tpp = tcf->pools.elts;

    প্রতি (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
