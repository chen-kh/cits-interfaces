#include <stdio.h>
#include <stdlib.h>

#include "citssocket.h"
#include "parser.h"
#include "creator.h"
#include "encode_decode.h"

void test_spat(void* bytes, size_t no_bytes, Pha_t* phaList, const int len);


int main(){
	void* bytes;
	size_t no_bytes = 45;
	const int len = 8;
	Pha_t *phaList;
	bytes = calloc(no_bytes, 1);
	
	do{
		recv_socket(bytes, no_bytes);
		DataType type = determine_dataType(bytes);

		switch(type){
			case T_SPAT:
				phaList = (Pha_t*)calloc(8, sizeof(Pha_t));
				test_spat(bytes, no_bytes, phaList, len);
				break;
			default:
				return -1;
		}
	}while(0);
	
	return 0;
}

void test_spat(void* bytes, size_t no_bytes, Pha_t* phaList, const int len){
	int ret;
	parse_spat(bytes, no_bytes, phaList, len);
	int n;
	for(n=0; n<len; n++){
		printf("PhaseId = %d, Light = %d, LikelyEndTime = %ld\n", phaList[n].id, phaList[n].light, phaList[n].likelyEndTime);
	}
	SPAT_t *spat = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	create_spat_from_PhaList(phaList, len, spat);
	ret = check_constraints_spat(spat);
	assert(ret==0);
	
	// show something
	xml_print_spat(spat);

	// test encode to buffer
	EncodedSPAT_t encoded_spat;
	encode_spat_to_new_buffer(spat, &encoded_spat);
	SPAT_t *de_spat = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	decode_spat(&encoded_spat, de_spat);

	// test encode
	EncodedSPAT_t encoded_spat2;
	encoded_spat2.buffer = calloc(5, sizeof(SPAT_t));
	encode_spat(de_spat, &encoded_spat2);
	SPAT_t *de_spat2 = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	decode_spat(&encoded_spat2, de_spat2);
	xml_print_spat(de_spat2);
}