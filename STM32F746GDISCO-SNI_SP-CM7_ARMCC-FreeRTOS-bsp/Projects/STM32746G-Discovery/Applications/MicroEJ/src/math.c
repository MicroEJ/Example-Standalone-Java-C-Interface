
/*
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Keil uVision MicroLIB has some limitations on four math functions:
 * 	-	"scalbn" and "scalbnf" are declared both in two files (produce
 *  	a linker error: duplicated symbols).
 *  -	"nextafter" and "nextafterf" are not implemented in MicroLIB
 *  	(produce a linker error: missing symbols).
 *
 * MicroEJ EDC library points on these four functions. When Keil uVision
 * links a Java application against the MicroLIB, it links all available
 * functions (even if they are not used) before removing them from the
 * final executable file.
 *
 * If the Java application does not call these EDC Math APIs, it is useless
 * to fill these functions: keep the error message.
 *
 * If the Java application call these EDC Math APIs, the Keil project must
 * use the standard C library instead of using the MicroLIB. In addition of
 * this project configuration change, these functions must be updated to
 * call the equivalent functions in the standard library.
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>

/* Private functions ---------------------------------------------------------*/

#ifdef __MICROLIB
/*
 * Please read the file comment
 */
static int keil_microlib_error()
{
	printf("Due to limitations on Keil MicroLIB, the called function cannot work.\n");
	printf("Please implement the called function or use the standard Keil library.\n");
	while(1);
}
#endif // __MICROLIB

/* Public functions ----------------------------------------------------------*/

/*
 * Please read the file comment
 */
double microej_nextafter(double x, double y)
{
	#ifdef __MICROLIB
		return (double)keil_microlib_error();
	#else
		return nextafter(x, y);
	#endif
}

/*
 * Please read the file comment
 */
float microej_nextafterf(float x, float y)
{
	#ifdef __MICROLIB
		return (float)keil_microlib_error();
	#else
		return nextafterf(x, y);
	#endif
}

/*
 * Please read the file comment
 */
double microej_scalbn(double x, int exp)
{
	#ifdef __MICROLIB
		return (double)keil_microlib_error();
	#else
		return scalbn(x, exp);
	#endif
}

/*
 * Please read the file comment
 */
float microej_scalbnf(float x, int exp)
{
	#ifdef __MICROLIB
		return (float)keil_microlib_error();
	#else
		return scalbnf(x, exp);
	#endif
}

