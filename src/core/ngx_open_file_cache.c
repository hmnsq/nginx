
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


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MIN_READ_AHEAD  (128 * 1024)


ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
#ಆದರೆ (NGX_HAVE_OPENAT)
ಸ್ಥಿರ ngx_fd_t ngx_openat_file_owner(ngx_fd_t at_fd, ಸ್ಥಿರಾಂಕ u_char *name,
    ngx_int_t ಬಹುಲಕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ಲಘುಗಣಕ);
#ಆದರೆ (NGX_HAVE_O_ಮಾರ್ಗ)
ಸ್ಥಿರ ngx_int_t ngx_file_o_ಮಾರ್ಗ_info(ngx_fd_t fd, ngx_file_info_t *fi,
    ngx_log_t *ಲಘುಗಣಕ);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ngx_fd_t ngx_open_file_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *ದ, ngx_int_t ಬಹುಲಕ, ngx_int_t create,
    ngx_int_t access, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ngx_int_t ngx_file_info_wrapper(ngx_str_t *name,
    ngx_open_file_info_t *ದ, ngx_file_info_t *fi, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ngx_int_t ngx_open_and_stat_file(ngx_str_t *name,
    ngx_open_file_info_t *ದ, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_ಕೂಡಿಸು_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *ದ, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ಬಳಸುತ್ತದೆ, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_del_event(ngx_cached_open_file_t *file);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_expire_old_cached_files(ngx_open_file_cache_t *cache,
    ngx_uint_t n, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ಸ್ಥಿರ ngx_cached_open_file_t *
    ngx_open_file_lookup(ngx_open_file_cache_t *cache, ngx_str_t *name,
    uint32_t hash);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_open_file_cache_remove(ngx_event_t *ev);


ngx_open_file_cache_t *
ngx_open_file_cache_init(ngx_pool_t *pool, ngx_uint_t max, ಸಮಯ_t inactive)
{
    ngx_pool_cleanup_t     *cln;
    ngx_open_file_cache_t  *cache;

    cache = ngx_palloc(pool, ಗಾತ್ರ(ngx_open_file_cache_t));
    ಆದರೆ (cache == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_rbtree_init(&cache->rbtree, &cache->sentinel,
                    ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ngx_ಸರತಿ_init(&cache->expire_ಸರತಿ);

    cache->current = 0;
    cache->max = max;
    cache->inactive = inactive;

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(pool, 0);
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cln->handler = ngx_open_file_cache_cleanup;
    cln->ದತ್ತಾಂಶ = cache;

    ಹಿಂತಿರುಗಿ cache;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_open_file_cache_t  *cache = ದತ್ತಾಂಶ;

    ngx_ಸರತಿ_t             *q;
    ngx_cached_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                   "open file cache cleanup");

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (ngx_ಸರತಿ_ಖಾಲಿಗಳು(&cache->expire_ಸರತಿ)) {
            ನಿಲ್ಲಿಸು;
        }

        q = ngx_ಸರತಿ_ಕೊನೆಯ(&cache->expire_ಸರತಿ);

        file = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_cached_open_file_t, ಸರತಿ);

        ngx_ಸರತಿ_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                       "delete cached open file: %s", file->name);

        ಆದರೆ (!file->err && !file->is_dir) {
            file->close = 1;
            file->ಎಣಿಕೆ_2 = 0;
            ngx_close_cached_file(cache, file, 0, ngx_ಚಕ್ರ->ಲಘುಗಣಕ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }

    ಆದರೆ (cache->current) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "%ui items still left in open file cache",
                      cache->current);
    }

    ಆದರೆ (cache->rbtree.ಬೇರು != cache->rbtree.sentinel) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "rbtree still is not empty in open file cache");

    }
}


