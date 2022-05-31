#ifndef DLGADM_H
#define DLGADM_H

#include <QDialog>

namespace Ui {
class dlgAdm;
}

class dlgAdm : public QDialog
{
    Q_OBJECT

public:
    explicit dlgAdm(QWidget *parent = nullptr);
    ~dlgAdm();
    QStringList strList;
    bool isShow;
    //显示员工账号
    void disAccount();
    void dealClass();
    void dealMajor();
    //请假列表信息
    QStringList holidayList;
    //调课信息列表
    QStringList changeClassList;
    //课程安排信息列表
    QStringList classList;
    //转专业信息列表
    QStringList majorList;
    //个人信息列表
    QStringList stuList;
    //请假信息迭代器
    int it_holiday;
    //调课信息迭代器
    int it_changeclass;
    //转专业信息迭代器
    int it_changemajor;
    //login.txt登录信息
    QStringList list;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_toolBox_currentChanged(int index);


    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

signals:
    //传递注销指令
    void admSendLogOut(bool isShow);

private:
    Ui::dlgAdm *ui;
};

#endif // DLGADM_H
