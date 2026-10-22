
/*
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


நிலையான ngx_inline முழு
ngx_bpf(எண்ணிக்கை bpf_cmd cmd, ஒன்றிப்பு bpf_attr *attr, unsigned முழு size)
{
    திரும்பு syscall(__NR_bpf, cmd, attr, size);
}


வெறுமை
ngx_bpf_program_link(ngx_bpf_program_t *program, மாறிலி எழுத்து *குறியீடு, முழு fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    க்கு (i = 0; i < program->nrelocs; i++) {
        எனில் (ngx_strcmp(rl[i].name, குறியீடு) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


முழு
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    முழு             fd;
    ஒன்றிப்பு bpf_attr  attr;
#எனில் (NGX_DEBUG)
    எழுத்து            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, அளவு(ஒன்றிப்பு bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#எனில் (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, அளவு(attr));
    எனில் (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        திரும்பு -1;
    }

    திரும்பு fd;
}


முழு
ngx_bpf_map_create(ngx_log_t *log, எண்ணிக்கை bpf_map_type type, முழு சாவி_size,
    முழு மதிப்பு_size, முழு max_entries, uint32_t map_flags)
{
    முழு             fd;
    ஒன்றிப்பு bpf_attr  attr;

    ngx_memzero(&attr, அளவு(ஒன்றிப்பு bpf_attr));

    attr.map_type = type;
    attr.சாவி_size = சாவி_size;
    attr.மதிப்பு_size = மதிப்பு_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, அளவு(attr));
    எனில் (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        திரும்பு NGX_ERROR;
    }

    திரும்பு fd;
}


முழு
ngx_bpf_map_update(முழு fd, மாறிலி வெறுமை *சாவி, மாறிலி வெறுமை *மதிப்பு, uint64_t flags)
{
    ஒன்றிப்பு bpf_attr attr;

    ngx_memzero(&attr, அளவு(ஒன்றிப்பு bpf_attr));

    attr.map_fd = fd;
    attr.சாவி = (uintptr_t) சாவி;
    attr.மதிப்பு = (uintptr_t) மதிப்பு;
    attr.flags = flags;

    திரும்பு ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, அளவு(attr));
}


முழு
ngx_bpf_map_delete(முழு fd, மாறிலி வெறுமை *சாவி)
{
    ஒன்றிப்பு bpf_attr attr;

    ngx_memzero(&attr, அளவு(ஒன்றிப்பு bpf_attr));

    attr.map_fd = fd;
    attr.சாவி = (uintptr_t) சாவி;

    திரும்பு ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, அளவு(attr));
}


முழு
ngx_bpf_map_lookup(முழு fd, மாறிலி வெறுமை *சாவி, வெறுமை *மதிப்பு)
{
    ஒன்றிப்பு bpf_attr attr;

    ngx_memzero(&attr, அளவு(ஒன்றிப்பு bpf_attr));

    attr.map_fd = fd;
    attr.சாவி = (uintptr_t) சாவி;
    attr.மதிப்பு = (uintptr_t) மதிப்பு;

    திரும்பு ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, அளவு(attr));
}
