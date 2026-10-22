
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_RWLOCK_H_INCLUDED_
#घोषणा _NGX_RWLOCK_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


व्योम ngx_rwlock_wlock(ngx_atomic_t *lock);
व्योम ngx_rwlock_rlock(ngx_atomic_t *lock);
व्योम ngx_rwlock_unlock(ngx_atomic_t *lock);
व्योम ngx_rwlock_downgrade(ngx_atomic_t *lock);


#पूर्ण_अगर /* _NGX_RWLOCK_H_INCLUDED_ */
