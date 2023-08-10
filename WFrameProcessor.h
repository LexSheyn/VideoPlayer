#ifndef WFRAMEPROCESSOR_H
#define WFRAMEPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QVideoFrame>
#include <QAudioBuffer>
#include <QWidget>

class WFrameProcessor : public QObject
{
    Q_OBJECT;

public slots:

    void processFrame(QVideoFrame videoFrame, qint32 levels);

signals:

    void histogramReady(const QVector<qreal>& histogram);
};

#endif // WFRAMEPROCESSOR_H
