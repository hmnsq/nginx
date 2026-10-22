
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_cleanup(ಶೂನ್ಯ *data);
#ಯದಿ (NGX_HAVE_OPENAT)
ಸ್ಥಿರ ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ಧ್ರುವ u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#ಯದಿ (NGX_HAVE_O_PATH)
ಸ್ಥಿರ ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
ಸ್ಥಿರ ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
ಸ್ಥಿರ ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
ಸ್ಥಿರ ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cleanup(ಶೂನ್ಯ *data);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, ಪರಿಮಾಣ(ngx_open_file_cache_t));
    ಯದಿ (cache == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    ಪ್ರತ್ಯಾಗಮ cache;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_cleanup(ಶೂನ್ಯ *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (ngx_queue_empty(&cache->expire_queue)) {
            ಭಙ್ಗ;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        ಯದಿ (!file->err && !file->is_dir) {
            file->close = 1;
            file->ಎಣಿಕೆ = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } ಅನ್ಯಥಾ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    ಯದಿ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    ಯದಿ (cache->rbtree.ಬೇರು != cache->rbtree.sentinel) {
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

    ಯದಿ (cache == ರಿಕ್ತ) {

        ಯದಿ (of->test_only) {

            ಯದಿ (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, ಪರಿಮಾಣ(ngx_pool_cleanup_file_t));
        ಯದಿ (cln == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        ಯದಿ (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        ಪ್ರತ್ಯಾಗಮ rc;
    }

    cln = ngx_pool_cleanup_add(pool, ಪರಿಮಾಣ(ngx_open_file_cache_cleanup_t));
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    ಯದಿ (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        ಯದಿ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            ಯದಿ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                ಗಚ್ಛ failed;
            }

            ಗಚ್ಛ add_event;
        }

        ಯದಿ (file->use_event
            || (file->event == ರಿಕ್ತ
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#ಯದಿ (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            ಯದಿ (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                ಯದಿ (!file->is_dir) {
                    file->ಎಣಿಕೆ++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } ಅನ್ಯಥಾ {
                of->err = file->err;
#ಯದಿ (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#ಅನ್ಯಥಾ
                of->failed = ngx_open_file_n;
#endif
            }

            ಗಚ್ಛ found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->ಎಣಿಕೆ, file->err);

        ಯದಿ (file->is_dir) {

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

        ಯದಿ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            ಗಚ್ಛ failed;
        }

        ಯದಿ (of->is_dir) {

            ಯದಿ (file->is_dir || file->err) {
                ಗಚ್ಛ update;
            }

            /* file became directory */

        } ಅನ್ಯಥಾ ಯದಿ (of->err == 0) {  /* file */

            ಯದಿ (file->is_dir || file->err) {
                ಗಚ್ಛ add_event;
            }

            ಯದಿ (of->uniq == file->uniq) {

                ಯದಿ (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                ಗಚ್ಛ update;
            }

            /* file was changed */

        } ಅನ್ಯಥಾ { /* error to cache */

            ಯದಿ (file->err || file->is_dir) {
                ಗಚ್ಛ update;
            }

            /* file was removed, etc. */
        }

        ಯದಿ (file->ಎಣಿಕೆ == 0) {

            ngx_open_file_del_event(file);

            ಯದಿ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            ಗಚ್ಛ add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        file->close = 1;

        ಗಚ್ಛ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    ಯದಿ (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        ಗಚ್ಛ failed;
    }

create:

    ಯದಿ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(ಪರಿಮಾಣ(ngx_cached_open_file_t), pool->log);

    ಯದಿ (file == ರಿಕ್ತ) {
        ಗಚ್ಛ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    ಯದಿ (file->name == ರಿಕ್ತ) {
        ngx_free(file);
        file = ರಿಕ್ತ;
        ಗಚ್ಛ failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->ಗಂಟು.ಕೀಲಿ = hash;

    ngx_rbtree_ಸೇರಿಸು(&cache->rbtree, &file->ಗಂಟು);

    cache->current++;

    file->uses = 1;
    file->ಎಣಿಕೆ = 0;
    file->use_event = 0;
    file->event = ರಿಕ್ತ;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#ಯದಿ (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    ಯದಿ (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        ಯದಿ (!of->is_dir) {
            file->ಎಣಿಕೆ++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_ಸೇರಿಸು_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->ಎಣಿಕೆ, file->err, file->uses);

    ಯದಿ (of->err == 0) {

        ಯದಿ (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

failed:

    ಯದಿ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ಯದಿ (file->ಎಣಿಕೆ == 0) {

            ಯದಿ (file->fd != NGX_INVALID_FILE) {
                ಯದಿ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } ಅನ್ಯಥಾ {
            file->close = 1;
        }
    }

    ಯದಿ (of->fd != NGX_INVALID_FILE) {
        ಯದಿ (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


#ಯದಿ (NGX_HAVE_OPENAT)

ಸ್ಥಿರ ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ಧ್ರುವ u_char *name,
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

    ಯದಿ (fd == NGX_INVALID_FILE) {
        ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
    }

    ಯದಿ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        ಗಚ್ಛ failed;
    }

#ಯದಿ (NGX_HAVE_O_PATH)
    ಯದಿ (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        ಗಚ್ಛ failed;
    }
#ಅನ್ಯಥಾ
    ಯದಿ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        ಗಚ್ಛ failed;
    }
#endif

    ಯದಿ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ fd;

failed:

    ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
}


#ಯದಿ (NGX_HAVE_O_PATH)

ಸ್ಥಿರ ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    ಸ್ಥಿರ ngx_uint_t  use_fstat = 1;

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

    ಯದಿ (use_fstat) {
        ಯದಿ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಯದಿ (ngx_errno != NGX_EBADF) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    ಯದಿ (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


ಸ್ಥಿರ ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#ಯದಿ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    ಯದಿ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
    }

    ಪ್ರತ್ಯಾಗಮ fd;

#ಅನ್ಯಥಾ

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    ಯದಿ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        ಯದಿ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
        }

        ಪ್ರತ್ಯಾಗಮ fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    ಯದಿ (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        ಯದಿ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } ಅನ್ಯಥಾ ಯದಿ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        ಯದಿ (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            ಪ್ರತ್ಯಾಗಮ NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } ಅನ್ಯಥಾ {
        at_fd = NGX_AT_FDCWD;
    }

    ಪ್ರತಿ ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        ಯದಿ (cp == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }

        ಯದಿ (cp == p) {
            p++;
            ಅನುವರ್ತ;
        }

        *cp = '\0';

        ಯದಿ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } ಅನ್ಯಥಾ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        ಯದಿ (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            ಗಚ್ಛ failed;
        }

        ಯದಿ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    ಯದಿ (p == end) {

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
        ಗಚ್ಛ done;
    }

    ಯದಿ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } ಅನ್ಯಥಾ {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    ಯದಿ (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    ಯದಿ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    ಪ್ರತ್ಯಾಗಮ fd;
#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#ಯದಿ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    ಯದಿ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        ಪ್ರತ್ಯಾಗಮ NGX_FILE_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ rc;

#ಅನ್ಯಥಾ

    ngx_fd_t  fd;

    ಯದಿ (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        ಯದಿ (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            ಪ್ರತ್ಯಾಗಮ NGX_FILE_ERROR;
        }

        ಪ್ರತ್ಯಾಗಮ rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    ಯದಿ (fd == NGX_INVALID_FILE) {
        ಪ್ರತ್ಯಾಗಮ NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    ಯದಿ (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    ಪ್ರತ್ಯಾಗಮ rc;
#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    ಯದಿ (of->fd != NGX_INVALID_FILE) {

        ಯದಿ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (of->uniq == ngx_file_uniq(&fi)) {
            ಗಚ್ಛ done;
        }

    } ಅನ್ಯಥಾ ಯದಿ (of->test_dir) {

        ಯದಿ (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ngx_is_dir(&fi)) {
            ಗಚ್ಛ done;
        }
    }

    ಯದಿ (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } ಅನ್ಯಥಾ {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    ಯದಿ (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_is_dir(&fi)) {
        ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } ಅನ್ಯಥಾ {
        of->fd = fd;

        ಯದಿ (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            ಯದಿ (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        ಯದಿ (of->directio <= ngx_file_size(&fi)) {
            ಯದಿ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } ಅನ್ಯಥಾ {
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

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    ಯದಿ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    file->use_event = 0;

    file->event = ngx_calloc(ಪರಿಮಾಣ(ngx_event_t), log);
    ಯದಿ (file->event== ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    fev = ngx_alloc(ಪರಿಮಾಣ(ngx_open_file_cache_event_t), log);
    ಯದಿ (fev == ರಿಕ್ತ) {
        ngx_free(file->event);
        file->event = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
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

    ಯದಿ (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    ಪ್ರತ್ಯಾಗಮ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cleanup(ಶೂನ್ಯ *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->ಎಣಿಕೆ--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->ಎಣಿಕೆ, file->uses, file->close);

    ಯದಿ (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_ಸೇರಿಸು_head(&cache->expire_queue, &file->queue);

        ಯದಿ (file->uses >= min_uses || file->ಎಣಿಕೆ) {
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    ngx_open_file_del_event(file);

    ಯದಿ (file->ಎಣಿಕೆ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (file->fd != NGX_INVALID_FILE) {

        ಯದಿ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    ಯದಿ (!file->close) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    ಯದಿ (file->event == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    (ಶೂನ್ಯ) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->ಎಣಿಕೆ ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = ರಿಕ್ತ;
    file->use_event = 0;
}


ಸ್ಥಿರ ಶೂನ್ಯ
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

    ಯಾವತ್ (n < 3) {

        ಯದಿ (ngx_queue_empty(&cache->expire_queue)) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ಯದಿ (n++ != 0 && now - file->accessed <= cache->inactive) {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        ಯದಿ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } ಅನ್ಯಥಾ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (ಗಂಟು->ಕೀಲಿ < temp->ಕೀಲಿ) {

            p = &temp->ಎಡ;

        } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->ಕೀಲಿ > temp->ಕೀಲಿ) {

            p = &temp->ಬಲ;

        } ಅನ್ಯಥಾ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ಗಂಟು;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->ಎಡ : &temp->ಬಲ;
        }

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ಸ್ಥಿರ ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_ಗಂಟು_t       *ಗಂಟು, *sentinel;
    ngx_cached_open_file_t  *file;

    ಗಂಟು = cache->rbtree.ಬೇರು;
    sentinel = cache->rbtree.sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        ಯದಿ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ಗಂಟು;

        rc = ngx_strcmp(name->data, file->name);

        ಯದಿ (rc == 0) {
            ಪ್ರತ್ಯಾಗಮ file;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ಗಂಟು);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ರಿಕ್ತ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
