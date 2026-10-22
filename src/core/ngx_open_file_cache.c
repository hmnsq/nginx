
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


/*
 * open file cache caches
 *    open file handles with stat() info;
 *    directories stat() info;
 *    files and directories errors: not found, access denied, etc.
 */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MIN_READ_AHEAD  (128 * 1024)


ਸਥਿਰ ਖਾਲੀ ngx_open_file_cache_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);
#ਜੇ (NGX_HAVE_OPENAT)
ਸਥਿਰ ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ਅਚਲ u_char *name,
    ngx_int_t ਬਹੁਲਕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ਲਘੂਗਣਕ);
#ਜੇ (NGX_HAVE_O_ਰਸਤਾ)
ਸਥਿਰ ngx_int_t ngx_file_o_ਰਸਤਾ_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *ਲਘੂਗਣਕ);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
ਸਥਿਰ ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *ਦਾ, ngx_int_t ਬਹੁਲਕ, ngx_int_t create,
    ngx_int_t access, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *ਦਾ, ngx_file_info_t *fi, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *ਦਾ, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_open_file_ਜੋੜੋ_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *ਦਾ, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_open_file_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ਵਰਤਦਾ, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ਸਥਿਰ ਖਾਲੀ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_open_file_cache_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);
ਸਥਿਰ ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ਸਥਿਰ ਖਾਲੀ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, ਸਮਾਂ_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, ਆਕਾਰ(ngx_open_file_cache_t));
    ਜੇ (cache == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ਪਾਓ_ਮੁੱਲ);

    ngx_ਕਤਾਰ_init(&cache->expire_ਕਤਾਰ);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_ਜੋੜੋ(pool, 0);
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->ਅੰਕੜੇ = cache;

    ਵਾਪਸ cache;
}


