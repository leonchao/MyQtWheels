#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include <QThread>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavutil/timecode.h>
#include <libavutil/bprint.h>
}

class DecodeThread : public QThread
{
    Q_OBJECT
public:
    explicit DecodeThread(const QString &fileName,QObject *parent = nullptr);
    void stop();
    void run();
signals:
    void sig_quit();
public slots:
    void onQuit();
private:
    bool _stopThread = false;
    QString _fileName = "";
    AVFormatContext *_ifmt_ctx = nullptr;
    AVCodecContext *_codec_ctx = nullptr;
    AVCodec *_codec = nullptr;
    int _videoStream = -1;
    int _audioStream = -1;
};

#endif // DECODETHREAD_H
