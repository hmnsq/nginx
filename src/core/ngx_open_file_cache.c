
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#నిర్వచించు NGX_MIN_READ_AHEAD  (128 * 1024)


స్థిర శూన్య ngx_open_file_cache_cleanup(శూన్య *దత్తాంశం);
#ఒకవేళ (NGX_HAVE_OPENAT)
స్థిర ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, స్థిరరాశి u_char *name,
    ngx_int_t బహుళకం, ngx_int_t create, ngx_int_t access, ngx_log_t *సంవర్గమానం);
#ఒకవేళ (NGX_HAVE_O_మార్గం)
స్థిర ngx_int_t ngx_file_o_మార్గం_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *సంవర్గమానం);
#అయితే_ముగింపు
#అయితే_ముగింపు
స్థిర ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *యొక్క, ngx_int_t బహుళకం, ngx_int_t create,
    ngx_int_t access, ngx_log_t *సంవర్గమానం);
స్థిర ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *యొక్క, ngx_file_info_t *fi, ngx_log_t *సంవర్గమానం);
స్థిర ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *యొక్క, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_open_file_కూడు_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *యొక్క, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_open_file_cleanup(శూన్య *దత్తాంశం);
స్థిర శూన్య ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ఉపయోగిస్తుంది, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_open_file_del_event(ngx_cached_open_file_t *file);
స్థిర శూన్య ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_open_file_cache_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);
స్థిర ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
స్థిర శూన్య ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, సమయం_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, పరిమాణం(ngx_open_file_cache_t));
    ఒకవేళ (cache == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_చొప్పించు_విలువ);

    ngx_వరుస_init(&cache->expire_వరుస);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_కూడు(pool, 0);
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->దత్తాంశం = cache;

    తిరిగి cache;
}


