/*************************************************************
File name: buffer.c
* Compiler: MS Visual Studio 2019
* Author: Justin Warkus-Fortin (0409667940) , Kalen Gladu-Lauridsen (040975372)
* Course: CST 8152  Compilers, Lab Section: [014]
* Assignment: A1.
* Date: Feb 06 2021
* Professor: Paulo Sousa / Abdullah Kadri
* Purpose: This file is the main code for creating and using a Buffer; the first part of our Compiler(A1)
* Function list:
            bCreate();
            bAddCh();
            bFinish();
            bRetract();
            bSetMarkOffset();
            bGetAddChOffset();
            bGetSize();
            bGetMarkOffset();
            bRestore();
            bGetChOffset();
            bufferAddCPosition();
            bGetFlags();
            bClean();
            bFree();
            bIsFull();
            bGetMode();
            bDisplay();
            bLoad();
            bIsEmpty();
            bRewind();
            bGetIncrement();
            bGetCh();
            bGetContent();
*************************************************************/

#include "buffer.h"
/************************************************************
* Purpose:			 Creates a new buffer for a buffer structure and allocates/de-allocates memory required; returns NULL on any failure.
*  Author:			 Justin Warkus-Fortin, Kalen Gladu-Lauridsen (With help from Paulo Sousa lab classes)
*  Called functions: calloc(), malloc(), free()
*  Parameters:		 size: (short), increment: (char; casted as unsigned later on), mode: (char either 'f' 'a' 'm')
*  Return value:	 bPointer, NULL
*  Algorithm:
                     if(size = valid, set default size & inc)   Returns NULL if Buffer's size is not in range,
                     if(mode = 'f' 'a' 'm',  sets tempMode for later) Returns NULL if mode is not f, a or m,
                     Calloc() call for buffer structure; Returns NULL if Buffer structure cannot be made,
                     Malloc() call for the content of the buffer; Returns NULL if content is not allocated
                     Free() call to deallocate any memory if content is empty
                     Assigns values to the required variables and returns pointer to buffer strucutre
**************************************************************/
bPointer bCreate(short size, char increment, char mode) {
    bPointer b = NULL;
    char tempMode;

    if (size < 0 || size > MAX_SIZE) {
        return NULL;
    }
    /*If the size is 0 set the size to DEFAULT_SIZE(200) and increment to DEFAULT_INCREMENT(15)*/
    if (size == 0) {
        size = DEFAULT_SIZE;
        increment = DEFAULT_INCREMENT;
    }

    /*Checks mode, and sets tempMode + increment value to appropriate value if required*/
    switch (mode) {
    case 'f':
        tempMode = FIXMODE;
        increment = 0;
        break;
    case 'a':
        tempMode = ADDMODE;
        if ((unsigned char)increment == 0) { /* Should the increment be 0, change the mode to FIXMODE*/
            tempMode = FIXMODE;
            break;
        }
        if ((unsigned char)increment < 1 || (unsigned char)increment > 255)
            return NULL;
        break;
    case 'm':
        tempMode = MULMODE;
        if ((unsigned char)increment < 1 || (unsigned char)increment > MAXINCREMENT)
            return NULL;
        break;

    default:
        return NULL;
    }
    /*Creates a pointer to the allocated Buffer Structure */
    b = (bPointer)calloc(1, sizeof(bStructure));
    if (!b)
        return NULL;

    /*malloc for content (allocates memory of value size)*/
    b->content = (char*)malloc(size);

    /*if content is null, de-allocates any possible allocated memory*/
    if (!b->content) {
        free(b);
        return NULL;
    }

    /*Setting the values for the buffer*/
    b->mode = tempMode;
    b->size = size;
    b->increment = increment;
    b->flags = DEFAULT_FLAGS;

    return b;
}


