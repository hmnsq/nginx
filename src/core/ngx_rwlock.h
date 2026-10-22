
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RWLOCK_H_INCLUDED_
#define _NGX_RWLOCK_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


શૂન્ય ngx_rwlock_wlock(ngx_atomic_t *lock);
શૂન્ય ngx_rwlock_rlock(ngx_atomic_t *lock);
શૂન્ય ngx_rwlock_unlock(ngx_atomic_t *lock);
શૂન્ય ngx_rwlock_downgrade(ngx_atomic_t *lock);


#endif /* _NGX_RWLOCK_H_INCLUDED_ */
