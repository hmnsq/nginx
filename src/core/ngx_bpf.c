
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


स्थिर ngx_inline पूर्णाङ्क
ngx_bpf(गणना bpf_cmd cmd, सङ्घ bpf_attr *attr, अचिन्हित पूर्णाङ्क size)
{
    प्रत्यागम syscall(__NR_bpf, cmd, attr, size);
}


शून्य
ngx_bpf_program_link(ngx_bpf_program_t *program, ध्रुव वर्ण *प्रतीक, पूर्णाङ्क fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    प्रति (i = 0; i < program->nrelocs; i++) {
        यदि (ngx_strcmp(rl[i].name, प्रतीक) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


पूर्णाङ्क
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    पूर्णाङ्क             fd;
    सङ्घ bpf_attr  attr;
#यदि (NGX_DEBUG)
    वर्ण            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#यदि (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, परिमाण(attr));
    यदि (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        प्रत्यागम -1;
    }

    प्रत्यागम fd;
}


पूर्णाङ्क
ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णाङ्क कुंजी_size,
    पूर्णाङ्क मूल्य_size, पूर्णाङ्क max_entries, uint32_t map_flags)
{
    पूर्णाङ्क             fd;
    सङ्घ bpf_attr  attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_type = type;
    attr.कुंजी_size = कुंजी_size;
    attr.मूल्य_size = मूल्य_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, परिमाण(attr));
    यदि (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम fd;
}


पूर्णाङ्क
ngx_bpf_map_update(पूर्णाङ्क fd, ध्रुव शून्य *कुंजी, ध्रुव शून्य *मूल्य, uint64_t flags)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;
    attr.मूल्य = (uintptr_t) मूल्य;
    attr.flags = flags;

    प्रत्यागम ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_delete(पूर्णाङ्क fd, ध्रुव शून्य *कुंजी)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;

    प्रत्यागम ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_lookup(पूर्णाङ्क fd, ध्रुव शून्य *कुंजी, शून्य *मूल्य)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.कुंजी = (uintptr_t) कुंजी;
    attr.मूल्य = (uintptr_t) मूल्य;

    प्रत्यागम ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, परिमाण(attr));
}