ਸਥਿਰ ਖਾਲੀ
ngx_open_file_cache_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_open_file_cache_t  *cache = ਅੰਕੜੇ;

    ngx_ਕਤਾਰ_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                   "open file cache cleanup");

    ਲਈ ( ;; ) {

        ਜੇ (ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&cache->expire_ਕਤਾਰ)) {
            ਰੋਕੋ;
        }

        q = ngx_ਕਤਾਰ_ਆਖਰੀ(&cache->expire_ਕਤਾਰ);

        file = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_cached_open_file_t, ਕਤਾਰ);

        ngx_ਕਤਾਰ_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ਗੰਢ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                       "delete cached open file: %s", file->name);

        ਜੇ (!file->err && !file->is_dir) {
            file->close = 1;
            file->ਗਿਣਤੀ_2 = 0;
            ngx_close_cached_file(cache, file, 0, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ);

        } ਨਹੀਂਤਾਂ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    ਜੇ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    ਜੇ (cache->rbtree.ਜੜ੍ਹ != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *ਦਾ, ngx_pool_t *pool)
{
    ਸਮਾਂ_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    ਦਾ->fd = NGX_INVALID_FILE;
    ਦਾ->err = 0;

    ਜੇ (cache == ਨਿਰਾ) {

        ਜੇ (ਦਾ->test_only) {

            ਜੇ (ngx_file_info_wrapper(name, ਦਾ, &fi, pool->ਲਘੂਗਣਕ)
                == NGX_FILE_ERROR)
            {
                ਵਾਪਸ NGX_ERROR;
            }

            ਦਾ->uniq = ngx_file_uniq(&fi);
            ਦਾ->mtime = ngx_file_mtime(&fi);
            ਦਾ->ਮਾਪ = ngx_file_ਮਾਪ(&fi);
            ਦਾ->fs_ਮਾਪ = ngx_file_fs_ਮਾਪ(&fi);
            ਦਾ->is_dir = ngx_is_dir(&fi);
            ਦਾ->is_file = ngx_is_file(&fi);
            ਦਾ->is_link = ngx_is_link(&fi);
            ਦਾ->is_exec = ngx_is_exec(&fi);

            ਵਾਪਸ NGX_OK;
        }

        cln = ngx_pool_cleanup_ਜੋੜੋ(pool, ਆਕਾਰ(ngx_pool_cleanup_file_t));
        ਜੇ (cln == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, ਦਾ, pool->ਲਘੂਗਣਕ);

        ਜੇ (rc == NGX_OK && !ਦਾ->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->ਅੰਕੜੇ;

            clnf->fd = ਦਾ->fd;
            clnf->name = name->ਅੰਕੜੇ;
            clnf->ਲਘੂਗਣਕ = pool->ਲਘੂਗਣਕ;
        }

        ਵਾਪਸ rc;
    }

    cln = ngx_pool_cleanup_ਜੋੜੋ(pool, ਆਕਾਰ(ngx_open_file_cache_cleanup_t));
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    now = ngx_ਸਮਾਂ();

    hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_long(name->ਅੰਕੜੇ, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    ਜੇ (file) {

        file->ਵਰਤਦਾ++;

        ngx_ਕਤਾਰ_remove(&file->ਕਤਾਰ);

        ਜੇ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, ਦਾ, pool->ਲਘੂਗਣਕ);

            ਜੇ (rc != NGX_OK && (ਦਾ->err == 0 || !ਦਾ->errors)) {
                ਜਾਓ failed;
            }

            ਜਾਓ ਜੋੜੋ_event;
        }

        ਜੇ (file->use_event
            || (file->event == ਨਿਰਾ
                && (ਦਾ->uniq == 0 || ਦਾ->uniq == file->uniq)
                && now - file->created < ਦਾ->valid
#ਜੇ (NGX_HAVE_OPENAT)
                && ਦਾ->disable_symlinks == file->disable_symlinks
                && ਦਾ->disable_symlinks_from == file->disable_symlinks_from
#ਜੇ_ਅੰਤ
            ))
        {
            ਜੇ (file->err == 0) {

                ਦਾ->fd = file->fd;
                ਦਾ->uniq = file->uniq;
                ਦਾ->mtime = file->mtime;
                ਦਾ->ਮਾਪ = file->ਮਾਪ;

                ਦਾ->is_dir = file->is_dir;
                ਦਾ->is_file = file->is_file;
                ਦਾ->is_link = file->is_link;
                ਦਾ->is_exec = file->is_exec;
                ਦਾ->is_directio = file->is_directio;

                ਜੇ (!file->is_dir) {
                    file->ਗਿਣਤੀ_2++;
                    ngx_open_file_ਜੋੜੋ_event(cache, file, ਦਾ, pool->ਲਘੂਗਣਕ);
                }

            } ਨਹੀਂਤਾਂ {
                ਦਾ->err = file->err;
#ਜੇ (NGX_HAVE_OPENAT)
                ਦਾ->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#ਨਹੀਂਤਾਂ
                ਦਾ->failed = ngx_open_file_n;
#ਜੇ_ਅੰਤ
            }

            ਜਾਓ ਮਿਲਿਆ;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->ਲਘੂਗਣਕ, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->ਗਿਣਤੀ_2, file->err);

        ਜੇ (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            ਦਾ->test_dir = 1;
        }

        ਦਾ->fd = file->fd;
        ਦਾ->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, ਦਾ, pool->ਲਘੂਗਣਕ);

        ਜੇ (rc != NGX_OK && (ਦਾ->err == 0 || !ਦਾ->errors)) {
            ਜਾਓ failed;
        }

        ਜੇ (ਦਾ->is_dir) {

            ਜੇ (file->is_dir || file->err) {
                ਜਾਓ update;
            }

            /* file became directory */

        } ਨਹੀਂਤਾਂ ਜੇ (ਦਾ->err == 0) {  /* file */

            ਜੇ (file->is_dir || file->err) {
                ਜਾਓ ਜੋੜੋ_event;
            }

            ਜੇ (ਦਾ->uniq == file->uniq) {

                ਜੇ (file->event) {
                    file->use_event = 1;
                }

                ਦਾ->is_directio = file->is_directio;

                ਜਾਓ update;
            }

            /* file was changed */

        } ਨਹੀਂਤਾਂ { /* error to cache */

            ਜੇ (file->err || file->is_dir) {
                ਜਾਓ update;
            }

            /* file was removed, etc. */
        }

        ਜੇ (file->ਗਿਣਤੀ_2 == 0) {

            ngx_open_file_del_event(file);

            ਜੇ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            ਜਾਓ ਜੋੜੋ_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ਗੰਢ);

        cache->current--;

        file->close = 1;

        ਜਾਓ create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, ਦਾ, pool->ਲਘੂਗਣਕ);

    ਜੇ (rc != NGX_OK && (ਦਾ->err == 0 || !ਦਾ->errors)) {
        ਜਾਓ failed;
    }

