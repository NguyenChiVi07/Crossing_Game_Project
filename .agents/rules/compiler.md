---
trigger: always_on
---

# Cấu hình Trình biên dịch C/C++
- Khi biên dịch, chạy lệnh terminal hoặc kiểm thử code, luôn sử dụng `gcc-15` và `g++-15` (GNU GCC từ Homebrew).
- Không tự động sử dụng `clang` hoặc `gcc` mặc định của hệ thống macOS.
- Nếu tạo Makefile hoặc CMake, hãy thiết lập rõ:
- CC = gcc-15
- CXX = g++-15