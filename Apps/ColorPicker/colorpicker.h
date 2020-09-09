#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>

class CColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit CColorPicker(QColor color,QWidget *parent = nullptr);
    void getPixmap();
    bool checkPixelInRound(int x, int y, bool initPixmap = false);
    void setH(const int &hue);
    void setS(const int &sat);
    int curH();
    int curS();
protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *e);

signals:
    void colorChanged(int h,int s);

public slots:

private:
    int _hue = 0;
    int _sat = 0;
    int _val = 0;

    int _curhue = 0;
    int _cursat = 0;
    int _curval = 0;

    QPainterPath _roundPath;
    bool _mousePressed = false;
    qreal _radius = 50.0;
    QPixmap _pix;
};

#endif // COLORPICKER_H
