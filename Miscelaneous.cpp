#include "Miscelaneous.h"

#define AUDIO_CODEC_PCM "audio/pcm"

qreal computePeakValue(const QAudioFormat &format)
{
    if (!format.isValid())
    {
        return qreal(0);
    }

    if (format.codec() != AUDIO_CODEC_PCM)
    {
        return qreal(0);
    }

    const qint32 sampleSize = format.sampleSize();

    switch (format.sampleType())
    {
    case QAudioFormat::Unknown:
    {
        break;
    }

    case QAudioFormat::Float:
    {
        if (sampleSize != 32)
        {
            return qreal(0);
        }

        return qreal(1.00003);
    }

    case QAudioFormat::SignedInt:
    {
        if (sampleSize == 32)
        {
            return qreal(INT_MAX);
        }

        if (sampleSize == 16)
        {
            return qreal(SHRT_MAX);
        }

        if (sampleSize == 8)
        {
            return qreal(CHAR_MAX);
        }

        break;
    }

    case QAudioFormat::UnSignedInt:
    {
        if (sampleSize == 32)
        {
            return qreal(UINT_MAX);
        }

        if (sampleSize == 16)
        {
            return qreal(USHRT_MAX);
        }

        if (sampleSize == 8)
        {
            return qreal(UCHAR_MAX);
        }

        break;
    }

    default: break;
    }

    return qreal(0);
}

template<typename T>
static QVector<qreal> computeBufferLevels(const T* buffer, qint32 frameCount, qint32 channelCount)
{
    QVector<qreal> maxValues;

    maxValues.fill(0, channelCount);

    for (qint32 i = 0; i < frameCount; ++i)
    {
        for (qint32 j = 0; j < channelCount; ++j)
        {
            const qint32 index = ((i * channelCount) + j);

            const qreal value = qAbs(qreal(buffer[index]));

            if (value > maxValues[j])
            {
                maxValues.replace(j, value);
            }
        }
    }

    return maxValues;
}

QVector<qreal> computeAudioBufferLevels(const QAudioBuffer &buffer)
{
    QVector<qreal> values;

    if (!buffer.isValid())
    {
        return values;
    }

    const qint32                   frameCount  = buffer.frameCount();
    const QAudioFormat             audioFormat = buffer.format();
    const qint32                   sampleSize  = audioFormat.sampleSize();
    const QAudioFormat::SampleType sampleType  = audioFormat.sampleType();

    if (!audioFormat.isValid() || audioFormat.byteOrder() != QAudioFormat::LittleEndian)
    {
        return values;
    }

    if (audioFormat.codec() != AUDIO_CODEC_PCM)
    {
        return values;
    }

    const qint32 channelCount = audioFormat.channelCount();

    values.fill(0, channelCount);

    const qreal peakValue = computePeakValue(audioFormat);

    if (qFuzzyCompare(peakValue, qreal(0)))
    {
        return values;
    }

    switch (sampleType)
    {
    case QAudioFormat::Unknown:
    case QAudioFormat::Float:
    {
        if (sampleSize == 32)
        {
            values = computeBufferLevels(buffer.constData<float>(), frameCount, channelCount);

            for (qint32 i = 0; i < values.size(); ++i)
            {
                values[i] /= peakValue;
            }
        }

        break;
    }

    case QAudioFormat::SignedInt:
    {
        if (sampleSize == 32)
        {
            values = computeBufferLevels(buffer.constData<qint32>(), frameCount, channelCount);
        }
        else if (sampleSize == 16)
        {
            values = computeBufferLevels(buffer.constData<qint16>(), frameCount, channelCount);
        }
        else if (sampleSize == 8)
        {
            values = computeBufferLevels(buffer.constData<qint8>(), frameCount, channelCount);
        }

        for (qint32 i = 0; i < values.size(); ++i)
        {
            values[i] /= peakValue;
        }

        break;
    }

    case QAudioFormat::UnSignedInt:
    {
        if (sampleSize == 32)
        {
            values = computeBufferLevels(buffer.constData<quint32>(), frameCount, channelCount);
        }
        else if (sampleSize == 16)
        {
            values = computeBufferLevels(buffer.constData<quint16>(), frameCount, channelCount);
        }
        else if (sampleSize == 8)
        {
            values = computeBufferLevels(buffer.constData<quint8>(), frameCount, channelCount);
        }

        for (qint32 i = 0; i < values.size(); ++i)
        {
            values[i] = qAbs(values[i] - (peakValue / 2.0)) / (peakValue / 2.0);
        }

        break;
    }

    default: break;
    }

    return values;
}



































































