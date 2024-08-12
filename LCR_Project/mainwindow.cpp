#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSerialPortInfo"
#include <QSerialPort>
#include <QMessageBox>
#include <QDateTime>
#include <qmath.h>

#define FLAG_OPEN 0
#define FLAG_C 1
#define FLAG_L 2
#define FLAG_R 3
#define FLAG_DF 4
#define FLAG_DB 5
#define FLAG_ESR 6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList serialNamePort;

    serialPort = new QSerialPort(this);
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(manual_serialPortReadyRead())); /*手动连接槽函数*/

    /*找出当前连接的串口并显示到serialCb*/
    // foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    //{
    // serialNamePort<<info.portName();// 自动扫描当前可用串口，返回值追加到字符数组中
    //}
    // ui->serialCb->addItems(serialNamePort);// 可用串口号，显示到串口选择下拉框中
    ui->serialCb->clear();
    // 通过QSerialPortInfo查找可用串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->serialCb->addItem(info.portName());
    }

    // 发送、接收计数清零
    sendNum = 0;
    recvNum = 0;
    // 状态栏
    QStatusBar *sBar = statusBar();
    // 状态栏的收、发计数标签
    lblSendNum = new QLabel(this);
    lblRecvNum = new QLabel(this);
    lblPortState = new QLabel(this);
    lblPortState->setText("Connected");
    // 设置串口状态标签为绿色 表示已连接状态
    lblPortState->setStyleSheet("color:red");

    // 设置标签最小大小
    lblSendNum->setMinimumSize(100, 20);
    lblRecvNum->setMinimumSize(100, 20);
    lblPortState->setMinimumSize(550, 20);
    setNumOnLabel(lblSendNum, "S: ", sendNum);
    setNumOnLabel(lblRecvNum, "R: ", recvNum);
    // 从右往左依次添加
    sBar->addPermanentWidget(lblPortState);
    sBar->addPermanentWidget(lblSendNum);
    sBar->addPermanentWidget(lblRecvNum);

    // 定时发送-定时器
    timSend = new QTimer;
    timSend->setInterval(1000); // 设置默认定时时长1000ms

    connect(timSend, &QTimer::timeout, this, [=]()
    { on_sendBt_clicked(); });

    // 定时输入间隔-定时器
    timSend = new QTimer;
    timSend->setInterval(50); // 设置默认定时时长50ms
    connect(timSend, &QTimer::timeout, this, [=]()
    {  });

    //设置显示位数
    ui->lcdNumber->setDigitCount(5);
    ui->lcdNumber->setMode(QLCDNumber::Dec);
    //设置小数点占的空间： true 1 false 2
    ui->lcdNumber->setSmallDecimalPoint(true);
    //    ui->lcdNumber->display("120.45UF");

    //    QByteArray array = "B:1.54V";
    //    receive_process(array);
    //    updateLCD(array);
    //ui->label_4->setText("元件类型：二极管 上负下正");

    //    QImage image("D:/QT_Project/LCR_Project/D.png");

    //    ui->picLabel->setPixmap(QPixmap::fromImage(image));
    //    ui->picLabel->resize(100,100);
    //    ui->picLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 检测通讯端口槽函数
void MainWindow::on_btnSerialCheck_clicked()
{
    ui->serialCb->clear();
    // 通过QSerialPortInfo查找可用串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->serialCb->addItem(info.portName());
    }
}

QByteArray MainWindow::filterAndConvertToFloat(QByteArray* byteArray) {
    QByteArray numericOnly;
    for (int i = 0; i < byteArray->size(); ++i) {
        char ch = byteArray->at(i); // 使用at()方法获取特定位置的字符
        if ((ch >= '0' && ch <= '9') || ch == '.') {
            numericOnly.append(ch);
        }
    }

    return numericOnly;
}

void MainWindow::receive_process(QByteArray array)
{
    char firstByte;
    if (!array.isEmpty()) {
        // 安全地获取第一个元素
        firstByte = array.at(0);
    }

    // 遍历后两个元素进行检测

    // 检测元素的值，例如检查是否为 'A'
    if (firstByte == 'C')
    {
        flag_LCRD = FLAG_C;

    }
    else if(firstByte == 'L')
    {
        flag_LCRD = FLAG_L;
    }
    else if(firstByte == 'R')
    {
        flag_LCRD = FLAG_R;
    }
    // 检测元素的值，例如检查是否为 'A'
    else if (firstByte == 'F')
    {
        flag_LCRD = FLAG_DF;
    }
    else if(firstByte == 'B')
    {
        flag_LCRD = FLAG_DB;
    }
    else if(firstByte == 'E')
    {
        flag_LCRD = FLAG_ESR;
    }
    else if(firstByte == 'O')
    {
        flag_LCRD = FLAG_OPEN;
    }

}

