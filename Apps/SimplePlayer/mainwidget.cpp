#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "decodethread.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    _decodeThread = new DecodeThread("123");
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pushButton_start_clicked()
{
    qDebug() << _decodeThread->isRunning();
    _decodeThread->start();
}

void MainWidget::on_pushButton_stop_clicked()
{
    _decodeThread->stop();
}
