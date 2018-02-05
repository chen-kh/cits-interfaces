#include <sys/types.h>  
#include <netinet/in.h>  
#include <netdb.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <arpa/inet.h>


/* function for receiving socket to buffer pre-allocated with para len specified
 * return 0 for receive well, -1 if failed.
 */
int revc_socket(char* buffer, const int len);

/* check if data is damaged
 */
int check_valid(char* bytes);

/* recv spat socket
 * return 0 for success, -1 otherwise
 */
int recv_spat(char* buffer, size_t no_bytes);


/* test function for broadccasting spat data
 */
int udp_broadcast();