#include "WAudioLevel.h"

#include <QPainter>

WAudioLevel::WAudioLevel(QWidget *parent)
    : QWidget (parent)
    , m_level (0)
{
    this->setMinimumHeight(15);
    this->setMaximumHeight(50);
}

WAudioLevel::~WAudioLevel()
{
}

void WAudioLevel::setLevel(qreal level)
{
    if (m_level != level)
    {
        m_level = level;

        this->update();
    }
}

void WAudioLevel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const qint32 width  = this->width();
    const qint32 height = this->height();

    QPainter painter(this);

    // Draw level.

    const qreal widthLevel = m_level * width;

    painter.fillRect(0, 0, widthLevel, height, Qt::red);

    // Clear the rest of the control.

    painter.fillRect(widthLevel, 0, width, height, Qt::black);
}
