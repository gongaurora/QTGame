#ifndef TIMER_H
#define TIMER_H
#include <QTimer>

class Timer : public QTimer
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr);

signals:

public slots:
    void start(int msec);
    void start();
    void pause();
    void resume();


private:
    bool isPause;
    bool isSingle;
    int intervalPrev;
    int remainingTimeMsec;
};

#endif // TIMER_H
