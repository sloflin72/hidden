// ---------------------------------------------------------------------------80
//                ICQ plugin for Miranda Instant Messenger
//                ________________________________________
// 
// Copyright � 2000,2001 Richard Hughes, Roland Rabien, Tristan Van de Vreede
// Copyright � 2001,2002 Jon Keating, Richard Hughes
// Copyright � 2002,2003,2004 Martin  berg, Sam Kothari, Robert Rainwater
// Copyright � 2004,2005,2006 Joe Kucera
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// -----------------------------------------------------------------------------
//
// File name      : $Source: /cvsroot/miranda/miranda/protocols/IcqOscarJ/m_icq.h,v $
// Revision       : $Revision: 4113 $
// Last change on : $Date: 2006-10-29 17:29:39 -0400 (Sun, 29 Oct 2006) $
// Last change by : $Author: ghazan $
//
// DESCRIPTION:
//
//  Describe me here please...
//
// -----------------------------------------------------------------------------

#ifndef M_ICQ_H__
#define M_ICQ_H__ 1


// Note: In v0.3 the part before "/Servicename" is dynamic. It will be the name of the protocol.
// Example: If the plugin was loaded from ICQ.dll, the service name is "ICQ/Servicename", and if
// the dll was Icq2.dll, the service name will be "Icq2/Servicename". This behaviour is temporary
// until proper multiaccounts are implemented.


//start a search of all ICQ users by e-mail
//wParam=0
//lParam=(LPARAM)(const char*)email
//returns a handle to the search on success, NULL on failure
//Results are returned using the same scheme documented in PSS_BASICSEARCH
//**DEPRECATED** in favour of PS_SEARCHBYEMAIL
typedef struct {   //extended search result structure, used for all searches
  PROTOSEARCHRESULT hdr;
  DWORD uin;
  BYTE auth;
  char* uid;
} ICQSEARCHRESULT;
#define MS_ICQ_SEARCHBYEMAIL   "/SearchByEmail"

//start a search of all ICQ users by details
//wParam=0
//lParam=(LPARAM)(ICQDETAILSSEARCH*)&ids
//returns a handle to the search on success, NULL on failure
//Results are returned using the same scheme documented in PSS_BASICSEARCH
//**DEPRECATED** in favour of PS_SEARCHBYNAME
typedef struct {
  char *nick;
  char *firstName;
  char *lastName;
} ICQDETAILSSEARCH;
#define MS_ICQ_SEARCHBYDETAILS   "/SearchByDetails"

// Request authorization
// wParam=(WPARAM)hContact
#define MS_REQ_AUTH "/ReqAuth"

// Grant authorization
// wParam=(WPARAM)hContact;
#define MS_GRANT_AUTH "/GrantAuth"

// Revoke authorization
// wParam=(WPARAM)hContact
#define MS_REVOKE_AUTH "/RevokeAuth"

// Display XStatus detail (internal use only)
// wParam=(WPARAM)hContact;
#define MS_XSTATUS_SHOWDETAILS "/ShowXStatusDetails"

//Send an SMS via the ICQ network
//wParam=(WPARAM)(const char*)szPhoneNumber
//lParam=(LPARAM)(const char*)szMessage
//Returns a HANDLE to the send on success, or NULL on failure
//szPhoneNumber should be the full number with international code and preceeded
//by a +

//When the server acks the send, an ack will be broadcast:
// type=ICQACKTYPE_SMS, result=ACKRESULT_SENTREQUEST, lParam=(LPARAM)(char*)szInfo
//At this point the message is queued to be delivered. szInfo contains the raw
//XML data of the ack. Here's what I got when I tried:
//"<sms_response><source>airbornww.com</source><deliverable>Yes</deliverable><network>BT Cellnet, United Kingdom</network><message_id>[my uin]-1-1955988055-[destination phone#, without +]</message_id><messages_left>0</messages_left></sms_response>\r\n"

//Now the hProcess has been deleted. The only way to track which receipt
//corresponds with which response is to parse the <message_id> field.

