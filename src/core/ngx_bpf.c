
/*
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>

#నిర్వచించు NGX_BPF_LOGBUF_కొలత  (32 * 1024)


స్థిర ngx_inline పూర్ణాంకం
ngx_bpf(గణన bpf_cmd cmd, సంఘం bpf_attr *attr, గుర్తులేని పూర్ణాంకం కొలత)
{
    తిరిగి syscall(__NR_bpf, cmd, attr, కొలత);
}


శూన్య
ngx_bpf_కార్యక్రమం_link(ngx_bpf_కార్యక్రమం_t *కార్యక్రమం, స్థిరరాశి అక్షరం *చిహ్నం, పూర్ణాంకం fd)
{
    ngx_uint_t        i;
    ngx_bpf_reloc_t  *rl;

    rl = కార్యక్రమం->relocs;

    కొరకు (i = 0; i < కార్యక్రమం->nrelocs; i++) {
        ఒకవేళ (ngx_strcmp(rl[i].name, చిహ్నం) == 0) {
            కార్యక్రమం->ins[rl[i].స్థానభ్రంశం].src_reg = 1;
            కార్యక్రమం->ins[rl[i].స్థానభ్రంశం].imm = fd;
        }
    }
}


పూర్ణాంకం
ngx_bpf_ఎక్కించు_కార్యక్రమం(ngx_log_t *సంవర్గమానం, ngx_bpf_కార్యక్రమం_t *కార్యక్రమం)
{
    పూర్ణాంకం             fd;
    సంఘం bpf_attr  attr;
#ఒకవేళ (NGX_DEBUG)
    అక్షరం            buf[NGX_BPF_LOGBUF_కొలత];
#అయితే_ముగింపు

    ngx_memzero(&attr, పరిమాణం(సంఘం bpf_attr));

    attr.license = (uintptr_t) కార్యక్రమం->license;
    attr.prog_type = కార్యక్రమం->type;
    attr.insns = (uintptr_t) కార్యక్రమం->ins;
    attr.insn_cnt = కార్యక్రమం->nins;

#ఒకవేళ (NGX_DEBUG)
    /* for verifier errors */
    attr.log_buf = (uintptr_t) buf;
    attr.log_కొలత = NGX_BPF_LOGBUF_కొలత;
    attr.log_level = 1;
#అయితే_ముగింపు

    fd = ngx_bpf(BPF_PROG_ఎక్కించు, &attr, పరిమాణం(attr));
    ఒకవేళ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      "failed to load BPF program");

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0,
                       "bpf verifier: %s", buf);

        తిరిగి -1;
    }

    తిరిగి fd;
}


పూర్ణాంకం
ngx_bpf_map_create(ngx_log_t *సంవర్గమానం, గణన bpf_map_type type, పూర్ణాంకం కీలకం_కొలత,
    పూర్ణాంకం విలువ_కొలత, పూర్ణాంకం max_entries, uint32_t map_flags)
{
    పూర్ణాంకం             fd;
    సంఘం bpf_attr  attr;

    ngx_memzero(&attr, పరిమాణం(సంఘం bpf_attr));

    attr.map_type = type;
    attr.కీలకం_కొలత = కీలకం_కొలత;
    attr.విలువ_కొలత = విలువ_కొలత;
    attr.max_entries = max_entries;
    attr.map_flags = map_flags;

    fd = ngx_bpf(BPF_MAP_CREATE, &attr, పరిమాణం(attr));
    ఒకవేళ (fd < 0) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      "failed to create BPF map");
        తిరిగి NGX_ERROR;
    }

    తిరిగి fd;
}


పూర్ణాంకం
ngx_bpf_map_update(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం, స్థిరరాశి శూన్య *విలువ, uint64_t flags)
{
    సంఘం bpf_attr attr;

    ngx_memzero(&attr, పరిమాణం(సంఘం bpf_attr));

    attr.map_fd = fd;
    attr.కీలకం = (uintptr_t) కీలకం;
    attr.విలువ = (uintptr_t) విలువ;
    attr.flags = flags;

    తిరిగి ngx_bpf(BPF_MAP_UPDATE_ELEM, &attr, పరిమాణం(attr));
}


పూర్ణాంకం
ngx_bpf_map_delete(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం)
{
    సంఘం bpf_attr attr;

    ngx_memzero(&attr, పరిమాణం(సంఘం bpf_attr));

    attr.map_fd = fd;
    attr.కీలకం = (uintptr_t) కీలకం;

    తిరిగి ngx_bpf(BPF_MAP_DELETE_ELEM, &attr, పరిమాణం(attr));
}


పూర్ణాంకం
ngx_bpf_map_lookup(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం, శూన్య *విలువ)
{
    సంఘం bpf_attr attr;

    ngx_memzero(&attr, పరిమాణం(సంఘం bpf_attr));

    attr.map_fd = fd;
    attr.కీలకం = (uintptr_t) కీలకం;
    attr.విలువ = (uintptr_t) విలువ;

    తిరిగి ngx_bpf(BPF_MAP_LOOKUP_ELEM, &attr, పరిమాణం(attr));
}
