#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/segment.h>

asmlinkage long sys_mycopy (const char *src, const char *dst) {
    struct file *psrc;
    struct file *pdst;
    loff_t read_pos = 0, write_pos = 0;
    int read_bytes_num = 0;
    char buf[100];

    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    psrc = filp_open(src, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (IS_ERR(psrc)) {
        printk("mycopy fail to open file.\n");
        return -1;
    }
    pdst = filp_open(dst, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    read_bytes_num = vfs_read(psrc, buf, 90, &read_pos);
    while (read_bytes_num != 0) {
        vfs_write(pdst, buf, read_bytes_num, &write_pos);
        read_bytes_num = vfs_read(psrc, buf, 90, &read_pos);
    }

    filp_close(psrc, 0);
    filp_close(pdst, 0);

    set_fs(old_fs);

    return 0;
}