//At a (possibly much) later time the SMS will have been delivered. An ack will
//be broadcast:
// type=ICQACKTYPE_SMS, result=ACKRESULT_SUCCESS, hProcess=NULL, lParam=(LPARAM)(char*)szInfo
//Note that the result will always be success even if the send failed, just to
//save needing to have an attempt at an XML parser in the ICQ module.
//Here's the szInfo for a success:
//"<sms_delivery_receipt><message_id>[my uin]-1--1461632229-[dest phone#, without +]</message_id><destination>[dest phone#, without +]</destination><delivered>Yes</delivered><text>[first 20 bytes of message]</text><submition_time>Tue, 30 Oct 2001 22:35:16 GMT</submition_time><delivery_time>Tue, 30 Oct 2001 22:34:00 GMT</delivery_time></sms_delivery_receipt>"
//And here's a failure:
//"<sms_delivery_receipt><message_id>[my uin]-1-1955988055-[destination phone#, without leading +]</message_id><destination>[destination phone#, without leading +]</destination><delivered>No</delivered><submition_time>Tue, 23 Oct 2001 23:17:02 GMT</submition_time><error_code>999999</error_code><error><id>15</id><params><param>0</param><param>Multiple message submittion failed</param></params></error></sms_delivery_receipt>"

//SMSes received from phones come through this same ack, again to avoid having
//an XML parser in the protocol module. Here's one I got:
//"<sms_message><source>MTN</source><destination_UIN>[UIN of recipient, ie this account]</destination_UIN><sender>[sending phone number, without +]</sender><senders_network>[contains one space, because I sent from ICQ]</senders_network><text>[body of the message]</text><time>Fri, 16 Nov 2001 03:12:33 GMT</time></sms_message>"
#define ICQACKTYPE_SMS      1001
#define ICQEVENTTYPE_SMS    2001    //database event type
#define MS_ICQ_SENDSMS      "/SendSMS" 

//e-mail express
//db event added to NULL contact
//blob format is:
//ASCIIZ    text, usually of the form "Subject: %s\r\n%s"
//ASCIIZ    from name
//ASCIIZ    from e-mail
#define ICQEVENTTYPE_EMAILEXPRESS 2002  //database event type

//www pager
//db event added to NULL contact
//blob format is:
//ASCIIZ    text, usually "Sender IP: xxx.xxx.xxx.xxx\r\n%s"
//ASCIIZ    from name
//ASCIIZ    from e-mail
#define ICQEVENTTYPE_WEBPAGER   2003    //database event type

//for server-side lists, used internally only
//hProcess=dwSequence
//lParam=server's error code, 0 for success
#define ICQACKTYPE_SERVERCLIST  1003

//for rate warning distribution (mainly upload dlg)
//hProcess=Rate class ID
//lParam=server's status code
#define ICQACKTYPE_RATEWARNING  1004

//received Xtraz Notify response
//hProcess=dwSequence
//lParam=contents of RES node
#define ICQACKTYPE_XTRAZNOTIFY_RESPONSE 1005

//received Custom Status details response
//hProcess=dwSequence
//lParam=0
#define ICQACKTYPE_XSTATUS_RESPONSE 1006


//Update user details on server
//Permited operation types:
#define CIXT_BASIC      0x0001
#define CIXT_MORE       0x0002
#define CIXT_WORK       0x0004
#define CIXT_CONTACT    0x0008
#define CIXT_LOCATION   0x0010
#define CIXT_BACKGROUND 0x0020
#define CIXT_FULL       0x003F
//wParam=operationType
#define PS_CHANGEINFOEX "/ChangeInfoEx"

//Change nickname in White pages
//lParam=(LPARAM)(const char*)szNewNickName
#define PS_SET_NICKNAME "/SetNickname"

//Set password for current session
//lParam=(LPARAM)(const char*)szPassword
#define PS_ICQ_SETPASSWORD "/SetPassword"

//miranda/icqoscar/statusmsgreq event
//called when our status message is requested
//wParam=(BYTE)msgType
//lParam=(DWORD)uin
//msgType is one of the ICQ_MSGTYPE_GET###MSG constants in icq_constants.h
//uin is the UIN of the contact requesting our status message
#define ME_ICQ_STATUSMSGREQ      "/StatusMsgReq"


//set owner avatar
//wParam=0
//lParam=(const char *)Avatar file name
//return=0 for success
#define PS_ICQ_SETMYAVATAR "/SetMyAvatar"

//get current owner avatar
//wParam=(char *)Buffer to file name
//lParam=(int)Buffer size
//return=0 for success
#define PS_ICQ_GETMYAVATAR "/GetMyAvatar"

//get size limit for avatar image
//wParam=(int *)max width of avatar - will be set
//lParam=(int *)max height of avatar - will be set
//return=0 for success
#define PS_ICQ_GETMYAVATARMAXSIZE "/GetMyAvatarMaxSize"

