#include "encode_decode.h"
#include "for_debug.h"

/* Problem remains:
 * 1. how to determine buffer size allocated?
 * 	  if the init_buffer_size is not large enough, it causes problem while decoding.
 * 2. how to determine buffer_size?
 * 	  if the buffer_size is not large enough, the encode result return -1
 */
int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat){
	size_t buffer_size = 1024; // ???
	asn_enc_rval_t arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
	encoded_spat->no_bytes_encoded = arv.encoded;
	if(arv.encoded > 0){
		CITS_DEBUG("SPAT encode may SUCCESS, number of encoded bytes = %ld", arv.encoded);
		return 0;
	}else{
		CITS_DEBUG("SPAT encode FAILED, number of encoded bytes = %ld", arv.encoded);
		return -1;
	}
}

int encode_spat_to_new_buffer(SPAT_t *spat, EncodedSPAT_t *encoded_spat){
	size_t init_buffer_size = sizeof(SPAT_t);
	encoded_spat->buffer = calloc(1, init_buffer_size);
	size_t buffer_size = 10 * init_buffer_size; // ???
	asn_enc_rval_t arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
	if(arv.encoded > init_buffer_size){
		encoded_spat->buffer = calloc(1, arv.encoded);
		arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
	}
	encoded_spat->no_bytes_encoded = arv.encoded;
	if(arv.encoded > 0){
		CITS_DEBUG("SPAT encode may SUCCESS, number of encoded bytes = %ld", arv.encoded);
		return 0;
	}else{
		CITS_DEBUG("SPAT encode FAILED, number of encoded bytes = %ld", arv.encoded);
		free(encoded_spat->buffer);
		encoded_spat->buffer = NULL;
		return -1;
	}
}

int decode_spat(EncodedSPAT_t *encoded_spat, void *spat){
	size_t mss = 0; // not check max stack size
  	asn_codec_ctx_t cc = {mss};
  	const asn_dec_rval_t rval = ber_decode(&cc, &asn_DEF_SPAT, &spat, encoded_spat->buffer, encoded_spat->no_bytes_encoded);
	if (rval.code != RC_OK) {
    	CITS_DEBUG("ber_decode FAILED, broken encoding at byte %ld", rval.consumed);
    	return -1;
  	} else {
    	CITS_DEBUG("Decode SPAT SUCCESS!");
  		return 0;
  	}
}