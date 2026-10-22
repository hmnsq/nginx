
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


static ngx_inline ຈຳນວນເຕັມ
ngx_bpf(ga_nanaa bpf_cmd cmd, sa_ngha bpf_attr *attr, unsigned ຈຳນວນເຕັມ size)
{
    ສົ່ງຄືນ syscall(__NR_bpf, cmd, attr, size);
}


void
ngx_bpf_program_link(ngx_bpf_program_t *program, const var_na *symbol, ຈຳນວນເຕັມ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    ສຳລັບ (i = 0; i < program->nrelocs; i++) {
        ຖ້າ (ngx_strcmp(rl[i].name, symbol) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


ຈຳນວນເຕັມ
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    ຈຳນວນເຕັມ             fd;
    sa_ngha bpf_attr  attr;
#ຖ້າ (NGX_DEBUG)
    var_na            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#ຖ້າ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, parimaa_na(attr));
    ຖ້າ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ສົ່ງຄືນ -1;
    }

    ສົ່ງຄືນ fd;
}


ຈຳນວນເຕັມ
ngx_bpf_map_create(ngx_log_t *log, ga_nanaa bpf_map_type type, ຈຳນວນເຕັມ key_size,
    ຈຳນວນເຕັມ value_size, ຈຳນວນເຕັມ max_entries, uint32_t map_flags)
{
    ຈຳນວນເຕັມ             fd;
    sa_ngha bpf_attr  attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_type = type;
    attr.key_size = key_size;
    attr.value_size = value_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, parimaa_na(attr));
    ຖ້າ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສົ່ງຄືນ fd;
}


ຈຳນວນເຕັມ
ngx_bpf_map_update(ຈຳນວນເຕັມ fd, const void *key, const void *value, uint64_t flags)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;
    attr.flags = flags;

    ສົ່ງຄືນ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, parimaa_na(attr));
}


ຈຳນວນເຕັມ
ngx_bpf_map_delete(ຈຳນວນເຕັມ fd, const void *key)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;

    ສົ່ງຄືນ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, parimaa_na(attr));
}


ຈຳນວນເຕັມ
ngx_bpf_map_lookup(ຈຳນວນເຕັມ fd, const void *key, void *value)
{
    sa_ngha bpf_attr attr;

    ngx_memzero(&attr, parimaa_na(sa_ngha bpf_attr));

    attr.map_fd = fd;
    attr.key = (uintptr_t) key;
    attr.value = (uintptr_t) value;

    ສົ່ງຄືນ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, parimaa_na(attr));
}
