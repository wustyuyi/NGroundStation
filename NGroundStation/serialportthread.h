#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <qmutex.h>

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)      ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


#define NCLink_Head0 0xFF
#define NCLink_Head1 0xFC
#define NCLink_End0  0xA1
#define NCLink_End1  0xA2
#define BufferSizeMax (65536*2)


extern qint64 thr_cnt1,thr_cnt2;
extern qint64 thr_cnt3,thr_cnt4;
extern qint64 thr_cnt5;






//第一帧数据，功能字0x01  数据长度12
extern float pitch,roll,yaw;//姿态角
extern float pitch_gyro,roll_gyro,yaw_gyro;//姿态角速度
extern float vbat;
extern float imu_temp;
extern uint8_t fly_mode;//飞行模式
extern uint8_t armed_flag;//解锁标志

//第二帧数据，功能字0x02  数据长度18
extern int16_t ax,ay,az;
extern int16_t gx,gy,gz;
extern int16_t mx,my,mz;


//第三帧数据，功能字0x03  数据长度16
extern int16_t rc[8];


//第四帧数据，功能字0x04  数据长度14
extern double gps_lng,gps_lat,gps_alt;
extern int16_t gps_pdop;
extern uint8_t gps_state;
extern uint8_t gps_satellite_num;

//第五帧数据，功能字0x05  数据长度8
extern float lat_vel_obs,lng_vel_obs;
extern float lat_pos_obs,lng_pos_obs;

//第六帧数据，功能字0x06  数据长度8
extern float alt_obs_baro,alt_obs_ult;
extern float opt_vel_p,opt_vel_r;


//第七帧数据，功能字0x06  数据长度8
extern float alt_pos_fus;//融合高度
extern float alt_vel_fus;//融合高度
extern float alt_accel_fus;//融合高度

//第八帧数据，功能字0x08  数据长度24
extern float lat_pos_fusion,lng_pos_fusion;
extern float lat_vel_fusion,lng_vel_fusion;
extern float lat_accel_fusion,lng_accel_fusion;


//第九帧数据，功能字0x09  数据长度36
extern float userdata[9];

//第10、11、12、13、14、15帧数据，功能字0x0A、0x0B、0x0C、0x0D、0x0E、0x0F  数据长度18
extern uint16_t kp[18];
extern uint16_t ki[18];
extern uint16_t kd[18];

//第16帧数据，功能字0x10  数据长度16
extern uint16_t targeheight,safeheight,safevbat,maxheight;
extern uint16_t maxradius,maxupvel,maxdownvel,maxhorvel;

//第17帧数据，功能字11  数据长度36
extern uint8_t gyro_auto_cal_flag;
extern float gyro_x_raw,gyro_y_raw,gyro_z_raw;
extern float acce_x_raw,acce_y_raw,acce_z_raw;
extern float mag_x_raw,mag_y_raw,mag_z_raw;


//第17帧数据，功能字12  数据长度56
extern float gyro_x_offset,gyro_y_offset,gyro_z_offset;
extern float acce_x_offset,acce_y_offset,acce_z_offset;
extern float acce_x_scale,acce_y_scale,acce_z_scale;
extern float mag_x_offset,mag_y_offset,mag_z_offset;
extern float pitch_offset,roll_offset;


#endif // SERIALPORTTHREAD_H
