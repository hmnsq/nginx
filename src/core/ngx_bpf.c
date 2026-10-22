
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#تعریف_کرو NGX_BPF_LOGBUF_سائز  (32 * 1024)


ساکن ngx_inline صحیح
ngx_bpf(شمار bpf_cmd cmd, اتحاد bpf_attr *attr, غیرعلامتی صحیح سائز)
{
    واپس syscall(__NR_bpf, cmd, attr, سائز);
}


خالی
ngx_bpf_پروگرام_link(ngx_bpf_پروگرام_t *پروگرام, ثابت حرف *علامت, صحیح fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = پروگرام->relocs;

    برائے (i = 0; i < پروگرام->nrelocs; i++) {
        اگر (ngx_strcmp(rl[i].name, علامت) == 0) {
            پروگرام->ins[rl[i].انحراف].src_reg = 1;
            پروگرام->ins[rl[i].انحراف].imm = fd;
        }
    }
}


صحیح
ngx_bpf_بھرو_پروگرام(ngx_log_t *لوگارتھم, ngx_bpf_پروگرام_t *پروگرام)
{
    صحیح             fd;
    اتحاد bpf_attr  attr;
#اگر (NGX_DEBUG)
    حرف            buf[NGX_BPF_LOGBUF_سائز];
#اگر_ختم

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.license = (uintptr_t) پروگرام->license;
    attr.prog_type = پروگرام->type;
    attr.insns = (uintptr_t) پروگرام->ins;
    attr.insn_cnt = پروگرام->nins;

#اگر (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_سائز = NGX_BPF_LOGBUF_سائز;
    attr.log_level = 1;
#اگر_ختم

    fd = ngx_bpf(BPF_PROG_بھرو, &attr, حجم(attr));
    اگر (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "bpf verifier: %s", buf);

        واپس -1;
    }

    واپس fd;
}


صحیح
ngx_bpf_map_create(ngx_log_t *لوگارتھم, شمار bpf_map_type type, صحیح کلید_سائز,
    صحیح قدر_سائز, صحیح max_entries, uint32_t map_flags)
{
    صحیح             fd;
    اتحاد bpf_attr  attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_type = type;
    attr.کلید_سائز = کلید_سائز;
    attr.قدر_سائز = قدر_سائز;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, حجم(attr));
    اگر (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      "failed to create BPF map");
        واپس NGX_ERROR;
    }

    واپس fd;
}


صحیح
ngx_bpf_map_update(صحیح fd, ثابت خالی *کلید, ثابت خالی *قدر, uint64_t flags)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.قدر = (uintptr_t) قدر;
    attr.flags = flags;

    واپس ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, حجم(attr));
}


صحیح
ngx_bpf_map_delete(صحیح fd, ثابت خالی *کلید)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;

    واپس ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, حجم(attr));
}


صحیح
ngx_bpf_map_lookup(صحیح fd, ثابت خالی *کلید, خالی *قدر)
{
    اتحاد bpf_attr attr;

    ngx_memzero(&attr, حجم(اتحاد bpf_attr));

    attr.map_fd = fd;
    attr.کلید = (uintptr_t) کلید;
    attr.قدر = (uintptr_t) قدر;

    واپس ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, حجم(attr));
}