స్థిర శూన్య
ngx_open_file_cache_cleanup(శూన్య *దత్తాంశం)
{
    ngx_open_file_cache_t  *cache = దత్తాంశం;

    ngx_వరుస_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                   "open file cache cleanup");

    కొరకు ( ;; ) {

        ఒకవేళ (ngx_వరుస_ఖాళీలు(&cache->expire_వరుస)) {
            ఆపు;
        }

        q = ngx_వరుస_చివరి(&cache->expire_వరుస);

        file = ngx_వరుస_దత్తాంశం(q, ngx_cached_open_file_t, వరుస);

        ngx_వరుస_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->కణుపు);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_చక్రం->సంవర్గమానం, 0,
                       "delete cached open file: %s", file->name);

        ఒకవేళ (!file->err && !file->is_dir) {
            file->close = 1;
            file->లెక్క = 0;
            ngx_close_cached_file(cache, file, 0, ngx_చక్రం->సంవర్గమానం);

        } లేకపోతే {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    ఒకవేళ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    ఒకవేళ (cache->rbtree.మూలం != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *యొక్క, ngx_pool_t *pool)
{
    సమయం_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    యొక్క->fd = NGX_INVALID_FILE;
    యొక్క->err = 0;

    ఒకవేళ (cache == ఖాళీ) {

        ఒకవేళ (యొక్క->test_only) {

            ఒకవేళ (ngx_file_info_wrapper(name, యొక్క, &fi, pool->సంవర్గమానం)
                == NGX_FILE_ERROR)
            {
                తిరిగి NGX_ERROR;
            }

            యొక్క->uniq = ngx_file_uniq(&fi);
            యొక్క->mtime = ngx_file_mtime(&fi);
            యొక్క->కొలత = ngx_file_కొలత(&fi);
            యొక్క->fs_కొలత = ngx_file_fs_కొలత(&fi);
            యొక్క->is_dir = ngx_is_dir(&fi);
            యొక్క->is_file = ngx_is_file(&fi);
            యొక్క->is_link = ngx_is_link(&fi);
            యొక్క->is_exec = ngx_is_exec(&fi);

            తిరిగి NGX_OK;
        }

        cln = ngx_pool_cleanup_కూడు(pool, పరిమాణం(ngx_pool_cleanup_file_t));
        ఒకవేళ (cln == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, యొక్క, pool->సంవర్గమానం);

        ఒకవేళ (rc == NGX_OK && !యొక్క->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->దత్తాంశం;

            clnf->fd = యొక్క->fd;
            clnf->name = name->దత్తాంశం;
            clnf->సంవర్గమానం = pool->సంవర్గమానం;
        }

        తిరిగి rc;
    }

    cln = ngx_pool_cleanup_కూడు(pool, పరిమాణం(ngx_open_file_cache_cleanup_t));
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    now = ngx_సమయం();

    hash = ngx_చక్రీయ_తనిఖీ_32_long(name->దత్తాంశం, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    ఒకవేళ (file) {

        file->ఉపయోగిస్తుంది++;

        ngx_వరుస_remove(&file->వరుస);

        ఒకవేళ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, యొక్క, pool->సంవర్గమానం);

            ఒకవేళ (rc != NGX_OK && (యొక్క->err == 0 || !యొక్క->errors)) {
                వెళ్ళు failed;
            }

            వెళ్ళు కూడు_event;
        }

        ఒకవేళ (file->use_event
            || (file->event == ఖాళీ
                && (యొక్క->uniq == 0 || యొక్క->uniq == file->uniq)
                && now - file->created < యొక్క->valid
#ఒకవేళ (NGX_HAVE_OPENAT)
                && యొక్క->disable_symlinks == file->disable_symlinks
                && యొక్క->disable_symlinks_from == file->disable_symlinks_from
#అయితే_ముగింపు
            ))
        {
            ఒకవేళ (file->err == 0) {

                యొక్క->fd = file->fd;
                యొక్క->uniq = file->uniq;
                యొక్క->mtime = file->mtime;
                యొక్క->కొలత = file->కొలత;

                యొక్క->is_dir = file->is_dir;
                యొక్క->is_file = file->is_file;
                యొక్క->is_link = file->is_link;
                యొక్క->is_exec = file->is_exec;
                యొక్క->is_directio = file->is_directio;

                ఒకవేళ (!file->is_dir) {
                    file->లెక్క++;
                    ngx_open_file_కూడు_event(cache, file, యొక్క, pool->సంవర్గమానం);
                }

            } లేకపోతే {
                యొక్క->err = file->err;
#ఒకవేళ (NGX_HAVE_OPENAT)
                యొక్క->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#లేకపోతే
                యొక్క->failed = ngx_open_file_n;
#అయితే_ముగింపు
            }

            వెళ్ళు దొరికింది;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->సంవర్గమానం, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->లెక్క, file->err);

        ఒకవేళ (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            యొక్క->test_dir = 1;
        }

        యొక్క->fd = file->fd;
        యొక్క->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, యొక్క, pool->సంవర్గమానం);

        ఒకవేళ (rc != NGX_OK && (యొక్క->err == 0 || !యొక్క->errors)) {
            వెళ్ళు failed;
        }

        ఒకవేళ (యొక్క->is_dir) {

            ఒకవేళ (file->is_dir || file->err) {
                వెళ్ళు update;
            }

            /* file became directory */

        } లేకపోతే ఒకవేళ (యొక్క->err == 0) {  /* file */

            ఒకవేళ (file->is_dir || file->err) {
                వెళ్ళు కూడు_event;
            }

            ఒకవేళ (యొక్క->uniq == file->uniq) {

                ఒకవేళ (file->event) {
                    file->use_event = 1;
                }

                యొక్క->is_directio = file->is_directio;

                వెళ్ళు update;
            }

            /* file was changed */

        } లేకపోతే { /* error to cache */

            ఒకవేళ (file->err || file->is_dir) {
                వెళ్ళు update;
            }

            /* file was removed, etc. */
        }

        ఒకవేళ (file->లెక్క == 0) {

            ngx_open_file_del_event(file);

            ఒకవేళ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->సంవర్గమానం, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            వెళ్ళు కూడు_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->కణుపు);

        cache->current--;

        file->close = 1;

        వెళ్ళు create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, యొక్క, pool->సంవర్గమానం);

    ఒకవేళ (rc != NGX_OK && (యొక్క->err == 0 || !యొక్క->errors)) {
        వెళ్ళు failed;
    }

