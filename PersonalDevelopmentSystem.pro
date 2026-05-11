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
    gpacalculator.cpp \
    advicepage.cpp

HEADERS += \
    mainwindow.h \
    coursepage.h \
    experiencepage.h \
    awardpage.h \
    databasemanager.h \
    gpacalculator.h \
    advicepage.h

FORMS += \
    mainwindow.ui \
    experiencepage.ui \
    awardpage.ui

CONFIG += console
CONFIG -= app_bundle