//check if image format supported for avatars
//wParam = 0
//lParam = PA_FORMAT_*   // avatar format
//return = 1 (supported) or 0 (not supported)
#define PS_ICQ_ISAVATARFORMATSUPPORTED "/IsAvatarFormatSupported"


/* Custom Status helper API *
 - to set custom status message & title use PS_ICQ_GETCUSTOMSTATUS to obtain
   DB settings and write values to them (UTF-8 strings best). (obsolete)
 - use PS_ICQ_GETCUSTOMSTATUSEX and PS_ICQ_SETCUSTOMSTATUSEX for controling Custom Status
 - custom messages for each user supported - ME_ICQ_STATUSMSGREQ with type MTYPE_SCRIPT_NOTIFY
 */
#define CSSF_MASK_STATUS    0x0001  // status member valid for set/get
#define CSSF_MASK_NAME      0x0002  // pszName member valid for set/get
#define CSSF_MASK_MESSAGE   0x0004  // pszMessage member valid for set/get
#define CSSF_DISABLE_UI     0x0040  // disable default custom status UI, wParam = bEnable
#define CSSF_DEFAULT_NAME   0x0080  // only with CSSF_MASK_NAME and get API to get default custom status name (wParam = status)
#define CSSF_STATUSES_COUNT 0x0100  // returns number of custom statuses in wParam, only get API
#define CSSF_STR_SIZES      0x0200  // returns sizes of custom status name & message (wParam & lParam members) in chars
#define CSSF_UNICODE        0x1000  // strings are in UCS-2

#if defined(_UNICODE)
  #define CSSF_TCHAR  CSSF_UNICODE
#else
  #define CSSF_TCHAR  0
#endif


typedef struct {
  int cbSize;         // size of the structure
  int flags;          // combination of CSSF_*
  int *status;        // custom status id
  union {
    char *pszName;    // buffer for custom status name
    TCHAR *ptszName;
    WCHAR *pwszName;
  };
  union {
    char *pszMessage; // buffer for custom status message
    TCHAR *ptszMessage;
    WCHAR *pwszMessage;
  };
  WPARAM *wParam;     // extra params, see flags
  LPARAM *lParam;
} ICQ_CUSTOM_STATUS;


// Sets owner current custom status (obsolete)
//wParam = (int)N   // custom status id (1-32)
//lParam = 0         
//return = N (id of status set) or 0 (failed - probably bad params)
#define PS_ICQ_SETCUSTOMSTATUS "/SetXStatus"

// Sets owner current custom status
//wParam = 0                          // reserved
//lParam = (ICQ_CUSTOM_STATUS*)pData  // contains what to set and new values
//return = 0 (for success)
#define PS_ICQ_SETCUSTOMSTATUSEX "/SetXStatusEx"

// Retrieves custom status details for specified hContact
//wParam = (HANDLE)hContact
//lParam = (ICQ_CUSTOM_STATUS*)pData  // receives details (members must be prepared)
//return = 0 (for success)
#define PS_ICQ_GETCUSTOMSTATUSEX "/GetXStatusEx"

// Retrieves specified custom status icon
//wParam = (int)N  // custom status id (1-32), 0 = my current custom status
//lParam = 0
//return = HICON   // custom status icon (use DestroyIcon to release resources)
#define PS_ICQ_GETCUSTOMSTATUSICON "/GetXStatusIcon"

// Get Custom status DB field names & current owner custom status (obsolete)
//wParam = (char**)szDBTitle // will receive title DB setting name (do not free)
//lParam = (char**)szDBMsg   // will receive message DB setting name
//return = N  // current custom status id if successful, 0 otherwise
#define PS_ICQ_GETCUSTOMSTATUS "/GetXStatus"

// Request Custom status details (messages) for specified contact
//wParam = hContact  // request custom status details for this contact
//lParam = 0  
//return = (int)dwSequence   // if successful it is sequence for ICQACKTYPE_XSTATUS_RESPONSE
                             // 0 failed to request (e.g. auto-request enabled)
                             // -1 delayed (rate control) - sequence unknown
#define PS_ICQ_REQUESTCUSTOMSTATUS "/RequestXStatusDetails"

// Called when contact changes custom status and extra icon is set to clist_mw
//wParam = hContact    // contact changing status
//lParam = hIcon       // HANDLE to clist extra icon set as custom status
#define ME_ICQ_CUSTOMSTATUS_EXTRAICON_CHANGED "/XStatusExtraIconChanged"

#endif // M_ICQ_H__
