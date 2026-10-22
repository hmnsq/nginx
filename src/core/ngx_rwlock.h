
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ ngx_rwlock_wlock(ngx_atomic_t *lock);
ಶೂನ್ಯ ngx_rwlock_rlock(ngx_atomic_t *lock);
ಶೂನ್ಯ ngx_rwlock_unlock(ngx_atomic_t *lock);
ಶೂನ್ಯ ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
