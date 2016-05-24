/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */ 

/* Includes ------------------------------------------------------------------*/

#include "LLDISPLAY_configuration.h"

#include "stm32746g_discovery_lcd.h"
#include "LLDISPLAY_intern.h"
#include "LLDISPLAY_COPY_IMPL.h"
#include "LLDISPLAY_EXTRA_IMPL.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "microej.h"
#include "bsp_util.h"
#include "framerate.h"

/* Defines -------------------------------------------------------------------*/

//#define DEBUG_FRAME_BUFFER
#define	WAIT_OS_FLAG 0x10  // Warning: an os flag is used in LLMJVM_RTX.c

// declare MicroEJ Display buffer
BSP_DECLARE_BUFFER(DISPLAY_MEM)

#define BUFFER_SIZE (WIDTH*HEIGHT*LLDISPLAY_BPP/8)

#define BACK_BUFFER (DISPLAY_MEM_START)
#define FRAME_BUFFER (BACK_BUFFER + BUFFER_SIZE)
#define WORKINGBUFFER_START (FRAME_BUFFER + BUFFER_SIZE)
#define WORKINGBUFFER_END (DISPLAY_MEM_END)

#define LTDC_Layer LTDC_Layer2
 
/* Structs -------------------------------------------------------------------*/

/*
 * destination, same memory layout and BPP than LCD frame buffer
 */
typedef struct
{
	uint32_t address;
	uint16_t width;
	uint16_t x;
	uint16_t y;
} DMA2D_LayerDestinationTypeDef;

/*
 * foreground is used for copy and blend modes
 */
typedef struct
{
	uint32_t address;
	uint32_t alphaAndColor;
	uint32_t lut_address;

	uint16_t width;
	uint16_t area_x;
	uint16_t area_y;
	uint16_t area_width;
	uint16_t area_height;

	uint8_t format;
	uint8_t bpp;
	uint8_t lut_color_mode;
	uint8_t lut_size;
} DMA2D_LayerForegroundTypeDef;

/* Global --------------------------------------------------------------------*/

static DMA2D_HandleTypeDef hdma2d_eval;
static DMA2D_CLUTCfgTypeDef hdma2d_lut;
static xSemaphoreHandle dma2d_sem;
static uint8_t dma2d_running;

static DMA2D_LayerDestinationTypeDef DMA2D_LayerDestinationInitStruct;
static DMA2D_LayerForegroundTypeDef DMA2D_LayerForegroundInitStruct;

/* Private API ---------------------------------------------------------------*/

static void lcd_dma2d_wait(void)
{
	while(dma2d_running == 1)
	{
		xSemaphoreTake(dma2d_sem, portMAX_DELAY);
	}
}

static void lcd_enable_interrupt(void)
{
	// ask to reload buffer address
	// (it will throw an interrupt)
	LTDC->SRCR = LTDC_SRCR_VBR;
}

