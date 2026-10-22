
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کرو NGX_BPF_LOGBUF_ناپ  (32 * 1024)


ساكن ngx_inline صحيح
ngx_bpf(تعداد bpf_cmd cmd, اتحاد bpf_attr *attr, بنانشانی صحيح ناپ)
{
    أرجع syscall(__NR_bpf, cmd, attr, ناپ);
}


فراغ
ngx_bpf_پروگرام_link(ngx_bpf_پروگرام_t *پروگرام, ثابت حرف *علامت, صحيح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = پروگرام->relocs;

    لكل (i = 0; i < پروگرام->nrelocs; i++) {
        إذا (ngx_strcmp(rl[i].name, علامت) == 0) {
            پروگرام->ins[rl[i].وتھ].src_reg = 1;
            پروگرام->ins[rl[i].وتھ].imm = fd;
        }
    }
}


صحيح
ngx_bpf_لدو_پروگرام(ngx_log_t *لوگارتھم, ngx_bpf_پروگرام_t *پروگرام)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;
#إذا (NGX_DEBUG)
    حرف            buf[NGX_BPF_LOGBUF_ناپ];
#جے_انت

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.license = (uintptr_t) پروگرام->license;
    attr.prog_type = پروگرام->type;
    attr.insns = (uintptr_t) پروگرام->ins;
    attr.insn_cnt = پروگرام->nins;

#إذا (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_ناپ = NGX_BPF_LOGBUF_ناپ;
    attr.log_level = 1;
#جے_انت

    fd = ngx_bpf(BPF_PROG_لدو, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "bpf verifier: %s", buf);

        أرجع -1;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_create(ngx_log_t *لوگارتھم, تعداد bpf_map_type type, صحيح کلید_ناپ,
    صحيح قدر_ناپ, صحيح max_entries, uint32_t map_flags)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_type = type;
    attr.کلید_ناپ = کلید_ناپ;
    attr.قدر_ناپ = قدر_ناپ;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      "failed to create BPF map");
        أرجع NGX_ERROR;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلید, ثابت فراغ *قدر, uint64_t flags)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.قدر = (uintptr_t) قدر;
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
ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلید, فراغ *قدر)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.قدر = (uintptr_t) قدر;

    أرجع ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, حجم(attr));
}
