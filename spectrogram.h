#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QtDataVisualization/Q3DSurface>

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(QWidget *parent = nullptr);

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);
private:
    Q3DSurface *m_surface = nullptr;
    QWidget *m_container = nullptr;

};

#endif // SPECTROGRAM_H
