# 📝 Bài tập 3 — Character device + GPIO Driver

Tạo một character device file và một ứng dụng ở userspace.

* Khi ứng dụng App (tầng userspace) ghi "on" vào device file, kernel sẽ in ra "Turn LED on" và cấu hình GPIO27 lên mức high.
* Khi App ghi "off", kernel sẽ in ra "Turn LED off" và cấu hình GPIO27 xuống mức low.

---

### 05-KM-full-app-chdev-N-gpio-driver
├── kernelspace
│   ├── full-app-chdev-N-gpio.c
│   └── Makefile
├── readme.md
└── userspace
    ├── App.c
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

### 🛠️ Guide
    build App.c --> .exe
    build full-app-chdev-N-gpio.c --> .ko --> insmod into kernel
    (Tương tự readme.md của 02-KM-app-chdev-driver)
