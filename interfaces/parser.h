#include <LightState.h>


typedef enum phaseid{
	V_East = 0,
	V_South = 1,
	V_West = 2,
	V_North = 3,
	P_East = 4,
	P_South = 5,
	P_West = 6,
	P_North = 7,
	PhaseIdError = -1
}PhaseID;


typedef enum lightid{
	Dark = LightState_dark,
	Red = LightState_stop_And_Remain,
	Green = LightState_permissive_Movement_Allowed,
	Yellow = LightState_intersection_clearance
}Light;


// enum of data type
typedef enum type{
	T_ERROR = -1,
	T_SPAT = 0
} DataType;


typedef struct pha{
	PhaseID id; // 0-7
	Light light; // 0-3
	long likelyEndTime;
} Pha_t;


/* function for determining the received buffer contains which type of data.
 * the return value representing type which should be defined before.
 */
DataType determine_dataType(char* buffer);

/* parse bytes to Pha_t list or array.
 * return 0 for parse well, -1 otherwise
 */
int parse_spat(char *bytes, size_t no_bytes, Pha_t *phaList, const int len);

/* parse bytes to BSM ... todo
 */
int parse_bsm(void *bytes, size_t no_bytes, void todo);
int parse_rsm(void *bytes, size_t no_bytes, void todo);
int parse_map(void *bytes, size_t no_bytes, void todo);


/***********************************
 *  internal and useful functions  *
 ***********************************/

PhaseID get_phaseId_by_no(int no);

Light get_light_by_byte(char byte);

long get_likelyEndTime_by_bytes(char byte1,char byte2);

int checkd_checksum_by_bytes(char byte1,char byte2,char byte3,char byte4);