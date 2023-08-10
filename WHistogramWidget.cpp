#include "WHistogramWidget.h"

#include "WAudioLevel.h"
#include "Miscelaneous.h"

#include <QHBoxLayout>
#include <QPainter>

WHistogramWidget::WHistogramWidget(QWidget *parent)
    : QWidget      (parent)
    , m_levelCount (128)
    , mb_busy      (false)
{
    m_frameProcessor.moveToThread(&m_processorThread);

    qRegisterMetaType<QVector<qreal>>("QVector<qreal>");

    connect(&m_frameProcessor, &WFrameProcessor::histogramReady, this, &WHistogramWidget::setHistogram);

    m_processorThread.start(QThread::LowestPriority);

    this->setLayout(new QHBoxLayout());
}

WHistogramWidget::~WHistogramWidget()
{
    m_processorThread.quit();
    m_processorThread.wait(QDeadlineTimer(QDeadlineTimer::Forever));
}

void WHistogramWidget::setLevelCount(qint32 levelCount)
{
    m_levelCount = levelCount;
}

void WHistogramWidget::processVideoFrame(const QVideoFrame &videoFrame)
{
    if (mb_busy && videoFrame.isValid())
    {
        // Drop frame.

        return;
    }

    mb_busy = true;

    QMetaObject::invokeMethod(&m_frameProcessor, "processFrame", Qt::QueuedConnection, Q_ARG(QVideoFrame, videoFrame), Q_ARG(qint32, m_levelCount));
}

void WHistogramWidget::processAudioBuffer(const QAudioBuffer &audioBuffer)
{
    const qint32 channelCount = audioBuffer.format().channelCount();

    if (m_audioLevels.count() != channelCount)
    {
        qDeleteAll(m_audioLevels);

        m_audioLevels.clear();

        for (qint32 i = 0; i < channelCount; ++i)
        {
            WAudioLevel* audioLevel = new WAudioLevel(this);

            m_audioLevels.append(audioLevel);

            this->layout()->addWidget(audioLevel);
        }
    }

    QVector<qreal> audioLevels = computeAudioBufferLevels(audioBuffer);

    for (qint32 i = 0; i < audioLevels.count(); ++i)
    {
        m_audioLevels[i]->setLevel(audioLevels[i]);
    }
}

void WHistogramWidget::setHistogram(const QVector<qreal> &histogram)
{
    mb_busy = false;

    m_histogram = histogram;

    this->update();
}

void WHistogramWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!m_audioLevels.isEmpty())
    {
        return;
    }

    const qint32 width  = this->width();
    const qint32 height = this->height();

    QPainter painter(this);

    if (m_histogram.isEmpty())
    {
        painter.fillRect(0, 0, width, height, Qt::black);

        return;
    }

    const qreal barWidth = width / (qreal)m_histogram.size();

    for (qint32 i = 0; i < m_histogram.size(); ++i)
    {
        const qreal barHeight = m_histogram[i] * height;

        // Draw level.

        painter.fillRect(barWidth * i, height - barHeight, barWidth * (i + 1), height, Qt::red);

        // Clear the rest of the control.

        painter.fillRect(barWidth * i, 0, barWidth * (i + 1), height - barHeight, Qt::black);
    }
}


























































