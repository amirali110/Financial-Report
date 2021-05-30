QT       += core gui
QT      +=charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    all_trade_reports.cpp \
    main.cpp \
    mainwindow.cpp \
    read_file.cpp \
    trade_report.cpp \
    write_file.cpp

HEADERS += \
    all_trade_reports.h \
    mainwindow.h \
    read_file.h \
    trade_report.h \
    write_file.h

FORMS += \
    all_trade_reports.ui \
    mainwindow.ui \
    trade_report.ui

TRANSLATIONS += \
    Financial_Report_v1_fa_IR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    C:/Users/Hojati/Desktop/HiCalendar-main/HiCalendar.pro
