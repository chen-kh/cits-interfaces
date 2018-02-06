#include "parser.h"


DataType determine_dataType(char* buffer){
	return T_SPAT;
}

int parse_spat(char *bytes, size_t no_bytes, Pha_t *phaList, const int len){
	int i;
	for(i=0; i<len; i++){
		PhaseID id = get_phaseId_by_no(i);
		// judge if id is PhaseIdError
		Light light = get_light_by_byte(bytes[5+5*i]);
		long time = get_likelyEndTime_by_bytes(bytes[6+5*i],bytes[7+5*i]);
		
		phaList[i].id = id;
		phaList[i].light = light;
		phaList[i].likelyEndTime = time;
		
		if(check_checksum_by_bytes(bytes[8+5*i],bytes[5+5*i],bytes[6+5*i],bytes[7+5*i]) != 0){
			return -1;
		}
	}
	return 0;
}

PhaseID get_phaseId_by_no(int no){
	switch(no){
		case V_East:
			return V_East;
		case V_South:
			return V_South;
		case V_West:
			return V_West;
		case V_North:
			return V_North;
		case P_East:
			return P_East;
		case P_South:
			return P_South;
		case P_West:
			return P_West;
		case P_North:
			return P_North;
		default:
			return PhaseIdError;
	}
}

Light get_light_by_byte(char byte){
	switch(byte&(0x3)){
		case 0:
			return Dark;
		case 1:
			return Green;
		case 2:
			return Yellow;
		case 3:
			return Red;
		default:
			return -1;
	}
}

long get_likelyEndTime_by_bytes(char byte1, char byte2){
	long time = 0;
	time =(byte1&(0XF0))*1000;
	time+=(byte1&(0X0F))*100;
	time+=(byte2&(0XF0))*10;
	time+=(byte2&(0X0F));
	return time;
}

int checkd_checksum_by_bytes(char byte1,char byte2,char byte3,char byte4){	
	char checkend=0x7f&(byte2^byte3^byte4);
	printf("checkend=%d",checkend);
	if(byte1 != checkend){
		printf("校验错误\n");
		return -1;
	}
	else
		return 0;
}
