#include "encode_decode.h"
#include <stdlib.h>

int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat){
	size_t init_buffer_size = sizeof(SPAT_t);
	encoded_spat->buffer = calloc(1, init_buffer_size);
	size_t buffer_size = init_buffer_size;
	asn_enc_rval_t arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
	if(arv.encoded > init_buffer_size){
		free(encoded_spat->buffer);
		encoded_spat->buffer = calloc(1, arv.encoded);
		arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
	}
	assert(arv.encoded > 0);
	encoded_spat->no_bytes_encoded = arv.encoded;
	return 0;
}

int decode_spat(EncodedSPAT_t *encoded_spat, void *spat){
	size_t mss = 0; // not check max stack size
  	asn_codec_ctx_t cc = {mss};
  	const asn_dec_rval_t rval = ber_decode(&cc, &asn_DEF_SPAT, &spat, encoded_spat->buffer, encoded_spat->no_bytes_encoded);
	if (rval.code != RC_OK) {
    	perror("ber_decode() failed");
    	fprintf(stderr, "Broken encoding at byte %ld\n", rval.consumed);
  	} else {
    	printf("Decode SPAT SUCCESS!\n");
  	}
  	return 0;
}