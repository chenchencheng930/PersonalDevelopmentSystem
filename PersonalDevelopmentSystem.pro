#-------------------------------------------------
#
# Project created by QtCreator 2026-04-02T14:35:33
#
#-------------------------------------------------

QT += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PersonalDevelopmentSystem
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    coursepage.cpp \
    experiencepage.cpp \
    awardpage.cpp \
    databasemanager.cpp \
    gpacalculator.cpp

HEADERS += \
    mainwindow.h \
    coursepage.h \
    experiencepage.h \
    awardpage.h \
    databasemanager.h \
    gpacalculator.h

FORMS += \
    mainwindow.ui \
    experiencepage.ui \
    awardpage.ui


# 编译器默认配置
CONFIG += console
CONFIG -= app_bundle



