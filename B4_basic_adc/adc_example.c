#include "wifiiot_adc.h"
#include "wifiiot_uart.h"
#include "wifiiot_gpio_ex.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#include"tempsensor.h"
#define TEMP_SENSOR_ADC_CHANNEL WIFI_IOT_ADC_CHANNEL_3 // 根据BearPi的ADC引脚配置

// 函数：读取温度传感器数据并转换为温度值
float ReadTEMPSensor(void)
{
    unsigned short data = 0;
    AdcRead(TEMP_SENSOR_ADC_CHANNEL, &data, WIFI_IOT_ADC_EQU_MODEL_8, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0);

    // 将ADC值转换为电压（假设参考电压为3.3V）
    float voltage = (float)data * 3.3 / 4096.0;

    // 适用于温度传感器的电压转换公式（假设简化线性关系）
    float tempValue = voltage * 25.0; // 根据具体传感器的校准曲线进行调整

    return tempValue;
}

// 主任务函数
static void TEMPSensorTask(void)
{
    while (1)
    {
        float temp = ReadTEMPSensor();
        printf("\r\n******************************温度值 is %.2f\r\n", temp);
        usleep(1000000); // 每秒读取一次
    }
}

// 初始化函数
static void TEMPSensorInit(void)
{
    osThreadAttr_t attr;

    // 创建任务
    attr.name = "TEMPSensorTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)TEMPSensorTask, NULL, &attr) == NULL) {
      printf("[TEMPSensorInit] Failed to create TEMPSensorTask!\n");
    }
}

APP_FEATURE_INIT(TEMPSensorInit);