ngx_int_t
ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *ದ, ngx_pool_t *pool)
{
    ಸಮಯ_t                          now;
    uint32_t                        hash;
    ngx_int_t                       rc;
    ngx_file_info_t                 fi;
    ngx_pool_cleanup_t             *cln;
    ngx_cached_open_file_t         *file;
    ngx_pool_cleanup_file_t        *clnf;
    ngx_open_file_cache_cleanup_t  *ofcln;

    ದ->fd = NGX_INVALID_FILE;
    ದ->err = 0;

    ಆದರೆ (cache == ಖಾಲಿ) {

        ಆದರೆ (ದ->test_only) {

            ಆದರೆ (ngx_file_info_wrapper(name, ದ, &fi, pool->ಲಘುಗಣಕ)
                == NGX_FILE_ERROR)
            {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ದ->uniq = ngx_file_uniq(&fi);
            ದ->mtime = ngx_file_mtime(&fi);
            ದ->ಗಾತ್ರ_2 = ngx_file_ಗಾತ್ರ(&fi);
            ದ->fs_ಗಾತ್ರ = ngx_file_fs_ಗಾತ್ರ(&fi);
            ದ->is_dir = ngx_is_dir(&fi);
            ದ->is_file = ngx_is_file(&fi);
            ದ->is_link = ngx_is_link(&fi);
            ದ->is_exec = ngx_is_exec(&fi);

            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        cln = ngx_pool_cleanup_ಕೂಡಿಸು(pool, ಗಾತ್ರ(ngx_pool_cleanup_file_t));
        ಆದರೆ (cln == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        rc = ngx_open_and_stat_file(name, ದ, pool->ಲಘುಗಣಕ);

        ಆದರೆ (rc == NGX_OK && !ದ->is_dir) {
            cln->handler = ngx_pool_cleanup_file;
            clnf = cln->ದತ್ತಾಂಶ;

            clnf->fd = ದ->fd;
            clnf->name = name->ದತ್ತಾಂಶ;
            clnf->ಲಘುಗಣಕ = pool->ಲಘುಗಣಕ;
        }

        ಹಿಂತಿರುಗಿ rc;
    }

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(pool, ಗಾತ್ರ(ngx_open_file_cache_cleanup_t));
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    now = ngx_ಸಮಯ();

    hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_long(name->ದತ್ತಾಂಶ, name->len);

    file = ngx_open_file_lookup(cache, name, hash);

    ಆದರೆ (file) {

        file->ಬಳಸುತ್ತದೆ++;

        ngx_ಸರತಿ_remove(&file->ಸರತಿ);

        ಆದರೆ (file->fd == NGX_INVALID_FILE && file->err == 0 && !file->is_dir) {

            /* file was not used often enough to keep open */

            rc = ngx_open_and_stat_file(name, ದ, pool->ಲಘುಗಣಕ);

            ಆದರೆ (rc != NGX_OK && (ದ->err == 0 || !ದ->errors)) {
                ಹೋಗು failed;
            }

            ಹೋಗು ಕೂಡಿಸು_event;
        }

        ಆದರೆ (file->use_event
            || (file->event == ಖಾಲಿ
                && (ದ->uniq == 0 || ದ->uniq == file->uniq)
                && now - file->created < ದ->valid
#ಆದರೆ (NGX_HAVE_OPENAT)
                && ದ->disable_symlinks == file->disable_symlinks
                && ದ->disable_symlinks_from == file->disable_symlinks_from
#ಆದರೆ_ಅಂತ್ಯ
            ))
        {
            ಆದರೆ (file->err == 0) {

                ದ->fd = file->fd;
                ದ->uniq = file->uniq;
                ದ->mtime = file->mtime;
                ದ->ಗಾತ್ರ_2 = file->ಗಾತ್ರ_2;

                ದ->is_dir = file->is_dir;
                ದ->is_file = file->is_file;
                ದ->is_link = file->is_link;
                ದ->is_exec = file->is_exec;
                ದ->is_directio = file->is_directio;

                ಆದರೆ (!file->is_dir) {
                    file->ಎಣಿಕೆ_2++;
                    ngx_open_file_ಕೂಡಿಸು_event(cache, file, ದ, pool->ಲಘುಗಣಕ);
                }

            } ಇಲ್ಲದಿದ್ದರೆ {
                ದ->err = file->err;
#ಆದರೆ (NGX_HAVE_OPENAT)
                ದ->failed = file->disable_symlinks ? ngx_openat_file_n
                                                    : ngx_open_file_n;
#ಇಲ್ಲದಿದ್ದರೆ
                ದ->failed = ngx_open_file_n;
#ಆದರೆ_ಅಂತ್ಯ
            }

            ಹೋಗು ಸಿಕ್ಕಿತು;
        }

        ngx_log_debug4(NGX_LOG_DEBUG_CORE, pool->ಲಘುಗಣಕ, 0,
                       "retest open file: %s, fd:%d, c:%d, e:%d",
                       file->name, file->fd, file->ಎಣಿಕೆ_2, file->err);

        ಆದರೆ (file->is_dir) {

            /*
             * chances that directory became file are very small
             * so test_dir flag allows to use a single syscall
             * in ngx_file_info() instead of three syscalls
             */

            ದ->test_dir = 1;
        }

        ದ->fd = file->fd;
        ದ->uniq = file->uniq;

        rc = ngx_open_and_stat_file(name, ದ, pool->ಲಘುಗಣಕ);

        ಆದರೆ (rc != NGX_OK && (ದ->err == 0 || !ದ->errors)) {
            ಹೋಗು failed;
        }

        ಆದರೆ (ದ->is_dir) {

            ಆದರೆ (file->is_dir || file->err) {
                ಹೋಗು update;
            }

            /* file became directory */

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದ->err == 0) {  /* file */

            ಆದರೆ (file->is_dir || file->err) {
                ಹೋಗು ಕೂಡಿಸು_event;
            }

            ಆದರೆ (ದ->uniq == file->uniq) {

                ಆದರೆ (file->event) {
                    file->use_event = 1;
                }

                ದ->is_directio = file->is_directio;

                ಹೋಗು update;
            }

            /* file was changed */

        } ಇಲ್ಲದಿದ್ದರೆ { /* error to cache */

            ಆದರೆ (file->err || file->is_dir) {
                ಹೋಗು update;
            }

            /* file was removed, etc. */
        }

        ಆದರೆ (file->ಎಣಿಕೆ_2 == 0) {

            ngx_open_file_del_event(file);

            ಆದರೆ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, pool->ಲಘುಗಣಕ, ngx_errno,
                              ngx_close_file_n " \"%V\" failed", name);
            }

            ಹೋಗು ಕೂಡಿಸು_event;
        }

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        file->close = 1;

        ಹೋಗು create;
    }

    /* not found */

    rc = ngx_open_and_stat_file(name, ದ, pool->ಲಘುಗಣಕ);

    ಆದರೆ (rc != NGX_OK && (ದ->err == 0 || !ದ->errors)) {
        ಹೋಗು failed;
    }

create:

    ಆದರೆ (cache->current >= cache->max) {
        ngx_expire_old_cached_files(cache, 0, pool->ಲಘುಗಣಕ);
    }

    file = ngx_alloc(ಗಾತ್ರ(ngx_cached_open_file_t), pool->ಲಘುಗಣಕ);

    ಆದರೆ (file == ಖಾಲಿ) {
        ಹೋಗು failed;
    }

    file->name = ngx_alloc(name->len + 1, pool->ಲಘುಗಣಕ);

    ಆದರೆ (file->name == ಖಾಲಿ) {
        ngx_free(file);
        file = ಖಾಲಿ;
        ಹೋಗು failed;
    }

    ngx_cpystrn(file->name, name->ದತ್ತಾಂಶ, name->len + 1);

    file->ಗಂಟು.ಕೀಲಿ = hash;

    ngx_rbtree_ಸೇರಿಸು(&cache->rbtree, &file->ಗಂಟು);

    cache->current++;

    file->ಬಳಸುತ್ತದೆ = 1;
    file->ಎಣಿಕೆ_2 = 0;
    file->use_event = 0;
    file->event = ಖಾಲಿ;

ಕೂಡಿಸು_event:

    ngx_open_file_ಕೂಡಿಸು_event(cache, file, ದ, pool->ಲಘುಗಣಕ);

update:

    file->fd = ದ->fd;
    file->err = ದ->err;
#ಆದರೆ (NGX_HAVE_OPENAT)
    file->disable_symlinks = ದ->disable_symlinks;
    file->disable_symlinks_from = ದ->disable_symlinks_from;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ದ->err == 0) {
        file->uniq = ದ->uniq;
        file->mtime = ದ->mtime;
        file->ಗಾತ್ರ_2 = ದ->ಗಾತ್ರ_2;

        file->close = 0;

        file->is_dir = ದ->is_dir;
        file->is_file = ದ->is_file;
        file->is_link = ದ->is_link;
        file->is_exec = ದ->is_exec;
        file->is_directio = ದ->is_directio;

        ಆದರೆ (!ದ->is_dir) {
            file->ಎಣಿಕೆ_2++;
        }
    }

    file->created = now;

ಸಿಕ್ಕಿತು:

    file->accessed = now;

    ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&cache->expire_ಸರತಿ, &file->ಸರತಿ);

    ngx_log_debug5(NGX_LOG_DEBUG_CORE, pool->ಲಘುಗಣಕ, 0,
                   "cached open file: %s, fd:%d, c:%d, e:%d, u:%d",
                   file->name, file->fd, file->ಎಣಿಕೆ_2, file->err, file->ಬಳಸುತ್ತದೆ);

    ಆದರೆ (ದ->err == 0) {

        ಆದರೆ (!ದ->is_dir) {
            cln->handler = ngx_open_file_cleanup;
            ofcln = cln->ದತ್ತಾಂಶ;

            ofcln->cache = cache;
            ofcln->file = file;
            ofcln->min_ಬಳಸುತ್ತದೆ = ದ->min_ಬಳಸುತ್ತದೆ;
            ofcln->ಲಘುಗಣಕ = pool->ಲಘುಗಣಕ;
        }

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;

failed:

    ಆದರೆ (file) {
        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ಆದರೆ (file->ಎಣಿಕೆ_2 == 0) {

            ಆದರೆ (file->fd != NGX_INVALID_FILE) {
                ಆದರೆ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_ALERT, pool->ಲಘುಗಣಕ, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file->name);
                }
            }

            ngx_free(file->name);
            ngx_free(file);

        } ಇಲ್ಲದಿದ್ದರೆ {
            file->close = 1;
        }
    }

    ಆದರೆ (ದ->fd != NGX_INVALID_FILE) {
        ಆದರೆ (ngx_close_file(ದ->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, pool->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


#ಆದರೆ (NGX_HAVE_OPENAT)

ಸ್ಥಿರ ngx_fd_t
ngx_openat_file_owner(ngx_fd_t at_fd, ಸ್ಥಿರಾಂಕ u_char *name,
    ngx_int_t ಬಹುಲಕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ಲಘುಗಣಕ)
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

    fd = ngx_openat_file(at_fd, name, ಬಹುಲಕ, create, access);

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
    }

    ಆದರೆ (ngx_file_at_info(at_fd, name, &atfi, AT_SYMLINK_NOFOLLOW)
        == NGX_FILE_ERROR)
    {
        err = ngx_errno;
        ಹೋಗು failed;
    }

#ಆದರೆ (NGX_HAVE_O_ಮಾರ್ಗ)
    ಆದರೆ (ngx_file_o_ಮಾರ್ಗ_info(fd, &fi, ಲಘುಗಣಕ) == NGX_ERROR) {
        err = ngx_errno;
        ಹೋಗು failed;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        err = ngx_errno;
        ಹೋಗು failed;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (fi.st_uid != atfi.st_uid) {
        err = NGX_ELOOP;
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ fd;

failed:

    ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", name);
    }

    ngx_ಗಣ_errno(err);

    ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
}


#ಆದರೆ (NGX_HAVE_O_ಮಾರ್ಗ)

ಸ್ಥಿರ ngx_int_t
ngx_file_o_ಮಾರ್ಗ_info(ngx_fd_t fd, ngx_file_info_t *fi, ngx_log_t *ಲಘುಗಣಕ)
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

    ಆದರೆ (use_fstat) {
        ಆದರೆ (ngx_fd_info(fd, fi) != NGX_FILE_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ಆದರೆ (ngx_errno != NGX_EBADF) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_log_error(NGX_LOG_NOTICE, ಲಘುಗಣಕ, 0,
                      "fstat(O_PATH) failed with EBADF, "
                      "switching to fstatat(AT_EMPTY_PATH)");

        use_fstat = 0;
    }

    ಆದರೆ (ngx_file_at_info(fd, "", fi, AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ) != NGX_FILE_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* NGX_HAVE_OPENAT */


ಸ್ಥಿರ ngx_fd_t
ngx_open_file_wrapper(ngx_str_t *name, ngx_open_file_info_t *ದ,
    ngx_int_t ಬಹುಲಕ, ngx_int_t create, ngx_int_t access, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_fd_t  fd;

#ಆದರೆ !(NGX_HAVE_OPENAT)

    fd = ngx_open_file(name->ದತ್ತಾಂಶ, ಬಹುಲಕ, create, access);

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ದ->err = ngx_errno;
        ದ->failed = ngx_open_file_n;
        ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
    }

    ಹಿಂತಿರುಗಿ fd;

#ಇಲ್ಲದಿದ್ದರೆ

    u_char           *p, *cp, *ಕೊನೆ;
    ngx_fd_t          at_fd;
    ngx_str_t         at_name;

    ಆದರೆ (ದ->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {
        fd = ngx_open_file(name->ದತ್ತಾಂಶ, ಬಹುಲಕ, create, access);

        ಆದರೆ (fd == NGX_INVALID_FILE) {
            ದ->err = ngx_errno;
            ದ->failed = ngx_open_file_n;
            ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
        }

        ಹಿಂತಿರುಗಿ fd;
    }

    p = name->ದತ್ತಾಂಶ;
    ಕೊನೆ = p + name->len;

    at_name = *name;

    ಆದರೆ (ದ->disable_symlinks_from) {

        cp = p + ದ->disable_symlinks_from;

        *cp = '\0';

        at_fd = ngx_open_file(p, NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        *cp = '/';

        ಆದರೆ (at_fd == NGX_INVALID_FILE) {
            ದ->err = ngx_errno;
            ದ->failed = ngx_open_file_n;
            ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
        }

        at_name.len = ದ->disable_symlinks_from;
        p = cp + 1;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '/') {

        at_fd = ngx_open_file("/",
                              NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                              NGX_FILE_OPEN, 0);

        ಆದರೆ (at_fd == NGX_INVALID_FILE) {
            ದ->err = ngx_errno;
            ದ->failed = ngx_openat_file_n;
            ಹಿಂತಿರುಗಿ NGX_INVALID_FILE;
        }

        at_name.len = 1;
        p++;

    } ಇಲ್ಲದಿದ್ದರೆ {
        at_fd = NGX_AT_FDCWD;
    }

    ಗಾಗಿ ( ;; ) {
        cp = ngx_strlchr(p, ಕೊನೆ, '/');
        ಆದರೆ (cp == ಖಾಲಿ) {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (cp == p) {
            p++;
            ಮುಂದುವರಿ;
        }

        *cp = '\0';

        ಆದರೆ (ದ->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER) {
            fd = ngx_openat_file_owner(at_fd, p,
                                       NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK,
                                       NGX_FILE_OPEN, 0, ಲಘುಗಣಕ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            fd = ngx_openat_file(at_fd, p,
                           NGX_FILE_ಹುಡುಕು|NGX_FILE_NONBLOCK|NGX_FILE_NOFOLLOW,
                           NGX_FILE_OPEN, 0);
        }

        *cp = '/';

        ಆದರೆ (fd == NGX_INVALID_FILE) {
            ದ->err = ngx_errno;
            ದ->failed = ngx_openat_file_n;
            ಹೋಗು failed;
        }

        ಆದರೆ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", &at_name);
        }

        p = cp + 1;
        at_fd = fd;
        at_name.len = cp - at_name.ದತ್ತಾಂಶ;
    }

    ಆದರೆ (p == ಕೊನೆ) {

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

        fd = ngx_openat_file(at_fd, ".", ಬಹುಲಕ, create, access);
        ಹೋಗು ಮುಗಿದ;
    }

    ಆದರೆ (ದ->disable_symlinks == NGX_DISABLE_SYMLINKS_NOTOWNER
        && !(create & (NGX_FILE_CREATE_OR_OPEN|NGX_FILE_TRUNCATE)))
    {
        fd = ngx_openat_file_owner(at_fd, p, ಬಹುಲಕ, create, access, ಲಘುಗಣಕ);

    } ಇಲ್ಲದಿದ್ದರೆ {
        fd = ngx_openat_file(at_fd, p, ಬಹುಲಕ|NGX_FILE_NOFOLLOW, create, access);
    }

ಮುಗಿದ:

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ದ->err = ngx_errno;
        ದ->failed = ngx_openat_file_n;
    }

failed:

    ಆದರೆ (at_fd != NGX_AT_FDCWD && ngx_close_file(at_fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", &at_name);
    }

    ಹಿಂತಿರುಗಿ fd;
#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_file_info_wrapper(ngx_str_t *name, ngx_open_file_info_t *ದ,
    ngx_file_info_t *fi, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_int_t  rc;

#ಆದರೆ !(NGX_HAVE_OPENAT)

    rc = ngx_file_info(name->ದತ್ತಾಂಶ, fi);

    ಆದರೆ (rc == NGX_FILE_ERROR) {
        ದ->err = ngx_errno;
        ದ->failed = ngx_file_info_n;
        ಹಿಂತಿರುಗಿ NGX_FILE_ERROR;
    }

    ಹಿಂತಿರುಗಿ rc;

#ಇಲ್ಲದಿದ್ದರೆ

    ngx_fd_t  fd;

    ಆದರೆ (ದ->disable_symlinks == NGX_DISABLE_SYMLINKS_OFF) {

        rc = ngx_file_info(name->ದತ್ತಾಂಶ, fi);

        ಆದರೆ (rc == NGX_FILE_ERROR) {
            ದ->err = ngx_errno;
            ದ->failed = ngx_file_info_n;
            ಹಿಂತಿರುಗಿ NGX_FILE_ERROR;
        }

        ಹಿಂತಿರುಗಿ rc;
    }

    fd = ngx_open_file_wrapper(name, ದ, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                               NGX_FILE_OPEN, 0, ಲಘುಗಣಕ);

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ಹಿಂತಿರುಗಿ NGX_FILE_ERROR;
    }

    rc = ngx_fd_info(fd, fi);

    ಆದರೆ (rc == NGX_FILE_ERROR) {
        ದ->err = ngx_errno;
        ದ->failed = ngx_fd_info_n;
    }

    ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%V\" failed", name);
    }

    ಹಿಂತಿರುಗಿ rc;
#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_open_and_stat_file(ngx_str_t *name, ngx_open_file_info_t *ದ,
    ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_fd_t         fd;
    ngx_file_info_t  fi;

    ಆದರೆ (ದ->fd != NGX_INVALID_FILE) {

        ಆದರೆ (ngx_file_info_wrapper(name, ದ, &fi, ಲಘುಗಣಕ) == NGX_FILE_ERROR) {
            ದ->fd = NGX_INVALID_FILE;
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ದ->uniq == ngx_file_uniq(&fi)) {
            ಹೋಗು ಮುಗಿದ;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದ->test_dir) {

        ಆದರೆ (ngx_file_info_wrapper(name, ದ, &fi, ಲಘುಗಣಕ) == NGX_FILE_ERROR) {
            ದ->fd = NGX_INVALID_FILE;
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ngx_is_dir(&fi)) {
            ಹೋಗು ಮುಗಿದ;
        }
    }

    ಆದರೆ (!ದ->ಲಘುಗಣಕ) {

        /*
         * Use non-blocking open() not to hang on FIFO files, etc.
         * This flag has no effect on a regular files.
         */

        fd = ngx_open_file_wrapper(name, ದ, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK,
                                   NGX_FILE_OPEN, 0, ಲಘುಗಣಕ);

    } ಇಲ್ಲದಿದ್ದರೆ {
        fd = ngx_open_file_wrapper(name, ದ, NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS, ಲಘುಗಣಕ);
    }

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ದ->fd = NGX_INVALID_FILE;
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_fd_info_n " \"%V\" failed", name);

        ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        ದ->fd = NGX_INVALID_FILE;

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_is_dir(&fi)) {
        ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%V\" failed", name);
        }

        ದ->fd = NGX_INVALID_FILE;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ದ->fd = fd;

        ಆದರೆ (ದ->read_ahead && ngx_file_ಗಾತ್ರ(&fi) > NGX_MIN_READ_AHEAD) {
            ಆದರೆ (ngx_read_ahead(fd, ದ->read_ahead) == NGX_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                              ngx_read_ahead_n " \"%V\" failed", name);
            }
        }

        ಆದರೆ (ದ->directio <= ngx_file_ಗಾತ್ರ(&fi)) {
            ಆದರೆ (ngx_directio_on(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                              ngx_directio_on_n " \"%V\" failed", name);

            } ಇಲ್ಲದಿದ್ದರೆ {
                ದ->is_directio = 1;
            }
        }
    }

