
/*
 * Copyright (C) Ruslan Ermilov
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_RWLOCK_H_INCLUDED_
#నిర్వచించు _NGX_RWLOCK_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


శూన్య ngx_rwlock_wlock(ngx_atomic_t *తాళం);
శూన్య ngx_rwlock_rlock(ngx_atomic_t *తాళం);
శూన్య ngx_rwlock_unlock(ngx_atomic_t *తాళం);
శూన్య ngx_rwlock_downgrade(ngx_atomic_t *తాళం);


#అయితే_ముగింపు /* _NGX_RWLOCK_H_INCLUDED_ */
