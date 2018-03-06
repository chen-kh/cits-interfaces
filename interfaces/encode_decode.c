#include "encode_decode.h"
#include "for_debug.h"
#include <constr_TYPE.h>

/* 
 */
int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat, size_t buffer_size){
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
	ssize_t size_encoding = determine_encoding_size(&asn_DEF_SPAT, spat);
	encoded_spat->buffer = calloc(1, size_encoding);
	size_t buffer_size = size_encoding; // ???
	asn_enc_rval_t arv = der_encode_to_buffer(&asn_DEF_SPAT, spat, encoded_spat->buffer, buffer_size);
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
    	// ASN_STRUCT_FREE(asn_DEF_SPAT, spat);
    	return -1;
  	} else {
    	CITS_DEBUG("Decode SPAT SUCCESS!");
  		return 0;
  	}
}

ssize_t determine_encoding_size(struct asn_TYPE_descriptor_s *type_descriptor, void *struct_ptr/* Structure to be encoded */){
	asn_enc_rval_t arv = der_encode(type_descriptor, struct_ptr, pass_write_out_f, stdout);
	return arv.encoded;
}

/* function used to dertermin the size of the structure's encodeing before actually doing the encoding*/
static int pass_write_out_f(const void *buffer, size_t size, void *key) {
	return 0;
}