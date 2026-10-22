
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


ساكن ngx_inline فراغ ngx_cpuid(uint32_t i, uint32_t *buf);


#إذا ( __i386__ )

ساكن ngx_inline فراغ
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


#وإلا /* __amd64__ */


ساكن ngx_inline فراغ
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

فراغ
ngx_cpuinfo(فراغ)
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

    إذا (vbuf[0] == 0) {
        أرجع;
    }

    ngx_cpuid(1, cpu);

    إذا (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        بدل ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        حالة 5:
            ngx_cacheline_size = 32;
            اكسر;

        /* Pentium Pro, II, III */
        حالة 6:
            ngx_cacheline_size = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            إذا (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_size = 64;
            }

            اكسر;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        حالة 15:
            ngx_cacheline_size = 128;
            اكسر;
        }

    } وإلا إذا (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_size = 64;
    }
}

#وإلا


فراغ
ngx_cpuinfo(فراغ)
{
}


#endif
