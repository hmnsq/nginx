
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


#परिभाषय NGX_MIN_READ_AHEAD  (128 * 1024)


स्थिर शून्य ngx_open_file_cache_cleanup(शून्य *दत्त);
#यदि (NGX_HAVE_OPENAT)
स्थिर ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ध्रुव u_char *name,
    ngx_int_t बहुलकम्, ngx_int_t create, ngx_int_t access, ngx_log_t *लघुगणकः);
#यदि (NGX_HAVE_O_मार्गः)
स्थिर ngx_int_t ngx_file_o_मार्गः_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *लघुगणकः);
#यदि_अन्तः
#यदि_अन्तः
स्थिर ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *सम्बन्धि, ngx_int_t बहुलकम्, ngx_int_t create,
    ngx_int_t access, ngx_log_t *लघुगणकः);
स्थिर ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *सम्बन्धि, ngx_file_info_t *fi, ngx_log_t *लघुगणकः);
स्थिर ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *सम्बन्धि, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_open_file_सङ्कलय_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *सम्बन्धि, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_open_file_cleanup(शून्य *दत्त);
स्थिर शून्य ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_उपयुङ्क्ते, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_open_file_del_event(ngx_cached_open_file_t *file);
स्थिर शून्य ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_open_file_cache_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
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
    यदि (cache == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_निवेशय_मानम्);

    ngx_queue_init(&cache->expire_queue);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_सङ्कलय(pool, 0);
    यदि (cln == रिक्त) {
        प्रत्यागम रिक्त;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->दत्त = cache;

    प्रत्यागम cache;
}


