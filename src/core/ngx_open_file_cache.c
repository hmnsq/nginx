
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#घोषणा NGX_MIN_READ_AHEAD  (128 * 1024)


अटल व्योम ngx_open_file_cache_cleanup(व्योम *तथ्य);
#अगर (NGX_HAVE_OPENAT)
अटल ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, स्थिर u_char *name,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log);
#अगर (NGX_HAVE_O_PATH)
अटल ngx_int_t ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *log);
#पूर्ण_अगर
#पूर्ण_अगर
अटल ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_int_t mode, ngx_int_t create,
    ngx_int_t access, ngx_log_t *log);
अटल ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_file_info_t *fi, ngx_log_t *log);
अटल ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_log_t *log);
अटल व्योम ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log);
अटल व्योम ngx_open_file_cleanup(व्योम *तथ्य);
अटल व्योम ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log);
अटल व्योम ngx_open_file_del_event(ngx_cached_open_file_t *file);
अटल व्योम ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *log);
अटल व्योम ngx_open_file_cache_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
अटल ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
अटल व्योम ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, समय_प्रकार inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, माप(ngx_open_file_cache_t));
    अगर (cache == शून्य) {
        वापस शून्य;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_जोड़ो_मूल्य);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_add(pool, 0);
    अगर (cln == शून्य) {
        वापस शून्य;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->तथ्य = cache;

    वापस cache;
}


