#ifndef __WOCHATDEF_H__
#define __WOCHATDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct XChatMessage
{
	struct XChatMessage* next;
	struct XChatMessage* prev;
	uint32_t  id;
	uint32_t* icon;     // the bitmap data of this icon
	uint8_t   w;        // the width in pixel of this icon
	uint8_t   h;        // the height in pixel of this icon
	int  height;   // in pixel
	int  width;    // in pixel
	uint16_t  state;
	uint64_t  ts;		// the time stamp. 
	uint16_t* name;      // The name of this people      
	uint16_t* message;  // real message
	uint16_t* textWrapTab; // the struct of this table is the same as _TextWrapIdxTab
	uint8_t* obj;       // point to GIF/APNG/Video/Pic etc
} XChatMessage;

typedef struct XChatGroup
{
	struct XChatGroup* next;
	uint16_t  id;			// Group ID
	uint32_t* icon;			// the bitmap data of this icon
	uint8_t   w;			    // the width in pixel of this icon
	uint8_t   h;			    // the height in pixel of this icon
	uint16_t  width;		    // in pixel
	uint16_t  height;		// in pixel
	uint16_t  unread;		// how many unread messages? if more than 254, use ... 
	uint16_t  member;		// how many members in this group?
	uint16_t* name;			// the group name
	uint64_t  ts;			// the time stamp. 
	uint16_t* tsText;			// the group name
	uint16_t* lastmsg;		// the last message of this group
	XChatMessage* headMessage;
	XChatMessage* tailMessage;
	MemoryContext mempool;
} XChatGroup;

typedef struct XMQTTMessage
{
	char* host;
	int   port;
	char* topic;
	char* message;
	int   msglen;
} XMQTTMessage;

#ifdef __cplusplus
}
#endif

#endif /* __WOCHATDEF_H__ */

