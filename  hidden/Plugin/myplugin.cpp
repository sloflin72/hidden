// myPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

/*
Plugin of miranda IM(ICQ) for Communicating with users of the baseProtocol. 
Copyright (C) 2004 Daniel Savi (dss@brturbo.com)

-> Based on J. Lawler BaseProtocol <-

Added:
 - plugin option window
 - make.bat (vc++)
 - tinyclib http://msdn.microsoft.com/msdnmag/issues/01/01/hood/default.aspx
 - C++ version

Miranda ICQ: the free icq client for MS Windows 
Copyright (C) 2000-2  Richard Hughes, Roland Rabien & Tristan Van de Vreede
*/
#include <string>
#include "Stegano.h"
#include "baseProtocol.h"
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>

#include <list>


//#include "Encoder.h"
//#include "Decoder.h"
//#include "Consts.h"


#define DO_DEBUG __asm int 3
using namespace std;
ofstream gLogFile;

HINSTANCE hinstance;
PLUGINLINK *pluginLink;
HANDLE hHookEventAdd;
HANDLE hEnableMenu;
CLISTMENUITEM mi;
//HANDLE hHookRebuildMenu;
bool fEnabled;
int interval;
HANDLE hSndWndEvent;
char szModuleName[] = "myPlugin.dll";


typedef struct _MYCONTACTINFO {
	HANDLE hContact;
	HWND hWndDlg;
} MYCONTACTINFO, *PMYCONTACTINFO;

list<MYCONTACTINFO> *pHandleList;

HICON hIconSteno;
HICON hIconDisSteno;
HANDLE hServiceToggle, hMenuToggle;

#define DBSETTING_STENO_DISABLE "Disable stenography"

#define MODULE "myPlugin"
#define MS_STENO_TOGGLE 		MODULE "/StenoOnOff"

PLUGININFO pluginInfo={
		sizeof(PLUGININFO),
		"Steganography messanger",
		PLUGIN_MAKE_VERSION(0,0,0,1),
		"Steganography messanger",
		"Andrey Smetanin",
		"OriginalNull@gmail.com",
		"Copyright(C) 2007 Fizteh students",
		"http://code.google.com/p/hidden/",
		0,
		0
};




extern "C" __declspec(dllexport) PLUGININFO* MirandaPluginInfo(DWORD mirandaVersion)
{
/*	if (mirandaVersion < PLUGIN_MAKE_VERSION(0,4,0,0))
		return NULL;
	else
*/

		return &pluginInfo;
}

//=====================================================
// Unloads plugin
//=====================================================

extern "C" __declspec(dllexport)int Unload(void)
{

//	UnhookEvent(hHookEventAdd);
	if (pHandleList)
		delete pHandleList;

	if (hSndWndEvent)
		UnhookEvent(hSndWndEvent);
	gLogFile << "Unload" << endl;
	gLogFile.close();
	
	return 0;
}

//=====================================================
// WINAPI DllMain
//=====================================================

BOOL WINAPI DllMain(HINSTANCE hinst,DWORD fdwReason,LPVOID lpvReserved)
{
	hinstance = hinst;
	return TRUE;
}

void WriteCharLog(char *buffer, int size)
{
	gLogFile << "BEGIN MESSAGE" << endl;
	gLogFile << "Size is " << size << endl;
	for (int i = 0; i < size; i++) {
		gLogFile.put(buffer[i]);
	}
	gLogFile << "END MESSAGE" << endl;
}
void WriteLog(PBYTE Buffer, int Size)
{
	for (int i = 0; i < Size; i++) {
		gLogFile << (int)Buffer[i];
	}
	gLogFile << "end log" << endl;
}

WNDPROC oldEncProc;
WNDPROC oldDecProc;
WNDPROC oldClearProc;

char upperchar(char ch)
{
  if ((ch>='a') && (ch<='z'))
  {
      ch='A'+(ch - 'a');
      return ch;
   }
  else return ch;
};

#define SEPARATOR "*enter*"

HANDLE GetContactFromWnd(HWND hWnd)
{
	list<MYCONTACTINFO>::iterator it;

	
	gLogFile << "GetContactFromWnd " << (void *)pHandleList << endl;

	for (it = pHandleList->begin(); it!= pHandleList->end(); it++) {
		if (it->hWndDlg == hWnd) {
			return it->hContact;
		}
	}
	return NULL;
}

