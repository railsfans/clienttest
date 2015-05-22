#include <stdio.h>
//#include <hiredis/hiredis.h>
#include  "hiredis/hiredis.h"

int main()
{
redisContext *conn = redisConnect("127.0.0.2", 6379); 
if(conn->err){
printf("connection error: %s\n", conn->errstr);
}
printf("%d,connect success\n",conn->err);
return 1;
}
