#include "./header/controlsetting.h"
#include "ui_controlsetting.h"

ControlSetting::ControlSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    send_control_setting();
}

//
ControlSetting::~ControlSetting()
{
    delete ui;
}

void ControlSetting::send_control_setting(){

    connect(ui->OK_btn,SIGNAL(clicked()),this,SLOT(setvalid()));
    connect(ui->Cancel_btn,SIGNAL(clicked()),this,SLOT(setinvalid()));
}

void ControlSetting::setvalid(){
    QByteArray arrctl5030;
    QByteArray arrctl5330;

    /*
     * added on 2019.4.3
    */
    QByteArray arrctl5130;
    QByteArray arrctl5230;
    QByteArray arrctl5430;
    QByteArray arrctl5530;


    arrctl5030.resize(8);
    arrctl5130.resize(8);
    arrctl5230.resize(8);
    arrctl5330.resize(8);
    arrctl5430.resize(8);
    arrctl5530.resize(8);

    for(int i = 0 ; i < 8 ; i++){
        arrctl5030[i] = 0;
        arrctl5130[i] = 0;
        arrctl5230[i] = 0;
        arrctl5330[i] = 0;
        arrctl5430[i] = 0;
        arrctl5530[i] = 0;
    }

    int clear_meter ;
    int reset_cutheight;
    int control_unloader;
    int auto_loader;
    int auto_cutheight;
    int reset_weight;

    if(ui->cbx_4->currentIndex() == 1)          clear_meter = 1;            //清零
    else                                        clear_meter = 0;            //无动作

    if(ui->cbx_3->currentIndex() == 0)          reset_cutheight = 0;        //脱开
    else if (ui->cbx_3->currentIndex() == 1)    reset_cutheight = 1;        //割台提升
    else if (ui->cbx_3->currentIndex() == 2)    reset_cutheight = 2;        //割台下降
    else                                        reset_cutheight = 3;        //未定义

    if(ui->cbx_1->currentIndex() == 0)          control_unloader = 4;       //停止
    else if (ui->cbx_1->currentIndex() == 1)    control_unloader = 5;       //动态展开
    else if (ui->cbx_1->currentIndex() == 2)    control_unloader = 6;       //静态展开
    else if (ui->cbx_1->currentIndex() == 3)    control_unloader = 7;       //收回
    else                                        control_unloader = 0;       //无动作

    if(ui->cbx_5->currentIndex() == 0)          auto_loader = 0;            //关闭负荷自动调节
    else if (ui->cbx_5->currentIndex() == 1)    auto_loader = 1;            //开启负荷自动调节
    else                                        auto_loader = 2;            //无动作

    if(ui->cbx_6->currentIndex() == 0)          auto_cutheight = 0;         //关闭割台高度自动调节
    else if (ui->cbx_6->currentIndex() == 1)    auto_cutheight = 1;         //开启割台高度自动调节
    else                                        auto_cutheight = 2;         //无动作

    if(ui->cbx_7->currentIndex() == 0)          reset_weight = 0;           //无动作
    else if (ui->cbx_7->currentIndex() == 1)    reset_weight = 1;           //清零
    else                                        reset_weight = 2;           //未定义

    if(ui->cbx_9->currentIndex() == 1)          arrctl5330[0] = 1;          //静音
    else if (ui->cbx_9->currentIndex() == 2)    arrctl5330[0] = 2;          //播放
    else                                        arrctl5330[0] = 0;          //无动作


    /*
     * added on 2019.4.5
    */
    int StalkMoisture = (int)(ui->l_StalkMoisture->text().toFloat()*100);   //茎干含水率设置值
    arrctl5330[1] = StalkMoisture%256;
    arrctl5330[2] = StalkMoisture/256;

    int CroppingIntensity = (int)(ui->l_CroppingIntensity->text().toFloat()*100);
    arrctl5330[3] = CroppingIntensity%256;                                  //作物密度设置值
    arrctl5330[4] = CroppingIntensity/256;

    int GrassGrainRatio = ui->l_GrassrGrainratio->text().toFloat();       //草谷比设置值
    arrctl5330[5] = GrassGrainRatio;

    int PlantHeight = ui->l_PlantHeight->text().toFloat();                //农作物高度设置值
    arrctl5330[6] = PlantHeight%256;
    arrctl5330[7] = PlantHeight/256;


    int AxialRollerSpeedMin = ui->l_AxialRollerSpeedMin->text().toFloat(); //轴流滚筒转速下限设置值
    arrctl5430[0] = AxialRollerSpeedMin%256;
    arrctl5430[1] = AxialRollerSpeedMin/256;

    int AxialRollerSpeedMax = ui->l_AxialRollerSpeedMax->text().toFloat(); //轴流滚筒转速上限设置值
    arrctl5430[2] = AxialRollerSpeedMax%256;
    arrctl5430[3] = AxialRollerSpeedMax/256;


    if(ui->cbx_heartbeat->currentIndex() == 0) arrctl5530[0] = 1;           //01:仪表在线
    else arrctl5530[0] = 0;                                                 //00:仪表掉线


    /*
     * added on 2019.4.3 18FF5130
    */
    float le = ui->l_weight1000->text().toFloat();  //千粒重设置值
    int pfh = ui->l_pfh->text().toFloat();          //仿形高度设置值
    int cutx = ui->l_cuth_max->text().toFloat();    //割台返回高度上限
    int cutn = ui->l_cuth_min->text().toFloat();    //割台返回高度下限

    /*
     * added on 2019.4.3
    */
    arrctl5130[0] = ((int)(le*100))%256;    //千粒重设置值
    arrctl5130[1] = (int(le*100))/256;
    arrctl5130[2] = pfh%256;                //仿形高度设置值
    arrctl5130[3] = pfh/256;
    arrctl5130[4] = cutx%256;               //割台返回高度上限
    arrctl5130[5] = cutx/256;
    arrctl5130[6] = cutn%256;               //割台返回高度下限
    arrctl5130[7] = cutn/256;

    /*
     * added on 2019.4.3 18FF5230
    */
    int rsls = ui->l_rsls->text().toFloat();                    //轴流滚筒堵塞转速低报警设置值
    int concaveclset = ui->l_concaveclset->text().toFloat();    //凹版间隙调节值
    int cuttermode = 0;                                         //切碎器工作模式
    int grainkd = 0;                                            //当前作物种类

    if(ui->cutterMode->currentIndex() == 1)   cuttermode = 1;          //排草
    else cuttermode = 0;                                               //切碎

    if(ui->grain_kind->currentIndex() == 0)          grainkd = 0;       //"水稻"
    else if (ui->grain_kind->currentIndex() == 1)    grainkd = 1;       //"小麦"
    else if (ui->grain_kind->currentIndex() == 2)    grainkd = 2;       //"玉米"
    else if (ui->grain_kind->currentIndex() == 3)    grainkd = 3;       //"大豆"L
    else if (ui->grain_kind->currentIndex() == 4)    grainkd = 15;      //"其他"

    int ConcaveclClearanceSetMax = ui->l_ConcaveclSetMax->text().toFloat(); //凹板间隙上限
    int ConcaveclClearanceSetMin = ui->l_ConcaveclSetMin->text().toFloat(); //凹板间隙下限

    arrctl5230[1] = rsls % 256;
    arrctl5230[2] = rsls / 256;
    arrctl5230[3] = concaveclset;
    arrctl5230[6] = (cuttermode << 4) | grainkd;
    arrctl5230[4] = ConcaveclClearanceSetMax;
    arrctl5230[5] = ConcaveclClearanceSetMin;
    arrctl5230[0]  = arrctl5230[7] = 0;

    arrctl5030[0] = (clear_meter << 2) | (reset_cutheight << 4);
    arrctl5030[1] = control_unloader | (auto_loader << 3) | (auto_cutheight << 5);
    arrctl5030[2] = reset_weight;

    for(int i = 0 ; i < 8 ; i++){
        qDebug() << "arrctl5030(" << i << "):" << (int)arrctl5030.at(i);
        qDebug() << "arrctl5330(" << i << "):" << (int)arrctl5330.at(i);
    }
    can0_send(arrctl5030,0x18FF5030);
    can0_send(arrctl5330,0x18FF5330);

    /*
     * added on 2019.4.3
    */
    can0_send(arrctl5130,0x18FF5130);
    can0_send(arrctl5230,0x18FF5230);

    this->hide();
}

void ControlSetting::setinvalid()
{
    this->hide();
}
