
/*
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Valentin V. Bartenev
 * Copyright (C) Ruslan Ermilov
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_thread_pool.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_array_t               pools;
} ngx_thread_pool_conf_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_thread_task_t        *ಮೊದಲ;
    ngx_thread_task_t       **ಕೊನೆಯ;
} ngx_thread_pool_ಸರತಿ_t;

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_thread_pool_ಸರತಿ_init(q)                                         \
    (q)->ಮೊದಲ = ಖಾಲಿ;                                                        \
    (q)->ಕೊನೆಯ = &(q)->ಮೊದಲ


ರಚನೆ ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_ಸರತಿ_t   ಸರತಿ;
    ngx_int_t                 ಕಾಯುವಿಕೆ;
    ngx_thread_cond_t         cond;

    ngx_log_t                *ಲಘುಗಣಕ;

    ngx_str_t                 name;
    ngx_uint_t                ಎಳೆಗಳು;
    ngx_int_t                 max_ಸರತಿ;

    u_char                   *file;
    ngx_uint_t                ಸಾಲು;
};


ಸ್ಥಿರ ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *ಲಘುಗಣಕ,
    ngx_pool_t *pool);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_exit_handler(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ngx_log_t *ಲಘುಗಣಕ);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_thread_pool_ಚಕ್ರ(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_handler(ngx_event_t *ev);

ಸ್ಥಿರ ಅಕ್ಷರ *ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_thread_pool_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_thread_pool_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ngx_int_t ngx_thread_pool_init_ಕೆಲಸಗಾರ(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_thread_pool_exit_ಕೆಲಸಗಾರ(ngx_ಚಕ್ರ_t *ಚಕ್ರ);


ಸ್ಥಿರ ngx_command_t  ngx_thread_pool_commands[] = {

    { ngx_string("thread_pool"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_TAKE23,
      ngx_thread_pool,
      0,
      0,
      ಖಾಲಿ },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_thread_pool_module_ctx = {
    ngx_string("thread_pool"),
    ngx_thread_pool_create_conf,
    ngx_thread_pool_init_conf
};


ngx_module_t  ngx_thread_pool_module = {
    NGX_MODULE_V1,
    &ngx_thread_pool_module_ctx,           /* module context */
    ngx_thread_pool_commands,              /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ಖಾಲಿ,                                  /* init master */
    ಖಾಲಿ,                                  /* init module */
    ngx_thread_pool_init_ಕೆಲಸಗಾರ,           /* init process */
    ಖಾಲಿ,                                  /* init thread */
    ಖಾಲಿ,                                  /* exit thread */
    ngx_thread_pool_exit_ಕೆಲಸಗಾರ,           /* exit process */
    ಖಾಲಿ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_str_t  ngx_thread_pool_default = ngx_string("default");

ಸ್ಥಿರ ngx_uint_t               ngx_thread_pool_task_id;
ಸ್ಥಿರ ngx_atomic_t             ngx_thread_pool_ಮುಗಿದ_ಬೀಗ;
ಸ್ಥಿರ ngx_thread_pool_ಸರತಿ_t  ngx_thread_pool_ಮುಗಿದ;