static void lcd_dma2d_wakeup(void)
{
	dma2d_running = 0;
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(dma2d_sem, &xHigherPriorityTaskWoken);
	if(xHigherPriorityTaskWoken != pdFALSE )
	{
		// Force a context switch here.
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/*
 * Fill a buffer area whose size is equal to LCD buffer size.
 */
static void lcd_fillRect(uint32_t bufferAddress, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t bufferWidth, uint32_t color)
{
	// adjust start address
	bufferAddress += (y * bufferWidth + x) * LLDISPLAY_BPP/8;

	HAL_DMA2D_DeInit(&hdma2d_eval);
	hdma2d_eval.Init.Mode         = DMA2D_R2M;
	hdma2d_eval.Init.OutputOffset = bufferWidth - width;
	HAL_DMA2D_Init(&hdma2d_eval) ;
	dma2d_running = 1;
	HAL_DMA2D_Start_IT(&hdma2d_eval, color, bufferAddress, width, height);
}

/*
 * Copy a buffer area to another buffer
 */
static void lcd_memCopy(void) 
{
	HAL_DMA2D_DeInit(&hdma2d_eval);

	uint32_t srcAddress = DMA2D_LayerForegroundInitStruct.address;
	uint32_t destAddress = DMA2D_LayerDestinationInitStruct.address;

	// adjust start addresses
	srcAddress += (DMA2D_LayerForegroundInitStruct.area_y * DMA2D_LayerForegroundInitStruct.width + DMA2D_LayerForegroundInitStruct.area_x) * LLDISPLAY_BPP/8;
	destAddress += (DMA2D_LayerDestinationInitStruct.y * DMA2D_LayerDestinationInitStruct.width + DMA2D_LayerDestinationInitStruct.x) * LLDISPLAY_BPP/8;

	// destination
	hdma2d_eval.Init.Mode         = DMA2D_M2M;
	hdma2d_eval.Init.OutputOffset = DMA2D_LayerDestinationInitStruct.width - DMA2D_LayerForegroundInitStruct.area_width;

	// foreground
	hdma2d_eval.LayerCfg[1].InputOffset = DMA2D_LayerForegroundInitStruct.width - DMA2D_LayerForegroundInitStruct.area_width;
	hdma2d_eval.LayerCfg[1].InputColorMode = LLDISPLAY_BPP_get_DMA2D_COLOR_MODE();
	HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1);

	/* DMA2D Initialization */
	HAL_DMA2D_Init(&hdma2d_eval) ;
	dma2d_running = 1;
	HAL_DMA2D_Start_IT(&hdma2d_eval, srcAddress, destAddress, DMA2D_LayerForegroundInitStruct.area_width,  DMA2D_LayerForegroundInitStruct.area_height);
}

/*
 * destination is always a buffer whose size is equal to LCD buffer size.
 */
static void lcd_blend(void)
{
	HAL_DMA2D_DeInit(&hdma2d_eval);

	uint32_t backgroundAddress = DMA2D_LayerDestinationInitStruct.address;
	uint32_t foregroundAddr = DMA2D_LayerForegroundInitStruct.address;

	// adjust start addresses
	backgroundAddress += (DMA2D_LayerDestinationInitStruct.y * DMA2D_LayerDestinationInitStruct.width + DMA2D_LayerDestinationInitStruct.x) * LLDISPLAY_BPP/8;
	foregroundAddr += (DMA2D_LayerForegroundInitStruct.area_y * DMA2D_LayerForegroundInitStruct.width + DMA2D_LayerForegroundInitStruct.area_x) * DMA2D_LayerForegroundInitStruct.bpp/8;

	// destination
	hdma2d_eval.Init.Mode         = DMA2D_M2M_BLEND;
	hdma2d_eval.Init.OutputOffset = DMA2D_LayerDestinationInitStruct.width - DMA2D_LayerForegroundInitStruct.area_width;

	// background
	hdma2d_eval.LayerCfg[0].InputOffset = DMA2D_LayerDestinationInitStruct.width - DMA2D_LayerForegroundInitStruct.area_width;
	hdma2d_eval.LayerCfg[0].InputColorMode = LLDISPLAY_BPP_get_DMA2D_COLOR_MODE();
	hdma2d_eval.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d_eval.LayerCfg[0].InputAlpha = 255;
	HAL_DMA2D_ConfigLayer(&hdma2d_eval, 0);

	// foreground
	HAL_DMA2D_DisableCLUT(&hdma2d_eval, 1);
	hdma2d_eval.LayerCfg[1].InputOffset = DMA2D_LayerForegroundInitStruct.width - DMA2D_LayerForegroundInitStruct.area_width;
	hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_LayerForegroundInitStruct.format;
	hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
	hdma2d_eval.LayerCfg[1].InputAlpha = DMA2D_LayerForegroundInitStruct.alphaAndColor;
	HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1);
	HAL_DMA2D_Init(&hdma2d_eval) ;
	if (DMA2D_LayerForegroundInitStruct.format == CM_L4 || DMA2D_LayerForegroundInitStruct.format == CM_L8)
	{
		hdma2d_lut.pCLUT = (uint32_t*)DMA2D_LayerForegroundInitStruct.lut_address;
		hdma2d_lut.CLUTColorMode = DMA2D_LayerForegroundInitStruct.lut_color_mode;
		hdma2d_lut.Size = DMA2D_LayerForegroundInitStruct.lut_size;
		HAL_DMA2D_ConfigCLUT(&hdma2d_eval, hdma2d_lut, 1);
		HAL_DMA2D_EnableCLUT(&hdma2d_eval, 1);
	}

	dma2d_running = 1;
	HAL_DMA2D_BlendingStart_IT(&hdma2d_eval, foregroundAddr, backgroundAddress, backgroundAddress, DMA2D_LayerForegroundInitStruct.area_width,  DMA2D_LayerForegroundInitStruct.area_height);
}