void MainWindow::updateLCD(QByteArray array)
{
    //    char lastChar = byteArray[byteArray.size() - 1];

    int SIZE = array.size();
    int i = 0;
    for(i = 0; i < SIZE; i++)
    {
        if(array[i] == '\0')
            break;
    }
    int len = i;
    char secondLastChar = array[len - 2];
    char fourthLastChar = array[len - 4];

    QByteArray floatNumber = filterAndConvertToFloat(&array);

    if(flag_LCRD == FLAG_ESR)
    {

        ui->unitLabel_2->setText("Ω");
        QString numberStr = QString::fromUtf8(floatNumber);
        ui->lcdNumber_2->display(numberStr);
        return;
    }
    else if(flag_LCRD == FLAG_C)
    {
        switch (secondLastChar) {
        case 'u':
            ui->unitLabel->setText("uF");

            break;
        case 'n':
            ui->unitLabel->setText("nF");

            break;
        case 'p':
            ui->unitLabel->setText("pF");
            break;
        default:
            break;
        }
    }
    else if(flag_LCRD == FLAG_R)
    {
        switch (fourthLastChar) {
        case 'k':
            ui->unitLabel->setText("kΩ");
            break;
        case 'M':
            ui->unitLabel->setText("MΩ");
            break;
        default:
            ui->unitLabel->setText("Ω");
            break;
        }
    }
    else if(flag_LCRD == FLAG_L)
    {
        switch (secondLastChar) {
        case 'u':
            ui->unitLabel->setText("uH");

            break;
        case 'm':
            ui->unitLabel->setText("mH");
            break;
        default:
            break;
        }
    }
    else if(flag_LCRD == FLAG_DF || flag_LCRD == FLAG_DB)
    {
        ui->unitLabel->setText("V");
    }
    QString numberStr = QString::fromUtf8(floatNumber);
    ui->lcdNumber->display(numberStr);
}

/**
 * 手动处理串口接收数据的函数
 * 无参数
 * 无返回值
 */
