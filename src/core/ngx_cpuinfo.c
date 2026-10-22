
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#जर (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


स्थिर ngx_inline शून्य ngx_cpuid(uint32_t i, uint32_t *buf);


#जर ( __i386__ )

स्थिर ngx_inline शून्य
ngx_cpuid(uint32_t i, uint32_t *buf)
{

    /*
     * we could not use %ebx as output parameter if gcc builds PIC,
     * and we could not save %ebx on stack, because %esp is used,
     * when the -fomit-frame-pointer optimization is specified.
     */

    __asm__ (

    "    mov    %%ebx, %%esi;  "

    "    cpuid;                "
    "    mov    %%eax, (%1);   "
    "    mov    %%ebx, 4(%1);  "
    "    mov    %%edx, 8(%1);  "
    "    mov    %%ecx, 12(%1); "

    "    mov    %%esi, %%ebx;  "

    : : "a" (i), "D" (buf) : "ecx", "edx", "esi", "memory" );
}


#अन्यथा /* __amd64__ */


स्थिर ngx_inline शून्य
ngx_cpuid(uint32_t i, uint32_t *buf)
{
    uint32_t  eax, ebx, ecx, edx;

    __asm__ (

        "cpuid"

    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (i) );

    buf[0] = eax;
    buf[1] = ebx;
    buf[2] = edx;
    buf[3] = ecx;
}


#endif


/* auto detect the L2 cache line size of modern and widespread CPUs */

शून्य
ngx_cpuinfo(शून्य)
{
    u_char    *vendor;
    uint32_t   vbuf[5], cpu[4], model;

    vbuf[0] = 0;
    vbuf[1] = 0;
    vbuf[2] = 0;
    vbuf[3] = 0;
    vbuf[4] = 0;

    ngx_cpuid(0, vbuf);

    vendor = (u_char *) &vbuf[1];

    जर (vbuf[0] == 0) {
        परत;
    }

    ngx_cpuid(1, cpu);

    जर (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        निवड ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        स्थिती 5:
            ngx_cacheline_size = 32;
            थांबा;

        /* Pentium Pro, II, III */
        स्थिती 6:
            ngx_cacheline_size = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            जर (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_size = 64;
            }

            थांबा;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        स्थिती 15:
            ngx_cacheline_size = 128;
            थांबा;
        }

    } अन्यथा जर (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_size = 64;
    }
}

#अन्यथा


शून्य
ngx_cpuinfo(शून्य)
{
}


#endif