/************************************************************
* Purpose:			 Adds character passed in from the parameter and adds it to the content of the Buffer at the postion of the return value for "addCOffset()";
                     returns NULL on any failure. Returns a pointer to the Buffer
*  Author:			 Justin Warkus-Fortin, Kalen Gladu-Lauridsen (With help from Paulo Sousa for some logic issues)
*  Called functions: bGetSize(), bGetIncrement(), realloc()
*  Parameters:		 pBuffer : constant bPointer, ch : char
*  Return value:	 bPointer, NULL
*  Algorithm:
                     Returns NULL if mode's value is FIXMODE -(0),
                     Returns NULL if the new size is out of range,
                     Returns NULL if Buffer's size is equal to the maximum size,
                     Returns NULL if newContent could not be created
**************************************************************/
bPointer bAddCh(bPointer const pBuffer, char ch) {
    int newSize = 0, availableSpace = 0, newInc = 0;
    char* newContent;
    if (!pBuffer) {
        return NULL;
    }
    /*Reseting flags's r_flag bit using cosntant RESET_R_FLAG */
    pBuffer->flags = pBuffer->flags & RESET_R_FLAG;


    if (pBuffer->addCOffset == pBuffer->size) {
        /*Changes sizes depending on which mode the buffer is in*/
        if (pBuffer->mode == FIXMODE) {
            return NULL;
        }
        if (pBuffer->mode == ADDMODE) {
            newSize = bGetSize(pBuffer) + bGetIncrement(pBuffer);
            if (newSize < 0 || newSize > MAX_VALUE) {
                return NULL;
            }
            else if (newSize == MAX_VALUE) {
                newSize = MAX_SIZE;
            }
            else {
                pBuffer->size = bGetSize(pBuffer) + (short)bGetIncrement(pBuffer);
            }
        }
        if (pBuffer->mode == MULMODE) {
            if (bGetSize(pBuffer) == MAX_SIZE) {
                return NULL;
            }
            /*Using the formula in specs page 8*/
            availableSpace = MAX_SIZE - bGetSize(pBuffer);
            (int)newInc = (int)((short)availableSpace * (float)(pBuffer->increment / 100.0));
            newSize = bGetSize(pBuffer) + newInc;
            if (newInc == 0 && newSize < MAX_SIZE) {
                newSize = MAX_SIZE;
            }
            else if (newSize > MAX_SIZE || newSize < 0) {
                return NULL;
            }
        }
        /*Reallocate content pointer to perform a bitwise check and a NULL check before changing the content to the newContent*/
        newContent = (char*)realloc(pBuffer->content, newSize);
        if (!newContent) {
            return NULL;
        }
        if (newContent != pBuffer->content) {
            pBuffer->flags = pBuffer->flags | SET_R_FLAG; /*Setting r_flag bit to 1*/

        }
        pBuffer->content = newContent;
        pBuffer->size = newSize;
    }
    /*Saving new sizes*/
    pBuffer->content[pBuffer->addCOffset] = ch;
    pBuffer->addCOffset++;
    return pBuffer;
}

/************************************************************
* Purpose:			 Re-initializes Buffer's getCOffset, markOffset, getCOffset
*  Author:			 Kalen Gladu-Lauridsen
*  Parameters:		 pBuffer : constant bPointer
*  Return value:	 RT_FAIL_1, 0
*  Algorithm:
*                    Returns RT_FAIL_1 if pBuffer is NULL
                     set addCOffset, getCOffset, markOffset to 0
                     Returns 0 otherwise


**************************************************************/
int bClean(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    /*Re-initializing data members*/
    pBuffer->getCOffset = 0;
    pBuffer->markOffset = 0;
    pBuffer->addCOffset = 0;

    return 0;
}

