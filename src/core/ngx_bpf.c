
/*
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


സ്ഥിതം ngx_inline പൂർണ്ണസംഖ്യ
ngx_bpf(എണ്ണൽ bpf_cmd cmd, സംഘം bpf_attr *attr, unsigned പൂർണ്ണസംഖ്യ size)
{
    തിരികെ syscall(__NR_bpf, cmd, attr, size);
}


ശൂന്യം
ngx_bpf_program_link(ngx_bpf_program_t *program, സ്ഥിരം അക്ഷരം *ചിഹ്നം, പൂർണ്ണസംഖ്യ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    വേണ്ടി (i = 0; i < program->nrelocs; i++) {
        എങ്കിൽ (ngx_strcmp(rl[i].name, ചിഹ്നം) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


പൂർണ്ണസംഖ്യ
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    പൂർണ്ണസംഖ്യ             fd;
    സംഘം bpf_attr  attr;
#എങ്കിൽ (NGX_DEBUG)
    അക്ഷരം            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, വലുപ്പം(സംഘം bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#എങ്കിൽ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, വലുപ്പം(attr));
    എങ്കിൽ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        തിരികെ -1;
    }

    തിരികെ fd;
}


പൂർണ്ണസംഖ്യ
ngx_bpf_map_create(ngx_log_t *log, എണ്ണൽ bpf_map_type type, പൂർണ്ണസംഖ്യ താക്കോൽ_size,
    പൂർണ്ണസംഖ്യ മൂല്യം_size, പൂർണ്ണസംഖ്യ max_entries, uint32_t map_flags)
{
    പൂർണ്ണസംഖ്യ             fd;
    സംഘം bpf_attr  attr;

    ngx_memzero(&attr, വലുപ്പം(സംഘം bpf_attr));

    attr.map_type = type;
    attr.താക്കോൽ_size = താക്കോൽ_size;
    attr.മൂല്യം_size = മൂല്യം_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, വലുപ്പം(attr));
    എങ്കിൽ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        തിരികെ NGX_ERROR;
    }

    തിരികെ fd;
}


പൂർണ്ണസംഖ്യ
ngx_bpf_map_update(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ, സ്ഥിരം ശൂന്യം *മൂല്യം, uint64_t flags)
{
    സംഘം bpf_attr attr;

    ngx_memzero(&attr, വലുപ്പം(സംഘം bpf_attr));

    attr.map_fd = fd;
    attr.താക്കോൽ = (uintptr_t) താക്കോൽ;
    attr.മൂല്യം = (uintptr_t) മൂല്യം;
    attr.flags = flags;

    തിരികെ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, വലുപ്പം(attr));
}


പൂർണ്ണസംഖ്യ
ngx_bpf_map_delete(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ)
{
    സംഘം bpf_attr attr;

    ngx_memzero(&attr, വലുപ്പം(സംഘം bpf_attr));

    attr.map_fd = fd;
    attr.താക്കോൽ = (uintptr_t) താക്കോൽ;

    തിരികെ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, വലുപ്പം(attr));
}


പൂർണ്ണസംഖ്യ
ngx_bpf_map_lookup(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *താക്കോൽ, ശൂന്യം *മൂല്യം)
{
    സംഘം bpf_attr attr;

    ngx_memzero(&attr, വലുപ്പം(സംഘം bpf_attr));

    attr.map_fd = fd;
    attr.താക്കോൽ = (uintptr_t) താക്കോൽ;
    attr.മൂല്യം = (uintptr_t) മൂല്യം;

    തിരികെ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, വലുപ്പം(attr));
}