void MainWindow::manual_serialPortReadyRead()
{
    DelayMSec(10);
    // 从串口读取所有可用数据
    QByteArray recBuf = serialPort->readAll();
    ;
    QString str_rev;

    if (recBuf.isEmpty()) {

        return;
    }
    // 更新接收字节的总数
    recvNum += recBuf.size();
    // 在状态栏更新接收字节的显示
    setNumOnLabel(lblRecvNum, "R: ", recvNum);

    receive_process(recBuf);

    updateLCD(recBuf);

    if(flag_LCRD == FLAG_OPEN)
    {
        ui->label_4->setText("断路");
        ui->lcdNumber->display("");
        ui->unitLabel->setText("");
    }
    else if(flag_LCRD == FLAG_ESR)
        recBuf.remove(0,4);
    else
        recBuf.remove(0,2);

    if(flag_LCRD == FLAG_DF)
    {
        recBuf = "二极管上正下负：" + recBuf;
        ui->label_4->setText("元件类型：二极管 上正下负");

    }
    else if(flag_LCRD == FLAG_DB)
    {
        recBuf = "二极管上负下正：" + recBuf;
        ui->label_4->setText("元件类型：二极管 上负下正");
    }
    else if(flag_LCRD == FLAG_C)
    {
        recBuf = "电容：" + recBuf;
        ui->label_4->setText("元件类型：电容");
    }
    else if(flag_LCRD == FLAG_L)
    {
        recBuf = "电感：" + recBuf;
        ui->label_4->setText("元件类型：电感");
    }
    else if(flag_LCRD == FLAG_R)
    {
        recBuf = "电阻：" + recBuf;
        ui->label_4->setText("元件类型：电阻");
    }
    else if(flag_LCRD == FLAG_ESR)
    {
        recBuf = "ESR：" + recBuf;
    }

    if(flag_LCRD != FLAG_ESR)
    {
        ui->lcdNumber_2->display("");
    }


    // 检查是否以十六进制显示数据
    if (ui->chk_rev_hex->checkState() == false)
    {
        // 检查是否显示接收时间
        if (ui->chk_rev_time->checkState() == Qt::Checked)
        {
            // 当选择了显示时间时，添加当前时间到接收数据前
            QDateTime nowtime = QDateTime::currentDateTime();
            str_rev = "[" + nowtime.toString("yyyy-MM-dd hh:mm:ss") + "] ";
            str_rev += QString(recBuf).append("\r\n");
        }
        else
        {
            // 直接显示接收到的文本，根据需要添加换行
            if (ui->chk_rev_line->checkState() == Qt::Checked)
            {
                str_rev = QString(recBuf).append("\r\n");
            }
            else
            {
                str_rev = QString(recBuf);
            }
        }
    }
    else
    {
        // 以十六进制显示数据，并转换为大写
        QString str1 = recBuf.toHex().toUpper();
        // 为十六进制数据每两个字符添加一个空格，提高可读性
        QString str2;
        for (int i = 0; i < str1.length(); i += 2)
        {
            str2 += str1.mid(i, 2);
            str2 += " ";
        }
        // 检查是否显示接收时间
        if (ui->chk_rev_time->checkState() == Qt::Checked)
        {
            // 当选择了显示时间时，添加当前时间到十六进制数据前
            QDateTime nowtime = QDateTime::currentDateTime();
            str_rev = "[" + nowtime.toString("yyyy-MM-dd hh:mm:ss") + "] ";
            str_rev += str2.append("\r\n");
        }
        else
        {
            // 根据是否选择换行，添加换行符
            if (ui->chk_rev_line->checkState() == Qt::Checked)
                str_rev += str2.append("\n");
            else
                str_rev = str2;
        }
    }
    // 先清屏
    //    ui->recvEdit->clear();
    DelayMSec(100);
    // 将处理后的数据插入到文本编辑器中
    ui->recvEdit->insertPlainText(str_rev);
    DelayMSec(100);
    // 将光标移动到文本末尾，确保新数据能被看到
    ui->recvEdit->moveCursor(QTextCursor::End);


}

/*打开串口*/
void MainWindow::on_openBt_clicked()
{
    /*串口初始化*/
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;

    // 获取串口波特率
    // baudRate = ui->baundrateCb->currentText().toInt();直接字符串转换为 int 的方法

    if (ui->baundrateCb->currentText() == "1200")
        baudRate = QSerialPort::Baud1200;
    else if (ui->baundrateCb->currentText() == "2400")
        baudRate = QSerialPort::Baud2400;
    else if (ui->baundrateCb->currentText() == "4800")
        baudRate = QSerialPort::Baud4800;
    else if (ui->baundrateCb->currentText() == "9600")
        baudRate = QSerialPort::Baud9600;
    else if (ui->baundrateCb->currentText() == "19200")
        baudRate = QSerialPort::Baud19200;
    else if (ui->baundrateCb->currentText() == "38400")
        baudRate = QSerialPort::Baud38400;
    else if (ui->baundrateCb->currentText() == "57600")
        baudRate = QSerialPort::Baud57600;
    else if (ui->baundrateCb->currentText() == "115200")
        baudRate = QSerialPort::Baud115200;

    // 获取串口数据位
    if (ui->databitCb->currentText() == "5")
        dataBits = QSerialPort::Data5;
    else if (ui->databitCb->currentText() == "6")
        dataBits = QSerialPort::Data6;
    else if (ui->databitCb->currentText() == "7")
        dataBits = QSerialPort::Data7;
    else if (ui->databitCb->currentText() == "8")
        dataBits = QSerialPort::Data8;

    // 获取串口停止位
    if (ui->stopbitCb->currentText() == "1")
        stopBits = QSerialPort::OneStop;
    else if (ui->stopbitCb->currentText() == "1.5")
        stopBits = QSerialPort::OneAndHalfStop;
    else if (ui->stopbitCb->currentText() == "2")
        stopBits = QSerialPort::TwoStop;

    // 获取串口奇偶校验位
    if (ui->checkbitCb->currentText() == "none")
    {
        checkBits = QSerialPort::NoParity;
    }
    else if (ui->checkbitCb->currentText() == "奇校验")
    {
        checkBits = QSerialPort::OddParity;
    }
    else if (ui->checkbitCb->currentText() == "偶校验")
    {
        checkBits = QSerialPort::EvenParity;
    }
    else
    {
    }

    // 初始化串口属性，设置 端口号、波特率、数据位、停止位、奇偶校验位数
    serialPort->setPortName(ui->serialCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);

    // 根据初始化好的串口属性，打开串口
    // 如果打开成功，反转打开按钮显示和功能。打开失败，无变化，并且弹出错误对话框。
    if (ui->openBt->text() == "打开串口")
    {
        if (serialPort->open(QIODevice::ReadWrite) == true)
        {
            // QMessageBox::
            ui->openBt->setText("关闭串口");
            // 让端口号下拉框不可选，避免误操作（选择功能不可用，控件背景为灰色）
            ui->serialCb->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(this, "错误提示", "串口打开失败！！！\r\n该串口可能被占用\r\n请选择正确的串口");
        }
        // statusBar 状态栏显示端口状态
        QString sm = "%1 OPENED, %2, 8, NONE, 1";
        QString status = sm.arg(serialPort->portName()).arg(serialPort->baudRate());
        lblPortState->setText(status);
        lblPortState->setStyleSheet("color:green");
    }
    else
    {
        serialPort->close();
        ui->openBt->setText("打开串口");
        // 端口号下拉框恢复可选，避免误操作
        ui->serialCb->setEnabled(true);
        // statusBar 状态栏显示端口状态
        QString sm = "%1 CLOSED";
        QString status = sm.arg(serialPort->portName());
        lblPortState->setText(status);
        lblPortState->setStyleSheet("color:red");
    }
}

