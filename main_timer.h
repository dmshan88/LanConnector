#ifndef MAIN_TIMER_H
#define MAIN_TIMER_H

#include <QTimer>
class MainTimer : public QObject
{
    Q_OBJECT
public:
    explicit MainTimer(QObject *parent = 0);
    void SetInterval(int interval) {timer_->setInterval(interval);}
    void Start(void){timer_->start();}
    void Stop(void) {timer_->stop();}

signals:

public slots:
private slots:
    void SlotTimeout(void);
private:
    QTimer *timer_;
    const static int kDefaultInterval;

};

#endif // MAIN_TIMER_H
