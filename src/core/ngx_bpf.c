
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


स्थिर ngx_inline पूर्णाङ्क
ngx_bpf(गणना bpf_cmd cmd, सङ्घ bpf_attr *attr, अचिन्हित पूर्णाङ्क size)
{
    फर्काऊ syscall(__NR_bpf, cmd, attr, size);
}


शून्य
ngx_bpf_program_link(ngx_bpf_program_t *program, अचर वर्ण *चिह्न, पूर्णाङ्क fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    लागि (i = 0; i < program->nrelocs; i++) {
        यदि (ngx_strcmp(rl[i].name, चिह्न) == 0) {
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

        फर्काऊ -1;
    }

    फर्काऊ fd;
}


पूर्णाङ्क
ngx_bpf_map_create(ngx_log_t *log, गणना bpf_map_type type, पूर्णाङ्क साँचो_size,
    पूर्णाङ्क मान_size, पूर्णाङ्क max_entries, uint32_t map_flags)
{
    पूर्णाङ्क             fd;
    सङ्घ bpf_attr  attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_type = type;
    attr.साँचो_size = साँचो_size;
    attr.मान_size = मान_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, परिमाण(attr));
    यदि (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        फर्काऊ NGX_ERROR;
    }

    फर्काऊ fd;
}


पूर्णाङ्क
ngx_bpf_map_update(पूर्णाङ्क fd, अचर शून्य *साँचो, अचर शून्य *मान, uint64_t flags)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.साँचो = (uintptr_t) साँचो;
    attr.मान = (uintptr_t) मान;
    attr.flags = flags;

    फर्काऊ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_delete(पूर्णाङ्क fd, अचर शून्य *साँचो)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.साँचो = (uintptr_t) साँचो;

    फर्काऊ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, परिमाण(attr));
}


पूर्णाङ्क
ngx_bpf_map_lookup(पूर्णाङ्क fd, अचर शून्य *साँचो, शून्य *मान)
{
    सङ्घ bpf_attr attr;

    ngx_memzero(&attr, परिमाण(सङ्घ bpf_attr));

    attr.map_fd = fd;
    attr.साँचो = (uintptr_t) साँचो;
    attr.मान = (uintptr_t) मान;

    फर्काऊ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, परिमाण(attr));
}
