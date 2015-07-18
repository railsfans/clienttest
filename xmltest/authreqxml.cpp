#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
typedef enum MESSAGE_TYPE
{
    AUTH_REQ = 0,  // ADDE -> ADDC
    AUTH_RESP,  // ADDC -> ADDE

    UPDATE_FULL_PROGLIST_REQ, // ADDC -> ADDE
    UPDATE_FULL_PROGLIST_RESP, // ADDE -> ADDC

    UPDATE_ALL_GRP_INFO_REQ, // ADDC -> ADDE
    UPDATE_ALL_GRP_INFO_RESP, // ADDE -> ADDC
    
    GET_CAR_LIST_REQ,       // ADDE -> ADDC
    GET_CAR_LIST_RESP,       // ADDC -> ADDE
    
    GET_PLAY_LIST_REQ,      // ADDE -> ADDC
    GET_PLAY_LIST_RESP,      // ADDC -> ADDE

    GET_PPLAY_LIST_REQ,      // ADDE -> ADDC
    GET_PPLAY_LIST_RESP,      // ADDC -> ADDE
    
    GET_PROG_LIST_REQ,      // ADDE -> ADDC
    GET_PROG_LIST_RESP,      // ADDC -> ADDE    

    GET_TEXT_LIST_REQ,      // ADDE -> ADDC
    GET_TEXT_LIST_RESP,      // ADDC -> ADDE    

    GET_STA_PLAY_LIST_REQ,      // ADDE -> ADDC
    GET_STA_PLAY_LIST_RESP,      // ADDC -> ADDE    

    HEART_BEAT,      // ADDC -> ADDE, ADDL -> ADDE
    HEART_BEAT_RESP,      // ADDC -> ADDE, ADDL -> ADDE

    MONITOR_INFO,      // ADDE -> ADDL    

    UPDATE_SW_REQ,      // ADDUS -> ADDUC    
    UPDATE_SW_RESP,      // ADDUC -> ADDUS    

    UPDATE_SYS_PARAMS_REQ,
    UPDATE_SYS_PARAMS_RESP,

    UPLOAD_ED_INFOS_REQ,
    UPLOAD_ED_INFOS_RESP,

    QUERY_UPDATE_INFO_REQ,
    QUERY_UPDATE_INFO_RESP,

    POST_PLAY_LOG_REQ,
    POST_PLAY_LOG_RESP,

    POST_CAR_PARAMS_REQ,
    POST_CAR_PARAMS_RESP,

    POST_SYS_LOG_REQ,
    POST_SYS_LOG_RESP,

    REBOOT_STATION_REQ,
    REBOOT_STATION_RESP,

    UPDATE_CAR_PARAMS_REQ,
    UPDATE_CAR_PARAMS_RESP,    
    
    MESSAGE_TYPE_COUNT,

    UNKNOWN_CMD = 0xffff,
    
}MESSAGE_TYPE;
static char * MessageType[MESSAGE_TYPE_COUNT] = 
{
    "AUTH_REQ",
    "AUTH_DEVICE_RESP",
    "UPDATE_FULL_PROGLIST_REQ", 
    "UPDATE_FULL_PROGLIST_RESP",     
    "UPDATE_ALL_GRP_INFO_REQ",
    "UPDATE_ALL_GRP_INFO_RESP",
    "GET_CAR_LIST_REQ",
    "GET_CAR_LIST_RESP",
    "GET_PLAY_LIST_REQ",
    "GET_PLAY_LIST_RESP",
    "GET_PPLAY_LIST_REQ",
    "GET_PPLAY_LIST_RESP",
    "GET_PROG_LIST_REQ",
    "GET_PROG_LIST_RESP",    
    "GET_TEXT_LIST_REQ",
    "GET_TEXT_LIST_RESP",    
    "GET_STA_PLAY_LIST_REQ",
    "GET_STA_PLAY_LIST_RESP",    
    "HEART_BEAT",    
    "HEART_BEAT_RESP",        
    "MONITOR_INFOS",
    "UPDATE_SW_REQ",
    "UPDATE_SW_RESP",
    "UPDATE_SYS_PARAMS_REQ",
    "UPDATE_SYS_PARAMS_RESP",
    "UPLOAD_ED_INFOS_REQ",
    "UPLOAD_ED_INFOS_RESP",
    "QUERY_UPDATE_INFO_REQ",
    "QUERY_UPDATE_INFO_RESP",
    "POST_PLAY_LOG_REQ",
    "POST_PLAY_LOG_RESP",
    "POST_CAR_PARAMS_REQ",
    "POST_CAR_PARAMS_RESP",
    "POST_SYS_LOG_REQ",
    "POST_SYS_LOG_RESP",            
    "REBOOT_STATION_REQ",            
    "REBOOT_STATION_RESP",         
    "UPDATE_CAR_PARAMS_REQ",
    "UPDATE_CAR_PARAMS_RESP"    
};
#define __MAX_INT_CHARS 50 
static  char * itoa(int i) 
{ 
    static char a[__MAX_INT_CHARS]; 
    char *b = a + sizeof(a) - 1; 
    int sign = (i < 0); 

    if (sign) 
        i = -i; 
    *b = 0; 
    do 
    { 
        *--b = '0' + (i % 10); 
        i /= 10; 
    } while (i); 
    
    if (sign) 
        *--b = '-'; 
       
    return b; 
    
} 
xmlDocPtr AppXmlBuildHeader(MESSAGE_TYPE message_type, int sequence_number, int session_id)
{
    if (message_type >= MESSAGE_TYPE_COUNT)
    {
        printf("undefined message type [%d]\n", message_type);
        return NULL;
    }
    
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    if (doc == NULL)
    {
        printf("call xmlNewDoc failure\n");
        return NULL;
    }

    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"Message");

    xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "1.0");
    
    xmlDocSetRootElement(doc,root_node);

    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"Header");

    xmlNewProp( node ,BAD_CAST"Message_Type",BAD_CAST MessageType[message_type]);
    
    xmlNewProp( node ,BAD_CAST"Sequence_Number",BAD_CAST itoa(sequence_number));

    xmlNewProp( node ,BAD_CAST"Session_ID",BAD_CAST itoa(session_id));

    xmlAddChild(root_node,node);
    
    return doc;	
}
#define sequence_number 0
int main(int argc, char **argv)

