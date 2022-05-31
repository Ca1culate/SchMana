#include "dlgtea.h"
#include "ui_dlgtea.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QStandardItemModel>
#include <QPainter>
#include <QFont>
#include <QPainterPath>
extern QString theUser;

dlgTea::dlgTea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTea)
{
    ui->setupUi(this);
    resize(600,500);
    isShow = false;
    ui->tabWidget->setTabEnabled(2,false);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
}

dlgTea::~dlgTea()
{
    delete ui;
}

void dlgTea::readTeaInfo()
{
    //读取记录了教师、号码、密码的txt文件
    QFile teaFile("./data//teacherinfo.txt");
    if(!teaFile.exists())
    {
        QMessageBox::warning(this,tr("未找到教师信息"),tr("未找到教师信息记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!teaFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream teaStream(&teaFile);
    teaStream.setCodec("utf-8");
    QString str;
    list.clear();
    while (!teaStream.atEnd()) {
        str = teaStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        list.append(str.split(" "));
    }
    //qDebug() << list;
    teaFile.close();


    //读取课表信息

    QFile classFile("./data//classinfo.txt");
    if(!classFile.exists())
    {
        QMessageBox::warning(this,tr("未找到课程信息"),tr("未找到课表信息记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!classFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream classStream(&classFile);
    classStream.setCodec("utf-8");

    teaClassList.clear();
    while (!classStream.atEnd()) {
        str = classStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        if(str.contains(theUser))
        {

            teaClassList.append(str);
        }

    }
    //qDebug() << classList;
    classFile.close();
}

void dlgTea::arrClass()
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,tr("课程名称"));
    model->setHeaderData(1,Qt::Horizontal,tr("上课周次"));
    model->setHeaderData(2,Qt::Horizontal,tr("上课日期"));
    model->setHeaderData(3,Qt::Horizontal,tr("上课时间"));
    model->setRowCount(teaClassList.size());
    for (int i = 0;i < teaClassList.size() ;i++ ) {
        QString st = teaClassList.at(i);
        qDebug() << "st:" << st;
        QStringList sl = st.split(" ");
        qDebug() << "sl:" << sl;
        model->setHeaderData(i,Qt::Vertical,sl.at(1));
        model->setItem(i,0,new QStandardItem(sl.at(2)));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,1,new QStandardItem(sl.at(4)));
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,2,new QStandardItem(sl.at(5)));
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,3,new QStandardItem(sl.at(6)));
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }


    ui->tableView->setModel(model);
    //自适应内容大小
    //    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->show();
    isShow = true;

}

void dlgTea::setTeaInfo()
{
    int i  = list.indexOf(theUser);
    tea.setSerial(theUser);
    tea.setName(list.at(i+1));
    tea.setMajor(list.at(i+3));
    arrClass();
}

void dlgTea::setWelcome()
{
    QString strl = "欢迎您，";
    strl += tea.getName();
    strl += "老师";
    setWindowTitle(strl);
}

void dlgTea::on_pushButton_clicked()
{
    isShow = false;
    emit teaSendLogOut(true);
}


void dlgTea::on_pushButton_2_clicked()
{
    QString st = ui->lineEdit->text();
    if(st.isEmpty())
    {
        ui->textEdit->clear();
        return;
    }
    for (const auto &sl : teaClassList) {
        if(sl.contains(st)){
            ui->textEdit->append(sl);
        }
    }
}


void dlgTea::on_pushButton_3_clicked()
{

    //调课申请，
    QString st = ui->textEdit->toPlainText();
    if(!st.isEmpty())
    {
        int i = teaClassList.indexOf(st);

        if(i!=-1)
        {
            QString teaclass = teaClassList.at(i);
            qDebug() << teaclass;
            QStringList tealist = teaclass.split(" ");
            qDebug() << tealist;
            tealist.replace(5,ui->spinBox->text());
            qDebug() << tealist;
            tealist.replace(6,ui->spinBox_2->text());
            qDebug() << tealist;
            teaclass = tealist.join(" ");
            qDebug() << teaclass;
            /*
             * 修改classinfo.txt的代码
            teaClassList.replace(teaClassList.indexOf(st),teaclass);
            qDebug() << teaClassList;
            ui->textEdit->setText(teaclass);
            */


            QFile f("./data//changeclass.txt");

            if (f.open(QIODevice::ReadWrite | QIODevice::Text))
            {
                QTextStream stream(&f);
                stream.setCodec("utf-8");
                stream.seek(f.size());
                stream << teaclass << "\n";
                f.flush();
                f.close();
                //申请调课成功
                QMessageBox::information(this,tr("调课成功"),tr("申请调课成功"));
            }
            else
            {
                 QMessageBox::warning(this,tr("调课成功"),tr("申请调课成功"));
            }




        }

        return;
    }
}


void dlgTea::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit_2->setDate(QDate::currentDate());
    }
}


void dlgTea::on_pushButton_4_clicked()
{
    QDate dateStart = ui->dateEdit->date();
    QDate dateEnd = ui->dateEdit_2->date();
    int ds = dateStart.daysTo(dateEnd);

    if(ds>0){
        qDebug() << ds;
        QMessageBox::information(this,tr("请假申请"),tr("提交请假申请成功，请等待管理员审批！"));
        QFile f("./data//holiday.txt");

        if (f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&f);
            stream.setCodec("utf-8");
            stream.seek(f.size());
            stream << tea.getSerial() << " " << tea.getName() << " " << dateStart.toString("yyyy-MM-dd") << " " << dateEnd.toString("yyyy-MM-dd") << " " << QString::number( ds) << " " << tr("否") << "\n";
            qDebug() << ds;
            f.flush();
            f.close();
        }
    }
    else
    {
        QMessageBox::warning(this,tr("日期错误"),tr("请选择正确的起始时间！"));
    }

}

