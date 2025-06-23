# 📜 Linux Kernel Module Exercises for Raspberry Pi 4 (64-bit)

This repository contains hands-on exercises on writing, inserting/removing Linux kernel modules and interacting with GPIOs on a RPi 4 64-bit.


### Insert and remove module:

   ```sh
   sudo insmod exam.ko       # Insert module into the kernel
   sudo rmmod exam           # Remove module from the kernel
   sudo dmesg -w             # View kernel logs
   ```

---

## 📝 Exercises

### Bài tập 1:
Tạo một character device file và một ứng dụng ở userspace.
* Khi ứng dụng App (tầng userspace) ghi "on" vào device file, kernel sẽ in ra "Turn LED on".
* Khi App ghi "off", kernel sẽ in ra "Turn LED off".
`Giải ở 02-KM-app-chdev-driver`


### Bài tập 2: 
Cấu hình điều chân GPIO27 của RPI 4 64bit thông qua việc truy cập vào các thanh ghi. 
`Giải ở 03-KM-legacy-gpio-driver (đọc/ghi thanh ghi) hoặc 04-KM-subsystem-gpio-driver (dùng subsystem)`


### Bài tập 3:
Tạo một character device file và một ứng dụng ở userspace.
* Khi ứng dụng App (tầng userspace) ghi "on" vào device file, kernel sẽ in ra "Turn LED on" và cấu hình GPIO27 lên mức high.
* Khi App ghi "off", kernel sẽ in ra "Turn LED off" và cấu hình GPIO27 xuống mức low.
`Giải ở 05-KM-full-app-chdev-N-gpio-driver`

---

```shell
### kernel-module-linux
├── 01-KM-hello-world
│   ├── exam.c
│   └── Makefile
├── 02-KM-app-chdev-driver
│   ├── images
│   │   ├── chdev_insert_write.jpg
│   │   └── chdev_remove.jpg
│   ├── kernelspace
│   │   ├── character-device-driver.c
│   │   └── Makefile
│   ├── readme.md
│   └── userspace
│       ├── App.c
│       └── Makefile
├── 03-KM-legacy-gpio-driver
│   ├── legacy-gpio.c
│   └── Makefile
├── 04-KM-subsystem-gpio-driver
│   ├── 01-integer-based
│   │   ├── gpio.c
│   │   └── Makefile
│   └── 02-descriptor-based
│       ├── gpio.c
│       └── Makefile
├── 05-KM-full-app-chdev-N-gpio-driver
│   ├── kernelspace
│   │   ├── full-app-chdev-N-gpio.c
│   │   └── Makefile
│   ├── readme.md
│   └── userspace
│       ├── App.c
│       └── Makefile
└── readme.md
```