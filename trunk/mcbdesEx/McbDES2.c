/**
 ****************************************************************************
 * <P> McbDES2.c - implementation file for implementing DES and Triple DES 
 * (without use of external libs).  
 * The DES mode of operation is Electronic Code Book (ECB) or Chain Block 
 * Coding (CBC) - Cipher Feedback (CFB) are not supported. 
 *
 * I have attempted to write this source to the ANSI standard so that it can 
 * be easily ported to other environments (such as C370).  Note that this
 * implementation of DES is not particularly efficient - the Baltimore stuff
 * on NT is at least 10 times as fast, however as this has been written 
 * mainly for portability, I do not have the luxury of adding a lot of
 * optimisations.  This code could however be optimised, even portably, 
 * although not with my grasp of maths.
 * 
 * For an detailed description on how DES works, please see the following
 * document: http://www.aci.net/kalliste/des.htm.  
 * All of the code within this module is based on this document.</P>
 *
 * @version     V1.0
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	8th June      	2000	 - 	(V1.0) Creation (MCB)
 *  21st January    2003     -  (V2.0) Modification (MCB).  
 *                              Added Chain Block Coding (CBC) for better
 *                              encryption.
 ****************************************************************************
 */

/*
 ****************************************************************************
 * Include all necessary include files
 ****************************************************************************
 */
#include "McbDES2.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
 ****************************************************************************
 * Macros for manipulating bits
 ****************************************************************************
 */
#define McbGETBIT(lpBytes, nBitzb)                                           \
    ((lpBytes)[(nBitzb)/8] & (1 << (7 - ((nBitzb) % 8))))
#define McbSETBIT(lpBytes, nBitzb)                                           \
    ((lpBytes)[(nBitzb)/8] |= (1 << (7 - ((nBitzb) % 8))))
#define McbCLEARBIT(lpBytes, nBitzb)                                         \
    ((lpBytes)[(nBitzb)/8] &= ~(1 << (7 - ((nBitzb) % 8))))

#define McbRotate26BitsLeft2(lIn, lOut)                                      \
    (lOut) = ((lIn) << 2) | (((lIn) & 0x0C000000) >> 26);                                   

#define McbRotate26BitsLeft1(lIn, lOut)                                      \
    (lOut) = ((lIn) << 1) | (((lIn) & 0x08000000) >> 27);

#define McbGetKeyPair(lpBytes, lC, lD)                                       \
	lpBytes[0] = ((lC) & 0xFF00000) >> 20;                                  \
    lpBytes[1] = ((lC) & 0x00FF000) >> 12;                                  \
    lpBytes[2] = ((lC) & 0x0000FF0) >> 4;                                   \
    lpBytes[3] = (((lC) & 0x000000F) << 4) | (((lD) & 0xF000000) >> 24);    \
    lpBytes[4] = ((lD) & 0x0FF0000) >> 16;                                  \
    lpBytes[5] = ((lD) & 0x000FF00) >> 8;                                   \
    lpBytes[6] = ((lD) & 0x00000FF);

   
/*
 ****************************************************************************
 * define to trace bits of operations
 ****************************************************************************
 */
#ifdef McbDEBUGBITS 

/**
 ****************************************************************************
 * <P> Output the bits </P>
 *
 * @methodName  McbTraceBits
 *
 * @param       char * lpBytes		
 * @param       cbBytes		
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	24th May       	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbTraceBits(const char * lpBytes, int cbBits)
{
    int n;

    for (n = 0; n<cbBits; n++)
    {
        if (McbGETBIT(lpBytes, n))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
}/* McbTraceBits */

#endif /*McbDEBUGBITS*/

/**
 ****************************************************************************
 * <P> For a given in buffer, out buffer and table this function maps bits 
 * which are true given that the entries in the table show the new 
 * arrangement of the bits from their initial order.</P>
 *
 * @methodName  McbMapTrueBits
 *
 * @param       unsigned char * lpMap		
 * @param       int cbMap		
 * @param       unsigned char * lpIn		
 * @param       unsigned char * const lpOut		
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	24th May       	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbMapTrueBits(const unsigned char * lpMap, unsigned int cbMap, 
                   const unsigned char * lpIn, unsigned char * const lpOut)
{
    unsigned char chBitIndex;

   /*
    *************************************************************************
    * translate bits according to table
    *************************************************************************
    */
	for (chBitIndex=0; chBitIndex<cbMap; chBitIndex++)
    {
        if (McbGETBIT(lpIn, lpMap[chBitIndex]))
        {
            McbSETBIT(lpOut, chBitIndex);
        }
    }
}/* McbMapTrueBits */

