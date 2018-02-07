#ifndef _ENCODE_DECODE_
#define _ENCODE_DECODE_

#include <SPAT.h>

typedef struct encoded_spat{
	void *buffer;
	size_t no_bytes_encoded;
}EncodedSPAT_t;

/* encode spat to EncodedSPAT_t(struct) which should be pre-allocated.
 * note that the pre-allocated buffer must large enough.
 * return 0 for success, -1 fail
 */
int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat);

/* encode spat to EncodedSPAT_t(struct) which allocate himself.
 */
int encode_spat_to_new_buffer(SPAT_t *spat, EncodedSPAT_t *encoded_spat);

/* decode EncodedSPAT_t to void pointer which could be casted to SPAT_t
 */ 
int decode_spat(EncodedSPAT_t *encoded_spat, void *spat);

/* to do
 */ 
int encode_bsm();
int encode_bsm_to_new_buffer();
int decode_bsm();
int encode_rsm();
int encode_rsm_to_new_buffer();
int decode_rsm();
int and_others();

#endif