create:

    ਜੇ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->ਲਘੂਗਣਕ);
    }

    file = ngx_alloc(ਆਕਾਰ(ngx_cached_open_file_t), pool->ਲਘੂਗਣਕ);

    ਜੇ (file == ਨਿਰਾ) {
        ਜਾਓ failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->ਲਘੂਗਣਕ);

    ਜੇ (file->name == ਨਿਰਾ) {
        ngx_free(file);
        file = ਨਿਰਾ;
        ਜਾਓ failed;
    }

    ngx_cpystrn(file->name, name->ਅੰਕੜੇ, name->len + 1);

    file->ਗੰਢ.ਕੁੰਜੀ = hash;

    ngx_rbtree_ਪਾਓ(&cache->rbtree, &file->ਗੰਢ);

    cache->current++;

    file->ਵਰਤਦਾ = 1;
    file->ਗਿਣਤੀ_2 = 0;
    file->use_event = 0;
    file->event = ਨਿਰਾ;

ਜੋੜੋ_event:

    ngx_open_file_ਜੋੜੋ_event(cache, file, ਦਾ, pool->ਲਘੂਗਣਕ);

update:

    file->fd = ਦਾ->fd;
    file->err = ਦਾ->err;
#ਜੇ (NGX_HAVE_OPENAT)
    file->disable_symlinks = ਦਾ->disable_symlinks;
    file->disable_symlinks_from = ਦਾ->disable_symlinks_from;
#ਜੇ_ਅੰਤ

    ਜੇ (ਦਾ->err == 0) {
        file->uniq = ਦਾ->uniq;
        file->mtime = ਦਾ->mtime;
        file->ਮਾਪ = ਦਾ->ਮਾਪ;

        file->close = 0;

        file->is_dir = ਦਾ->is_dir;
        file->is_file = ਦਾ->is_file;
        file->is_link = ਦਾ->is_link;
        file->is_exec = ਦਾ->is_exec;
        file->is_directio = ਦਾ->is_directio;

        ਜੇ (!ਦਾ->is_dir) {
            file->ਗਿਣਤੀ_2++;
        }
    }

    file->created = now;

