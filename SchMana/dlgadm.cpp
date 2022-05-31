#include "dlgadm.h"
#include "ui_dlgadm.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItemModel>
dlgAdm::dlgAdm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgAdm)
{
    ui->setupUi(this);
    isShow = false;
    it_holiday = 1;
    it_changeclass = 1;
    it_changemajor = 1;
    ui->widget->setVisible(false);
}

dlgAdm::~dlgAdm()
{
    delete ui;
}

void dlgAdm::disAccount()
{

    list.clear();
    strList.clear();
    //读取记录了角色、号码、密码的txt文件
    QFile loginFile("./data//login.txt");
    if(!loginFile.exists())
    {
        QMessageBox::warning(this,tr("员工信息"),tr("未找到账号密码记录文件，请检查对应路径下是否存在该文件！"));
        return ;
    }
    if(!loginFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    //用文件流的方式打开并按行读取，信息放入qstringlist
    QTextStream loginStream(&loginFile);
    loginStream.setCodec("utf-8");
    QString str;

    while (!loginStream.atEnd()) {
        str = loginStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        list.append(str);
        strList.append(str.split(" "));
    }


    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,tr("角色"));
    model->setHeaderData(1,Qt::Horizontal,tr("员工号/学号"));
    model->setHeaderData(2,Qt::Horizontal,tr("密码"));
    model->setRowCount(list.size());
    for (int i = 0;i < list.size() ;i++ ) {
        QString st = list.at(i);
        //qDebug() << "st:" << st;
        QStringList sl = st.split(" ");
        //qDebug() << "sl:" << sl;
        model->setItem(i,0,new QStandardItem(sl.at(0)));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,1,new QStandardItem(sl.at(1)));
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,2,new QStandardItem(sl.at(2)));
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
    }


    ui->tableView->setModel(model);
    //自适应内容大小
    //    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->show();
    isShow = true;

}




void dlgAdm::dealClass()
{
    changeClassList.clear();
    QFile f("./data//changeclass.txt");
    if(!f.exists())
    {
        QMessageBox::warning(this,tr("未找到申请调课信息"),tr("未找到调课记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    QTextStream fStream(&f);
    fStream.setCodec("utf-8");
    QString str;
    while (!fStream.atEnd()) {
        str = fStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        changeClassList.append(str);
    }
    f.close();
    //qDebug() << changeClassList;
    if(changeClassList.isEmpty())
        ui->textEdit_2->setText(tr("无待审批的调课信息"));
    else
        ui->textEdit_2->setText(changeClassList.at(0));
    ui->pushButton_9->setEnabled(false);
    f.setFileName("./data//classinfo.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        while (!stream.atEnd()) {
            str = stream.readLine();
            auto s = str.trimmed();
            Q_UNUSED(s)
            classList.append(str);
        }
        f.close();
    }

}

void dlgAdm::dealMajor()
{
    majorList.clear();
    QFile f("./data//changemajor.txt");
    if(!f.exists())
    {
        QMessageBox::warning(this,tr("未找到转专业信息"),tr("未找到转专业记录文件，请检查对应路径下是否存在该文件！"));
        return;
    }
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("未获得权限"),tr("缺少打开的权限"));
        return;
    }
    QTextStream fStream(&f);
    fStream.setCodec("utf-8");
    QString str;
    while (!fStream.atEnd()) {
        str = fStream.readLine();
        auto s = str.trimmed();
        Q_UNUSED(s)
        majorList.append(str);
    }
    f.close();
    if(majorList.isEmpty())
    {
        ui->textEdit_3->setText(tr("无待审批的转专业信息"));
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
    }
    else
    {

        ui->textEdit_3->setText(majorList.at(0));
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_12->setEnabled(false);
    }
    ui->pushButton_12->setEnabled(false);
    f.setFileName("./data//studentinfo.txt");
    if (f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        while (!stream.atEnd()) {
            str = stream.readLine();
            auto s = str.trimmed();
            Q_UNUSED(s)
            stuList.append(str);
        }
        f.close();
    }

}

void dlgAdm::on_pushButton_clicked()
{
    disAccount();
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()|| ui->lineEdit_3->text().isEmpty()|| ui->lineEdit_4->text().isEmpty())
    {
        QMessageBox::warning(this,tr("存在空项"),tr("不能为空"));
        return;
    }
    else if(strList.contains(ui->lineEdit->text()))
    {
        QMessageBox::information(this,tr("添加失败"),tr("添加教师失败,请检查是否存在相同工号"));
        return;
    }
    else
    {
        QFile f("./data//teacherinfo.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&f);
            stream.setCodec("utf-8");
            stream.seek(f.size());
            QString str = ui->lineEdit->text() + " " + ui->lineEdit_2->text() + " " + ui->lineEdit_3->text() + " " + ui->lineEdit_4->text();
            //qDebug() << str;
            stream << str << "\n";
            //f.flush();
            f.close();
        }
        f.setFileName("./data//login.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&f);
            stream.setCodec("utf-8");
            stream.seek(f.size());
            QString str = "教师 " + ui->lineEdit->text() + " " + ui->lineEdit_3->text();
            //qDebug() << str;
            stream << str << "\n";
            //f.flush();
            f.close();
        }
        QMessageBox::information(this,tr("添加成功"),tr("添加教师成功"));
    }
}


