#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(QWidget *parent = nullptr);

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);
    QSurface3DSeries* series() { return m_series; }
private:
    Q3DSurface *m_surface = nullptr;
    QWidget *m_container = nullptr;
    QSurface3DSeries *m_series = nullptr;
};

#endif // SPECTROGRAM_H
