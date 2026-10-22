
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کن NGX_BPF_LOGBUF_اندازه  (32 * 1024)


ساكن ngx_inline صحيح
ngx_bpf(تعداد bpf_cmd cmd, اتحاد bpf_attr *attr, بیعلامت صحيح اندازه)
{
    أرجع syscall(__NR_bpf, cmd, attr, اندازه);
}


فراغ
ngx_bpf_برنامه_link(ngx_bpf_برنامه_t *برنامه, ثابت حرف *نماد, صحيح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = برنامه->relocs;

    لكل (i = 0; i < برنامه->nrelocs; i++) {
        إذا (ngx_strcmp(rl[i].name, نماد) == 0) {
            برنامه->ins[rl[i].جابه_جایی].src_reg = 1;
            برنامه->ins[rl[i].جابه_جایی].imm = fd;
        }
    }
}


صحيح
ngx_bpf_بارگیری_برنامه(ngx_log_t *لگاریتم, ngx_bpf_برنامه_t *برنامه)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;
#إذا (NGX_DEBUG)
    حرف            buf[NGX_BPF_LOGBUF_اندازه];
#پایان_اگر

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.license = (uintptr_t) برنامه->license;
    attr.prog_type = برنامه->type;
    attr.insns = (uintptr_t) برنامه->ins;
    attr.insn_cnt = برنامه->nins;

#إذا (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_اندازه = NGX_BPF_LOGBUF_اندازه;
    attr.log_level = 1;
#پایان_اگر

    fd = ngx_bpf(BPF_PROG_بارگیری, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لگاریتم, 0,
                       "bpf verifier: %s", buf);

        أرجع -1;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_create(ngx_log_t *لگاریتم, تعداد bpf_map_type type, صحيح کلید_اندازه,
    صحيح مقدار_اندازه, صحيح max_entries, uint32_t map_flags)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_type = type;
    attr.کلید_اندازه = کلید_اندازه;
    attr.مقدار_اندازه = مقدار_اندازه;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      "failed to create BPF map");
        أرجع NGX_ERROR;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلید, ثابت فراغ *مقدار, uint64_t flags)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.مقدار = (uintptr_t) مقدار;
    attr.flags = flags;

    أرجع ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کلید)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;

    أرجع ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلید, فراغ *مقدار)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.مقدار = (uintptr_t) مقدار;

    أرجع ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, حجم(attr));
}
