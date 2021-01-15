/****************************************************************************

*****************************************************************************/

#include <cyg/kernel/kapi.h>

void rtc_time_start(void)
{
}

void gxos_get_rtc_time_ms(unsigned int *cur_time)
{
	return;
}

unsigned int cur_time_start=0,cur_time_finish=0;
static int rtc_inited=0;
void RTC_Init(void)
{

}	
	
void StartTimer(void)
{
/*
	start_rtc();
*/

	rtc_time_start();//start rtc timer
	gxos_get_rtc_time_ms(&cur_time_start);  //get RTC time
}

unsigned int GetTimerMSec(void)
{

/*
	stop_rtc();
        time_token = rRTC_SEC * 1000000 + rRTC_MS * 1000 + rRTC_US*10;
        time_token /= 100000;	
*/
	gxos_get_rtc_time_ms(&cur_time_finish);  //get RTC time
	        
	return (cur_time_finish-cur_time_start);

}