struct MessageWindowDataInternal
{
	HANDLE hContact;
	HANDLE hDbEventFirst, hDbEventLast;
	HANDLE hSendId;
	int sendCount;
	HBRUSH hBkgBrush;
	int splitterPos, originalSplitterPos;
	char *sendBuffer;
	SIZE minEditBoxSize;
	RECT minEditInit;
	int lineHeight;
	int windowWasCascaded;
	int nFlash;
	int nFlashMax;
	int nLabelRight;
	int nTypeSecs;
	int nTypeMode;
	int avatarWidth;
	int avatarHeight;
	int limitAvatarH;
	HBITMAP avatarPic;
	DWORD nLastTyping;
	int showTyping;
	HWND hwndStatus;
	DWORD lastMessage;
	char *szProto;
	WORD wStatus;
	WORD wOldStatus;
	void *cmdList;   //TCmdList *
	void *cmdListCurrent; //TCmdList *
	int bIsRtl, bIsFirstAppend, bIsAutoRTL;
	int lastEventType;
};



void RealSendMessage(HANDLE hContact, const char *buffer, int size) 
{
	int wait_time;
/*
	dat->sendBuffer = (char *)realloc(dat->sendBuffer, size*(sizeof(TCHAR) + 1));
	memcpy(dat->sendBuffer, buffer, size);
	dat->bIsRtl = 0;
*/
	gLogFile << "time begin " << time(NULL) << endl;
	wait_time = 500 + rand()%1000;
	gLogFile << "wait_time " << wait_time << endl;
	Sleep(wait_time);
	gLogFile << "time end " << time(NULL) << endl;

	CallContactService(hContact, PSS_MESSAGE, 0, (LPARAM)buffer);
	gLogFile << "send time " << time(NULL) << endl;
}

void SendEncodedMessage(string &message, HANDLE hContact) 
{
	string::size_type idx, idx_prev = 0;
	char *data;
	string oneMessage;

	srand(time(NULL));
	while ((idx = message.find(SEPARATOR, idx_prev)) != string::npos) {
		RealSendMessage(hContact, message.substr(idx_prev, idx - idx_prev).data(), idx - idx_prev);
		idx_prev = idx + strlen(SEPARATOR);			
	}

}

void Decode(string &instr, string & outstr) 
{
	outstr.clear();
	outstr.reserve(instr.length());

	for (int i = 0; i < instr.length(); i++) {
		outstr+= upperchar(instr[i]);
	}
}

#define IDC_MESSAGE 1002
#define IDC_LOG 1001

typedef struct _THREADINFO {
	char *Buffer;
	int BufferSize;
	HANDLE hContact;
} THREADINFO, *PTHREADINFO;

DWORD SendThread(LPVOID lpThreadParameter)
{
	string instr, outstr;
	PTHREADINFO ThreadInfo = (PTHREADINFO)lpThreadParameter;
	CStegano Stegano;

	gLogFile << "SendThread " << GetCurrentThreadId() << endl;
	instr.assign(ThreadInfo->Buffer, ThreadInfo->BufferSize);
	
	instr.erase(instr.size()-1, 1);
	Stegano.Encode(instr, outstr);
	SendEncodedMessage(outstr, ThreadInfo->hContact);
	free(ThreadInfo->Buffer);
	free(ThreadInfo);
	return 0;
}

void SendWorker(PTHREADINFO ThreadInfo)
{
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendThread, ThreadInfo, 0, NULL);
}

#define IDC_LOG                         1001




#define TIME_STAMP_END ": "

bool FindTimeStamp(string &str, string::size_type pos, string::size_type &end)
{
	end = str.find(TIME_STAMP_END, pos);
	if (end == string::npos) return false;
	end+= 2;
	return true;
}

void ParseLogData(string &logdata, string &out)
{
	string temp;
	string::size_type begin, end, posr;
	begin = 0;
	temp = logdata;
	bool first = true;
	while (FindTimeStamp(temp, begin, end)) {
		temp.erase(begin, end - begin);
		if (!first) {
			temp.insert(begin, SEPARATOR);
			begin+= strlen(SEPARATOR);
		} else {
			first = false;
		}

		end = begin;
		posr = temp.find("\r\n", begin);
		begin = temp.find("\r\n", posr + 2);
		
		if (posr != string::npos) {
			if ((begin == string::npos) || (begin != posr + 2)) {
				// "\r\n"
				temp.erase(posr, 2);
				begin = posr;
			} else {
				// "\r\n\r\n"
				temp.erase(begin, 2);
				temp.erase(posr, 1);
				begin = posr + 1;
			}
		} else {
			begin = end;
		}
	}
	
	begin = temp.rfind(SEPARATOR);
	if ((temp.length() - begin) == strlen(SEPARATOR)) {
		temp.erase(begin, temp.length() - begin);
	}
	out = temp;
}

