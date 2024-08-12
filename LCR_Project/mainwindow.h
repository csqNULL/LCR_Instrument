#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QString>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSerialPort *serialPort;//定义串口指针
    uint8_t flag_channel_coding = 0;    // 是否进行信道编码
    uint8_t flag_input_pattern = 1;     // 编码输入形式，默认十进制
    uint8_t flag_output_pattern = 0;    // 编码输出形式，默认二进制
    uint8_t flag_LCRD;                  // 元件种类

    int end_flag = 0xff;
    int led_index = 0x70;
private slots:

    /*手动连接槽函数*/
    void manual_serialPortReadyRead();

    /*以下为mainwindow.ui文件中点击“转到槽”自动生成的函数*/
    void on_openBt_clicked();

    void on_sendBt_clicked();

    void on_clearBt_clicked();

    void on_btnClearSend_clicked();

    void on_chkTimSend_stateChanged(int arg1);

    void on_btnSerialCheck_clicked();


private:
    Ui::MainWindow *ui;

    // 发送、接收字节计数
    long sendNum, recvNum;
    QLabel *lblSendNum;
    QLabel *lblRecvNum;
    QLabel *lblPortState;
    void setNumOnLabel(QLabel *lbl, QString strS, long num);

    int JudCodingOverflow();
    bool JudCodingBeyond();
    void CalculateCapacity();
    void EditTableSize();
    void EditTableData();
    void DelayMSec(unsigned int msec);
    void receive_process(QByteArray array);

    void updateLCD(QByteArray array);

    QByteArray filterAndConvertToFloat(QByteArray* byteArray);

    // 定时发送-定时器
    QTimer *timSend;
    QTimer *timInput;
    QTimer *timSendCommand;
    //QTimer *timCheckPort;
};
#endif // MAINWINDOW_H

