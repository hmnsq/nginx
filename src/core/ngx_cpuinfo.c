
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#ఒకవేళ (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


స్థిర ngx_inline శూన్య ngx_cpuid(uint32_t i, uint32_t *buf);


#ఒకవేళ ( __i386__ )

స్థిర ngx_inline శూన్య
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


#లేకపోతే /* __amd64__ */


స్థిర ngx_inline శూన్య
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


#అయితే_ముగింపు


/* auto detect the L2 cache line size of modern and widespread CPUs */

శూన్య
ngx_cpuinfo(శూన్య)
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

    ఒకవేళ (vbuf[0] == 0) {
        తిరిగి;
    }

    ngx_cpuid(1, cpu);

    ఒకవేళ (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        ఎంపిక ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        స్థితి 5:
            ngx_cacheline_కొలత = 32;
            ఆపు;

        /* Pentium Pro, II, III */
        స్థితి 6:
            ngx_cacheline_కొలత = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            ఒకవేళ (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_కొలత = 64;
            }

            ఆపు;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        స్థితి 15:
            ngx_cacheline_కొలత = 128;
            ఆపు;
        }

    } లేకపోతే ఒకవేళ (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_కొలత = 64;
    }
}

#లేకపోతే


శూన్య
ngx_cpuinfo(శూన్య)
{
}


#అయితే_ముగింపు
