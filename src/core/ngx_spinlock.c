
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य
ngx_spinlock(ngx_atomic_t *तालकम्, ngx_atomic_int_t मानम्, ngx_uint_t spin)
{

#यदि (NGX_HAVE_ATOMIC_OPS)

    ngx_uint_t  i, n;

    प्रति ( ;; ) {

        यदि (*तालकम् == 0 && ngx_atomic_cmp_समुच्चयः(तालकम्, 0, मानम्)) {
            प्रत्यागम;
        }

        यदि (ngx_ncpu > 1) {

            प्रति (n = 1; n < spin; n <<= 1) {

                प्रति (i = 0; i < n; i++) {
                    ngx_cpu_pause();
                }

                यदि (*तालकम् == 0 && ngx_atomic_cmp_समुच्चयः(तालकम्, 0, मानम्)) {
                    प्रत्यागम;
                }
            }
        }

        ngx_sched_yield();
    }

#अन्यथा

#यदि (NGX_तन्तवः)

#दोषः ngx_spinlock() or ngx_atomic_cmp_समुच्चयः() are not defined !

#यदि_अन्तः

#यदि_अन्तः

}
