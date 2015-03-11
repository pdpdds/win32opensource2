/**
 ****************************************************************************
 * <P> McbDES2.h - declaration file for implementing DES and Triple DES
 * (without use of external libs).
 * The DES mode of operation is Electronic Code Book (ECB) or Chain Block
 * Coding (CBC) - Cipher Feedback (CFB) is not supported.
 *
 * I have attempted to write this source to the ANSI standard so that it can
 * be easily ported to other environments (such as C370).
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
 *    8th June          2000     -     (V1.0) Creation (MCB)
 *   21st January       2003     -     (V2.0) Modification (MCB).
 *                                     Added Chain Block Coding (CBC) for 
 *                                     better encryption.
 ****************************************************************************
 */

/*
 ****************************************************************************
 * Include once
 ****************************************************************************
 */
#ifndef McbOurMateDES2_Included
#define McbOurMateDES2_Included

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
 * @param       char * lpIn        - (in) pointer to the buffer to encrypt/
 *                                decrypt.
 * @param       long cbIn        - (in) number of bytes in the in buffer
 * @param       char Key1[8]    - (in) the first eight byte key to encrypt or
 *                                decrypt with.
 * @param       char Key2[8]    - (in) the second eight byte key to encrypt
 *                                or decrypt with - this is will only be used
 *                                if the number of times to encrypt (nEncNo)
 *                                is greater than one.
 * @param       char * lpOut    - (out) pointer to the output buffer - if
 *                                this is NULL then the no encryption/
 *                                decryption will take place but the required
 *                                length of the out buffer will be
 *                                calculated.
 * @param       long * pcbOut    - (in/out) pointer to the number of bytes in
 *                                the output buffer.
 *                                On entry, this parameter is used to ensure
 *                                the output buffer is large enough to hold
 *                                the results of the encryption/decryption
 *                                On exit, this parameter is populated with
 *                                the number of bytes required for the output
 *                                buffer.
 * @param       int nEncrypt    - (in) non zero to encrypt, zero to decrypt.
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
 *    8th June          2000        - (V1.0) Creation (MCB)
 *   21st January       2003        - (V2.0) Modification (MCB).
 *                                    Added Chain Block Coding (CBC) for 
 *                                    better encryption.
 ****************************************************************************
 */
int McbDoDES2(unsigned char * lpIn, unsigned long cbIn, unsigned char Key1[8],
    unsigned char Key2[8], unsigned char * lpOut, unsigned long * pcbOut,
    int nEncrypt, int nPadding, int nEncNo, int nCBC);

/**
 ****************************************************************************
 * FOR BACKWARD COMPATABILITY ONLY.
 ****************************************************************************
 * For compatability (uses Electronic Code Book ECB)
 ****************************************************************************
 */
int McbDoDES(unsigned char * lpIn, unsigned long cbIn, unsigned char Key1[8],
    unsigned char Key2[8], unsigned char * lpOut, unsigned long * pcbOut,
    int nEncrypt, int nPadding, int nEncNo);

/*
 ****************************************************************************
 * ELECTRONIC CODE BOOK ELECTRONIC CODE BOOK ELECTRONIC CODE BOOK ELECTRONIC
 ****************************************************************************
 * Macros to simplify call to McbDoDES - see comments for the function for
 * parameter details.
 ****************************************************************************
 */

/*
 ****************************************************************************
 * Single DES encryption
 ****************************************************************************
 */
#define McbDESEncrypt(lpIn, cbIn, szKey, lpOut, pcbOut)                     \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey, szKey,                    \
    (unsigned char*)(lpOut), pcbOut, 1, 0, 1)

/*
 ****************************************************************************
 * Single DES encryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbDESEncryptPad(lpIn, cbIn, szKey, lpOut, pcbOut)                  \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey, szKey,                    \
    (unsigned char*)(lpOut), pcbOut, 1, 1, 1)

/*
 ****************************************************************************
 * Single DES decryption
 ****************************************************************************
 */
#define McbDESDecrypt(lpIn, cbIn, szKey, lpOut, pcbOut)                     \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey, szKey,                    \
    (unsigned char*)(lpOut), pcbOut, 0, 0, 1)

/*
 ****************************************************************************
 * Single DES decryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbDESDecryptPad(lpIn, cbIn, szKey, lpOut, pcbOut)                  \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey, szKey,                    \
    (unsigned char*)(lpOut), pcbOut, 0, 1, 1)

/*
 ****************************************************************************
 * Triple DES encryption
 ****************************************************************************
 */
#define McbDES3Encrypt(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)           \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                  \
    (unsigned char*)(lpOut), pcbOut, 1, 0, 3)

/*
 ****************************************************************************
 * Triple DES encryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbDES3EncryptPad(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)        \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                  \
    (unsigned char*)(lpOut), pcbOut, 1, 1, 3)

/*
 ****************************************************************************
 * Triple DES decryption
 ****************************************************************************
 */
#define McbDES3Decrypt(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)           \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                  \
    (unsigned char*)(lpOut), pcbOut, 0, 0, 3)

