#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
class CColorPicker;
class QSpinBox;
class QLineEdit;
class QLabel;

class CColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CColorWidget(QColor color = QColor(),QWidget *parent = nullptr);
    QColor getCurrentColor();
    void updateUI(const qreal &scaleRatio);
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    void upUI();
signals:
    void currentColorChanged(QColor color);
public slots:
    void setHSV(const int &h,const int &s,const int &v);
    void setHSV();
    void setRGB();
    void setColorName(const QString &name);
private:
    CColorPicker *_colorPicker = nullptr;
    QColor _currentColor,_lastColor;    //当前颜色,传入颜色
    bool _isMousePressed = false;
    QSpinBox *_hueSpin;     //h
    QSpinBox *_satSpin;     //s
    QSpinBox *_valSpin;     //v
    QSpinBox *_rSpin;       //r
    QSpinBox *_gSpin;       //g
    QSpinBox *_bSpin;       //b

    QLabel *_hLabel;
    QLabel *_sLabel;
    QLabel *_vLabel;
    QLabel *_rLabel;
    QLabel *_gLabel;
    QLabel *_bLabel;

    QLineEdit *_colorNameEdit;      //颜色名字16进制

    QRect _valRect;                 //v 编辑区域
    QRect _compareRect;             //对比区域

    QList<QColor> _definedColorList;    //预定义颜色

    qreal _ratio = 1.0;

};

#endif // COLORWIDGET_H
