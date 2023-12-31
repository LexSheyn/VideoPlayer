QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Miscelaneous.cpp \
        WAudioLevel.cpp \
        WHistogramWidget.cpp \
        WPlayer.cpp \
        WPlayerControls.cpp \
        WPlaylistModel.cpp \
        WVideoWidget.cpp \
        main.cpp \
        WFrameProcessor.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Miscelaneous.h \
    WAudioLevel.h \
    WFrameProcessor.h \
    WHistogramWidget.h \
    WPlayer.h \
    WPlayerControls.h \
    WPlaylistModel.h \
    WVideoWidget.h
