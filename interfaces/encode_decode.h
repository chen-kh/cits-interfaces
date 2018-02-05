#include <SPAT.h>


typedef struct encoded_spat{
	void *buffer;
	size_t no_bytes_encoded;
}EncodedSPAT_t;



int encode_spat(SPAT_t *spat, EncodedSPAT_t *encoded_spat);

int decode_spat(EncodedSPAT_t *encoded_spat, void *spat);