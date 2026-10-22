
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#যদি (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


স্থির ngx_inline শূন্য ngx_cpuid(uint32_t i, uint32_t *buf);


#যদি ( __i386__ )

স্থির ngx_inline শূন্য
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


#অন্যথা /* __amd64__ */


স্থির ngx_inline শূন্য
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

শূন্য
ngx_cpuinfo(শূন্য)
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

    যদি (vbuf[0] == 0) {
        ফেরত;
    }

    ngx_cpuid(1, cpu);

    যদি (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        নির্বাচন ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        ক্ষেত্র 5:
            ngx_cacheline_আকার = 32;
            বিরতি;

        /* Pentium Pro, II, III */
        ক্ষেত্র 6:
            ngx_cacheline_আকার = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            যদি (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_আকার = 64;
            }

            বিরতি;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        ক্ষেত্র 15:
            ngx_cacheline_আকার = 128;
            বিরতি;
        }

    } অন্যথা যদি (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_আকার = 64;
    }
}

#অন্যথা


শূন্য
ngx_cpuinfo(শূন্য)
{
}


#endif
