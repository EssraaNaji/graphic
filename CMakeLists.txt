cmake_minimum_required(VERSION 3.20)
project(untit)  # اسم المشروع كما يظهر في الصورة عندك

set(CMAKE_CXX_STANDARD 17)

# 1. إخبار البرنامج أين يجد ملفات الهيدر (ملفات .h)
# هذا سيحل مشكلة الخط الأحمر
include_directories("libraries/include")

# 2. إخبار البرنامج أين يجد ملفات المكتبة (ملفات .a)
link_directories("libraries/lib/x64")

add_executable(untit main.cpp)

# 3. ربط المكتبات بالمشروع
# freeglut: المكتبة التي حملتها
# opengl32 & glu32: مكتبات أساسية في الويندوز
target_link_libraries(untit freeglut opengl32 glu32)