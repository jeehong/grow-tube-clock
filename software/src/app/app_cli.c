/* FreeRTOS includes. */
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "os_inc.h"

#include "mid_cli.h"
#include "mid_th.h"
#include "mid_rtc.h"

#include "app_inc.h"

#include "bsp.h"

#include "lwip/netif.h"

build_var(info, "Device information.", 0);
build_var(clear, "Clear Terminal.", 0);
build_var(date, "Read system current time.", 0);
build_var(sdate, "Set system current time,\r\n		format: sdate Y[0,99] M[1,12] D[1,31] W[1-7] H[0-23] M[0-59] S[0-59]", 7);
build_var(th, "Read the ambient temperature(C) and humidity(%).", 0);
build_var(led, "Turn ON/OFF led blink,\r\n		format: led color[R,G,B] state[1:ON,0:OFF].", 2);
build_var(reboot, "Reboot system.", 0);
build_var(lcd, "Turn ON/OFF lcd display,\r\n		format: lcd state[1:ON,0:OFF].", 1);
#if ( configUSE_TRACE_FACILITY == 1 )
build_var(top, "List all the tasks state.", 0);
#endif
build_var(setlcd, "Time ON/OFF lcd display and led blink.\r\n		format: setlcd on[800] off[1930]", 2);
build_var(ip, "Report network infomation of host.\r\n		format: ip", 0);

static void app_cli_register(void)
{
	mid_cli_register(&info);
	mid_cli_register(&clear);
	mid_cli_register(&date);
	mid_cli_register(&sdate);
	mid_cli_register(&th);
	mid_cli_register(&led);
	mid_cli_register(&reboot);
	mid_cli_register(&lcd);
	mid_cli_register(&setlcd);
    mid_cli_register(&ip);
	#if ( configUSE_TRACE_FACILITY == 1 )
	mid_cli_register(&top);
	#endif
}

cmd_handle(info)
{
	U8 index;
	
	const static char *dev_info[] =
	{
		" 	Dev:	STM32F103RCT6 \r\n",
		" 	Cpu:	ARM 32-bit Cortex-M3\r\n",
		" 	Freq: 	72 MHz max,1.25 DMIPS/MHz\r\n",
		" 	Mem:	256 Kbytes of Flash memory\r\n",
		" 	Ram:	64 Kbytes of SRAM\r\n",
		NULL
	};

	(void) help_info;
	(void) argv;
	configASSERT(dest);

	for(index = 0; dev_info[index] != NULL; index++)
	{
		strcat(dest, dev_info[index]);
	}

	return pdFALSE;
}

cmd_handle(clear)
{
	const static char *clear_string = "\033[H\033[J";

	(void) help_info;
	(void) argv;
	configASSERT(dest);

	strcpy(dest, clear_string);

	return pdFALSE;
}

cmd_handle(date)
{
	struct rtc_time tm;

	(void) help_info;
	(void) argv;
	configASSERT(dest);

	mid_rtc_get_time(&tm);
	sprintf(dest, "\t20%02d-%02d-%02d %d %02d:%02d:%02d\r\n", 
												tm.year,
												tm.mon,
												tm.mday,
												tm.wday,
												tm.hour,
												tm.min,
												tm.sec);
	
	return pdFALSE;
}

cmd_handle(sdate)
{
	struct rtc_time tm;

	(void) help_info;
	configASSERT(dest);

	tm.year = atoi(argv[1]);
	tm.mon = atoi(argv[2]);
	tm.mday = atoi(argv[3]);
	tm.wday = atoi(argv[4]);
	tm.hour = atoi(argv[5]);
	tm.min = atoi(argv[6]);
	tm.sec = atoi(argv[7]);
	if(rtc_valid_tm(&tm))
	{
		mid_rtc_set_time(&tm);
		memset(&tm, 0, sizeof(struct rtc_time));
		mid_rtc_get_time(&tm);
		sprintf(dest, "\tSet time success,current: 20%d-%d-%d %d %d:%d:%d\r\n", 
													tm.year,
													tm.mon,
													tm.mday,
													tm.wday,
													tm.hour,
													tm.min,
													tm.sec);
	}
	else
	{
		strcpy(dest, "	Format incorrect,please try again!\r\n");
	}

	return pdFALSE;
}

cmd_handle(th)
{
	struct _mid_th_data_t xp_th;
	
	(void) help_info;
	(void) argv;
	configASSERT(dest);

	xp_th.temperature = app_th_get_data(TEMP);
	xp_th.humidity = app_th_get_data(HUM);
	if(xp_th.temperature != 0 || xp_th.humidity != 0)
	{
		sprintf(dest, "\tTemperature %.1f(C)  Humidity %.1f(%%)\r\n", xp_th.temperature, xp_th.humidity);
	}
	else
	{
		sprintf(dest, "\tError: read temperature and humidity timeout!\r\n");
	}
	return pdFALSE;
}

cmd_handle(led)
{
	TaskHandle_t pled;
	char state, color, ret = pdTRUE;
	
	(void) help_info;
	configASSERT(dest);
	
	color = *argv[1];
	state = atoi(argv[2]);
	if((color != 'R' && color != 'G' && color != 'B') 
		|| (state != 1 && state != 0))
	{
		sprintf(dest, "\tCommand of led control Format incorrect,please try again.\r\n");
		ret = pdFALSE;
	}

	if(ret == pdTRUE)
	{
		pled = main_get_task_handle(TASK_HANDLE_LED);
		switch(color)
		{
			case 'R': app_led_set_color(0); break;
			case 'G': app_led_set_color(1); break;
			case 'B': app_led_set_color(2); break;
			default: break;
		}
		
		if(state == 1)
		{
			vTaskResume(pled);
		}
		else
		{
			vTaskSuspend(pled);
		}
		sprintf(dest, "\tLed task is %s\r\n", state ? "working." : "stoped.");
        ret = pdFALSE;
	}
	return ret;
}

