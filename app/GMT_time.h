#ifndef _GMT_TIME_H_
#define _GMT_TIME_H_
#include <stdio.h>
#include <string.h>
#include <time.h>
 
typedef struct LocalTimeInformation
{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
}LocalTimeInfo;

 /*计算当前时间到格林威治时间总共过了多少秒，以当前北京地区东八区时间为准*/
 unsigned long mktime_second(unsigned int year0,unsigned int mon0,unsigned int day,unsigned int hour,unsigned int min,unsigned int sec);

 
 /*通过格林威治时间，计算本地时间*/
 int GMT_toLocalTime(unsigned long gmt_time,unsigned int* year,unsigned int* month,unsigned int* day,unsigned int* hour,unsigned int* minute,unsigned int* sec);
 int day_diff(int year_start, int month_start, int day_start
			 , int year_end, int month_end, int day_end);

			 
 #endif


