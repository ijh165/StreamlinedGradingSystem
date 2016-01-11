#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T23:01:29
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SGSr10
TEMPLATE = app


SOURCES += main.cpp\
    streamlinedgradingsystem.cpp \
    Control/GUIControl/gui_interaction.cpp \
    Model/CLASSES/academicadmin.cpp \
    Model/CLASSES/account.cpp \
    Model/CLASSES/activity.cpp \
    Model/CLASSES/adminassist.cpp \
    Model/CLASSES/course.cpp \
    Model/CLASSES/instructor.cpp \
    Model/CLASSES/marker.cpp \
    Model/CLASSES/sysadmin.cpp \
    Control/DatabaseAPI/dbmanager.cpp \
    Model/CLASSES/rubric.cpp \
    Model/CLASSES/testcase.cpp

HEADERS  += \
    Model/CLASSES/academicadmin.h \
    Model/CLASSES/account.h \
    Model/CLASSES/activity.h \
    Model/CLASSES/adminassist.h \
    Model/CLASSES/course.h \
    Model/CLASSES/instructor.h \
    Model/CLASSES/marker.h \
    Model/CLASSES/sysadmin.h \
    Control/DatabaseAPI/dbmanager.h \
    Model/CLASSES/rubric.h \
    Model/CLASSES/rubric.h \
    Model/CLASSES/testcase.h \
    streamlinedgradingsystem.h

FORMS    += streamlinedgradingsystem.ui