ಸ್ಥಿರ ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_log_t *ಲಘುಗಣಕ, ngx_pool_t *pool)
{
    ಪೂರ್ಣಾಂಕ             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    ಆದರೆ (ngx_notify == ಖಾಲಿ) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, 0,
               "the configured event method cannot be used with thread pools");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_thread_pool_ಸರತಿ_init(&tp->ಸರತಿ);

    ಆದರೆ (ngx_thread_mutex_create(&tp->mtx, ಲಘುಗಣಕ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_thread_cond_create(&tp->cond, ಲಘುಗಣಕ) != NGX_OK) {
        (ಶೂನ್ಯ) ngx_thread_mutex_destroy(&tp->mtx, ಲಘುಗಣಕ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    tp->ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    err = pthread_attr_init(&attr);
    ಆದರೆ (err) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, err,
                      "pthread_attr_init() failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ಆದರೆ (err) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, err,
                      "pthread_attr_setdetachstate() failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

#ಆದರೆ 0
    err = pthread_attr_setstacksize(&attr, PTHREAD_ರಾಶಿ_MIN);
    ಆದರೆ (err) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, err,
                      "pthread_attr_setstacksize() failed");
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ (n = 0; n < tp->ಎಳೆಗಳು; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_ಚಕ್ರ, tp);
        ಆದರೆ (err) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, err,
                          "pthread_create() failed");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    (ಶೂನ್ಯ) pthread_attr_destroy(&attr);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    ಅಸ್ಥಿರ ngx_uint_t  ಬೀಗ;

    ngx_memzero(&task, ಗಾತ್ರ(ngx_thread_task_t));

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (ಶೂನ್ಯ *) &ಬೀಗ;

    ಗಾಗಿ (n = 0; n < tp->ಎಳೆಗಳು; n++) {
        ಬೀಗ = 1;

        ಆದರೆ (ngx_thread_task_post(tp, &task) != NGX_OK) {
            ಹಿಂತಿರುಗಿ;
        }

        ಆಗ (ಬೀಗ) {
            ngx_sched_yield();
        }

        task.event.ಸಕ್ರಿಯ = 0;
    }

    (ಶೂನ್ಯ) ngx_thread_cond_destroy(&tp->cond, tp->ಲಘುಗಣಕ);

    (ಶೂನ್ಯ) ngx_thread_mutex_destroy(&tp->mtx, tp->ಲಘುಗಣಕ);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_exit_handler(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_uint_t *ಬೀಗ = ದತ್ತಾಂಶ;

    *ಬೀಗ = 0;

    pthread_exit(ಖಾಲಿ);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, ಗಾತ್ರ(ngx_thread_task_t) + ಗಾತ್ರ_2);
    ಆದರೆ (task == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    task->ctx = task + 1;

    ಹಿಂತಿರುಗಿ task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    ಆದರೆ (task->event.ಸಕ್ರಿಯ) {
        ngx_log_error(NGX_LOG_ALERT, tp->ಲಘುಗಣಕ, 0,
                      "task #%ui already active", task->id);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_thread_mutex_ಬೀಗ(&tp->mtx, tp->ಲಘುಗಣಕ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (tp->ಕಾಯುವಿಕೆ >= tp->max_ಸರತಿ) {
        (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->ಲಘುಗಣಕ);

        ngx_log_error(NGX_LOG_ERR, tp->ಲಘುಗಣಕ, 0,
                      "thread pool \"%V\" queue overflow: %i tasks waiting",
                      &tp->name, tp->ಕಾಯುವಿಕೆ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    task->event.ಸಕ್ರಿಯ = 1;

    task->id = ngx_thread_pool_task_id++;
    task->ಮುಂದಿನ = ಖಾಲಿ;

    ಆದರೆ (ngx_thread_cond_signal(&tp->cond, tp->ಲಘುಗಣಕ) != NGX_OK) {
        (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->ಲಘುಗಣಕ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    *tp->ಸರತಿ.ಕೊನೆಯ = task;
    tp->ಸರತಿ.ಕೊನೆಯ = &task->ಮುಂದಿನ;

    tp->ಕಾಯುವಿಕೆ++;

    (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->ಲಘುಗಣಕ);

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ಲಘುಗಣಕ, 0,
                   "task #%ui added to thread pool \"%V\"",
                   task->id, &tp->name);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_thread_pool_ಚಕ್ರ(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_thread_pool_t *tp = ದತ್ತಾಂಶ;

    ಪೂರ್ಣಾಂಕ                 err;
    sigset_t            ಗಣ;
    ngx_thread_task_t  *task;

#ಆದರೆ 0
    ngx_ಸಮಯ_update();
#ಆದರೆ_ಅಂತ್ಯ

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, tp->ಲಘುಗಣಕ, 0,
                   "thread in pool \"%V\" started", &tp->name);

    sigfillset(&ಗಣ);

    sigdelset(&ಗಣ, SIGILL);
    sigdelset(&ಗಣ, SIGFPE);
    sigdelset(&ಗಣ, SIGSEGV);
    sigdelset(&ಗಣ, SIGBUS);

    err = pthread_sigmask(SIG_ಖಂಡ, &ಗಣ, ಖಾಲಿ);
    ಆದರೆ (err) {
        ngx_log_error(NGX_LOG_ALERT, tp->ಲಘುಗಣಕ, err, "pthread_sigmask() failed");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಗಾಗಿ ( ;; ) {
        ಆದರೆ (ngx_thread_mutex_ಬೀಗ(&tp->mtx, tp->ಲಘುಗಣಕ) != NGX_OK) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        /* the number may become negative */
        tp->ಕಾಯುವಿಕೆ--;

        ಆಗ (tp->ಸರತಿ.ಮೊದಲ == ಖಾಲಿ) {
            ಆದರೆ (ngx_thread_cond_wait(&tp->cond, &tp->mtx, tp->ಲಘುಗಣಕ)
                != NGX_OK)
            {
                (ಶೂನ್ಯ) ngx_thread_mutex_unlock(&tp->mtx, tp->ಲಘುಗಣಕ);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
        }

        task = tp->ಸರತಿ.ಮೊದಲ;
        tp->ಸರತಿ.ಮೊದಲ = task->ಮುಂದಿನ;

        ಆದರೆ (tp->ಸರತಿ.ಮೊದಲ == ಖಾಲಿ) {
            tp->ಸರತಿ.ಕೊನೆಯ = &tp->ಸರತಿ.ಮೊದಲ;
        }

        ಆದರೆ (ngx_thread_mutex_unlock(&tp->mtx, tp->ಲಘುಗಣಕ) != NGX_OK) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

#ಆದರೆ 0
        ngx_ಸಮಯ_update();
#ಆದರೆ_ಅಂತ್ಯ

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ಲಘುಗಣಕ, 0,
                       "run task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->handler(task->ctx, tp->ಲಘುಗಣಕ);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, tp->ಲಘುಗಣಕ, 0,
                       "complete task #%ui in thread pool \"%V\"",
                       task->id, &tp->name);

        task->ಮುಂದಿನ = ಖಾಲಿ;

        ngx_spinlock(&ngx_thread_pool_ಮುಗಿದ_ಬೀಗ, 1, 2048);

        *ngx_thread_pool_ಮುಗಿದ.ಕೊನೆಯ = task;
        ngx_thread_pool_ಮುಗಿದ.ಕೊನೆಯ = &task->ಮುಂದಿನ;

        ngx_ಸ್ಮರಣೆ_barrier();

        ngx_unlock(&ngx_thread_pool_ಮುಗಿದ_ಬೀಗ);

        (ಶೂನ್ಯ) ngx_notify(ngx_thread_pool_handler);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_handler(ngx_event_t *ev)
{
    ngx_event_t        *event;
    ngx_thread_task_t  *task;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ev->ಲಘುಗಣಕ, 0, "thread pool handler");

    ngx_spinlock(&ngx_thread_pool_ಮುಗಿದ_ಬೀಗ, 1, 2048);

    task = ngx_thread_pool_ಮುಗಿದ.ಮೊದಲ;
    ngx_thread_pool_ಮುಗಿದ.ಮೊದಲ = ಖಾಲಿ;
    ngx_thread_pool_ಮುಗಿದ.ಕೊನೆಯ = &ngx_thread_pool_ಮುಗಿದ.ಮೊದಲ;

    ngx_ಸ್ಮರಣೆ_barrier();

    ngx_unlock(&ngx_thread_pool_ಮುಗಿದ_ಬೀಗ);

    ಆಗ (task) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->ಲಘುಗಣಕ, 0,
                       "run completion handler for task #%ui", task->id);

        event = &task->event;
        task = task->ಮುಂದಿನ;

        event->complete = 1;
        event->ಸಕ್ರಿಯ = 0;

        event->handler(event);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_thread_pool_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_thread_pool_conf_t  *tcf;

    tcf = ngx_pcalloc(ಚಕ್ರ->pool, ಗಾತ್ರ(ngx_thread_pool_conf_t));
    ಆದರೆ (tcf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_array_init(&tcf->pools, ಚಕ್ರ->pool, 4,
                       ಗಾತ್ರ(ngx_thread_pool_t *))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ tcf;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_thread_pool_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf)
{
    ngx_thread_pool_conf_t *tcf = conf;

    ngx_uint_t           i;
    ngx_thread_pool_t  **tpp;

    tpp = tcf->pools.elts;

    ಗಾಗಿ (i = 0; i < tcf->pools.nelts; i++) {

        ಆದರೆ (tpp[i]->ಎಳೆಗಳು) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (tpp[i]->name.len == ngx_thread_pool_default.len
            && ngx_strncmp(tpp[i]->name.ದತ್ತಾಂಶ, ngx_thread_pool_default.ದತ್ತಾಂಶ,
                           ngx_thread_pool_default.len)
               == 0)
        {
            tpp[i]->ಎಳೆಗಳು = 32;
            tpp[i]->max_ಸರತಿ = 65536;
            ಮುಂದುವರಿ;
        }

        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "unknown thread pool \"%V\" in %s:%ui",
                      &tpp[i]->name, tpp[i]->file, tpp[i]->ಸಾಲು);

        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_thread_pool(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_str_t          *ಮೌಲ್ಯ;
    ngx_uint_t          i;
    ngx_thread_pool_t  *tp;

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    tp = ngx_thread_pool_ಕೂಡಿಸು(cf, &ಮೌಲ್ಯ[1]);

    ಆದರೆ (tp == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (tp->ಎಳೆಗಳು) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate thread pool \"%V\"", &tp->name);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    tp->max_ಸರತಿ = 65536;

    ಗಾಗಿ (i = 2; i < cf->ವಾದಗಳು->nelts; i++) {

        ಆದರೆ (ngx_strncmp(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ, "threads=", 8) == 0) {

            tp->ಎಳೆಗಳು = ngx_atoi(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ + 8, ಮೌಲ್ಯ[i].len - 8);

            ಆದರೆ (tp->ಎಳೆಗಳು == (ngx_uint_t) NGX_ERROR || tp->ಎಳೆಗಳು == 0) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid threads value \"%V\"", &ಮೌಲ್ಯ[i]);
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_strncmp(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ, "max_queue=", 10) == 0) {

            tp->max_ಸರತಿ = ngx_atoi(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ + 10, ಮೌಲ್ಯ[i].len - 10);

            ಆದರೆ (tp->max_ಸರತಿ == NGX_ERROR) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "invalid max_queue value \"%V\"", &ಮೌಲ್ಯ[i]);
                ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
            }

            ಮುಂದುವರಿ;
        }
    }

    ಆದರೆ (tp->ಎಳೆಗಳು == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%V\" must have \"threads\" parameter",
                           &cmd->name);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ngx_thread_pool_t *
ngx_thread_pool_ಕೂಡಿಸು(ngx_conf_t *cf, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    ngx_thread_pool_conf_t  *tcf;

    ಆದರೆ (name == ಖಾಲಿ) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(cf->ಚಕ್ರ, name);

    ಆದರೆ (tp) {
        ಹಿಂತಿರುಗಿ tp;
    }

    tp = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_thread_pool_t));
    ಆದರೆ (tp == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    tp->name = *name;
    tp->file = cf->conf_file->file.name.ದತ್ತಾಂಶ;
    tp->ಸಾಲು = cf->conf_file->ಸಾಲು;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(cf->ಚಕ್ರ->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = ngx_array_ತಳ್ಳು(&tcf->pools);
    ಆದರೆ (tpp == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    *tpp = tp;

    ಹಿಂತಿರುಗಿ tp;
}


ngx_thread_pool_t *
ngx_thread_pool_get(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx,
                                                  ngx_thread_pool_module);

    tpp = tcf->pools.elts;

    ಗಾಗಿ (i = 0; i < tcf->pools.nelts; i++) {

        ಆದರೆ (tpp[i]->name.len == name->len
            && ngx_strncmp(tpp[i]->name.ದತ್ತಾಂಶ, name->ದತ್ತಾಂಶ, name->len) == 0)
        {
            ಹಿಂತಿರುಗಿ tpp[i];
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ngx_int_t
ngx_thread_pool_init_ಕೆಲಸಗಾರ(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ಆದರೆ (ngx_process != NGX_PROCESS_ಕೆಲಸಗಾರ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx,
                                                  ngx_thread_pool_module);

    ಆದರೆ (tcf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ngx_thread_pool_ಸರತಿ_init(&ngx_thread_pool_ಮುಗಿದ);

    tpp = tcf->pools.elts;

    ಗಾಗಿ (i = 0; i < tcf->pools.nelts; i++) {
        ಆದರೆ (ngx_thread_pool_init(tpp[i], ಚಕ್ರ->ಲಘುಗಣಕ, ಚಕ್ರ->pool) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_thread_pool_exit_ಕೆಲಸಗಾರ(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       **tpp;
    ngx_thread_pool_conf_t   *tcf;

    ಆದರೆ (ngx_process != NGX_PROCESS_ಕೆಲಸಗಾರ
        && ngx_process != NGX_PROCESS_SINGLE)
    {
        ಹಿಂತಿರುಗಿ;
    }

    tcf = (ngx_thread_pool_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx,
                                                  ngx_thread_pool_module);

    ಆದರೆ (tcf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    tpp = tcf->pools.elts;

    ಗಾಗಿ (i = 0; i < tcf->pools.nelts; i++) {
        ngx_thread_pool_destroy(tpp[i]);
    }
}
