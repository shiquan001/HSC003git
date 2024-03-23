#include "GMT_time.h"

 /*
�㷨������
���㷨����˼���Ǽ���������ڵ� 0��3��1�յ�������Ȼ���������ȡ�����ļ����
m1 = (month_start + 9) % 12; �����ж������Ƿ����3�£�2�����ж�����ı�ʶ���������ڼ�¼��3�µļ��������
y1 = year_start - m1/10; �����1�º�2�£��򲻰�����ǰ�꣨��Ϊ�Ǽ��㵽0��3��1�յ���������
d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (day_start - 1);
? ? ���� 365*y1 �ǲ�����������һ���������
? ? y1/4 - y1/100 + y1/400 ?�Ǽ���������������һ�죬
(m2*306 + 5)/10?���ڼ��㵽��ǰ�µ�3��1�ռ��������306=365-31-28��1�º�2�£���5��ȫ���в���31���·ݵĸ���
(day_start - 1)?���ڼ��㵱ǰ�յ�1�յļ��������
*/
int day_diff(int year_start, int month_start, int day_start
			, int year_end, int month_end, int day_end)
{
	int y2, m2, d2;
	int y1, m1, d1;
	
	m1 = (month_start + 9) % 12;
	y1 = year_start - m1/10;
	d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (day_start - 1);
 
	m2 = (month_end + 9) % 12;
	y2 = year_end - m2/10;
	d2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (day_end - 1);
	
	return (d2 - d1);
}


/*���㵱ǰʱ�䵽��������ʱ���ܹ����˶����룬�Ե�ǰ��������������ʱ��Ϊ׼*/

unsigned long mktime_second(unsigned int year0,unsigned int mon0,unsigned int day,unsigned int hour,unsigned int min,unsigned int sec)
{
	int leap_years = 0;
	unsigned long days = 0;
	unsigned long seconds = 0;
	unsigned long resultValue = 0;
	int i = 0;
	int year = year0 - 1 ;
	int TIME_ZONE	= 8;//���ڱ�ʾ��ǰʱ����=8��ʾ����ʱ������������λСʱ����Ϊ�ȱ���������ʱ���8��Сʱ
	//              			     1,  2, 3, 4, 5, 6, 7, 8, 9, 10,11,12
	int isleapyear = 0;
    int month_days[] = {31, 28, 31,30,31, 30,31,31, 30,31,30,31};
	leap_years = year/4 - year/100;//������ͨ����
	leap_years += year/400;//������������
	//����Ϊ366�죬ƽ��Ϊ365��
	days = year * 365 + leap_years;//�����ǰ�����2000�꣬�򵽴˱������˴ӹ�Ԫ0�����1999��β������
	
	//�����Ƿ�������
	if((year0%4 == 0 && year0 % 100!=0) || year0%400==0) isleapyear = 1;//����������
	//��ƽ����㣬���ϸ���Ϊֹ�ܹ��ȹ�������
	for(i=0;i<mon0 - 1;i++) days += month_days[i];
	if(mon0 >2) days +=isleapyear;//2�·�����Ҫ��29�����
	days= days + day - 1;
    
	//daysӦ�ü�ȥ1970����ǰ������,1970/1/1 0:0:0 0
	//year = 1969 	leap_years = 1969/4-1969/100 + 1969/400 = 492 - 19 + 4 = 477
	//isleapyear = 0
	//days = 1969 * 365 + 477 = 719162
 
	//���ǵ�ʱ�������⣬ʵ����������Ӧ���ڵ�ǰСʱ�Ļ���֮�ϼ���ʱ��ʱ��TIME_ZONE
	//���ڱ���ʱ�䶫������ʵ��Ӧ�ü��㵱ǰʱ�䵽1970/1/1 08:0:0 0��������
	//�� seconds = 8 * 60 * 60
     
	seconds = (hour) * 60 * 60 + (min) * 60 + sec;
	resultValue = (days - 719162) * 24 * 60 * 60;
	resultValue	+= seconds;
	resultValue -= ((unsigned long)TIME_ZONE)*60*60;
	
	return resultValue;
}
 
 
/*ͨ����������ʱ�䣬���㱾��ʱ��*/
int GMT_toLocalTime(unsigned long gmt_time,unsigned int* year,unsigned int* month,unsigned int* day,unsigned int* hour,unsigned int* minute,unsigned int* sec)
{
	int TIME_ZONE	= 8;
	unsigned long gmtTime = gmt_time + TIME_ZONE * 60 * 60;
	int leap_years = 0;
	int month_days[] = {31, 28, 31,30,31, 30,31,31, 30,31,30,31};
	int i =0;
	int days;
			
	*sec = (int)(gmtTime%60);//������
	gmtTime = gmtTime/60;//�ܹ��ж��ٷ���
 
	*minute = (int)(gmtTime%60);
	gmtTime = gmtTime/60;//�ܹ��ж���Сʱ
	
	*hour = (int)(gmtTime%24);
	gmtTime = gmtTime/24;//�ܹ��ж�����
	
	//ȥ��Сʱ�������Ӻ�ת���ɴӹ�ԪԪ�꿪ʼ�����ڵ����� 
	//����������
	gmtTime += 719162;
	////���Ȳ��������������ݺ�����
	////�������
	*year = (int)(gmtTime/365);
	////days = ���ӹ�ԪԪ�꿪ʼ��year��������� + ��ǰ����Ѿ��ȹ�������������365�������
	days = (int)(gmtTime%365);////
	while(1)
	{
		//�ܹ��ж��ٸ����꣬����Ҫ��Ӧ�ļ�ȥ��Щ����
		leap_years = (*year)/4 - (*year)/100; //������ͨ����
		leap_years += (*year)/400; //������������
		if(days < leap_years)
		{
			days+=365;
			(*year)--;
		}else break;
	}
	days -= leap_years;
	(*year)++;
	days++;
	//��������ܹ��ȹ��˶�����
	if(((*year)%4 == 0 && (*year) % 100!=0) || (*year)%400==0) month_days[1] = 29;//����������,�޸Ķ��·�Ϊ29��
	*month = 1;
	for(i=0;i<12;i++)
	{
		if(days <= month_days[i])
		{
			break;
		}
		else
		{
			days -=month_days[i];
			(*month)++; 
		}
	}
	*day =days; 
	return 0;
}
/*��������ʱ�����1970��01��01��00ʱ00��00���������ڵ�������*/

