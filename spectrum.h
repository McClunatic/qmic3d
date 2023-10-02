#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>

class Spectrum : public QObject
{
    Q_OBJECT
public:
    explicit Spectrum(QObject *parent = nullptr);

signals:

};

#endif // SPECTRUM_H
