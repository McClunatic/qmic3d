#include "plotiodevice.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtDataVisualization/QSurfaceDataArray>

#include "fftw3.h"

#include <complex>

PlotIODevice::PlotIODevice(QLineSeries *lseries, QSurface3DSeries *sseries,
                           QObject *parent)
    : QIODevice{parent}
    , m_lineSeries{lseries}
    , m_surfaceSeries{sseries}
{
    auto axes = m_lineSeries->attachedAxes();
    auto xAxis = dynamic_cast<QLogValueAxis*>(axes[0]);
    auto yAxis = dynamic_cast<QValueAxis*>(axes[1]);
    xAxis->setRange(1., sampleRate / 2.);  // 0 to Nyquist frequency, Hz
    yAxis->setRange(0., 60.);  // 0 to 60, dB

    auto array = new QSurfaceDataArray;
    for (int i = 100; i >= 0; --i) {
        auto row = new QSurfaceDataRow;
        float t = 0.f - (float(i) * n_fft) / sampleRate;
        for (int j = 0; j < (n_fft / 2 + 1); ++j) {
            auto x = 1e-10f + (float(j) * sampleRate) / n_fft;
            *row << QVector3D(x, 0.f, t);
        }
        *array << row;
    }
    m_surfaceSeries->dataProxy()->resetArray(array);
}

qint64 PlotIODevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

qint64 PlotIODevice::writeData(const char *data, qint64 maxSize)
{
    using namespace std::complex_literals;

    const float* fdata = reinterpret_cast<const float*>(data);
    const qint64 fsize = maxSize * sizeof(char) / sizeof(float);

    if (m_buffer.capacity() == 0) {
        m_buffer.reserve(n_fft - 1);
        m_in.reserve(n_fft);
        m_out.resize(n_fft / 2 + 1);
    }

    // If there's not enough data for an FFT, move on
    if (fsize + m_buffer.size() < n_fft) {
        std::copy(fdata, fdata + fsize, std::back_inserter(m_buffer));
        return maxSize;
    }

    // Otherwise do an FFT and save the remainder
    int remainder = n_fft - m_buffer.size();

    // Put all the buffer in m_in, then full the rest from data
    std::copy(m_buffer.begin(), m_buffer.end(), m_in.begin());
    std::copy(fdata, fdata + remainder, std::back_inserter(m_in));

    // Put the rest of data in the buffer for later
    m_buffer.clear();
    std::copy(fdata + remainder, fdata + fsize, m_buffer.begin());

    // Perform 1D r2c FFT
    fftwf_plan p = fftwf_plan_dft_r2c_1d(
        n_fft,
        m_in.data(),
        reinterpret_cast<fftwf_complex*>(m_out.data()),
        FFTW_ESTIMATE);
    fftwf_execute(p);
    fftwf_destroy_plan(p);

    // Convert m_out to decibels
    for (std::complex<float> &c : m_out) {
        c = std::norm(c);
        c = 10. * std::log10(std::max(1.e-10f, c.real()));
    }

    auto proxy = m_surfaceSeries->dataProxy();
    auto t = proxy->array()->last()->last().z();
    t += float(n_fft) / sampleRate;

    QList<QPointF> line;
    auto row = new QSurfaceDataRow;
    for (int j = 0; j < (n_fft / 2 + 1); ++j) {
        auto x = std::max(1e-10f, (float(j)* sampleRate) / n_fft);
        auto y = m_out[j].real();
        line.append(QPointF(x, y));
        *row << QVector3D(x, y, t);
    }
    m_lineSeries->replace(line);
    proxy->removeRows(0, 1);
    proxy->addRow(row);

    return maxSize;
}