ਮਿਲਿਆ:

    file->accessed = now;

    ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&cache->expire_ਕਤਾਰ, &file->ਕਤਾਰ);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->ਲਘੂਗਣਕ, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->ਗਿਣਤੀ_2, file->err, file->ਵਰਤਦਾ);

    ਜੇ (ਦਾ->err == 0) {

        ਜੇ (!ਦਾ->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->ਅੰਕੜੇ;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_ਵਰਤਦਾ = ਦਾ->min_ਵਰਤਦਾ;
            ofcln->ਲਘੂਗਣਕ = pool->ਲਘੂਗਣਕ;
        }

        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_ERROR;

failed:

    ਜੇ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ਗੰਢ);

        cache->current--;

        ਜੇ (file->ਗਿਣਤੀ_2 == 0) {

            ਜੇ (file->fd != NGX_INVALID_FILE) {
                ਜੇ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->ਲਘੂਗਣਕ, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } ਨਹੀਂਤਾਂ {
            file->close = 1;
        }
    }

    ਜੇ (ਦਾ->fd != NGX_INVALID_FILE) {
        ਜੇ (ngx_close_file(ਦਾ->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    ਵਾਪਸ NGX_ERROR;
}


#ਜੇ (NGX_HAVE_OPENAT)

ਸਥਿਰ ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ਅਚਲ u_char *name,
    ngx_int_t ਬਹੁਲਕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ਲਘੂਗਣਕ)
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

    fd = ngx_openat_file(at_fd, name, ਬਹੁਲਕ, create, access);

    ਜੇ (fd == NGX_INVALID_FILE) {
        ਵਾਪਸ NGX_INVALID_FILE;
    }

    ਜੇ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        ਜਾਓ failed;
    }

#ਜੇ (NGX_HAVE_O_ਰਸਤਾ)
    ਜੇ (ngx_file_o_ਰਸਤਾ_info(fd, &fi, ਲਘੂਗਣਕ) == NGX_ERROR) {
        err = ngx_errno;
        ਜਾਓ failed;
    }
#ਨਹੀਂਤਾਂ
    ਜੇ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        ਜਾਓ failed;
    }
#ਜੇ_ਅੰਤ

    ਜੇ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        ਜਾਓ failed;
    }

    ਵਾਪਸ fd;

failed:

    ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_ਸੈੱਟ_errno(err);

    ਵਾਪਸ NGX_INVALID_FILE;
}


#ਜੇ (NGX_HAVE_O_ਰਸਤਾ)

ਸਥਿਰ ngx_int_t
ngx_file_o_ਰਸਤਾ_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *ਲਘੂਗਣਕ)
{
    ਸਥਿਰ ngx_uint_t  use_fstat = 1;

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

    ਜੇ (use_fstat) {
        ਜੇ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            ਵਾਪਸ NGX_OK;
        }

        ਜੇ (ngx_errno != NGX_EBADF) {
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, ਲਘੂਗਣਕ, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    ਜੇ (ngx_file_at_info(fd, "", fi, AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ) != NGX_FILE_ERROR) {
        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_ERROR;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* NGX_HAVE_OPENAT */


ਸਥਿਰ ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *ਦਾ,
    ngx_int_t ਬਹੁਲਕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_fd_t  fd;

#ਜੇ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->ਅੰਕੜੇ, ਬਹੁਲਕ, create, access);

    ਜੇ (fd == NGX_INVALID_FILE) {
        ਦਾ->err = ngx_errno;
        ਦਾ->failed = ngx_open_file_n;
        ਵਾਪਸ NGX_INVALID_FILE;
    }

    ਵਾਪਸ fd;

#ਨਹੀਂਤਾਂ

    u_char           *p, *cp, *ਸਿਰਾ;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    ਜੇ (ਦਾ->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->ਅੰਕੜੇ, ਬਹੁਲਕ, create, access);

        ਜੇ (fd == NGX_INVALID_FILE) {
            ਦਾ->err = ngx_errno;
            ਦਾ->failed = ngx_open_file_n;
            ਵਾਪਸ NGX_INVALID_FILE;
        }

        ਵਾਪਸ fd;
    }

    p = name->ਅੰਕੜੇ;
    ਸਿਰਾ = p + name->len;

    at_name = *name;

    ਜੇ (ਦਾ->disable_symlinks_from) {

        cp = p + ਦਾ->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_ਖੋਜ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        ਜੇ (at_fd == NGX_INVALID_FILE) {
            ਦਾ->err = ngx_errno;
            ਦਾ->failed = ngx_open_file_n;
            ਵਾਪਸ NGX_INVALID_FILE;
        }

        at_name.len = ਦਾ->disable_symlinks_from;
        p = cp + 1;

    } ਨਹੀਂਤਾਂ ਜੇ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_ਖੋਜ|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        ਜੇ (at_fd == NGX_INVALID_FILE) {
            ਦਾ->err = ngx_errno;
            ਦਾ->failed = ngx_openat_file_n;
            ਵਾਪਸ NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } ਨਹੀਂਤਾਂ {
        at_fd = NGX_AT_FDCWD;
    }

    ਲਈ ( ;; ) {
        cp = ngx_strlchr(p, ਸਿਰਾ, '/');
        ਜੇ (cp == ਨਿਰਾ) {
            ਰੋਕੋ;
        }

        ਜੇ (cp == p) {
            p++;
            ਜਾਰੀ;
        }

        *cp = '\0';

        ਜੇ (ਦਾ->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_ਖੋਜ|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, ਲਘੂਗਣਕ);

        } ਨਹੀਂਤਾਂ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_ਖੋਜ|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        ਜੇ (fd == NGX_INVALID_FILE) {
            ਦਾ->err = ngx_errno;
            ਦਾ->failed = ngx_openat_file_n;
            ਜਾਓ failed;
        }

        ਜੇ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.ਅੰਕੜੇ;
    }

    ਜੇ (p == ਸਿਰਾ) {

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

        fd = ngx_openat_file(at_fd, ".", ਬਹੁਲਕ, create, access);
        ਜਾਓ ਪੂਰਾ;
    }

    ਜੇ (ਦਾ->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, ਬਹੁਲਕ, create, access, ਲਘੂਗਣਕ);

    } ਨਹੀਂਤਾਂ {
        fd = ngx_openat_file(at_fd, p, ਬਹੁਲਕ|NGX_FILE_NOFOLLOW, create, access);
    }

