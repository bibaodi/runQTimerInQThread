#include <QCoreApplication>
#include <QTimer>
#include <QtCore>
#include <stdio.h>
#include <stdlib.h>

static int counter = 0;

class Consumer : public QThread {
    Q_OBJECT
  public:
    static Consumer *getPointer() { return Consumer::m_ptr; }
    Consumer(QObject *parent = NULL) : QThread(parent) {
        m_ptr = this;
        fprintf(stderr, "create Consumer%p\n", QThread::currentThreadId());
    }
    ~Consumer() {
        // fprintf(stderr, "delete Consumer%p\n", QThread::currentThreadId());
        fprintf(stderr, "delete Consumer\n");
        m_ptr = 0;
    }

    void run() override {
        fprintf(stderr, "start Consumer%p\n", QThread::currentThreadId());
        exec();
    }

  public:
    static Consumer *m_ptr;
  public slots:
    void slot_timerOut() {
        fprintf(stderr, "%d:timer out,thread=%p\n", counter++, QThread::currentThreadId());
        if (counter > 10) {
            emit sig_jobEnd();
        }
    };
  signals:
    void sig_jobEnd();
};

Consumer *Consumer::m_ptr = nullptr;

class TimerDemo : public QThread {
    Q_OBJECT
  public:
    TimerDemo(QObject *parent = NULL) : QThread(parent) {}
    ~TimerDemo() { fprintf(stderr, "distroy timer thread%p\n", QThread::currentThreadId()); }
    void run() override {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, Consumer::getPointer(), &Consumer::slot_timerOut);
        connect(Consumer::getPointer(), &Consumer::sig_jobEnd, this, &TimerDemo::deleteLater);
        fprintf(stderr, "create timer.%p\n", QThread::currentThreadId());
        fprintf(stderr, "start timer thread%p\n", QThread::currentThreadId());
        m_timer->start(100);
        fprintf(stderr, "exit timer thread.\n");
        exec();
    }
    QTimer *m_timer;
  signals:
    void stringConsumed(const QString &text);
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    Consumer consumer;
    TimerDemo td;
    app.connect(&consumer, SIGNAL(sig_jobEnd()), &consumer, SLOT(quit()));
    // app.connect(&consumer, SIGNAL(sig_jobEnd()), &app, SLOT(quit()));
    td.start();
    consumer.start();
    fprintf(stderr, "start finish:%p\n", QThread::currentThreadId());
    app.exec();
    fprintf(stderr, "after exec:%p\n", QThread::currentThreadId());
    consumer.wait();
    td.wait();
    return 0;
}
#include "./include/main.moc"