create:

    ఒకవేళ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->సంవర్గమానం);
    }

    file = ngx_alloc(పరిమాణం(ngx_cached_open_file_t), pool->సంవర్గమానం);

    ఒకవేళ (file == ఖాళీ) {
        వెళ్ళు failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->సంవర్గమానం);

    ఒకవేళ (file->name == ఖాళీ) {
        ngx_free(file);
        file = ఖాళీ;
        వెళ్ళు failed;
    }

    ngx_cpystrn(file->name, name->దత్తాంశం, name->len + 1);

    file->కణుపు.కీలకం = hash;

    ngx_rbtree_చొప్పించు(&cache->rbtree, &file->కణుపు);

    cache->current++;

    file->ఉపయోగిస్తుంది = 1;
    file->లెక్క = 0;
    file->use_event = 0;
    file->event = ఖాళీ;

కూడు_event:

    ngx_open_file_కూడు_event(cache, file, యొక్క, pool->సంవర్గమానం);

update:

    file->fd = యొక్క->fd;
    file->err = యొక్క->err;
#ఒకవేళ (NGX_HAVE_OPENAT)
    file->disable_symlinks = యొక్క->disable_symlinks;
    file->disable_symlinks_from = యొక్క->disable_symlinks_from;
#అయితే_ముగింపు

    ఒకవేళ (యొక్క->err == 0) {
        file->uniq = యొక్క->uniq;
        file->mtime = యొక్క->mtime;
        file->కొలత = యొక్క->కొలత;

        file->close = 0;

        file->is_dir = యొక్క->is_dir;
        file->is_file = యొక్క->is_file;
        file->is_link = యొక్క->is_link;
        file->is_exec = యొక్క->is_exec;
        file->is_directio = యొక్క->is_directio;

        ఒకవేళ (!యొక్క->is_dir) {
            file->లెక్క++;
        }
    }

    file->created = now;

దొరికింది:

    file->accessed = now;

    ngx_వరుస_చొప్పించు_తల(&cache->expire_వరుస, &file->వరుస);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->సంవర్గమానం, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->లెక్క, file->err, file->ఉపయోగిస్తుంది);

    ఒకవేళ (యొక్క->err == 0) {

        ఒకవేళ (!యొక్క->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->దత్తాంశం;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_ఉపయోగిస్తుంది = యొక్క->min_ఉపయోగిస్తుంది;
            ofcln->సంవర్గమానం = pool->సంవర్గమానం;
        }

        తిరిగి NGX_OK;
    }

    తిరిగి NGX_ERROR;

failed:

    ఒకవేళ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->కణుపు);

        cache->current--;

        ఒకవేళ (file->లెక్క == 0) {

            ఒకవేళ (file->fd != NGX_INVALID_FILE) {
                ఒకవేళ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->సంవర్గమానం, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } లేకపోతే {
            file->close = 1;
        }
    }

    ఒకవేళ (యొక్క->fd != NGX_INVALID_FILE) {
        ఒకవేళ (ngx_close_file(యొక్క->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    తిరిగి NGX_ERROR;
}


#ఒకవేళ (NGX_HAVE_OPENAT)

స్థిర ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, స్థిరరాశి u_char *name,
    ngx_int_t బహుళకం, ngx_int_t create, ngx_int_t access, ngx_log_t *సంవర్గమానం)
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

    fd = ngx_openat_file(at_fd, name, బహుళకం, create, access);

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        తిరిగి NGX_INVALID_FILE;
    }

    ఒకవేళ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        వెళ్ళు failed;
    }

#ఒకవేళ (NGX_HAVE_O_మార్గం)
    ఒకవేళ (ngx_file_o_మార్గం_info(fd, &fi, సంవర్గమానం) == NGX_ERROR) {
        err = ngx_errno;
        వెళ్ళు failed;
    }
#లేకపోతే
    ఒకవేళ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        వెళ్ళు failed;
    }
#అయితే_ముగింపు

    ఒకవేళ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        వెళ్ళు failed;
    }

    తిరిగి fd;

failed:

    ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_సమితి_errno(err);

    తిరిగి NGX_INVALID_FILE;
}


#ఒకవేళ (NGX_HAVE_O_మార్గం)

