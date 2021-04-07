#include "qioutils.h"


static size_t qIOUtil_xBase_U32toA( qUINT32_t num, char* str, qUINT8_t base );
static char qIOUtil_NibbleToX( qUINT8_t value );
static const char * qIOUtil_DiscardWhitespaces( const char *s, size_t maxlen );
static const char * qIOUtil_CheckStrSign( const char *s, int *sgn );

static const char * qIOUtil_DiscardWhitespaces( const char *s, size_t maxlen ){
    /*cstat -MISRAC2012-Rule-13.5 */ 
    while( ( maxlen > (size_t)0 ) && ( 0 != isspace( (int)*s ) ) ){ /*isspace is known to have no side effects*/  
    /*cstat +MISRAC2012-Rule-13.5 */  
        s++; /*discard whitespaces*/ /*MISRAC2004-17.4_a deviation allowed*/ 
        --maxlen;
    }    
    return s;
}
/*============================================================================*/
static const char * qIOUtil_CheckStrSign( const char *s, int *sgn ){
    if( '-' == *s ){ /*if negative found*/
        *sgn = -1; /*set the sign*/
        ++s; /*move to next*/ /*MISRAC2004-17.4_a deviation allowed*/ 
    } 
    else if( '+' == *s ){
        ++s; /*plus sign ignored, move to next*/  /*MISRAC2004-17.4_a deviation allowed*/ 
    } 
    else{
        /*nothing to do*/
    }  
    return s;
}
/*============================================================================*/
/*
Returns a pointer to the first occurrence of character in the C string str.

The terminating null-character is considered part of the C string. Therefore, 
it can also be located in order to retrieve a pointer to the end of a string.

Parameters:

    - s : pointer to the null-terminated byte string to be examined
    - c : Character to be located. It is passed as its int promotion, but it 
        is internally converted back to char for the comparison.
    - maxlen : 	maximum number of characters to examine

Return value:

    A pointer to the first occurrence of character in <str>.
    If the character is not found, the function returns a null pointer.
*/
char* qIOUtil_StrChr( const char *s, int c, size_t maxlen ){
    char *RetValue = NULL;
    do{
        if( (int)(*s) == c ){
            /*cstat -MISRAC2012-Rule-11.8*/
            RetValue = (char*)s; /*MISRAC2012-Rule-11.8 deviation allowed*/
            /*cstat +MISRAC2012-Rule-11.8*/
            break;
        }
        --maxlen;
    }while( ( '\0' != ( *s++ ) ) && ( maxlen > 0u ) );
    return RetValue;
}
/*============================================================================*/
/* size_t qIOUtil_StrLen( const char* str, size_t maxlen )

Returns the length of the given null-terminated byte string, that is, the number 
of characters in a character array whose first element is pointed to by <str> up 
to and not including the first null character.
The function returns zero if <str> is a null pointer and returns <maxlen> if the 
null character was not found in the first <maxlen> bytes of <str>.

Parameters:

    - str : pointer to the null-terminated byte string to be examined
    - maxlen : 	maximum number of characters to examine

Return value:

  The length of the null-terminated byte string str on success, zero if <str> 
  is a null pointer, <maxlen> if the null character was not found.
*/
size_t qIOUtil_StrLen( const char* str, size_t maxlen ){
    size_t count;

    if( ( NULL == str ) || ( (size_t)0 == maxlen ) ){
        count = 0;
    }
    else{
        count = 0;
        while( ( (char)'\0' != *str ) && ( maxlen > (size_t)0 ) ) {
            ++count;
            --maxlen;
            ++str;
        }        
    }
    return count;
}
/*============================================================================*/
/*size_t qIOUtil_StrlCpy( char * dst, const char * src, size_t maxlen )
 
Copies up to <maxlen> - 1 characters from the null-terminated string <src> to 
<dst>, null-terminating the result.
 
Parameters:

    - dst : the destination string
    - src: the source string
    - maxlen : Max number of characters to copy

Return value:

  The length of <src>
*/
size_t qIOUtil_StrlCpy( char * dst, const char * src, size_t maxlen ){
    const size_t srclen = qIOUtil_StrLen( src, Q_IOUTIL_MAX_STRLEN );

    if( ( srclen + 1u ) < maxlen ){
        (void)memcpy( dst, src, srclen + 1u );
    } 
    else if ( 0u != maxlen ) {
        (void)memcpy( dst, src, maxlen - 1u );
        dst[ maxlen-1u ] = (char)'\0';
    }
    else{
        /*nothing to do here*/
    }
    return srclen;
}
/*============================================================================*/
/*makes the basic conversion of unsigned integer to ASCII. NULL Terminator not included*/
static size_t qIOUtil_xBase_U32toA( qUINT32_t num, char* str, qUINT8_t base ){
    size_t i = 0u;
    
    if( ( 0uL == num ) || ( 0u == base ) ){ /* Handle 0 explicitly, otherwise empty string is printed for 0 */
        str[ i++ ] = '0';  /*MISRAC2004-17.4_b deviation allowed*/        
    }
    else{
        qUINT32_t rem;
        while( 0uL != num ){ /*Process individual digits*/
            rem = num % (qUINT32_t)base;
            /*cstat -CERT-INT30-C_a*/
            str[ i++ ] = ( rem > 9uL )? (char)((qUINT8_t)( rem - 10uL ) + 'A' ) : (char)( (qUINT8_t)rem + '0' );
            /*cstat +CERT-INT30-C_a*/
            num = num/base;
        }
        qIOUtil_SwapBytes( str, i );/*Reverse the string*/
    }
    return i;       
}
/*============================================================================*/
static char qIOUtil_NibbleToX( qUINT8_t value ){
    char ch;
    
    ch = (char)( (qUINT8_t)( value & 0x0Fu ) + '0' );
    return (char)( ( ch > '9' )? (char)( ch + 7 ) : ch );
}
/*============================================================================*/
/*void qIOUtil_SwapBytes( void *Data, const size_t n )
 
Invert the endianess for n bytes of the specified memory location
 
Parameters:

    - Data : a pointer to block of data
    - n : the number of bytes to swap
*/
/*============================================================================*/
void qIOUtil_SwapBytes( void *Data, const size_t n ){
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    qUINT8_t *p = Data, tmp; /*MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    size_t lo, hi;

    hi = n - 1u;
    for( lo = 0u ; hi > lo ; ++lo ){
        tmp = p[ lo ];
        p[ lo ] = p[ hi ];
        p[ hi ] = tmp;
        --hi;
    }
}
/*============================================================================*/
/*qBool_t qIOUtil_CheckEndianness( void )
 
Check the system endianess
  
Return value:

    qTrue if Little-Endian, otherwise returns qFalse
*/
qBool_t qIOUtil_CheckEndianness( void ){
    qUINT16_t i = 1u;

    return (qBool_t)( *( (qUINT8_t*)&i ) );
}
/*============================================================================*/
/*void qIOUtil_OutputString( qPutChar_t fcn, void* pStorage, const char *s, qBool_t AIP )
 
Wrapper method to write a string through <fcn> 

Parameters:

    - fcn : The basic output byte function
    - pStorage : The storage pointer passed to <fcn>
    - s: The string to be written
    - AIP : Auto-Increment the storage-pointer
*/
void qIOUtil_OutputString( qPutChar_t fcn, void* pStorage, const char *s, qBool_t AIP ){
    size_t i = 0u;
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    char *xPtr = pStorage; /*MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    if( qTrue == AIP ){
        while( (char)'\0' != *s ){
            fcn( &xPtr[ i ] ,  *s++ );
            ++i;
        }
    }
    else{
        while( '\0' != *s ){
            fcn( pStorage, *s++ );
        }
    }
}
/*============================================================================*/
void qIOUtil_PrintXData( qPutChar_t fcn, void* pStorage, void *Data, size_t n ){
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    qUINT8_t *pdat =(qUINT8_t*)Data;  /*MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    size_t i;

    for( i = 0u ; i < n ; ++i ){
        fcn( pStorage, qIOUtil_NibbleToX( pdat[ i ] >> 4u ) );   /*MISRAC2004-17.4_b deviation allowed*/ 
        fcn( pStorage, qIOUtil_NibbleToX( pdat[ i ] & 0x0Fu ) ); /*MISRAC2004-17.4_b deviation allowed*/ 
        fcn( pStorage, ' ');
    }
    fcn( pStorage, '\r' );
    fcn( pStorage, '\n' );
}
/*============================================================================*/
/*void qIOUtil_OutputRaw( qPutChar_t fcn, void* pStorage, void *Data, const size_t n, qBool_t AIP )
 
Wrapper method to write n RAW data through fcn
  
Parameters:

    - fcn : The basic output byte function
    - pStorage : The storage pointer passed to <fcn>
    - Data: Buffer to read data from
    - n: The size of <data>
    - AIP : Auto-Increment the storage-pointer
*/
void qIOUtil_OutputRaw( qPutChar_t fcn, void* pStorage, void *Data, const size_t n, qBool_t AIP ){
    size_t i;
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    char *cdata = Data; /*MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    char *xPtr = pStorage; /*MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    if( qTrue == AIP ){
        for( i = 0u ; i < n ; ++i ){
            fcn( &xPtr[ i ] , cdata[ i ] );
        }
    }
    else{
        for( i = 0u ; i < n ; ++i ){
            fcn( pStorage, cdata[ i ] );
        }
    }
}
/*============================================================================*/
/*void qIOUtil_InputRaw( const qGetChar_t fcn, void* pStorage, void *Data, const size_t n, qBool_t AIP )

Wrapper method to get <n> RAW data through <fcn>
  
Parameters:

    - fcn : The basic input byte function
    - pStorage : The storage pointer passed to <fcn>
    - Data: Buffer to read data from
    - n: Number of bytes to get
    - AIP : Auto-Increment the storage-pointer
*/
void qIOUtil_InputRaw( const qGetChar_t fcn, void* pStorage, void *Data, const size_t n, qBool_t AIP ){
    size_t i;
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    char *cdata = Data; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    char *xPtr = pStorage; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed*/
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    if( qTrue == AIP ){
        for( i = 0u ; i < n ; ++i ){
            cdata[ i ] = fcn( &xPtr[ i ] );
        }
    }
    else{
        for( i = 0u ; i < n ; ++i ){
            cdata[ i ] = fcn( pStorage );
        }
    }
}
/*============================================================================*/
/*void qIOUtil_U32toX( qUINT32_t value, char *str, qINT8_t n )
 
Converts an unsigned integer value to a null-terminated string using the 16 base
and stores the result in the array given by <str> parameter.
<str> should be an array long enough to contain any possible value.
  
Parameters:

    - value : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.
    - n: The number of chars used to represent the value in <str> 

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_U32toX( qUINT32_t value, char *str, qINT8_t n ){ 
    qBase_t i;
    /*cstat -CERT-STR34-C*/
    str[ n ] = '\0'; /*MISRAC2004-17.4_b deviation allowed*/ /*CERT-STR34-C deviation allowed*/
    for( i = ( (qBase_t)n - 1 ) ; i >= 0 ; --i ){
        str[ i ] = qIOUtil_NibbleToX( (qUINT8_t)value ); /*MISRAC2004-17.4_b deviation allowed*/ 
        value >>= 4uL;
    }
    /*cstat +CERT-STR34-C*/
    return str;
}
/*============================================================================*/
/*uint32_t qIOUtil_XtoU32( const char *s )
  
Converts the input string <s> consisting of hexadecimal digits into an unsigned 
integer value. The input parameter <s> should consist exclusively of hexadecimal 
digits, with optional whitespaces. The string will be processed one character at
a time, until the function reaches a character which it doesn't recognize
(including a null character).
  
Parameters:

    - s : The hex string to be converted

Return value:

  The numeric value uint32_t
*/
qUINT32_t qIOUtil_XtoU32( const char *s ) {
    qUINT32_t val = 0uL;

    if( NULL != s ){
        qUINT8_t nparsed = 0u;
        qUINT8_t xByte;

        while ( ( (char)'\0' != *s ) && ( nparsed < 8u) ) { /*loop until the end of the string or the number of parsed chars exceeds the 32bit notation*/
            xByte = (qUINT8_t)toupper( (int)*s++ ); /*get the hex char, considerate only upper case*/ /*MISRAC2004-17.4_a deviation allowed*/ 
            if( 0 != isxdigit( (int)xByte ) ){ /*if is a valid hex digit*/
                ++nparsed; /*increase the parsed char count*/
                if ( ( (char)xByte >= '0' ) && ( (char)xByte <= '9') ){
                    xByte = (qUINT8_t)( xByte - (qUINT8_t)48u ); /* '48u' = '0' -> make the conversion in the 0-9 range*/ 
                } 
                else if ( ( (char)xByte >= 'A' ) && ( (char)xByte <='F') ){
                    xByte = (qUINT8_t)( xByte - (qUINT8_t)75u );  /* 75u = 'A' + 10 -> make the conversion in the A-F range*/        
                }     
                else{
                    /*nothing to do */
                }     
                val = (qUINT32_t)((qUINT32_t)(val << 4uL) | ((qUINT32_t)xByte & 0xFuL) );  /*add the corresponding nibble to the output*/                
            }
            else if( 0 != isspace( (int)xByte ) ){
                /*discard any white-space char*/
            } 
            else{
                break;  /*not valid chars break the conversion*/       
            }
        }
    }
    return val;
}
/*============================================================================*/
/* qFloat64_t qIOUtil_AtoF( const char *s )
Parses the C string s, interpreting its content as a floating point number and 
returns its value as a double(qFloat64_t). The function first discards as many 
whitespace characters (as in isspace) as necessary until the first non-whitespace 
character is found. Then, starting from this character, takes as many characters 
as possible that are valid following a syntax resembling that of floating point 
literals, and interprets them as a numerical value. The rest of the string after
the last valid character is ignored and has no effect on the behavior of this 
function.
 
Parameters:

    - s : The string beginning with the representation of a floating-point number.

Return value:

    On success, the function returns the converted floating point number as 
    a double(qFloat64_t) value.
    If no valid conversion could be performed, the function returns zero (0.0).
    If the converted value would be out of the range of representable values by
    a double(qFloat64_t), it causes undefined behavior
*/
qFloat64_t qIOUtil_AtoF( const char *s ){
    qFloat64_t rez = 0.0, fact;
    qBool_t point_seen = qFalse;
    int sgn = 1;
    char c;

    #if ( Q_ATOF_FULL == 1 )
        int power2, powersign = 1;
        qFloat64_t power = 1.0, efactor;
    #endif
   
    s = qIOUtil_DiscardWhitespaces( s, Q_IOUTIL_MAX_STRLEN );
    s = qIOUtil_CheckStrSign( s, &sgn );
    /*cstat -CERT-FLP36-C*/
    fact = ( qFloat64_t )sgn; /*CERT-FLP36-C deviation allowed*/

    while( (char)'\0' != ( c = *s) ) { /*MISRAC2004-17.4_a deviation allowed*/ 
        if( '.' == c ){
            point_seen = qTrue; 
        }
        else if( 0 != isdigit( (int)c ) ){
            if( qTrue == point_seen ){
                fact *= 0.1;
            }
            rez = rez * 10.0 + ( (double)c ) - ( (double)'0' ); /*CERT-FLP36-C deviation allowed*/
        }
        else{
            break;
        }
        s++;
    }
    /*cstat +CERT-FLP36-C*/
    #if ( Q_ATOF_FULL == 1 )
    if( ( 'e'  == *s ) || ( 'E' == *s ) ){
	    s++;
	    if( ( '-' == *s ) || ( '+' == *s ) ){
		    powersign = ( '-' == *s )? -1 : 1;
            s++;    
        } 
        for( power2 = 0; isdigit( *s ); s++ ){
            power2 = power2 * 10 + ( *s - '0' );
        }
        if( power2 > 0){
 		    efactor = ( -1 == powersign )? 0.1 : 10.0;
        	for( power = 1; 0 != power2; power2-- ){
            	power *= efactor;
            }	
	    }
    }    
    return power * rez * fact;   
    #else
        return rez * fact;
    #endif      
}
/*============================================================================*/
/* char* qIOUtil_FtoA( qFloat32_t f, char *str, qUINT8_t precision )

Converts a float value to a formatted string.

Parameters:

    - num : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.
    - precision: Desired number of significant fractional digits in the string.
                 (The max allowed precision is MAX_FTOA_PRECISION=10)

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_FtoA( qFloat32_t num, char *str, qUINT8_t precision ){ /*limited to precision=10*/
    if( NULL != str ){
        if( ( num >= 0.0f ) && ( num < 1.0E-38 ) ){ /*handle the 0.0f*/
            str[ 0 ] = '0';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 1 ] = '.';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 2 ] = '0';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 3 ] = (char)'\0'; /*MISRAC2004-17.4_b deviation allowed*/       
        }
        else if( qTrue == qIOUtil_IsInf(num) ){ /*handle the infinity*/
            str[ 0 ] = ( num > 0.0f )? '+' : '-'; /*MISRAC2004-17.4_b deviation allowed*/
            str[ 1 ] = 'i';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 2 ] = 'n';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 3 ] = 'f';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 4 ] = (char)'\0'; /*MISRAC2004-17.4_b deviation allowed*/  
        }
        else if( qTrue == qIOUtil_IsNan(num) ){ /*handle the NAN*/
            str[ 0 ] = 'n';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 1 ] = 'a';  /*MISRAC2004-17.4_b deviation allowed*/
            str[ 2 ] = 'n';  /*MISRAC2004-17.4_b deviation allowed*/ 
            str[ 3 ] = (char)'\0'; /*MISRAC2004-17.4_b deviation allowed*/
        }
        else{
            qUINT32_t intPart;
            size_t i = 0u;

            if( precision > Q_MAX_FTOA_PRECISION ){
                precision = Q_MAX_FTOA_PRECISION; /*clip the precision*/
            }
            
            if( num < 0.0f ){ /*handle the negative numbers*/
                num = -num; /*leave it positive for the convert method*/
                str[ i++ ] = '-'; /*add the negative sign*/
            }
            
            intPart = (qUINT32_t)num; /*get the integer parts*/
            /*cstat -CERT-FLP36-C*/
            num -= (qFloat32_t)intPart; /*get the floating-point part subtracting the integer part from the original value*/ /*CERT-FLP36-C deviation allowed*/
            i += qIOUtil_xBase_U32toA( intPart, &str[ i ], 10u ); /*convert the integer part in decimal form*/
            if( precision > 0u ){ /*decimal part*/
                char c;

                str[ i++ ] = '.'; /*place decimal point*/ /*MISRAC2004-17.4_b deviation allowed*/
                while( 0u != precision-- ){ /*convert until precision reached*/
                    num *= 10.0f;  /*start moving the floating-point part one by one multiplying by 10*/
                    c = (char)num; /*get the bcd byte*/
                    str[ i++ ] = (char) ((qUINT8_t)c + '0' ); /*convert to ASCII and put it inside the buffer*/
                    num -= (qFloat32_t)c; /*Subtract the processed floating-point digit*/
                }
            }
            /*cstat +CERT-FLP36-C*/
            str[ i ] = (char)'\0'; /*put the null char*/ /*MISRAC2004-17.4_b deviation allowed*/
        }
    }
    return str;
}
/*============================================================================*/
/*int qIOUtil_AtoI( const char *s )
Parses the C-string <s> interpreting its content as an integral number, which is 
returned as a value of type int. The function first discards as many whitespace
characters (as in isspace) as necessary until the first non-whitespace character 
is found. Then, starting from this character, takes an optional initial plus or
minus sign followed by as many base-10 digits as possible, and interprets them 
as a numerical value.
The string can contain additional characters after those that form the integral
number, which are ignored and have no effect on the behavior of this function.
If the first sequence of non-whitespace characters in <s> is not a valid integral 
number, or if no such sequence exists because either <s> is empty or it contains 
only whitespace characters, no conversion is performed and zero is returned.

Parameters:

    - s : The string beginning with the representation of a integer number.

Return value:

On success, the function returns the converted integral number as an int value.
If the converted value would be out of the range of representable values by 
an int, it causes undefined behavior.
*/
int qIOUtil_AtoI( const char *s ){
    int RetValue = 0;

    if( NULL != s ){
        int res = 0; /*holds the resulting integer*/
        int sgn = 1; /*only to hold the sign*/
        
        s = qIOUtil_DiscardWhitespaces( s, Q_IOUTIL_MAX_STRLEN );
        s = qIOUtil_CheckStrSign( s, &sgn );
        while( '\0' != *s ){ /*iterate until null char is found*/
            if ( ( *s < '0' ) || ( *s > '9' ) ){
                break; 
            }
            res = res * 10 + ( (int)*s) - ((int)'0' ); /*if the char is digit, compute the resulting integer*/
            ++s; /*MISRAC2004-17.4_a deviation allowed*/ 
        }
        RetValue =  sgn * res; /*return the computed integer with sign*/
    }
    return RetValue;
}
/*============================================================================*/
/* char* qIOUtil_UtoA( qUINT32_t num, char* str, qUINT8_t base )

Converts an unsigned value to a null-terminated string using the specified base 
and stores the result in the array given by <str> parameter. 

<str> should be an array long enough to contain any possible value: 
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 
32-bits platforms.

Parameters:

    - num : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.
    - base: Numerical base used to represent the value as a string, between 2 
            and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary. 

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_UtoA( qUINT32_t num, char* str, qUINT8_t base ){
    if( NULL != str ){
        size_t i;

        i = qIOUtil_xBase_U32toA( num, str, base ); /*make the unsigned conversion without the null terminator*/
        str[ i ] = (char)'\0'; /*add the null terminator*/ /*MISRAC2004-17.4_b deviation allowed*/
    }
    return str;
}
/*============================================================================*/
/* char* qIOUtil_ItoA(q INT32_t num, char* str, qUINT8_t base )

Converts an integer value to a null-terminated string using the specified base 
and stores the result in the array given by <str> parameter. If base is 10 and 
value is negative, the resulting string is preceded with a minus sign (-). 
With any other base, value is always considered unsigned.

<str> should be an array long enough to contain any possible value: 
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 
32-bits platforms.

Parameters:

    - num : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.
    - base: Numerical base used to represent the value as a string, between 2 
            and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary. 

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_ItoA( qINT32_t num, char* str, qUINT8_t base ){
    if( NULL != str ){
        size_t i = 0u;

        if( num < 0 ){ 
            if( 10u == base ){ /*handle negative numbers only with 10-base*/
                str[ i++ ] = '-';/*put the sign at the begining*/ /*MISRAC2004-17.4_b deviation allowed*/
            } 
            num = -num;
        }
        i += qIOUtil_xBase_U32toA( (qUINT32_t)num, &str[ i ], base ); /*make the unsigned conversion without the null terminator*/   /*MISRAC2004-17.4_b deviation allowed*/ 
        str[ i ] = '\0'; /*Append string terminator*/ /*MISRAC2004-17.4_b deviation allowed*/
    }
    return str;
}
/*============================================================================*/
/* char* qIOUtil_BtoA( qBool_t num, char *str )

Converts a boolean value to a null-terminated string. Input is considered true
with any value different to zero (0).

<str> should be an array long enough to contain the output

Parameters:

    - num : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_BtoA( qBool_t num, char *str ){
    if( NULL != str ){
        if( qTrue == num ){
            str[ 0 ] ='t'; str[ 1 ] = 'r'; str[ 2 ] = 'u'; str[ 3 ] = 'e'; str[ 4 ] = '\0'; /*MISRAC2004-17.4_b deviation allowed*/
        }
        else{
            str [ 0 ] = 'f'; str[ 1 ] = 'a'; str[ 2 ] = 'l'; str[ 3 ] = 's'; str[ 4 ] = 'e'; str[ 5 ] = '\0'; /*MISRAC2004-17.4_b deviation allowed*/
        }
    }
    return str;
}
/*============================================================================*/
/* char* qIOUtil_QBtoA( qBool_t num, char *str )

Converts a qBool_t value to a null-terminated string. Input is considered true
with any value different to zero (0).

<str> should be an array long enough to contain the output

Parameters:

    - num : Value to be converted to a string.
    - str : Array in memory where to store the resulting null-terminated string.

Return value:

  A pointer to the resulting null-terminated string, same as parameter <str>
*/
char* qIOUtil_QBtoA( qBool_t num, char *str ){
    if( NULL != str ){
        switch( (qUINT8_t)num ){
            case qTrue:
                str[0]='t'; str[1]='r'; str[2]='u'; str[3]='e'; str[4]='\0'; /*MISRAC2004-17.4_b deviation allowed*/
                break;
            case qFalse:
                str[0]='f'; str[1]='a'; str[2]='l'; str[3]='s'; str[4]='e'; str[5]='\0';  /*MISRAC2004-17.4_b deviation allowed*/
                break;
            case qRESPONSETIMEOUT:
                str[0]='t'; str[1]='i'; str[2]='m'; str[3]='e'; str[4]='o'; str[5]='u';str[6]='t';str[7]='\0'; /*MISRAC2004-17.4_b deviation allowed*/
                break;
            case qRISING:
                str[0]='r'; str[1]='i'; str[2]='s'; str[3]='i'; str[4]='n'; str[5]='g';str[6]='\0'; /*MISRAC2004-17.4_b deviation allowed*/
                break;
            case qFALLING:
                str[0]='f'; str[1]='a'; str[2]='l'; str[3]='l'; str[4]='i'; str[5]='n';str[6]='g';str[7]='\0'; /*MISRAC2004-17.4_b deviation allowed*/
                break;
            default:
                str[0]='u'; str[1]='n'; str[2]='k'; str[3]='n'; str[4]='o'; str[5]='w';str[6]='n';str[7]='\0'; /*MISRAC2004-17.4_b deviation allowed*/
                break;            
        }
    }
    return str;
}
/*============================================================================*/
/*qBool_t qIOUtil_IsInf( qFloat32_t f )
Determines if the given floating point number arg is positive or negative infinity

Parameters:

    - f : Floating point value(32bits).

Return value:
 
    qTrue is argument has an infinite value, otherwise qFalse
*/
qBool_t qIOUtil_IsInf( qFloat32_t f ){
    qUINT32_t u = 0uL;

    (void)memcpy( &u, &f, sizeof(u) );
    return ( ( 0x7f800000uL == u ) || ( 0xff800000uL == u ) )? qTrue : qFalse;
}
/*============================================================================*/
/*qBool_t qIsNan( qFloat32_t f )
Determines if the given floating point number arg is a not-a-number (NaN) value. 

Parameters:

    - f : Floating point value(32bits).

Return value:

    qTrue is argument is NaN, otherwise qFalse
*/
qBool_t qIOUtil_IsNan( qFloat32_t f ){
    qUINT32_t u = 0uL;

    (void)memcpy( &u, &f, sizeof(u) );
    return ( ( 0x7F800000uL == ( u & 0x7F800000uL ) ) && ( 0uL != (u & 0x7FFFFFuL) ) )? qTrue : qFalse;
}
/*============================================================================*/
