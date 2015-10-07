/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#include <sni.h>

/**
* \brief
* \param	aValue
* \return	twice the input value, stored as a portable int
*/
jint Java_com_is2t_examples_java2C_NativeCCallExample_someCFunctionReturningTwiceAValue(jint aValue) {
	return aValue*2;
}
