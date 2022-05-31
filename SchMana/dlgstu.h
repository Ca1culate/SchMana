#ifndef DLGSTU_H
#define DLGSTU_H

#include <QDialog>
#include "student.h"
#include <QStandardItemModel>


namespace Ui {
class dlgStu;
}

class dlgStu : public QDialog
{
    Q_OBJECT

public:
    explicit dlgStu(QWidget *parent = nullptr);
    ~dlgStu();
    void setWelcome();
    //设置个人信息页面
    void setPerInfo();
    //设置个人课表
    void setPerTimetable();
    //转专业函数
    void setNewMajor();

    Student stu;
    //存储学生信息的list
    QStringList list;
    //存储课程信息的list
    QStringList classList;
    //存储专业信息的list
    QStringList majorList;
    void readStuInfo();


    QStandardItemModel *model;

    bool isShow;



signals:
    //传递注销指令
    void sendLogOut(bool isLogout);
private slots:
    //槽函数

    void on_pushButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::dlgStu *ui;
};

#endif // DLGSTU_H