cmd_handle(reboot)
{
	(void) help_info;
	configASSERT(dest);

	NVIC_SystemReset();
	
	return pdFALSE;
}

cmd_handle(ip)
{
    struct ip_addr ipaddr, netmask, gw;
    u8_t mac[NETIF_MAX_HWADDR_LEN];
    
	(void) help_info;
	configASSERT(dest);

	LwIP_get_network_info(&ipaddr, &netmask, &gw, mac);
    
    sprintf(dest, "\tip_addr: %d.%d.%d.%d\r\n\tnetmask: %d.%d.%d.%d\r\n\tgateway: %d.%d.%d.%d\r\n\tmac    : %02X-%02X-%02X-%02X-%02X-%02X\r\n",
                        ip4_addr1(&ipaddr),
                        ip4_addr2(&ipaddr),
                        ip4_addr3(&ipaddr),
                        ip4_addr4(&ipaddr),
                        ip4_addr1(&netmask),
                        ip4_addr2(&netmask),
                        ip4_addr3(&netmask),
                        ip4_addr4(&netmask),
                        ip4_addr1(&gw),
                        ip4_addr2(&gw),
                        ip4_addr3(&gw),
                        ip4_addr4(&gw),
                        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
	
	return pdFALSE;
}

cmd_handle(lcd)
{
	char state, ret = pdTRUE;
	
	(void) help_info;
	(void) argv;
	configASSERT(dest);
	
	state = atoi(argv[1]);
	if((state != 1) && (state != 0))
	{
		sprintf(dest, "\tCommand of lcd control Format incorrect,please try again.\r\n");
		ret = pdFALSE;
	}

	if(ret == pdTRUE)
	{
		if(state == 1)
		{
			app_display_on(NULL);
		}
		else
		{
			app_display_off(NULL);
		}
		sprintf(dest, "\tLcd display task is %s\r\n", state ? "working." : "stoped.");
	}
	return pdFALSE;
}

cmd_handle(top)
{
#if ( configUSE_TRACE_FACILITY == 1 )
	#define MAX_TASKS 	(30)

	static U8 curr_task = 0;
	static TaskStatus_t ptasks[MAX_TASKS];
	TimeOut_t running_time;
	TaskStatus_t *p;
	U32 run_time;
	U8 num_of_tasks = uxTaskGetNumberOfTasks();
	char temp_str[30];

	(void) help_info;
	(void) argv;
	configASSERT(dest);
	vTaskDelay(10);
	if(curr_task == 0)
	{
		vTaskSetTimeOutState(&running_time);
		if(num_of_tasks > MAX_TASKS)
		{
			sprintf(dest, "Error: real num(%d) of tasks more than defines(%d)!\r\n", num_of_tasks, MAX_TASKS);
		}
		uxTaskGetSystemState(ptasks, MAX_TASKS, NULL);
		sprintf(dest, "\033[32;49;7m        PRI     STATE   MEM(W)  %%TIME   NAME            \033[32;49;0m\r\n");
	}
	p = &ptasks[curr_task];
	sprintf(temp_str, "\t%d\t", p->uxCurrentPriority);
	strcat(dest, temp_str);
	switch(p->eCurrentState)
	{
		case eRunning: strcat(dest, "run"); break;
		case eReady: strcat(dest, "ready"); break;
		case eBlocked: strcat(dest, "block"); break;
		case eSuspended: strcat(dest, "suspend"); break;
		case eDeleted: strcat(dest, "deleted"); break;
		case eInvalid: strcat(dest, "invalid"); break;
		default: strcat(dest, "null"); break;
	}
	run_time = (float)p->ulRunTimeCounter / (float)running_time.xTimeOnEntering * 1000;
	sprintf(temp_str, "\t%d\t%c%d\t", p->usStackHighWaterMark, run_time < 10 ? '<' : ' ', run_time < 10 ? 1 : run_time / 10);
	strcat(dest, temp_str);
	strcat(dest, p->pcTaskName);
	strcat(dest, "\r\n");
	curr_task ++;
	if(curr_task == num_of_tasks)
	{
		curr_task = 0;
		return pdFALSE;
	}
	else
		return pdTRUE;
#endif
}

cmd_handle(setlcd)
{
	U16 on_hour, on_min, off_hour, off_min;
	U16 on, off;

	(void) help_info;
	configASSERT(dest);

	on = atoi(argv[1]);
	off = atoi(argv[2]);

	on_hour = on / 100;
	on_min = on % 100;
	off_hour = off / 100;
	off_min = off % 100;
	
	if(on_hour != 0
		|| on_min != 0
		|| off_hour != 0
		|| off_min != 0)
	{
		app_time_set_showtime(on, off);
		sprintf(dest, "\tSet show time success,current: ON-%02d:%02d  OFF-%02d:%02d\r\n", 
													on_hour,
													on_min,
													off_hour,
													off_min);
	}
	else
	{
		app_time_get_showtime(&on, &off);
		on_hour = on / 100;
		on_min = on % 100;
		off_hour = off / 100;
		off_min = off % 100;
		sprintf(dest, "\tFormat incorrect,please try again! current: ON-%02d:%02d  OFF-%02d:%02d\r\n",
													on_hour,
													on_min,
													off_hour,
													off_min);
	}

	return pdFALSE;
}

void app_cli_init(U8 priority, char *t, TaskHandle_t *handle)
{
	mid_cli_init(400, priority, t, handle);

	app_cli_register();
}


