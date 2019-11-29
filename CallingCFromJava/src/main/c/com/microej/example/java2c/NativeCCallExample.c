/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#include <sni.h>

/**
* \brief
* \param	aValue
* \return	twice the input value, stored as a portable int
*/
jint Java_com_microej_example_java2c_NativeCCallExample_someCFunctionReturningTwiceAValue(jint aValue) {
	return aValue*2;
}
