#include "mainwindow.h"
#include "idgenerator.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QPixmap>
#include <QUrlQuery>
#include<QSql>
#include <QFileDialog>
#include <QJsonObject>
#include<QJsonParseError>
#include<QTableView>
#include<QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port=1225;
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any,port))
    {
        qDebug()<<"error";
    }
    else
    {
        qDebug()<<"Server Ok";
    }
    // isconnected();

    ip="192.10.10.102";
    number=0;
    testint=0;
    type="put";
    enter=true;
    QString path = QDir::currentPath();
    path.append("/mydb.db");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    QString fileName;
    if(!db.open()){
        qDebug()<<"error";

    }
    qDebug()<<"Connection OK.";
    QSqlQuery qur;
    qur.exec("select Barcode,Info from Base;");
    qur.lastError();
    QSqlRecord rec = qur.record();
    //connect(TestBarcodeButton, SIGNAL(clicked()), this, SLOT(setText()));
    while(qur.next())
    {
        barlist=qur.value(rec.indexOf("Barcode")).toString()+"      "+qur.value(rec.indexOf("Info")).toString();
        qDebug()<< barlist;
        ui->comboBox->addItem(barlist);
    }
    //    ui->tableView->setModel(&model);
    ui->textEdit->hide();
    networkManager = new QNetworkAccessManager();
    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
    // Получаем данные, а именно JSON файл с сайта по определённому url
    networkManager->get(QNetworkRequest(QUrl("http://qt.nokia.com")));
    ui->tableWidget->setColumnWidth(0, this->width()/7);
    ui->tableWidget->setColumnWidth(1, this->width()/6);
    ui->tableWidget->setColumnWidth(2, this->width()/7);
}


MainWindow::~MainWindow()
{
    delete ui;
}
//QSql
//QSqlQuery query;
//QNetworkRequest

void MainWindow::on_put2lightButton_clicked()
{

    ui->ModeField->setText("Режим Put2Light");
    ui->tableWidget->setColumnHidden(2,true);
    qDebug()<<ui->tableWidget->isColumnHidden(2);
    type="put";
}

void MainWindow::on_pick2lightButton_clicked()
{
    ui->ModeField->setText("Режим Pick2Light");
    ui->tableWidget->setColumnHidden(2,false);
    qDebug()<<ui->tableWidget->isColumnHidden(2);
    type="pick";

}

void MainWindow::product()
{
    QPixmap pic = QPixmap();
    QSqlQuery query;
    QByteArray outByteArray;
    query.exec("SELECT * FROM Base WHERE Barcode="+barcodeM+";");
    QSqlRecord rec = query.record();
    while(query.next())
    {
        ui->StatusLabel->setText("Номер ячейки:"+query.value(rec.indexOf("NF")).toString());
        ui->ProductInformationLeft->setText("Информация о товаре: \r"+query.value(rec.indexOf("Info")).toString());
        outByteArray=query.value(rec.indexOf("Img")).toByteArray();
    }
    pic.loadFromData(outByteArray);
    ui->ImageLeft->setPixmap(pic);
    QTableWidgetItem *var;
    QList<QTableWidgetItem*> found = ui->tableWidget->findItems(barcodeM, Qt::MatchExactly);
    foreach (var, found)
    {
        if(var->flags()==Qt::ItemIsUserCheckable)
        {
            found.removeFirst();
        }
    }
    if(found.count() != 0)
    {
        ui->tableWidget->item(found.at(0)->row(),0)->setFlags(Qt::ItemIsUserCheckable);
        for(int i=0;i<found.count();i++)
        {
            if(ui->tableWidget->item(found.at(i)->row(),0)->flags()==Qt::ItemIsUserCheckable)
            {
                ui->tableWidget->item(found.at(i)->row(),0)->setBackground(Qt::red);
                ti=found.at(i)->row()+1;
                pl=ui->tableWidget->item(found.at(i)->row(),1)->text();
                number++;
                light=found.at(i)->row();
            }
            else
            {
                //Если нужно будет сделать цветовую индикацию
                // ui->tableWidget->item(found.at(i)->row(),0)->setBackground(Qt::blue);
            }
        }
        qDebug()<<"OK";
        QNetworkRequest request(QUrl("http://" + ip + "/wms/puttask"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject json;
        json.insert("task_id", ti);
        json.insert("order_id", 1);
        json.insert("type", "put");
        json.insert("place_id",  pl);
        if(number==ui->tableWidget->rowCount()){
            json.insert("is_last", 1);
        }
        else{
            json.insert("is_last", 0);
        }
        qDebug()<<json;
        QNetworkAccessManager nam;
        QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());
        while(!reply->isFinished())
        {
            qApp->processEvents();
        }
        QByteArray response_data = reply->readAll();
        qDebug()<<response_data;
    }
    else{
        number=0;
        qDebug()<<"Dont receive";
    }
    qDebug()<<QDateTime::currentDateTime();
}


void MainWindow::on_StartButton_clicked()
{
    //ui->StartButton->
    if(ui->StartButton->text()=="Старт")
    {
        ui->StartButton->setText("Стоп");
        if(type=="pick"){
            pickproduct();
        }
    }
    else {
        ui->StartButton->setText("Старт");
        for(int i=0 ; i < ui->tableWidget->rowCount(); i++)
        {
            //            QString style(
            //                "QTableWidget::item {"
            //                   "background-color: rgba(255,255,255,255);border: none;"
            //                "}"
            //            );

            //          //  ui->tableWidget->setStyleSheet(style);
            ui->tableWidget->item(i,0)->setFlags(Qt::NoItemFlags);
            ui->tableWidget->item(i,0)->setBackground(Qt::white);
            ui->tableWidget->item(i,0)->setForeground(Qt::black);
            number=0;
        }
    }
}



void MainWindow::on_AddProductButton_clicked()
{
    qDebug()<<fileName;
    QString NF,Barcode,Con,Info,Am;
    NF = ui->NewcellField->toPlainText();
    Barcode = ui->textEdit->toPlainText().trimmed();
    QPixmap picture(fileName);
    Info = ui->ProductInformationFiled->toPlainText();
    Am = ui->ProductAmmountField->toPlainText();
    QByteArray inByteArray;
    QBuffer buf(&inByteArray);
    buf.open(QIODevice::WriteOnly);
    picture.save(&buf,"PNG");
    QSqlQuery query;
    //query.prepare("INSERT INTO Base(NF, Barcode, Info , Am) VALUES('234','234','wer','wer')");
    query.exec("SELECT * FROM Base WHERE Barcode="+Barcode+";");
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Con=query.value(rec.indexOf("Pid")).toString();
    }
    if(!Con.length()==1){

        query.prepare("INSERT INTO Base(NF, Barcode,Info , Am,Img) VALUES(?,?,?,?,?)");
        query.addBindValue(NF);
        query.addBindValue(Barcode);
        query.addBindValue(Info);
        query.addBindValue(Am);
        query.addBindValue(inByteArray);
        query.exec();
    }
    else{
        qDebug()<<"In Base";
    }
}

