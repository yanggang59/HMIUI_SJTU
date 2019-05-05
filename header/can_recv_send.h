#ifndef CAN_RECV_SEND
#define CAN_RECV_SEND

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>	//这四条是can通信的头文件
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/socket.h>
#include <linux/can/error.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

/* linux 文件的读写头文件 */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <QByteArray>

#ifndef PF_CAN			//条件编译
    #define PF_CAN 29
#endif

#ifndef AF_CAN
    #define AF_CAN PF_CAN
    #define CAN_EFF_FLAG 0x80000000U	//扩展帧的标识
#endif

extern int can0_recv(can_frame frame[]);
extern int can0_recv2(can_frame frame[]);

extern void can0_send(QByteArray data,int id);    //can0 send

extern void can1_send(QByteArray data ,int id);  //can1 send

extern void set_bitrate(void);

#endif // CAN_RECV_SEND

