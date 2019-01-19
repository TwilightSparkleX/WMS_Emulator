#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialog.h"
#include <QtSql>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMainWindow>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QtXml>
#include <QKeyEvent>
#include <QUuid>
#include<QTableView>
#include<QSqlError>
#include<QVector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QJsonDocument doc;
    QJsonParseError jerr;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString ip,pl;
    qint32 port,light;
    int last;
    QString imagesrc,barcodeM,barlist,testvar;
    QString jsons,orderid,type,placeid,islast;
    QTableView view1;
    QSqlQueryModel model;
    //updatevar
    int number,testint,ti;
signals:
    void onReady();
private slots:

    void on_put2lightButton_clicked();

    void on_pick2lightButton_clicked();


    void product();

    void on_StartButton_clicked();

    void on_AddProductButton_clicked();

    void onResult(QNetworkReply *reply);

    void on_ScanButton_clicked();

    void on_textEdit_textChanged();

    void on_AddImageButton_clicked();

    void on_ChangeProductButton_clicked();

    void on_DeleteProductButton_clicked();

    void newConnection();

    void isconnected();

    void pickproduct();

    void on_addToList_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_ConfigWindow_clicked();

    void changeText(QString text,qint32 portvar);

    void on_SetIP_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    void keyPressEvent(QKeyEvent *e);
    Dialog* d;
    bool enter;
    bool idfine;
    int id;
    QSqlDatabase db;
    QPixmap picture;
    QString fileName;
    QTcpServer *server;
};

#endif // MAINWINDOW_H