void MainWindow::onResult(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
}

void MainWindow::on_ScanButton_clicked()
{
    ui->Group1->hide();
    ui->textEdit->show();
    ui->textEdit->setFocus();
    ui->textEdit->selectAll();
}

void MainWindow::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toPlainText();
    if(text.length()>13)
    {
        ui->textEdit->hide();
        ui->Group1->show();
        ui->CellNumberCell->setFocus();
        //Для номера
        ui->BarcodeLabel->setText("ШК товара: "+text.trimmed());

        QSqlQuery query;
        QByteArray outByteArray;
        QPixmap outPixmap= QPixmap();
        query.exec("SELECT * FROM Base WHERE Barcode="+text.trimmed()+";");
        QSqlRecord rec = query.record();

        while(query.next())
        {
            ui->ProductIDFiled->setText(query.value(rec.indexOf("Pid")).toString());
            ui->NewcellField->setText(query.value(rec.indexOf("NF")).toString());
            ui->ProductInformationFiled->setText(query.value(rec.indexOf("Info")).toString());
            ui->ProductAmmountField->setText(query.value(rec.indexOf("Am")).toString());
            outByteArray=query.value(rec.indexOf("Img")).toByteArray();

        }
        outPixmap.loadFromData(outByteArray);
        ui->ImageRight->setPixmap(outPixmap);
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    barcodeM=barcodeM+e->key();
    if((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return))
    {
        QString lastKey = e->text();
        barcodeM=barcodeM.left(barcodeM.length() - 1);
        product();
        barcodeM="";
    }
}

void MainWindow::on_AddImageButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Select Image"), "/home/", tr("Image Files (*.png *.jpg)"));
    QPixmap picture(fileName);
    ui->ImageRight->setPixmap(picture);

}

void MainWindow::on_ChangeProductButton_clicked()
{
    QPixmap picture(fileName);
    QByteArray inByteArray;
    QBuffer buf(&inByteArray);
    buf.open(QIODevice::WriteOnly);
    picture.save(&buf,"PNG");
    QString Am = ui->ProductAmmountField->toPlainText();
    QString Info =ui->ProductInformationFiled->toPlainText();
    QSqlQuery query;
    QSqlRecord rec = query.record();
    query.prepare("UPDATE Base SET Info = :info,Am = :am,Nf = :nf WHERE Barcode="+ui->textEdit->toPlainText().trimmed()+";");
    query.bindValue(":info", Info);
    query.bindValue(":am", Am);
    query.bindValue(":nf", ui->NewcellField->toPlainText());
    if (query.exec())
    {
        qDebug()<<"ok";
    }
    else
    {
        qDebug()<<query.lastError().text();
    }
}