/**
 ****************************************************************************
 * <P> For a given key, create the permuted K+ based on table PC1. </P>
 *
 * @methodName  McbCreateKeyPlus
 *
 * @param       unsigned char Key[8]		
 * @param       char KeyPlus[7]		
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	24th May       	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbCreateKeyPlus(const unsigned char Key[8], unsigned char KeyPlus[7])
{    
    static unsigned char tblPC1[] =
    {
        56, 48, 40, 32, 24, 16,  8,
         0, 57, 49, 41, 33, 25, 17, 
         9,  1, 58, 50, 42, 34, 26, 
        18, 10,  2, 59, 51, 43, 35, 
        62, 54, 46, 38, 30, 22, 14, 
         6, 61, 53, 45, 37, 29, 21, 
        13,  5, 60, 52, 44, 36, 28, 
        20, 12,  4, 27, 19, 11,  3
    };

    memset(KeyPlus, 0, 7);
    McbMapTrueBits(tblPC1, sizeof(tblPC1), Key, KeyPlus);

}/* McbCreateKeyPlus */

/**
 ****************************************************************************
 * <P> Create the Initial permuation from the message. </P>
 *
 * @methodName  McbCreateInitialPermutation
 *
 * @param       unsigned char Message[8]		
 * @param       char IP[8]		
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	7th June      	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbCreateInitialPermutation(const unsigned char Message[8], 
                                unsigned char IP[8])
{
    static unsigned char tblIP[] =
    {
        57, 49, 41, 33, 25, 17,  9,  1, 
        59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5,
        63, 55, 47, 39, 31, 23, 15,  7,
        56, 48, 40, 32, 24, 16,  8,  0,
        58, 50, 42, 34, 26, 18, 10,  2,
        60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6
    };

    memset(IP, 0, 8);
    McbMapTrueBits(tblIP, sizeof(tblIP), Message, IP);

}/* McbCreateInitialPermutation */

/**
 ****************************************************************************
 * <P> Create array of 16 subkeys from the key.</P>
 *
 * @methodName  McbCreateSubKeys
 *
 * @param       unsigned char KeyPlus[7]		
 * @param       unsigned char SubKeys[16][6]
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	7th June      	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbCreateSubKeys(const unsigned char Key[8], 
                     unsigned char SubKeys[16][6])
{
   /*
    *************************************************************************
    * table which dicates the number of left shifts required for the pairing
    *************************************************************************
    */
	static unsigned char lShifts[] = 
    {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
    };

   /*
    *************************************************************************
    * table used to permutate 
    *************************************************************************
    */
	static unsigned char tblPC2[] =
    {
        13, 16, 10, 23,  0,  4, 
         2, 27, 14,  5, 20,  9, 
        22, 18, 11,  3, 25,  7, 
        15,  6, 26, 19, 12,  1, 
        40, 51, 30, 36, 46, 54, 
        29, 39, 50, 44, 32, 47, 
        43, 48, 38, 55, 33, 52, 
        45, 41, 49, 35, 28, 31
    };

    long lPair[17][2];
    int n;
    unsigned char bytePair[7];
    unsigned char KeyPlus[7];

   /*
    *************************************************************************
    * create permutated key (K+)
    *************************************************************************
    */
	McbCreateKeyPlus(Key, KeyPlus);

   /*
    *************************************************************************
    * Obtain first pair in long format (C0 in [0][0], D0 in [0][1])
    *************************************************************************
    */
	lPair[0][0] = (KeyPlus[0] << 20) | (KeyPlus[1] << 12) | 
        (KeyPlus[2] << 4) | ((KeyPlus[3] & 0xF0) >> 4);

    lPair[0][1] = ((KeyPlus[3] & 0x0F) << 24) | (KeyPlus[4] << 16) | 
        (KeyPlus[5] << 8) | KeyPlus[6] ;

    
   /*
    *************************************************************************
    * Create pairings (Cx, Dx) by left shifting bits from the previous pair
    * by either one or two shifts (depending on lShifts table).
    *************************************************************************
    */
	for (n = 1; n < 17; n++)
    {
        if (lShifts[n-1] == 2)
        {
            McbRotate26BitsLeft2(lPair[n-1][0], lPair[n][0]);
            McbRotate26BitsLeft2(lPair[n-1][1], lPair[n][1]);
        }
        else
        {
            McbRotate26BitsLeft1(lPair[n-1][0], lPair[n][0]);
            McbRotate26BitsLeft1(lPair[n-1][1], lPair[n][1]);
        }
    }
   

