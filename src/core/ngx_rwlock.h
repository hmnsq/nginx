
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_RWLOCK_H_INCLUDED_
#تعریف_کن _NGX_RWLOCK_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ ngx_rwlock_wlock(ngx_atomic_t *قفل);
فراغ ngx_rwlock_rlock(ngx_atomic_t *قفل);
فراغ ngx_rwlock_unlock(ngx_atomic_t *قفل);
فراغ ngx_rwlock_downgrade(ngx_atomic_t *قفل);


#پایان_اگر /* _NGX_RWLOCK_H_INCLUDED_ */