/************************************************************
* Purpose:	         Frees all memory of which has been allocated
                     returns RT_FAIL_1 on any failure. Returns a pointer to the Buffer
*  Author:			 Justin Warkus-Fortin
*  Called functions: free()
*  Parameters:		 pBuffer : constant bPointer
*  Return value:	 RT_FAIL_1, 1, 0
*  Algorithm:
                     Returns 1 if the character Buffer is full,
                     Returns RT_FAIL_1 on failure,
                     Returns 0 otherwise
**************************************************************/
int bFree(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    if (pBuffer->size == MAX_SIZE) {
        return 1;
    }
    /*De-allocates the buffer's content*/
    if (pBuffer->content != NULL) {
        free(pBuffer->content);
        
        pBuffer->content = NULL;
    }
    /*De-allocates the buffer's pointer*/
    if (pBuffer != NULL) free(pBuffer);
    return 0;
}

/************************************************************
* Purpose:			 Checks if the Buffer is full
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:		 pBuffer : constant bPointer
*  Return value:	 RT_FAIL_1 , 1 , 0
*  Algorithm:        Returns 1 if full,
                     Returns RT_FAIL_1 on failure,
                     Returns 0 otherwise
**************************************************************/
int bIsFull(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    if (pBuffer->addCOffset == pBuffer->size) {
        return 1;
    }
    return 0;
}

/************************************************************
* Purpose:			 Getter for Buffer's Character Offset position
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:		 pBuffer : constant bPointer
*  Return value:	 RT_FAIL_1, pBffer->addCOffset
*  Algorithm:
                     Returns addCOffset,
                     Returns RT_FAIL_1 on failure
**************************************************************/
short bGetAddChOffset(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    return pBuffer->addCOffset;
}

/************************************************************
*  Purpose:          Getter for Buffer's size
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, pBuffer->size
*  Algorithm:
                     Returns Buffer size,
                     Returns RT_FAIL_1 on failure
**************************************************************/
short bGetSize(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    return pBuffer->size;
}

/************************************************************
*  Purpose:          Getter for Buffer's mode
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, pBuffer->mode
*  Algorithm:
                     Returns RT_FAIL_1 on failure
                     Returns Buffer's mode,
**************************************************************/
int bGetMode(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    return pBuffer->mode;
}

/************************************************************
*  Purpose:          Getter for Buffer's markOffset
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     pBuffer->markOffset
*  Algorithm:
                     Returns Buffer's markOffset,
                     Returns RT_FAIL_1 on failure
**************************************************************/
short bGetMarkOffset(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    return pBuffer->markOffset;
}

/************************************************************
*  Purpose:          Sets mark to the Buffer's markOffset passed in parameters
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer, mark : short
*  Return value:     RT_FAIL_1, 0
*  Algorithm:
*                    Returns RT_FAIL_1 if mark is not within the current Buffer limit,
                     Returns RT_FAIL_1 on failure,
                     Returns 0 otherwise
**************************************************************/
short bSetMarkOffset(bPointer const pBuffer, short mark) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    mark = pBuffer->markOffset;
    /*Checks if mark is within buffer limit*/
    if (mark < 0 || mark > pBuffer->addCOffset) {
        return RT_FAIL_1;
    }
    return 0;
}

/************************************************************
*  Purpose:          Reallocates the Buffer size of content and adjusts pBuffer->flags
*  Author:			 Justin Warkus-Fortin
*  Called functions: realloc()
*  Parameters:       pBuffer : constant bPointer, ch : char
*  Return value:     NULL, pBuffer
*  Algorithm:
                     Reallocate memory for adjusted size of content
                     Adjusts flags if required
                     Returns NULL if unable to perform required operation,
                     Returns NULL on failure
**************************************************************/
bPointer bFinish(bPointer const pBuffer, char ch) {
    char* temp;
    if (pBuffer == NULL)
        return NULL;
    pBuffer->size = (pBuffer->addCOffset + 1);
    temp = (char*)realloc(pBuffer->content, pBuffer->size); /*Adjusting the new size*/
    if (temp == NULL) {
        return NULL;
    }
    if (temp != pBuffer->content) {
        pBuffer->flags |= SET_R_FLAG; /*Setting the r_flag bit*/
    }
    pBuffer->content = temp;
    pBuffer->content[pBuffer->addCOffset++] = ch; /*Adding ch to the end of the char buffer*/

    return pBuffer;
}

