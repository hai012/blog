



dma_irq()
{

}






int tun_dma_init(void)
{
    src = dma_alloc_writecombine(NULL, BUF_SIZE, &src_phys, GFP_KERNEL);

    if(NULL == src) {
        printk("can't alloc buffer for src\n");
        return -1;
    }

    dst = dma_alloc_writecombine(NULL, BUF_SIZE, &dst_phys, GFP_KERNEL);
    if(NULL == dst) {
        printk("can't alloc buffer for dst\n");
        goto out_dma_free_writecombine;
    }



    request_irq();


out_dma_free_dst
    dma_free_writecombine(NULL, BUF_SIZE, dst, dst_phys);
out_dma_free_src:
    dma_free_writecombine(NULL, BUF_SIZE, src, src_phys);

}



int tun_dma_exit(void)
{

    dma_free_writecombine(NULL, BUF_SIZE, src, src_phys);
    dma_free_writecombine(NULL, BUF_SIZE, dst, dst_phys);

}