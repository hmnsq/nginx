
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


ایستا ngx_inline صحیح
ngx_bpf(شمارش bpf_cmd cmd, اجتماع bpf_attr *attr, unsigned صحیح size)
{
    بازگشت syscall(__NR_bpf, cmd, attr, size);
}


تهی
ngx_bpf_program_link(ngx_bpf_program_t *program, ثابت نویسه *نماد, صحیح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    برای (i = 0; i < program->nrelocs; i++) {
        اگر (ngx_strcmp(rl[i].name, نماد) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


صحیح
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    صحیح             fd;
    اجتماع bpf_attr  attr;
#اگر (NGX_DEBUG)
    نویسه            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, اندازه(اجتماع bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#اگر (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, اندازه(attr));
    اگر (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        بازگشت -1;
    }

    بازگشت fd;
}


صحیح
ngx_bpf_map_create(ngx_log_t *log, شمارش bpf_map_type type, صحیح کلید_size,
    صحیح مقدار_size, صحیح max_entries, uint32_t map_flags)
{
    صحیح             fd;
    اجتماع bpf_attr  attr;

    ngx_memzero(&attr, اندازه(اجتماع bpf_attr));

    attr.map_type = type;
    attr.کلید_size = کلید_size;
    attr.مقدار_size = مقدار_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, اندازه(attr));
    اگر (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        بازگشت NGX_ERROR;
    }

    بازگشت fd;
}


صحیح
ngx_bpf_map_update(صحیح fd, ثابت تهی *کلید, ثابت تهی *مقدار, uint64_t flags)
{
    اجتماع bpf_attr attr;

    ngx_memzero(&attr, اندازه(اجتماع bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.مقدار = (uintptr_t) مقدار;
    attr.flags = flags;

    بازگشت ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, اندازه(attr));
}


صحیح
ngx_bpf_map_delete(صحیح fd, ثابت تهی *کلید)
{
    اجتماع bpf_attr attr;

    ngx_memzero(&attr, اندازه(اجتماع bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;

    بازگشت ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, اندازه(attr));
}


صحیح
ngx_bpf_map_lookup(صحیح fd, ثابت تهی *کلید, تهی *مقدار)
{
    اجتماع bpf_attr attr;

    ngx_memzero(&attr, اندازه(اجتماع bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.مقدار = (uintptr_t) مقدار;

    بازگشت ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, اندازه(attr));
}
