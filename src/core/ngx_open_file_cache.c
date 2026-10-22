
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


ایستا تهی ngx_open_file_cache_cleanup(تهی *data);
#اگر (NGX_HAVE_OPENAT)
ایستا ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#اگر (NGX_HAVE_O_PATH)
ایستا ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
ایستا ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
ایستا ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
ایستا ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
ایستا تهی ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
ایستا تهی ngx_open_file_cleanup(تهی *data);
ایستا تهی ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
ایستا تهی ngx_open_file_del_event(ngx_cached_open_file_t *file);
ایستا تهی ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
ایستا تهی ngx_open_file_cache_rbtree_درج_مقدار(ngx_rbtree_گره_t *temp,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ایستا ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ایستا تهی ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, اندازه(ngx_open_file_cache_t));
    اگر (cache == پوچ) {
        بازگشت پوچ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_درج_مقدار);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    اگر (cln == پوچ) {
        بازگشت پوچ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    بازگشت cache;
}


ایستا تهی
ngx_open_file_cache_cleanup(تهی *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    برای ( ;; ) {

        اگر (ngx_queue_empty(&cache->expire_queue)) {
            بشکن;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        اگر (!file->err && !file->is_dir) {
            file->close = 1;
            file->شمار = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } وگرنه {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    اگر (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    اگر (cache->rbtree.ریشه != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool)
{
    time_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    of->fd = NGX_INVALID_FILE;
    of->err = 0;

    اگر (cache == پوچ) {

        اگر (of->test_only) {

            اگر (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                بازگشت NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            بازگشت NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, اندازه(ngx_pool_cleanup_file_t));
        اگر (cln == پوچ) {
            بازگشت NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        اگر (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        بازگشت rc;
    }

    cln = ngx_pool_cleanup_add(pool, اندازه(ngx_open_file_cache_cleanup_t));
    اگر (cln == پوچ) {
        بازگشت NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    اگر (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        اگر (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            اگر (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                برو failed;
            }

            برو add_event;
        }

        اگر (file->use_event
            || (file->event == پوچ
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#اگر (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            اگر (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                اگر (!file->is_dir) {
                    file->شمار++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } وگرنه {
                of->err = file->err;
#اگر (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#وگرنه
                of->failed = ngx_open_file_n;
#endif
            }

            برو found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->شمار, file->err);

        اگر (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            of->test_dir = 1;
        }

        of->fd = file->fd;
        of->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, of, pool->log);

        اگر (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            برو failed;
        }

        اگر (of->is_dir) {

            اگر (file->is_dir || file->err) {
                برو update;
            }

            /* file became directory */

        } وگرنه اگر (of->err == 0) {  /* file */

            اگر (file->is_dir || file->err) {
                برو add_event;
            }

            اگر (of->uniq == file->uniq) {

                اگر (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                برو update;
            }

            /* file was changed */

        } وگرنه { /* error to cache */

            اگر (file->err || file->is_dir) {
                برو update;
            }

            /* file was removed, etc. */
        }

        اگر (file->شمار == 0) {

            ngx_open_file_del_event(file);

            اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            برو add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        file->close = 1;

        برو create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    اگر (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        برو failed;
    }

create:

    اگر (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(اندازه(ngx_cached_open_file_t), pool->log);

    اگر (file == پوچ) {
        برو failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    اگر (file->name == پوچ) {
        ngx_free(file);
        file = پوچ;
        برو failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->گره.کلید = hash;

    ngx_rbtree_درج(&cache->rbtree, &file->گره);

    cache->current++;

    file->uses = 1;
    file->شمار = 0;
    file->use_event = 0;
    file->event = پوچ;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#اگر (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    اگر (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        اگر (!of->is_dir) {
            file->شمار++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_درج_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->شمار, file->err, file->uses);

    اگر (of->err == 0) {

        اگر (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        بازگشت NGX_OK;
    }

    بازگشت NGX_ERROR;

failed:

    اگر (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        اگر (file->شمار == 0) {

            اگر (file->fd != NGX_INVALID_FILE) {
                اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } وگرنه {
            file->close = 1;
        }
    }

    اگر (of->fd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    بازگشت NGX_ERROR;
}


#اگر (NGX_HAVE_OPENAT)

ایستا ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ثابت u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_err_t        err;
    ngx_file_info_t  fi, atfi;

    /*
     * To allow symlinks with the same owner, use openat() (followed
     * by fstat()) and fstatat(AT_SYMLINK_NOFOLLOW), and then compare
     * uids between fstat() and fstatat().
     *
     * As there is a race between openat() and fstatat() we don't
     * know if openat() in fact opened symlink or not.  Therefore,
     * we have to compare uids even if fstatat() reports the opened
     * component isn't a symlink (as we don't know whether it was
     * symlink during openat() or not).
     */

    fd = ngx_openat_file(at_fd, name, mode, create, access);

    اگر (fd == NGX_INVALID_FILE) {
        بازگشت NGX_INVALID_FILE;
    }

    اگر (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        برو failed;
    }

#اگر (NGX_HAVE_O_PATH)
    اگر (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        برو failed;
    }
#وگرنه
    اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        برو failed;
    }
#endif

    اگر (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        برو failed;
    }

    بازگشت fd;

failed:

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    بازگشت NGX_INVALID_FILE;
}


#اگر (NGX_HAVE_O_PATH)

ایستا ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    ایستا ngx_uint_t  use_fstat = 1;

    /*
     * In Linux 2.6.39 the O_PATH flag was introduced that allows to obtain
     * a descriptor without actually opening file or directory.  It requires
     * less permissions for path components, but till Linux 3.6 fstat() returns
     * EBADF on such descriptors, and fstatat() with the AT_EMPTY_PATH flag
     * should be used instead.
     *
     * Three scenarios are handled in this function:
     *
     * 1) The kernel is newer than 3.6 or fstat() with O_PATH support was
     *    backported by vendor.  Then fstat() is used.
     *
     * 2) The kernel is newer than 2.6.39 but older than 3.6.  In this case
     *    the first call of fstat() returns EBADF and we fallback to fstatat()
     *    with AT_EMPTY_PATH which was introduced at the same time as O_PATH.
     *
     * 3) The kernel is older than 2.6.39 but nginx was build with O_PATH
     *    support.  Since descriptors are opened with O_PATH|O_RDONLY flags
     *    and O_PATH is ignored by the kernel then the O_RDONLY flag is
     *    actually used.  In this case fstat() just works.
     */

    اگر (use_fstat) {
        اگر (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            بازگشت NGX_OK;
        }

        اگر (ngx_errno != NGX_EBADF) {
            بازگشت NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    اگر (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        بازگشت NGX_OK;
    }

    بازگشت NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


ایستا ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#اگر !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    اگر (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        بازگشت NGX_INVALID_FILE;
    }

    بازگشت fd;

#وگرنه

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    اگر (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        اگر (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            بازگشت NGX_INVALID_FILE;
        }

        بازگشت fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    اگر (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        اگر (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            بازگشت NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } وگرنه اگر (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        اگر (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            بازگشت NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } وگرنه {
        at_fd = NGX_AT_FDCWD;
    }

    برای ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        اگر (cp == پوچ) {
            بشکن;
        }

        اگر (cp == p) {
            p++;
            ادامه;
        }

        *cp = '\0';

        اگر (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_جستجو|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } وگرنه {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_جستجو|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        اگر (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            برو failed;
        }

        اگر (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    اگر (p == end) {

        /*
         * If pathname ends with a trailing slash, assume the last path
         * component is a directory and reopen it with requested flags;
         * if not, fail with ENOTDIR as per POSIX.
         *
         * We cannot rely on O_DIRECTORY in the loop above to check
         * that the last path component is a directory because
         * O_DIRECTORY doesn't work on FreeBSD 8.  Fortunately, by
         * reopening a directory, we don't depend on it at all.
         */

        fd = ngx_openat_file(at_fd, ".", mode, create, access);
        برو done;
    }

    اگر (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } وگرنه {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    اگر (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    اگر (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    بازگشت fd;
#endif
}


ایستا ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#اگر !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    اگر (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        بازگشت NGX_FILE_ERROR;
    }

    بازگشت rc;

#وگرنه

    ngx_fd_t  fd;

    اگر (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        اگر (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            بازگشت NGX_FILE_ERROR;
        }

        بازگشت rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    اگر (fd == NGX_INVALID_FILE) {
        بازگشت NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    اگر (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    بازگشت rc;
#endif
}


ایستا ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    اگر (of->fd != NGX_INVALID_FILE) {

        اگر (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            بازگشت NGX_ERROR;
        }

        اگر (of->uniq == ngx_file_uniq(&fi)) {
            برو done;
        }

    } وگرنه اگر (of->test_dir) {

        اگر (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            بازگشت NGX_ERROR;
        }

        اگر (ngx_is_dir(&fi)) {
            برو done;
        }
    }

    اگر (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } وگرنه {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    اگر (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        بازگشت NGX_ERROR;
    }

    اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        بازگشت NGX_ERROR;
    }

    اگر (ngx_is_dir(&fi)) {
        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } وگرنه {
        of->fd = fd;

        اگر (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            اگر (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        اگر (of->directio <= ngx_file_size(&fi)) {
            اگر (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } وگرنه {
                of->is_directio = 1;
            }
        }
    }

done:

    of->uniq = ngx_file_uniq(&fi);
    of->mtime = ngx_file_mtime(&fi);
    of->size = ngx_file_size(&fi);
    of->fs_size = ngx_file_fs_size(&fi);
    of->is_dir = ngx_is_dir(&fi);
    of->is_file = ngx_is_file(&fi);
    of->is_link = ngx_is_link(&fi);
    of->is_exec = ngx_is_exec(&fi);

    بازگشت NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ایستا تهی
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    اگر (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        بازگشت;
    }

    file->use_event = 0;

    file->event = ngx_calloc(اندازه(ngx_event_t), log);
    اگر (file->event== پوچ) {
        بازگشت;
    }

    fev = ngx_alloc(اندازه(ngx_open_file_cache_event_t), log);
    اگر (fev == پوچ) {
        ngx_free(file->event);
        file->event = پوچ;
        بازگشت;
    }

    fev->fd = of->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->data = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->log = ngx_cycle->log;

    اگر (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = پوچ;
        بازگشت;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    بازگشت;
}


ایستا تهی
ngx_open_file_cleanup(تهی *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->شمار--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


ایستا تهی
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->شمار, file->uses, file->close);

    اگر (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_درج_head(&cache->expire_queue, &file->queue);

        اگر (file->uses >= min_uses || file->شمار) {
            بازگشت;
        }
    }

    ngx_open_file_del_event(file);

    اگر (file->شمار) {
        بازگشت;
    }

    اگر (file->fd != NGX_INVALID_FILE) {

        اگر (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    اگر (!file->close) {
        بازگشت;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ایستا تهی
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    اگر (file->event == پوچ) {
        بازگشت;
    }

    (تهی) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->شمار ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = پوچ;
    file->use_event = 0;
}


ایستا تهی
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *log)
{
    time_t                   now;
    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_time();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    وقتی (n < 3) {

        اگر (ngx_queue_empty(&cache->expire_queue)) {
            بازگشت;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        اگر (n++ != 0 && now - file->accessed <= cache->inactive) {
            بازگشت;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->گره);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        اگر (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } وگرنه {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ایستا تهی
ngx_open_file_cache_rbtree_درج_مقدار(ngx_rbtree_گره_t *temp,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    برای ( ;; ) {

        اگر (گره->کلید < temp->کلید) {

            p = &temp->چپ;

        } وگرنه اگر (گره->کلید > temp->کلید) {

            p = &temp->راست;

        } وگرنه { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) گره;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->چپ : &temp->راست;
        }

        اگر (*p == sentinel) {
            بشکن;
        }

        temp = *p;
    }

    *p = گره;
    گره->parent = temp;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


ایستا ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_گره_t       *گره, *sentinel;
    ngx_cached_open_file_t  *file;

    گره = cache->rbtree.ریشه;
    sentinel = cache->rbtree.sentinel;

    وقتی (گره != sentinel) {

        اگر (hash < گره->کلید) {
            گره = گره->چپ;
            ادامه;
        }

        اگر (hash > گره->کلید) {
            گره = گره->راست;
            ادامه;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) گره;

        rc = ngx_strcmp(name->data, file->name);

        اگر (rc == 0) {
            بازگشت file;
        }

        گره = (rc < 0) ? گره->چپ : گره->راست;
    }

    بازگشت پوچ;
}


ایستا تهی
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->گره);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = پوچ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
