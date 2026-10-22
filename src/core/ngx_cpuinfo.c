
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ଯଦି (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ ngx_cpuid(uint32_t i, uint32_t *buf);


#ଯଦି ( __i386__ )

ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
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


#ଅନ୍ୟଥା /* __amd64__ */


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
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

ଶୂନ୍ୟ
ngx_cpuinfo(ଶୂନ୍ୟ)
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

    ଯଦି (vbuf[0] == 0) {
        ଫେରନ୍ତୁ;
    }

    ngx_cpuid(1, cpu);

    ଯଦି (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        ବାଛ ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        ସ୍ଥିତି 5:
            ngx_cacheline_size = 32;
            ଭାଙ୍ଗ;

        /* Pentium Pro, II, III */
        ସ୍ଥିତି 6:
            ngx_cacheline_size = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            ଯଦି (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_size = 64;
            }

            ଭାଙ୍ଗ;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        ସ୍ଥିତି 15:
            ngx_cacheline_size = 128;
            ଭାଙ୍ଗ;
        }

    } ଅନ୍ୟଥା ଯଦି (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_size = 64;
    }
}

#ଅନ୍ୟଥା


ଶୂନ୍ୟ
ngx_cpuinfo(ଶୂନ୍ୟ)
{
}


#endif