/*
 ****************************************************************************
 * Triple DES decryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbDES3DecryptPad(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)        \
    McbDoDES((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                  \
    (unsigned char*)(lpOut), pcbOut, 0, 1, 3)

/*
 ****************************************************************************
 * FOR BACKWARD COMPATABILITY ONLY.
 ****************************************************************************
 * Macro useful for determining the required size of a message given the
 * number of bytes in the original message
 ****************************************************************************
 */
#define McbMESSAGESIZE(bytesInMessage)                                      \
    ((((bytesInMessage)/8) + ((bytesInMessage) % 8 > 0)) * 8)

/*
 ****************************************************************************
 * FOR BACKWARD COMPATABILITY ONLY.
 ****************************************************************************
 * Macro useful for determining the required size of a cryptogram with
 * padding.
 ****************************************************************************
 */
#define McbCRYPTOGRAMSIZEWITHPADDING(bytesInMessage)                        \
    ((((bytesInMessage)/8) + 1) * 8)

/*
 ****************************************************************************
 * CIPHER BLOCK CHAINING CIPHER BLOCK CHAINING CIPHER BLOCK CHAINING CIPHER B
 ****************************************************************************
 * Macros to simplify call to McbDoDES2 - see comments for the function for
 * parameter details.
 ****************************************************************************
 */

/*
 ****************************************************************************
 * Single CBC DES encryption
 ****************************************************************************
 */
#define McbCBCEncrypt(lpIn, cbIn, szKey, lpOut, pcbOut)                     \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey, szKey,                   \
    (unsigned char*)(lpOut), pcbOut, 1, 0, 1, 1)

/*
 ****************************************************************************
 * Single CBC DES encryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbCBCEncryptPad(lpIn, cbIn, szKey, lpOut, pcbOut)                  \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey, szKey,                   \
    (unsigned char*)(lpOut), pcbOut, 1, 1, 1, 1)

/*
 ****************************************************************************
 * Single CBC DES decryption
 ****************************************************************************
 */
#define McbCBCDecrypt(lpIn, cbIn, szKey, lpOut, pcbOut)                     \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey, szKey,                   \
    (unsigned char*)(lpOut), pcbOut, 0, 0, 1, 1)

/*
 ****************************************************************************
 * Single CBC DES decryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbCBCDecryptPad(lpIn, cbIn, szKey, lpOut, pcbOut)                  \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey, szKey,                   \
    (unsigned char*)(lpOut), pcbOut, 0, 1, 1, 1)

/*
 ****************************************************************************
 * Triple CBC DES encryption
 ****************************************************************************
 */
#define McbCBC3Encrypt(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)           \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                 \
    (unsigned char*)(lpOut), pcbOut, 1, 0, 3, 1)

/*
 ****************************************************************************
 * Triple CBC DES encryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbCBC3EncryptPad(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)        \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                 \
    (unsigned char*)(lpOut), pcbOut, 1, 1, 3, 1)

/*
 ****************************************************************************
 * Triple CBC DES decryption
 ****************************************************************************
 */
#define McbCBC3Decrypt(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)           \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                 \
    (unsigned char*)(lpOut), pcbOut, 0, 0, 3, 1)

/*
 ****************************************************************************
 * Triple CBC DES decryption with PKCS#5 padding (see above for details)
 ****************************************************************************
 */
#define McbCBC3DecryptPad(lpIn, cbIn, szKey1, szKey2, lpOut, pcbOut)        \
    McbDoDES2((unsigned char*)(lpIn), cbIn, szKey1, szKey2,                 \
    (unsigned char*)(lpOut), pcbOut, 0, 1, 3, 1)

/*
 ****************************************************************************
 * Calculate the number of bytes required for the CRYPTOGRAM given the number
 * of bytes in the text.
 * Based on multiples of 8 for as many blocks of 8 there are in the text plus
 * another 8 bytes if CBC is used plus another 8 bytes if padding is required
 * and the number of bytes in the text ends on an 8 byte boundary.
 ****************************************************************************
 */
#define McbCRYPTSIZE(bPadding, bCBC, cbText)                                \
    ((((cbText)/8) + (((cbText) % 8 > 0) ? 1 : ((bPadding) ? 1 : 0)) +      \
    ((bCBC) ? 1 : 0)) * 8)

/*
 ****************************************************************************
 * Calculate the number of bytes required for the TEXT given the number of
 * bytes IN THE CRYPTOGRAM.
 * The padding flag is ignored - if padding has been used in the cryptogram
 * then the 8 bytes are not subtracted because there is no way of knowing
 * whether the padding has meant using the extra 8 bytes without looking at
 * the decrypted crypt's content.
 ****************************************************************************
 */
#define McbTEXTSIZEFROMCRYPT(bPadding, bCBC, cbCrypt)                       \
    ((((cbCrypt)/8) - ((bCBC) ? 1 : 0)) * 8)

/*
 ****************************************************************************
 * Calculate the number of bytes required for the TEXT given the initial
 * number of bytes IN THE TEXT.
 * This is required to allow the additional 8 bytes when for padding is used.
 ****************************************************************************
 */
#define McbTEXTSIZE(bPadding, bCBC, cbText)                                 \
    McbTEXTSIZEFROMCRYPT(bPadding, bCBC, McbCRYPTSIZE(bPadding, bCBC,       \
    cbText))

#endif /*McbOurMateDES2_Included*/
