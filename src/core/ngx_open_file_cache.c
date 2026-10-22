
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
#सचे (NGX_HAVE_OPENAT)
स्थिर ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ध्रुव u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#सचे (NGX_HAVE_O_PATH)
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
स्थिर शून्य ngx_open_file_cache_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
स्थिर ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
स्थिर शून्य ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, time_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, परिमाण(ngx_open_file_cache_t));
    सचे (cache == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_निवेशय_मानम्);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    सचे (cln == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->data = cache;

    पटिनिवत्त cache;
}


स्थिर शून्य
ngx_open_file_cache_cleanup(शून्य *data)
{
    ngx_open_file_cache_t  *cache = data;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    अत्थाय ( ;; ) {

        सचे (ngx_queue_empty(&cache->expire_queue)) {
            भङ्ग;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        सचे (!file->err && !file->is_dir) {
            file->close = 1;
            file->गणनम् = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } अञ्ञथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    सचे (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    सचे (cache->rbtree.मूलम् != cache->rbtree.sentinel) {
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

    सचे (cache == रिक्त) {

        सचे (of->test_only) {

            सचे (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                पटिनिवत्त NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->size = ngx_file_size(&fi);
            of->fs_size = ngx_file_fs_size(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            पटिनिवत्त NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, परिमाण(ngx_pool_cleanup_file_t));
        सचे (cln == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        सचे (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = of->fd;
            clnf->name = name->data;
            clnf->log = pool->log;
        }

        पटिनिवत्त rc;
    }

    cln = ngx_pool_cleanup_add(pool, परिमाण(ngx_open_file_cache_cleanup_t));
    सचे (cln == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->data, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    सचे (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        सचे (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            सचे (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                गच्छ failed;
            }

            गच्छ add_event;
        }

        सचे (file->use_event
            || (file->event == रिक्त
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#सचे (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#endif
            ))
        {
            सचे (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->size = file->size;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                सचे (!file->is_dir) {
                    file->गणनम्++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } अञ्ञथा {
                of->err = file->err;
#सचे (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#अञ्ञथा
                of->failed = ngx_open_file_n;
#endif
            }

            गच्छ found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->गणनम्, file->err);

        सचे (file->is_dir) {

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

        सचे (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            गच्छ failed;
        }

        सचे (of->is_dir) {

            सचे (file->is_dir || file->err) {
                गच्छ update;
            }

            /* file became directory */

        } अञ्ञथा सचे (of->err == 0) {  /* file */

            सचे (file->is_dir || file->err) {
                गच्छ add_event;
            }

            सचे (of->uniq == file->uniq) {

                सचे (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                गच्छ update;
            }

            /* file was changed */

        } अञ्ञथा { /* error to cache */

            सचे (file->err || file->is_dir) {
                गच्छ update;
            }

            /* file was removed, etc. */
        }

        सचे (file->गणनम् == 0) {

            ngx_open_file_del_event(file);

            सचे (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            गच्छ add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        file->close = 1;

        गच्छ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    सचे (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        गच्छ failed;
    }

create:

    सचे (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(परिमाण(ngx_cached_open_file_t), pool->log);

    सचे (file == रिक्त) {
        गच्छ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    सचे (file->name == रिक्त) {
        ngx_free(file);
        file = रिक्त;
        गच्छ failed;
    }

    ngx_cpystrn(file->name, name->data, name->len + 1);

    file->ग्रन्थिः.कुञ्चिका = hash;

    ngx_rbtree_निवेशय(&cache->rbtree, &file->ग्रन्थिः);

    cache->current++;

    file->uses = 1;
    file->गणनम् = 0;
    file->use_event = 0;
    file->event = रिक्त;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#सचे (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#endif

    सचे (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->size = of->size;

        file->close = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        सचे (!of->is_dir) {
            file->गणनम्++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_निवेशय_head(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->गणनम्, file->err, file->uses);

    सचे (of->err == 0) {

        सचे (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->data;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त NGX_ERROR;

failed:

    सचे (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        सचे (file->गणनम् == 0) {

            सचे (file->fd != NGX_INVALID_FILE) {
                सचे (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } अञ्ञथा {
            file->close = 1;
        }
    }

    सचे (of->fd != NGX_INVALID_FILE) {
        सचे (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    पटिनिवत्त NGX_ERROR;
}


#सचे (NGX_HAVE_OPENAT)

स्थिर ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ध्रुव u_char *name,
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

    सचे (fd == NGX_INVALID_FILE) {
        पटिनिवत्त NGX_INVALID_FILE;
    }

    सचे (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        गच्छ failed;
    }

#सचे (NGX_HAVE_O_PATH)
    सचे (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        गच्छ failed;
    }
#अञ्ञथा
    सचे (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        गच्छ failed;
    }
#endif

    सचे (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        गच्छ failed;
    }

    पटिनिवत्त fd;

failed:

    सचे (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    पटिनिवत्त NGX_INVALID_FILE;
}


#सचे (NGX_HAVE_O_PATH)

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

    सचे (use_fstat) {
        सचे (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            पटिनिवत्त NGX_OK;
        }

        सचे (ngx_errno != NGX_EBADF) {
            पटिनिवत्त NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    सचे (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त NGX_ERROR;
}

#endif

#endif /* NGX_HAVE_OPENAT */


स्थिर ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#सचे !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->data, mode, create, access);

    सचे (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        पटिनिवत्त NGX_INVALID_FILE;
    }

    पटिनिवत्त fd;

#अञ्ञथा

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    सचे (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->data, mode, create, access);

        सचे (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            पटिनिवत्त NGX_INVALID_FILE;
        }

        पटिनिवत्त fd;
    }

    p = name->data;
    end = p + name->len;

    at_name = *name;

    सचे (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        सचे (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            पटिनिवत्त NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } अञ्ञथा सचे (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        सचे (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            पटिनिवत्त NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } अञ्ञथा {
        at_fd = NGX_AT_FDCWD;
    }

    अत्थाय ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        सचे (cp == रिक्त) {
            भङ्ग;
        }

        सचे (cp == p) {
            p++;
            अनुवर्त;
        }

        *cp = '\0';

        सचे (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } अञ्ञथा {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        सचे (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            गच्छ failed;
        }

        सचे (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.data;
    }

    सचे (p == end) {

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
        गच्छ done;
    }

    सचे (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } अञ्ञथा {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    सचे (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    सचे (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    पटिनिवत्त fd;
#endif
}


स्थिर ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#सचे !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->data, fi);

    सचे (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        पटिनिवत्त NGX_FILE_ERROR;
    }

    पटिनिवत्त rc;

#अञ्ञथा

    ngx_fd_t  fd;

    सचे (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->data, fi);

        सचे (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            पटिनिवत्त NGX_FILE_ERROR;
        }

        पटिनिवत्त rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    सचे (fd == NGX_INVALID_FILE) {
        पटिनिवत्त NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    सचे (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    सचे (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    पटिनिवत्त rc;
#endif
}


स्थिर ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    सचे (of->fd != NGX_INVALID_FILE) {

        सचे (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            पटिनिवत्त NGX_ERROR;
        }

        सचे (of->uniq == ngx_file_uniq(&fi)) {
            गच्छ done;
        }

    } अञ्ञथा सचे (of->test_dir) {

        सचे (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            पटिनिवत्त NGX_ERROR;
        }

        सचे (ngx_is_dir(&fi)) {
            गच्छ done;
        }
    }

    सचे (!of->log) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, log);

    } अञ्ञथा {
        fd = ngx_open_file_wrapper(name, of, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, log);
    }

    सचे (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        सचे (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        पटिनिवत्त NGX_ERROR;
    }

    सचे (ngx_is_dir(&fi)) {
        सचे (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } अञ्ञथा {
        of->fd = fd;

        सचे (of->read_ahead && ngx_file_size(&fi) > NGX_MIN_READ_AHEAD) {
            सचे (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        सचे (of->directio <= ngx_file_size(&fi)) {
            सचे (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } अञ्ञथा {
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

    पटिनिवत्त NGX_OK;
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

    सचे (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        पटिनिवत्त;
    }

    file->use_event = 0;

    file->event = ngx_calloc(परिमाण(ngx_event_t), log);
    सचे (file->event== रिक्त) {
        पटिनिवत्त;
    }

    fev = ngx_alloc(परिमाण(ngx_open_file_cache_event_t), log);
    सचे (fev == रिक्त) {
        ngx_free(file->event);
        file->event = रिक्त;
        पटिनिवत्त;
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

    सचे (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->data);
        ngx_free(file->event);
        file->event = रिक्त;
        पटिनिवत्त;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    पटिनिवत्त;
}


स्थिर शून्य
ngx_open_file_cleanup(शून्य *data)
{
    ngx_open_file_cache_cleanup_t  *c = data;

    c->file->गणनम्--;

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
                   file->name, file->fd, file->गणनम्, file->uses, file->close);

    सचे (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_निवेशय_head(&cache->expire_queue, &file->queue);

        सचे (file->uses >= min_uses || file->गणनम्) {
            पटिनिवत्त;
        }
    }

    ngx_open_file_del_event(file);

    सचे (file->गणनम्) {
        पटिनिवत्त;
    }

    सचे (file->fd != NGX_INVALID_FILE) {

        सचे (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    सचे (!file->close) {
        पटिनिवत्त;
    }

    ngx_free(file->name);
    ngx_free(file);
}


स्थिर शून्य
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    सचे (file->event == रिक्त) {
        पटिनिवत्त;
    }

    (शून्य) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->गणनम् ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->data);
    ngx_free(file->event);
    file->event = रिक्त;
    file->use_event = 0;
}


स्थिर शून्य
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

    याव (n < 3) {

        सचे (ngx_queue_empty(&cache->expire_queue)) {
            पटिनिवत्त;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_data(q, ngx_cached_open_file_t, queue);

        सचे (n++ != 0 && now - file->accessed <= cache->inactive) {
            पटिनिवत्त;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        सचे (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } अञ्ञथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


स्थिर शून्य
ngx_open_file_cache_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t       **p;
    ngx_cached_open_file_t    *file, *file_temp;

    अत्थाय ( ;; ) {

        सचे (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) {

            p = &temp->वामः;

        } अञ्ञथा सचे (ग्रन्थिः->कुञ्चिका > temp->कुञ्चिका) {

            p = &temp->दक्षिणः;

        } अञ्ञथा { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ग्रन्थिः;
            file_temp = (ngx_cached_open_file_t *) temp;

            p = (ngx_strcmp(file->name, file_temp->name) < 0)
                    ? &temp->वामः : &temp->दक्षिणः;
        }

        सचे (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


स्थिर ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_ग्रन्थिः_t       *ग्रन्थिः, *sentinel;
    ngx_cached_open_file_t  *file;

    ग्रन्थिः = cache->rbtree.मूलम्;
    sentinel = cache->rbtree.sentinel;

    याव (ग्रन्थिः != sentinel) {

        सचे (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ग्रन्थिः;

        rc = ngx_strcmp(name->data, file->name);

        सचे (rc == 0) {
            पटिनिवत्त file;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    पटिनिवत्त रिक्त;
}


स्थिर शून्य
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->data;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ग्रन्थिः);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = रिक्त;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->data);
    ngx_free(ev);
}
