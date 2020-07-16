#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include "qdlglogin.h"

int exitCode = 0;//系统关闭时状态码
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("seetaface.db");

    if (!db.open())
    {
        QMessageBox::critical(NULL,"连接结果", "连接数据库失败");
        return 0;
    }

    QDlgLogin *dlgLogin = new QDlgLogin;

    do{
        if(dlgLogin->exec()==QDlgLogin::Accepted )
        {
            MainWindow w;
            w.setWindowTitle("SeetaFace");
            w.setWindowIcon(QIcon(":/new/prefix1/logo.png"));
            w.show();
            exitCode  = a.exec();
        }
       }while( exitCode  == 888  );

    return a.exec();
}