अटल व्योम
ngx_open_file_cache_cleanup(व्योम *तथ्य)
{
    ngx_open_file_cache_t  *cache = तथ्य;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                   "open file cache cleanup");

    क्रम ( ;; ) {

        अगर (ngx_queue_empty(&cache->expire_queue)) {
            अवरोध;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_तथ्य(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->गाँठ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_cycle->log, 0,
                       "delete cached open file: %s", file->name);

        अगर (!file->err && !file->is_dir) {
            file->बंद = 1;
            file->गिनती = 0;
            ngx_close_cached_file(cache, file, 0, ngx_cycle->log);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    अगर (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    अगर (cache->rbtree.मूल != cache->rbtree.sentinel) {
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

    अगर (cache == शून्य) {

        अगर (of->test_only) {

            अगर (ngx_file_info_wrapper(name, of, &fi, pool->log)
                == NGX_FILE_ERROR)
            {
                वापस NGX_ERROR;
            }

            of->uniq = ngx_file_uniq(&fi);
            of->mtime = ngx_file_mtime(&fi);
            of->आकार = ngx_file_आकार(&fi);
            of->fs_आकार = ngx_file_fs_आकार(&fi);
            of->is_dir = ngx_is_dir(&fi);
            of->is_file = ngx_is_file(&fi);
            of->is_link = ngx_is_link(&fi);
            of->is_exec = ngx_is_exec(&fi);

            वापस NGX_OK;
        }

        cln = ngx_pool_cleanup_add(pool, माप(ngx_pool_cleanup_file_t));
        अगर (cln == शून्य) {
            वापस NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, of, pool->log);

        अगर (rc == NGX_OK && !of->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->तथ्य;

            clnf->fd = of->fd;
            clnf->name = name->तथ्य;
            clnf->log = pool->log;
        }

        वापस rc;
    }

    cln = ngx_pool_cleanup_add(pool, माप(ngx_open_file_cache_cleanup_t));
    अगर (cln == शून्य) {
        वापस NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_crc32_long(name->तथ्य, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    अगर (file) {

        file->uses++;

        ngx_queue_remove(&file->queue);

        अगर (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, of, pool->log);

            अगर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
                जाओ failed;
            }

            जाओ add_event;
        }

        अगर (file->use_event
            || (file->event == शून्य
                && (of->uniq == 0 || of->uniq == file->uniq)
                && now - file->created < of->valid
#अगर (NGX_HAVE_OPENAT)
                && of->disable_symlinks == file->disable_symlinks
                && of->disable_symlinks_from == file->disable_symlinks_from
#पूर्ण_अगर
            ))
        {
            अगर (file->err == 0) {

                of->fd = file->fd;
                of->uniq = file->uniq;
                of->mtime = file->mtime;
                of->आकार = file->आकार;

                of->is_dir = file->is_dir;
                of->is_file = file->is_file;
                of->is_link = file->is_link;
                of->is_exec = file->is_exec;
                of->is_directio = file->is_directio;

                अगर (!file->is_dir) {
                    file->गिनती++;
                    ngx_open_file_add_event(cache, file, of, pool->log);
                }

            } अन्यथा {
                of->err = file->err;
#अगर (NGX_HAVE_OPENAT)
                of->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#अन्यथा
                of->failed = ngx_open_file_n;
#पूर्ण_अगर
            }

            जाओ found;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->log, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->गिनती, file->err);

        अगर (file->is_dir) {

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

        अगर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
            जाओ failed;
        }

        अगर (of->is_dir) {

            अगर (file->is_dir || file->err) {
                जाओ update;
            }

            /* file became directory */

        } अन्यथा अगर (of->err == 0) {  /* file */

            अगर (file->is_dir || file->err) {
                जाओ add_event;
            }

            अगर (of->uniq == file->uniq) {

                अगर (file->event) {
                    file->use_event = 1;
                }

                of->is_directio = file->is_directio;

                जाओ update;
            }

            /* file was changed */

        } अन्यथा { /* error to cache */

            अगर (file->err || file->is_dir) {
                जाओ update;
            }

            /* file was removed, etc. */
        }

        अगर (file->गिनती == 0) {

            ngx_open_file_del_event(file);

            अगर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            जाओ add_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->गाँठ);

        cache->current--;

        file->बंद = 1;

        जाओ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, of, pool->log);

    अगर (rc != NGX_OK && (of->err == 0 || !of->errors)) {
        जाओ failed;
    }

create:

    अगर (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->log);
    }

    file = ngx_alloc(माप(ngx_cached_open_file_t), pool->log);

    अगर (file == शून्य) {
        जाओ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->log);

    अगर (file->name == शून्य) {
        ngx_free(file);
        file = शून्य;
        जाओ failed;
    }

    ngx_cpystrn(file->name, name->तथ्य, name->len + 1);

    file->गाँठ.कुंजी = hash;

    ngx_rbtree_जोड़ो(&cache->rbtree, &file->गाँठ);

    cache->current++;

    file->uses = 1;
    file->गिनती = 0;
    file->use_event = 0;
    file->event = शून्य;

add_event:

    ngx_open_file_add_event(cache, file, of, pool->log);

update:

    file->fd = of->fd;
    file->err = of->err;
#अगर (NGX_HAVE_OPENAT)
    file->disable_symlinks = of->disable_symlinks;
    file->disable_symlinks_from = of->disable_symlinks_from;
#पूर्ण_अगर

    अगर (of->err == 0) {
        file->uniq = of->uniq;
        file->mtime = of->mtime;
        file->आकार = of->आकार;

        file->बंद = 0;

        file->is_dir = of->is_dir;
        file->is_file = of->is_file;
        file->is_link = of->is_link;
        file->is_exec = of->is_exec;
        file->is_directio = of->is_directio;

        अगर (!of->is_dir) {
            file->गिनती++;
        }
    }

    file->created = now;

found:

    file->accessed = now;

    ngx_queue_जोड़ो_शीर्ष(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->log, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->गिनती, file->err, file->uses);

    अगर (of->err == 0) {

        अगर (!of->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->तथ्य;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_uses = of->min_uses;
            ofcln->log = pool->log;
        }

        वापस NGX_OK;
    }

    वापस NGX_ERROR;

failed:

    अगर (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->गाँठ);

        cache->current--;

        अगर (file->गिनती == 0) {

            अगर (file->fd != NGX_INVALID_FILE) {
                अगर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
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

    अगर (of->fd != NGX_INVALID_FILE) {
        अगर (ngx_close_file(of->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    वापस NGX_ERROR;
}


#अगर (NGX_HAVE_OPENAT)

अटल ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, स्थिर u_char *name,
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

    अगर (fd == NGX_INVALID_FILE) {
        वापस NGX_INVALID_FILE;
    }

    अगर (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        जाओ failed;
    }

#अगर (NGX_HAVE_O_PATH)
    अगर (ngx_file_o_path_info(fd, &fi, log) == NGX_ERROR) {
        err = ngx_errno;
        जाओ failed;
    }
#अन्यथा
    अगर (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        जाओ failed;
    }
#पूर्ण_अगर

    अगर (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        जाओ failed;
    }

    वापस fd;

failed:

    अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_set_errno(err);

    वापस NGX_INVALID_FILE;
}


#अगर (NGX_HAVE_O_PATH)

अटल ngx_int_t
ngx_file_o_path_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *log)
{
    अटल ngx_uint_t  use_fstat = 1;

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

    अगर (use_fstat) {
        अगर (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            वापस NGX_OK;
        }

        अगर (ngx_errno != NGX_EBADF) {
            वापस NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, log, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    अगर (ngx_file_at_info(fd, "", fi, AT_EMPTY_PATH) != NGX_FILE_ERROR) {
        वापस NGX_OK;
    }

    वापस NGX_ERROR;
}

#पूर्ण_अगर

#पूर्ण_अगर /* NGX_HAVE_OPENAT */


अटल ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_int_t mode, ngx_int_t create, ngx_int_t access, ngx_log_t *log)
{
    ngx_fd_t  fd;

#अगर !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->तथ्य, mode, create, access);

    अगर (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_open_file_n;
        वापस NGX_INVALID_FILE;
    }

    वापस fd;

#अन्यथा

    u_char           *p, *cp, *end;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    अगर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->तथ्य, mode, create, access);

        अगर (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            वापस NGX_INVALID_FILE;
        }

        वापस fd;
    }

    p = name->तथ्य;
    end = p + name->len;

    at_name = *name;

    अगर (of->disable_symlinks_from) {

        cp = p + of->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_खोज|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        अगर (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_open_file_n;
            वापस NGX_INVALID_FILE;
        }

        at_name.len = of->disable_symlinks_from;
        p = cp + 1;

    } अन्यथा अगर (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_खोज|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        अगर (at_fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            वापस NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } अन्यथा {
        at_fd = NGX_AT_FDCWD;
    }

    क्रम ( ;; ) {
        cp = ngx_strlchr(p, end, '/');
        अगर (cp == शून्य) {
            अवरोध;
        }

        अगर (cp == p) {
            p++;
            जारी;
        }

        *cp = '\0';

        अगर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_खोज|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, log);

        } अन्यथा {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_खोज|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        अगर (fd == NGX_INVALID_FILE) {
            of->err = ngx_errno;
            of->failed = ngx_openat_file_n;
            जाओ failed;
        }

        अगर (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.तथ्य;
    }

    अगर (p == end) {

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
        जाओ done;
    }

    अगर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, mode, create, access, log);

    } अन्यथा {
        fd = ngx_openat_file(at_fd, p, mode|NGX_FILE_NOFOLLOW, create, access);
    }

done:

    अगर (fd == NGX_INVALID_FILE) {
        of->err = ngx_errno;
        of->failed = ngx_openat_file_n;
    }

failed:

    अगर (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    वापस fd;
#पूर्ण_अगर
}


अटल ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_file_info_t *fi, ngx_log_t *log)
{
    ngx_int_t  rc;

#अगर !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->तथ्य, fi);

    अगर (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_file_info_n;
        वापस NGX_FILE_ERROR;
    }

    वापस rc;

#अन्यथा

    ngx_fd_t  fd;

    अगर (of->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->तथ्य, fi);

        अगर (rc == NGX_FILE_ERROR) {
            of->err = ngx_errno;
            of->failed = ngx_file_info_n;
            वापस NGX_FILE_ERROR;
        }

        वापस rc;
    }

    fd = ngx_open_file_wrapper(name, of, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, log);

    अगर (fd == NGX_INVALID_FILE) {
        वापस NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    अगर (rc == NGX_FILE_ERROR) {
        of->err = ngx_errno;
        of->failed = ngx_fd_info_n;
    }

    अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    वापस rc;
#पूर्ण_अगर
}


अटल ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *of,
    ngx_log_t *log)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    अगर (of->fd != NGX_INVALID_FILE) {

        अगर (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            वापस NGX_ERROR;
        }

        अगर (of->uniq == ngx_file_uniq(&fi)) {
            जाओ done;
        }

    } अन्यथा अगर (of->test_dir) {

        अगर (ngx_file_info_wrapper(name, of, &fi, log) == NGX_FILE_ERROR) {
            of->fd = NGX_INVALID_FILE;
            वापस NGX_ERROR;
        }

        अगर (ngx_is_dir(&fi)) {
            जाओ done;
        }
    }

    अगर (!of->log) {

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

    अगर (fd == NGX_INVALID_FILE) {
        of->fd = NGX_INVALID_FILE;
        वापस NGX_ERROR;
    }

    अगर (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, log, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

        वापस NGX_ERROR;
    }

    अगर (ngx_is_dir(&fi)) {
        अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        of->fd = NGX_INVALID_FILE;

    } अन्यथा {
        of->fd = fd;

        अगर (of->read_ahead && ngx_file_आकार(&fi) > NGX_MIN_READ_AHEAD) {
            अगर (ngx_read_ahead(fd, of->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        अगर (of->directio <= ngx_file_आकार(&fi)) {
            अगर (ngx_directio_on(fd) == NGX_FILE_ERROR) {
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
    of->आकार = ngx_file_आकार(&fi);
    of->fs_आकार = ngx_file_fs_आकार(&fi);
    of->is_dir = ngx_is_dir(&fi);
    of->is_file = ngx_is_file(&fi);
    of->is_link = ngx_is_link(&fi);
    of->is_exec = ngx_is_exec(&fi);

    वापस NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

अटल व्योम
ngx_open_file_add_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *of, ngx_log_t *log)
{
    ngx_open_file_cache_event_t  *fev;

    अगर (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !of->events
        || file->event
        || of->fd == NGX_INVALID_FILE
        || file->uses < of->min_uses)
    {
        वापस;
    }

    file->use_event = 0;

    file->event = ngx_calloc(माप(ngx_event_t), log);
    अगर (file->event== शून्य) {
        वापस;
    }

    fev = ngx_alloc(माप(ngx_open_file_cache_event_t), log);
    अगर (fev == शून्य) {
        ngx_free(file->event);
        file->event = शून्य;
        वापस;
    }

    fev->fd = of->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->तथ्य = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->log = ngx_cycle->log;

    अगर (ngx_add_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->तथ्य);
        ngx_free(file->event);
        file->event = शून्य;
        वापस;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    वापस;
}


अटल व्योम
ngx_open_file_cleanup(व्योम *तथ्य)
{
    ngx_open_file_cache_cleanup_t  *c = तथ्य;

    c->file->गिनती--;

    ngx_close_cached_file(c->cache, c->file, c->min_uses, c->log);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->log);
}


अटल व्योम
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_uses, ngx_log_t *log)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, log, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->गिनती, file->uses, file->बंद);

    अगर (!file->बंद) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_जोड़ो_शीर्ष(&cache->expire_queue, &file->queue);

        अगर (file->uses >= min_uses || file->गिनती) {
            वापस;
        }
    }

    ngx_open_file_del_event(file);

    अगर (file->गिनती) {
        वापस;
    }

    अगर (file->fd != NGX_INVALID_FILE) {

        अगर (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    अगर (!file->बंद) {
        वापस;
    }

    ngx_free(file->name);
    ngx_free(file);
}


अटल व्योम
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    अगर (file->event == शून्य) {
        वापस;
    }

    (व्योम) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->गिनती ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->तथ्य);
    ngx_free(file->event);
    file->event = शून्य;
    file->use_event = 0;
}


अटल व्योम
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

    जबतक (n < 3) {

        अगर (ngx_queue_empty(&cache->expire_queue)) {
            वापस;
        }

        q = ngx_queue_last(&cache->expire_queue);

        file = ngx_queue_तथ्य(q, ngx_cached_open_file_t, queue);

        अगर (n++ != 0 && now - file->accessed <= cache->inactive) {
            वापस;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->गाँठ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "expire cached open file: %s", file->name);

        अगर (!file->err && !file->is_dir) {
            file->बंद = 1;
            ngx_close_cached_file(cache, file, 0, log);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


अटल व्योम
ngx_open_file_cache_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_rbtree_गाँठ_t       **p;
    ngx_cached_open_file_t    *file, *file_अस्थायी;

    क्रम ( ;; ) {

        अगर (गाँठ->कुंजी < अस्थायी->कुंजी) {

            p = &अस्थायी->बायाँ;

        } अन्यथा अगर (गाँठ->कुंजी > अस्थायी->कुंजी) {

            p = &अस्थायी->दायाँ;

        } अन्यथा { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) गाँठ;
            file_अस्थायी = (ngx_cached_open_file_t *) अस्थायी;

            p = (ngx_strcmp(file->name, file_अस्थायी->name) < 0)
                    ? &अस्थायी->बायाँ : &अस्थायी->दायाँ;
        }

        अगर (*p == sentinel) {
            अवरोध;
        }

        अस्थायी = *p;
    }

    *p = गाँठ;
    गाँठ->parent = अस्थायी;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}


अटल ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_गाँठ_t       *गाँठ, *sentinel;
    ngx_cached_open_file_t  *file;

    गाँठ = cache->rbtree.मूल;
    sentinel = cache->rbtree.sentinel;

    जबतक (गाँठ != sentinel) {

        अगर (hash < गाँठ->कुंजी) {
            गाँठ = गाँठ->बायाँ;
            जारी;
        }

        अगर (hash > गाँठ->कुंजी) {
            गाँठ = गाँठ->दायाँ;
            जारी;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) गाँठ;

        rc = ngx_strcmp(name->तथ्य, file->name);

        अगर (rc == 0) {
            वापस file;
        }

        गाँठ = (rc < 0) ? गाँठ->बायाँ : गाँठ->दायाँ;
    }

    वापस शून्य;
}


अटल व्योम
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->तथ्य;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->गाँठ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = शून्य;
    file->use_event = 0;

    file->बंद = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->log);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->तथ्य);
    ngx_free(ev);
}
