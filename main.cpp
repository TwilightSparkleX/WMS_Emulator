#include "mainwindow.h"
#include <QDomDocument>
#include <QApplication>
#include<dialog.h>
int main(int argc, char *argv[])
{
//    QString pathfordriver=QDir::currentPath();
//    pathfordriver.append("/plugins");
//    QCoreApplication::addLibraryPath(pathfordriver);
//    qDebug()<<pathfordriver;
    QApplication a(argc, argv);
    MainWindow w;


    w.show();
    //d.show();

//    db.setUserName ("elton");
//    db.setHostName("epica");
//    db. setPassword ( "password") ;
//    QDomDocument doc ( "addressbook");
//    QDomElement domElement = doc.createElement("addressbook");
//    doc.appendChild(domElement);
//    QDomElement contactl = contact(doc, "Piggy", "+49 631322187", "piggy@mega.de");
//    QDomElement contact2 = contact (doc, "Kermit", "+49 631322181", "kermit@mega.de");
//    QDomElement contact3 = contact (doc, "Gonzo", "+49 631322186", "gonzo@mega.de");

//    domElement.appendChild(contactl);
//    domElement.appendChild(contact2);
//    domElement.appendChild(contactЗ);
//    QFile file ("addressbook.xml");
//    if(file.open(QIODevice::WriteOnly)){

//        QTextStream(&file) << doc.toString();
//        file.close();
//    }
    return a.exec();
}

