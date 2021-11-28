#ifndef BUILD_H
#define BUILD_H

#include <QObject>

class Build : public QObject
{
    Q_OBJECT
public:
    explicit Build(QObject *parent = nullptr);

signals:

};

#endif // BUILD_H
