#ifndef _CREATOR_
#define _CREATOR_

/* This header file for creat all specified/needed dataType from char array.
 * SPAT, BSM, RSM, MAP, ...
 */
#include <SPAT.h>
#include "parser.h"

/* create instance of SPAT structure pre-allocated
 * return 0 for well done, -1 otherwise
 */
int create_spat_from_PhaList(Pha_t *phaList, const int len, SPAT_t *spat);

/* init spat with default value
 * return 0 for well done, -1 otherwise
 */
// int init_spat(SPAT_t *spat);

/* check constaints of spat
 * return 0 if check welll, -1 otherwise
 */
int check_constraints_spat(const SPAT_t *spat);


/*********************************************
 *    internally useful functions            *
 *********************************************/
void xml_print_spat(SPAT_t *spat);

#endif






