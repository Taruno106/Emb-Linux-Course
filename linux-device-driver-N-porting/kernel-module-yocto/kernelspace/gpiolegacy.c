#include <linux/cdev.h>    /* Define cdev_init(), cdev_add() */
#include <linux/device.h>  /* Define device_create(), class_create() */
#include <linux/fs.h>      /* Define alloc_chrdev_region(), register_chrdev_region() */
#include <linux/module.h>  /* Define module_init(), module_exit() */
#include <linux/slab.h>    /* Define kfree() */
#include <linux/uaccess.h> /* Define copy_to_user(), copy_from_user() */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>

// character device
#define NPAGES 1

#define DRIVER_AUTHOR "pmtoan pmtoancd@gmail.com"
#define DRIVER_DESC "Kernel module: Low-level GPIO access to clear GPIO27 on Raspberry Pi 4"
#define DRIVER_VERS "1.0"

struct m_chdev {
    int32_t size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} m_dev;

// driver gpio
#define GPIO_BASE_PHYS 0xFE200000 // BCM2711 physical GPIO base
#define GPIO_SIZE 0x30            // Enough to access up to GPCLR0

#define GPIO_PIN 27
#define HIGH 1
#define LOW 0

static void __iomem *gpio_base;

/* Function Prototypes */
static int __init chdev_init(void);
static void __exit chdev_exit(void);
static int driver_gpio_module(int status);
static int m_open(struct inode *inode, struct file *file);
static int m_release(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *file, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t m_write(struct file *file, const char *user_buf, size_t size, loff_t *offset);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = m_read,
    .write = m_write,
    .open = m_open,
    .release = m_release,
};

static int driver_gpio_module(int status) {
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
    if (status) {
        writel(1 << GPIO_PIN, gpio_base + 0x1C); // Set GPIO27 = set HIGH (in GPSET0: offset 0x1c)
        pr_info("GPIO27 set to output and SET (HIGH)\n");
    } else {
        writel(1 << GPIO_PIN, gpio_base + 0x28); // Clear GPIO27 = set LOW (in GPCLR0: offset 0x28)
        pr_info("GPIO27 set to output and cleared (LOW)\n");
    }

    return 0;
}

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file) {
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file) {
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *file, char __user *user_buf, size_t size, loff_t *offset) {
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_read = (size > m_dev.size - *offset) ? (m_dev.size - *offset) : size;

    /* Copy from mapped area to user buffer */
    if (copy_to_user(user_buf, m_dev.kmalloc_ptr + *offset, to_read))
        return -EFAULT;

    *offset += to_read;

    return to_read;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *file, const char *user_buf, size_t size, loff_t *offset) {
    size_t to_write;

    pr_info("System call write() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_write = (size + *offset > NPAGES * PAGE_SIZE) ? (NPAGES * PAGE_SIZE - *offset) : size;

    /* Copy from user buffer to mapped area */
    memset(m_dev.kmalloc_ptr, 0, NPAGES * PAGE_SIZE);
    if (copy_from_user(m_dev.kmalloc_ptr + *offset, user_buf, to_write) != 0)
        return -EFAULT;

    // Bỏ newline nếu có
    if (m_dev.kmalloc_ptr[to_write - 1] == '\n')
        m_dev.kmalloc_ptr[to_write - 1] = '\0';

    pr_info("Data from usr: %s", m_dev.kmalloc_ptr);

    if (strcmp(m_dev.kmalloc_ptr, "on") == 0) {
        pr_info("Turned on LED\n");
        driver_gpio_module(HIGH);
    } else if (strcmp(m_dev.kmalloc_ptr, "off") == 0) {
        pr_info("Turned off LED\n");
        driver_gpio_module(LOW);
    }

    *offset += to_write;
    m_dev.size = *offset;

    return to_write;
}

static int __init chdev_init(void) {
    /* 1.0 Dynamic allocating device number (cat /proc/devices) */
    if (alloc_chrdev_region(&m_dev.dev_num, 0, 1, "m_character_device") < 0) {
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(m_dev.dev_num), MINOR(m_dev.dev_num));

    /* 2.0 Creating struct class */
    if ((m_dev.m_class = class_create(THIS_MODULE, "m_class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    /* 3.0 Creating device */
    if ((device_create(m_dev.m_class, NULL, m_dev.dev_num, NULL, "m_character_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }

    /* 4.0 Creating cdev structure */
    cdev_init(&m_dev.m_cdev, &fops);

    /* 4.1 Adding character device to the system */
    if ((cdev_add(&m_dev.m_cdev, m_dev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device;
    }

    // /* 5.0 Allocate kernel buffer */
    m_dev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL);
    if (!m_dev.kmalloc_ptr) {
        pr_err("Cannot allocate memory\n");
        goto rm_device;
    }

    pr_info("Hello world kernel module\n");
    return 0;

rm_device:
    device_destroy(m_dev.m_class, m_dev.dev_num);
rm_class:
    class_destroy(m_dev.m_class);
rm_device_numb:
    unregister_chrdev_region(m_dev.dev_num, 1);
    return -1;
}

static void __exit chdev_exit(void) {
    iounmap(gpio_base); // unmap gpio_base
    kfree(m_dev.kmalloc_ptr);                     /* 5.0 */
    cdev_del(&m_dev.m_cdev);                      /* 4.0 */
    device_destroy(m_dev.m_class, m_dev.dev_num); /* 3.0 */
    class_destroy(m_dev.m_class);                 /* 2.0 */
    unregister_chrdev_region(m_dev.dev_num, 1);   /* 1.0 */
    pr_info("Goodbye\n");
}

module_init(chdev_init);
module_exit(chdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
