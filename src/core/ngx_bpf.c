
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


ಸ್ಥಿರ ngx_inline ಪೂರ್ಣಾಙ್ಕ
ngx_bpf(ಗಣನಾ bpf_cmd cmd, ಸಙ್ಘ bpf_attr *attr, unsigned ಪೂರ್ಣಾಙ್ಕ size)
{
    ಪ್ರತ್ಯಾಗಮ syscall(__NR_bpf, cmd, attr, size);
}


ಶೂನ್ಯ
ngx_bpf_program_link(ngx_bpf_program_t *program, ಧ್ರುವ ವರ್ಣ *ಚಿಹ್ನೆ, ಪೂರ್ಣಾಙ್ಕ fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    ಪ್ರತಿ (i = 0; i < program->nrelocs; i++) {
        ಯದಿ (ngx_strcmp(rl[i].name, ಚಿಹ್ನೆ) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


ಪೂರ್ಣಾಙ್ಕ
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    ಪೂರ್ಣಾಙ್ಕ             fd;
    ಸಙ್ಘ bpf_attr  attr;
#ಯದಿ (NGX_DEBUG)
    ವರ್ಣ            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, ಪರಿಮಾಣ(ಸಙ್ಘ bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#ಯದಿ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, ಪರಿಮಾಣ(attr));
    ಯದಿ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        ಪ್ರತ್ಯಾಗಮ -1;
    }

    ಪ್ರತ್ಯಾಗಮ fd;
}


ಪೂರ್ಣಾಙ್ಕ
ngx_bpf_map_create(ngx_log_t *log, ಗಣನಾ bpf_map_type type, ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ_size,
    ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_size, ಪೂರ್ಣಾಙ್ಕ max_entries, uint32_t map_flags)
{
    ಪೂರ್ಣಾಙ್ಕ             fd;
    ಸಙ್ಘ bpf_attr  attr;

    ngx_memzero(&attr, ಪರಿಮಾಣ(ಸಙ್ಘ bpf_attr));

    attr.map_type = type;
    attr.ಕೀಲಿ_size = ಕೀಲಿ_size;
    attr.ಮೌಲ್ಯ_size = ಮೌಲ್ಯ_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, ಪರಿಮಾಣ(attr));
    ಯದಿ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ fd;
}


ಪೂರ್ಣಾಙ್ಕ
ngx_bpf_map_update(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ, uint64_t flags)
{
    ಸಙ್ಘ bpf_attr attr;

    ngx_memzero(&attr, ಪರಿಮಾಣ(ಸಙ್ಘ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;
    attr.ಮೌಲ್ಯ = (uintptr_t) ಮೌಲ್ಯ;
    attr.flags = flags;

    ಪ್ರತ್ಯಾಗಮ ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, ಪರಿಮಾಣ(attr));
}


ಪೂರ್ಣಾಙ್ಕ
ngx_bpf_map_delete(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ)
{
    ಸಙ್ಘ bpf_attr attr;

    ngx_memzero(&attr, ಪರಿಮಾಣ(ಸಙ್ಘ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;

    ಪ್ರತ್ಯಾಗಮ ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, ಪರಿಮಾಣ(attr));
}


ಪೂರ್ಣಾಙ್ಕ
ngx_bpf_map_lookup(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ)
{
    ಸಙ್ಘ bpf_attr attr;

    ngx_memzero(&attr, ಪರಿಮಾಣ(ಸಙ್ಘ bpf_attr));

    attr.map_fd = fd;
    attr.ಕೀಲಿ = (uintptr_t) ಕೀಲಿ;
    attr.ಮೌಲ್ಯ = (uintptr_t) ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, ಪರಿಮಾಣ(attr));
}
