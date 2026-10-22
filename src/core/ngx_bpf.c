
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


static ngx_inline số_nguyên
ngx_bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned số_nguyên size)
{
    trả_về syscall(__NR_bpf, cmd, attr, size);
}


void
ngx_bpf_program_link(ngx_bpf_program_t *program, const char *symbol, số_nguyên fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    cho (i = 0; i < program->nrelocs; i++) {
        nếu (ngx_strcmp(rl[i].name, symbol) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


số_nguyên
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    số_nguyên             fd;
    union bpf_attr  attr;
#nếu (NGX_DEBUG)
    char            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#nếu (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, sizeof(attr));
    nếu (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        trả_về -1;
    }

    trả_về fd;
}


số_nguyên
ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, số_nguyên key_size,
    số_nguyên value_size, số_nguyên max_entries, uint32_t map_flags)
{
    số_nguyên             fd;
    union bpf_attr  attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_type = type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, sizeof(attr));
    nếu (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        trả_về NGX_ERROR;
    }

    trả_về fd;
}


số_nguyên
ngx_bpf_map_update(số_nguyên fd, const void *key, const void *value, uint64_t flags)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;
    attr.flags = flags;

    trả_về ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
}


số_nguyên
ngx_bpf_map_delete(số_nguyên fd, const void *key)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;

    trả_về ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, sizeof(attr));
}


số_nguyên
ngx_bpf_map_lookup(số_nguyên fd, const void *key, void *value)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;

    trả_về ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
}
