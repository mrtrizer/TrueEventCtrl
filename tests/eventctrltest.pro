#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T17:58:12
#
#-------------------------------------------------

QT       += core testlib
QT       -= gui

include("../src/eventctrl.pri")

TARGET = TrueMsgCtrl
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += MONO_THREAD

TEMPLATE = app

INCLUDEPATH += "../src/"

SOURCES += main.cpp

