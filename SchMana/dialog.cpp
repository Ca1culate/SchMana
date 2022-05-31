#include "dialog.h"
#include "ui_dialog.h"
#include <QIcon>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QIODevice>
#include <QStringList>
#include <QMouseEvent>
#include <QPainter>


//全局变量分配配置
QString theUser;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //去除窗口边框
        setWindowFlags(Qt::FramelessWindowHint | windowFlags());

        //设置窗口背景为透明
              setAttribute(Qt::WA_TranslucentBackground);

    ui->m_user->setText(tr("admin"));
    ui->m_user->setEnabled(false);

    this->setWindowIcon(QIcon(":/img/img/Nwafu.ico"));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_btnLogIn_clicked()
{
    bool loginSuccess = false;
    if(ui->m_role->currentText() != tr("管理员") && (ui->m_user->text().isEmpty() || ui->m_passwd->text().isEmpty()))
        QMessageBox::warning(this,tr("不能为空"),tr("请输入用户名和密码"));
    else if(ui->m_role->currentText() == tr("管理员") && (ui->m_passwd->text().isEmpty()))
        QMessageBox::warning(this,tr("不能为空"),tr("请输入用户名和密码"));
    else
    {
        //        if(ui->m_role->currentText() == tr("管理员"))
        //            loginSuccess = isLogin(ui->m_role->currentText(),tr("1000"),ui->m_passwd->text());

        //        else
        loginSuccess = isLogin(ui->m_role->currentText(),ui->m_user->text(),ui->m_passwd->text());
    }

    if(loginSuccess && ui->m_role->currentText() == tr("学生"))
    {
        this->hide();
        theUser = ui->m_user->text();
        qDebug() << theUser;
        connect(&dlgstu,SIGNAL(sendLogOut(bool)),this,SLOT(receiveLogOut()));
        dlgstu.readStuInfo();
        dlgstu.setPerInfo();
        //dlgstu.setPerTimetable();
        dlgstu.setNewMajor();
        dlgstu.setModal(true);
        dlgstu.show();
    }
    else if (loginSuccess && ui->m_role->currentText() == tr("教师")){
        this->hide();
        theUser = ui->m_user->text();
        connect(&dlgtea,SIGNAL(teaSendLogOut(bool)),this,SLOT(receiveTeaLogOut()));

        dlgtea.readTeaInfo();
        dlgtea.setTeaInfo();
        dlgtea.setWelcome();
        dlgtea.setModal(true);
        dlgtea.show();
    }
    else if(loginSuccess)
    {
        this->hide();
        theUser = ui->m_user->text();
        connect(&dlgadm,SIGNAL(admSendLogOut(bool)),this,SLOT(receiveAdmLogOut()));
        dlgadm.dealClass();
        dlgadm.dealMajor();
        dlgadm.setModal(true);
        dlgadm.show();
    }

}


void Dialog::on_m_role_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("管理员"))
    {
        ui->m_user->setText(tr("admin"));
        ui->m_user->setEnabled(false);
    }

    else
    {
        ui->m_user->setEnabled(true);
        ui->m_user->clear();
    }

}

void Dialog::receiveLogOut()
{
    dlgstu.close();
    this->show();
    this->ui->m_passwd->clear();
}

void Dialog::receiveTeaLogOut()
{
    dlgtea.close();
    this->show();
    this->ui->m_passwd->clear();
}

void Dialog::receiveAdmLogOut()
{
    dlgadm.close();
    this->show();
    this->ui->m_passwd->clear();
}

bool Dialog::isLogin(QString arg1,QString arg2,QString arg3)
{
    //读取记录了角色、号码、密码的txt文件
    QFile loginFile("./data//login.txt");
    if(!loginFile.exists())
    {
        QMessageBox::warning(this,tr("未找到登录信息"),tr("未找到学号密码记录文件，请检查对应路径下是否存在该文件！"));
        return false;
    }
    if(!loginFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return false;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream loginStream(&loginFile);
    loginStream.setCodec("utf-8");
    QString str;
    QStringList list;
    while (!loginStream.atEnd()) {
        str = loginStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        list.append(str.split(" "));
    }
    qDebug() << list;
    //判断用户名角色和密码是否一致
    int i  = list.indexOf(arg2);
    if((i != -1 && list.at(i-1) == arg1) && (list.at(i+1) == arg3))
        return true;
    else
    {
        QMessageBox::warning(this,tr("未找到登录信息"),tr("请核对角色的账号密码后重新输入！"));
        return false;
    }

}

void Dialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        //移到左上角
        move(e->globalPos() - p);
    }

}

