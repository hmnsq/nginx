
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಆದರೆ (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_cpuid(uint32_t i, uint32_t *buf);


#ಆದರೆ ( __i386__ )

ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
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


#ಇಲ್ಲದಿದ್ದರೆ /* __amd64__ */


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
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


#ಆದರೆ_ಅಂತ್ಯ


/* auto detect the L2 cache line size of modern and widespread CPUs */

ಶೂನ್ಯ
ngx_cpuinfo(ಶೂನ್ಯ)
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

    ಆದರೆ (vbuf[0] == 0) {
        ಹಿಂತಿರುಗಿ;
    }

    ngx_cpuid(1, cpu);

    ಆದರೆ (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        ಆಯ್ಕೆ ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        ಸ್ಥಿತಿ 5:
            ngx_cacheline_ಗಾತ್ರ = 32;
            ನಿಲ್ಲಿಸು;

        /* Pentium Pro, II, III */
        ಸ್ಥಿತಿ 6:
            ngx_cacheline_ಗಾತ್ರ = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            ಆದರೆ (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_ಗಾತ್ರ = 64;
            }

            ನಿಲ್ಲಿಸು;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        ಸ್ಥಿತಿ 15:
            ngx_cacheline_ಗಾತ್ರ = 128;
            ನಿಲ್ಲಿಸು;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_ಗಾತ್ರ = 64;
    }
}

#ಇಲ್ಲದಿದ್ದರೆ


ಶೂನ್ಯ
ngx_cpuinfo(ಶೂನ್ಯ)
{
}


#ಆದರೆ_ಅಂತ್ಯ