/*
 * Check DMA2D limitations: first and last vertical lines addresses must be aligned
 * on 8 bits.
 */
static void lcd_align_X4_data(void* drawing)
{
	uint8_t xAlign = ((LLDISPLAY_SDrawImage*)drawing)->src.x & 1;
	uint8_t wAlign = ((LLDISPLAY_SDrawImage*)drawing)->src.width & 1;

	if (xAlign == 0)
	{
		if (wAlign == 0)
		{
			// easy case: first and last vertical lines are aligned on 8 bits
		}
		else
		{
			// cannot draw last vertical line
			((LLDISPLAY_SDrawImage*)drawing)->src.width--;
		}
	}
	else
	{
		if (wAlign == 0)
		{
			// worst case: cannot draw first and last vertical lines
			((LLDISPLAY_SDrawImage*)drawing)->src.x++;
			((LLDISPLAY_SDrawImage*)drawing)->x_dest++;
			((LLDISPLAY_SDrawImage*)drawing)->src.width -= 2;
		}
		else
		{
			// cannot draw first vertical line
			((LLDISPLAY_SDrawImage*)drawing)->src.x++;
			((LLDISPLAY_SDrawImage*)drawing)->x_dest++;
			((LLDISPLAY_SDrawImage*)drawing)->src.width--;
		}
	}
}

static void lcd_prepare_A48_data(void* drawing, uint32_t* alphaAndColor)
{
	// for A4 and A8 formats, alphaAndColor is both the global alpha + wanted color
	*(alphaAndColor) <<= 24;
	*(alphaAndColor) |= (((LLDISPLAY_SDrawImage*)drawing)->color & 0xffffff);
}

static void lcd_init_destination_struct(uint32_t address, int32_t x_dest, int32_t y_dest, uint16_t width)
{
	DMA2D_LayerDestinationInitStruct.address = address;
	DMA2D_LayerDestinationInitStruct.width = width;
	DMA2D_LayerDestinationInitStruct.x = (uint32_t)x_dest;
	DMA2D_LayerDestinationInitStruct.y = (uint32_t)y_dest;
}

static void lcd_init_foreground_struct(uint32_t address, int32_t alpha, int32_t x_src, int32_t y_src, int32_t width_src, int32_t height_src, int32_t width)
{
	DMA2D_LayerForegroundInitStruct.address = address;
	DMA2D_LayerForegroundInitStruct.alphaAndColor = alpha;
	DMA2D_LayerForegroundInitStruct.area_x = (uint32_t)x_src;
	DMA2D_LayerForegroundInitStruct.area_y = (uint32_t)y_src;
	DMA2D_LayerForegroundInitStruct.area_width = (uint32_t)width_src;
	DMA2D_LayerForegroundInitStruct.area_height = (uint32_t)height_src;
	DMA2D_LayerForegroundInitStruct.width = width;
}

static void lcd_init_foreground_lut_struct(void* src, int srcAddr)
{
	DMA2D_LayerForegroundInitStruct.lut_address = srcAddr;
	DMA2D_LayerForegroundInitStruct.address = DMA2D_LayerForegroundInitStruct.lut_address;
	DMA2D_LayerForegroundInitStruct.lut_size = ((LLDISPLAY_SImageLUT*)src)->lut_size;
	if (((LLDISPLAY_SImageLUT*)src)->lut_format == LLDISPLAY_EXTRA_IMAGE_RGB888)
	{
		DMA2D_LayerForegroundInitStruct.address += (((DMA2D_LayerForegroundInitStruct.lut_size + 1) * 24) / 8);
		DMA2D_LayerForegroundInitStruct.lut_color_mode = DMA2D_CCM_RGB888;
	}
	else
	{
		DMA2D_LayerForegroundInitStruct.address += (((DMA2D_LayerForegroundInitStruct.lut_size + 1) * 32) / 8);
		DMA2D_LayerForegroundInitStruct.lut_color_mode = DMA2D_CCM_ARGB8888;
	}
}