స్థిర ngx_int_t
ngx_file_o_మార్గం_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *సంవర్గమానం)
{
    స్థిర ngx_uint_t  use_fstat = 1;

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

    ఒకవేళ (use_fstat) {
        ఒకవేళ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            తిరిగి NGX_OK;
        }

        ఒకవేళ (ngx_errno != NGX_EBADF) {
            తిరిగి NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, సంవర్గమానం, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    ఒకవేళ (ngx_file_at_info(fd, "", fi, AT_ఖాళీలు_మార్గం) != NGX_FILE_ERROR) {
        తిరిగి NGX_OK;
    }

    తిరిగి NGX_ERROR;
}

#అయితే_ముగింపు

#అయితే_ముగింపు /* NGX_HAVE_OPENAT */


స్థిర ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *యొక్క,
    ngx_int_t బహుళకం, ngx_int_t create, ngx_int_t access, ngx_log_t *సంవర్గమానం)
{
    ngx_fd_t  fd;

#ఒకవేళ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->దత్తాంశం, బహుళకం, create, access);

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        యొక్క->err = ngx_errno;
        యొక్క->failed = ngx_open_file_n;
        తిరిగి NGX_INVALID_FILE;
    }

    తిరిగి fd;

#లేకపోతే

    u_char           *p, *cp, *చివర;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    ఒకవేళ (యొక్క->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->దత్తాంశం, బహుళకం, create, access);

        ఒకవేళ (fd == NGX_INVALID_FILE) {
            యొక్క->err = ngx_errno;
            యొక్క->failed = ngx_open_file_n;
            తిరిగి NGX_INVALID_FILE;
        }

        తిరిగి fd;
    }

    p = name->దత్తాంశం;
    చివర = p + name->len;

    at_name = *name;

    ఒకవేళ (యొక్క->disable_symlinks_from) {

        cp = p + యొక్క->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_శోధన|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        ఒకవేళ (at_fd == NGX_INVALID_FILE) {
            యొక్క->err = ngx_errno;
            యొక్క->failed = ngx_open_file_n;
            తిరిగి NGX_INVALID_FILE;
        }

        at_name.len = యొక్క->disable_symlinks_from;
        p = cp + 1;

    } లేకపోతే ఒకవేళ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_శోధన|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        ఒకవేళ (at_fd == NGX_INVALID_FILE) {
            యొక్క->err = ngx_errno;
            యొక్క->failed = ngx_openat_file_n;
            తిరిగి NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } లేకపోతే {
        at_fd = NGX_AT_FDCWD;
    }

    కొరకు ( ;; ) {
        cp = ngx_strlchr(p, చివర, '/');
        ఒకవేళ (cp == ఖాళీ) {
            ఆపు;
        }

        ఒకవేళ (cp == p) {
            p++;
            కొనసాగు;
        }

        *cp = '\0';

        ఒకవేళ (యొక్క->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_శోధన|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, సంవర్గమానం);

        } లేకపోతే {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_శోధన|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        ఒకవేళ (fd == NGX_INVALID_FILE) {
            యొక్క->err = ngx_errno;
            యొక్క->failed = ngx_openat_file_n;
            వెళ్ళు failed;
        }

        ఒకవేళ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.దత్తాంశం;
    }

    ఒకవేళ (p == చివర) {

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

        fd = ngx_openat_file(at_fd, ".", బహుళకం, create, access);
        వెళ్ళు పూర్తి;
    }

    ఒకవేళ (యొక్క->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, బహుళకం, create, access, సంవర్గమానం);

    } లేకపోతే {
        fd = ngx_openat_file(at_fd, p, బహుళకం|NGX_FILE_NOFOLLOW, create, access);
    }

పూర్తి:

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        యొక్క->err = ngx_errno;
        యొక్క->failed = ngx_openat_file_n;
    }

failed:

    ఒకవేళ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    తిరిగి fd;
#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *యొక్క,
    ngx_file_info_t *fi, ngx_log_t *సంవర్గమానం)
{
    ngx_int_t  rc;

#ఒకవేళ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->దత్తాంశం, fi);

    ఒకవేళ (rc == NGX_FILE_ERROR) {
        యొక్క->err = ngx_errno;
        యొక్క->failed = ngx_file_info_n;
        తిరిగి NGX_FILE_ERROR;
    }

    తిరిగి rc;

#లేకపోతే

    ngx_fd_t  fd;

    ఒకవేళ (యొక్క->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->దత్తాంశం, fi);

        ఒకవేళ (rc == NGX_FILE_ERROR) {
            యొక్క->err = ngx_errno;
            యొక్క->failed = ngx_file_info_n;
            తిరిగి NGX_FILE_ERROR;
        }

        తిరిగి rc;
    }

    fd = ngx_open_file_wrapper(name, యొక్క, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, సంవర్గమానం);

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        తిరిగి NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    ఒకవేళ (rc == NGX_FILE_ERROR) {
        యొక్క->err = ngx_errno;
        యొక్క->failed = ngx_fd_info_n;
    }

    ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    తిరిగి rc;
