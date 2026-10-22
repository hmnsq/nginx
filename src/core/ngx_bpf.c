
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


स्थिर ngx_inline पूर्णांक
ngx_bpf(गणना bpf_cmd cmd, संघ bpf_attr *attr, अचिन्हित पूर्णांक size)
{
    परत syscall(__NR_bpf, cmd, attr, size);
}


शून्य
ngx_bpf_program_link(ngx_bpf_program_t *program, स्थिरांक अक्षर *चिन्ह, पूर्णांक fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    साठी (i = 0; i < program->nrelocs; i++) {
        जर (ngx_strcmp(rl[i].name, चिन्ह) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


पूर्णांक
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    पूर्णांक             fd;
    संघ bpf_attr  attr;
#जर (NGX_DEBUG)
    अक्षर            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, आकार(संघ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#जर (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, आकार(attr));
    जर (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        परत -1;
    }

    परत fd;
}


पूर्णांक
ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णांक किल्ली_size,
    पूर्णांक मूल्य_size, पूर्णांक max_entries, uint32_t map_flags)
{
    पूर्णांक             fd;
    संघ bpf_attr  attr;

    ngx_memzero(&attr, आकार(संघ bpf_attr));

    attr.map_type = type;
    attr.किल्ली_size = किल्ली_size;
    attr.मूल्य_size = मूल्य_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, आकार(attr));
    जर (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        परत NGX_ERROR;
    }

    परत fd;
}


पूर्णांक
ngx_bpf_map_update(पूर्णांक fd, स्थिरांक शून्य *किल्ली, स्थिरांक शून्य *मूल्य, uint64_t flags)
{
    संघ bpf_attr attr;

    ngx_memzero(&attr, आकार(संघ bpf_attr));

    attr.map_fd = fd;
    attr.किल्ली = (uintptr_t) किल्ली;
    attr.मूल्य = (uintptr_t) मूल्य;
    attr.flags = flags;

    परत ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, आकार(attr));
}


पूर्णांक
ngx_bpf_map_delete(पूर्णांक fd, स्थिरांक शून्य *किल्ली)
{
    संघ bpf_attr attr;

    ngx_memzero(&attr, आकार(संघ bpf_attr));

    attr.map_fd = fd;
    attr.किल्ली = (uintptr_t) किल्ली;

    परत ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, आकार(attr));
}


पूर्णांक
ngx_bpf_map_lookup(पूर्णांक fd, स्थिरांक शून्य *किल्ली, शून्य *मूल्य)
{
    संघ bpf_attr attr;

    ngx_memzero(&attr, आकार(संघ bpf_attr));

    attr.map_fd = fd;
    attr.किल्ली = (uintptr_t) किल्ली;
    attr.मूल्य = (uintptr_t) मूल्य;

    परत ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, आकार(attr));
}
