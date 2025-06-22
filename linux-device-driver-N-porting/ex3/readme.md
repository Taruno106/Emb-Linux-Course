character device file 
app-chdev-driver


kernel-module-linux
1. 01-KM-hello-world
    exam.c

2. 02-KM-app-chdev-driver
   userspace:
        App.c
   kernelspace:
        character-device-driver.c

3. 03-KM-legacy-gpio-driver
    legacy-gpio.c

4. 04-KM-subsystem-gpio-driver
    subsystem-gpio.c

5. 05-KM-full-app-chdev-N-gpio-driver
    userspace:
        App.c
    kernelspace:
        full-app-chdev-N-gpio.c



kernel-module-yocto
    readme.md
        build App.c --> recipe
        build chdev-gpio.c --> .ko
    add video
