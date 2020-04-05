#include "Headfile.h"
#include "serialportthread.h"
#include <iostream>
#include <QMessageBox>



qint64 thr_cnt1=0,thr_cnt2=0;
qint64 thr_cnt3=0,thr_cnt4=0;
qint64 thr_cnt5=0;

//第一帧数据，功能字0x01  数据长度24
float pitch=0,roll=0,yaw=0;//姿态角
float pitch_gyro=0,roll_gyro=0,yaw_gyro=0;//姿态角速度
float vbat=0;
float imu_temp=0;
uint8_t fly_mode=0;//飞行模式
uint8_t armed_flag=0;//解锁标志

//第二帧数据，功能字0x02  数据长度18
int16_t ax,ay,az;
int16_t gx,gy,gz;
int16_t mx,my,mz;

//第三帧数据，功能字0x03  数据长度16
int16_t rc[8];

//第四帧数据，功能字0x04  数据长度14
double gps_lng=0,gps_lat=0,gps_alt=0;
int16_t gps_pdop=0;
uint8_t gps_state=0;
uint8_t gps_satellite_num=0;

//第五帧数据，功能字0x05  数据长度8
float lat_pos_obs=0,lng_pos_obs=0;
float lat_vel_obs=0,lng_vel_obs=0;

//第六帧数据，功能字0x06  数据长度16
float alt_obs_baro=0,alt_obs_ult=0;
float opt_vel_p=0,opt_vel_r=0;

//第七帧数据，功能字0x07  数据长度8
float alt_pos_fus=0;//融合高度
float alt_vel_fus=0;//融合高度
float alt_accel_fus=0;//融合高度

//第八帧数据，功能字0x08  数据长度24
float lat_pos_fusion=0,lng_pos_fusion=0;
float lat_vel_fusion=0,lng_vel_fusion=0;
float lat_accel_fusion=0,lng_accel_fusion=0;

//第九帧数据，功能字0x09  数据长度36
float userdata[9]={0};

//第10、11、12、13、14、15帧数据，功能字0x0A、0x0B、0x0C、0x0D、0x0E、0x0F  数据长度18
uint16_t kp[18]={0};
uint16_t ki[18]={0};
uint16_t kd[18]={0};


//第16帧数据，功能字11  数据长度36
uint8_t gyro_auto_cal_flag=0;
float gyro_x_raw=0;
float gyro_y_raw=0;
float gyro_z_raw=0;
float acce_x_raw=0;
float acce_y_raw=0;
float acce_z_raw=0;
float mag_x_raw=0;
float mag_y_raw=0;
float mag_z_raw=0;


//第17帧数据，功能字12  数据长度56
float gyro_x_offset;
float gyro_y_offset;
float gyro_z_offset;
float acce_x_offset;
float acce_y_offset;
float acce_z_offset;
float acce_x_scale;
float acce_y_scale;
float acce_z_scale;
float mag_x_offset;
float mag_y_offset;
float mag_z_offset;
float pitch_offset;
float roll_offset;


//第16帧数据，功能字0x10  数据长度16
uint16_t targeheight,safeheight,safevbat,maxheight;
uint16_t maxradius,maxupvel,maxdownvel,maxhorvel;
