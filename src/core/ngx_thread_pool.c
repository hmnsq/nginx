
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_thread_pool.h>


প্রকারলেখ কাঠামো {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


প্রকারলেখ কাঠামো {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = ফাঁকা;                                                        \
    (q)->last = &(q)->first


কাঠামো ngx_thread_pool_s {
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
স্থির শূন্য ngx_thread_pool_exit_handler(শূন্য *তথ্য, ngx_log_t *log);

স্থির শূন্য *ngx_thread_pool_cycle(শূন্য *তথ্য);
স্থির শূন্য ngx_thread_pool_handler(ngx_event_t *ev);

স্থির অক্ষর *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);

স্থির শূন্য *ngx_thread_pool_create_conf(ngx_cycle_t *cycle);
স্থির অক্ষর *ngx_thread_pool_init_conf(ngx_cycle_t *cycle, শূন্য *conf);

স্থির ngx_int_t ngx_thread_pool_init_worker(ngx_cycle_t *cycle);
স্থির শূন্য ngx_thread_pool_exit_worker(ngx_cycle_t *cycle);


স্থির ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ফাঁকা },

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
    ফাঁকা,                                  /* init master */
    ফাঁকা,                                  /* init module */
    ngx_thread_pool_init_worker,           /* init process */
    ফাঁকা,                                  /* init thread */
    ফাঁকা,                                  /* exit thread */
    ngx_thread_pool_exit_worker,           /* exit process */
    ফাঁকা,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_str_t  ngx_thread_pool_default = ngx_string("default");

স্থির ngx_uint_t               ngx_thread_pool_task_id;
স্থির ngx_atomic_t             ngx_thread_pool_done_lock;
স্থির ngx_thread_pool_queue_t  ngx_thread_pool_done;


স্থির ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *log, ngx_pool_t *pool)
{
    পূর্ণসংখ্যা             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    যদি (ngx_notify == ফাঁকা) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
               "the configured event method cannot be used with thread pools");
        ফেরত NGX_ERROR;
    }

    ngx_thread_pool_queue_init(&tp->queue);

    যদি (ngx_thread_mutex_create(&tp->mtx, log) != NGX_OK) {
        ফেরত NGX_ERROR;
    }

    যদি (ngx_thread_cond_create(&tp->cond, log) != NGX_OK) {
        (শূন্য) ngx_thread_mutex_destroy(&tp->mtx, log);
        ফেরত NGX_ERROR;
    }

    tp->log = log;

    err = pthread_attr_init(&attr);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_init() failed");
        ফেরত NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setdetachstate() failed");
        ফেরত NGX_ERROR;
    }

#যদি 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_স্তূপ_MIN);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, log, err,
                      "pthread_attr_setstacksize() failed");
        ফেরত NGX_ERROR;
    }
#endif

    জন্য (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        যদি (err) {
            ngx_log_error(NGX_LOG_ALERT, log, err,
                          "pthread_create() failed");
            ফেরত NGX_ERROR;
        }
    }

    (শূন্য) pthread_attr_destroy(&attr);

    ফেরত NGX_OK;
}


