/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\otm8009.h"
#include "modules\ov7725.h"

#include "qCopterMV_bsp.h"
/*====================================================================================================*/
/*====================================================================================================*/
void NMI_Handler( void ) { while(1); }
void HardFault_Handler( void ) { while(1); }
void MemManage_Handler( void ) { while(1); }
void BusFault_Handler( void ) { while(1); }
void UsageFault_Handler( void ) { while(1); }
void SVC_Handler( void ) {}
void DebugMon_Handler( void ) {}
void PendSV_Handler( void ) {}
void SysTick_Handler( void ) { HAL_IncTick(); }
/*====================================================================================================*/
/*====================================================================================================*/
//void WWDG_IRQHandler( void )
//void PVD_IRQHandler( void )
//void TAMP_STAMP_IRQHandler( void )
//void RTC_WKUP_IRQHandler( void )
//void FLASH_IRQHandler( void )
//void RCC_IRQHandler( void )
//void EXTI0_IRQHandler( void )
//void EXTI1_IRQHandler( void )
//void EXTI2_IRQHandler( void )
//void EXTI3_IRQHandler( void )
//void EXTI4_IRQHandler( void )
//void DMA1_Stream0_IRQHandler( void )
//void DMA1_Stream1_IRQHandler( void )
//void DMA1_Stream2_IRQHandler( void )
//void DMA1_Stream3_IRQHandler( void )
//void DMA1_Stream4_IRQHandler( void )
//void DMA1_Stream5_IRQHandler( void )
//void DMA1_Stream6_IRQHandler( void )
//void ADC_IRQHandler( void )
//void CAN1_TX_IRQHandler( void )
//void CAN1_RX0_IRQHandler( void )
//void CAN1_RX1_IRQHandler( void )
//void CAN1_SCE_IRQHandler( void )
//void EXTI9_5_IRQHandler( void )
//void TIM1_BRK_TIM9_IRQHandler( void )
//void TIM1_UP_TIM10_IRQHandler( void )
//void TIM1_TRG_COM_TIM11_IRQHandler( void )
//void TIM1_CC_IRQHandler( void )
//void TIM2_IRQHandler( void )
//void TIM3_IRQHandler( void )
//void TIM4_IRQHandler( void )
//void I2C1_EV_IRQHandler( void )
//void I2C1_ER_IRQHandler( void )
//void I2C2_EV_IRQHandler( void )
//void I2C2_ER_IRQHandler( void )
//void SPI1_IRQHandler( void )
//void SPI2_IRQHandler( void )
//void USART1_IRQHandler( void )
//void USART2_IRQHandler( void )
//void USART3_IRQHandler( void )
//void EXTI15_10_IRQHandler( void )
//void RTC_Alarm_IRQHandler( void )
//void OTG_FS_WKUP_IRQHandler( void )
//void TIM8_BRK_TIM12_IRQHandler( void )
//void TIM8_UP_TIM13_IRQHandler( void )
//void TIM8_TRG_COM_TIM14_IRQHandler( void )
//void TIM8_CC_IRQHandler( void )
//void DMA1_Stream7_IRQHandler( void )
//void FMC_IRQHandler( void )
//void SDIO_IRQHandler( void )
//void TIM5_IRQHandler( void )
//void SPI3_IRQHandler( void )
//void UART4_IRQHandler( void )
//void UART5_IRQHandler( void )
//void TIM6_DAC_IRQHandler( void )
//void TIM7_IRQHandler( void )
//void DMA2_Stream0_IRQHandler( void )
//void DMA2_Stream1_IRQHandler( void )
//void DMA2_Stream2_IRQHandler( void )
//void DMA2_Stream3_IRQHandler( void )
//void DMA2_Stream4_IRQHandler( void )
//void ETH_IRQHandler( void )
//void ETH_WKUP_IRQHandler( void )
//void CAN2_TX_IRQHandler( void )
//void CAN2_RX0_IRQHandler( void )
//void CAN2_RX1_IRQHandler( void )
//void CAN2_SCE_IRQHandler( void )
//void OTG_FS_IRQHandler( void )
//void DMA2_Stream5_IRQHandler( void )
//void DMA2_Stream6_IRQHandler( void )
//void DMA2_Stream7_IRQHandler( void )
//void USART6_IRQHandler( void )
//void I2C3_EV_IRQHandler( void )
//void I2C3_ER_IRQHandler( void )
//void OTG_HS_EP1_OUT_IRQHandler( void )
//void OTG_HS_EP1_IN_IRQHandler( void )
//void OTG_HS_WKUP_IRQHandler( void )
//void OTG_HS_IRQHandler( void )
extern uint16_t cameraImg[FRAME_W * FRAME_H];
extern __IO uint32_t fps_count;
void DCMI_IRQHandler( void )
{
  if(DCMI_GetITStatus(DCMI_IT_FRAME)!= RESET) {
    fps_count++;
    LED_B_Toggle();
    // flip
    for(uint32_t i = 0; i < (FRAME_H >> 1); i++) {
      uint16_t tmp = 0;
      uint16_t *ptr = &cameraImg[(FRAME_H - i - 1) * FRAME_W];
      // swap
      for(uint32_t j = 0; j < FRAME_W; j++) {
        tmp = cameraImg[i * FRAME_W + j];
        cameraImg[i * FRAME_W + j] = *ptr;
        *ptr = tmp;
        ptr++;
      }
    }
    LCD_SetWindow(0, 0, FRAME_W - 1, FRAME_H - 1);
    for(uint32_t i = 0; i < FRAME_W * FRAME_H; i++) {
      LCD_RAM = cameraImg[i];
    }
    LCD_SetWindow(FRAME_W, 0, FRAME_W + FRAME_W - 1, FRAME_W - 1);
    for(uint32_t i = 0; i < FRAME_W * FRAME_H; i++) {
      LCD_RAM = cameraImg[i] & (~(GREEN | BLUE));
    }
    LCD_SetWindow(0, FRAME_H, FRAME_W - 1, FRAME_H + FRAME_W - 1);
    for(uint32_t i = 0; i < FRAME_W * FRAME_H; i++) {
      LCD_RAM = cameraImg[i] & (~(RED | BLUE));
    }
    LCD_SetWindow(FRAME_W, FRAME_H, FRAME_W + FRAME_W - 1, FRAME_H + FRAME_W - 1);
    for(uint32_t i = 0; i < FRAME_W * FRAME_H; i++) {
      LCD_RAM = cameraImg[i] & (~(RED | GREEN));
    }
    DCMI_CaptureCmd(ENABLE);
    DCMI_ClearITPendingBit(DCMI_IT_FRAME);
  }
}
//void CRYP_IRQHandler( void )
//void HASH_RNG_IRQHandler( void )
//void FPU_IRQHandler( void )
//void UART7_IRQHandler( void )
//void UART8_IRQHandler( void )
//void SPI4_IRQHandler( void )
//void SPI5_IRQHandler( void )
//void SPI6_IRQHandler( void )
//void SAI1_IRQHandler( void )
//void LTDC_IRQHandler( void )
//void LTDC_ER_IRQHandler( void )
//void DMA2D_IRQHandler( void )
/*====================================================================================================*/
/*====================================================================================================*/
