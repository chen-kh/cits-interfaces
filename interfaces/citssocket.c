#include <sys/types.h>  
#include <netinet/in.h>  
#include <netdb.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "citssocket.h"
#include "for_debug.h"


int recv_socket(char *buffer,const int len_buf){
    int sockListen;  
    if((sockListen = socket(AF_INET, SOCK_DGRAM, 0)) == -1){  
        CITS_DEBUG("socket fail");  
        return -1;  
    }  
    int set = 1;  
    setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));  
    struct sockaddr_in recvAddr;  
    memset(&recvAddr, 0, sizeof(struct sockaddr_in));
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(4001);
    recvAddr.sin_addr.s_addr = INADDR_ANY;
    // 必须绑定，否则无法监听  
    if(bind(sockListen, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){  
        CITS_DEBUG("bind fail");  
        return -1;  
    }
    int recvbytes;
    int addrLen = sizeof(struct sockaddr_in); 
    recvbytes = recvfrom(sockListen, buffer, len_buf, 0, (struct sockaddr *)&recvAddr, &addrLen);
    if(recvbytes!= -1){  
        CITS_DEBUG("receive a broadCast messgse:%s\n",buffer);  
    }else{  
        CITS_DEBUG("recvfrom fail");
        return -1;    
    }  
    close(sockListen);  
    return 0;  
}  


int udp_broadcast(){  
    char msg[] = {0xEE,0xBB,0x00,0xC7,0xFE,0x01,0x00,0x06,0x07,0xFE,0x07,
        0x00,0x06,0x01,0xFE,0x09,0x00,0x06,0x0F,0xFE,0x0F,0x00,0x06,0x09,0xFE,0x13,
        0x00,0x06,0x15,0xFE,0x15,0x00,0x06,0x13,0xFE,0x1B,0x00,0x06,0x1D,0xFE,0x1D,0x00,0x06,0x1B,0x0A};  
    int brdcFd;
    if((brdcFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1){  
        CITS_DEBUG("socket fail");  
        return -1;  
    }  
    int optval = 1;//这个值一定要设置，否则可能导致sendto()失败  
    setsockopt(brdcFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
    struct sockaddr_in theirAddr;
    memset(&theirAddr, 0, sizeof(struct sockaddr_in));
    theirAddr.sin_family = AF_INET;
    theirAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    theirAddr.sin_port = htons(4001);
    int sendBytes;
    sendBytes = sendto(brdcFd, msg, 45, 0,(struct sockaddr *)&theirAddr, sizeof(struct sockaddr));
    if(sendBytes == -1){  
        CITS_DEBUG("sendto fail");
        return -1;
    }
    CITS_DEBUG("msg=%x\n", msg[2]);
    close(brdcFd);
    return 0;
}
