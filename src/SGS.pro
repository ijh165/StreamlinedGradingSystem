#--------------------------------------------------
#
# Project created by QtCreator 2015-07-08 T23:01:29
#
#--------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StreamlinedGradingSystem
TEMPLATE = app

SOURCES += main.cpp\
    streamlinedgradingsystem.cpp \
    Control/gui_interaction.cpp \
    Model/CLASSES/academicadmin.cpp \
    Model/CLASSES/account.cpp \
    Model/CLASSES/activity.cpp \
    Model/CLASSES/adminassist.cpp \
    Model/CLASSES/course.cpp \
    Model/CLASSES/instructor.cpp \
    Model/CLASSES/marker.cpp \
    Model/CLASSES/sysadmin.cpp \
    Model/CLASSES/rubric.cpp \
    Model/CLASSES/testcase.cpp \
    Model/DatabaseAPI/dbmanager.cpp

HEADERS  += \
    streamlinedgradingsystem.h \
    Model/CLASSES/academicadmin.h \
    Model/CLASSES/account.h \
    Model/CLASSES/activity.h \
    Model/CLASSES/adminassist.h \
    Model/CLASSES/course.h \
    Model/CLASSES/instructor.h \
    Model/CLASSES/marker.h \
    Model/CLASSES/sysadmin.h \
    Model/CLASSES/rubric.h \
    Model/CLASSES/testcase.h \
    Model/DatabaseAPI/dbManager.h

FORMS    += streamlinedgradingsystem.ui