/************************************************************
*  Purpose:          Prints out all the contents of the Buffer
*  Author:			 Justin Warkus-Fortin
*  Called functions: bGetCh(), printf()
*  Parameters:       pBuffer : constant bPointer, nl : char
*  Return value:     RT_FAIL_1, i (number of characters)
*  Algorithm:
                     Loops through buffer printing characters until EOB
                     New Line if not nl=0


**************************************************************/
int bDisplay(bPointer const pBuffer, char nl) {
    int bool = 1;
    int i = 0; /* Number of characters */
    char ch;
    if (pBuffer == NULL)
        return RT_FAIL_1;
    while (bool) {
        ch = bGetCh(pBuffer);
        if ((pBuffer->flags & CHECK_EOB) != 0) { /*End of Buffer*/
            break;
        }
        printf("%c", ch); /*Prints each char*/
        i++;
    }

    if (nl != 0) {
        printf("\n");
    }
    return i;
}

/************************************************************
*  Purpose:          Loads a specified file into the Buffer
*  Author:			 Justin Warkus-Fortin (Lab class collab partly)
*  Called functions: fgetc(), feof(), bAddCh(), ungetc()
*  Parameters:       pBuffer : constant bPointer, fi : constant *FILE
*  Return value:     RT_FAIL_1, LOAD_FAIL, charsAdded
*  Algorithm:
                     while(not end of file) read 1 character at a time
                     if end of file break
                     if bAddCh fails, unget to put back char
**************************************************************/
int bLoad(bPointer const pBuffer, FILE* const fi) {
    char c;
    int charsAdded = 0;
    if (pBuffer == NULL || fi == NULL) {
        return RT_FAIL_1;
    }
    /*read one char at a time*/
    while (1) {
        c = (char)fgetc(fi);
        /*Checking for end of file*/
        if (feof(fi)) {
            break;
        }
        /*Checking if current character cannot be added*/
        if (!bAddCh(pBuffer, c)) {
            ungetc(c, fi);
            return LOAD_FAIL;
        }
        charsAdded++;
    }
    return charsAdded;
}

/************************************************************
*  Purpose:          Checks if the Buffer is empty
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, 0, 1
*  Algorithm:
*                    if buffer is NULL, it fails
*                    Checks addCOffset, if its 0, it returns 1 indicating empty
**************************************************************/
int bIsEmpty(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return RT_FAIL_1;
    }
    if (pBuffer->addCOffset == 0) {
        return 1;
    }
    return 0;
}


/************************************************************
*  Purpose:          Gets The character located at position getCOffset, Sets and resets flags
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, SET_EOB, RESET_EOB,  pBuffer->content[pBuffer->getCOffset++]
*  Algorithm:
                     Returns content[pBuffer->getCOffset++] -Character located at gectCOffset,
                     Returns 0 if Buffer's getCOffset and Buffer's
                     addCOffset are equal,
                     Returns RT_FAIL_1 on failure
**************************************************************/
char bGetCh(bPointer const pBuffer) {
    if (pBuffer == NULL)
        return RT_FAIL_1;
    if (pBuffer->getCOffset == pBuffer->addCOffset) {
        pBuffer->flags |= SET_EOB; /*Setting flags field's eob_flag bit to 1*/
        return 0;
    }
    else
        pBuffer->flags &= RESET_EOB; /*Setting flags field's eob_flag bit to 0*/

    return pBuffer->content[pBuffer->getCOffset++];
}


/************************************************************
*  Purpose:          Sets Buffer's getCOffset and Buffer's marOffset to 0
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, 0
*  Algorithm:
                     Returns RT_FAIL_1 on failure,
                     Sets getCOffset and MarkOffset to 0 and returns 0
**************************************************************/
int bRewind(bPointer const pBuffer) {
    if (pBuffer == NULL)
        return RT_FAIL_1;
    /*Setting getCOffset and markOffset back to 0*/
    pBuffer->getCOffset = 0;
    pBuffer->markOffset = 0;
    return 0;
}


