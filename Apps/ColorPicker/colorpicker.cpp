#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QDebug>
#include "cstring"
#include <QTime>

CColorPicker::CColorPicker(QColor color, QWidget *parent) : QWidget(parent)
{
    color.getHsv(&_curhue,&_cursat,&_curval);
}

void CColorPicker::getPixmap()
{
    qreal w = width();
    qreal h = height();
    qreal r = width() / 2;
    QImage img(w, h, QImage::Format_RGB32);
    int x, y;
    uint *pixel = (uint *) img.scanLine(0);
    img.fill(QColor(255,255,255,255));
    QTime time;
    time.start();
    for (y = 0; y < h; y++) {
        const uint *end = pixel + (int)w;
        x = 0;
        while (pixel < end) {
            QColor c;
            if(checkPixelInRound(x,y,true))
            {
                c.setHsv(_hue, _sat, 200);
                *pixel = c.rgb();
            }
            ++pixel;
            ++x;
        }
    }
    _pix = QPixmap::fromImage(img);
}

bool CColorPicker::checkPixelInRound(int x, int y,bool initPixmap)
{
    qreal r = width() / 2;
    qreal angle = atan2(r - (qreal)x,r - (qreal)y) / 3.141592653 * 180.0;
    if(angle < 0)
        angle += 360;
    qreal rnew = 0;
    if(initPixmap)
        _hue = angle;
    else
        _curhue = angle;
    if(x == r)
        rnew = fabs(r - y);
    else if(y == r)
        rnew = fabs(r - x);
    else
        rnew = fabs((r - (qreal)x) / sin(angle / 180.0 * 3.141592653));
    int sat = qMin(255.0,qMax(0.0,(rnew - r + _radius) / _radius * 255.0));
    if(initPixmap)
        _sat = sat;
    else
        _cursat = sat;
    if(!initPixmap)
        update();
    return (rnew <= r) && (rnew >= r - _radius);
}

void CColorPicker::setH(const int &hue)
{
    if(_curhue == hue)
        return;
    _curhue = hue;
    update();
    emit colorChanged(_curhue,_cursat);
}

void CColorPicker::setS(const int &sat)
{
    if(_cursat == sat)
        return;
    _cursat = sat;
    update();
    emit colorChanged(_curhue,_cursat);
}

int CColorPicker::curH()
{
    return _curhue;
}

int CColorPicker::curS()
{
    return _cursat;
}

void CColorPicker::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.fillRect(this->rect(),Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(_roundPath);
    painter.drawPixmap(this->rect(),_pix);
    QPainterPath path;
    path.addRect(this->rect());
    path = path.subtracted(_roundPath);
    painter.fillPath(path,Qt::black);
    painter.setClipping(false);
    painter.setPen(Qt::white);
    QPointF pos;
    qreal r = width() / 2 - _radius + (qreal)_cursat / 255.0 * _radius;
    if(_curhue >= 0 && _curhue < 90)
    {
        pos.setX(width() / 2 - r * sin(_curhue / 180.0 * 3.141592653));
        pos.setY(height() / 2 - r * cos(_curhue / 180.0 * 3.141592653));
    }
    else if(_curhue >= 90 && _curhue < 180)
    {
        pos.setX(width() / 2 - r * sin((180.0 - _curhue) / 180.0 * 3.141592653));
        pos.setY(height() / 2 + r * cos((180.0 - _curhue) / 180.0 * 3.141592653));
    }
    else if(_curhue >= 180 && _curhue < 270)
    {
        pos.setX(width() / 2 + r * sin((_curhue - 180.0) / 180.0 * 3.141592653));
        pos.setY(height() / 2 + r * cos((_curhue - 180.0) / 180.0 * 3.141592653));
    }
    else
    {
        pos.setX(width() / 2 + r * sin((360.0 - _curhue) / 180.0 * 3.141592653));
        pos.setY(height() / 2 - r * cos((360.0 - _curhue) / 180.0 * 3.141592653));
    }

    painter.setRenderHint(QPainter::Antialiasing,false);
    painter.drawLine(pos.x() - 4,pos.y(),pos.x() + 4,pos.y());
    painter.drawLine(pos.x(),pos.y() - 4,pos.x(),pos.y() + 4);
}

void CColorPicker::mousePressEvent(QMouseEvent *event)
{
    if(_roundPath.contains(event->pos()))
    {
        _mousePressed = true;
        checkPixelInRound(event->pos().x(),event->pos().y());
        emit colorChanged(_curhue,_cursat);
    }
}

void CColorPicker::mouseMoveEvent(QMouseEvent *event)
{
    if(!_mousePressed)
        return;
    checkPixelInRound(event->pos().x(),event->pos().y());
    emit colorChanged(_curhue,_cursat);
}

void CColorPicker::mouseReleaseEvent(QMouseEvent *event)
{
    _mousePressed = false;
}

void CColorPicker::resizeEvent(QResizeEvent *e)
{
    if(width() != height())
    {
        int max = qMin(width(),height());
        resize(max,max);
        return;
    }
    QPainterPath path;
    path.addEllipse(this->rect().adjusted(1,1,-1,-1));
    _radius = width() / 2 - width() / 8;
    path.addEllipse(QPointF(width() / 2,height() / 2),width() / 8 + 1,width() / 8 + 1);
    _roundPath = path;
    getPixmap();
    update();
}
