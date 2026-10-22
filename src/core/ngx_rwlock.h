
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ଶୂନ୍ୟ ngx_rwlock_wlock(ngx_atomic_t *lock);
ଶୂନ୍ୟ ngx_rwlock_rlock(ngx_atomic_t *lock);
ଶୂନ୍ୟ ngx_rwlock_unlock(ngx_atomic_t *lock);
ଶୂନ୍ୟ ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
