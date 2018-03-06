#include "creator.h"
#include "for_debug.h"

int create_spat_from_PhaList(Pha_t *phaList, const int len, SPAT_t *spat){
	int ret;

	IntersectionState_t *state = (IntersectionState_t*)calloc(1, sizeof(IntersectionState_t));
	
	NodeReferenceID_t intersectionId;
	intersectionId.id = 1;

	int i;
	for(i=0; i<len;i++){
		Pha_t pha = phaList[i];
		PhaseID pid = pha.id;
		Light lid = pha.light;
		long likelyEndTime = pha.likelyEndTime;

		Phase_t *phase = (Phase_t*)calloc(1, sizeof(Phase_t));
		
		PhaseState_t *phaseState = (PhaseState_t*)calloc(1,sizeof(PhaseState_t));		
		asn_long2INTEGER(&phaseState->light, lid); // set light id (dark, red, green, yellow)

		TimeChangeDetails_t *timing = (TimeChangeDetails_t*)calloc(1, sizeof(TimeChangeDetails_t));
		/* constraint: value >= 0 && value <= 36001
		 */
		timing->startTime = 0;// default value
		timing->likelyEndTime = likelyEndTime; // set likelyEndTime
		
		phaseState->timing = timing;
		
		ret = ASN_SEQUENCE_ADD(&phase->phaseStates, phaseState);
		CITS_DEBUG("%s add to %s %s!", var2str(phaseState), var2str(phase->phaseStates), ret == 0 ? "SUCCESS" : "FAIL");
		if(ret != 0){
			return -1;
		}
		phase->id = pid; // set phase id

		ret = ASN_SEQUENCE_ADD(&state->phases, phase);
		CITS_DEBUG("%s add to %s %s!", var2str(phase), var2str(state->phases), ret == 0 ? "SUCCESS" : "FAIL");
		if(ret != 0){
			return -1;
		}
	}
	state->intersectionId = intersectionId;
	/* IntersectionStatusObject constraint: (size = 8 * st->size - (st->bits_unused & 0x07)) == 16
	 */
	spat->msgCnt = 1; // default value
	state->status.buf = calloc(2, 1);
	state->status.size = 2;
	state->status.bits_unused = 0;
	ASN_SEQUENCE_ADD(&spat->intersections, state);
	CITS_DEBUG("%s add to %s %s!", var2str(state), var2str(spat->intersections), ret == 0 ? "SUCCESS" : "FAIL");
	if(ret != 0){
		return -1;
	}
	CITS_DEBUG("SPAT create %s!", ret == 0 ? "SUCCESS" : "FAIL");
	return 0;
}

int check_constraints_spat(const SPAT_t *struct_ptr){
	char error_buffer[128];
	size_t error_length = sizeof(error_buffer);
	const int ret = asn_check_constraints(&asn_DEF_SPAT, struct_ptr, error_buffer, &error_length);
	CITS_DEBUG("asn check constraint for spat %s! ret=%d", ret==0 ? "SUCCESS" : "FAIL", ret);
	return ret == 0 ? 0 : -1;
}

void xml_print_spat(SPAT_t *spat){
	xer_fprint(stdout, &asn_DEF_SPAT, spat);
}