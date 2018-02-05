#include "ispat.h"


int create_spat_from_PhaList(Pha_t *phaList, const int len, SPAT_t *spat){
	int ret;

	IntersectionStateList_t	 *intersections = (IntersectionStateList_t*)calloc(1,sizeof(IntersectionStateList_t));
	IntersectionState_t *state = (IntersectionState_t*)calloc(1, sizeof(IntersectionState_t));
	
	NodeReferenceID_t intersectionId;
	intersectionId.id = 1;
	PhaseList_t *phases = (PhaseList_t*)calloc(1, sizeof(PhaseList_t));

	int i;
	for(i=0; i<len;i++){
		Pha_t pha = phaList[i];
		PhaseID pid = pha.id;
		Light lid = pha.light;
		long likelyEndTime = pha.likelyEndTime;

		Phase_t *phase = (Phase_t*)calloc(1, sizeof(Phase_t));
		
		PhaseStateList_t *phaseStates = (PhaseStateList_t*)calloc(1, sizeof(PhaseStateList_t));
		
		PhaseState_t *phaseState = (PhaseState_t*)calloc(1,sizeof(PhaseState_t));
		
		LightState_t *lst = (LightState_t*)calloc(1,sizeof(LightState_t));
		asn_long2INTEGER(lst, lid); // set light id (dark, red, green, yellow)
		phaseState->light = *lst;
		free(lst);
		
		TimeChangeDetails_t *timing = (TimeChangeDetails_t*)calloc(1, sizeof(TimeChangeDetails_t));
		/* constraint: value >= 0 && value <= 36001
		 */
		timing->startTime = 0;// default value
		timing->likelyEndTime = likelyEndTime; // set likelyEndTime
		
		phaseState->timing = timing;
		
		ret = ASN_SEQUENCE_ADD(phaseStates, phaseState);
		assert(ret==0);
		
		phase->id = pid; // set phase id
		phase->phaseStates = *phaseStates;
		free(phaseStates);
		
		ret = ASN_SEQUENCE_ADD(phases, phase);
		assert(ret==0);
	}
	state->intersectionId = intersectionId;
	/* IntersectionStatusObject constraint: (size = 8 * st->size - (st->bits_unused & 0x07)) == 16
	 */
	spat->msgCnt = 1; // default value
	state->status.buf = calloc(2, 1);
	state->status.size = 2;
	state->status.bits_unused = 0;
	state->phases = *phases;
	free(phases);
	ASN_SEQUENCE_ADD(intersections, state);
	spat->intersections = *intersections;
	free(intersections);
	return 0;
}

int check_constraints_spat(const SPAT_t *struct_ptr){
	char error_buffer[128];
	size_t error_length = sizeof(error_buffer);
	const int ret = asn_check_constraints(&asn_DEF_SPAT, struct_ptr, error_buffer, &error_length);
	return ret == 0 ? 0 : -1;
}