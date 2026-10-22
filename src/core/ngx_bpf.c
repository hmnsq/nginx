
/*
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


ସ୍ଥିର ngx_inline ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf(ଗଣନା bpf_cmd cmd, ସଂଘ bpf_attr *attr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    ଫେରନ୍ତୁ syscall(__NR_bpf, cmd, attr, size);
}


ଶୂନ୍ୟ
ngx_bpf_program_link(ngx_bpf_program_t *program, ଅଚଳ ଅକ୍ଷର *ପ୍ରତୀକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    ପାଇଁ (i = 0; i < program->nrelocs; i++) {
        ଯଦି (ngx_strcmp(rl[i].name, ପ୍ରତୀକ) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା             fd;
    ସଂଘ bpf_attr  attr;
#ଯଦି (NGX_DEBUG)
    ଅକ୍ଷର            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, ଆକାର(ସଂଘ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#ଯଦି (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, ଆକାର(attr));
    ଯଦି (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ଫେରନ୍ତୁ -1;
    }

    ଫେରନ୍ତୁ fd;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf_map_create(ngx_log_t *log, ଗଣନା bpf_map_type type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି_size,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_entries, uint32_t map_flags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା             fd;
    ସଂଘ bpf_attr  attr;

    ngx_memzero(&attr, ଆକାର(ସଂଘ bpf_attr));

    attr.map_type = type;
    attr.ଚାବି_size = ଚାବି_size;
    attr.ମୂଲ୍ୟ_size = ମୂଲ୍ୟ_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, ଆକାର(attr));
    ଯଦି (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ fd;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf_map_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି, ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ, uint64_t flags)
{
    ସଂଘ bpf_attr attr;

    ngx_memzero(&attr, ଆକାର(ସଂଘ bpf_attr));

    attr.map_fd = fd;
    attr.ଚାବି = (uintptr_t) ଚାବି;
    attr.ମୂଲ୍ୟ = (uintptr_t) ମୂଲ୍ୟ;
    attr.flags = flags;

    ଫେରନ୍ତୁ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, ଆକାର(attr));
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf_map_delete(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି)
{
    ସଂଘ bpf_attr attr;

    ngx_memzero(&attr, ଆକାର(ସଂଘ bpf_attr));

    attr.map_fd = fd;
    attr.ଚାବି = (uintptr_t) ଚାବି;

    ଫେରନ୍ତୁ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, ଆକାର(attr));
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ngx_bpf_map_lookup(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *ଚାବି, ଶୂନ୍ୟ *ମୂଲ୍ୟ)
{
    ସଂଘ bpf_attr attr;

    ngx_memzero(&attr, ଆକାର(ସଂଘ bpf_attr));

    attr.map_fd = fd;
    attr.ଚାବି = (uintptr_t) ଚାବି;
    attr.ମୂଲ୍ୟ = (uintptr_t) ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, ଆକାର(attr));
}