void dlgAdm::on_pushButton_2_clicked()
{
    disAccount();
    if(ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty()|| ui->lineEdit_7->text().isEmpty()|| ui->lineEdit_8->text().isEmpty()|| ui->lineEdit_9->text().isEmpty()|| ui->lineEdit_10->text().isEmpty())
    {
        QMessageBox::warning(this,tr("存在空项"),tr("不能为空"));
        return;
    }
    else if(strList.contains(ui->lineEdit_7->text()))
    {
        QMessageBox::information(this,tr("添加失败"),tr("添加学生失败，请检查是否存在相同学号"));
        return;
    }
    else
    {
        QFile f("./data//studentinfo.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&f);
            stream.setCodec("utf-8");
            stream.seek(f.size());
            QString str = ui->lineEdit_7->text() + " " + ui->lineEdit_8->text() + " " + ui->lineEdit_5->text() + " " + ui->lineEdit_6->text() + " " + ui->lineEdit_9->text() + " " + ui->lineEdit_10->text() + " " + ui->comboBox->currentText();
            //qDebug() << str;
            stream << str << "\n";
            //f.flush();
            f.close();
        }
        f.setFileName("./data//login.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&f);
            stream.setCodec("utf-8");
            stream.seek(f.size());
            QString str = "学生 " + ui->lineEdit_7->text() + " " + ui->lineEdit_5->text();
            //qDebug() << str;
            stream << str << "\n";
            //f.flush();
            f.close();
        }
        QMessageBox::information(this,tr("添加成功"),tr("添加学生成功"));
    }
}


void dlgAdm::on_pushButton_3_clicked()
{
    isShow = false;
    emit admSendLogOut(true);
}


void dlgAdm::on_toolBox_currentChanged(int index)
{
    if(index == 2)
        disAccount();
}


void dlgAdm::on_pushButton_4_clicked()
{

    //通过申请
    QString st = ui->textEdit->toPlainText();
    QStringList sl = st.split(" ");
    sl.replace(5,tr("是"));
    int index = holidayList.indexOf(st);
    st = sl.join(" ");
    holidayList.replace(index,st);
    QFile f("./data//holiday.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());
        //qDebug() << str;
        for (const auto s : holidayList) {
            stream << s << "\n";
        }
        //f.flush();
        f.close();
    }
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

}

void dlgAdm::on_pushButton_5_clicked()
{
    //拒绝申请
    QString st = ui->textEdit->toPlainText();
    QStringList sl = st.split(" ");
    sl.replace(5,tr("未通过"));
    int index = holidayList.indexOf(st);
    st = sl.join(" ");
    holidayList.replace(index,st);
    QFile f("./data//holiday.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());
        for (const auto s : holidayList) {
            stream << s << "\n";

        }
        //f.flush();
        f.close();
    }
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
}

void dlgAdm::on_pushButton_6_clicked()
{
    //下一条
    ui->textEdit->clear();
    if(it_holiday<holidayList.size() && holidayList.size()!=0)
    {
        ui->textEdit->setText(holidayList.at(it_holiday));
        it_holiday++;
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
    }
    else
    {
        ui->textEdit->setText(tr("审批完成"));
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    }


}

void dlgAdm::on_pushButton_7_clicked()
{
    //调课信息通过
    //通过申请
    QString st = ui->textEdit_2->toPlainText();
    QStringList sl = st.split(" ");
    int index = 0;
    for (index = 0;index < classList.size();index++)
    {
        QStringList slClass = classList.at(index).split(" ");
        if(slClass.at(1) == sl.at(1))
        {
            classList.replace(index,st);

            break;
        }

    }
    QFile f("./data//classinfo.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());
        //qDebug() << str;
        for (const auto s : classList) {
            stream << s << "\n";

        }
        f.close();
    }
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_7->setEnabled(false);



}

