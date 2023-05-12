/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "touchsensing.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>
#include <stdlib.h>
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
TSC_HandleTypeDef htsc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char msg[]="press the button to start the match!\r";
char win[]="Congratulation, you win\r";
char lose[]="Game over, try again\r";
char green[]="\x1b[32m";
char new_screen[]="\x1b[H";
char hide_cursor[]="\x1b[?25l";
//int esc=27;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TSC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int start=0;
int game=0;			// 1=ongoing match 2=user victory 0=not started/user lost

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin==GPIO_PIN_0){						// if the blue button is pressed it will start the match
		start=1;
	}else if(GPIO_Pin==GPIO_PIN_0){						// enable the reset with the button
		NVIC_SystemReset();
	}
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
  MX_USART1_UART_Init();
  MX_TSC_Init();
  MX_TOUCHSENSING_Init();
  /* USER CODE BEGIN 2 */

  	srand(time(0));
  while(1){
		start=0;
		int count=0;
		HAL_UART_Transmit(&huart1,(const uint8_t*)&msg,37,100);					//inform the user of how start playing
		HAL_UART_Transmit(&huart1,(const uint8_t*)&green,5,100);				//set color of the court green
		HAL_UART_Transmit(&huart1,(const uint8_t*)&hide_cursor,6,100);				//hide the cursor to make it better

		//prematch waiting for the press of the button
		while (start==0){
			HAL_Delay(100);
			if(count>=10){
				count=0;
				HAL_UART_Transmit(&huart1,(const uint8_t*)&green,5,100);
				HAL_UART_Transmit(&huart1,(const uint8_t*)&hide_cursor,6,100);
				HAL_UART_Transmit(&huart1,(const uint8_t*)&msg,37,100);
			}else{
				count++;
			}
		}
		// set up the match
		HAL_UART_Transmit(&huart1,(const uint8_t*)&new_screen,3,100);
		const int size=40;
		  char campo[30][40]={{'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'},
				  	  	  	  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
							  {'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','o',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
							  {'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
							  {'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'}};
		  int px=19;			//x of the ball
		  int py=14;			//y of the ball
		  int gamer=14;			//y of the player
		  int computer=14;		//y of the computer
		  int i;
		  int direction=-1; 		//-1=left -2=left-up -3=left-down 1=right 2=right-up 3=right-down
		  const char ball='o';
		  const char space=' ';
		  const char end_line='\r';
		  game=1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		  while (1)
	  	  	  {
		  	  	//show the court to the user
				  for (i = 0; i < 30; i=i+1) {
					  HAL_UART_Transmit(&huart1,(const uint8_t*)&campo[i],40,100);
					  HAL_UART_Transmit(&huart1,(const uint8_t*)&end_line,1,100);
				  }
				 // changing the position of the computer according to the direction with possibility of fail
				  int n = rand()%1000;
				  if(n<980){
					  if(direction==-2 || direction==2){
						  if(campo[computer-2][size-1]==space && py<size-2){
							  campo[computer-2][size-1]='|';
							  campo[computer+1][size-1]=space;
							  computer--;
						  }
					  }else if(direction==-3 || direction==3){
						  if(campo[computer+2][size-1]==space && py>1){
							  campo[computer+2][size-1]='|';
							  campo[computer-1][size-1]=space;
							  computer++;
						  }
					  }
				  }

				  campo[py][px]=space;

				  // changing position of the ball according to the direction of his direction
				  if (direction==-1){
					  if (px==0){
						  game=0;					//the player has missed the ball
						  break;
				  }else if(px==1 && campo[py][0]=='|'){				//check the possibility of rebound on the player
						  if(gamer==py){				//changing direction according on where the player hit the ball
							  direction=1;
							  campo[py][px]=ball;
						  }else if(gamer==py-1){
							  direction=3;
							  campo[py][px]=ball;
						  }else{
							  direction=2;
							  campo[py][px]=ball;
						  }
					  }else{
						  campo[py][px-1]=ball;				//no rebounds so it moves the ball
						  px=px-1;
					  }
				  }else if (direction==-2){
					  if (px==0){
						  game=0;					//the player has missed the ball
						  break;
					  }else if(px==1 && campo[py-1][0]=='|'){		//check the possibility of rebound on the player
						  if(gamer==py-1){				//changing direction according on where the player hit the ball
							  direction=1;
							  campo[py][px]=ball;
						  }else if(gamer==py-2){
							  direction=3;
							  campo[py][px]=ball;
						  }else{
							  direction=2;
							  campo[py][px]=ball;
						  }
					  }else{
						  if(campo[py-1][px-1]=='-'){			//if the ball rebound on the upper part of the court it change the direction
							  direction=-3;
							  campo[py][px]=ball;
						  }else{
							  campo[py-1][px-1]=ball;		//no rebounds so it moves the ball
							  px=px-1;
							  py=py-1;
						  }
					  }
				  }else if (direction==-3){
					  if (px==0){
						  game=0;					//the player has missed the ball
						  break;
					  }else if(px==1 && campo[py+1][0]=='|'){		//check the possibility of rebound on the player
						  if(gamer==py+1){				//changing direction according on where the player hit the ball
							  direction=1;
							  campo[py][px]=ball;
						  }else if(gamer==py){
							  direction=3;
							  campo[py][px]=ball;
						  }else{
							  direction=2;
							  campo[py][px]=ball;
						  }
					  }else{
						  if(campo[py+1][px-1]=='-'){			//if the ball rebound on the lower part of the court it change the direction
							  direction=-2;
							  campo[py][px]=ball;
						  }else{
							  campo[py+1][px-1]=ball;		//no rebounds so it moves the ball
							  px=px-1;
							  py=py+1;
						  }
					  }
				  }else if(direction==1){
					  if (px==size-1){
						  game=2;					//the computer has missed the ball
						  break;
					  }else if(px==size-2 && campo[py][size-1]=='|'){ 	//check the possibility of rebound on the computer
						  if(computer==py){				//changing direction according on where the computer hit the ball
							  direction=-1;
							  campo[py][px]=ball;
						  }else if(computer==py-1){
							  direction=-3;
							  campo[py][px]=ball;
						  }else{
							  direction=-2;
							  campo[py][px]=ball;
						  }
					  }else{
						  campo[py][px+1]=ball;				//no rebounds so it moves the ball
						  px=px+1;
					  }
				  }else if(direction==2){
					  if (px==size-1){
						  game=2;					//the computer has missed the ball
						  break;
					  }else if(px==size-2 && campo[py-1][size-1]=='|'){	//check the possibility of rebound on the computer
						  if(computer==py-1){				//changing direction according on where the computer hit the ball
							  direction=-1;
							  campo[py][px]=ball;
						  }else if(computer==py-2){
							  direction=-3;
							  campo[py][px]=ball;
						  }else{
							  direction=-2;
							  campo[py][px]=ball;
						  }
					  }else{
						  if(campo[py-1][px+1]=='-'){			//if the ball rebound on the upper part of the court it change the direction
							  direction=3;
							  campo[py][px]=ball;
						  }else{
							  campo[py-1][px+1]=ball;		//no rebounds so it moves the ball
							  px=px+1;
							  py=py-1;
						  }
					  }
				  }else if(direction==3){
					  if (px==size-1){
						  game=2;					//the computer has missed the ball
						  break;
					  }else if(px==size-2 && campo[py+1][size-1]=='|'){	//check the possibility of rebound on the computer
						  if(computer==py){				//changing direction according on where the computer hit the ball
							  direction=-1;
							  campo[py][px]=ball;
						  }else if(computer==py-1){
							  direction=-3;
							  campo[py][px]=ball;
						  }else{
							  direction=-2;
							  campo[py][px]=ball;
						  }
					  }else{
						  if(campo[py+1][px+1]=='-'){			//if the ball rebound on the lower part of the court it change the direction
							  direction=2;
							  campo[py][px]=ball;
						  }else{
							  campo[py+1][px+1]=ball;		//no rebounds so it moves the ball
							  px=px+1;
							  py=py+1;
						  }
					  }
				  }

				  //detect what the player has pressed on the touch bar to change the position of the player
				  extern CONST TSL_LinRot_T MyLinRots[];
				  tsl_user_status_t status = TSL_USER_STATUS_BUSY;
				  do {
					 HAL_Delay(10);
					 status = tsl_user_Exec();
				  }while(status==TSL_USER_STATUS_BUSY); //exit when the measurement is done
				  if(MyLinRots[0].p_Data->StateId == TSL_STATEID_DETECT){
					  if(MyLinRots[0].p_Data->Position >= 5 && MyLinRots[0].p_Data->Position < 50) //if detected the lower part of the bar
						  {
						  if(campo[gamer+2][0]==space){						//change position if it is not on the corner
								campo[gamer+2][0]='|';
								campo[gamer-1][0]=space;
								gamer++;
							 }
						  }
					   if(MyLinRots[0].p_Data->Position >= 80 && MyLinRots[0].p_Data->Position < 120)//if detected the upper part of the bar
						  {
						  if(campo[gamer-2][0]==space){						//change position if it is not on the corner
							  campo[gamer-2][0]='|';
							  campo[gamer+1][0]=space;
							  gamer=gamer-1;
							  }
						  }
				  }
				  HAL_UART_Transmit(&huart1,(const uint8_t*)&new_screen,3,100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  }
	  HAL_UART_Transmit(&huart1,(const uint8_t*)&new_screen,3,100);
	  if(game==0){
		  HAL_UART_Transmit(&huart1,(const uint8_t*)&lose,21,100);		// user lost
	  }else if(game==2){
		  HAL_UART_Transmit(&huart1,(const uint8_t*)&win,24,100);		// user victory
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TSC Initialization Function
  * @param None
  * @retval None
  */
static void MX_TSC_Init(void)
{

  /* USER CODE BEGIN TSC_Init 0 */

  /* USER CODE END TSC_Init 0 */

  /* USER CODE BEGIN TSC_Init 1 */

  /* USER CODE END TSC_Init 1 */

  /** Configure the TSC peripheral
  */
  htsc.Instance = TSC;
  htsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  htsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  htsc.Init.SpreadSpectrum = DISABLE;
  htsc.Init.SpreadSpectrumDeviation = 1;
  htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  htsc.Init.MaxCountValue = TSC_MCV_8191;
  htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  htsc.Init.MaxCountInterrupt = DISABLE;
  htsc.Init.ChannelIOs = TSC_GROUP1_IO3|TSC_GROUP2_IO3|TSC_GROUP3_IO2;
  htsc.Init.ShieldIOs = 0;
  htsc.Init.SamplingIOs = TSC_GROUP1_IO4|TSC_GROUP2_IO4|TSC_GROUP3_IO3;
  if (HAL_TSC_Init(&htsc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TSC_Init 2 */

  /* USER CODE END TSC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