LRESULT DecWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	HWND hParentWnd, hLogWnd, hWndEdit;
	struct MessageWindowDataInternal *dat = NULL;
	PTHREADINFO ThreadInfo = (PTHREADINFO)malloc(sizeof(THREADINFO));
	string out, parseout, finalout;
	CStegano Stegano;
	if (uMsg == WM_LBUTTONDOWN) {

		gLogFile << "DecWndProc my button down" << endl;
		hParentWnd = GetParent(hWnd);
		hLogWnd = GetDlgItem(hParentWnd, IDC_LOG);
		
		int size = GetWindowTextLengthA(hLogWnd) + 1;
		if (size > 1) {
			char *buffer = (char *)malloc(size);
//			DO_DEBUG;
			GetWindowTextA(hLogWnd, buffer, size);
			out.assign(buffer, size);
			gLogFile << "incoming message" << endl << out << endl << "end out" << endl;
			
			ParseLogData(out, parseout);
			
			gLogFile << "parsed message" << endl << parseout << endl << "end parsed message" << endl;

//			DO_DEBUG;
			
			Stegano.Decode(parseout, finalout);
//			DO_DEBUG;
			//DECODE!!!!
//			CreateWindowW("Edit", "DecodedMessage", dwStyle, 100, 100, 100, 100, NULL, NULL, hinstance, NULL);
			
			hWndEdit = CreateWindow(L"edit", L"Decoded message", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER,
								100, 100, 200, 60, NULL, NULL, hinstance, NULL);
			
			lResult = SendMessage(hWndEdit, EM_SETREADONLY, TRUE, 0);  

			SetWindowTextA(hWndEdit, finalout.c_str());
			
			gLogFile << "Edit handle " << hWndEdit << endl; 
//			MessageBoxA(hParentWnd, parseout.c_str(), "DecodeMessage", MB_TOPMOST);
			
		
			free(buffer);
		}
	}

	return CallWindowProcW(oldClearProc, hWnd, uMsg, wParam, lParam);
}

LRESULT ClearWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lResult;
	HWND hParentWnd, hLogWnd;

	if (uMsg == WM_LBUTTONDOWN) {

		gLogFile << "ClearWndProc my button down" << endl;
		hParentWnd = GetParent(hWnd);
		hLogWnd = GetDlgItem(hParentWnd, IDC_LOG);
		if (SetWindowText(hLogWnd, NULL) == NULL) {
			gLogFile << "ClearWndProc SetWindowText error " << GetLastError() << endl;
		}
	}

	return CallWindowProcW(oldClearProc, hWnd, uMsg, wParam, lParam);
}

LRESULT EncWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lResult;
	HWND hParentWnd, hEdit;
	struct MessageWindowDataInternal *dat = NULL;
	PTHREADINFO ThreadInfo = (PTHREADINFO)malloc(sizeof(THREADINFO));

	if (uMsg == WM_LBUTTONDOWN) {

		gLogFile << "EncWndProc my button down" << endl;
		hParentWnd = GetParent(hWnd);

		dat = (struct MessageWindowDataInternal *)GetWindowLong(hParentWnd, GWL_USERDATA);

		if (!dat) {
			return CallWindowProcW(oldEncProc, hWnd, uMsg, wParam, lParam);
		}

		if (hParentWnd) {
			hEdit = GetDlgItem(hParentWnd, IDC_MESSAGE);
			
			ThreadInfo->BufferSize = GetWindowTextLengthA(hEdit) + 1;
			
			if (ThreadInfo->BufferSize > 1) {
				ThreadInfo->Buffer = (char *)malloc(ThreadInfo->BufferSize*sizeof(char)); 
				ThreadInfo->hContact = dat->hContact;

				if (GetWindowTextA(hEdit, ThreadInfo->Buffer, ThreadInfo->BufferSize)) {
					SendWorker(ThreadInfo);					
				} else {
					free(ThreadInfo->Buffer);
					free(ThreadInfo);
				}
			}
		}
	}

	return CallWindowProcW(oldEncProc, hWnd, uMsg, wParam, lParam);
}

