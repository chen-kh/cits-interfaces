#include <stdio.h>
#include "ispat.h"
#include "citssocket.h"


int main(){
	void* bytes;
	size_t no_bytes;
	const int len = 8;
	Pha_t *phaList = (Pha_t*)calloc(8, sizeof(Pha_t));
	int ret;
	bytes = calloc(no_bytes, 1);
	
	recv_socket(bytes, no_bytes);
	DataType type = determine_dataType(bytes);

	switch(type){
		case T_SPAT:
			test_spat(bytes, no_bytes, phaList, len);
			break;
		default:
			return -1;
	}
	
	return 0;
}

int test_spat(void* bytes, size_t no_bytes, Pha_t* phaList, const int len){
	parse_spat(bytes, no_bytes, phaList, len);
	int n;
	for(n=0; n<len; n++){
		printf("%d, %d, %ld\n", phaList[n].id, phaList[n].light, phaList[n].likelyEndTime);
	}
	SPAT_t *spat = (SPAT_t*)calloc(1, sizeof(SPAT_t));
	create_spat_from_PhaList(phaList, len, spat);
	ret = check_constraints_spat(spat);
	assert(ret==0);
	// show something
	printf("msgCnt = %d\n", (int)spat->msgCnt);
	// printf("timeStamp = %ld\n", *spat->timeStamp);
	// printf("%s\n", "name:");
	// printf("\tname.size = %d\n", spat->name->size);
	// printf("\tname.buf = %s\n", (char *)spat->name->buf);
	printf("%s\n", "intersections:");
	printf("\tsize = %d\n", spat->intersections.list.size);
	printf("\tcount = %d\n", spat->intersections.list.count);
	int i;
	// size is limited 1..32
	for(i = 0; i<1; i++){
		printf("\t    arr[%d]:\n", i);
		IntersectionState_t *intersection0 = spat->intersections.list.array[i];
		// printf("\t\tintersectionId.region = %ld\n", *intersection0->intersectionId.region);
		printf("\t\tintersectionId.id = %ld\n", intersection0->intersectionId.id);
		printf("\t\tintersection.status.buf = %s\n", (char *)*intersection0->status.buf);
		printf("\t\tintersection.status.size = %d\n", intersection0->status.size);
		printf("\t\tintersection.status.bits_unused = %d\n", intersection0->status.bits_unused);
		// printf("\t\tintersection.moy = %ld\n", *intersection0->moy);
		// printf("\t\tintersection.timeStamp = %ld\n", *intersection0->timeStamp);
		printf("\t\tintersection.phases:\n");
		printf("\t\t    size = %d\n", intersection0->phases.list.size);
		printf("\t\t    count = %d\n", intersection0->phases.list.count);
		int j;
		for(j = 0; j < 1; j++){
			Phase_t* phase0 = intersection0->phases.list.array[j];
			printf("\t\t    array[%d]:\n", j);
			printf("\t\t\tphase.id = %ld\n", phase0->id);
			printf("\t\t\tphase.phaseStates:\n");
			printf("\t\t\t    size = %d\n", phase0->phaseStates.list.size);
			printf("\t\t\t    count = %d\n", phase0->phaseStates.list.count);
			int k;
			for(k=0; k<1; k++){
				PhaseState_t *phaseState0 = phase0->phaseStates.list.array[k];
				printf("\t\t\t    array[%d]:\n",k);
				long* light = (long*)calloc(1, sizeof(long));
				asn_INTEGER2long(&phaseState0->light, light);
				printf("\t\t\t\tphaseState.light = %ld\n", *light);
				printf("\t\t\t\tphaseState.timing:\n");
				printf("\t\t\t\t\tstartTime = %ld\n", phaseState0->timing->startTime);
				printf("\t\t\t\t\tLikelyEndTime = %ld\n", phaseState0->timing->likelyEndTime);
			}
		}
	}
}