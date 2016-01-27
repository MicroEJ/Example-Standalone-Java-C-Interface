/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
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
		"1.1. This is the first line",
		"1.2. This is the next line",
		"1.3. This is the final line",
		NULL
};

MSG_FILE FILE_2 = {
		"2.1. File #2 has different lines, some of which are longer",
		"2.2. But it only has two lines",
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

Messenger_data_t Messenger_data_get_next_line(int8_t senderID, int32_t* fileIndex, int32_t* lineIndex)
{
	Messenger_data_t result;
	result.sender_ID = senderID;
	result.content_Length = 0;
	memset(result.content,0,MESSENGER_DATA_MAX_STRING_LENGTH);

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
						strncpy((char*)result.content,(*currentFile)[*lineIndex],MESSENGER_DATA_MAX_STRING_LENGTH);
						result.content_Length = strlen((char*)result.content);
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

void Messenger_data_toString(const Messenger_data_t* data, char * output)
{
	if ( NULL != data )
	{
		snprintf(output,MESSENGER_DATA_MAX_STRING_LENGTH,"ID : %d {%s}",data->sender_ID, data->content);
	}
	else
	{
		snprintf(output,(unsigned)strlen(""),"");
		printf("%s error : pointer to data is NULL !\n",__PRETTY_FUNCTION__);
	}
}
