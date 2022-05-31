#ifndef DLGTEA_H
#define DLGTEA_H

#include <QDialog>
#include <QStringList>

#include "teacher.h"
namespace Ui {
class dlgTea;
}

class dlgTea : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTea(QWidget *parent = nullptr);
    ~dlgTea();

    void readTeaInfo();
    void arrClass();
    void setTeaInfo();
    void setWelcome();

    Teacher tea;

    bool isShow;

    QStringList list;

    QStringList teaClassList;

signals:
    //传递注销指令
    void teaSendLogOut(bool isLogout);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_4_clicked();

private:
    Ui::dlgTea *ui;
};

#endif // DLGTEA_H
