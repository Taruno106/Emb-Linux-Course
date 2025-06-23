/* BT2:
    Cấu hình điều chân GPIO27 của RPI 4 64bit (BCM2711) thông qua việc 
    truy cập vào các thanh ghi.
*/

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DRIVER_AUTHOR "pmtoan pmtoancd@gmail.com"
#define DRIVER_DESC   "Low-level GPIO access to GPIO27 on Raspberry Pi 4"
#define DRIVER_VERS   "1.0"

#define GPIO_BASE_PHYS 0xFE200000 // BCM2711 physical GPIO base
#define GPIO_SIZE 0x30            // Enough to access up to GPCLR0

#define GPIO_PIN 27

static void __iomem *gpio_base;

static int __init gpio_module_init(void) {
    u32 val;

    // ioremap physical GPIO address
    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base) {
        pr_err("Failed to ioremap GPIO base\n");
        return -ENOMEM;
    }

    pr_info("GPIO module loaded\n");

    // 1. Set GPIO27 as output (in GPFSEL2: offset 0x08)
    val = readl(gpio_base + 0x08);
    val &= ~(7 << 21); // Clear bits 23:21 (GPIO27)
    val |= (1 << 21);  // Set to 001 (Output)
    writel(val, gpio_base + 0x08);

    // 2. set HIGH/LOW
    writel(1 << GPIO_PIN, gpio_base + 0x28); // Clear GPIO27 = set LOW (in GPCLR0: offset 0x28)
    pr_info("GPIO27 set to output and cleared (LOW)\n");

    // writel(1 << GPIO_PIN, gpio_base + 0x1C); // Set GPIO27 = set HIGH (in GPSET0: offset 0x1c)
    // pr_info("GPIO27 set to output and SET (HIGH)\n");

    return 0;
}

static void __exit gpio_module_exit(void) {
    iounmap(gpio_base);
    pr_info("GPIO module unloaded\n");
}

module_init(gpio_module_init);
module_exit(gpio_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
