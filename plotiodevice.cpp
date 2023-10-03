#include "plotiodevice.h"

PlotIODevice::PlotIODevice(QLineSeries *lseries, QSurface3DSeries *sseries,
                           QObject *parent)
    : QIODevice{parent}
    , m_lineSeries{lseries}
    , m_surfaceSeries{sseries}
{
}

qint64 PlotIODevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

qint64 PlotIODevice::writeData(const char *data, qint64 maxSize)
{
    if (m_buffer.isEmpty()) {
        m_buffer.reserve(sampleCount);
    }

    // for each n_fft-length chunk of data
    //   index = m_buffer.size()
    //   time = sample rate
    //   do an fft
    //   convert amplitude to power
    //   convert power to DB
    //   store latest DB in m_lineSeries (freq, DB) (or...)
    //   store latest DB in m_surfaceSeries (time, freq, DB)
    return 1;
}