/************************************************************
*  Purpose:          Decrements getCOffset by 1
*  Author:			 Justin Warkus-Fortin, Kalen Gladu-Lauridsen
*  Called functions:
*  Parameters:       pBuffer : constant bPointer
*  Return value:     pBuffer, NULL
*  Algorithm:
                     Returns Buffer's pointer,
                     Returns NULL on failure
**************************************************************/
bPointer bRetract(bPointer const pBuffer) {
    if (pBuffer == NULL)
        return NULL;

    pBuffer->getCOffset--; /* Goes back by 1 */

    return pBuffer;
}

/************************************************************
*  Purpose:          Resets getCOffset to markOffset's value
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, 0
*  Algorithm:
                     Returns RT_FAIL_1 on failure,
                     Returns 0 otherwise
**************************************************************/
short bRestore(bPointer const pBuffer) {
    if (pBuffer == 0)
        return RT_FAIL_1;
    pBuffer->getCOffset = pBuffer->markOffset;

    return 0;
}

/************************************************************
*  Purpose:          Getter for Character Offset
*  Author:			 Justin Warkus-Fortin, Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, pBuffer->getCOffset
*  Algorithm:
                     Returns Buffer's getCOffset,
                     Returns RT_FAIL_1 on failure
**************************************************************/
short bGetChOffset(bPointer const pBuffer) {
    if (pBuffer == NULL || pBuffer->addCOffset || pBuffer->addCOffset < 0) {
        return RT_FAIL_1;
    }
    return pBuffer->getCOffset;
}


/************************************************************
*  Purpose:          Getter for Buffer's increment
*  Author:			 Justin Warkus-Fortin
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     (size_t)RT_INC_FAIL, (unsigned char)pBuffer->increment
*  Algorithm:
                     Returns positive value of Buffer's increment casted as an unsigned char to avoid overflow and data loss
                     Returns RT_INC_FAIL of failure
**************************************************************/
size_t bGetIncrement(bPointer const pBuffer) {
    if (pBuffer == NULL) {
        return (size_t)RT_INC_FAIL;
    }
    return (unsigned char)pBuffer->increment;
}


/************************************************************
*  Purpose:          Getter for Buffer's content
*  Author:			 Justin Warkus-Fortin
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer, chPosition : short
*  Return value:     pBuffer->content + chPosition
*  Algorithm:
                     Returns Buffer's content and char's location,
                     Returns NULL on failure
**************************************************************/
char* bGetContent(bPointer const pBuffer, short chPosition) {
    if (!pBuffer)
        return NULL;

    return pBuffer->content + chPosition;
}


/************************************************************
*  Purpose:          Getter for Buffer's getCOffset
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer : constant bPointer
*  Return value:     RT_FAIL_1, pBuffer->getCOffest
*  Algorithm:
                     Returns Buffer's getCOffset,
                     Returns RT_FAIL_1 on failure,
**************************************************************/
short bufferAddCPosition(bStructure* const pBuffer) {
    if (pBuffer == NULL)
        return RT_FAIL_1;
    return pBuffer->getCOffset;
}


/************************************************************
*  Purpose:          Getter for Buffer's flags
*  Author:			 Kalen Gladu-Lauridsen
*  Called functions: N/A
*  Parameters:       pBuffer: const bPointer
*  Return value:     pBuffeer->flags, RT_FAIL_1
*  Algorithm:
                     Returns Buffer's flags,
                     Returns RT_FAIL_1 on failure
**************************************************************/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
unsigned short bGetFlags(bPointer const pBuffer) {
    if (pBuffer == NULL)
        return RT_FAIL_1;
    return pBuffer->flags;
}
#else
#define 
#endif
