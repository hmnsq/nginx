
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


સ્થિર શૂન્ય ngx_open_file_cache_cleanup(શૂન્ય *data);
#જો (NGX_HAVE_OPENAT)
સ્થિર ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, અચલ u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#જો (NGX_HAVE_O_PATH)
સ્થિર ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
સ્થિર ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
સ્થિર ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
સ્થિર ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
સ્થિર શૂન્ય ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
સ્થિર શૂન્ય ngx_open_file_cleanup(શૂન્ય *data);
સ્થિર શૂન્ય ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
સ્થિર શૂન્ય ngx_open_file_del_event(ngx_cached_open_file_t *file);
સ્થિર શૂન્ય ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
સ્થિર શૂન્ય ngx_open_file_cache_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);
સ્થિર ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
સ્થિર શૂન્ય ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, કદ(ngx_open_file_cache_t));
    જો (cache == ખાલી) {
        પાછા ખાલી;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ઉમેરો_કિંમત);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    જો (cln == ખાલી) {
        પાછા ખાલી;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    પાછા cache;
}


સ્થિર શૂન્ય
ngx_open_file_cache_cleanup(શૂન્ય *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    માટે ( ;; ) {

        જો (ngx_queue_empty(&cache->expire_queue)) {
            તોડો;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ગાંઠ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        જો (!file->err && !file->is_dir) {
            file->close = 1;
            file->ગણતરી = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } નહીંતર {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    જો (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    જો (cache->rbtree.મૂળ != cache->rbtree.sentinel) {
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

    જો (cache == ખાલી) {

        જો (of->test_only) {

            જો (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                પાછા NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            પાછા NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, કદ(ngx_pool_cleanup_file_t));
        જો (cln == ખાલી) {
            પાછા NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        જો (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        પાછા rc;
    }

    cln = ngx_pool_cleanup_add(pool, કદ(ngx_open_file_cache_cleanup_t));
    જો (cln == ખાલી) {
        પાછા NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    જો (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        જો (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            જો (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                જાઓ failed;
            }

            જાઓ add_event;
        }

        જો (file->use_event
            || (file->event == ખાલી
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#જો (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            જો (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                જો (!file->is_dir) {
                    file->ગણતરી++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } નહીંતર {
                of->err = file->err;
#જો (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#નહીંતર
                of->failed = ngx_open_file_n;
#endif
            }

            જાઓ found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->ગણતરી, file->err);

        જો (file->is_dir) {

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

        જો (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            જાઓ failed;
        }

        જો (of->is_dir) {

            જો (file->is_dir || file->err) {
                જાઓ update;
            }

            /* file became directory */

        } નહીંતર જો (of->err == 0) {  /* file */

            જો (file->is_dir || file->err) {
                જાઓ add_event;
            }

            જો (of->uniq == file->uniq) {

                જો (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                જાઓ update;
            }

            /* file was changed */

        } નહીંતર { /* error to cache */

            જો (file->err || file->is_dir) {
                જાઓ update;
            }

            /* file was removed, etc. */
        }

        જો (file->ગણતરી == 0) {

            ngx_open_file_del_event(file);

            જો (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            જાઓ add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ગાંઠ);

        cache->current--;

        file->close = 1;

        જાઓ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    જો (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        જાઓ failed;
    }

create:

    જો (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(કદ(ngx_cached_open_file_t), pool->log);

    જો (file == ખાલી) {
        જાઓ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    જો (file->name == ખાલી) {
        ngx_free(file);
        file = ખાલી;
        જાઓ failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->ગાંઠ.ચાવી = hash;

    ngx_rbtree_ઉમેરો(&cache->rbtree, &file->ગાંઠ);

    cache->current++;

    file->uses = 1;
    file->ગણતરી = 0;
    file->use_event = 0;
    file->event = ખાલી;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#જો (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    જો (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        જો (!of->is_dir) {
            file->ગણતરી++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_ઉમેરો_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->ગણતરી, file->err, file->uses);

    જો (of->err == 0) {

        જો (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        પાછા NGX_OK;
    }

    પાછા NGX_ERROR;

failed:

    જો (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ગાંઠ);

        cache->current--;

        જો (file->ગણતરી == 0) {

            જો (file->fd != NGX_INVALID_FILE) {
                જો (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } નહીંતર {
            file->close = 1;
        }
    }

    જો (of->fd != NGX_INVALID_FILE) {
        જો (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    પાછા NGX_ERROR;
}


#જો (NGX_HAVE_OPENAT)

સ્થિર ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, અચલ u_char *name,
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

    જો (fd == NGX_INVALID_FILE) {
        પાછા NGX_INVALID_FILE;
    }

    જો (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        જાઓ failed;
    }

#જો (NGX_HAVE_O_PATH)
    જો (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        જાઓ failed;
    }
#નહીંતર
    જો (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        જાઓ failed;
    }
#endif

    જો (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        જાઓ failed;
    }

    પાછા fd;

failed:

    જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    પાછા NGX_INVALID_FILE;
}


#જો (NGX_HAVE_O_PATH)

સ્થિર ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    સ્થિર ngx_uint_t  use_fstat = 1;

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

    જો (use_fstat) {
        જો (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            પાછા NGX_OK;
        }

        જો (ngx_errno != NGX_EBADF) {
            પાછા NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    જો (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        પાછા NGX_OK;
    }

    પાછા NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


સ્થિર ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#જો !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    જો (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        પાછા NGX_INVALID_FILE;
    }

    પાછા fd;

#નહીંતર

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    જો (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        જો (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            પાછા NGX_INVALID_FILE;
        }

        પાછા fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    જો (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_શોધ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        જો (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            પાછા NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } નહીંતર જો (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_શોધ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        જો (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            પાછા NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } નહીંતર {
        at_fd = NGX_AT_FDCWD;
    }

    માટે ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        જો (cp == ખાલી) {
            તોડો;
        }

        જો (cp == p) {
            p++;
            ચાલુ;
        }

        *cp = '\0';

        જો (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_શોધ|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } નહીંતર {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_શોધ|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        જો (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            જાઓ failed;
        }

        જો (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    જો (p == end) {

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
        જાઓ done;
    }

    જો (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } નહીંતર {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    જો (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    જો (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    પાછા fd;
#endif
}


સ્થિર ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#જો !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    જો (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        પાછા NGX_FILE_ERROR;
    }

    પાછા rc;

#નહીંતર

    ngx_fd_t  fd;

    જો (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        જો (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            પાછા NGX_FILE_ERROR;
        }

        પાછા rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    જો (fd == NGX_INVALID_FILE) {
        પાછા NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    જો (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    પાછા rc;
#endif
}


સ્થિર ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    જો (of->fd != NGX_INVALID_FILE) {

        જો (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            પાછા NGX_ERROR;
        }

        જો (of->uniq == ngx_file_uniq(&fi)) {
            જાઓ done;
        }

    } નહીંતર જો (of->test_dir) {

        જો (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            પાછા NGX_ERROR;
        }

        જો (ngx_is_dir(&fi)) {
            જાઓ done;
        }
    }

    જો (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } નહીંતર {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    જો (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        પાછા NGX_ERROR;
    }

    જો (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        પાછા NGX_ERROR;
    }

    જો (ngx_is_dir(&fi)) {
        જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } નહીંતર {
        of->fd = fd;

        જો (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            જો (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        જો (of->directio <= ngx_file_size(&fi)) {
            જો (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } નહીંતર {
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

    પાછા NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

સ્થિર શૂન્ય
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    જો (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        પાછા;
    }

    file->use_event = 0;

    file->event = ngx_calloc(કદ(ngx_event_t), log);
    જો (file->event== ખાલી) {
        પાછા;
    }

    fev = ngx_alloc(કદ(ngx_open_file_cache_event_t), log);
    જો (fev == ખાલી) {
        ngx_free(file->event);
        file->event = ખાલી;
        પાછા;
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

    જો (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = ખાલી;
        પાછા;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    પાછા;
}


સ્થિર શૂન્ય
ngx_open_file_cleanup(શૂન્ય *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->ગણતરી--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


સ્થિર શૂન્ય
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->ગણતરી, file->uses, file->close);

    જો (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_ઉમેરો_head(&cache->expire_queue, &file->queue);

        જો (file->uses >= min_uses || file->ગણતરી) {
            પાછા;
        }
    }

    ngx_open_file_del_event(file);

    જો (file->ગણતરી) {
        પાછા;
    }

    જો (file->fd != NGX_INVALID_FILE) {

        જો (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    જો (!file->close) {
        પાછા;
    }

    ngx_free(file->name);
    ngx_free(file);
}


સ્થિર શૂન્ય
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    જો (file->event == ખાલી) {
        પાછા;
    }

    (શૂન્ય) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->ગણતરી ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = ખાલી;
    file->use_event = 0;
}


સ્થિર શૂન્ય
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

    જ્યારે (n < 3) {

        જો (ngx_queue_empty(&cache->expire_queue)) {
            પાછા;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        જો (n++ != 0 && now - file->accessed <= cache->inactive) {
            પાછા;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ગાંઠ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        જો (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } નહીંતર {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


સ્થિર શૂન્ય
ngx_open_file_cache_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_rbtree_ગાંઠ_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    માટે ( ;; ) {

        જો (ગાંઠ->ચાવી < temp->ચાવી) {

            p = &temp->ડાબું;

        } નહીંતર જો (ગાંઠ->ચાવી > temp->ચાવી) {

            p = &temp->જમણું;

        } નહીંતર { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ગાંઠ;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->ડાબું : &temp->જમણું;
        }

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}


સ્થિર ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_ગાંઠ_t       *ગાંઠ, *sentinel;
    ngx_cached_open_file_t  *file;

    ગાંઠ = cache->rbtree.મૂળ;
    sentinel = cache->rbtree.sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        જો (hash < ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (hash > ગાંઠ->ચાવી) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ગાંઠ;

        rc = ngx_strcmp(name->data, file->name);

        જો (rc == 0) {
            પાછા file;
        }

        ગાંઠ = (rc < 0) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
    }

    પાછા ખાલી;
}


સ્થિર શૂન્ય
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ગાંઠ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ખાલી;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
