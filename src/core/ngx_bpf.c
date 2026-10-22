
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


static ngx_inline 整数
ngx_bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned 整数 size)
{
    返回 syscall(__NR_bpf, cmd, attr, size);
}


void
ngx_bpf_program_link(ngx_bpf_program_t *program, const char *symbol, 整数 fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    为 (i = 0; i < program->nrelocs; i++) {
        如果 (ngx_strcmp(rl[i].name, symbol) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


整数
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    整数             fd;
    union bpf_attr  attr;
#如果 (NGX_DEBUG)
    char            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#如果 (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, sizeof(attr));
    如果 (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        返回 -1;
    }

    返回 fd;
}


整数
ngx_bpf_map_create(ngx_log_t *log, enum bpf_map_type type, 整数 key_size,
    整数 value_size, 整数 max_entries, uint32_t map_flags)
{
    整数             fd;
    union bpf_attr  attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_type = type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, sizeof(attr));
    如果 (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        返回 NGX_ERROR;
    }

    返回 fd;
}


整数
ngx_bpf_map_update(整数 fd, const void *key, const void *value, uint64_t flags)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;
    attr.flags = flags;

    返回 ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
}


整数
ngx_bpf_map_delete(整数 fd, const void *key)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;

    返回 ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, sizeof(attr));
}


整数
ngx_bpf_map_lookup(整数 fd, const void *key, void *value)
{
    union bpf_attr attr;

    ngx_memzero(&attr, sizeof(union bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;

    返回 ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
}
