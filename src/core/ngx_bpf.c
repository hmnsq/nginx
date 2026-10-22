
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


static ngx_inline ཧྲིལ
ngx_bpf(ga_nanaa bpf_cmd cmd, sa_ngha bpf_attr *attr, unsigned ཧྲིལ size)
{
    ལོག syscall(__NR_bpf, cmd, attr, size);
}


void
ngx_bpf_program_link(ngx_bpf_program_t *program, const var_na *symbol, ཧྲིལ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    དོན་ལུ (i = 0; i < program->nrelocs; i++) {
        གལ་སྲིད (ngx_strcmp(rl[i].name, symbol) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


ཧྲིལ
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    ཧྲིལ             fd;
    sa_ngha bpf_attr  attr;
#གལ་སྲིད (NGX_DEBUG)
    var_na            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#གལ་སྲིད (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, parimaa_na(attr));
    གལ་སྲིད (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ལོག -1;
    }

    ལོག fd;
}


ཧྲིལ
ngx_bpf_map_create(ngx_log_t *log, ga_nanaa bpf_map_type type, ཧྲིལ key_size,
    ཧྲིལ value_size, ཧྲིལ max_entries, uint32_t map_flags)
{
    ཧྲིལ             fd;
    sa_ngha bpf_attr  attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_type = type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, parimaa_na(attr));
    གལ་སྲིད (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ལོག NGX_ERROR;
    }

    ལོག fd;
}


ཧྲིལ
ngx_bpf_map_update(ཧྲིལ fd, const void *key, const void *value, uint64_t flags)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;
    attr.flags = flags;

    ལོག ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, parimaa_na(attr));
}


ཧྲིལ
ngx_bpf_map_delete(ཧྲིལ fd, const void *key)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;

    ལོག ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, parimaa_na(attr));
}


ཧྲིལ
ngx_bpf_map_lookup(ཧྲིལ fd, const void *key, void *value)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;

    ལོག ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, parimaa_na(attr));
}
