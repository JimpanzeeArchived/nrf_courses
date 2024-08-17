#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    while(1) {
        printk("Hello World!\n\r");
        k_msleep(1000);
    }
}

/*prj.conf is left empty because console drivers
needed by the printk statement are enabled by default 
by the baord configuration file.*/

