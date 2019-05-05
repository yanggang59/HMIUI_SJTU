#ifndef PARAMETERSETTING_H
#define PARAMETERSETTING_H

#include <QDialog>
#include "thread.h"
#include "can_recv_send.h"

extern char dataArray130[8];
extern char dataArray230[8];
extern char dataArray330[8];
extern char dataArray430[8];

extern int flag_send_setting;

extern QByteArray arr12;
extern QByteArray arr33;
extern QByteArray arr43;


namespace Ui {
class ParameterSetting;
}

class ParameterSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ParameterSetting(QWidget *parent = 0);
    ~ParameterSetting();

    //18FF1405
    void showSetSpeed1405(int arr[8]);

    //18FF2615
    void showSetSpeed2615(int arr[8]);

    //18FF2817
    void showSetSpeed2817(int arr[8]);

    //18FF2715
    void showFlow2715(int arr[8]);

    //18FF2815
    void showPressure2815(int arr[8]);


private:
    Ui::ParameterSetting *ui;
    void send_para_setting();

    QTimer *updateParaTimer;   // to update the data

    //18FF1405
    qreal cleanFanSpeedSetValue;                          //清选风机设置值

    //18FF2615
    qreal cutterFrequencySetValue;                        //割刀频率设置值
    qreal augerSpeedSetValue;                             //搅龙转速设置值

    //18FF2817
    qreal rollerSpeedSetValue;                            //滚筒转速设置值

    //18FF2715
    qreal winderHydraulicMotorFlow;                       //拨禾轮液压马达流量
    qreal feedingAugerHydraulicMotorFlow;                 //喂入绞龙液压马达流量
    qreal cutterHydraulicMotorFlow;                       //割刀液压马达流量

    qreal winderSpeedSetValue;                            //拨禾轮转速设置值

    //18FF2815
    qreal winderHydraulicMotorTakeOilPressure;           //拨禾轮液压马达进油压力
    qreal winderHydraulicMotorReturnOilPressure;         //拨禾轮液压马达进油压力
    qreal feedingAugerHydraulicMotorTakeOilPressure;     //喂入绞龙液压马达进油压力
    qreal feedingAugerHydraulicMotorReturnOilPressure;   //喂入绞龙液压马达进油压力
    qreal cutterHydraulicMotorTakeOilPressure;           //割刀液压马达进油压力
    qreal cutterHydraulicMotorReturnOilPressure;         //割刀液压马达回油压力


private slots:
    void setvalid();
    void setinvalid();
    void show_parameter();

};

#endif // PARAMETERSETTING_H
