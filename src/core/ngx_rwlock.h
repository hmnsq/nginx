
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ശൂന്യം ngx_rwlock_wlock(ngx_atomic_t *lock);
ശൂന്യം ngx_rwlock_rlock(ngx_atomic_t *lock);
ശൂന്യം ngx_rwlock_unlock(ngx_atomic_t *lock);
ശൂന്യം ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
