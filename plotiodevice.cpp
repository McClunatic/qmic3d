#include "plotiodevice.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

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

    QList<QPointF> line;
    for (int i = 0; i < (n_fft / 2 + 1); ++i) {
        auto x = std::max(1e-10f, float(i * sampleRate) / n_fft);
        line.append(QPointF(x, m_out[i].real()));
    }
    m_lineSeries->replace(line);
    //   store latest DB in m_surfaceSeries (time, freq, DB)
    return maxSize;
}
