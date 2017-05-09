/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pwm_out.h"
/** @addtogroup STM32F0-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay = 0;
static __IO uint32_t OnDelay = 0;

#define ON_DELAY     7200000//Two hours in milliseconds
int pwmIndex = 0;
int pwmList[] = {
    100,
    50,
    20,
    5,
    0,
    -1
};
static int flag = 0xFF;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  
  /* Configure LED3 and LED4 on STM32F0-Discovery */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Initialize User_Button on STM32F0-Discovery */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   
  {
    GPIO_InitTypeDef  GPIO_InitStructure;
  /* Configure the GPIO_LED pin */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIOA->BRR = GPIO_Pin_9;


//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIOA->BRR = GPIO_Pin_8;
  }


  PWM_Init();
  PWM_SetDutyCycle(pwmList[pwmIndex]);

  while(1)
  {  
    static uint8_t buttonPress = 0;

    /* Check if the user button is pressed */
    if(STM_EVAL_PBGetState(BUTTON_USER) == SET)
    {
      buttonPress = (buttonPress << 1 ) | 0x01;

      if( !flag && (buttonPress > 0x01))
      {

        pwmIndex++;
        if(pwmList[pwmIndex] < 0)
          pwmIndex = 0;

        PWM_SetDutyCycle(pwmList[pwmIndex]);

        Delay(1000);
      }


      /* BlinkSpeed: 1 -> 2 -> 0, then re-cycle */
      /* Turn on LD4 Blue LED during 1s each time User button is pressed */
      STM_EVAL_LEDToggle(LED3);

    }
    else
    {
      buttonPress = 0;
    }


    if(buttonPress == 0x01)
    {
      if(flag)
      {
        OnDelay = ON_DELAY;
        STM_EVAL_LEDOn(LED4);
        GPIOA->BSRR = GPIO_Pin_9;// | GPIO_Pin_8;
        PWM_Enable();
      }
      else
      {
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
        GPIOA->BRR = GPIO_Pin_9;// | GPIO_Pin_8;
        PWM_Disable();
      }

      flag ^= 0xFF;
    }

    Delay(500);

    //after the on delay, switch off everything
    if(!flag && !OnDelay)
    {
      flag = 0xFF;
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED3);
      GPIOA->BRR = GPIO_Pin_9;// | GPIO_Pin_8;
      PWM_Disable();
    }

  }


  Delay(100);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }


  if (OnDelay != 0x00)
  {
    OnDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