#define BUTTON_SIZE_X 26
#define BUTTON_SIZE_Y 24
#define ENCODE_BUTTON_NAME L"En"
#define DECODE_BUTTON_NAME L"De"
#define CLEAR_BUTTON_NAME L"CL"
#define XSHIFT 200
int RenderWindowEvent(WPARAM wParam, LPARAM lParam)
{
	MYCONTACTINFO ContactInfo;
	MessageWindowEventData *hEventHandle;
	StatusIconData IconData = {0};
	int Result, newBlobSize, oldBlobSize;
	HANDLE hContact;
	HANDLE hDbEvent;
	HWND hWndDlg, hWndEnc, hWndDec, hWndClear;
	RECT Rect;

	hEventHandle = (MessageWindowEventData *)lParam;
	
	if(hEventHandle->uType == MSG_WINDOW_EVT_OPEN) {
	
		hWndDlg = hEventHandle->hwndWindow;

		hWndEnc = CreateWindowEx(0, L"button", ENCODE_BUTTON_NAME, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			XSHIFT, 2, BUTTON_SIZE_X, BUTTON_SIZE_Y, hWndDlg, NULL, hinstance, NULL);
	
		hWndDec = CreateWindowEx(0, L"button", DECODE_BUTTON_NAME, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			XSHIFT + BUTTON_SIZE_X, 2, BUTTON_SIZE_X, BUTTON_SIZE_Y, hWndDlg, NULL, hinstance, NULL);
		
		hWndClear = CreateWindowEx(0, L"button", CLEAR_BUTTON_NAME, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			XSHIFT + 2*BUTTON_SIZE_X, 2, BUTTON_SIZE_X, BUTTON_SIZE_Y, hWndDlg, NULL, hinstance, NULL);
		
		ContactInfo.hContact = hEventHandle->hContact;
		ContactInfo.hWndDlg = hWndDlg;

		pHandleList->push_front(ContactInfo);
		oldEncProc = (WNDPROC)SetWindowLongW(hWndEnc, GWL_WNDPROC, (LONG)(EncWndProc));
		oldClearProc = (WNDPROC)SetWindowLongW(hWndClear, GWL_WNDPROC, (LONG)(ClearWndProc));
		oldDecProc = (WNDPROC)SetWindowLongW(hWndDec, GWL_WNDPROC, (LONG)(DecWndProc));
	}

	if(hEventHandle->uType == MSG_WINDOW_EVT_CLOSE) {

		hWndDlg = hEventHandle->hwndWindow;
		list<MYCONTACTINFO>::iterator it;
		
		gLogFile << (void *)pHandleList << endl;
		
		for (it = pHandleList->begin(); it != pHandleList->end(); it++) {
			if (it->hWndDlg == hWndDlg) {
				pHandleList->erase(it);
				break;
			}
		}
	}
	return 0;
}

int MainInit(WPARAM wParam, LPARAM lParam)
{
	gLogFile << "MainInit" << endl;
	
	if(GetModuleHandle(L"SRMM")) {

		hSndWndEvent = HookEvent(ME_MSG_WINDOWEVENT, RenderWindowEvent);

	}
	return 0;
}


//=====================================================
// Called when plugin is loaded into Miranda
//=====================================================
extern "C" int __declspec(dllexport)Load(PLUGINLINK *link)
{
	
	gLogFile.open("c:\\mypluginLog.txt");
	gLogFile << "Load" << endl;

	pluginLink = link;

	hIconSteno = NULL;
	hIconDisSteno = NULL;
	hServiceToggle = NULL;
	hMenuToggle = NULL;

	DBVARIANT dbv;
	HANDLE hContact = ( HANDLE )CallService( MS_DB_CONTACT_FINDFIRST, 0, 0 );
	
	pHandleList = new list<MYCONTACTINFO>;
	
	gLogFile << "PRIVED" << endl;
	gLogFile << "pHandleList " << (void *)pHandleList << endl;

	HookEvent(ME_SYSTEM_MODULESLOADED,MainInit);

	return 0;
}
