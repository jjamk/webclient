#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpSocket socketTcp_ ;
    QSslSocket socketSsl_ ;

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_pbSend_clicked();

    void on_rbTcpSocket_toggled(bool checked);

    void on_rbSslSocket_toggled(bool checked);

    void on_pbClear_clicked();

private:
    Ui::Widget *ui;
    void restoreStateFromSettings();
    int rbsocket = 0 ;
};
#endif // WIDGET_H
