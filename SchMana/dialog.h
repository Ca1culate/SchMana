#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "student.h"
#include "dlgstu.h"
#include "dlgtea.h"
#include "dlgadm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    dlgTea dlgtea;
    dlgStu dlgstu;
    dlgAdm dlgadm;
    bool isLogin(QString arg1,QString arg2,QString arg3);

protected:

    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
        void mousePressEvent(QMouseEvent *e);//鼠标按下移动


private slots:
    void on_btnLogIn_clicked();

    void on_m_role_currentIndexChanged(const QString &arg1);

    //学生注销
    void receiveLogOut();

    //教师注销
    void receiveTeaLogOut();

    //管理员注销
    void receiveAdmLogOut();

private:
    Ui::Dialog *ui;

    QPoint p;
};
#endif // DIALOG_H