ਪੂਰਾ:

    ਜੇ (fd == NGX_INVALID_FILE) {
        ਦਾ->err = ngx_errno;
        ਦਾ->failed = ngx_openat_file_n;
    }

failed:

    ਜੇ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    ਵਾਪਸ fd;
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *ਦਾ,
    ngx_file_info_t *fi, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_int_t  rc;

#ਜੇ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->ਅੰਕੜੇ, fi);

    ਜੇ (rc == NGX_FILE_ERROR) {
        ਦਾ->err = ngx_errno;
        ਦਾ->failed = ngx_file_info_n;
        ਵਾਪਸ NGX_FILE_ERROR;
    }

    ਵਾਪਸ rc;

#ਨਹੀਂਤਾਂ

    ngx_fd_t  fd;

    ਜੇ (ਦਾ->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->ਅੰਕੜੇ, fi);

        ਜੇ (rc == NGX_FILE_ERROR) {
            ਦਾ->err = ngx_errno;
            ਦਾ->failed = ngx_file_info_n;
            ਵਾਪਸ NGX_FILE_ERROR;
        }

        ਵਾਪਸ rc;
    }

    fd = ngx_open_file_wrapper(name, ਦਾ, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, ਲਘੂਗਣਕ);

    ਜੇ (fd == NGX_INVALID_FILE) {
        ਵਾਪਸ NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    ਜੇ (rc == NGX_FILE_ERROR) {
        ਦਾ->err = ngx_errno;
        ਦਾ->failed = ngx_fd_info_n;
    }

    ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    ਵਾਪਸ rc;
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *ਦਾ,
    ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    ਜੇ (ਦਾ->fd != NGX_INVALID_FILE) {

        ਜੇ (ngx_file_info_wrapper(name, ਦਾ, &fi, ਲਘੂਗਣਕ) == NGX_FILE_ERROR) {
            ਦਾ->fd = NGX_INVALID_FILE;
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਦਾ->uniq == ngx_file_uniq(&fi)) {
            ਜਾਓ ਪੂਰਾ;
        }

    } ਨਹੀਂਤਾਂ ਜੇ (ਦਾ->test_dir) {

        ਜੇ (ngx_file_info_wrapper(name, ਦਾ, &fi, ਲਘੂਗਣਕ) == NGX_FILE_ERROR) {
            ਦਾ->fd = NGX_INVALID_FILE;
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ngx_is_dir(&fi)) {
            ਜਾਓ ਪੂਰਾ;
        }
    }

    ਜੇ (!ਦਾ->ਲਘੂਗਣਕ) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, ਦਾ, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, ਲਘੂਗਣਕ);

    } ਨਹੀਂਤਾਂ {
        fd = ngx_open_file_wrapper(name, ਦਾ, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, ਲਘੂਗਣਕ);
    }

    ਜੇ (fd == NGX_INVALID_FILE) {
        ਦਾ->fd = NGX_INVALID_FILE;
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        ਦਾ->fd = NGX_INVALID_FILE;

        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_is_dir(&fi)) {
        ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        ਦਾ->fd = NGX_INVALID_FILE;

    } ਨਹੀਂਤਾਂ {
        ਦਾ->fd = fd;

        ਜੇ (ਦਾ->read_ahead && ngx_file_ਮਾਪ(&fi) > NGX_MIN_READ_AHEAD) {
            ਜੇ (ngx_read_ahead(fd, ਦਾ->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        ਜੇ (ਦਾ->directio <= ngx_file_ਮਾਪ(&fi)) {
            ਜੇ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } ਨਹੀਂਤਾਂ {
                ਦਾ->is_directio = 1;
            }
        }
    }

ਪੂਰਾ:

    ਦਾ->uniq = ngx_file_uniq(&fi);
    ਦਾ->mtime = ngx_file_mtime(&fi);
    ਦਾ->ਮਾਪ = ngx_file_ਮਾਪ(&fi);
    ਦਾ->fs_ਮਾਪ = ngx_file_fs_ਮਾਪ(&fi);
    ਦਾ->is_dir = ngx_is_dir(&fi);
    ਦਾ->is_file = ngx_is_file(&fi);
    ਦਾ->is_link = ngx_is_link(&fi);
    ਦਾ->is_exec = ngx_is_exec(&fi);

    ਵਾਪਸ NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ਸਥਿਰ ਖਾਲੀ
ngx_open_file_ਜੋੜੋ_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *ਦਾ, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_open_file_cache_event_t  *fev;

    ਜੇ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !ਦਾ->events
        || file->event
        || ਦਾ->fd == NGX_INVALID_FILE
        || file->ਵਰਤਦਾ < ਦਾ->min_ਵਰਤਦਾ)
    {
        ਵਾਪਸ;
    }

    file->use_event = 0;

    file->event = ngx_calloc(ਆਕਾਰ(ngx_event_t), ਲਘੂਗਣਕ);
    ਜੇ (file->event== ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    fev = ngx_alloc(ਆਕਾਰ(ngx_open_file_cache_event_t), ਲਘੂਗਣਕ);
    ਜੇ (fev == ਨਿਰਾ) {
        ngx_free(file->event);
        file->event = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    fev->fd = ਦਾ->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->ਅੰਕੜੇ = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->ਲਘੂਗਣਕ = ngx_ਚੱਕਰ->ਲਘੂਗਣਕ;

    ਜੇ (ngx_ਜੋੜੋ_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->ਅੰਕੜੇ);
        ngx_free(file->event);
        file->event = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    ਵਾਪਸ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_open_file_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_open_file_cache_cleanup_t  *c = ਅੰਕੜੇ;

    c->file->ਗਿਣਤੀ_2--;

    ngx_close_cached_file(c->cache, c->file, c->min_ਵਰਤਦਾ, c->ਲਘੂਗਣਕ);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->ਲਘੂਗਣਕ);
}


ਸਥਿਰ ਖਾਲੀ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ਵਰਤਦਾ, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->ਗਿਣਤੀ_2, file->ਵਰਤਦਾ, file->close);

    ਜੇ (!file->close) {

        file->accessed = ngx_ਸਮਾਂ();

        ngx_ਕਤਾਰ_remove(&file->ਕਤਾਰ);

        ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(&cache->expire_ਕਤਾਰ, &file->ਕਤਾਰ);

        ਜੇ (file->ਵਰਤਦਾ >= min_ਵਰਤਦਾ || file->ਗਿਣਤੀ_2) {
            ਵਾਪਸ;
        }
    }

    ngx_open_file_del_event(file);

    ਜੇ (file->ਗਿਣਤੀ_2) {
        ਵਾਪਸ;
    }

    ਜੇ (file->fd != NGX_INVALID_FILE) {

        ਜੇ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    ਜੇ (!file->close) {
        ਵਾਪਸ;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ਸਥਿਰ ਖਾਲੀ
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    ਜੇ (file->event == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    (ਖਾਲੀ) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->ਗਿਣਤੀ_2 ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->ਅੰਕੜੇ);
    ngx_free(file->event);
    file->event = ਨਿਰਾ;
    file->use_event = 0;
}


ਸਥਿਰ ਖਾਲੀ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *ਲਘੂਗਣਕ)
{
    ਸਮਾਂ_t                   now;
    ngx_ਕਤਾਰ_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_ਸਮਾਂ();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    ਜਦੋਂ (n < 3) {

        ਜੇ (ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(&cache->expire_ਕਤਾਰ)) {
            ਵਾਪਸ;
        }

        q = ngx_ਕਤਾਰ_ਆਖਰੀ(&cache->expire_ਕਤਾਰ);

        file = ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, ngx_cached_open_file_t, ਕਤਾਰ);

        ਜੇ (n++ != 0 && now - file->accessed <= cache->inactive) {
            ਵਾਪਸ;
        }

        ngx_ਕਤਾਰ_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ਗੰਢ);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0,
                       "expire cached open file: %s", file->name);

        ਜੇ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, ਲਘੂਗਣਕ);

        } ਨਹੀਂਤਾਂ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_open_file_cache_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_rbtree_ਗੰਢ_t       **p;
    ngx_cached_open_file_t    *file, *file_ਆਰਜ਼ੀ;

    ਲਈ ( ;; ) {

        ਜੇ (ਗੰਢ->ਕੁੰਜੀ < ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਖੱਬਾ;

        } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->ਕੁੰਜੀ > ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = &ਆਰਜ਼ੀ->ਸੱਜਾ;

        } ਨਹੀਂਤਾਂ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ਗੰਢ;
            file_ਆਰਜ਼ੀ = (ngx_cached_open_file_t *) ਆਰਜ਼ੀ;

            p = (ngx_strcmp(file->name, file_ਆਰਜ਼ੀ->name) < 0)
                    ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;
        }

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}


ਸਥਿਰ ngx_cached_open_file_t *
ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash)
{
    ngx_int_t                rc;
    ngx_rbtree_ਗੰਢ_t       *ਗੰਢ, *sentinel;
    ngx_cached_open_file_t  *file;

    ਗੰਢ = cache->rbtree.ਜੜ੍ਹ;
    sentinel = cache->rbtree.sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        ਜੇ (hash < ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (hash > ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ਗੰਢ;

        rc = ngx_strcmp(name->ਅੰਕੜੇ, file->name);

        ਜੇ (rc == 0) {
            ਵਾਪਸ file;
        }

        ਗੰਢ = (rc < 0) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->ਅੰਕੜੇ;
    file = fev->file;

    ngx_ਕਤਾਰ_remove(&file->ਕਤਾਰ);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ਗੰਢ);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ਨਿਰਾ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->ਲਘੂਗਣਕ);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->ਅੰਕੜੇ);
    ngx_free(ev);
}
