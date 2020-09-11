#include "colorwidget.h"
#include "colorpicker.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QToolButton>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>

CColorWidget::CColorWidget(QColor color, QWidget *parent)
    : QWidget(parent),
      _currentColor(color),
      _lastColor(color)
{
    _colorPicker = new CColorPicker(color,this);
    _hueSpin = new QSpinBox(this);
    _satSpin = new QSpinBox(this);
    _valSpin = new QSpinBox(this);
    _rSpin = new QSpinBox(this);
    _gSpin = new QSpinBox(this);
    _bSpin = new QSpinBox(this);

    _hueSpin->setRange(0,359);
    _satSpin->setRange(0,255);
    _valSpin->setRange(0,255);
    _rSpin->setRange(0,255);
    _gSpin->setRange(0,255);
    _bSpin->setRange(0,255);

    _hLabel = new QLabel("H:",this);
    _sLabel = new QLabel("S:",this);
    _vLabel = new QLabel("V:",this);
    _rLabel = new QLabel("R:",this);
    _gLabel = new QLabel("G:",this);
    _bLabel = new QLabel("B:",this);


    _colorNameEdit = new QLineEdit(this);
    _colorNameEdit->setText(_currentColor.name());

    _hueSpin->setValue(_colorPicker->curH());
    _satSpin->setValue(_colorPicker->curS());
    connect(_colorPicker,&CColorPicker::colorChanged,[this](int h,int s){
        if(_valSpin->value() == 0)
            setHSV(h,s,200);
        else
            setHSV(h,s,_valSpin->value());
    });
    connect(_hueSpin,SIGNAL(valueChanged(int)),this,SLOT(setHSV()));
    connect(_satSpin,SIGNAL(valueChanged(int)),this,SLOT(setHSV()));
    connect(_valSpin,SIGNAL(valueChanged(int)),this,SLOT(setHSV()));

    connect(_rSpin,SIGNAL(valueChanged(int)),this,SLOT(setRGB()));
    connect(_gSpin,SIGNAL(valueChanged(int)),this,SLOT(setRGB()));
    connect(_bSpin,SIGNAL(valueChanged(int)),this,SLOT(setRGB()));

    connect(_colorNameEdit,SIGNAL(textChanged(QString)),this,SLOT(setColorName(QString)));

    _definedColorList.append(QColor(255,0,0));
    _definedColorList.append(QColor(0,255,0));
    _definedColorList.append(QColor(0,0,255));
    _definedColorList.append(QColor(0,0,0));
    _definedColorList.append(QColor(255,255,255));
    _definedColorList.append(QColor(255,255,0));

    _pickColorBtn = new QToolButton(this);
    connect(_pickColorBtn,&QToolButton::clicked,[this]{
        _fullScreenImage = QApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
        if(_maskWidget == nullptr)
            _maskWidget = new QWidget();
        _maskWidget->setWindowFlags(_maskWidget->windowFlags() | Qt::FramelessWindowHint | Qt::Popup);
        _maskWidget->resize(QApplication::primaryScreen()->geometry().size());
        qDebug() << "222222222222222222222222" << _maskWidget->size() << QApplication::primaryScreen()->geometry();
        _maskWidget->move(0,0);
        _maskWidget->setWindowOpacity(0.1);
        _maskWidget->raise();
        _maskWidget->setMouseTracking(true);
        _maskWidget->installEventFilter(this);
        _maskWidget->show();
    });
}

QColor CColorWidget::getCurrentColor()
{
    return _currentColor;
}

void CColorWidget::updateUI(const qreal &scaleRatio)
{
    _ratio = scaleRatio;
    upUI();
}

void CColorWidget::resizeEvent(QResizeEvent *event)
{
    upUI();
}

void CColorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),QColor("#1e1e1e"));
    QLinearGradient gra(_valRect.bottomLeft(),_valRect.bottomRight());
    QColor startColor;
    QColor endColor;
    startColor.setHsv(_colorPicker->curH(),_colorPicker->curS(),0);
    endColor.setHsv(_colorPicker->curH(),_colorPicker->curS(),255);
    gra.setColorAt(0,startColor);
    gra.setColorAt(1,endColor);
    painter.setBrush(gra);
    painter.setPen(Qt::NoPen);
    painter.drawRect(_valRect.adjusted(2,2,-2,-2));
    qreal x = (qreal)_valSpin->value() / 255.0 * (_valRect.width() - 8) + _valRect.left() + 4;
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    QPainterPath path;
    path.moveTo(x,_valRect.top() + 4 * sqrt(3));
    path.lineTo(x - 4,_valRect.top());
    path.lineTo(x + 4,_valRect.top());
    path.lineTo(x,_valRect.top() + 4 * sqrt(3));
    path.lineTo(x,_valRect.bottom() - 4 * sqrt(3));
    path.lineTo(x - 4,_valRect.bottom());
    path.lineTo(x + 4,_valRect.bottom());
    path.lineTo(x,_valRect.bottom() - 4 * sqrt(3));

//    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPath(path);
//    painter.setRenderHint(QPainter::Antialiasing,false);

    painter.fillRect(_compareRect.adjusted(0,0,0,-_compareRect.height() / 2),_currentColor);
    painter.fillRect(_compareRect.adjusted(0,_compareRect.height() / 2,0,0),_lastColor);
}

void CColorWidget::mousePressEvent(QMouseEvent *event)
{
    if(_valRect.contains(event->pos()))
    {
        _isMousePressed = true;
        qreal val = (event->localPos().x() - _valRect.left()) / _valRect.width() * 255.0;
        val = qMin(255.0,qMax(0.0,val));
        _valSpin->blockSignals(true);
        _valSpin->setValue(val);
        _valSpin->blockSignals(false);
        setHSV();
        update();
    }
}

void CColorWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!_isMousePressed)
        return;
    qreal val = (event->localPos().x() - _valRect.left()) / _valRect.width() * 255.0;
    val = qMin(255.0,qMax(0.0,val));
    _valSpin->blockSignals(true);
    _valSpin->setValue(val);
    _valSpin->blockSignals(false);
    setHSV();
    update();
}

void CColorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _isMousePressed = false;
}

bool CColorWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == _maskWidget)
    {
        if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QRgb rgb = _fullScreenImage.toImage().pixel(mouseEvent->pos());
            qDebug() << "111111111111111111" << qRed(rgb) << qGreen(rgb) << qBlue(rgb);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QRgb rgb = _fullScreenImage.toImage().pixel(mouseEvent->pos());
            setRGB(qRed(rgb),qGreen(rgb),qBlue(rgb));
        }
    }
    return QWidget::eventFilter(watched,event);
}