void MainWindow::on_DeleteProductButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    qDebug()<<port;
    QSqlQuery query;
    // query.exec("UPDATE Base SET Am = 20 WHERE Pid = 4;");
    query.exec("DELETE FROM Base WHERE Barcode="+text.trimmed()+";");
}

void MainWindow::on_addToList_clicked()
{
    placeid=ui->comboBox_2->currentText();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QTableWidgetItem *itemb=new QTableWidgetItem (ui->comboBox->currentText().split(" ").at(0));
    QTableWidgetItem *itemp=new QTableWidgetItem (placeid);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,itemb);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,itemp);
    // itemb->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    //  itemp->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    last = ui->tableWidget->rowCount();
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    qDebug()<<ui->comboBox->currentText();
}

void MainWindow::on_ConfigWindow_clicked()
{
    d = new Dialog();

    QObject::connect(d, SIGNAL(buttonPressed1(QString,qint32)), this, SLOT(changeText(QString,qint32)));
    // QObject::connect(d, SIGNAL(buttonPressed1(QString,qint32)), this, SLOT(changePort(qint32)));
    d->show();
}

void MainWindow::changeText(QString text,qint32 portvar)
{
    qDebug()<<"IP: "<<text;
    qDebug()<<"Port: "<<portvar;
    port=portvar;
    ip=text;
    delete d;
}

void MainWindow::newConnection()
{
    QTcpSocket *socket= server->nextPendingConnection();
    qDebug()<<"somedebugmessage";

    QByteArray sendmessage;
    sendmessage.append("HTTP/1.0 200 OK\r\n");
    sendmessage.append("Server: Valya_WMS_Emulator\r\n");
    //  sendmessage.append("X-Powered-By: PHP/5.2.4-2ubuntu5wm1\r\n");
    //  sendmessage.append("Last-Modified: Wed, 11 Feb 2009 11:20:59 GMT\r\n");
    //  sendmessage.append("Content-Language: ru\r\n");
    sendmessage.append("Content-Type: text/html; charset=utf-8\r\n");
    //  sendmessage.append("Content-Length: 1234\r\n");
    sendmessage.append("Connection: close\r\n");
    sendmessage.append("\r\n");
    sendmessage.append("OK\r\n");
    socket->write(sendmessage);
    qDebug()<<"sd1";
    socket->waitForBytesWritten(500);
    QByteArray data= socket->readAll();
    QList<QByteArray> lines  = data.split('\r\n');
    qDebug() << lines.at(6);
    QJsonDocument doc = QJsonDocument::fromJson(lines.at(6));
    //get the jsonObject
    QJsonObject jObject = doc.object();
    QJsonValue result = jObject.value("result");
    QJsonValue jv = jObject.value("task_id");
    if(result.toString()=="ok")
    {
        qDebug()<<jv.toInt();
        if(ui->tableWidget->rowCount() != 0)
        {
        ui->tableWidget->item(jv.toInt()-1,0)->setBackground(Qt::green);
        }
    }
    socket->close();
    //ui->tableWidget->item(light,0)->setBackground(Qt::green);
    qDebug()<<"sd2";
}

void MainWindow::pickproduct()
{
    QJsonObject json;
    QNetworkRequest request(QUrl("http://"+ip+"/wms/puttask"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkAccessManager nam;
    json.insert("order_id", 1);
    json.insert("type", "pick");
    QJsonObject taskpoint;
    QString num;
    QJsonArray taskarr;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        num=QString::number(i);
        QString place=ui->tableWidget->item(i,1)->text();
        taskpoint ["place_id"] = place ;//not 1,here was num
        taskpoint ["count"] = num;
        taskpoint ["task_id"] = i+1;
        taskarr.push_back(taskpoint);
    }
    json.insert("tasks",taskarr);
    QJsonDocument doc(json);
    qDebug() << doc.toJson();
    QNetworkReply *reply=  nam.post(request, doc.toJson());
    while(!reply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();
    qDebug()<<response_data;
}

void MainWindow::on_SetIP_clicked()
{
    QNetworkRequest request(QUrl("http://"+ip+"/wms/puttask"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("type", "set_ip");
    qDebug()<<ip;
    qDebug()<<port;
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());
    while(!reply->isFinished())
    {
        qApp->processEvents();
    }
    if(reply->readAll()=="ok")
    {
        ui->SetIP->setStyleSheet("background-color: #00FF00");
    }
    else
    {
        ui->SetIP->setStyleSheet("background-color: red");
    }
}
void MainWindow::isconnected()
{

}

void MainWindow::on_DeleteButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
