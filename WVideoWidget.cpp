#include "WVideoWidget.h"

#include <QMouseEvent>
#include <QKeyEvent>

WVideoWidget::WVideoWidget(QWidget *parent)
    : QVideoWidget (parent)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette palette = this->palette();

    palette.setColor(QPalette::Window, Qt::black);

    this->setPalette(palette);

    this->setAttribute(Qt::WA_OpaquePaintEvent);
}

WVideoWidget::~WVideoWidget()
{
}

void WVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    const bool b_fullScreen = this->isFullScreen();

    this->setFullScreen(!b_fullScreen);

    event->accept();
}

void WVideoWidget::keyReleaseEvent(QKeyEvent *event)
{
    const Qt::Key               key               = (Qt::Key)event->key();
    const Qt::KeyboardModifiers keyboardModifiers = event->modifiers();
    const bool                  b_fullScreen      = this->isFullScreen();

    if (key == Qt::Key_Escape && b_fullScreen)
    {
        this->setFullScreen(false);

        event->accept();
    }
    else if (key == Qt::Key_Enter && (keyboardModifiers & Qt::Key_Alt))
    {
        this->setFullScreen(!b_fullScreen);

        event->accept();
    }
    else
    {
        QVideoWidget::keyReleaseEvent(event);
    }
}

// TO DO:
// File: https://code.qt.io/cgit/qt/qtmultimedia.git/tree/examples/multimediawidgets/player/videowidget.cpp?h=5.15
// Line: 53