स्थिर शून्य
ngx_open_file_cache_cleanup(शून्य *दत्त)
{
    ngx_open_file_cache_t  *cache = दत्त;

    ngx_queue_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_आवृत्तिः->लघुगणकः, 0,
                   "open file cache cleanup");

    प्रति ( ;; ) {

        यदि (ngx_queue_रिक्तानि(&cache->expire_queue)) {
            भङ्ग;
        }

        q = ngx_queue_चरमः(&cache->expire_queue);

        file = ngx_queue_दत्त(q, ngx_cached_open_file_t, queue);

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_आवृत्तिः->लघुगणकः, 0,
                       "delete cached open file: %s", file->name);

        यदि (!file->err && !file->is_dir) {
            file->close = 1;
            file->गणनम् = 0;
            ngx_close_cached_file(cache, file, 0, ngx_आवृत्तिः->लघुगणकः);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    यदि (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    यदि (cache->rbtree.मूलम् != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *सम्बन्धि, ngx_pool_t *pool)
{
    time_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    सम्बन्धि->fd = NGX_INVALID_FILE;
    सम्बन्धि->err = 0;

    यदि (cache == रिक्त) {

        यदि (सम्बन्धि->test_only) {

            यदि (ngx_file_info_wrapper(name, सम्बन्धि, &fi, pool->लघुगणकः)
                == NGX_FILE_ERROR)
            {
                प्रत्यागम NGX_ERROR;
            }

            सम्बन्धि->uniq = ngx_file_uniq(&fi);
            सम्बन्धि->mtime = ngx_file_mtime(&fi);
            सम्बन्धि->आकार = ngx_file_आकार(&fi);
            सम्बन्धि->fs_आकार = ngx_file_fs_आकार(&fi);
            सम्बन्धि->is_dir = ngx_is_dir(&fi);
            सम्बन्धि->is_file = ngx_is_file(&fi);
            सम्बन्धि->is_link = ngx_is_link(&fi);
            सम्बन्धि->is_exec = ngx_is_exec(&fi);

            प्रत्यागम NGX_OK;
        }

        cln = ngx_pool_cleanup_सङ्कलय(pool, परिमाण(ngx_pool_cleanup_file_t));
        यदि (cln == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, सम्बन्धि, pool->लघुगणकः);

        यदि (rc == NGX_OK && !सम्बन्धि->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->दत्त;

            clnf->fd = सम्बन्धि->fd;
            clnf->name = name->दत्त;
            clnf->लघुगणकः = pool->लघुगणकः;
        }

        प्रत्यागम rc;
    }

    cln = ngx_pool_cleanup_सङ्कलय(pool, परिमाण(ngx_open_file_cache_cleanup_t));
    यदि (cln == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    now = ngx_time();

    hash = ngx_चक्रीयपरीक्षा_32_long(name->दत्त, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    यदि (file) {

        file->उपयुङ्क्ते++;

        ngx_queue_remove(&file->queue);

        यदि (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, सम्बन्धि, pool->लघुगणकः);

            यदि (rc != NGX_OK && (सम्बन्धि->err == 0 || !सम्बन्धि->errors)) {
                गच्छ failed;
            }

            गच्छ सङ्कलय_event;
        }

        यदि (file->use_event
            || (file->event == रिक्त
                && (सम्बन्धि->uniq == 0 || सम्बन्धि->uniq == file->uniq)
                && now - file->created < सम्बन्धि->valid
#यदि (NGX_HAVE_OPENAT)
                && सम्बन्धि->disable_symlinks == file->disable_symlinks
                && सम्बन्धि->disable_symlinks_from == file->disable_symlinks_from
#यदि_अन्तः
            ))
        {
            यदि (file->err == 0) {

                सम्बन्धि->fd = file->fd;
                सम्बन्धि->uniq = file->uniq;
                सम्बन्धि->mtime = file->mtime;
                सम्बन्धि->आकार = file->आकार;

                सम्बन्धि->is_dir = file->is_dir;
                सम्बन्धि->is_file = file->is_file;
                सम्बन्धि->is_link = file->is_link;
                सम्बन्धि->is_exec = file->is_exec;
                सम्बन्धि->is_directio = file->is_directio;

                यदि (!file->is_dir) {
                    file->गणनम्++;
                    ngx_open_file_सङ्कलय_event(cache, file, सम्बन्धि, pool->लघुगणकः);
                }

            } अन्यथा {
                सम्बन्धि->err = file->err;
#यदि (NGX_HAVE_OPENAT)
                सम्बन्धि->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#अन्यथा
                सम्बन्धि->failed = ngx_open_file_n;
#यदि_अन्तः
            }

            गच्छ प्राप्तम्;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->लघुगणकः, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->गणनम्, file->err);

        यदि (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            सम्बन्धि->test_dir = 1;
        }

        सम्बन्धि->fd = file->fd;
        सम्बन्धि->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, सम्बन्धि, pool->लघुगणकः);

        यदि (rc != NGX_OK && (सम्बन्धि->err == 0 || !सम्बन्धि->errors)) {
            गच्छ failed;
        }

        यदि (सम्बन्धि->is_dir) {

            यदि (file->is_dir || file->err) {
                गच्छ update;
            }

            /* file became directory */

        } अन्यथा यदि (सम्बन्धि->err == 0) {  /* file */

            यदि (file->is_dir || file->err) {
                गच्छ सङ्कलय_event;
            }

            यदि (सम्बन्धि->uniq == file->uniq) {

                यदि (file->event) {
                    file->use_event = 1;
                }

                सम्बन्धि->is_directio = file->is_directio;

                गच्छ update;
            }

            /* file was changed */

        } अन्यथा { /* error to cache */

            यदि (file->err || file->is_dir) {
                गच्छ update;
            }

            /* file was removed, etc. */
        }

        यदि (file->गणनम् == 0) {

            ngx_open_file_del_event(file);

            यदि (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->लघुगणकः, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            गच्छ सङ्कलय_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        file->close = 1;

        गच्छ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, सम्बन्धि, pool->लघुगणकः);

    यदि (rc != NGX_OK && (सम्बन्धि->err == 0 || !सम्बन्धि->errors)) {
        गच्छ failed;
    }

create:

    यदि (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->लघुगणकः);
    }

    file = ngx_alloc(परिमाण(ngx_cached_open_file_t), pool->लघुगणकः);

    यदि (file == रिक्त) {
        गच्छ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->लघुगणकः);

    यदि (file->name == रिक्त) {
        ngx_free(file);
        file = रिक्त;
        गच्छ failed;
    }

    ngx_cpystrn(file->name, name->दत्त, name->len + 1);

    file->ग्रन्थिः.कुञ्चिका = hash;

    ngx_rbtree_निवेशय(&cache->rbtree, &file->ग्रन्थिः);

    cache->current++;

    file->उपयुङ्क्ते = 1;
    file->गणनम् = 0;
    file->use_event = 0;
    file->event = रिक्त;

सङ्कलय_event:

    ngx_open_file_सङ्कलय_event(cache, file, सम्बन्धि, pool->लघुगणकः);

update:

    file->fd = सम्बन्धि->fd;
    file->err = सम्बन्धि->err;
#यदि (NGX_HAVE_OPENAT)
    file->disable_symlinks = सम्बन्धि->disable_symlinks;
    file->disable_symlinks_from = सम्बन्धि->disable_symlinks_from;
#यदि_अन्तः

    यदि (सम्बन्धि->err == 0) {
        file->uniq = सम्बन्धि->uniq;
        file->mtime = सम्बन्धि->mtime;
        file->आकार = सम्बन्धि->आकार;

        file->close = 0;

        file->is_dir = सम्बन्धि->is_dir;
        file->is_file = सम्बन्धि->is_file;
        file->is_link = सम्बन्धि->is_link;
        file->is_exec = सम्बन्धि->is_exec;
        file->is_directio = सम्बन्धि->is_directio;

        यदि (!सम्बन्धि->is_dir) {
            file->गणनम्++;
        }
    }

    file->created = now;

प्राप्तम्:

    file->accessed = now;

    ngx_queue_निवेशय_शीर्ष(&cache->expire_queue, &file->queue);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->लघुगणकः, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->गणनम्, file->err, file->उपयुङ्क्ते);

    यदि (सम्बन्धि->err == 0) {

        यदि (!सम्बन्धि->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->दत्त;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_उपयुङ्क्ते = सम्बन्धि->min_उपयुङ्क्ते;
            ofcln->लघुगणकः = pool->लघुगणकः;
        }

        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_ERROR;

failed:

    यदि (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        यदि (file->गणनम् == 0) {

            यदि (file->fd != NGX_INVALID_FILE) {
                यदि (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->लघुगणकः, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } अन्यथा {
            file->close = 1;
        }
    }

    यदि (सम्बन्धि->fd != NGX_INVALID_FILE) {
        यदि (ngx_close_file(सम्बन्धि->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    प्रत्यागम NGX_ERROR;
}


#यदि (NGX_HAVE_OPENAT)

स्थिर ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ध्रुव u_char *name,
    ngx_int_t बहुलकम्, ngx_int_t create, ngx_int_t access, ngx_log_t *लघुगणकः)
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

    fd = ngx_openat_file(at_fd, name, बहुलकम्, create, access);

    यदि (fd == NGX_INVALID_FILE) {
        प्रत्यागम NGX_INVALID_FILE;
    }

    यदि (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        गच्छ failed;
    }

#यदि (NGX_HAVE_O_मार्गः)
    यदि (ngx_file_o_मार्गः_info(fd, &fi, लघुगणकः) == NGX_ERROR) {
        err = ngx_errno;
        गच्छ failed;
    }
#अन्यथा
    यदि (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        गच्छ failed;
    }
#यदि_अन्तः

    यदि (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        गच्छ failed;
    }

    प्रत्यागम fd;

failed:

    यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_समुच्चयः_errno(err);

    प्रत्यागम NGX_INVALID_FILE;
}


#यदि (NGX_HAVE_O_मार्गः)

स्थिर ngx_int_t
ngx_file_o_मार्गः_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *लघुगणकः)
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

    यदि (use_fstat) {
        यदि (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            प्रत्यागम NGX_OK;
        }

        यदि (ngx_errno != NGX_EBADF) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, लघुगणकः, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    यदि (ngx_file_at_info(fd, "", fi, AT_रिक्तानि_मार्गः) != NGX_FILE_ERROR) {
        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_ERROR;
}

#यदि_अन्तः

#यदि_अन्तः /* NGX_HAVE_OPENAT */


स्थिर ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *सम्बन्धि,
    ngx_int_t बहुलकम्, ngx_int_t create, ngx_int_t access, ngx_log_t *लघुगणकः)
{
    ngx_fd_t  fd;

#यदि !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->दत्त, बहुलकम्, create, access);

    यदि (fd == NGX_INVALID_FILE) {
        सम्बन्धि->err = ngx_errno;
        सम्बन्धि->failed = ngx_open_file_n;
        प्रत्यागम NGX_INVALID_FILE;
    }

    प्रत्यागम fd;

#अन्यथा

    u_char           *p, *cp, *अन्तः;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    यदि (सम्बन्धि->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->दत्त, बहुलकम्, create, access);

        यदि (fd == NGX_INVALID_FILE) {
            सम्बन्धि->err = ngx_errno;
            सम्बन्धि->failed = ngx_open_file_n;
            प्रत्यागम NGX_INVALID_FILE;
        }

        प्रत्यागम fd;
    }

    p = name->दत्त;
    अन्तः = p + name->len;

    at_name = *name;

    यदि (सम्बन्धि->disable_symlinks_from) {

        cp = p + सम्बन्धि->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        यदि (at_fd == NGX_INVALID_FILE) {
            सम्बन्धि->err = ngx_errno;
            सम्बन्धि->failed = ngx_open_file_n;
            प्रत्यागम NGX_INVALID_FILE;
        }

        at_name.len = सम्बन्धि->disable_symlinks_from;
        p = cp + 1;

    } अन्यथा यदि (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        यदि (at_fd == NGX_INVALID_FILE) {
            सम्बन्धि->err = ngx_errno;
            सम्बन्धि->failed = ngx_openat_file_n;
            प्रत्यागम NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } अन्यथा {
        at_fd = NGX_AT_FDCWD;
    }

    प्रति ( ;; ) {
        cp = ngx_strlchr(p, अन्तः, '/');
        यदि (cp == रिक्त) {
            भङ्ग;
        }

        यदि (cp == p) {
            p++;
            अनुवर्त;
        }

        *cp = '\0';

        यदि (सम्बन्धि->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, लघुगणकः);

        } अन्यथा {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_अन्वेषणम्|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        यदि (fd == NGX_INVALID_FILE) {
            सम्बन्धि->err = ngx_errno;
            सम्बन्धि->failed = ngx_openat_file_n;
            गच्छ failed;
        }

        यदि (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.दत्त;
    }

    यदि (p == अन्तः) {

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

        fd = ngx_openat_file(at_fd, ".", बहुलकम्, create, access);
        गच्छ समाप्तम्;
    }

    यदि (सम्बन्धि->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, बहुलकम्, create, access, लघुगणकः);

    } अन्यथा {
        fd = ngx_openat_file(at_fd, p, बहुलकम्|NGX_FILE_NOFOLLOW, create, access);
    }

समाप्तम्:

    यदि (fd == NGX_INVALID_FILE) {
        सम्बन्धि->err = ngx_errno;
        सम्बन्धि->failed = ngx_openat_file_n;
    }

failed:

    यदि (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    प्रत्यागम fd;
#यदि_अन्तः
}


स्थिर ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *सम्बन्धि,
    ngx_file_info_t *fi, ngx_log_t *लघुगणकः)
{
    ngx_int_t  rc;

#यदि !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->दत्त, fi);

    यदि (rc == NGX_FILE_ERROR) {
        सम्बन्धि->err = ngx_errno;
        सम्बन्धि->failed = ngx_file_info_n;
        प्रत्यागम NGX_FILE_ERROR;
    }

    प्रत्यागम rc;

#अन्यथा

    ngx_fd_t  fd;

    यदि (सम्बन्धि->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->दत्त, fi);

        यदि (rc == NGX_FILE_ERROR) {
            सम्बन्धि->err = ngx_errno;
            सम्बन्धि->failed = ngx_file_info_n;
            प्रत्यागम NGX_FILE_ERROR;
        }

        प्रत्यागम rc;
    }

    fd = ngx_open_file_wrapper(name, सम्बन्धि, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, लघुगणकः);

    यदि (fd == NGX_INVALID_FILE) {
        प्रत्यागम NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    यदि (rc == NGX_FILE_ERROR) {
        सम्बन्धि->err = ngx_errno;
        सम्बन्धि->failed = ngx_fd_info_n;
    }

    यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    प्रत्यागम rc;
#यदि_अन्तः
}


स्थिर ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *सम्बन्धि,
    ngx_log_t *लघुगणकः)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    यदि (सम्बन्धि->fd != NGX_INVALID_FILE) {

        यदि (ngx_file_info_wrapper(name, सम्बन्धि, &fi, लघुगणकः) == NGX_FILE_ERROR) {
            सम्बन्धि->fd = NGX_INVALID_FILE;
            प्रत्यागम NGX_ERROR;
        }

        यदि (सम्बन्धि->uniq == ngx_file_uniq(&fi)) {
            गच्छ समाप्तम्;
        }

    } अन्यथा यदि (सम्बन्धि->test_dir) {

        यदि (ngx_file_info_wrapper(name, सम्बन्धि, &fi, लघुगणकः) == NGX_FILE_ERROR) {
            सम्बन्धि->fd = NGX_INVALID_FILE;
            प्रत्यागम NGX_ERROR;
        }

        यदि (ngx_is_dir(&fi)) {
            गच्छ समाप्तम्;
        }
    }

    यदि (!सम्बन्धि->लघुगणकः) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, सम्बन्धि, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, लघुगणकः);

    } अन्यथा {
        fd = ngx_open_file_wrapper(name, सम्बन्धि, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, लघुगणकः);
    }

    यदि (fd == NGX_INVALID_FILE) {
        सम्बन्धि->fd = NGX_INVALID_FILE;
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, लघुगणकः, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        सम्बन्धि->fd = NGX_INVALID_FILE;

        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_is_dir(&fi)) {
        यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        सम्बन्धि->fd = NGX_INVALID_FILE;

    } अन्यथा {
        सम्बन्धि->fd = fd;

        यदि (सम्बन्धि->read_ahead && ngx_file_आकार(&fi) > NGX_MIN_READ_AHEAD) {
            यदि (ngx_read_ahead(fd, सम्बन्धि->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        यदि (सम्बन्धि->directio <= ngx_file_आकार(&fi)) {
            यदि (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } अन्यथा {
                सम्बन्धि->is_directio = 1;
            }
        }
    }

समाप्तम्:

    सम्बन्धि->uniq = ngx_file_uniq(&fi);
    सम्बन्धि->mtime = ngx_file_mtime(&fi);
    सम्बन्धि->आकार = ngx_file_आकार(&fi);
    सम्बन्धि->fs_आकार = ngx_file_fs_आकार(&fi);
    सम्बन्धि->is_dir = ngx_is_dir(&fi);
    सम्बन्धि->is_file = ngx_is_file(&fi);
    सम्बन्धि->is_link = ngx_is_link(&fi);
    सम्बन्धि->is_exec = ngx_is_exec(&fi);

    प्रत्यागम NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

स्थिर शून्य
ngx_open_file_सङ्कलय_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *सम्बन्धि, ngx_log_t *लघुगणकः)
{
    ngx_open_file_cache_event_t  *fev;

    यदि (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !सम्बन्धि->events
        || file->event
        || सम्बन्धि->fd == NGX_INVALID_FILE
        || file->उपयुङ्क्ते < सम्बन्धि->min_उपयुङ्क्ते)
    {
        प्रत्यागम;
    }

    file->use_event = 0;

    file->event = ngx_calloc(परिमाण(ngx_event_t), लघुगणकः);
    यदि (file->event== रिक्त) {
        प्रत्यागम;
    }

    fev = ngx_alloc(परिमाण(ngx_open_file_cache_event_t), लघुगणकः);
    यदि (fev == रिक्त) {
        ngx_free(file->event);
        file->event = रिक्त;
        प्रत्यागम;
    }

    fev->fd = सम्बन्धि->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->दत्त = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->लघुगणकः = ngx_आवृत्तिः->लघुगणकः;

    यदि (ngx_सङ्कलय_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->दत्त);
        ngx_free(file->event);
        file->event = रिक्त;
        प्रत्यागम;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    प्रत्यागम;
}


स्थिर शून्य
ngx_open_file_cleanup(शून्य *दत्त)
{
    ngx_open_file_cache_cleanup_t  *c = दत्त;

    c->file->गणनम्--;

    ngx_close_cached_file(c->cache, c->file, c->min_उपयुङ्क्ते, c->लघुगणकः);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->लघुगणकः);
}


स्थिर शून्य
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_उपयुङ्क्ते, ngx_log_t *लघुगणकः)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, लघुगणकः, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->गणनम्, file->उपयुङ्क्ते, file->close);

    यदि (!file->close) {

        file->accessed = ngx_time();

        ngx_queue_remove(&file->queue);

        ngx_queue_निवेशय_शीर्ष(&cache->expire_queue, &file->queue);

        यदि (file->उपयुङ्क्ते >= min_उपयुङ्क्ते || file->गणनम्) {
            प्रत्यागम;
        }
    }

    ngx_open_file_del_event(file);

    यदि (file->गणनम्) {
        प्रत्यागम;
    }

    यदि (file->fd != NGX_INVALID_FILE) {

        यदि (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    यदि (!file->close) {
        प्रत्यागम;
    }

    ngx_free(file->name);
    ngx_free(file);
}


स्थिर शून्य
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    यदि (file->event == रिक्त) {
        प्रत्यागम;
    }

    (शून्य) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->गणनम् ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->दत्त);
    ngx_free(file->event);
    file->event = रिक्त;
    file->use_event = 0;
}


