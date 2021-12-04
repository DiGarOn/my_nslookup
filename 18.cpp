#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

/*
struct addrinfo {
	int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc. 
	int ai_family; // AF_INET, AF_INET6, AF_UNSPEC 
	int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
	int ai_protocol; // use 0 for "any"
	size_t ai_addrlen; // size of ai_addr in bytes 
	struct sockaddr *ai_addr; // 
	struct sockaddr_in or _in6 char *ai_canonname; // full canonical hostname
	struct addrinfo *ai_next; // linked list, next node 
};
*/

/*
int getaddrinfo(const char *node, // e.g. "www.example.com" or IP 
				const char *service, // e.g. "http" or port number
				const struct addrinfo *hints,
				struct addrinfo **res);
*/

int main(int argc, char *argv[]) {
	addrinfo hints, *res, *i;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if(argc != 2) {
		cout << "error\n";
		return 1;
	}
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 		// AF_INET or AF_INET6 to force version 
	hints.ai_socktype = SOCK_STREAM;	
	
	//обработка ошибки получения данных  
	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) { 
		cout << "getaddrinfo: " << gai_strerror(status) << "\n"; 
		return 2;
	}
  
  	cout << "IP addresses for " << argv[1] << ": \n";

	
	for(i = res;i != NULL; i = i->ai_next) { 
		void *addr;
 		string ips;
		if (i->ai_family == AF_INET) { // IPv4
			sockaddr_in *ipv4 = (sockaddr_in *)i->ai_addr; 
			addr = &(ipv4->sin_addr);
			ips = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)i->ai_addr; 
			addr = &(ipv6->sin6_addr);
			ips = "IPv6";
		}
        // convert the IP to a string and print it:
		inet_ntop(i->ai_family, addr, ipstr, sizeof ipstr);
        cout <<  ips << " : " << ipstr << "\n";
    }
	freeaddrinfo(res); // free the linked list return 0;

	return 0;
}