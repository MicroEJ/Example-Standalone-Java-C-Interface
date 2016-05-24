/* 
 * Copyright 2013-2015 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-CIAR-ARM - 1.0.0 - DO NOT EDIT IT
 */
/* 
 * Implementation header file. Shall only be included by client implementation C files.
 */
#include <LLDISPLAY_HWLAYERS.h>
#include <stdint.h>
#include <intern\LLDISPLAY_HWLAYERS_impl.h>
#ifndef LLDISPLAY_HWLAYERS_IMPL
#warning "Implementation tag has not been defined. Default implementation tag is LLDISPLAY_HWLAYERS_IMPL"
#endif
// --------------------------------------------------------------------------------
// -                      Functions that must be implemented                      -
// --------------------------------------------------------------------------------

/**
 * Initialize the display. 
 * @return 1 when the initialization has been completed
 */
uint8_t LLDISPLAY_HWLAYERS_IMPL_initialize(LLDISPLAY_HWLAYERS* env);

/**
 * Gets the display's width in pixels.<br>
 * The LOGICAL size is the one reported to the application.<br>
 * The PHYSICAL size takes alignment constraints into account : there may be
 * unused pixels between each line of the display.<br>
 * <br>
 * It is a constraint that PHYSICAL >= LOGICAL
 * @param logical 1 for LOGICAL, 0 for PHYSICAL
 * @return the display's width in pixels. 
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getWidth(LLDISPLAY_HWLAYERS* env, uint8_t logical);

/**
 * Gets the display's height in pixels.<br>
 * The LOGICAL size is the one reported to the application.<br>
 * The PHYSICAL size takes alignment constraints into account : there may be
 * unused pixels between each line of the display.<br>
 * <br>
 * It is a constraint that PHYSICAL >= LOGICAL
 * @param logical 1 for LOGICAL, 0 for PHYSICAL
 * @return the display's height in pixels. 
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getHeight(LLDISPLAY_HWLAYERS* env, uint8_t logical);

/**
 * Converts the 32-bit ARGB color format (A-R-G-B) into the display color format.<br>
 * Note: the alpha level may be ignored if the display pixel representation
 * does not hold the alpha level information.
 * @return the converted color
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_convertRGBColorToDisplayColor(LLDISPLAY_HWLAYERS* env, int32_t color);

/**
 * Converts the display color format into a 32-bit RGB color format (A-R-G-B).<br>
 * Note: the alpha level may be assigned to 0xff (fully opaque) if the display 
 * pixel representation does not hold the alpha level information.
 * @return the converted color
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_convertDisplayColorToRGBColor(LLDISPLAY_HWLAYERS* env, int32_t color);

/**
 * Sets the new contrast.
 * @param contrast a value between 0 and 100
 */
void LLDISPLAY_HWLAYERS_IMPL_setContrast(LLDISPLAY_HWLAYERS* env, int32_t contrast);

/**
 * Gets the current contrast.
 * @return a value between 0 and 100
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getContrast(LLDISPLAY_HWLAYERS* env);

/**
 * Sets the new backlight value.
 * @param backlight a value between 0 and 100
 */
void LLDISPLAY_HWLAYERS_IMPL_setBacklight(LLDISPLAY_HWLAYERS* env, int32_t backLight);

/**
 * Returns 1 when the display module can manage the display backlight.
 */
uint8_t LLDISPLAY_HWLAYERS_IMPL_hasBackLight(LLDISPLAY_HWLAYERS* env);

/**
 * Gets the current backlight value.
 * @return a value between 0 and 100
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getBacklight(LLDISPLAY_HWLAYERS* env);

/**
 * Turns on the backlight.
 */
void LLDISPLAY_HWLAYERS_IMPL_backlightOn(LLDISPLAY_HWLAYERS* env);

/**
 * Turns off the backlight.
 */
void LLDISPLAY_HWLAYERS_IMPL_backlightOff(LLDISPLAY_HWLAYERS* env);

/**
 * Gets an adjustment to apply to font pixels that are neither fully transparent nor fully opaque.
 * The adjustment is added to each color component of the pixels, so a positive value makes the
 * pixels lighter, a negative value makes them darker.
 * @param bpp the number of bits-per-pixel of the font for which the adjustment is needed
 * @return pixel color adjustment value
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getFontAlphaAdjustment(LLDISPLAY_HWLAYERS* env, int32_t bpp);

/**
 * Returns the start address of the buffer used to store the display stack's heap.
 * The heap can contain:
 * <ul>
 * <li>the dynamic user images,</li>
 * <li>the working buffers and the decoded images of embedded image decoders (for instance the PNG decoder)</li>
 * </ul>
 * A too small value can cause OutOfMemory errors and incomplete drawings. <br>
 * Ensure the heap start address is aligned on 32 bits or ensure the CPU can manupulates unaligned memory
 * addresses. 
 * @return the working buffer start address
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getWorkingBufferStartAddress(LLDISPLAY_HWLAYERS* env);

/**
 * Returns the end address of the buffer used to store the display stack's heap.
 * @see LLDISPLAY_HWLAYERS_getWorkingBufferStartAddress
 * @return the working buffer end address
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getWorkingBufferEndAddress(LLDISPLAY_HWLAYERS* env);

/**
 * Asks if the diplay is a grayscale display or not. 
 * @return 1 when the display is not a grayscale display
 */
