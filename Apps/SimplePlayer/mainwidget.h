#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
class DecodeThread;
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::MainWidget *ui;
    DecodeThread *_decodeThread = nullptr;
};

#endif // MAINWIDGET_H
