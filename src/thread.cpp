#include "thread.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "can_recv_send.h"		// CAN 数据的头文件

#define SLEEP msleep(0)

/**********************************************************************
 * Name: num_mon[34][8]
 * Description: used to define 8 bytes of data in 28 CAN id
 *       num_mon[0]  represents 18FFFEF0; num_mon[1]  represents 18FFFDF0;num_mon[2]  represents 18FF0803;
 *       num_mon[3]  represents 18FF0903; num_mon[4]  represents 18FF1003;num_mon[5]  represents 18FF1103;
 *       num_mon[6]  represents 18FF1203; num_mon[7]  represents 18FF1305;num_mon[8]  represents 18FF1405;
 *       num_mon[9]  represents 18FF1507; num_mon[10] represents 18FF1607;num_mon[11] represents 18FF1707;
 *       num_mon[12] represents 18FF1809; num_mon[13] represents 18FF1909;num_mon[14] represents 18FF2011;
 *       num_mon[15] represents 18FF2111; num_mon[16] represents 18FF2213;num_mon[17] represents 18FF2313;
 *       num_mon[18] represents 18FF2413; num_mon[19] represents 18FF2913;num_mon[20] represents 18FF2515;
 *       num_mon[21] represents 18FF2615; num_mon[22] represents 18FF2717;num_mon[23] represents 18FF2817;
 *       num_mon[24] represents 18FF5030; num_mon[25] represents 18FF5130;num_mon[26] represents 18FF5230;
 *       num_mon[27] represents 18FF5330; num_mon[28] represents 18FEEE00;num_mon[29] represents 18FFEEF0;
 *       num_mon[30] represents 18FEF700; num_mon[31] represents 0CF00400;num_mon[32] represents 18FF2715;
 *       num_mon[33] represents 18FF2815;
 * Date: 2017-9-30
 * Author: shone
 **********************************************************************/

int num_mon[34][8];
int can_recv_flag = 0;	
int get_flag = -1;     //区分是第几页页面

Thread::Thread()
{
    stopped = false;	//析构函数的初始化
    warn_timer = new QTimer(this);

}