#ifdef McbDEBUGBITS 
   /*
    *************************************************************************
    * Translate long variation of pair into byte bit field and debug bit 
    * fields
    *************************************************************************
    */
    printf("\n");
    for (n=0; n < 17; n++)
    {
        McbGetKeyPair(bytePair, lPair[n][0], lPair[n][1]);
  
        printf("P %2d: ", n);
        McbTraceBits(bytePair, 56);
        printf("\n");
    }
#endif /*McbDEBUGBITS*/

   /*
    *************************************************************************
    * clear the subkeys array
    *************************************************************************
    */
	memset(SubKeys, 0, 16*6);
    
   /*
    *************************************************************************
    * create the subkeys array using the pairing permutated with table PC2
    *************************************************************************
    */
    for (n=0; n < 16; n++)
    {
       /*
        *********************************************************************
        * convert long pair into byte pair
        *********************************************************************
        */
		McbGetKeyPair(bytePair, lPair[n+1][0], lPair[n+1][1]);

       /*
        *********************************************************************
        * permutate the bits from the byte pair into the subkey
        *********************************************************************
        */
		McbMapTrueBits(tblPC2, sizeof(tblPC2), bytePair, SubKeys[n]);
    }
    
}/* McbCreateSubKeys */

/**
 ****************************************************************************
 * <P> Obtain nibble from the SBox table. </P>
 *
 * @methodName  char McbGetSBoxNibble
 *
 * @param       char tblSBox[8][4][16]		
 * @param       char sbox		
 * @param       int idxSBox		
 *
 * @return      unsigned
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	8th June      	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
unsigned char McbGetSBoxNibble(unsigned char sbox, unsigned int idxSBox)
{
   /*
    *************************************************************************
    * SBox used for to convert the 48 bits into the 32 bit output.  The 48 
    * bits are used as 8 bunches of 6 bits.  The first index used for the 
    * table is dictated by the number of the 6bit bunch (ie the first 6bits 
    * refer to tblSBox[0], the last refers to tblSBox[7]).  Then the msb and 
    * lsb are combined together to obtain the row within the SBox.  Bits
    * 2 to 5 are used to obtain the column within the sbox.
    * Eg.
    * If the FIRST 6 bytes are 010001 then the reference to the SBox will 
    * be: tblSBox[0][1][8]
    *             |  |  |
    *          FIRST |  |
    *                |  |
    *    MSB and LSB 01 |
    *                   |
    *      Bits 2 to 5 1000 (8)
    *************************************************************************
    */
	static unsigned char tblSBox[8][4][16] = 
    {
        {
            {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
            {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
            {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
            {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
        },

        { 
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
            {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
            {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
            {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
        },

        {
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
            {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
            {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
            {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
        },

        {
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
            {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
            {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
            {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
        },

        {
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
            {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
            {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
            {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
        },

        {
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
            {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
            {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
            {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
        },

        {
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
            {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
            {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
            {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
        },

        {
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
            {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
            {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
            {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
        }
    };

    unsigned char row, col, nibble;

    row = ((sbox & 0x20) >> 4) | (sbox & 0x01);
    col = ((sbox & 0x1E) >> 1);

    nibble = tblSBox[idxSBox][row][col];

    return nibble;

}/* char McbGetSBoxNibble */

/**
 ****************************************************************************
 * <P> Function for encrypting each RIGHT half of each block of encryption 
 * data by expanding the in data from 32 bits to 48; exclusively ORing (XOR)
 * these 48 bits with the Subkey, passing this data through the SBOX to 
 * return back to 32 bits and applying a final table permutation. </P>
 *
 * @methodName  McbMapThroughSBox
 *
 * @param       unsigned char Right[4]		
 * @param       unsigned char SubKey[6]		
 * @param       char output32[4]		
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	8th June      	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbMapThroughSBox(const unsigned char Right[4], 
                      const unsigned char SubKey[6], 
                      unsigned char output32[4])
{
   /*
    *************************************************************************
    * selection table used to expand 32 bits to 48 prior to XORing with the
    * subkey.
    *************************************************************************
    */
	static unsigned char tblEBitSelection[48] =
    {
        31,  0,  1,  2,  3,  4,
         3,  4,  5,  6,  7,  8,
         7,  8,  9, 10, 11, 12,
        11, 12, 13, 14, 15, 16,
        15, 16, 17, 18, 19, 20, 
        19, 20, 21, 22, 23, 24, 
        23, 24, 25, 26, 27, 28,
        27, 28, 29, 30, 31,  0
    };

   /*
    *************************************************************************
    * permutation table applied after running through the sbox
    *************************************************************************
    */
	static unsigned char tblfP[32] = 
    {
        15,  6, 19, 20, 
        28, 11, 27, 16, 
         0, 14, 22, 25, 
         4, 17, 30,  9, 
         1,  7, 23, 13, 
        31, 26,  2,  8, 
        18, 12, 29,  5, 
        21, 10,  3, 24
    };

    unsigned char output48[6];    
    unsigned char sbox32[4];
    unsigned char nSboxIndex;
    int n;

   /*
    *************************************************************************
    * clear the 32 bits 
    *************************************************************************
    */
	memset(sbox32, 0, 4);

   /*
    *************************************************************************
    * create E(n) via using Ebit selection permutation table (effectively
    * creating 48 bits from 32)
    *************************************************************************
    */
	memset(output48, 0, 6);
    McbMapTrueBits(tblEBitSelection, sizeof(tblEBitSelection), Right, 
        output48);

   /*
    *************************************************************************
    * Exclusively or (XOR) 48 bit output with the subkey
    *************************************************************************
    */
	for(n = 0; n < 6; n++)
    {
        output48[n] ^= SubKey[n];
    }

   /*
    *************************************************************************
    * Obtain each six bits in turn (eight on total) to create the 32b output.
    * Each six bit set represents a location in the sbox table.  This is 
    * defined as the first and last bit for the row and the middle for bits 
    * comprise the column.  These six bits are then used to obtain a nibble
    * which are catenated into the resulting 32 bit output buffer.
    *************************************************************************
    */
    nSboxIndex = (output48[0] & 0xFC) >> 2;
    sbox32[0] |= McbGetSBoxNibble(nSboxIndex, 0) << 4;        
    
    nSboxIndex = ((output48[0] & 0x03) << 4) | ((output48[1] & 0xFC) >> 4);
    sbox32[0] |= McbGetSBoxNibble(nSboxIndex, 1);

    nSboxIndex = ((output48[1] & 0x0F) << 2) | ((output48[2] & 0xC0) >> 6);
    sbox32[1] |= McbGetSBoxNibble(nSboxIndex, 2) << 4;    

    nSboxIndex = (output48[2] & 0x3F);
    sbox32[1] |= McbGetSBoxNibble(nSboxIndex, 3);

    nSboxIndex = (output48[3] & 0xFC) >> 2;
    sbox32[2] |= McbGetSBoxNibble(nSboxIndex, 4) << 4;

    nSboxIndex = ((output48[3] & 0x03) << 4) | ((output48[4] & 0xFC) >> 4);
    sbox32[2] |= McbGetSBoxNibble(nSboxIndex, 5);

    nSboxIndex = ((output48[4] & 0x0F) << 2) | ((output48[5] & 0xC0) >> 6);
    sbox32[3] |= McbGetSBoxNibble(nSboxIndex, 6) << 4;

    nSboxIndex = (output48[5] & 0x3F);
    sbox32[3] |= McbGetSBoxNibble(nSboxIndex, 7);

   /*
    *************************************************************************
    * Apply the permutation to the sbox to obtain the output
    *************************************************************************
    */
	memset(output32, 0, 4);
    McbMapTrueBits(tblfP, sizeof(tblfP), sbox32, output32);

#ifdef McbDEBUGBITS 
    printf("O32 : ");
    McbTraceBits(output32, 32);
    printf("\n");    
#endif /*McbDEBUGBITS*/

}/* McbMapThroughSBox */
   
/**
 ****************************************************************************
 * <P> Encode 64 bits of data. </P>
 *
 * @methodName  McbEncode64Bits
 *
 * @param       unsigned char in[8]		        - data to be encrypted
 * @param       char out[8]		                - output buffer
 * @param       unsigned char SubKeys[16][6]	- the 16, 48 bit subkeys
 * @param       nEncrypt		                - specifies whether to
 *                                                encrypt (nonzero) or 
 *                                                decrypt(0)
 *
 * @return      void
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	8th June      	2000	 - 	(V1.0) Creation (MCB)
 ****************************************************************************
 */
void McbEncode64Bits(const unsigned char in[8], unsigned char out[8], 
                    const unsigned char SubKeys[16][6], int nEncrypt)
{    
   /*
    *************************************************************************
    * Table used for applying final permutation to the 64 bit block of data
    *************************************************************************
    */
	static unsigned char tblIPmin1[64] = 
    {                             
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41,  9, 49, 17, 57, 25,
        32, 0, 40,  8, 48, 16, 56, 24
    };

   /*
    *************************************************************************
    * table use to create the initial permutation
    *************************************************************************
    */
	static unsigned char tblIP[] =
    {
        57, 49, 41, 33, 25, 17,  9,  1, 
        59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5,
        63, 55, 47, 39, 31, 23, 15,  7,
        56, 48, 40, 32, 24, 16,  8,  0,
        58, 50, 42, 34, 26, 18, 10,  2,
        60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6
    };


   /*
    *************************************************************************
    * buffers
    *************************************************************************
    */
	unsigned char data[17][8];
    unsigned char output32[4];   
    unsigned char swapped64[8];    
    int n, n2;

   /*
    *************************************************************************
    * create the initial permutation from the message
    *************************************************************************
    */
    memset(data, 0, 8);
    McbMapTrueBits(tblIP, sizeof(tblIP), in, data[0]);
    
   /*
    *************************************************************************
    * Encryption
    *************************************************************************
    */
	if (nEncrypt != 0)
    {
        for(n = 1; n < 17; n++)
        {        
           /*
            *****************************************************************
            * Ln = R(n-1)
            *****************************************************************
            */
            *(unsigned long*)(data[n]) = *(unsigned long*)(&(data[n-1][4]));

           /*
            *****************************************************************
            * Call function to pass key through the sbox f(R(n-1), K(n))
            *****************************************************************
            */
            McbMapThroughSBox(&(data[n-1][4]), SubKeys[n-1], output32);

           /*
            *****************************************************************
            * Rn = L(n-1) ^ f(R(n-1), K(n))
            *****************************************************************
            */
            for(n2 = 0; n2 < 4; n2++)
            {
                data[n][n2+4] = data[n-1][n2] ^ output32[n2];
            }          
        }
    }
   /*
    *************************************************************************
    * Decryption
    *************************************************************************
    */
    else 
    {
        for(n = 1; n < 17; n++)
        {        
           /*
            *****************************************************************
            * Ln = R(n-1)
            *****************************************************************
            */
            *(unsigned long*)(data[n]) = *(unsigned long*)(&(data[n-1][4]));

           /*
            *****************************************************************
            * Call function to pass key through the sbox f(R(n-1), K(16-n))
            *****************************************************************
            */
            McbMapThroughSBox(&(data[n-1][4]), SubKeys[16-n], output32);

           /*
            *****************************************************************
            * Rn = L(n-1) ^ f(R(n-1), K(n))
            *****************************************************************
            */
            for(n2 = 0; n2 < 4; n2++)
            {
                data[n][n2+4] = data[n-1][n2] ^ output32[n2];
            }          
        }
   		
    }/* end else */

   /*
    *************************************************************************
    * reverse order of bits in the last 64 bit block L(16)R(16) -> R(16)L(16)
    *************************************************************************
    */
	for(n = 0; n < 4; n++)
    {
        swapped64[n] = data[16][n+4];
        swapped64[n+4] = data[16][n];
    }

   /*
    *************************************************************************
    * Apply the FINAL permutation to obtain the output
    *************************************************************************
    */
	memset(out, 0, 8);
    McbMapTrueBits(tblIPmin1, sizeof(tblIPmin1), swapped64, out);

#ifdef McbDEBUGBITS 
    printf("R   : ");
    McbTraceBits(out, 64);
    printf("\n");    
#endif /*McbDEBUGBITS*/

}/* McbEncode64Bits */

/**
 ****************************************************************************
 * <P> Performs single/triple DES encryption/decryption on a given buffer 
 * and places the results in an out buffer.  Triple DES is just DES done 
 * two times with two keys in a particular order. 
 *
 * If padding is enabled (nPadding != 0) then the PKCS#5 standard will be
 * used for padding.  When data is padded in this way, the EXACT size of the
 * original data will be obtained when the cryptogram is decrypted rather 
 * than returning a multiple of eight - the only shortcoming with using this
 * padding method is that an extra eight bytes will be added to the 
 * cryptogram when encrypted if the size of the original data ends is a 
 * multiple of eight.  For further details see the PKCS#5 comments near the 
 * start of this function. </P>
 *
 * @methodName  McbDoDES
 *
 * @param       char * lpIn	    - (in) pointer to the buffer to encrypt/
 *                                decrypt.
 * @param       long cbIn		- (in) number of bytes in the in buffer
 * @param       char Key1[8]	- (in) the first eight byte key to encrypt or 
 *                                decrypt with.
 * @param       char Key2[8]	- (in) the second eight byte key to encrypt
 *                                or decrypt with - this is will only be used
 *                                if the number of times to encrypt (nEncNo)
 *                                is greater than one.
 * @param       char * lpOut	- (out) pointer to the output buffer - if 
 *                                this is NULL then the no encryption/
 *                                decryption will take place but the required
 *                                length of the out buffer will be 
 *                                calculated.
 * @param       long * pcbOut	- (in/out) pointer to the number of bytes in 
 *                                the output buffer.  
 *                                On entry, this parameter is used to ensure
 *                                the output buffer is large enough to hold
 *                                the results of the encryption/decryption
 *                                On exit, this parameter is populated with 
 *                                the number of bytes required for the output 
 *                                buffer.
 * @param       int nEncrypt	- (in) non zero to encrypt, zero to decrypt.
 * @param       int nPadding    - (in) non zero for PKCS#5 padding, zero for
 *                                no padding (see above).
 * @param       int nEncNo      - (in) number of times to perform the 
 *                                operation (ie 1 for single des, 3 for 
 *                                triple des).
 * @param       int nCBC        - (in) non zero if Chain Block Cipher (CBC) 
 *                                mode is required, otherwise 0 for 
 *                                Electronic Code Book (ECB).
 *
 * @return      int             - 1 on success, 0 on failure.  Failure will 
 *                                occur if:
 *                                the supplied buffer (lpOut) is not large 
 *                                enough to hold the result of the operation,
 *                                the in buffer pointer or out buffer pointer 
 *                                are NULL.
 *
 * @exception   none
 *
 * @author      Martyn C Brown
 *
 * @changeHistory  
 *	8th June      	2000	    - (V1.0) Creation (MCB)
 *  21st January    2003        - (V2.0) Modification (MCB).  
 *                                Added Chain Block Coding (CBC) for better
 *                                encryption.
 ****************************************************************************
 */
int McbDoDES2(unsigned char * lpIn, unsigned long cbIn, unsigned char Key1[8], 
    unsigned char Key2[8], unsigned char * lpOut, unsigned long * pcbOut, 
    int nEncrypt, int nPadding, int nEncNo, int nCBC)
{        
    unsigned char bBuffTooSmall;
    unsigned char SubKeys[2][16][6]; 
    unsigned char in[8], pad, bIsPadded;
    unsigned long cbBlock, cbOut, cbCrypt, n;
    long cbRemaining;
    int nEncryptMode;
    unsigned char *lpTmp1, *lpTmp2;
    
   /*
    *************************************************************************
    * CS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDIN
    * KCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDI
    * PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADD
    *  PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PADDING PKCS#5 PAD
    *************************************************************************
    *
    * Padding works by using spare/extra byte(s) to determine the actual 
    * length of the original message.  
    *
    * On encryption when padding is enabled, 1 to 8 bytes are used with the 
    * cryptogram (depending on the length of the original message) before 
    * encryption.  Each of these byte(s) are set to the total number of extra 
    * bytes used for padding..
    *
    * Examples:
    * Where each digit represents a byte, M represents the message before
    * encryption, other digits represent the value of that particular byte.
    *
    * Data: M M M M M M M M M M M M M M M M 8 8 8 8 8 8 8 8
    * Meaning: If the original message was a multiple of 8 bytes long then 
    * before encryption an extra 8 bytes would be ADDED with the value of 8.
    *
    * Data: M M M M M M M 1
    * Meaning If the original message was a multiple of 7 bytes long then 
    * before encryption the last byte would be set to the value of 1.
    *
    * Data: M M M M M M M M M M M M M M M M M M M M 4 4 4 4
    * If the original message was a multiple of 4 bytes long then before 
    * encryption the four last bytes would be set to the value of 4.
    *
    *
    * On decryption if padding is enabled then when the cryptogram is 
    * decrypted, the last byte(s) are checked to see if they are correctly
    * padded (as to the above standard).  If they are then the value of the 
    * last byte can be removed from the length of the decrypted message.
    *
    * Examples:
    * Where each digit represents a byte, D is the message after decryption,
    * other bytes represent the value of that particular byte.
    * 
    * Data: D D D D D D D 1
    * Length: 7
    * Meaning: If the last byte is 1 then 1 will be subtracted from the total 
    * length of the returned message.
    *
    * Data: D D D D D D D D D D D D D 3 3 3
    * Length: 13
    * Meaning: If the last byte was 3 then the last three bytes will be 
    * checked to ensure they are ALL 3 - if they are then cryptogram is is 
    * correctly padded and 3 is subtracted from the length of the returned 
    * message.
    *************************************************************************
    */

   /*
    *************************************************************************
    * Calculate the size of the out buffer based on the encryption method and
    * whether padding is used.
    *************************************************************************
    */
    if (nEncrypt)
    {
        if (nCBC)
        {
            cbOut = McbCRYPTSIZE(nPadding, 1, cbIn);
            cbCrypt = cbOut - 8;
        }
        else
        {
            cbOut = McbCRYPTSIZE(nPadding, 0, cbIn);
            cbCrypt = cbOut;
        }
    }
    else
    {        
        cbOut = McbTEXTSIZEFROMCRYPT(nPadding, nCBC, cbIn);
        cbCrypt = cbOut;
    }

   /*
    *************************************************************************
    * ensure the out buffer is large enough for the operation
    *************************************************************************
    */
	bBuffTooSmall = *pcbOut < cbOut;
    *pcbOut = cbOut;    

   /*
    *************************************************************************
    * validate the parameters
    *************************************************************************
    */
	if (!lpOut || bBuffTooSmall || !lpIn)
    {
        return 0;
    }

   /*
    *************************************************************************
    * create the 16 subkeys for each key
    *************************************************************************
    */
	McbCreateSubKeys(Key1, SubKeys[0]);

    if (nEncNo > 1)
    {
        McbCreateSubKeys(Key2, SubKeys[1]);
    }

   /*
    *************************************************************************
    * Create the initialisation vector (IV) used for the first block.
    *
    * See: http://sun.soci.niu.edu/~rslade/secgloss.htm
    * "...
    * initialization vector (IV) a sequence of random bytes appended to the 
    * front of the plaintext before encryption by a block cipher, or used as 
    * a part of the first step in a block cipher procedure that uses some 
    * form of chaining
    * ..."
    *************************************************************************
    */
	if (nCBC) 
    {
        if (nEncrypt)
        {
           /*
            *****************************************************************
            * Create the random part of the IV.
            *****************************************************************
            */
			for (n = 0; n < 8; n++)
            {
                in[n] = (rand() >> 4) % 256;
            }   

           /*
            *****************************************************************
            * Encrypt the random values to get the IV.
            *****************************************************************
            */
            nEncryptMode = nEncrypt;

            for(n = 0; n < (unsigned int)nEncNo; n++)
            {
               /*
                *************************************************************
                * encode/decode the 64 bit block with the key
                *************************************************************
                */
                McbEncode64Bits(in, lpOut, 
                    (const unsigned char (*)[6])SubKeys[n % 2 == 0 ? 0 : 1], 
                    nEncryptMode);

               /*
                *************************************************************
                * swap the encryption mode
                *************************************************************
                */
                nEncryptMode = !nEncryptMode;
            }

           /*
            *****************************************************************
            * Point to the next block
            *****************************************************************
            */
            lpOut += 8;
        }
       /*
        *********************************************************************
        * If we are decrypting.
        *********************************************************************
        */
        else 
        {
           /*
            *****************************************************************
            * Point past the IV
            *****************************************************************
            */
			lpIn += 8;
            cbIn -= 8;
        }
    }

   /*
    *************************************************************************
    * encrypt/decrypt each 64 bit block 
    *************************************************************************
    */
	cbBlock = 0;
    cbRemaining = cbIn;
    for(cbBlock = 0; cbBlock < cbCrypt; cbBlock += 8)
    {
       /*
        *********************************************************************
        * Take a copy of the message to encrypt/decrypt
        *********************************************************************
        */

       /*
        *********************************************************************
        * If we are encrypting with padding then the last iteration of this 
        * loop may end on an exact eight bytes boundary of the in block.  If 
        * this occurs then we need to populate an extra byte for padding to
        * the PKCS#5 standard (see above)
        *********************************************************************
        */
		if (cbRemaining == 0)
        {
            memset(in, 8, 8);            
        }
       /*
        *********************************************************************
        * If we have less than eight bytes remaining on the in buffer then
        * popluate the remaining characters with bytes as according to the
        * PKCS#5 standard.
        *********************************************************************
        */
		else if (cbRemaining < 8)
        {
            memset(in+cbRemaining, 8-cbRemaining, 8-cbRemaining);
            memcpy(in, lpIn+cbBlock, cbRemaining);
        }
        else
        {
            memcpy(in, lpIn+cbBlock, 8);
        }

       /*
        *********************************************************************
        * For encryption, Chain Block Cipher (CBC) works like this:
        * Where 'C' is cryptogram and 'M' is unencrypted message.
        * 1.  Create a random block (Initialisation Vector - IV) seed with 
        *     key to get C 0->7.
        * 2.  XOR C 0->7 with M 0->7, then encrypt to get C 8->15.
        * 3.  XOR C 8->15 with M 8->15, then encrypt to get C 16->23.     
        * n.  XOR C n->n+7 with M n+8->n+15, then encrypt to get C n+8->n+15.
        *********************************************************************
        */
		if (nCBC && nEncrypt)
        {
            lpTmp1 = lpOut+cbBlock-8;            

            for (n = 0; n < 8; n++)
            {
                in[n] ^= lpTmp1[n];
            }
        }

       /*
        *********************************************************************
        * perform the encryption/decryption the specified number of times
        *********************************************************************
        */
        nEncryptMode = nEncrypt;

		for(n = 0; n < (unsigned int)nEncNo; n++)
        {
           /*
            *****************************************************************
            * encode/decode the 64 bit block with the key
            *****************************************************************
            */
            McbEncode64Bits(in, lpOut+cbBlock, 
                (const unsigned char (*)[6])SubKeys[n % 2 == 0 ? 0 : 1], 
                nEncryptMode);

           /*
            *****************************************************************
            * swap the encryption mode
            *****************************************************************
            */
			nEncryptMode = !nEncryptMode;

           /*
            *****************************************************************
            * take copy of results for next iteration
            *****************************************************************
            */
			memcpy(in, lpOut+cbBlock, 8);
        }

       /*
        *********************************************************************
        * For decryption, Chain Block Cipher (CBC) works like this:
        * Where 'C' is cryptogram and 'M' is unencrypted message.
        *
        * For decryption:
        * 1.  Decrypt C 0->7 for M 0->7.
        * 2.  Decrypt C 8->15, exclusively OR with C 0->7 to get M 8->15.
        * 3.  Decrypt C 16->23, exclusively OR with C 8->15 to get M 16->23.
        * 4.  Decrypt C n+8->n+15, exclusively OR with C n->n+7 to get 
        *     M n+8-n+15.
        *********************************************************************
        */
		if (nCBC && !nEncrypt)
        {
            lpTmp1 = lpIn+cbBlock-8;
            lpTmp2 = lpOut+cbBlock;

            for (n = 0; n < 8; n++)
            {
                lpTmp2[n] ^= lpTmp1[n];
            }
        }

        cbRemaining-=8;
    }

   /*
    *************************************************************************
    * if we are decrypting with padding then we need to calculate the actual
    * size of the message
    *************************************************************************
    */
	if (!nEncrypt && nPadding)
    {		
       /*
        *********************************************************************
        * first ensure PKCS#5 padding is enabled
        *********************************************************************
        */
		pad = lpOut[cbOut-1];

       /*
        *********************************************************************
        * pad value must be 8 or less inorder to be valid
        *********************************************************************
        */
		if (pad <= 8)
        {
            bIsPadded = 1;

           /*
            *****************************************************************
            * check that each byte in padding part of the byte are all set to 
            * the value of the pad.
            *****************************************************************
            */
			for(n=cbOut-2; n>(cbOut-pad); n--)
            {
                if (lpOut[n] != pad)
                {
                    bIsPadded = 0;
                    break;
                }
            }

           /*
            *****************************************************************
            * if this is still true then we ARE using PKCS#5 padding
            *****************************************************************
            */
			if (bIsPadded)
            {
               /*
                *************************************************************
                * take the last byte which dictates the number of spare bytes 
                * used then subtract this from the number of bytes in the out 
                * message
                *************************************************************
                */
		        *pcbOut -= pad;
            }
        }
    }

    return 1;

}/* McbDoDES2 */

/*
 ****************************************************************************
 * For compatibility (uses Electronic Code Book ECB)
 ****************************************************************************
 */
int McbDoDES(unsigned char * lpIn, unsigned long cbIn, unsigned char Key1[8], 
    unsigned char Key2[8], unsigned char * lpOut, unsigned long * pcbOut, 
    int nEncrypt, int nPadding, int nEncNo)
{
    return McbDoDES2(lpIn, cbIn, Key1, Key2, lpOut, pcbOut, nEncrypt, 
        nPadding, nEncNo, 0);
}