स्थिर शून्य
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *लघुगणकः)
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

    यावत् (n < 3) {

        यदि (ngx_queue_रिक्तानि(&cache->expire_queue)) {
            प्रत्यागम;
        }

        q = ngx_queue_चरमः(&cache->expire_queue);

        file = ngx_queue_दत्त(q, ngx_cached_open_file_t, queue);

        यदि (n++ != 0 && now - file->accessed <= cache->inactive) {
            प्रत्यागम;
        }

        ngx_queue_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ग्रन्थिः);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, लघुगणकः, 0,
                       "expire cached open file: %s", file->name);

        यदि (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, लघुगणकः);

        } अन्यथा {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


स्थिर शून्य
ngx_open_file_cache_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t       **p;
    ngx_cached_open_file_t    *file, *file_अस्थायी;

    प्रति ( ;; ) {

        यदि (ग्रन्थिः->कुञ्चिका < अस्थायी->कुञ्चिका) {

            p = &अस्थायी->वामः;

        } अन्यथा यदि (ग्रन्थिः->कुञ्चिका > अस्थायी->कुञ्चिका) {

            p = &अस्थायी->दक्षिणः;

        } अन्यथा { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ग्रन्थिः;
            file_अस्थायी = (ngx_cached_open_file_t *) अस्थायी;

            p = (ngx_strcmp(file->name, file_अस्थायी->name) < 0)
                    ? &अस्थायी->वामः : &अस्थायी->दक्षिणः;
        }

        यदि (*p == sentinel) {
            भङ्ग;
        }

        अस्थायी = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
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

    यावत् (ग्रन्थिः != sentinel) {

        यदि (hash < ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        यदि (hash > ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ग्रन्थिः;

        rc = ngx_strcmp(name->दत्त, file->name);

        यदि (rc == 0) {
            प्रत्यागम file;
        }

        ग्रन्थिः = (rc < 0) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
    }

    प्रत्यागम रिक्त;
}


स्थिर शून्य
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->दत्त;
    file = fev->file;

    ngx_queue_remove(&file->queue);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ग्रन्थिः);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = रिक्त;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->लघुगणकः);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->दत्त);
    ngx_free(ev);
}
