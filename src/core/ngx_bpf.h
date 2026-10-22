
/*
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_BPF_H_INCLUDED_
#నిర్వచించు _NGX_BPF_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>

#అన్తర్భావ <linux/bpf.h>


రకనిర్వచనం నిర్మాణం {
    అక్షరం                *name;
    పూర్ణాంకం                  స్థానభ్రంశం;
} ngx_bpf_reloc_t;

రకనిర్వచనం నిర్మాణం {
    అక్షరం                *license;
    గణన bpf_prog_type   type;
    నిర్మాణం bpf_insn     *ins;
    కొలత_t               nins;
    ngx_bpf_reloc_t     *relocs;
    కొలత_t               nrelocs;
} ngx_bpf_కార్యక్రమం_t;


శూన్య ngx_bpf_కార్యక్రమం_link(ngx_bpf_కార్యక్రమం_t *కార్యక్రమం, స్థిరరాశి అక్షరం *చిహ్నం,
    పూర్ణాంకం fd);
పూర్ణాంకం ngx_bpf_ఎక్కించు_కార్యక్రమం(ngx_log_t *సంవర్గమానం, ngx_bpf_కార్యక్రమం_t *కార్యక్రమం);

పూర్ణాంకం ngx_bpf_map_create(ngx_log_t *సంవర్గమానం, గణన bpf_map_type type, పూర్ణాంకం కీలకం_కొలత,
    పూర్ణాంకం విలువ_కొలత, పూర్ణాంకం max_entries, uint32_t map_flags);
పూర్ణాంకం ngx_bpf_map_update(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం, స్థిరరాశి శూన్య *విలువ,
    uint64_t flags);
పూర్ణాంకం ngx_bpf_map_delete(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం);
పూర్ణాంకం ngx_bpf_map_lookup(పూర్ణాంకం fd, స్థిరరాశి శూన్య *కీలకం, శూన్య *విలువ);

#అయితే_ముగింపు /* _NGX_BPF_H_INCLUDED_ */
