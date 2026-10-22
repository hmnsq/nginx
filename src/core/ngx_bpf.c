
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>

#define NGX_BPF_LOGBUF_আকার  (32 * 1024)


স্থির ngx_inline পূর্ণসংখ্যা
ngx_bpf(গণনা bpf_cmd cmd, সংঘ bpf_attr *attr, unsigned পূর্ণসংখ্যা আকার_2)
{
    ফেরত syscall(__NR_bpf, cmd, attr, আকার_2);
}


শূন্য
ngx_bpf_program_link(ngx_bpf_program_t *program, ধ্রুবক অক্ষর *প্রতীক, পূর্ণসংখ্যা fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    জন্য (i = 0; i < program->nrelocs; i++) {
        যদি (ngx_strcmp(rl[i].name, প্রতীক) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


পূর্ণসংখ্যা
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    পূর্ণসংখ্যা             fd;
    সংঘ bpf_attr  attr;
#যদি (NGX_DEBUG)
    অক্ষর            buf[NGX_BPF_LOGBUF_আকার];
#endif

    ngx_memzero(&attr, আকার(সংঘ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#যদি (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_আকার = NGX_BPF_LOGBUF_আকার;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, আকার(attr));
    যদি (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ফেরত -1;
    }

    ফেরত fd;
}


পূর্ণসংখ্যা
ngx_bpf_map_create(ngx_log_t *log, গণনা bpf_map_type type, পূর্ণসংখ্যা চাবি_আকার,
    পূর্ণসংখ্যা মান_আকার, পূর্ণসংখ্যা max_entries, uint32_t map_flags)
{
    পূর্ণসংখ্যা             fd;
    সংঘ bpf_attr  attr;

    ngx_memzero(&attr, আকার(সংঘ bpf_attr));

    attr.map_type = type;
    attr.চাবি_আকার = চাবি_আকার;
    attr.মান_আকার = মান_আকার;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, আকার(attr));
    যদি (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ফেরত NGX_ERROR;
    }

    ফেরত fd;
}


পূর্ণসংখ্যা
ngx_bpf_map_update(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি, ধ্রুবক শূন্য *মান, uint64_t flags)
{
    সংঘ bpf_attr attr;

    ngx_memzero(&attr, আকার(সংঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;
    attr.মান = (uintptr_t) মান;
    attr.flags = flags;

    ফেরত ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, আকার(attr));
}


পূর্ণসংখ্যা
ngx_bpf_map_delete(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি)
{
    সংঘ bpf_attr attr;

    ngx_memzero(&attr, আকার(সংঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;

    ফেরত ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, আকার(attr));
}


পূর্ণসংখ্যা
ngx_bpf_map_lookup(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *চাবি, শূন্য *মান)
{
    সংঘ bpf_attr attr;

    ngx_memzero(&attr, আকার(সংঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;
    attr.মান = (uintptr_t) মান;

    ফেরত ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, আকার(attr));
}
