
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#define NGX_MIN_READ_AHEAD  (128 * 1024)


स्थिर शून्य ngx_open_file_cache_cleanup(शून्य *data);
#जर (NGX_HAVE_OPENAT)
स्थिर ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, स्थिरांक u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#जर (NGX_HAVE_O_PATH)
स्थिर ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#endif
#endif
स्थिर ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
स्थिर ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
स्थिर ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
स्थिर शून्य ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
स्थिर शून्य ngx_open_file_cleanup(शून्य *data);
स्थिर शून्य ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
स्थिर शून्य ngx_open_file_del_event(ngx_cached_open_file_t *file);
स्थिर शून्य ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
स्थिर शून्य ngx_open_file_cache_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel);
स्थिर ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
स्थिर शून्य ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, समय_प्रकार inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, आकार(ngx_open_file_cache_t));
    जर (cache == रिक्त) {
        परत रिक्त;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_घाला_मूल्य);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    जर (cln == रिक्त) {
        परत रिक्त;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    परत cache;
}


स्थिर शून्य
ngx_open_file_cache_cleanup(शून्य *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    साठी ( ;; ) {

        जर (ngx_queue_empty(&cache->expire_queue)) {
            थांबा;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->गाठ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        जर (!file->err && !file->is_dir) {
            file->बंद = 1;
            file->मोजणी = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    जर (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    जर (cache->rbtree.मूळ != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool)
{
    समय_प्रकार                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    of->fd = NGX_INVALID_FILE;
    of->err = 0;

    जर (cache == रिक्त) {

        जर (of->test_only) {

            जर (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                परत NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            परत NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, आकार(ngx_pool_cleanup_file_t));
        जर (cln == रिक्त) {
            परत NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        जर (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        परत rc;
    }

    cln = ngx_pool_cleanup_add(pool, आकार(ngx_open_file_cache_cleanup_t));
    जर (cln == रिक्त) {
        परत NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    जर (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        जर (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            जर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                जा failed;
            }

            जा add_event;
        }

        जर (file->use_event
            || (file->event == रिक्त
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#जर (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            जर (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                जर (!file->is_dir) {
                    file->मोजणी++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } अन्यथा {
                of->err = file->err;
#जर (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#अन्यथा
                of->failed = ngx_open_file_n;
#endif
            }

            जा found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->मोजणी, file->err);

        जर (file->is_dir) {

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

        जर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            जा failed;
        }

        जर (of->is_dir) {

            जर (file->is_dir || file->err) {
                जा update;
            }

            /* file became directory */

        } अन्यथा जर (of->err == 0) {  /* file */

            जर (file->is_dir || file->err) {
                जा add_event;
            }

            जर (of->uniq == file->uniq) {

                जर (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                जा update;
            }

            /* file was changed */

        } अन्यथा { /* error to cache */

            जर (file->err || file->is_dir) {
                जा update;
            }

            /* file was removed, etc. */
        }

        जर (file->मोजणी == 0) {

            ngx_open_file_del_event(file);

            जर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            जा add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->गाठ);

        cache->current--;

        file->बंद = 1;

        जा create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    जर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        जा failed;
    }

create:

    जर (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(आकार(ngx_cached_open_file_t), pool->log);

    जर (file == रिक्त) {
        जा failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    जर (file->name == रिक्त) {
        ngx_free(file);
        file = रिक्त;
        जा failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->गाठ.किल्ली = hash;

    ngx_rbtree_घाला(&cache->rbtree, &file->गाठ);

    cache->current++;

    file->uses = 1;
    file->मोजणी = 0;
    file->use_event = 0;
    file->event = रिक्त;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#जर (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    जर (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->बंद = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        जर (!of->is_dir) {
            file->मोजणी++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_घाला_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->मोजणी, file->err, file->uses);

    जर (of->err == 0) {

        जर (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        परत NGX_OK;
    }

    परत NGX_ERROR;

failed:

    जर (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->गाठ);

        cache->current--;

        जर (file->मोजणी == 0) {

            जर (file->fd != NGX_INVALID_FILE) {
                जर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } अन्यथा {
            file->बंद = 1;
        }
    }

    जर (of->fd != NGX_INVALID_FILE) {
        जर (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    परत NGX_ERROR;
}


#जर (NGX_HAVE_OPENAT)

स्थिर ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, स्थिरांक u_char *name,
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

    जर (fd == NGX_INVALID_FILE) {
        परत NGX_INVALID_FILE;
    }

    जर (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        जा failed;
    }

#जर (NGX_HAVE_O_PATH)
    जर (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        जा failed;
    }
#अन्यथा
    जर (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        जा failed;
    }
#endif

    जर (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        जा failed;
    }

    परत fd;

failed:

    जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    परत NGX_INVALID_FILE;
}


#जर (NGX_HAVE_O_PATH)

स्थिर ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    स्थिर ngx_uint_t  use_fstat = 1;

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

    जर (use_fstat) {
        जर (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            परत NGX_OK;
        }

        जर (ngx_errno != NGX_EBADF) {
            परत NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    जर (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        परत NGX_OK;
    }

    परत NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


स्थिर ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#जर !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    जर (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        परत NGX_INVALID_FILE;
    }

    परत fd;

#अन्यथा

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    जर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        जर (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            परत NGX_INVALID_FILE;
        }

        परत fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    जर (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_शोध|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        जर (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            परत NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } अन्यथा जर (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_शोध|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        जर (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            परत NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } अन्यथा {
        at_fd = NGX_AT_FDCWD;
    }

    साठी ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        जर (cp == रिक्त) {
            थांबा;
        }

        जर (cp == p) {
            p++;
            सुरूठेवा;
        }

        *cp = '\0';

        जर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_शोध|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } अन्यथा {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_शोध|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        जर (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            जा failed;
        }

        जर (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    जर (p == end) {

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
        जा done;
    }

    जर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } अन्यथा {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    जर (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    जर (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    परत fd;
#endif
}


स्थिर ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#जर !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    जर (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        परत NGX_FILE_ERROR;
    }

    परत rc;

#अन्यथा

    ngx_fd_t  fd;

    जर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        जर (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            परत NGX_FILE_ERROR;
        }

        परत rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    जर (fd == NGX_INVALID_FILE) {
        परत NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    जर (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    परत rc;
#endif
}


स्थिर ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    जर (of->fd != NGX_INVALID_FILE) {

        जर (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            परत NGX_ERROR;
        }

        जर (of->uniq == ngx_file_uniq(&fi)) {
            जा done;
        }

    } अन्यथा जर (of->test_dir) {

        जर (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            परत NGX_ERROR;
        }

        जर (ngx_is_dir(&fi)) {
            जा done;
        }
    }

    जर (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } अन्यथा {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    जर (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        परत NGX_ERROR;
    }

    जर (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        परत NGX_ERROR;
    }

    जर (ngx_is_dir(&fi)) {
        जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } अन्यथा {
        of->fd = fd;

        जर (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            जर (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        जर (of->directio <= ngx_file_size(&fi)) {
            जर (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } अन्यथा {
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

    परत NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

स्थिर शून्य
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    जर (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        परत;
    }

    file->use_event = 0;

    file->event = ngx_calloc(आकार(ngx_event_t), log);
    जर (file->event== रिक्त) {
        परत;
    }

    fev = ngx_alloc(आकार(ngx_open_file_cache_event_t), log);
    जर (fev == रिक्त) {
        ngx_free(file->event);
        file->event = रिक्त;
        परत;
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

    जर (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = रिक्त;
        परत;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    परत;
}


स्थिर शून्य
ngx_open_file_cleanup(शून्य *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->मोजणी--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


स्थिर शून्य
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->मोजणी, file->uses, file->बंद);

    जर (!file->बंद) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_घाला_head(&cache->expire_queue, &file->queue);

        जर (file->uses >= min_uses || file->मोजणी) {
            परत;
        }
    }

    ngx_open_file_del_event(file);

    जर (file->मोजणी) {
        परत;
    }

    जर (file->fd != NGX_INVALID_FILE) {

        जर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    जर (!file->बंद) {
        परत;
    }

    ngx_free(file->name);
    ngx_free(file);
}


स्थिर शून्य
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    जर (file->event == रिक्त) {
        परत;
    }

    (शून्य) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->मोजणी ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = रिक्त;
    file->use_event = 0;
}


स्थिर शून्य
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *log)
{
    समय_प्रकार                   now;
    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_time();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    पर्यंत (n < 3) {

        जर (ngx_queue_empty(&cache->expire_queue)) {
            परत;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        जर (n++ != 0 && now - file->accessed <= cache->inactive) {
            परत;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->गाठ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        जर (!file->err && !file->is_dir) {
            file->बंद = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


स्थिर शून्य
ngx_open_file_cache_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *temp,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel)
{
    ngx_rbtree_गाठ_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    साठी ( ;; ) {

        जर (गाठ->किल्ली < temp->किल्ली) {

            p = &temp->डावा;

        } अन्यथा जर (गाठ->किल्ली > temp->किल्ली) {

            p = &temp->उजवा;

        } अन्यथा { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) गाठ;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->डावा : &temp->उजवा;
        }

        जर (*p == sentinel) {
            थांबा;
        }

        temp = *p;
    }

    *p = गाठ;
    गाठ->parent = temp;
    गाठ->डावा = sentinel;
    गाठ->उजवा = sentinel;
    ngx_rbt_red(गाठ);
}


स्थिर ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_गाठ_t       *गाठ, *sentinel;
    ngx_cached_open_file_t  *file;

    गाठ = cache->rbtree.मूळ;
    sentinel = cache->rbtree.sentinel;

    पर्यंत (गाठ != sentinel) {

        जर (hash < गाठ->किल्ली) {
            गाठ = गाठ->डावा;
            सुरूठेवा;
        }

        जर (hash > गाठ->किल्ली) {
            गाठ = गाठ->उजवा;
            सुरूठेवा;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) गाठ;

        rc = ngx_strcmp(name->data, file->name);

        जर (rc == 0) {
            परत file;
        }

        गाठ = (rc < 0) ? गाठ->डावा : गाठ->उजवा;
    }

    परत रिक्त;
}


स्थिर शून्य
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->गाठ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = रिक्त;
    file->use_event = 0;

    file->बंद = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
