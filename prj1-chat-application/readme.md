# Chat Application – Remote Message Exchange

## 📌 Mục tiêu
Ứng dụng chat mạng ngang hàng (peer-to-peer) sử dụng TCP sockets. Mỗi tiến trình sẽ vừa đóng vai trò **client** và **server** cho việc kết nối, gửi và nhận tin nhắn.

---

## 🧰 Yêu cầu hệ thống

- Hệ điều hành: Linux/macOS (khuyên dùng Linux)
- Trình biên dịch: GCC
- Công cụ build: `make` (có `Makefile` đi kèm)

---

## 🚀 Cài đặt và chạy ứng dụng

### 1. Biên dịch chương trình

```bash
make
```

### 2. Chạy chương trình

```bash
./chat <port_number>
```

Ví dụ:

```bash
./chat 4322
```

---

## 💻 Tính năng hỗ trợ

| Lệnh                             | Chức năng                                     |
| -------------------------------- | --------------------------------------------- |
| `help`                           | Hiển thị hướng dẫn sử dụng                    |
| `myip`                           | In ra IP thật của máy (không phải 127.0.0.1)  |
| `myport`                         | In ra cổng chương trình đang lắng nghe        |
| `connect <destination> <port>`   | Kết nối TCP đến một peer mới                  |
| `list`                           | Hiển thị danh sách các kết nối đang hoạt động |
| `terminate <connection_id>`      | Ngắt kết nối đã chỉ định                      |
| `send <connection_id> <message>` | Gửi tin nhắn đến peer                         |
| `exit`                           | Đóng tất cả kết nối và thoát chương trình     |

## 🎬 Video demo

[▶️ Xem video demo](https://youtu.be/-Kl-7nswT-4)

---

## 📄 Documents

| Tên tài liệu | Mô tả |
|--------------|------|
| [Requirements (PDF)](docs/chat_application_requirements.pdf) | Mô tả yêu cầu dự án dưới dạng PDF |
| [Requirements (DOCX)](docs/chat_application_requirements.docx) | Mô tả yêu cầu dự án dưới dạng Word |
