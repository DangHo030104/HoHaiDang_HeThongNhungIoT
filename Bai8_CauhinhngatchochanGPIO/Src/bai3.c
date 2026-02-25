/*
 * bai3.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Admin
 */
#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_exti.h>
#include <stm32f401re_syscfg.h>
#include <stm32f401re_rcc.h>
#include <misc.h>

/* ===================== DEFINE ===================== */
// Define Logic GPIO_PIN
#define GPIO_PIN_SET        1
#define GPIO_PIN_RESET      0

// Define GPIO PIN
#define LED_GPIO_PORT       GPIOA
#define LED_GPIO_PIN        GPIO_Pin_5
#define LEDControl_SetClock RCC_AHB1Periph_GPIOA

#define BUTTON_GPIO_PORT        GPIOB
#define BUTTON_GPIO_PIN         GPIO_Pin_3
#define BUTTONControl_SetClock  RCC_AHB1Periph_GPIOB

#define SYSCFG_Clock        RCC_APB2Periph_SYSCFG

/* ===================== PRIVATE DATA ===================== */
// Lưu ý: Status là biến toàn cục.
static uint8_t Status = 1;

/* ===================== FUNCTION PROTOTYPES ===================== */
static void Led_Init(void);
static void Interrupt_Init(void);
static void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t Status);

/* ===================== FUNCTIONS ===================== */
static void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LEDControl_SetClock, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

static void Interrupt_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable Clock Port B
    RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);

    // Init PB3 input pull-up
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = BUTTON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

    // Enable Clock Syscfg, Connect EXTI Line 3 to PB3 pin
    RCC_APB2PeriphClockCmd(SYSCFG_Clock, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

    // Configuration Interrupt EXTI3 (Falling edge)
    EXTI_InitStructure.EXTI_Line    = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Configuration NVIC for EXTI3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI3_IRQHandler(void)
{
    if (EXTI_GetFlagStatus(EXTI_Line3) == SET)
    {
        Status++;
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
}

static void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t Status)
{
    // SET bit in BSRR Registers
    if (Status == GPIO_PIN_SET)
    {
        GPIOx->BSRRL = GPIO_PIN;
    }
    if (Status == GPIO_PIN_RESET)
    {
        GPIOx->BSRRH = GPIO_PIN;
    }
}

/* ===================== MAIN ===================== */
int main(void)
{
    SystemCoreClockUpdate();
    Led_Init();
    Interrupt_Init();

    while (1)
    {
        if (Status % 2 != 0)
        {
            LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
        }
        else
        {
            LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
        }
    }
}

