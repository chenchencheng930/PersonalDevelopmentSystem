#-------------------------------------------------
#
# Project created by QtCreator 2026-04-02T14:35:33
#
#-------------------------------------------------

QT += core gui widgets sql

# 目标配置
TARGET = PersonalDevelopmentSystem
TEMPLATE = app

# 禁用废弃 API（兼容 Qt 5/6）
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # 适配 Qt 6.0 及以上

# 源码文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    coursepage.cpp \
    experiencepage.cpp \
    awardpage.cpp \
    advicepage.cpp \
    databasemanager.cpp \
    gpacalculator.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    coursepage.h \
    experiencepage.h \
    awardpage.h \
    advicepage.h \
    databasemanager.h \
    gpacalculator.h

# UI 文件
FORMS += \
    mainwindow.ui \
    experiencepage.ui \
    awardpage.ui

# 编译器默认配置
CONFIG += console
CONFIG -= app_bundle
