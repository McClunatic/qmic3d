#ifndef PLOTIODEVICE_H
#define PLOTIODEVICE_H

#include <QIODevice>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <complex>

class PlotIODevice : public QIODevice
{
public:
    explicit PlotIODevice(QLineSeries *lineSeries, QChart *specChart,
                          QObject *parent = nullptr);

    static const int n_fft{ 2048 };
    static const int sampleRate { 22050 };
    void plotAreaChanged(const QRectF& plotArea);

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QLineSeries *m_lineSeries = nullptr;
    QChart *m_specChart = nullptr;
    QList<float> m_buffer;
    QList<float> m_in;
    QList<std::complex<float>> m_out;

    QImage m_image;
    QList<QList<QVector3D>> m_specData;
};

#endif // PLOTIODEVICE_H
