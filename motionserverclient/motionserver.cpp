#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include <arpa/inet.h> 
#include <unistd.h>
#define MAXLINE 4096  
uint32_t CalcCRC(uint8_t * msg, int32_t msgLen)
{
    uint32_t crc = 0;
    int i;

    for(i = 0; i< msgLen; i++)
    {
        crc ^= (*msg++);
    }

    return crc;
}
#define MSG_START_CODE 0xAAAAAAAA
#define MSG_START_CODE_FIRST_BYTE (MSG_START_CODE & 0XFF)
#define CRC_LEN sizeof(uint32_t)

#define CALC_MSG_LEN(pMsg) ( sizeof(MSG_HEADER_T) + *((uint32_t * )pMsg + 2) + CRC_LEN)
  
#define BUILD_REQ_MSG(cmdtype, sendbuf, bodybuf, bufLen) \
do \
{ \
   STATION_MSG_HEADER_HANDLE  pHeaderMsg = (STATION_MSG_HEADER_HANDLE)sendbuf; \
   pHeaderMsg->startCode = MSG_START_CODE; \
   pHeaderMsg->cmdType = cmdtype; \
   pHeaderMsg->bodyLen = bufLen; \
   if (bufLen > 0) \
       memcpy( (uint8_t *)sendbuf + sizeof(STATION_MSG_HEADER_T), bodybuf, bufLen); \
   *(uint32_t *)((uint8_t *)sendbuf + sizeof(STATION_MSG_HEADER_T) + bufLen) = CalcCRC(sendbuf, sizeof(STATION_MSG_HEADER_T) + bufLen); \
	respLen = sizeof(STATION_MSG_HEADER_T) + bufLen + CRC_LEN; \
}while(0)

#define BUILD_RESP_MSG(cmdtype, sendbuf, bodybuf, bufLen) \
do \
{ \
   STATION_MSG_HEADER_HANDLE pHeaderMsg = (STATION_MSG_HEADER_HANDLE)sendbuf; \
   pHeaderMsg->startCode = MSG_START_CODE; \
   pHeaderMsg->cmdType = cmdtype; \
   pHeaderMsg->bodyLen = bufLen; \
   if (bufLen > 0)  \
       memcpy( (uint8_t *)sendbuf + sizeof(STATION_MSG_HEADER_T), bodybuf, bufLen); \
   *(uint32_t *)((uint8_t *)sendbuf + sizeof(STATION_MSG_HEADER_T) + bufLen) = CalcCRC(sendbuf, sizeof(STATION_MSG_HEADER_T) + bufLen); \
   respLen = sizeof(STATION_MSG_HEADER_T) + bufLen + CRC_LEN; \
}while(0)


typedef enum CMD_TYPE_E
{
  
    /////////////////// MCU and NETWORK SERVER //////////////////
    //REQ OF SEVER TO MCU
    CMD_S2M_SET_SYS_TIME_REQ = 0x41,
    CMD_S2M_SET_STS_STATIONID_REQ,
    CMD_S2M_GET_LOG_REQ,

    //RESP OF SERVER TO MCU
    CMD_S2M_SET_SYS_TIME_RESP = 0x51,
    CMD_S2M_SET_STS_STATIONID_RESP,
    CMD_S2M_GET_LOG_RESP,

    // REQ OF MCU TO SERVER
    CMD_M2S_SAVE_EXERCISE_DATA_REQ = 0X61,
    CMD_M2S_HEART_REQ,
    CMD_M2S_BATTERY_REQ,
    CMD_M2S_LOGIN_REQ=0x64,

    // RESP OF MCU TO SERVER
    CMD_M2S_SAVE_EXERCISE_DATA_RESP = 0X71,
    CMD_M2S_HEART_RESP,
    CMD_M2S_BATTERY_RESP,
    CMD_M2S_LOGIN_RESP=0x74,
}CMD_TYPE_E;

typedef struct STATION_MSG_HEADER_T
{
    uint32_t startCode;
    CMD_TYPE_E cmdType;
    uint32_t bodyLen;
}STATION_MSG_HEADER_T;
typedef struct LOGIN_T
{
	int id;
	int password;
}LOGIN_T;
typedef struct STATION_MSG_HEADER_T * STATION_MSG_HEADER_HANDLE;
int main(int argc, char** argv)  
{ 
	int     sockfd, n,rec_len;  
 	char    recvline[4096], sendline[4096];  
 	char    buf[MAXLINE];  
  	struct  sockaddr_in    servaddr;  
           
	char m_data[200];
        int respLen  = 0;
        unsigned char * psendbuf = (unsigned char *)m_data; 
        LOGIN_T m_login_data={0xd2,0x12345678};
	BUILD_REQ_MSG(CMD_M2S_LOGIN_REQ, psendbuf, &m_login_data, sizeof(m_login_data));	
	memcpy(sendline, &m_data, respLen );
     
 	if( argc != 3){  
     	printf("usage: ./client <ipaddress> <port>\n");  
     		exit(0);  
     	}  
   
   
     	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
     		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
     		exit(0);  
     	}  
   
   
     	memset(&servaddr, 0, sizeof(servaddr));  
     	servaddr.sin_family = AF_INET;  
     	servaddr.sin_port = htons(atoi(argv[2]));  
     	if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){  
     		printf("inet_pton error for %s\n",argv[1]);  
     		exit(0);  
     	}  
   
   
     	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
     		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
     		exit(0);  
     	}  
   
     	printf("send msg to server: \n");  
     	int j;

	 
     	if( send(sockfd, sendline,sizeof(m_data), 0) < 0)  
  	{  
     		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
     		exit(0);  
  	}
        for(j=0;j<5;j++)
     	{	 
		memset(buf,0,MAXLINE*sizeof(char));
       		if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  
           		perror("recv error");  
 	          	exit(1);  
       		}
 		buf[rec_len]  = '\0';  
     		printf("Received : %0x\n ",buf);  
     		STATION_MSG_HEADER_T m_header;
     		memcpy(&m_header,buf,sizeof(m_header));
     		printf("%0x,%0x,%0x\n",m_header.startCode,m_header.cmdType,m_header.bodyLen);
     	}  
    
    	close(sockfd);  
    	exit(0);  
}  

