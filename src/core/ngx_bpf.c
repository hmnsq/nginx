
/*
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


স্থির ngx_inline পূর্ণাঙ্ক
ngx_bpf(গণনা bpf_cmd cmd, সঙ্ঘ bpf_attr *attr, unsigned পূর্ণাঙ্ক size)
{
    ঘূৰাই syscall(__NR_bpf, cmd, attr, size);
}


শূন্য
ngx_bpf_program_link(ngx_bpf_program_t *program, ধ্রুৱ ৱর্ণ *প্ৰতীক, পূর্ণাঙ্ক fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    বাবে (i = 0; i < program->nrelocs; i++) {
        যদি (ngx_strcmp(rl[i].name, প্ৰতীক) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


পূর্ণাঙ্ক
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    পূর্ণাঙ্ক             fd;
    সঙ্ঘ bpf_attr  attr;
#যদি (NGX_DEBUG)
    ৱর্ণ            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, পরিমাণ(সঙ্ঘ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#যদি (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, পরিমাণ(attr));
    যদি (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ঘূৰাই -1;
    }

    ঘূৰাই fd;
}


পূর্ণাঙ্ক
ngx_bpf_map_create(ngx_log_t *log, গণনা bpf_map_type type, পূর্ণাঙ্ক চাবি_size,
    পূর্ণাঙ্ক মান_size, পূর্ণাঙ্ক max_entries, uint32_t map_flags)
{
    পূর্ণাঙ্ক             fd;
    সঙ্ঘ bpf_attr  attr;

    ngx_memzero(&attr, পরিমাণ(সঙ্ঘ bpf_attr));

    attr.map_type = type;
    attr.চাবি_size = চাবি_size;
    attr.মান_size = মান_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, পরিমাণ(attr));
    যদি (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই fd;
}


পূর্ণাঙ্ক
ngx_bpf_map_update(পূর্ণাঙ্ক fd, ধ্রুৱ শূন্য *চাবি, ধ্রুৱ শূন্য *মান, uint64_t flags)
{
    সঙ্ঘ bpf_attr attr;

    ngx_memzero(&attr, পরিমাণ(সঙ্ঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;
    attr.মান = (uintptr_t) মান;
    attr.flags = flags;

    ঘূৰাই ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, পরিমাণ(attr));
}


পূর্ণাঙ্ক
ngx_bpf_map_delete(পূর্ণাঙ্ক fd, ধ্রুৱ শূন্য *চাবি)
{
    সঙ্ঘ bpf_attr attr;

    ngx_memzero(&attr, পরিমাণ(সঙ্ঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;

    ঘূৰাই ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, পরিমাণ(attr));
}


পূর্ণাঙ্ক
ngx_bpf_map_lookup(পূর্ণাঙ্ক fd, ধ্রুৱ শূন্য *চাবি, শূন্য *মান)
{
    সঙ্ঘ bpf_attr attr;

    ngx_memzero(&attr, পরিমাণ(সঙ্ঘ bpf_attr));

    attr.map_fd = fd;
    attr.চাবি = (uintptr_t) চাবি;
    attr.মান = (uintptr_t) মান;

    ঘূৰাই ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, পরিমাণ(attr));
}
