
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


static ngx_inline 정수
ngx_bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned 정수 size)
{
    반환 syscall(__NR_bpf, cmd, attr, size);
}


void
ngx_bpf_program_link(ngx_bpf_program_t *program, const char *symbol, 정수 fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    위해 (i = 0; i < program->nrelocs; i++) {
        만약 (ngx_strcmp(rl[i].name, symbol) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


정수
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    정수             fd;
    union bpf_attr  attr;
#만약 (NGX_DEBUG)
    char            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#만약 (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, sizeof(attr));
    만약 (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        반환 -1;
    }

    반환 fd;
}


정수
ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, 정수 key_size,
    정수 value_size, 정수 max_entries, uint32_t map_flags)
{
    정수             fd;
    union bpf_attr  attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_type = type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, sizeof(attr));
    만약 (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        반환 NGX_ERROR;
    }

    반환 fd;
}


정수
ngx_bpf_map_update(정수 fd, const void *key, const void *value, uint64_t flags)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;
    attr.flags = flags;

    반환 ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
}


정수
ngx_bpf_map_delete(정수 fd, const void *key)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;

    반환 ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, sizeof(attr));
}


정수
ngx_bpf_map_lookup(정수 fd, const void *key, void *value)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;

    반환 ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
}