//控制器线程
void Thread::run()
{
    struct can_frame frame_temp;

    QObject::connect(warn_timer, SIGNAL(timeout()), this, SLOT(warnFun_slot()));

    warn_timer->start(5000);

    while(!stopped)
    {
        warn_timer->stop();		//连续的关闭和开启定时器，这样就不会每隔五秒timeout了
        warn_timer->start(5000);

        can_recv_flag = can0_recv(&frame_temp);	//得到CAN数据，储蓄在frame_temp里

        /* CAN的结构体数据转换成一般数组 */
        QByteArray bArray;
        bArray.resize(8);

        /* 根据不同ID向主线程（即界面线程）发送数据信号 */
        if(get_flag == 0){
            if(frame_temp.can_id == 0x98FFFEF0)                         //发动机工作时间和总里程,注意，这里是0x18FFFEF0+0x80000000
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[0][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FFFDF0)                    //系统电压等,注意，这里是0x18FFFDF0+0x80000000
            {

                for(int j = 0; j < 8; j++)
                {
                    num_mon[1][j] = (int)frame_temp.data[j];             //类型转换
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF0803)                    //米计里程
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[2][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1003)                    //卸粮筒等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[4][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1707)                    //粮食干重和升运器转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[11][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1909)                    //空、前进、倒档
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[13][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }else if(frame_temp.can_id == 0x98FF2011)                    //抛撒器角度和切碎器转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[14][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
        }else if(get_flag == 1){
            if(frame_temp.can_id == 0x98FF1607)                         //粮食干重
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[10][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF2011)                    //抛撒器角度和切碎器转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[14][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2313)                    //仿形高度等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[17][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2413)                    //割宽
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[18][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
        }else if(get_flag == 2){
            if(frame_temp.can_id == 0x98FF1003)                         //卸粮筒等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[4][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1103)                    //电磁阀等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[5][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

        }else if(get_flag == 3){

        }else {
            if(frame_temp.can_id == 0x98FF1405)                         //备用
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[8][j] = (int)frame_temp.data[j];
                }
                SLEEP;
                qDebug() << "page not in 0 1 2 3";
            }else if(frame_temp.can_id == 0x98FF2111)                    //备用
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[15][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }else if(frame_temp.can_id == 0x98FF2817)                    //备用
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[23][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

        }
    }
    stopped = false;
    exec();
}
void Thread::warnFun_slot()
{
    if(can_recv_flag == 0)
    {
        /* 通信故障的警告信号 */
        emit warn_signal();
    }
}

//发动机及传感器线程
Thread2::Thread2()
{
    stopped = false;	//析构函数的初始化
    warn_timer = new QTimer(this);
}

void Thread2::run()
{
    struct can_frame frame_temp;
    QObject::connect(warn_timer, SIGNAL(timeout()), this, SLOT(warnFun_slot()));
    warn_timer->start(5000);

    QString threadText3 = QString("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
    qDebug() << "can thread2 id is : " << threadText3;

    while(!stopped)
    {
        int can_recv_flag =0;	/* 判断can是否接受到数据的标志 */

        warn_timer->stop();		//连续的关闭和开启定时器，这样就不会每隔五秒timeout了
        warn_timer->start(5000);

        can_recv_flag = can0_recv2(&frame_temp);	//得到CAN数据，储蓄在frame_temp里

        /* CAN的结构体数据转换成一般数组 */
        QByteArray bArray;
        bArray.resize(8);

        /* 根据不同ID向主线程（即界面线程）发送数据信号 */
        if(get_flag == 0){
            if(frame_temp.can_id == 0x98FF1203)                         //复脱器转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[6][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF1305)                    //清选风扇等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[7][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1809)                    //损失率和含杂率
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[12][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2515)                    //割刀频率等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[20][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF2913)                    //行驶速度
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[19][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2717)                    //滚筒转速和转矩
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[22][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FEEE00)                    //发动冷却水温
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[28][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FEEF00)                    //发动机油压力
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[29][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FEF700)                    //系统电压
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[30][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x8CF00400)                    //发动机转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[31][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

        }else if(get_flag == 1){
            if(frame_temp.can_id == 0x98FF0903)                         //马达转速
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[3][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF1305)                    //清选风扇等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[7][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF1507)                    //含水率等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[9][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }else if(frame_temp.can_id == 0x98FF2515)                    //割刀频率等
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[20][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

            else if(frame_temp.can_id == 0x98FF2615)                    //喂入量
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[21][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2717)                    //滚筒转速和转矩
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[22][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2715)                    //拨禾轮液压马达流量的低高值...
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[32][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
            else if(frame_temp.can_id == 0x98FF2815)                    //拨禾轮液压马达进油压力的低高值...
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[33][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }

        }else if(get_flag == 2){

        }else if(get_flag == 3){

        }else{

            if(frame_temp.can_id == 0x98FF2213)                             //导航辅助驾驶调节
            {
                for(int j = 0; j < 8; j++)
                {
                    num_mon[16][j] = (int)frame_temp.data[j];
                }
                SLEEP;
            }
        }
    }
    stopped = false;
    exec();
}
void Thread2::warnFun_slot()
{
    if(can_recv_flag == 0)
    {
        /* 通信故障的警告信号 */
        emit warn_signal();
    }
}

void Thread2::stop()
{
    stopped = true;
}


/* 返回can数据的线程 */
ThreadBack::ThreadBack()
{
    stopped = false;
    send_timer = new QTimer(this);
}


/* 返回数据的线程 */
void ThreadBack::run()
{
    QObject::connect(send_timer, SIGNAL(timeout()), this, SLOT(can_return()));
    send_timer->start(100);

    QByteArray array;
    array.resize(8);

    if(flag_send_setting)
    {
        emit settingpara1(dataArray130);
        emit settingpara2(dataArray230);

        array = arr12;
        emit set_arr(array);
        flag_send_setting = 0;
    }
}

void ThreadBack::can_return()
{

}

void ThreadBack::canTest(QByteArray bArray)
{
    ba = bArray;
}


void ThreadBack::stop()
{
    stopped = true;
}
