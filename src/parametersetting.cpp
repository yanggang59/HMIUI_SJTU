#include "./header/parametersetting.h"
#include "ui_parametersetting.h"
#include <QDebug>

char dataArray130[8];
char dataArray230[8];
char dataArray330[8];
char dataArray430[8];

int flag_send_setting;

QByteArray arr12;
QByteArray arr23;
QByteArray arr33;
QByteArray arr43;

/*
added on 2019.2.27
*/
QByteArray arr1405;  //0-1字节为清选风机设置值的低高值
QByteArray arr2817;  //0-1字节为滚筒转速设置值的低高值
QByteArray arr2615;  //后4个字节为割刀转速设置值/搅龙转速设置值

QByteArray arr2715;
QByteArray arr2815;



ParameterSetting::ParameterSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    flag_send_setting = 0 ;

    send_para_setting();                                    //按钮点击事件

    updateParaTimer = new QTimer(this);

    QObject::connect(updateParaTimer,SIGNAL(timeout()),this,SLOT(show_parameter()));

    updateParaTimer->start(500);

}

ParameterSetting::~ParameterSetting()
{
    delete ui;
}

void ParameterSetting::send_para_setting(){

    connect(ui->OK_btn,SIGNAL(clicked()),this,SLOT(setvalid()));
    connect(ui->Cancel_btn,SIGNAL(clicked()),this,SLOT(setinvalid()));
}

void ParameterSetting::setvalid(){

    this->hide();
}

void ParameterSetting::setinvalid(){
    flag_send_setting = 0;
    this->hide();
}

void ParameterSetting::show_parameter()
{
        //18FF1405 ---8
        showSetSpeed1405(num_mon[8]);

        //18FF2615 ---21
        showSetSpeed2615(num_mon[21]);

        //18FF2817 ---23
        showSetSpeed2817(num_mon[23]);

        //18FF2715 ---32
        showFlow2715(num_mon[32]);

        //18FF2815 ---33
        showPressure2815(num_mon[33]);

}

void ParameterSetting::showSetSpeed1405(int arr[8])
{
    cleanFanSpeedSetValue = arr[0] + arr[1]*256;

    ui->l_cleanFanSpeedTarget->setText(tr("%1").arg(cleanFanSpeedSetValue));
}

void ParameterSetting::showSetSpeed2615(int arr[8])
{
    cutterFrequencySetValue = arr[4] + arr[5]*256;

    augerSpeedSetValue = arr[6] + arr[7]*256;

    ui->l_cutterFrequencyTarget->setText(tr("%1").arg(cutterFrequencySetValue));

    ui->l_augerSpeedTarget->setText(tr("%1").arg(augerSpeedSetValue));

}

void ParameterSetting::showSetSpeed2817(int arr[8])
{
    rollerSpeedSetValue = arr[0] + arr[1]*256;

    ui->l_rollerSpeedTarget->setText(tr("%1").arg(rollerSpeedSetValue));
}

void ParameterSetting::showFlow2715(int arr[8])
{
    winderHydraulicMotorFlow = arr[0] + arr[1]*256;
    feedingAugerHydraulicMotorFlow = arr[2] + arr[3]*256;
    cutterHydraulicMotorFlow = arr[4] + arr[5]*256;

    winderSpeedSetValue = arr[6];

    ui->l_winderHydraulicMotorFlow->setText(tr("%1").arg(winderHydraulicMotorFlow/10));
    ui->l_feedingAugerHydraulicMotorFlow->setText(tr("%1").arg(feedingAugerHydraulicMotorFlow/10));
    ui->l_cutterHydraulicMotorFlow->setText(tr("%1").arg(cutterHydraulicMotorFlow/10));

    ui->l_winderSpeedTarget->setText(tr("%1").arg(winderSpeedSetValue));
}

void ParameterSetting::showPressure2815(int arr[8])
{
    winderHydraulicMotorTakeOilPressure = arr[0];
    winderHydraulicMotorReturnOilPressure = arr[1];

    feedingAugerHydraulicMotorTakeOilPressure = arr[2];
    feedingAugerHydraulicMotorReturnOilPressure = arr[3];

    cutterHydraulicMotorTakeOilPressure = arr[4];
    cutterHydraulicMotorReturnOilPressure = arr[5];

    ui->l_winderHydraulicMotorTakeOilPressure->setText(tr("%1").arg(winderHydraulicMotorTakeOilPressure));
    ui->l_winderHydraulicMotorReturnOilPressure->setText(tr("%1").arg(winderHydraulicMotorReturnOilPressure));

    ui->l_feedingAugerHydraulicMotorTakeOilPressure->setText(tr("%1").arg(feedingAugerHydraulicMotorTakeOilPressure));
    ui->l_feedingAugerHydraulicMotorReturnOilPressure->setText(tr("%1").arg(feedingAugerHydraulicMotorReturnOilPressure));

    ui->l_cutterHydraulicMotorTakeOilPressure->setText(tr("%1").arg(cutterHydraulicMotorTakeOilPressure));
    ui->l_cutterHydraulicMotorReturnOilPressure->setText(tr("%1").arg(cutterHydraulicMotorReturnOilPressure));
}


