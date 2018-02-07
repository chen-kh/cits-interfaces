#ifndef	_CITSSOCKET_H_
#define	_CITSSOCKET_H_

/* function for receiving socket to buffer pre-allocated with para len specified
 * return 0 for receive well, -1 if failed.
 */
int recv_socket(char* buffer, const int len);

/* check if data is damaged
 */
int check_valid(char* bytes);

/* recv spat socket
 * return 0 for success, -1 otherwise
 */
int recv_spat(char* buffer, size_t no_bytes);

/* to do
 */
int udp_broadcast_spat(char* buffer, const int len);

/***********************************
 *       functions for test        *
 ***********************************/
int udp_broadcast();

#endif