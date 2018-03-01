#include <stdio.h>
#include <stdlib.h>

#include "citssocket.h"
#include "parser.h"
#include "creator.h"
#include "encode_decode.h"

void test_spat(void* bytes, size_t no_bytes, const int len);


int main(){
	void* bytes;
	size_t no_bytes = 45;
	const int len = 8;
	bytes = calloc(no_bytes, 1);
	do{
		recv_socket(bytes, no_bytes);
		DataType type = determine_dataType(bytes);

		switch(type){
			case T_SPAT:
				test_spat(bytes, no_bytes, len);
				// free(bytes);
				// bytes=NULL;
				break;
			default:
				return -1;
		}
	}while(0);
	
	return 0;
}

void test_spat(void* bytes, size_t no_bytes, const int len){
	Pha_t *phaList;
	phaList = (Pha_t*)calloc(8, sizeof(Pha_t));
	int ret;
	parse_spat(bytes, no_bytes, phaList, len);
	int n;
	for(n=0; n<len; n++){
		printf("PhaseId = %d, Light = %d, LikelyEndTime = %ld\n", phaList[n].id, phaList[n].light, phaList[n].likelyEndTime);
	}
	SPAT_t *spat = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	ret = create_spat_from_PhaList(phaList, len, spat);
	assert(ret==0);
	free(phaList);
	phaList=NULL;

	ret = check_constraints_spat(spat);
	assert(ret==0);
	
	// show something
	xml_print_spat(spat);

	// test encode to buffer
	EncodedSPAT_t encoded_spat;
	encode_spat_to_new_buffer(spat, &encoded_spat);
	SPAT_t *de_spat = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	decode_spat(&encoded_spat, de_spat);
	free(encoded_spat.buffer);
	
	// test encode
	EncodedSPAT_t encoded_spat2;
	ssize_t buffer_size = determine_encoding_size(&asn_DEF_SPAT, de_spat);
	encoded_spat2.buffer = calloc(1, buffer_size);
	encode_spat(de_spat, &encoded_spat2, buffer_size);
	SPAT_t *de_spat2 = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	decode_spat(&encoded_spat2, de_spat2);
	free(encoded_spat2.buffer);
	// show again
	xml_print_spat(de_spat2);
	
	// free spat allocated
	STRUCT_FREE(asn_DEF_SPAT, spat);
	STRUCT_FREE(asn_DEF_SPAT, de_spat);
	STRUCT_FREE(asn_DEF_SPAT, de_spat2);
}