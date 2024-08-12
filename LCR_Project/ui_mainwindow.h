/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnClearSend;
    QCheckBox *chk_send_line;
    QPushButton *sendBt;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *clearBt;
    QCheckBox *chk_rev_time;
    QCheckBox *chk_rev_line;
    QCheckBox *chk_rev_hex;
    QGroupBox *groupBox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QVBoxLayout *verticalLayout_7;
    QComboBox *serialCb;
    QComboBox *baundrateCb;
    QComboBox *databitCb;
    QComboBox *stopbitCb;
    QComboBox *checkbitCb;
    QVBoxLayout *verticalLayout_8;
    QPushButton *bitSerialCheck;
    QPushButton *openBt;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_2;
    QSpinBox *txtSendMs;
    QCheckBox *chkTimSend;
    QCheckBox *chk_send_hex;
    QWidget *layoutWidget3;
    QFormLayout *formLayout;
    QPlainTextEdit *recvEdit;
    QLabel *label_4;
    QLCDNumber *lcdNumber;
    QLabel *unitLabel;
    QLCDNumber *lcdNumber_2;
    QLabel *label_5;
    QLabel *unitLabel_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1213, 949);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btnClearSend = new QPushButton(centralwidget);
        btnClearSend->setObjectName(QString::fromUtf8("btnClearSend"));
        btnClearSend->setEnabled(false);
        btnClearSend->setGeometry(QRect(700, 810, 111, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        btnClearSend->setFont(font1);
        chk_send_line = new QCheckBox(centralwidget);
        chk_send_line->setObjectName(QString::fromUtf8("chk_send_line"));
        chk_send_line->setGeometry(QRect(990, 520, 121, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        chk_send_line->setFont(font2);
        sendBt = new QPushButton(centralwidget);
        sendBt->setObjectName(QString::fromUtf8("sendBt"));
        sendBt->setGeometry(QRect(940, 560, 221, 91));
        QFont font3;
        font3.setPointSize(24);
        sendBt->setFont(font3);
        sendBt->setContextMenuPolicy(Qt::NoContextMenu);
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(260, 498, 291, 181));
        groupBox_2->setFont(font1);
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 30, 251, 141));
        layoutWidget->setFont(font1);
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        clearBt = new QPushButton(layoutWidget);
        clearBt->setObjectName(QString::fromUtf8("clearBt"));
        clearBt->setFont(font1);

        gridLayout->addWidget(clearBt, 0, 0, 1, 1);

        chk_rev_time = new QCheckBox(layoutWidget);
        chk_rev_time->setObjectName(QString::fromUtf8("chk_rev_time"));

        gridLayout->addWidget(chk_rev_time, 1, 0, 1, 1);

        chk_rev_line = new QCheckBox(layoutWidget);
        chk_rev_line->setObjectName(QString::fromUtf8("chk_rev_line"));

        gridLayout->addWidget(chk_rev_line, 1, 1, 1, 1);

        chk_rev_hex = new QCheckBox(layoutWidget);
        chk_rev_hex->setObjectName(QString::fromUtf8("chk_rev_hex"));

        gridLayout->addWidget(chk_rev_hex, 0, 1, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(270, 88, 261, 381));
        groupBox->setFont(font2);
        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 30, 221, 341));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_6->addWidget(label_6);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_6->addWidget(label_7);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout_6->addWidget(label_8);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_6->addWidget(label_9);

        label_10 = new QLabel(layoutWidget1);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_6->addWidget(label_10);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        serialCb = new QComboBox(layoutWidget1);
        serialCb->setObjectName(QString::fromUtf8("serialCb"));

        verticalLayout_7->addWidget(serialCb);

        baundrateCb = new QComboBox(layoutWidget1);
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->addItem(QString());
        baundrateCb->setObjectName(QString::fromUtf8("baundrateCb"));

        verticalLayout_7->addWidget(baundrateCb);

        databitCb = new QComboBox(layoutWidget1);
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->addItem(QString());
        databitCb->setObjectName(QString::fromUtf8("databitCb"));

        verticalLayout_7->addWidget(databitCb);

        stopbitCb = new QComboBox(layoutWidget1);
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->addItem(QString());
        stopbitCb->setObjectName(QString::fromUtf8("stopbitCb"));

        verticalLayout_7->addWidget(stopbitCb);

        checkbitCb = new QComboBox(layoutWidget1);
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->addItem(QString());
        checkbitCb->setObjectName(QString::fromUtf8("checkbitCb"));

        verticalLayout_7->addWidget(checkbitCb);


        horizontalLayout_2->addLayout(verticalLayout_7);


        verticalLayout_5->addLayout(horizontalLayout_2);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        bitSerialCheck = new QPushButton(layoutWidget1);
        bitSerialCheck->setObjectName(QString::fromUtf8("bitSerialCheck"));

        verticalLayout_8->addWidget(bitSerialCheck);

        openBt = new QPushButton(layoutWidget1);
        openBt->setObjectName(QString::fromUtf8("openBt"));

        verticalLayout_8->addWidget(openBt);


        verticalLayout_5->addLayout(verticalLayout_8);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(260, 688, 291, 181));
        groupBox_3->setFont(font2);
        layoutWidget2 = new QWidget(groupBox_3);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(20, 30, 251, 131));
        gridLayout_2 = new QGridLayout(layoutWidget2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        txtSendMs = new QSpinBox(layoutWidget2);
        txtSendMs->setObjectName(QString::fromUtf8("txtSendMs"));

        gridLayout_2->addWidget(txtSendMs, 1, 1, 1, 1);

        chkTimSend = new QCheckBox(layoutWidget2);
        chkTimSend->setObjectName(QString::fromUtf8("chkTimSend"));

        gridLayout_2->addWidget(chkTimSend, 1, 0, 1, 1);

        chk_send_hex = new QCheckBox(layoutWidget2);
        chk_send_hex->setObjectName(QString::fromUtf8("chk_send_hex"));

        gridLayout_2->addWidget(chk_send_hex, 0, 0, 1, 1);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(250, 38, 2, 2));
        formLayout = new QFormLayout(layoutWidget3);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        recvEdit = new QPlainTextEdit(centralwidget);
        recvEdit->setObjectName(QString::fromUtf8("recvEdit"));
        recvEdit->setEnabled(true);
        recvEdit->setGeometry(QRect(570, 550, 351, 211));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(recvEdit->sizePolicy().hasHeightForWidth());
        recvEdit->setSizePolicy(sizePolicy1);
        recvEdit->setReadOnly(true);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(570, 90, 551, 61));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(20);
        label_4->setFont(font4);
        lcdNumber = new QLCDNumber(centralwidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(570, 160, 421, 181));
        lcdNumber->setAutoFillBackground(false);
        unitLabel = new QLabel(centralwidget);
        unitLabel->setObjectName(QString::fromUtf8("unitLabel"));
        unitLabel->setGeometry(QRect(1000, 250, 181, 91));
        QFont font5;
        font5.setPointSize(36);
        unitLabel->setFont(font5);
        lcdNumber_2 = new QLCDNumber(centralwidget);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(570, 400, 341, 121));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(570, 340, 551, 61));
        QFont font6;
        font6.setPointSize(20);
        label_5->setFont(font6);
        unitLabel_2 = new QLabel(centralwidget);
        unitLabel_2->setObjectName(QString::fromUtf8("unitLabel_2"));
        unitLabel_2->setGeometry(QRect(920, 460, 121, 61));
        unitLabel_2->setFont(font5);
        MainWindow->setCentralWidget(centralwidget);
        recvEdit->raise();
        label_4->raise();
        btnClearSend->raise();
        sendBt->raise();
        groupBox_2->raise();
        groupBox->raise();
        groupBox_3->raise();
        layoutWidget->raise();
        chk_send_line->raise();
        lcdNumber->raise();
        unitLabel->raise();
        lcdNumber_2->raise();
        label_5->raise();
        unitLabel_2->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1213, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        baundrateCb->setCurrentIndex(7);
        databitCb->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\344\270\212\344\275\215\346\234\272", nullptr));
        MainWindow->setStyleSheet(QString());
        btnClearSend->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        chk_send_line->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\226\260\350\241\214", nullptr));
        sendBt->setText(QCoreApplication::translate("MainWindow", "\346\265\213\351\207\217", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\350\256\276\347\275\256", nullptr));
        clearBt->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", nullptr));
        chk_rev_time->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\210\263", nullptr));
        chk_rev_line->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\346\215\242\350\241\214", nullptr));
        chk_rev_hex->setText(QCoreApplication::translate("MainWindow", "Hex\346\216\245\346\224\266", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        baundrateCb->setItemText(0, QCoreApplication::translate("MainWindow", "1200", nullptr));
        baundrateCb->setItemText(1, QCoreApplication::translate("MainWindow", "2400", nullptr));
        baundrateCb->setItemText(2, QCoreApplication::translate("MainWindow", "4800", nullptr));
        baundrateCb->setItemText(3, QCoreApplication::translate("MainWindow", "9600", nullptr));
        baundrateCb->setItemText(4, QCoreApplication::translate("MainWindow", "19200", nullptr));
        baundrateCb->setItemText(5, QCoreApplication::translate("MainWindow", "38400", nullptr));
        baundrateCb->setItemText(6, QCoreApplication::translate("MainWindow", "57600", nullptr));
        baundrateCb->setItemText(7, QCoreApplication::translate("MainWindow", "115200", nullptr));

        databitCb->setItemText(0, QCoreApplication::translate("MainWindow", "5", nullptr));
        databitCb->setItemText(1, QCoreApplication::translate("MainWindow", "6", nullptr));
        databitCb->setItemText(2, QCoreApplication::translate("MainWindow", "7", nullptr));
        databitCb->setItemText(3, QCoreApplication::translate("MainWindow", "8", nullptr));

        stopbitCb->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        stopbitCb->setItemText(1, QCoreApplication::translate("MainWindow", "1.5", nullptr));
        stopbitCb->setItemText(2, QCoreApplication::translate("MainWindow", "2", nullptr));

        checkbitCb->setItemText(0, QCoreApplication::translate("MainWindow", "none", nullptr));
        checkbitCb->setItemText(1, QCoreApplication::translate("MainWindow", "\345\245\207\346\240\241\351\252\214", nullptr));
        checkbitCb->setItemText(2, QCoreApplication::translate("MainWindow", "\345\201\266\346\240\241\351\252\214", nullptr));

        bitSerialCheck->setText(QCoreApplication::translate("MainWindow", "\346\243\200\346\265\213\344\270\262\345\217\243", nullptr));
        openBt->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\350\256\276\347\275\256", nullptr));
        chkTimSend->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\345\217\221\351\200\201", nullptr));
        chk_send_hex->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\201", nullptr));
        recvEdit->setPlainText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\205\203\344\273\266\347\261\273\345\236\213\357\274\232", nullptr));
        unitLabel->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "\347\255\211\346\225\210\344\270\262\350\201\224\347\224\265\351\230\273", nullptr));
        unitLabel_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
