#ifndef PLOTIODEVICE_H
#define PLOTIODEVICE_H

#include <QIODevice>
#include <QtCharts/QLineSeries>
#include <QtDataVisualization/QSurface3DSeries>

class PlotIODevice : public QIODevice
{
public:
    explicit PlotIODevice(QLineSeries *lseries, QSurface3DSeries *sseries,
                          QObject *parent = nullptr);

    static const int n_fft{ 2048 };
    static const int sampleCount{ 1 + n_fft / 2 };

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QLineSeries *m_lineSeries = nullptr;
    QSurface3DSeries *m_surfaceSeries = nullptr;
    QList<QPointF> m_buffer;
};

#endif // PLOTIODEVICE_H
