#ifndef _ENCODE_DECODE_H_
#define _ENCODE_DECODE_H_

#include <SPAT.h>

typedef struct encoded_spat{
	void *buffer;
	size_t no_bytes_encoded;
}EncodedSPAT_t;

/* encode spat to EncodedSPAT_t(struct) which should be pre-allocated.
 * note that the pre-allocated buffer must large enough.
 * return 0 for success, -1 fail
 */
int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat, size_t buffer_size);

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
/***************************************
 *    internal and useful function     *
 ***************************************/
/* function to determine the size of the structure's encoding before actually doing the encoding
 */
ssize_t determine_encoding_size(struct asn_TYPE_descriptor_s *type_descriptor, 
								void *struct_ptr/* Structure to be encoded */);
/* function used to determine the size of the structure's encoding before actually doing the encoding
 */
static int pass_write_out_f(const void *buffer, size_t size, void *key);

#endif