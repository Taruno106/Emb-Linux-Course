# 📝 Bài tập 3 — Character device + GPIO Driver

Tạo một character device file và một ứng dụng ở userspace.

* Khi ứng dụng App (tầng userspace) ghi "on" vào device file, kernel sẽ in ra "Turn LED on" và cấu hình GPIO27 lên mức high.
* Khi App ghi "off", kernel sẽ in ra "Turn LED off" và cấu hình GPIO27 xuống mức low.

---

### kernel-module-yocto
├── kernelspace
│   ├── gpiolegacy.c
│   └── Makefile
├── readme.md
└── userspace
    ├── app-chdev.c
    └── Makefile

---

### full flow code của bt 3:

userspace <-(1)-> kernelspace <-(2)-> hardware
(1) character device file (bài 02)
(2) gpio driver (bài 03 hoặc 04)

Tầng kernelspace là sự kết hợp của 02-KM-app-chdev-driver và 03-KM-legacy-gpio-driver 
(hoặc 02-KM-app-chdev-driver và 04-KM-subsystem-gpio-driver).
Bài 02 tạo 1 character device file và cho phép tầng userspace (App) gửi/đọc data đến tầng kernelspace,
và character device file này có thể giao tiếp xuống hardware
thông qua việc truy cập vào các thanh ghi (gpio driver) ở bài 03 hoặc dùng subsystem ở bài 04 do linux hỗ trợ.

---

### 🛠️ Build & Run Guide
1. Build `app-chdev.c` thành 1 recipe của Yocto. 
   (các bước làm theo link sau: https://devlinux.vn/blog/T%E1%BA%A1o-layer-%C4%91%C6%A1n-gi%E1%BA%A3n-v%C3%A0-th%C3%AAm-ch%C6%B0%C6%A1ng-tr%C3%ACnh-Helloworld)
2. Build `gpiolegacy.c` thành kernel module `.ko` ở source Yocto.
   (các bước làm theo link sau: https://devlinux.vn/blog/Quy-tr%C3%ACnh-ph%C3%A1t-tri%E1%BB%83n-Kernel-trong-Yocto-Project#heading-8)
3. Copy/scp module `.ko` sang `/home/root` trên board chạy Yocto.
4. chạy `insmod gpiolegacy.ko` và tiến hành chạy app-chdev on/off để bật/tắt LED trên board thực tế.

---

### Result

[▶️ Xem video result](https://youtu.be/Qo96WOx3xU8)