#if 0
int main()
{
	LocalTimeInfo LocalTime;
	unsigned long gmt_time=1552288600;  // 2019/3/11 15:16:40
	time_t seconds=1552288600;
	struct tm *gm_date;
	printf("����һ�������Լ�д�ļ����㷨��\n");
	/*�����Լ���ת������ ��������ʱ�� -> ����ʱ��*/
	GMT_toLocalTime(gmt_time,&LocalTime.year,&LocalTime.month,&LocalTime.day,&LocalTime.hour,&LocalTime.minute,&LocalTime.second);				
	printf("GMT_Time Input=%ld\n",gmt_time);
	/*��ӡ����ʱ��*/
	printf("MyLocalTime=%d-%d-%d %d:%d:%d\n",LocalTime.year,LocalTime.month,LocalTime.day,LocalTime.hour,LocalTime.minute,LocalTime.second); 										 
	/*�����Լ���ת������ ����ʱ�� -> ��������ʱ�� */										 
	printf("MyLocalTime To GMT_Time=%ld\n",mktime_second(LocalTime.year,LocalTime.month,LocalTime.day,LocalTime.hour,LocalTime.minute,LocalTime.second));
 

	printf("�����������ÿ⺯�������㷨��\n");									 
	/*���ÿ�ת������ ��������ʱ�� -> ����ʱ��   */
	gm_date=localtime(&seconds);
	//�⺯��Ϊ�˺ü�����Ǵ�1900�꿪ʼ���� GMT+8���Ǳ���ʱ��,stm32��Ҫ��8������linux����Ҫ��
	printf("SysLocalTime=%d-%d-%d %d:%d:%d\n",gm_date->tm_year+1900,gm_date->tm_mon+1, gm_date->tm_mday,gm_date->tm_hour,  gm_date->tm_min,gm_date->tm_sec);
												
	/*���ÿ�ת������ ����ʱ�� -> ��������ʱ�� */								   
	printf("SysLocalTime To LocalTime=%ld\n",mktime(gm_date));
}
#endif
