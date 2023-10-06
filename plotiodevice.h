#ifndef PLOTIODEVICE_H
#define PLOTIODEVICE_H

#include <QIODevice>
#include <QtCharts/QLineSeries>
#include <QtDataVisualization/QSurface3DSeries>

#include <complex>

class PlotIODevice : public QIODevice
{
public:
    explicit PlotIODevice(QLineSeries *lseries, QSurface3DSeries *sseries,
                          QObject *parent = nullptr);

    static const int n_fft{ 2048 };
    static const int sampleRate { 22050 };

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QLineSeries *m_lineSeries = nullptr;
    QSurface3DSeries *m_surfaceSeries = nullptr;
    QList<float> m_buffer;
    QList<float> m_in;
    QList<std::complex<float>> m_out;
};

#endif // PLOTIODEVICE_H
