
/*
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


સ્થિર ngx_inline પૂર્ણાંક
ngx_bpf(ગણના bpf_cmd cmd, સંઘ bpf_attr *attr, unsigned પૂર્ણાંક size)
{
    પાછા syscall(__NR_bpf, cmd, attr, size);
}


શૂન્ય
ngx_bpf_program_link(ngx_bpf_program_t *program, અચલ અક્ષર *પ્રતીક, પૂર્ણાંક fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    માટે (i = 0; i < program->nrelocs; i++) {
        જો (ngx_strcmp(rl[i].name, પ્રતીક) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


પૂર્ણાંક
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    પૂર્ણાંક             fd;
    સંઘ bpf_attr  attr;
#જો (NGX_DEBUG)
    અક્ષર            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, કદ(સંઘ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#જો (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, કદ(attr));
    જો (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        પાછા -1;
    }

    પાછા fd;
}


પૂર્ણાંક
ngx_bpf_map_create(ngx_log_t *log, ગણના bpf_map_type type, પૂર્ણાંક ચાવી_size,
    પૂર્ણાંક કિંમત_size, પૂર્ણાંક max_entries, uint32_t map_flags)
{
    પૂર્ણાંક             fd;
    સંઘ bpf_attr  attr;

    ngx_memzero(&attr, કદ(સંઘ bpf_attr));

    attr.map_type = type;
    attr.ચાવી_size = ચાવી_size;
    attr.કિંમત_size = કિંમત_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, કદ(attr));
    જો (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        પાછા NGX_ERROR;
    }

    પાછા fd;
}


પૂર્ણાંક
ngx_bpf_map_update(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી, અચલ શૂન્ય *કિંમત, uint64_t flags)
{
    સંઘ bpf_attr attr;

    ngx_memzero(&attr, કદ(સંઘ bpf_attr));

    attr.map_fd = fd;
    attr.ચાવી = (uintptr_t) ચાવી;
    attr.કિંમત = (uintptr_t) કિંમત;
    attr.flags = flags;

    પાછા ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, કદ(attr));
}


પૂર્ણાંક
ngx_bpf_map_delete(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી)
{
    સંઘ bpf_attr attr;

    ngx_memzero(&attr, કદ(સંઘ bpf_attr));

    attr.map_fd = fd;
    attr.ચાવી = (uintptr_t) ચાવી;

    પાછા ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, કદ(attr));
}


પૂર્ણાંક
ngx_bpf_map_lookup(પૂર્ણાંક fd, અચલ શૂન્ય *ચાવી, શૂન્ય *કિંમત)
{
    સંઘ bpf_attr attr;

    ngx_memzero(&attr, કદ(સંઘ bpf_attr));

    attr.map_fd = fd;
    attr.ચાવી = (uintptr_t) ચાવી;
    attr.કિંમત = (uintptr_t) કિંમત;

    પાછા ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, કદ(attr));
}
