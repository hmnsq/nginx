
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


#எனில் (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


நிலையான ngx_inline வெறுமை ngx_cpuid(uint32_t i, uint32_t *buf);


#எனில் ( __i386__ )

நிலையான ngx_inline வெறுமை
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


#இல்லையெனில் /* __amd64__ */


நிலையான ngx_inline வெறுமை
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

வெறுமை
ngx_cpuinfo(வெறுமை)
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

    எனில் (vbuf[0] == 0) {
        திரும்பு;
    }

    ngx_cpuid(1, cpu);

    எனில் (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        தேர்வு ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        நிலை 5:
            ngx_cacheline_size = 32;
            நிறுத்து;

        /* Pentium Pro, II, III */
        நிலை 6:
            ngx_cacheline_size = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            எனில் (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_size = 64;
            }

            நிறுத்து;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        நிலை 15:
            ngx_cacheline_size = 128;
            நிறுத்து;
        }

    } இல்லையெனில் எனில் (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_size = 64;
    }
}

#இல்லையெனில்


வெறுமை
ngx_cpuinfo(வெறுமை)
{
}


#endif
