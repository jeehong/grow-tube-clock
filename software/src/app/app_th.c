#include "os_inc.h"
#include "common_type.h"
#include "mid_th.h"
#include "mid_kalman.h"
#include "mid_dbg.h"
#include "app_inc.h"

static struct _mid_th_data_t th_data;
static struct _kalman1_state_t temp_state;

void app_th_task(void *parame)
{
	kalman1_init(&temp_state, 15, 0.01);
	
	while(1)
	{
		vTaskDelay(2000);

		if(mid_th_get_data(&th_data) != STATUS_NORMAL)
		{
			dbg_string("Error: Read Temperature&Humidity faild!\r\n");
		}
		//dbg_string("%f %f\r\n", th_data.temp, kalman1_filter(&temp_state, th_data.temp));
	}
}

void app_th_refresh_display(void)
{
	U8 sht_info[7];
	U32 temp32;
	
	temp32 = th_data.temperature * 10;
	sht_info[0] = temp32 / 100;
	sht_info[1] = temp32 % 100 /10;
	sht_info[2] = temp32 % 10;
	sht_info[3] = 0;
	temp32 = th_data.humidity;
	sht_info[4] = temp32 / 10;
	sht_info[5] = temp32 % 10;
	sht_info[6] = 0x11;
	app_display_show_info(sht_info);
}

float app_th_get_data(SHT10_INFO_e element)
{
	switch (element)
	{
		case TEMP:
			return th_data.temperature;
		case HUM:
			return th_data.humidity;
		default: return 0;
	}
}