uint8_t LLDISPLAY_HWLAYERS_IMPL_isColor(LLDISPLAY_HWLAYERS* env);

/**
 * Returns the number of colors the display can display.<br>
 * Usually the number of colors is <code>1 << BPP - 1</code>
 * @return the number of colors the display can display.
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getNumberOfColors(LLDISPLAY_HWLAYERS* env);

/**
 * Synchronizes the display stack drawing with the display framerate.<br>
 * This method is called by the display stack before the very first drawing
 * after a flush (this function is never call without a flush before). This 
 * allows to the display driver to wait for the right moment to allow the 
 * display stack to start using the back buffer in order to avoid flickering.<br>
 * This function is optional and can do nothing.
 */
void LLDISPLAY_HWLAYERS_IMPL_synchronize(LLDISPLAY_HWLAYERS* env);

/**
 * Returns the address of the back buffer. This buffer will be used by the 
 * display stack to draw application drawings.
 * @return the buffer address
 */
int32_t LLDISPLAY_HWLAYERS_IMPL_getBackBufferAddress(LLDISPLAY_HWLAYERS* env);

/**
 * Performs a flush: the content of given square in back buffer must be
 * copied into LCD buffer. 
 */
void LLDISPLAY_HWLAYERS_IMPL_copyBuffer(LLDISPLAY_HWLAYERS* env, int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax);

/**
 * Fills the specified rectangle with the given color.
 * @param areaAddress the memory start address of the destination area
 * @param x1 the x top-left coordinate of the rectangle to be filled
 * @param y1 the y top-left coordinate of the rectangle to be filled
 * @param x2 the x bottom-right coordinate of the rectangle to be filled
 * @param y2 the x bottom-right coordinate of the rectangle to be filled
 * @param areaSize the width or the height of the destination area (according pixel memory layout) 
 * @param color color is already in LCD pixel format 
 */
void LLDISPLAY_HWLAYERS_IMPL_fillRect(LLDISPLAY_HWLAYERS* env, int32_t areaAddress, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t areaSize, int32_t color);

/**
 * Enable or disable the layers mode. When enabled the driver must not flush the content
 * of the back buffer to the LCD buffer as usual. The flush will be done later thanks the 
 * call to {@link LLDISPLAY_HWLAYERS_layersFlush}.  
 * @param enableLayersMode 1 to enable the layers mode, 0 
 * to stay in classic configuration
 */
void LLDISPLAY_HWLAYERS_IMPL_prepareFlushMode(LLDISPLAY_HWLAYERS* env, uint8_t enableLayersMode);

/**
 * Specify the background color to apply when: <ul>
 * <li>the very first layer is transparent,</li>
 * <li>if the alpha blending of the very first layer is not fully opaque (see {@link LLDISPLAY_HWLAYERS_addLayer}),</li>
 * <li>if the very first layer is smaller than the display area.</li>
 * </ul>
 * Note: the given color is already in LCD pixel format
 * @param color the color to set
 */
void LLDISPLAY_HWLAYERS_IMPL_setLayerBackgroundColor(LLDISPLAY_HWLAYERS* env, int32_t color);

/**
 * Merge the given layer with the previous one. If it is the first one the driver has to
 * conserve its attributes to be able to merge it with the next one later.
 * @param layerAddress the layer buffer address (pixel (0,0) address) 
 * @param alpha the alpha blending value (between 0 and 255)
 * @param x_src the top-left corner X coordinate of the area to copy from the layer
 * @param y_src the top-left corner Y coordinate of the area to copy from the layer
 * @param width_src the width of the area to copy from the layer
 * @param height_src the height of the area to copy from the layer
 * @param width the layer width
 * @param height the layer height
 * @param x_dest the top-left corner X coordinate of the destination area
 * @param y_dest the top-left corner Y coordinate of the destination area
 * @return 1 when the layer has been merged, 0 when
 * the driver is not able to merge it
 */
uint8_t LLDISPLAY_HWLAYERS_IMPL_addLayer(LLDISPLAY_HWLAYERS* env, int32_t layerAddress, int32_t alpha, int32_t x_src, int32_t y_src, int32_t width_src, int32_t height_src, int32_t width, int32_t height, int32_t x_dest, int32_t y_dest);

/**
 * Perform the flush of the merged layers to the LCD buffer. If no layer has been
 * merged, nothing has to be done.
 */
void LLDISPLAY_HWLAYERS_IMPL_layersFlush(LLDISPLAY_HWLAYERS* env);