/* Interrupt functions -------------------------------------------------------*/

void DMA2D_IRQHandler(void)
{
	HAL_DMA2D_IRQHandler(&hdma2d_eval);
	lcd_dma2d_wakeup();
}

void LTDC_IRQHandler(void)
{
	if ((LTDC->ISR & LTDC_IER_FUIE) == LTDC_IER_FUIE)
	{
		LTDC->ICR = LTDC_IER_FUIE;
		printf("[LLDISPLAY] LTDC FIFO underrun\n");
	}
	else
	{
		// clear interrupt
		LTDC->ICR = LTDC_IER_FLAG;

		// launch the copy from backbuffer to lcd buffer
		lcd_memCopy();
	}
}

/* API -----------------------------------------------------------------------*/

void LLDISPLAY_COPY_IMPL_initialize(void)
{ 	
	BSP_LCD_Init();
	BSP_LCD_LayerInit(LTDC_ACTIVE_LAYER, FRAME_BUFFER, LLDISPLAY_BPP_get_LTDC_COLOR_MODE());
	dma2d_running = 0;

	vSemaphoreCreateBinary(dma2d_sem);
	xSemaphoreTake(dma2d_sem, 0);

	// interruptions
	HAL_NVIC_SetPriority(LTDC_IRQn, 5, 3);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);
	HAL_NVIC_SetPriority(DMA2D_IRQn, 5, 3);
	HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	hdma2d_eval.Init.ColorMode    = LLDISPLAY_BPP_get_DMA2D_COLOR_MODE();
	hdma2d_eval.Instance = DMA2D;

	// enable LCD interrupt
	LTDC->IER = LTDC_IER_FLAG | LTDC_IER_FUIE;
}

/*
 * This function has to return the start address of a memory space which will
 * be used to store the working buffer. This buffer is used by the display
 * module to decode dynamically the PNG images.
 *
 * The idea is to use all available remaining SDRAM memory. The SDRAM memory
 * already contains the 3 display buffers (back buffer, LCD buffer and DMA2D buffer).
 */
int32_t LLDISPLAY_COPY_IMPL_getWorkingBufferStartAddress(void)
{
	return WORKINGBUFFER_START;
}

void LLDISPLAY_COPY_IMPL_copyBuffer(int32_t srcAddr, int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)
{
#ifdef FRAMERATE_ENABLED
	framerate_increment();
#endif
	lcd_init_foreground_struct(srcAddr, 0/*useless*/, xmin, ymin, (xmax - xmin + 1), (ymax - ymin + 1), WIDTH);
	lcd_init_destination_struct(LTDC_Layer->CFBAR, xmin, ymin, WIDTH);
	lcd_enable_interrupt();

	// force to wait the end of copy (done by the DMA2D) even if the copy has not been started. An issue can occur
	// if the display stack calls synchronize() function BEFORE the start of copy.
	dma2d_running = 1;
}

int32_t LLDISPLAY_COPY_IMPL_getWorkingBufferEndAddress(void)
{
	return WORKINGBUFFER_END;
}

int32_t LLDISPLAY_COPY_IMPL_getBackBufferAddress(void)
{ 
	return BACK_BUFFER;
}

int32_t LLDISPLAY_COPY_IMPL_getHeight(int32_t type)
{
	return HEIGHT;
}

int32_t LLDISPLAY_COPY_IMPL_getWidth(int32_t type)
{
	return WIDTH;
}

void LLDISPLAY_COPY_IMPL_synchronize(void)
{
	// wait the end of the copy after a flush
	lcd_dma2d_wait();
}

void LLDISPLAY_EXTRA_IMPL_waitPreviousDrawing(void)
{
	// wait the previous DMA2D copy or blend
	lcd_dma2d_wait();
}

