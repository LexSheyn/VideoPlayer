#ifndef WVIDEOWIDGET_H
#define WVIDEOWIDGET_H

#include <QVideoWidget>

class WVideoWidget : public QVideoWidget
{
    Q_OBJECT;

public:

    explicit WVideoWidget(QWidget* parent = nullptr);
    virtual ~WVideoWidget() override;

protected:

    // QWidget.

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
};

#endif // WVIDEOWIDGET_H
