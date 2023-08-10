#ifndef WAUDIOLEVEL_H
#define WAUDIOLEVEL_H

#include <QWidget>

class WAudioLevel : public QWidget
{
    Q_OBJECT;

public:

    explicit WAudioLevel(QWidget* parent = nullptr);
    virtual ~WAudioLevel() override;

    // Using [0; 1.0] range.
    void setLevel(qreal level);

protected:

    // QWidget.

    virtual void paintEvent(QPaintEvent* event) override;

private:

    qreal m_level;
};

#endif // WAUDIOLEVEL_H
