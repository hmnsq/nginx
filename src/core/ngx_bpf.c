
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#define NGX_BPF_LOGBUF_SIZE  (32 * 1024)


ساكن ngx_inline صحيح
ngx_bpf(تعداد bpf_cmd cmd, اتحاد bpf_attr *attr, unsigned صحيح size)
{
    أرجع syscall(__NR_bpf, cmd, attr, size);
}


فراغ
ngx_bpf_program_link(ngx_bpf_program_t *program, ثابت حرف *رمز, صحيح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = program->relocs;

    لكل (i = 0; i < program->nrelocs; i++) {
        إذا (ngx_strcmp(rl[i].name, رمز) == 0) {
            program->ins[rl[i].offset].src_reg = 1;
            program->ins[rl[i].offset].imm = fd;
        }
    }
}


صحيح
ngx_bpf_load_program(ngx_log_t *log, ngx_bpf_program_t *program)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;
#إذا (NGX_DEBUG)
    حرف            buf[NGX_BPF_LOGBUF_SIZE];
#endif

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.license = (uintptr_t) program->license;
    attr.prog_type = program->type;
    attr.insns = (uintptr_t) program->ins;
    attr.insn_cnt = program->nins;

#إذا (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_size = NGX_BPF_LOGBUF_SIZE;
    attr.log_level = 1;
#endif

    fd = ngx_bpf(BPF_PROG_LOAD, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0,
                       "bpf verifier: %s", buf);

        أرجع -1;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_create(ngx_log_t *log, تعداد bpf_map_type type, صحيح مفتاح_size,
    صحيح قيمة_size, صحيح max_entries, uint32_t map_flags)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_type = type;
    attr.مفتاح_size = مفتاح_size;
    attr.قيمة_size = قيمة_size;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      "failed to create BPF map");
        أرجع NGX_ERROR;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_update(صحيح fd, ثابت فراغ *مفتاح, ثابت فراغ *قيمة, uint64_t flags)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.مفتاح = (uintptr_t) مفتاح;
    attr.قيمة = (uintptr_t) قيمة;
    attr.flags = flags;

    أرجع ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_delete(صحيح fd, ثابت فراغ *مفتاح)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.مفتاح = (uintptr_t) مفتاح;

    أرجع ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *مفتاح, فراغ *قيمة)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.مفتاح = (uintptr_t) مفتاح;
    attr.قيمة = (uintptr_t) قيمة;

    أرجع ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, حجم(attr));
}
