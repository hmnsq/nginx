
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تهی ngx_rwlock_wlock(ngx_atomic_t *lock);
تهی ngx_rwlock_rlock(ngx_atomic_t *lock);
تهی ngx_rwlock_unlock(ngx_atomic_t *lock);
تهی ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
