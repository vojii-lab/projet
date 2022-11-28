/* USER CODE BEGIN Header */
//Team A&B
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ARM_MATH_CM4
#include "arm_math.h"

#include "stdio.h"
#include "MCUFRIEND_kbv.h"
#include "avatar.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct Avatar avatar;
struct Obstacle obstacle;

int gameover = 0;
volatile int scroll = 0;
int timer = 0;
int full_timer = 0;
int speed = 200;

// parametres pour echo
volatile float valeur1_capteur = 0;
volatile float valeur2_capteur = 0;
volatile float difference_capteur = 0;
volatile int first_captured =0;
volatile float distance = 0;

// parametre pour faire bouger le dino et envoyer les obstacles

float scale = 210.0/25.0;                     // le scale est la valeur maximale voulu / l'écart max entre les mesures (30-5 = 25)
volatile float compteur = 0;
volatile float flag_no_detection = 0; 
volatile float postx = 0;

volatile float flag_recu = 0;
volatile float flag_go = 0;

//Valeurs pour le dino joué
float posdino = 0;
float iddino = 0;

//Valeurs constemment updatées
uint8_t pos;
uint8_t id;

//Valeurs gardées
uint8_t posdinoRecu=0;
uint8_t idRecu=0;
	

	
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifndef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


void HAL_SYSTICK_Callback(void) {
	full_timer++;
	timer++;
	
	if (timer >= speed){
		scroll = 1;
		timer = 0;
		//speed--;
	}
}

uint8_t data[4] = {255,0,0,254};
uint8_t Rx_data[4];
uint8_t count = 0;

//RECEPTION
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	
	//Récupère les messages
	for(int i=0; i<4; i++){
		if(Rx_data[i] == 255 && Rx_data[(i+3)%4]){
			pos = Rx_data[(i+1)%4];
			id = Rx_data[(i+2)%4];
		}
	}
	
	//On a reçu que l'adversaire a enlevé sa main
	if(id != 0 & flag_recu == 0){
			posdinoRecu = pos;
			idRecu = id;
			flag_recu = 1;   //On va alors cesser d'enregistrer de nouvelles valeurs tant que la main n'a pas été remise
		}
	
		//On a reçu que la main a été remise donc on envoie l'obstacle
	if (id == 0 & flag_recu == 1){
			flag_recu = 0; 
			flag_go = 1;  //On envoie l'obstacle
	}
	
	id = 0;

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	
	initGame(&avatar, &obstacle);
	drawAvatar(&avatar);

  // on start le timer du trig qui est en pwm //
	 HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	 
	 //on start le timer de l'écho qui est en input capture//
	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);

	//INITIALISATION OUT IN
	HAL_UART_Transmit_DMA(&huart5, data, sizeof(data));
	HAL_UART_Receive_DMA(&huart5, Rx_data, sizeof(Rx_data));
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		while(!scroll);
		scroll = 0;
	// test de la semaine 1 
	//	LCD_SetCursor(0,40); 
	//	LCD_Printf(" dis: %f",distance);
	
		moveAvatar(&avatar);
		moveObstacle(&obstacle);
		data[1] = posdino;
		data[2] = iddino;
		
		// si l'objet est plus loin que 30 cm trois fois de suite, alors on envoie un obstacle
		if(flag_no_detection == 1){  // pour le test, on n'envoie pas un obstacle, mais on affiche une ligne ou la position devrait etre. 
			LCD_DrawLine(100, postx, 110, postx, WHITE);
		}
		flag_no_detection = 0;
		iddino = 0;
		
		
		gameover = lookColision(&avatar, &obstacle);
		if (gameover) {
			HAL_Delay(5000);
				initGame(&avatar, &obstacle);
				drawAvatar(&avatar);
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
* @brief Retargets the C library printf function to the USART.
* @param None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
/* Place your implementation of fputc here */
/* e.g. write a character to the USART2 and Loop until the end
of transmission */
HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xFFFF);
return ch;
}

// on n'a pas besoin de callback le trig, puisqu'il n'y a rien a ajouter
// on doit callback le echo à la fin de la cpture pour faire la difference entre les valeurs mesurées sur le front montant et descendant
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim3)
{
	if (htim3->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
	if (first_captured == 0)
				{
					valeur1_capteur= HAL_TIM_ReadCapturedValue(htim3, TIM_CHANNEL_3);
					first_captured = 1;
	
				}
	else 
				{
					valeur2_capteur = HAL_TIM_ReadCapturedValue(htim3, TIM_CHANNEL_3);  // read second value

							if (valeur2_capteur > valeur1_capteur)
									{
										difference_capteur = valeur2_capteur-valeur1_capteur;
									}

							else if (valeur1_capteur > valeur2_capteur)
									{
										difference_capteur = (0xffff - valeur1_capteur) + valeur2_capteur;
									}

					distance = difference_capteur/58.82;
					
									
					if (distance < 5){                         // cas limite 1:  si la distance est petite que 5, alors le dino ne bouge pas et reste à 0
						posdino = 0;
						compteur = 0;
					}
					else if (distance > 30 && distance < 35){  // cas limite 2: si la distance est un peu plus grande que 30, alors le dino ne bouge pas et reste au max qui est 210
						posdino = 210;
						compteur = 0;
					}			
					else if (distance >= 35)                   //  cas limite 3: si la distance est plus grande que 35 cm
						{
							compteur ++;                           // on incrémente le compteur
							if (compteur >= 3)                     // et on check si le compteur a atteint 3 (3 lectures de 35 cm consécutif)
								{
							    flag_no_detection = 1;             // on set le flag a 1, pour trigger l'envoi des obstacles
									compteur = 0;                      // et on remet le compteur à 0
									postx = posdino;									// et la position envoyé (hauteur de l'obstacle) sera la derniere position de notre avatar
                  iddino = 4; 		//à randomiser							
								}
				    }
						else {                                   // cas voulu: si la distance est entre 5 et 30
							posdino = (distance - 5)*scale;    // on change la valeur de posy de l'avatar pour le faire bouger 
							compteur = 0;                          
						}
						
					

					
					first_captured = 0; // set it back to false
				}
		}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
