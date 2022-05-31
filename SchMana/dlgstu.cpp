#include "dlgstu.h"
#include "ui_dlgstu.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>


extern QString theUser;

dlgStu::dlgStu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgStu)
{
    ui->setupUi(this);
    resize(400,300);
    //QMessageBox::warning(this,tr("111"),tr("11111"));
    isShow = false;
    //qDebug() << theUser;
}

dlgStu::~dlgStu()
{
    delete ui;
}

void dlgStu::setWelcome()
{
    QString strl = "欢迎你，";
    strl += stu.getName();
    setWindowTitle(strl);
}

void dlgStu::setPerInfo()
{
    //判断用户名角色和密码是否一致
    int i  = list.indexOf(theUser);
    qDebug() << i;
    stu.setSerial(theUser);
    stu.setName(list.at(i+1));
    stu.setMajor(list.at(i+3));
    stu.setScore(list.at(i+5));
    stu.setClass(list.at(i+4));
    stu.setNewMajor(list.at(i+6));
    this->ui->lineEdit->setText(stu.getName());
    this->ui->lineEdit_2->setText(stu.getSerial());
    this->ui->lineEdit_3->setText(stu.getMajor());
    this->ui->lineEdit_4->setText(stu.getClass());
    this->ui->lineEdit_5->setText(stu.getScore());
    this->ui->lineEdit_6->setText(stu.getMajor());


}

void dlgStu::setPerTimetable()
{

    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,tr("星期一"));
    model->setHeaderData(1,Qt::Horizontal,tr("星期二"));
    model->setHeaderData(2,Qt::Horizontal,tr("星期三"));
    model->setHeaderData(3,Qt::Horizontal,tr("星期四"));
    model->setHeaderData(4,Qt::Horizontal,tr("星期五"));

    model->setRowCount(7);
    model->setHeaderData(0,Qt::Vertical,tr("第一节课"));
    model->setHeaderData(1,Qt::Vertical,tr("第二节课"));
    model->setHeaderData(2,Qt::Vertical,tr("第三节课"));
    model->setHeaderData(3,Qt::Vertical,tr("第四节课"));
    model->setHeaderData(4,Qt::Vertical,tr("第五节课"));
    model->setHeaderData(5,Qt::Vertical,tr("第六节课"));
    model->setHeaderData(6,Qt::Vertical,tr("第七节课"));

    QList<QString>::iterator it=classList.begin(),itend = classList.end();
    QString str;
    for ( ; it!=itend ;it = it+8) {
        if(*it == stu.getMajor())
        {
            str = *(it+1)+"\n" + *(it+2) + "\n学分:" + *(it+3) + "\n周次" + *(it+4);
            model->setItem((*(it+6)).toInt(),(*(it+5)).toInt(),new QStandardItem(str));
            model->item((*(it+6)).toInt(),(*(it+5)).toInt())->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui->tableView->setModel(model);
    //自适应内容大小
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->show();
    isShow = true;
    qDebug() << tr("setPerTimetable成功");



}

void dlgStu::setNewMajor()
{
    //ui->comboBox->addItems()
    ui->comboBox->clear();
    QStringList comList;
    int index = 1;


    for ( ; index < majorList.size() ;index = index+5) {
        qDebug() << majorList.at(index);
        qDebug() << tr("迭代器");
        comList.append( majorList.at(index));
    }
    qDebug() << comList;
    ui->comboBox->addItems(comList);
    //int i = majorList.indexOf()

}

void dlgStu::readStuInfo()
{
    //读取记录了角色、号码、密码的txt文件
    QFile stuFile("./data//studentinfo.txt");
    if(!stuFile.exists())
    {
        QMessageBox::warning(this,tr("未找到学生信息"),tr("未找到学生信息记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!stuFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream loginStream(&stuFile);
    loginStream.setCodec("utf-8");
    QString str;
    list.clear();
    while (!loginStream.atEnd()) {
        str = loginStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        list.append(str.split(" "));
    }
    //qDebug() << list;
    stuFile.close();


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

    classList.clear();
    while (!classStream.atEnd()) {
        str = classStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        classList.append(str.split(" "));
    }
    //qDebug() << classList;
    classFile.close();

    //读取专业信息
    QFile majorFile("./data/majorinfo.txt");
    if(!majorFile.exists())
    {
        QMessageBox::warning(this,tr("未找到专业信息"),tr("未找到专业信息记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!majorFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream majorStream(&majorFile);
    majorStream.setCodec("utf-8");

    majorList.clear();
    while (!majorStream.atEnd()) {
        str = majorStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        majorList.append(str.split(" "));
    }    
    majorFile.close();

}



void dlgStu::on_pushButton_clicked()
{
    isShow = false;
    emit sendLogOut(true);
}


void dlgStu::on_tabWidget_currentChanged(int index)
{
    if(index == 1 && (!isShow))
        setPerTimetable();
}


void dlgStu::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int i = majorList.indexOf(arg1);
    ui->lineEdit_7->setText(majorList.at(i+3));
    if(majorList.at(i+3).toInt() > 0)
        ui->pushButton_2->setEnabled(true);
    else
        ui->pushButton_2->setEnabled(false);;
}


void dlgStu::on_pushButton_2_clicked()
{

    //申请转专业
    QString st = ui->comboBox->currentText();
    QStringList sl;
    sl.append(list.at(0));
    sl.append(list.at(1));
    sl.append(list.at(2));
    sl.append(st);
    sl.append(list.at(4));
    sl.append(list.at(5));
    sl.append(list.at(6));

    sl.append(ui->lineEdit_6->text());
    st = sl.join(" ");
    QFile f("./data//changemajor.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());
        stream << st << "\n";
        f.flush();
        f.close();
        QMessageBox::information(this,tr("成功"),tr("转专业申请成功"));
    }



}

