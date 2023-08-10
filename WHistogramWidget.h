#ifndef WHISTOGRAMWIDGET_H
#define WHISTOGRAMWIDGET_H

#include "WFrameProcessor.h"

#include <QWidget>

class WAudioLevel;

class WHistogramWidget : public QWidget
{
    Q_OBJECT;

public:

    explicit WHistogramWidget(QWidget* parent = nullptr);
    virtual ~WHistogramWidget() override;

    void setLevelCount(qint32 levelCount);

public slots:

    void processVideoFrame(const QVideoFrame& videoFrame);
    void processAudioBuffer(const QAudioBuffer& audioBuffer);

    void setHistogram(const QVector<qreal>& histogram);

protected:

    // QWidget.

    virtual void paintEvent(QPaintEvent* event) override;

private:

    QVector<qreal>        m_histogram;
    qint32                m_levelCount;
    WFrameProcessor       m_frameProcessor;
    QThread               m_processorThread;
    bool                  mb_busy;
    QVector<WAudioLevel*> m_audioLevels;
};

#endif // WHISTOGRAMWIDGET_H
