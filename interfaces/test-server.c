#include <stdio.h>
#include "citssocket.h"
#include <unistd.h>

int main(){
	do{
		udp_broadcast();
		sleep(3);// 滞停3s打印a
	}while(1);
	return 0;
}