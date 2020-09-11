#include "decodethread.h"
#include <QDebug>
DecodeThread::DecodeThread(const QString &fileName, QObject *parent) : QThread(parent),
    _fileName(fileName)
{
}

void DecodeThread::stop()
{
    _stopThread = true;
    emit sig_quit();
}

void DecodeThread::run()
{
    connect(this,&DecodeThread::sig_quit,this,&DecodeThread::onQuit);
    while(1){
        if(_stopThread)
            break;
        qDebug() << _fileName << _stopThread;
        msleep(100);
    }
}

void DecodeThread::onQuit()
{
    QThread::quit();
}