/*发送数据*/
void MainWindow::on_sendBt_clicked()
{
    QByteArray send_data = "START";

    int b = 0;

    int a = serialPort->write(send_data);
    b = b + a;


    if (ui->chk_send_line->checkState() == Qt::Checked)
    {
        send_data.append("\r\n");
    }
    // 如发送成功，会返回发送的字节长度。失败，返回-1。
    //        int a = serialPort->write(array);
    // 发送字节计数并显示
    if (b > 0)
    {
        // 发送字节计数
        sendNum += b;
        // 状态栏显示计数值
        setNumOnLabel(lblSendNum, "S: ", sendNum);
    }
    // 相邻指令发送间隔500ms
    DelayMSec(500);



}
// 状态栏标签显示计数值
void MainWindow::setNumOnLabel(QLabel *lbl, QString strS, long num)
{
    // 标签显示
    QString strN;
    strN.sprintf("%ld", num);
    QString str = strS + strN;
    lbl->setText(str);
}

void MainWindow::DelayMSec(unsigned int msec)
{

    QEventLoop loop;                               // 定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit())); // 创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();                                   // 事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

/*清空*/
void MainWindow::on_clearBt_clicked()
{
    ui->recvEdit->clear();
    // 清除发送、接收字节计数
    sendNum = 0;
    recvNum = 0;
    // 状态栏显示计数值
    setNumOnLabel(lblSendNum, "S: ", sendNum);
    setNumOnLabel(lblRecvNum, "R: ", recvNum);
}

void MainWindow::on_btnClearSend_clicked()
{
    // 清除发送字节计数
    sendNum = 0;
    // 状态栏显示计数值
    setNumOnLabel(lblSendNum, "S: ", sendNum);
}
// 定时发送开关 选择复选框
void MainWindow::on_chkTimSend_stateChanged(int arg1)
{
    // 获取复选框状态，未选为0，选中为2
    if (arg1 == 0)
    {
        timSend->stop();
        // 时间输入框恢复可选
        ui->txtSendMs->setEnabled(true);
    }
    else
    {
        // 对输入的值做限幅，小于10ms会弹出对话框提示
        if (ui->txtSendMs->text().toInt() >= 10)
        {
            timSend->start(ui->txtSendMs->text().toInt()); // 设置定时时长，重新计数
            // 让时间输入框不可选，避免误操作（输入功能不可用，控件背景为灰色）
            ui->txtSendMs->setEnabled(false);
        }
        else
        {
            ui->chkTimSend->setCheckState(Qt::Unchecked);
            QMessageBox::critical(this, "错误提示", "定时发送的最小间隔为 10ms\r\n请确保输入的值 >=10");
        }
    }
}

