
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کړه NGX_BPF_LOGBUF_اندازه  (32 * 1024)


ساكن ngx_inline صحيح
ngx_bpf(تعداد bpf_cmd cmd, اتحاد bpf_attr *attr, بینښه صحيح اندازه)
{
    أرجع syscall(__NR_bpf, cmd, attr, اندازه);
}


فراغ
ngx_bpf_پروګرام_link(ngx_bpf_پروګرام_t *پروګرام, ثابت حرف *نښه, صحيح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = پروګرام->relocs;

    لكل (i = 0; i < پروګرام->nrelocs; i++) {
        إذا (ngx_strcmp(rl[i].name, نښه) == 0) {
            پروګرام->ins[rl[i].دننه_واټن].src_reg = 1;
            پروګرام->ins[rl[i].دننه_واټن].imm = fd;
        }
    }
}


صحيح
ngx_bpf_بارول_پروګرام(ngx_log_t *لوګاریتم, ngx_bpf_پروګرام_t *پروګرام)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;
#إذا (NGX_DEBUG)
    حرف            buf[NGX_BPF_LOGBUF_اندازه];
#د_که_پای

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.license = (uintptr_t) پروګرام->license;
    attr.prog_type = پروګرام->type;
    attr.insns = (uintptr_t) پروګرام->ins;
    attr.insn_cnt = پروګرام->nins;

#إذا (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_اندازه = NGX_BPF_LOGBUF_اندازه;
    attr.log_level = 1;
#د_که_پای

    fd = ngx_bpf(BPF_PROG_بارول, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوګاریتم, 0,
                       "bpf verifier: %s", buf);

        أرجع -1;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_create(ngx_log_t *لوګاریتم, تعداد bpf_map_type type, صحيح کلی_اندازه,
    صحيح ارزښت_اندازه, صحيح max_entries, uint32_t map_flags)
{
    صحيح             fd;
    اتحاد bpf_attr  attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_type = type;
    attr.کلی_اندازه = کلی_اندازه;
    attr.ارزښت_اندازه = ارزښت_اندازه;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, حجم(attr));
    إذا (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      "failed to create BPF map");
        أرجع NGX_ERROR;
    }

    أرجع fd;
}


صحيح
ngx_bpf_map_update(صحيح fd, ثابت فراغ *کلی, ثابت فراغ *ارزښت, uint64_t flags)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلی = (uintptr_t) کلی;
    attr.ارزښت = (uintptr_t) ارزښت;
    attr.flags = flags;

    أرجع ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_delete(صحيح fd, ثابت فراغ *کلی)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلی = (uintptr_t) کلی;

    أرجع ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, حجم(attr));
}


صحيح
ngx_bpf_map_lookup(صحيح fd, ثابت فراغ *کلی, فراغ *ارزښت)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلی = (uintptr_t) کلی;
    attr.ارزښت = (uintptr_t) ارزښت;

    أرجع ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, حجم(attr));
}
