//
//  str.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/20/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include <string.h>

char **StrSplit(char *string, char split)
{
    char temp[255];
    char **array;
    int i, j;
    int splitCount;
    
    //Count the number of times we'll be splitting
    splitCount = 1;
    for (i = 0; i < strlen(string); i++)
    {
        //Skip consecutive split characters
        if (i > 0 && string[i-1] != split && string[i] == split)
            splitCount++;
    }

    //Allocate the array buffer
    array = malloc((splitCount + 1) * sizeof(char*));
    if (!array)
        return NULL;

    j = 0;
    splitCount = 0;
    for (i = 0; i < strlen(string); i++)
    {
        temp[j] = string[i];
        if (i > 0 && string[i-1] != split && string[i] == split)
        {
        }
    }
}