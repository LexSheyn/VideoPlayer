#include "WFrameProcessor.h"

void WFrameProcessor::processFrame(QVideoFrame videoFrame, qint32 levels)
{
    QVector<qreal> histogram(levels);

    if (!levels)
    {
        return;
    }

    if (!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        return;
    }

    if (videoFrame.pixelFormat() == QVideoFrame::Format_YUV420P ||
        videoFrame.pixelFormat() == QVideoFrame::Format_NV12)
    {
        // Process YUV data.

        uchar* bytes = videoFrame.bits();

        for (qint32 y = 0; y < videoFrame.height(); ++y)
        {
            const uchar* lastPixel = bytes + videoFrame.width();

            for (uchar* currentPixel = bytes; currentPixel < lastPixel; currentPixel++)
            {
                const quint32 index = (*currentPixel * levels) >> 8;

                histogram[index] += 1.0;

                bytes += videoFrame.bytesPerLine();
            }
        }
    }
    else
    {
        const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(videoFrame.pixelFormat());

        if (imageFormat != QImage::Format_Invalid)
        {
            // Process RGB data.

            QImage image(videoFrame.bits(), videoFrame.width(), videoFrame.height(), imageFormat);

            image = image.convertToFormat(QImage::Format_RGB32);

            const QRgb* bytes = (const QRgb*)image.bits();

            for (qint32 y = 0; y < image.height(); ++y)
            {
                const QRgb* lastPixel = bytes + videoFrame.width();

                for (const QRgb* currentPixel = bytes; currentPixel < lastPixel; currentPixel++)
                {
                    const quint32 index = (qGray(*currentPixel) * levels) >> 8;

                    histogram[index] += 1.0;

                    bytes = (const QRgb*)((uchar*)bytes + image.bytesPerLine());
                }
            }
        }
    }

    // Find maximum value.

    qreal maxValue = 0.0;

    for (qint32 i = 0; i < histogram.size(); ++ i)
    {
        if (histogram[i] > maxValue)
        {
            maxValue = histogram[i];
        }
    }

    if (maxValue > 0.0)
    {
        for (qint32 i = 0; i < histogram.size(); ++i)
        {
            histogram[i] /= maxValue;
        }
    }

    videoFrame.unmap();

    emit histogramReady(histogram);
}













































