/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "messenger-data.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char* MSG_FILE[];

MSG_FILE FILE_1 = {
		"1.1. This is the first line\n",
		"1.2. This is the next line\n",
		"1.3. This is the Final line\n",
		NULL
};

MSG_FILE FILE_2 = {
		"2.1. File #2 has different lines, some of which are longer\n",
		"2.2. But it only has two lines\n",
		NULL
};

static MSG_FILE* MSG_FILE_ptrs[] =
{
	&FILE_1,
	&FILE_2,
	NULL
};
	
int32_t getFileLength( MSG_FILE file )
{
	int32_t count = 0;
	while (file[count])
	{
		count++;
	}
	return count;
}

int32_t getFilePtrsArrayLength( MSG_FILE* param_MSG_FILE_ptrs[] )
{
	int32_t count = 0;
	while (param_MSG_FILE_ptrs[count])
	{
		count++;
	}
	return count;
}

char* Messenger_data_get_next_line(int32_t* fileIndex, int32_t* lineIndex)
{
	char *result = NULL;
	if ( NULL != fileIndex && NULL != lineIndex)
	{
		if ( *fileIndex < getFilePtrsArrayLength(MSG_FILE_ptrs) )
		{
			if ( *fileIndex >= 0 )
			{
				MSG_FILE* currentFile = MSG_FILE_ptrs[*fileIndex];
				if ( *lineIndex < getFileLength(*currentFile) )
				{
					if ( *lineIndex >= 0 )
					{
						result = (*currentFile)[*lineIndex];
						*lineIndex = *lineIndex + 1;
					}
				}
				else
				{
					*lineIndex = 0;
					*fileIndex = * fileIndex + 1;
				}
			}
		}
		else
		{
			*fileIndex = 0;
		}
	}
	return result;
}