void CColorWidget::upUI()
{
    _colorPicker->setGeometry(20 * _ratio ,20 * _ratio ,width() - 40 * _ratio,width() - 40 * _ratio);
    _valRect = QRect(_colorPicker->geometry().left(),_colorPicker->geometry().bottom() + 12 * _ratio,_colorPicker->width(),20);
    _compareRect = QRect(_valRect.left() + 10 * _ratio,_valRect.bottom() + 20 * _ratio,52 * _ratio,52 * _ratio);
    QFont font;
    font.setPixelSize(12 * _ratio);
    _hLabel->setFont(font);
    _sLabel->setFont(font);
    _vLabel->setFont(font);
    _rLabel->setFont(font);
    _gLabel->setFont(font);
    _bLabel->setFont(font);
    _hLabel->adjustSize();
    _sLabel->adjustSize();
    _vLabel->adjustSize();
    _rLabel->adjustSize();
    _gLabel->adjustSize();
    _bLabel->adjustSize();
    _hLabel->setFixedHeight(20 * _ratio);
    _sLabel->setFixedHeight(20 * _ratio);
    _vLabel->setFixedHeight(20 * _ratio);
    _rLabel->setFixedHeight(20 * _ratio);
    _gLabel->setFixedHeight(20 * _ratio);
    _bLabel->setFixedHeight(20 * _ratio);
    _hLabel->move(_compareRect.right() + 20 * _ratio,_compareRect.top());
    _hueSpin->setGeometry(_hLabel->geometry().right() + 8 * _ratio,_hLabel->y(),40 * _ratio,20 * _ratio);
    _sLabel->move(_hLabel->geometry().left(),_hLabel->y() + _hLabel->height() + 8 * _ratio);
    _satSpin->setGeometry(_sLabel->geometry().right() + 8 * _ratio,_sLabel->y(),40 * _ratio,20 * _ratio);
    _vLabel->move(_hLabel->geometry().left(),_sLabel->y() + _sLabel->height() + 8 * _ratio);
    _valSpin->setGeometry(_vLabel->geometry().right() + 8 * _ratio,_vLabel->y(),40 * _ratio,20 * _ratio);

    _rLabel->move(_hueSpin->geometry().right() + 12 * _ratio,_hLabel->y());
    _rSpin->setGeometry(_rLabel->geometry().right() + 8 * _ratio,_rLabel->y(),40 * _ratio,20 * _ratio);
    _gLabel->move(_rLabel->x(),_rLabel->y() + _rLabel->height() + 8 * _ratio);
    _gSpin->setGeometry(_gLabel->geometry().right() + 8 * _ratio,_gLabel->y(),40 * _ratio,20 * _ratio);
    _bLabel->move(_gLabel->x(),_gLabel->y() + _gLabel->height() + 8 * _ratio);
    _bSpin->setGeometry(_bLabel->geometry().right() + 8 * _ratio,_bLabel->y(),40 * _ratio,20 * _ratio);

    _colorNameEdit->setGeometry(_compareRect.left(),_compareRect.bottom() + 8 * _ratio,_compareRect.width(),20 * _ratio);

    _pickColorBtn->setGeometry(_colorNameEdit->geometry().left(),_colorNameEdit->geometry().bottom() + 8 * _ratio,20 * _ratio, 20 * _ratio);
//    _hueSpin->setGeometry(_colorPicker->geometry().left(),_colorPicker->geometry().bottom() + 8,40,20);
//    _satSpin->setGeometry(_colorPicker->geometry().left(),_hueSpin->geometry().bottom() + 8,40,20);
//    _valSpin->setGeometry(_colorPicker->geometry().left(),_satSpin->geometry().bottom() + 8,40,20);

//    _rSpin->setGeometry(_hueSpin->geometry().right() + 10,_colorPicker->geometry().bottom() + 8,40,20);
//    _gSpin->setGeometry(_rSpin->geometry().left(),_rSpin->geometry().bottom() + 8,40,20);
//    _bSpin->setGeometry(_rSpin->geometry().left(),_gSpin->geometry().bottom() + 8,40,20);


//    _valRect = QRect(_colorPicker->geometry().left(),_valSpin->geometry().bottom() + 10,_colorPicker->width(),16);
//    _compareRect = QRect(_valRect.left(),_valRect.bottom() + 10, 40, 60);
}

void CColorWidget::setHSV(const int &h, const int &s, const int &v)
{
    _hueSpin->blockSignals(true);
    _satSpin->blockSignals(true);
    _valSpin->blockSignals(true);
    _hueSpin->setValue(h);
    _satSpin->setValue(s);
    _valSpin->setValue(v);
    _hueSpin->blockSignals(false);
    _satSpin->blockSignals(false);
    _valSpin->blockSignals(false);
    setHSV();
}