#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *యొక్క,
    ngx_log_t *సంవర్గమానం)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    ఒకవేళ (యొక్క->fd != NGX_INVALID_FILE) {

        ఒకవేళ (ngx_file_info_wrapper(name, యొక్క, &fi, సంవర్గమానం) == NGX_FILE_ERROR) {
            యొక్క->fd = NGX_INVALID_FILE;
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (యొక్క->uniq == ngx_file_uniq(&fi)) {
            వెళ్ళు పూర్తి;
        }

    } లేకపోతే ఒకవేళ (యొక్క->test_dir) {

        ఒకవేళ (ngx_file_info_wrapper(name, యొక్క, &fi, సంవర్గమానం) == NGX_FILE_ERROR) {
            యొక్క->fd = NGX_INVALID_FILE;
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (ngx_is_dir(&fi)) {
            వెళ్ళు పూర్తి;
        }
    }

    ఒకవేళ (!యొక్క->సంవర్గమానం) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, యొక్క, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, సంవర్గమానం);

    } లేకపోతే {
        fd = ngx_open_file_wrapper(name, యొక్క, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, సంవర్గమానం);
    }

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        యొక్క->fd = NGX_INVALID_FILE;
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, సంవర్గమానం, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        యొక్క->fd = NGX_INVALID_FILE;

        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_is_dir(&fi)) {
        ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        యొక్క->fd = NGX_INVALID_FILE;

    } లేకపోతే {
        యొక్క->fd = fd;

        ఒకవేళ (యొక్క->read_ahead && ngx_file_కొలత(&fi) > NGX_MIN_READ_AHEAD) {
            ఒకవేళ (ngx_read_ahead(fd, యొక్క->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        ఒకవేళ (యొక్క->directio <= ngx_file_కొలత(&fi)) {
            ఒకవేళ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } లేకపోతే {
                యొక్క->is_directio = 1;
            }
        }
    }

పూర్తి:

    యొక్క->uniq = ngx_file_uniq(&fi);
    యొక్క->mtime = ngx_file_mtime(&fi);
    యొక్క->కొలత = ngx_file_కొలత(&fi);
    యొక్క->fs_కొలత = ngx_file_fs_కొలత(&fi);
    యొక్క->is_dir = ngx_is_dir(&fi);
    యొక్క->is_file = ngx_is_file(&fi);
    యొక్క->is_link = ngx_is_link(&fi);
    యొక్క->is_exec = ngx_is_exec(&fi);

    తిరిగి NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

స్థిర శూన్య
ngx_open_file_కూడు_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *యొక్క, ngx_log_t *సంవర్గమానం)
{
    ngx_open_file_cache_event_t  *fev;

    ఒకవేళ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !యొక్క->events
        || file->event
        || యొక్క->fd == NGX_INVALID_FILE
        || file->ఉపయోగిస్తుంది < యొక్క->min_ఉపయోగిస్తుంది)
    {
        తిరిగి;
    }

    file->use_event = 0;

    file->event = ngx_calloc(పరిమాణం(ngx_event_t), సంవర్గమానం);
    ఒకవేళ (file->event== ఖాళీ) {
        తిరిగి;
    }

    fev = ngx_alloc(పరిమాణం(ngx_open_file_cache_event_t), సంవర్గమానం);
    ఒకవేళ (fev == ఖాళీ) {
        ngx_free(file->event);
        file->event = ఖాళీ;
        తిరిగి;
    }

    fev->fd = యొక్క->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->దత్తాంశం = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->సంవర్గమానం = ngx_చక్రం->సంవర్గమానం;

    ఒకవేళ (ngx_కూడు_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->దత్తాంశం);
        ngx_free(file->event);
        file->event = ఖాళీ;
        తిరిగి;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    తిరిగి;
}