void dlgAdm::on_pushButton_8_clicked()
{
    //调课信息未通过
    //拒绝申请
    QString st = ui->textEdit_2->toPlainText();
    QStringList sl = st.split(" ");
    //int index = changeClassList.indexOf(st);
    changeClassList.removeOne(st);

    QFile f("./data//changeclass.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());

        for (const auto s : changeClassList) {
            stream << s << "\n";
        }
        f.close();
    }
    it_changeclass--;
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_7->setEnabled(false);


}

void dlgAdm::on_pushButton_9_clicked()
{
    //下一条
    ui->textEdit_2->clear();
    if(it_changeclass<changeClassList.size() && changeClassList.size()!=0)
    {
        ui->textEdit_2->setText(changeClassList.at(it_changeclass));
        it_changeclass++;
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
    }
    else
    {
        ui->textEdit_2->setText(tr("审批完成"));
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);

    }

}

void dlgAdm::on_pushButton_10_clicked()
{
    //同意通过转专业申请
    //调课信息通过
    //通过申请
    QString st = ui->textEdit_3->toPlainText();

    QStringList sl = st.split(" ");
    QStringList str_;

    for(int ii = 0;ii < sl.size()-1 ; ii++)
    {
        str_.append(sl.at(ii));
    }
    QStringList majorInfoList;
    int index = 0;
    for (index = 0;index < stuList.size();index++)
    {
        QStringList slClass = stuList.at(index).split(" ");
        if(slClass.at(0) == sl.at(0))
        {
            QString str__ = str_.join(" ");
            stuList.replace(index,str__);
            break;
        }
    }
    QFile f("./data//studentinfo.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());
        //qDebug() << str;
        for (const auto s : stuList) {
            stream << s << "\n";
        }
        f.close();
        //qDebug() << stuList;
    }
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    it_changemajor--;

    //清除一条转专业信息
    majorList.removeOne(st);

    f.setFileName("./data//changemajor.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());

        for (const auto s : majorList) {
            stream << s << "\n";
        }
        f.close();
    }



    //专业信息减1
    f.setFileName("./data//majorinfo.txt");
    if (f.open(QIODevice::ReadWrite | QIODevice::Text ))
    {
        QString str;
        QTextStream stream(&f);

        stream.setCodec("utf-8");
        //qDebug() << str;
        while (!stream.atEnd()) {
            str = stream.readLine();
            auto s = str.trimmed();
            Q_UNUSED(s)
            majorInfoList.append(str.split(" "));

        }
        qDebug() << majorInfoList;
        index = majorInfoList.indexOf(sl.at(3));
        //转入专业信息减一
        if(index!=-1)
        {
            majorInfoList.replace(index+2,QString::number(majorInfoList.at(index+2).toInt() + 1));
            majorInfoList.replace(index+3,QString::number(majorInfoList.at(index+3).toInt() - 1));
        }
        //转出专业信息加一
        index = majorInfoList.indexOf(sl.at(7));

        if(index != -1)
        {
            majorInfoList.replace(index+2,QString::number(majorInfoList.at(index+2).toInt() -1 ));//已有人数
            majorInfoList.replace(index+3,QString::number(majorInfoList.at(index+3).toInt() + 1));//剩余容量
        }
        f.close();
    }
    f.setFileName("./data//majorinfo.txt");
    if(f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        QStringList ll;
        stream.setCodec("utf-8");
        stream.seek(f.size());
        for (index = 0;index<majorInfoList.size()-1 ;index+=5 ) {
            st = majorInfoList.at(index) + " " + majorInfoList.at(index+1) + " " + majorInfoList.at(index+2) + " " + majorInfoList.at(index+3) + " " +majorInfoList.at(index+4);
            stream << st << "\n";
        }
        f.close();
    }



}


void dlgAdm::on_pushButton_11_clicked()
{
    //转专业信息未通过
    //拒绝申请转专业
    QString st = ui->textEdit_3->toPlainText();
    QStringList sl = st.split(" ");
    //int index = changeClassList.indexOf(st);
    majorList.removeOne(st);

    QFile f("./data//changemajor.txt");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream(&f);
        stream.setCodec("utf-8");
        stream.seek(f.size());

        for (const auto s : majorList) {
            stream << s << "\n";
        }
        f.close();
    }
    it_changemajor--;
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
}


void dlgAdm::on_pushButton_12_clicked()
{
    //下一条转专业信息
    //下一条
    ui->textEdit_3->clear();
    if(it_changemajor<majorList.size() && majorList.size()!=0)
    {
        qDebug() << it_changemajor;
        ui->textEdit_3->setText(majorList.at(it_changemajor));
        it_changemajor++;
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
    }
    else
    {
        ui->textEdit_3->setText(tr("审批完成"));
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        QFile f("./data//changemajor.txt");
        if (f.open(QIODevice::WriteOnly | QIODevice::Text ))
        {
            f.close();
        }

    }

}