{
    char* package_buf=NULL;
    int package_size=0;
    char device_id[50]="ADDE-86-00-00-00-00-0000";
    xmlDocPtr wdoc = AppXmlBuildHeader((MESSAGE_TYPE)AUTH_REQ, sequence_number, 0);
    if (wdoc == NULL)
    {
        printf("build the header failure of xml package\n");
        return -1;
    }

    xmlNodePtr root_node = xmlDocGetRootElement(wdoc);
    if (root_node == NULL)
    {
        printf("get the root of xml package\n");
        xmlFreeDoc(wdoc);
        return -1;
    }
    int result=1, errorcode=2, token=3;
     xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "RESULT");
    
    xmlNewProp( node ,BAD_CAST "Value",BAD_CAST itoa(result) );

    xmlNewProp( node ,BAD_CAST "ErrorCode",BAD_CAST itoa(errorcode) );

    xmlAddChild(root_node,node);

    node = xmlNewNode(NULL, BAD_CAST "TOKEN");
    
    xmlNewProp( node ,BAD_CAST "Value",BAD_CAST itoa(token) );

    xmlAddChild(root_node,node);
//    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "DEVICE");

//    xmlNewProp( node ,BAD_CAST "ID",BAD_CAST device_id );

//    xmlAddChild(root_node,node);
    xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", wdoc, "UTF-8", 1);
    xmlCleanupParser();
    /* 将生产的xml包保存到内存中 */
    xmlDocDumpMemory(wdoc, (xmlChar **)package_buf, &package_size);

    xmlFreeDoc(wdoc);

	return 1;
}
