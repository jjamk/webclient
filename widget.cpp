#include "widget.h"
#include "ui_widget.h"
#include <QSettings>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    restoreStateFromSettings();

    QObject::connect(&socketTcp_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socketTcp_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socketTcp_, &QIODevice::readyRead, this, &Widget::doReadyRead);

    QObject::connect(&socketSsl_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socketSsl_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socketSsl_, &QIODevice::readyRead, this, &Widget::doReadyRead);

    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(false);
    ui->pbSend->setEnabled(false);
    ui->pbClear->setEnabled(false);

    //socket_.connected();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::restoreStateFromSettings()
{
    QSettings settings("auto_saved.ini", QSettings::IniFormat);

    int x = settings.value("window/x", 0).toInt();
    int y = settings.value("window/y", 0).toInt();
    int w = settings.value("window/width", 334).toInt();
    int h = settings.value("window/height", 467).toInt();
    this->setGeometry(x, y, w, h);

    ui->leHost->setText(settings.value("host", "www.naver.com").toString());
    ui->lePort->setText(settings.value("port", "80").toString());
    ui->pteMessage->setPlainText(settings.value("message", "").toString());
}
void Widget::closeEvent(QCloseEvent *event)
{
    QSettings settings("auto_saved.ini", QSettings::IniFormat);

    //windows x,y and size save
    settings.setValue("window/x", this->x());
    settings.setValue("window/y", this->y());
    settings.setValue("window/width", this->width());
    settings.setValue("window/height", this->height());

    //lineedit, plaintextedit save
    settings.setValue("host", ui->leHost->text());
    settings.setValue("port", ui->lePort->text());
    settings.setValue("message", ui->pteMessage->toPlainText());

    QWidget::closeEvent(event);

}

void Widget::doConnected() {
    QString msg = "Connected\n";
    ui->pteMessage->insertPlainText(msg);

    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);
    ui->pbSend->setEnabled(true);
}

void Widget::doDisconnected() {
    QString msg = "DisConnected\n";
    ui->pteMessage->insertPlainText(msg);

    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);
    ui->pbSend->setEnabled(false);

}

void Widget::doReadyRead() {
    if (rbsocket==1)
    {
        QString msg = socketTcp_.readAll();
        ui->pteMessage->insertPlainText(msg);
    }
    if (rbsocket==2){
        QString msg = socketSsl_.readAll();
        ui->pteMessage->insertPlainText(msg);
    }
}

void Widget::on_pbConnect_clicked()
{
    if (rbsocket==1)
    {
        socketTcp_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    }
    if (rbsocket==2){
        socketSsl_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    }
}


void Widget::on_pbDisconnect_clicked()
{
    if (rbsocket==1)
    {
        socketTcp_.disconnectFromHost();
    }
    if (rbsocket==2){
        socketSsl_.disconnectFromHost();
    }
}


void Widget::on_pbSend_clicked()
{
    if (rbsocket==1)
    {
        socketTcp_.write(ui->pteMesage2->toPlainText().toUtf8()); //qstring to qbyte array
    }
    if (rbsocket==2){
        socketSsl_.write(ui->pteMesage2->toPlainText().toUtf8()); //qstring to qbyte array
    }

}


void Widget::on_rbTcpSocket_toggled(bool checked)
{
    if (checked) {
        rbsocket = 1;
        ui->pbConnect->setEnabled(true);
        ui->pbClear->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        ui->pbSend->setEnabled(false);
    }
}

void Widget::on_rbSslSocket_toggled(bool checked)
{
    if (checked) {
        rbsocket=2;
        ui->pbConnect->setEnabled(true);
        ui->pbClear->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        ui->pbSend->setEnabled(false);
    }

}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