void CColorWidget::setHSV()
{
    int h = _hueSpin->value();
    int s = _satSpin->value();
    int v = _valSpin->value();
    _currentColor.setHsv(h,s,v);
    _rSpin->blockSignals(true);
    _gSpin->blockSignals(true);
    _bSpin->blockSignals(true);
    _rSpin->setValue(_currentColor.red());
    _gSpin->setValue(_currentColor.green());
    _bSpin->setValue(_currentColor.blue());
    _rSpin->blockSignals(false);
    _gSpin->blockSignals(false);
    _bSpin->blockSignals(false);

    _colorNameEdit->blockSignals(true);
    _colorNameEdit->setText(_currentColor.name());
    _colorNameEdit->blockSignals(false);

    _colorPicker->blockSignals(true);
    _colorPicker->setH(h);
    _colorPicker->setS(s);
    _colorPicker->blockSignals(false);
    update();
    emit currentColorChanged(_currentColor);
}

void CColorWidget::setRGB()
{
    int r = _rSpin->value();
    int g = _gSpin->value();
    int b = _bSpin->value();
    _currentColor.setRgb(r,g,b);
    int h,s,v;
    _currentColor.getHsv(&h,&s,&v);
    _hueSpin->blockSignals(true);
    _satSpin->blockSignals(true);
    _valSpin->blockSignals(true);
    _hueSpin->setValue(h);
    _satSpin->setValue(s);
    _valSpin->setValue(v);
    _hueSpin->blockSignals(false);
    _satSpin->blockSignals(false);
    _valSpin->blockSignals(false);
    _colorNameEdit->blockSignals(true);
    _colorNameEdit->setText(_currentColor.name());
    _colorNameEdit->blockSignals(false);
    _colorPicker->blockSignals(true);
    _colorPicker->setH(h);
    _colorPicker->setS(s);
    _colorPicker->blockSignals(false);
    update();
    emit currentColorChanged(_currentColor);
}

void CColorWidget::setRGB(int r, int g, int b)
{
    _currentColor.setRgb(r,g,b);
    int h,s,v;
    _rSpin->blockSignals(true);
    _gSpin->blockSignals(true);
    _bSpin->blockSignals(true);
    _rSpin->setValue(r);
    _gSpin->setValue(g);
    _bSpin->setValue(b);
    _rSpin->blockSignals(false);
    _gSpin->blockSignals(false);
    _bSpin->blockSignals(false);
    _currentColor.getHsv(&h,&s,&v);
    _hueSpin->blockSignals(true);
    _satSpin->blockSignals(true);
    _valSpin->blockSignals(true);
    _hueSpin->setValue(h);
    _satSpin->setValue(s);
    _valSpin->setValue(v);
    _hueSpin->blockSignals(false);
    _satSpin->blockSignals(false);
    _valSpin->blockSignals(false);
    _colorNameEdit->blockSignals(true);
    _colorNameEdit->setText(_currentColor.name());
    _colorNameEdit->blockSignals(false);
    _colorPicker->blockSignals(true);
    _colorPicker->setH(h);
    _colorPicker->setS(s);
    _colorPicker->blockSignals(false);
    update();
    emit currentColorChanged(_currentColor);
}

void CColorWidget::setColorName(const QString &name)
{
    _currentColor.setNamedColor(name);
    int h,s,v;
    _currentColor.getHsv(&h,&s,&v);
    _colorPicker->blockSignals(true);
    _colorPicker->setH(h);
    _colorPicker->setS(s);
    _colorPicker->blockSignals(false);
    _hueSpin->blockSignals(true);
    _satSpin->blockSignals(true);
    _valSpin->blockSignals(true);
    _hueSpin->setValue(h);
    _satSpin->setValue(s);
    _valSpin->setValue(v);
    _hueSpin->blockSignals(false);
    _satSpin->blockSignals(false);
    _valSpin->blockSignals(false);

    _rSpin->blockSignals(true);
    _gSpin->blockSignals(true);
    _bSpin->blockSignals(true);
    _rSpin->setValue(_currentColor.red());
    _gSpin->setValue(_currentColor.green());
    _bSpin->setValue(_currentColor.blue());
    _rSpin->blockSignals(false);
    _gSpin->blockSignals(false);
    _bSpin->blockSignals(false);
    update();
    emit currentColorChanged(_currentColor);
}
