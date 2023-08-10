#ifndef MISCELANEOUS_H
#define MISCELANEOUS_H

#include <QVector>
#include <QAudioFormat>
#include <QAudioBuffer>

// Only the most common sample formats are supported.
// @return
//      Maximum possible sample value for a given audio format.
qreal computePeakValue(const QAudioFormat& format);

// @return
//      Audio level for each channel.
QVector<qreal> computeAudioBufferLevels(const QAudioBuffer& buffer);

#endif // MISCELANEOUS_H