int32_t LLDISPLAY_EXTRA_IMPL_fillRect(void* dest, int32_t destAddr, void* rect, int32_t color)
{
	// ensure DMA2D is free
	lcd_dma2d_wait();
	
	if ((color & 0xff000000) != 0xff000000)
	{
		// cannot draw with a transparent color (not supported by DMA2D)
		return LLDISPLAY_EXTRA_NOT_SUPPORTED;
	}

	// use DMA2D to perform the drawing
	lcd_fillRect(destAddr, ((LLDISPLAY_SRectangle*)rect)->x, ((LLDISPLAY_SRectangle*)rect)->y, ((LLDISPLAY_SRectangle*)rect)->width, ((LLDISPLAY_SRectangle*)rect)->height, ((LLDISPLAY_SImage*)dest)->width, color);
	return LLDISPLAY_EXTRA_DRAWING_RUNNING;
}

int32_t LLDISPLAY_EXTRA_IMPL_drawImage(void* src, int32_t srcAddr, void* dest, int32_t destAddr, void* drawing)
{
	// ensure DMA2D is free
	lcd_dma2d_wait();
	
	uint8_t format, bpp;
	uint32_t alphaAndColor;

	format = ((LLDISPLAY_SImage*)src)->format;

	if (format == LLDISPLAY_EXTRA_IMAGE_LCD)
	{
		format = LLDISPLAY_BPP_get_lcd_format();
	}

	// for standard formats, alphaAndColor is a value between 0x00 and 0xff (only global alpha)
	alphaAndColor = ((LLDISPLAY_SDrawImage*)drawing)->alpha;

	switch(format)
	{
	case LLDISPLAY_EXTRA_IMAGE_RGB565:
		format = CM_RGB565;
		bpp = 16;
		break;
	case LLDISPLAY_EXTRA_IMAGE_ARGB8888:
		format = CM_ARGB8888;
		bpp = 32;
		break;
	case LLDISPLAY_EXTRA_IMAGE_RGB888:
		format = CM_RGB888;
		bpp = 24;
		break;
	case LLDISPLAY_EXTRA_IMAGE_ARGB1555:
		format = CM_ARGB1555;
		bpp = 16;
		break;
	case LLDISPLAY_EXTRA_IMAGE_ARGB4444:
		format = CM_ARGB4444;
		bpp = 16;
		break;
	case LLDISPLAY_EXTRA_IMAGE_A4:
		lcd_prepare_A48_data(drawing, &alphaAndColor);
		format = CM_A4;
		bpp = 4;
		lcd_align_X4_data(drawing);
		break;
	case LLDISPLAY_EXTRA_IMAGE_A8:
		lcd_prepare_A48_data(drawing, &alphaAndColor);
		format = CM_A8;
		bpp = 8;
		break;
	case LLDISPLAY_EXTRA_IMAGE_L4:
		format = CM_L4;
		bpp = 4;
		lcd_align_X4_data(drawing);
		break;
	case LLDISPLAY_EXTRA_IMAGE_L8:
		format = CM_L8;
		bpp = 8;
		break;
	default: return LLDISPLAY_EXTRA_NOT_SUPPORTED;
	}

	// TODO: manage (LLDISPLAY_SDestination*)dest)->format (task 12597)
	lcd_init_destination_struct(destAddr, ((LLDISPLAY_SDrawImage*)drawing)->x_dest, ((LLDISPLAY_SDrawImage*)drawing)->y_dest, ((LLDISPLAY_SImage*)dest)->width);
	lcd_init_foreground_struct(srcAddr, alphaAndColor, ((LLDISPLAY_SDrawImage*)drawing)->src.x, ((LLDISPLAY_SDrawImage*)drawing)->src.y, ((LLDISPLAY_SDrawImage*)drawing)->src.width, ((LLDISPLAY_SDrawImage*)drawing)->src.height, ((LLDISPLAY_SImage*)src)->width);
	if (format == CM_L4 || format == CM_L8)
	{
		lcd_init_foreground_lut_struct(src, srcAddr);	// FIXME do it after lcd_init_foreground_struct to erase DMA2D_LayerForegroundInitStruct.address
	}
	DMA2D_LayerForegroundInitStruct.format = format;
	DMA2D_LayerForegroundInitStruct.bpp = bpp;

	lcd_blend();

	return LLDISPLAY_EXTRA_DRAWING_RUNNING;
}
