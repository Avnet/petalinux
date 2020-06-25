// ----------------------------------------------------------------------------
//
//        ** **        **          **  ****      **  **********  **********
//       **   **        **        **   ** **     **  **              **
//      **     **        **      **    **  **    **  **              **
//     **       **        **    **     **   **   **  *********       **
//    **         **        **  **      **    **  **  **              **
//   **           **        ****       **     ** **  **              **
//  **  .........  **        **        **      ****  **********      **
//     ...........
//                                     Reach Further
//
// ----------------------------------------------------------------------------
//
//  This design is the property of Avnet.  Publication of this
//  design is not authorized without written consent from Avnet.
//
//  Please direct any questions to the Zed community support forum:
//     http://zedboard.org/forums/zed-english-forum
//
//  Product information is available at:
//     http://zedboard.org/
//
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2018 Avnet, Inc.
//                              All rights reserved.
//
// ----------------------------------------------------------------------------
//
//  Create Date:         Jun 16, 2020
//  Design Name:         PMIC PROGRAMMER
//  Module Name:         utils.h
//  Project Name:        PMIC PROGRAMMER
//
//  Description:         Application to program Infineon PMICs
//
//  Dependencies:
//
//  Revision:            Jun 16, 2020: 1.0 Initial version
//
// ----------------------------------------------------------------------------

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint-gcc.h>
#include <stdbool.h>

#define CLEAR_STDIN { int c; while((c = getchar()) != '\n' && c != EOF); }

// Emergency errors cannot be recovered.
#define ASSERT_EMERG( expr, ... )                               \
        do                                                      \
        {                                                       \
            if ( !(expr) )                                      \
            {                                                   \
            	fprintf(stderr, ##__VA_ARGS__);                 \
                exit(-1);                                       \
            }                                                   \
        } while ( 0 );                                          \

#define CHECK_RETURN( expr, ret_val, ... )                              \
        do                                                              \
        {                                                               \
            if ( !(expr) )                                              \
            {                                                           \
                fprintf(stderr, ##__VA_ARGS__);                         \
                return ret_val;                                         \
            }                                                           \
        } while ( 0 );                                                  \

#define CONFIRM( ret_val, ... )                                                                                 \
        do                                                                                                      \
        {                                                                                                       \
            char s[2];                                                                                          \
            printf("WARNING! Issuing a wrong command can damage your system!\nYou are about to ");             \
            fprintf(stdout, ##__VA_ARGS__);                                                                     \
            printf("Continue? [y/N] ");                                                                         \
            fflush(stdout);                                                                                     \
            if ( !fgets(s, 2, stdin) ||  (s[0] != 'Y' && s[0] != 'y') )                                         \
            {                                                                                                   \
                fprintf(stderr, "Aborting on user request.\n");                                                 \
                CLEAR_STDIN;                                                                                    \
                return ret_val;                                                                                 \
            }                                                                                                   \
            CLEAR_STDIN;                                                                                        \
            printf("\n");                                                                                       \
        } while ( 0 );                                                                                          \

void convert_word_to_binary ( uint16_t word, char *str );
#endif /* UTILS_H_ */