స్థిర శూన్య
ngx_open_file_cleanup(శూన్య *దత్తాంశం)
{
    ngx_open_file_cache_cleanup_t  *c = దత్తాంశం;

    c->file->లెక్క--;

    ngx_close_cached_file(c->cache, c->file, c->min_ఉపయోగిస్తుంది, c->సంవర్గమానం);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->సంవర్గమానం);
}


స్థిర శూన్య
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ఉపయోగిస్తుంది, ngx_log_t *సంవర్గమానం)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->లెక్క, file->ఉపయోగిస్తుంది, file->close);

    ఒకవేళ (!file->close) {

        file->accessed = ngx_సమయం();

        ngx_వరుస_remove(&file->వరుస);

        ngx_వరుస_చొప్పించు_తల(&cache->expire_వరుస, &file->వరుస);

        ఒకవేళ (file->ఉపయోగిస్తుంది >= min_ఉపయోగిస్తుంది || file->లెక్క) {
            తిరిగి;
        }
    }

    ngx_open_file_del_event(file);

    ఒకవేళ (file->లెక్క) {
        తిరిగి;
    }

    ఒకవేళ (file->fd != NGX_INVALID_FILE) {

        ఒకవేళ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    ఒకవేళ (!file->close) {
        తిరిగి;
    }

    ngx_free(file->name);
    ngx_free(file);
}


స్థిర శూన్య
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    ఒకవేళ (file->event == ఖాళీ) {
        తిరిగి;
    }

    (శూన్య) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->లెక్క ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->దత్తాంశం);
    ngx_free(file->event);
    file->event = ఖాళీ;
    file->use_event = 0;
}


స్థిర శూన్య
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *సంవర్గమానం)
{
    సమయం_t                   now;
    ngx_వరుస_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_సమయం();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    అయితే (n < 3) {

        ఒకవేళ (ngx_వరుస_ఖాళీలు(&cache->expire_వరుస)) {
            తిరిగి;
        }

        q = ngx_వరుస_చివరి(&cache->expire_వరుస);

        file = ngx_వరుస_దత్తాంశం(q, ngx_cached_open_file_t, వరుస);

        ఒకవేళ (n++ != 0 && now - file->accessed <= cache->inactive) {
            తిరిగి;
        }

        ngx_వరుస_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->కణుపు);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0,
                       "expire cached open file: %s", file->name);

        ఒకవేళ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, సంవర్గమానం);

        } లేకపోతే {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


స్థిర శూన్య
ngx_open_file_cache_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_rbtree_కణుపు_t       **p;
    ngx_cached_open_file_t    *file, *file_తాత్కాలిక;

    కొరకు ( ;; ) {

        ఒకవేళ (కణుపు->కీలకం < తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->ఎడమ;

        } లేకపోతే ఒకవేళ (కణుపు->కీలకం > తాత్కాలిక->కీలకం) {

            p = &తాత్కాలిక->కుడి;

        } లేకపోతే { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) కణుపు;
            file_తాత్కాలిక = (ngx_cached_open_file_t *) తాత్కాలిక;

            p = (ngx_strcmp(file->name, file_తాత్కాలిక->name) < 0)
                    ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;
        }

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}


స్థిర ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_కణుపు_t       *కణుపు, *sentinel;
    ngx_cached_open_file_t  *file;

    కణుపు = cache->rbtree.మూలం;
    sentinel = cache->rbtree.sentinel;

    అయితే (కణుపు != sentinel) {

        ఒకవేళ (hash < కణుపు->కీలకం) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (hash > కణుపు->కీలకం) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) కణుపు;

        rc = ngx_strcmp(name->దత్తాంశం, file->name);

        ఒకవేళ (rc == 0) {
            తిరిగి file;
        }

        కణుపు = (rc < 0) ? కణుపు->ఎడమ : కణుపు->కుడి;
    }

    తిరిగి ఖాళీ;
}


స్థిర శూన్య
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->దత్తాంశం;
    file = fev->file;

    ngx_వరుస_remove(&file->వరుస);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->కణుపు);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ఖాళీ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->సంవర్గమానం);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->దత్తాంశం);
    ngx_free(ev);
}
