
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر (( __i386__ || __amd64__ ) && ( __GNUC__ || __INTEL_COMPILER ))


ساکن ngx_inline خالی ngx_cpuid(uint32_t i, uint32_t *buf);


#اگر ( __i386__ )

ساکن ngx_inline خالی
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


#ورنہ /* __amd64__ */


ساکن ngx_inline خالی
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


#اگر_ختم


/* auto detect the L2 cache line size of modern and widespread CPUs */

خالی
ngx_cpuinfo(خالی)
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

    اگر (vbuf[0] == 0) {
        واپس;
    }

    ngx_cpuid(1, cpu);

    اگر (ngx_strcmp(vendor, "GenuineIntel") == 0) {

        انتخاب ((cpu[0] & 0xf00) >> 8) {

        /* Pentium */
        حالت 5:
            ngx_cacheline_سائز = 32;
            توڑو;

        /* Pentium Pro, II, III */
        حالت 6:
            ngx_cacheline_سائز = 32;

            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);

            اگر (model >= 0xd0) {
                /* Intel Core, Core 2, Atom */
                ngx_cacheline_سائز = 64;
            }

            توڑو;

        /*
         * Pentium 4, although its cache line size is 64 bytes,
         * it prefetches up to two cache lines during memory read
         */
        حالت 15:
            ngx_cacheline_سائز = 128;
            توڑو;
        }

    } ورنہ اگر (ngx_strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_سائز = 64;
    }
}

#ورنہ


خالی
ngx_cpuinfo(خالی)
{
}


#اگر_ختم