ಮುಗಿದ:

    ದ->uniq = ngx_file_uniq(&fi);
    ದ->mtime = ngx_file_mtime(&fi);
    ದ->ಗಾತ್ರ_2 = ngx_file_ಗಾತ್ರ(&fi);
    ದ->fs_ಗಾತ್ರ = ngx_file_fs_ಗಾತ್ರ(&fi);
    ದ->is_dir = ngx_is_dir(&fi);
    ದ->is_file = ngx_is_file(&fi);
    ದ->is_link = ngx_is_link(&fi);
    ದ->is_exec = ngx_is_exec(&fi);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


/*
 * we ignore any possible event setting error and
 * fallback to usual periodic file retests
 */

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_ಕೂಡಿಸು_event(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_open_file_info_t *ದ, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_open_file_cache_event_t  *fev;

    ಆದರೆ (!(ngx_event_flags & NGX_USE_VNODE_EVENT)
        || !ದ->events
        || file->event
        || ದ->fd == NGX_INVALID_FILE
        || file->ಬಳಸುತ್ತದೆ < ದ->min_ಬಳಸುತ್ತದೆ)
    {
        ಹಿಂತಿರುಗಿ;
    }

    file->use_event = 0;

    file->event = ngx_calloc(ಗಾತ್ರ(ngx_event_t), ಲಘುಗಣಕ);
    ಆದರೆ (file->event== ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    fev = ngx_alloc(ಗಾತ್ರ(ngx_open_file_cache_event_t), ಲಘುಗಣಕ);
    ಆದರೆ (fev == ಖಾಲಿ) {
        ngx_free(file->event);
        file->event = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    fev->fd = ದ->fd;
    fev->file = file;
    fev->cache = cache;

    file->event->handler = ngx_open_file_cache_remove;
    file->event->ದತ್ತಾಂಶ = fev;

    /*
     * although vnode event may be called while ngx_cycle->poll
     * destruction, however, cleanup procedures are run before any
     * memory freeing and events will be canceled.
     */

    file->event->ಲಘುಗಣಕ = ngx_ಚಕ್ರ->ಲಘುಗಣಕ;

    ಆದರೆ (ngx_ಕೂಡಿಸು_event(file->event, NGX_VNODE_EVENT, NGX_ONESHOT_EVENT)
        != NGX_OK)
    {
        ngx_free(file->event->ದತ್ತಾಂಶ);
        ngx_free(file->event);
        file->event = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    /*
     * we do not set file->use_event here because there may be a race
     * condition: a file may be deleted between opening the file and
     * adding event, so we rely upon event notification only after
     * one file revalidation on next file access
     */

    ಹಿಂತಿರುಗಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_open_file_cache_cleanup_t  *c = ದತ್ತಾಂಶ;

    c->file->ಎಣಿಕೆ_2--;

    ngx_close_cached_file(c->cache, c->file, c->min_ಬಳಸುತ್ತದೆ, c->ಲಘುಗಣಕ);

    /* drop one or two expired open files */
    ngx_expire_old_cached_files(c->cache, 1, c->ಲಘುಗಣಕ);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_close_cached_file(ngx_open_file_cache_t *cache,
    ngx_cached_open_file_t *file, ngx_uint_t min_ಬಳಸುತ್ತದೆ, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_log_debug5(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0,
                   "close cached open file: %s, fd:%d, c:%d, u:%d, %d",
                   file->name, file->fd, file->ಎಣಿಕೆ_2, file->ಬಳಸುತ್ತದೆ, file->close);

    ಆದರೆ (!file->close) {

        file->accessed = ngx_ಸಮಯ();

        ngx_ಸರತಿ_remove(&file->ಸರತಿ);

        ngx_ಸರತಿ_ಸೇರಿಸು_ತಲೆ(&cache->expire_ಸರತಿ, &file->ಸರತಿ);

        ಆದರೆ (file->ಬಳಸುತ್ತದೆ >= min_ಬಳಸುತ್ತದೆ || file->ಎಣಿಕೆ_2) {
            ಹಿಂತಿರುಗಿ;
        }
    }

    ngx_open_file_del_event(file);

    ಆದರೆ (file->ಎಣಿಕೆ_2) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (file->fd != NGX_INVALID_FILE) {

        ಆದರೆ (ngx_close_file(file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", file->name);
        }

        file->fd = NGX_INVALID_FILE;
    }

    ಆದರೆ (!file->close) {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_free(file->name);
    ngx_free(file);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_del_event(ngx_cached_open_file_t *file)
{
    ಆದರೆ (file->event == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    (ಶೂನ್ಯ) ngx_del_event(file->event, NGX_VNODE_EVENT,
                         file->ಎಣಿಕೆ_2 ? NGX_FLUSH_EVENT : NGX_CLOSE_EVENT);

    ngx_free(file->event->ದತ್ತಾಂಶ);
    ngx_free(file->event);
    file->event = ಖಾಲಿ;
    file->use_event = 0;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_expire_old_cached_files(ngx_open_file_cache_t *cache, ngx_uint_t n,
    ngx_log_t *ಲಘುಗಣಕ)
{
    ಸಮಯ_t                   now;
    ngx_ಸರತಿ_t             *q;
    ngx_cached_open_file_t  *file;

    now = ngx_ಸಮಯ();

    /*
     * n == 1 deletes one or two inactive files
     * n == 0 deletes least recently used file by force
     *        and one or two inactive files
     */

    ಆಗ (n < 3) {

        ಆದರೆ (ngx_ಸರತಿ_ಖಾಲಿಗಳು(&cache->expire_ಸರತಿ)) {
            ಹಿಂತಿರುಗಿ;
        }

        q = ngx_ಸರತಿ_ಕೊನೆಯ(&cache->expire_ಸರತಿ);

        file = ngx_ಸರತಿ_ದತ್ತಾಂಶ(q, ngx_cached_open_file_t, ಸರತಿ);

        ಆದರೆ (n++ != 0 && now - file->accessed <= cache->inactive) {
            ಹಿಂತಿರುಗಿ;
        }

        ngx_ಸರತಿ_remove(q);

        ngx_rbtree_delete(&cache->rbtree, &file->ಗಂಟು);

        cache->current--;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0,
                       "expire cached open file: %s", file->name);

        ಆದರೆ (!file->err && !file->is_dir) {
            file->close = 1;
            ngx_close_cached_file(cache, file, 0, ಲಘುಗಣಕ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_free(file->name);
            ngx_free(file);
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t       **p;
    ngx_cached_open_file_t    *file, *file_ತಾತ್ಕಾಲಿಕ;

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (ಗಂಟು->ಕೀಲಿ < ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಎಡ;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->ಕೀಲಿ > ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        } ಇಲ್ಲದಿದ್ದರೆ { /* node->key == temp->key */

            file = (ngx_cached_open_file_t *) ಗಂಟು;
            file_ತಾತ್ಕಾಲಿಕ = (ngx_cached_open_file_t *) ತಾತ್ಕಾಲಿಕ;

            p = (ngx_strcmp(file->name, file_ತಾತ್ಕಾಲಿಕ->name) < 0)
                    ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;
        }

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
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

    ಆಗ (ಗಂಟು != sentinel) {

        ಆದರೆ (hash < ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (hash > ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        /* hash == node->key */

        file = (ngx_cached_open_file_t *) ಗಂಟು;

        rc = ngx_strcmp(name->ದತ್ತಾಂಶ, file->name);

        ಆದರೆ (rc == 0) {
            ಹಿಂತಿರುಗಿ file;
        }

        ಗಂಟು = (rc < 0) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_open_file_cache_remove(ngx_event_t *ev)
{
    ngx_cached_open_file_t       *file;
    ngx_open_file_cache_event_t  *fev;

    fev = ev->ದತ್ತಾಂಶ;
    file = fev->file;

    ngx_ಸರತಿ_remove(&file->ಸರತಿ);

    ngx_rbtree_delete(&fev->cache->rbtree, &file->ಗಂಟು);

    fev->cache->current--;

    /* NGX_ONESHOT_EVENT was already deleted */
    file->event = ಖಾಲಿ;
    file->use_event = 0;

    file->close = 1;

    ngx_close_cached_file(fev->cache, file, 0, ev->ಲಘುಗಣಕ);

    /* free memory only when fev->cache and fev->file are already not needed */

    ngx_free(ev->ದತ್ತಾಂಶ);
    ngx_free(ev);
}
