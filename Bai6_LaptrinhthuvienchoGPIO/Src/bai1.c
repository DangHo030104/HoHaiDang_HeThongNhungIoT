/*
 * bai1.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Admin
 */
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stm32f401re.h>
#include <stm32f401re_rcc.h>
#include <stm32f401re_gpio.h>

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define BUTTON_PRESS        0
#define BUTTON_RELEASE      1

// Define LED GREEN
#define LED_GPIO_PORT       GPIOA
#define LED_GPIO_PIN        GPIO_Pin_0
#define LED_GPIO_CLOCK_EN   RCC_AHB1Periph_GPIOA

// Define BUZZER
#define BUZZER_GPIO_PORT    GPIOC
#define BUZZER_GPIO_PIN     GPIO_Pin_9
#define BUZZER_GPIO_CLOCK_EN RCC_AHB1Periph_GPIOC

// Define BUTTON B2
#define BUTTON_GPIO_PORT    GPIOB
#define BUTTON_GPIO_PIN     GPIO_Pin_3
#define BUTTON_GPIO_CLOCK_EN RCC_AHB1Periph_GPIOB

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void mainInit(void);
static void led_Init(void);
static void buttonB2_Init(void);
static void buzzer_Init(void);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

/* ====================== INIT FUNCTIONS ====================== */
static void led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED_GPIO_CLOCK_EN, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

static void buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BUZZER_GPIO_CLOCK_EN, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
}

static void buttonB2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BUTTON_GPIO_CLOCK_EN, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = BUTTON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

static void mainInit(void)
{
    led_Init();
    buzzer_Init();
    buttonB2_Init();
}

/* ====================== MAIN ====================== */
int main(void)
{
    mainInit();

    while (1)
    {
        if (GPIO_ReadInputDataBit(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == BUTTON_PRESS)
        {
            GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);          // LED ON
            GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);    // BUZZER ON
        }
        else
        {
            GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);        // LED OFF
            GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);  // BUZZER OFF
        }
    }
}