স্থির শূন্য
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    অস্থির ngx_uint_t  lock;

    ngx_memzero(&task, আকার(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (শূন্য *) &lock;

    জন্য (n = 0; n < tp->threads; n++) {
        lock = 1;

        যদি (ngx_thread_task_post(tp, &task) != NGX_OK) {
            ফেরত;
        }

        যখন (lock) {
            ngx_sched_yield();
        }

        task.event.active = 0;
    }

    (শূন্য) ngx_thread_cond_destroy(&tp->cond, tp->log);

    (শূন্য) ngx_thread_mutex_destroy(&tp->mtx, tp->log);
}


স্থির শূন্য
ngx_thread_pool_exit_handler(শূন্য *তথ্য, ngx_log_t *log)
{
    ngx_uint_t *lock = তথ্য;

    *lock = 0;

    pthread_exit(ফাঁকা);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, আকার_t আকার_2)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, আকার(ngx_thread_task_t) + আকার_2);
    যদি (task == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    task->ctx = task + 1;

    ফেরত task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    যদি (task->event.active) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, 0,
                      "task #%ui already active", task->id);
        ফেরত NGX_ERROR;
    }

    যদি (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
        ফেরত NGX_ERROR;
    }

    যদি (tp->waiting >= tp->max_queue) {
        (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

        ngx_log_error(NGX_LOG_ERR, tp->log, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->waiting);
        ফেরত NGX_ERROR;
    }

    task->event.active = 1;

    task->id = ngx_thread_pool_task_id++;
    task->পরবর্তী = ফাঁকা;

    যদি (ngx_thread_cond_signal(&tp->cond, tp->log) != NGX_OK) {
        (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
        ফেরত NGX_ERROR;
    }

    *tp->queue.last = task;
    tp->queue.last = &task->পরবর্তী;

    tp->waiting++;

    (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->log, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    ফেরত NGX_OK;
}


স্থির শূন্য *
ngx_thread_pool_cycle(শূন্য *তথ্য)
{
    ngx_thread_pool_t *tp = তথ্য;

    পূর্ণসংখ্যা                 err;
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

    err = pthread_sigmask(SIG_BLOCK, &set, ফাঁকা);
    যদি (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->log, err, "pthread_sigmask() failed");
        ফেরত ফাঁকা;
    }

    জন্য ( ;; ) {
        যদি (ngx_thread_mutex_lock(&tp->mtx, tp->log) != NGX_OK) {
            ফেরত ফাঁকা;
        }

        /* the number may become negative */
        tp->waiting--;

        যখন (tp->queue.first == ফাঁকা) {
            যদি (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->log)
                != NGX_OK)
            {
                (শূন্য) ngx_thread_mutex_unlock(&tp->mtx, tp->log);
                ফেরত ফাঁকা;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->পরবর্তী;

        যদি (tp->queue.first == ফাঁকা) {
            tp->queue.last = &tp->queue.first;
        }

        যদি (ngx_thread_mutex_unlock(&tp->mtx, tp->log) != NGX_OK) {
            ফেরত ফাঁকা;
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

        task->পরবর্তী = ফাঁকা;

        ngx_spinlock(&ngx_thread_pool_done_lock, 1, 2048);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->পরবর্তী;

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
    ngx_thread_pool_done.first = ফাঁকা;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;

    ngx_memory_barrier();

    ngx_unlock(&ngx_thread_pool_done_lock);

    যখন (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->পরবর্তী;

        event->complete = 1;
        event->active = 0;

        event->handler(event);
    }
}


স্থির শূন্য *
ngx_thread_pool_create_conf(ngx_cycle_t *cycle)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(cycle->pool, আকার(ngx_thread_pool_conf_t));
    যদি (tcf == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যদি (ngx_array_init(&tcf->pools, cycle->pool, 4,
                       আকার(ngx_thread_pool_t *))
        != NGX_OK)
    {
        ফেরত ফাঁকা;
    }

    ফেরত tcf;
}


স্থির অক্ষর *
ngx_thread_pool_init_conf(ngx_cycle_t *cycle, শূন্য *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    জন্য (i = 0; i < tcf->pools.nelts; i++) {

        যদি (tpp[i]->threads) {
            চালু;
        }

        যদি (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.তথ্য, ngx_thread_pool_default.তথ্য,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->threads = 32;
            tpp[i]->max_queue = 65536;
            চালু;
        }

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->সারি);

        ফেরত NGX_CONF_ERROR;
    }

    ফেরত NGX_CONF_OK;
}


স্থির অক্ষর *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_str_t          *মান;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    মান = cf->args->elts;

    tp = ngx_thread_pool_add(cf, &মান[1]);

    যদি (tp == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    যদি (tp->threads) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        ফেরত NGX_CONF_ERROR;
    }

    tp->max_queue = 65536;

    জন্য (i = 2; i < cf->args->nelts; i++) {

        যদি (ngx_strncmp(মান[i].তথ্য, "threads=", 8) == 0) {

            tp->threads = ngx_atoi(মান[i].তথ্য + 8, মান[i].len - 8);

            যদি (tp->threads == (ngx_uint_t) NGX_ERROR || tp->threads == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &মান[i]);
                ফেরত NGX_CONF_ERROR;
            }

            চালু;
        }

        যদি (ngx_strncmp(মান[i].তথ্য, "max_queue=", 10) == 0) {

            tp->max_queue = ngx_atoi(মান[i].তথ্য + 10, মান[i].len - 10);

            যদি (tp->max_queue == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &মান[i]);
                ফেরত NGX_CONF_ERROR;
            }

            চালু;
        }
    }

    যদি (tp->threads == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        ফেরত NGX_CONF_ERROR;
    }

    ফেরত NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_add(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    যদি (name == ফাঁকা) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->cycle, name);

    যদি (tp) {
        ফেরত tp;
    }

    tp = ngx_pcalloc(cf->pool, আকার(ngx_thread_pool_t));
    যদি (tp == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.তথ্য;
    tp->সারি = cf->conf_file->সারি;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ঠেলো(&tcf->pools);
    যদি (tpp == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    *tpp = tp;

    ফেরত tp;
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

    জন্য (i = 0; i < tcf->pools.nelts; i++) {

        যদি (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.তথ্য, name->তথ্য, name->len) == 0)
        {
            ফেরত tpp[i];
        }
    }

    ফেরত ফাঁকা;
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
        ফেরত NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    যদি (tcf == ফাঁকা) {
        ফেরত NGX_OK;
    }

    ngx_thread_pool_queue_init(&ngx_thread_pool_done);

    tpp = tcf->pools.elts;

    জন্য (i = 0; i < tcf->pools.nelts; i++) {
        যদি (ngx_thread_pool_init(tpp[i], cycle->log, cycle->pool) != NGX_OK) {
            ফেরত NGX_ERROR;
        }
    }

    ফেরত NGX_OK;
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
        ফেরত;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                  ngx_thread_pool_module);

    যদি (tcf == ফাঁকা) {
        ফেরত;
    }

    tpp = tcf->pools.elts;

    জন্য (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
