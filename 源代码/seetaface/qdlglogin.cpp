#include "qdlglogin.h"
#include "ui_qdlglogin.h"

extern QString userID;
extern int exitCode;
QDlgLogin::QDlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDlgLogin)
{
    ui->setupUi(this);
    ui->editPswd->setEchoMode(QLineEdit::Password);//密码输入不可见

    ui->editUser->setText("chen");
    ui->editPswd->setText("123");

}

QDlgLogin::~QDlgLogin()
{
    delete ui;
}

void QDlgLogin::on_btnOk_clicked()
{

    QString user = ui->editUser->text().trimmed();//输入用户名
    QString pwd = ui->editPswd->text().trimmed();//输入密码

    QSqlTableModel model;
    model.setTable("admin");
    model.setFilter(tr("name = '%1' and pwd = '%2' ").arg(user).arg(pwd));

    //根据条件筛选记录
    model.select();

    if(model.rowCount()==1)
    {
        Operator = user;
        QMessageBox::information(this, "登录", "登录成功！",QMessageBox::Ok);
        this->accept();//关闭登录界面
    }
    else
    {
        m_tryCount++;
        if(m_tryCount>3)
        {
            QMessageBox::critical(this,"错误","输入错误次数太多，强行退出");
            exitCode = 0;
            this->reject();//退出
        }
        else
            QMessageBox::warning(this,"错误提示","用户名或密码错误");
    }
}


void QDlgLogin::on_btnExit_clicked()
{
    exitCode = 0;
    this->close();
}
