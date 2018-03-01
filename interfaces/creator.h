#ifndef _CREATOR_H_
#define _CREATOR_H_

/* This header file for creat all specified/needed dataType from char array.
 * SPAT, BSM, RSM, MAP, ...
 */
#include <SPAT.h>
#include <constr_TYPE.h>
#include "parser.h"

#define	STRUCT_FREE(asn_DEF, ptr)	(asn_DEF).free_struct(&(asn_DEF),ptr,0)
#define	STRUCT_FREE_CONTENTS_ONLY(asn_DEF, ptr)	\
					(asn_DEF).free_struct(&(asn_DEF),ptr,1)
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






