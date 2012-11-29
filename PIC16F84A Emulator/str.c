//
//  str.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/20/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include <string.h>

#if 0
char **StrSplit(char *string, char split)
{
    char **array;
    int i;
    int splitCount;
    
    //Count the number of times we'll be splitting
    splitCount = 1;
    for (i = 0; i < strlen(string); i++)
    {
        if (string[i] == split)
            splitCount++;
    }

    //Allocate the buffer
    array = malloc(splitCount * sizeof(char*));
    
    for (i = 0; i < strlen(string); i++)
    {
        if (string[i] == split)
            splitCount++;
    }
}
#endif