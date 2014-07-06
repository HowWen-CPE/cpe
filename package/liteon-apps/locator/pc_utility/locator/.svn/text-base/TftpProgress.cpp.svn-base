// TftpProgess.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "TftpProgress.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTftpProgress dialog


CTftpProgress::CTftpProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CTftpProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTftpProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create (IDD, NULL); // critical statement for modeless dialog
	ProcessPos = 0;
}


void CTftpProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTftpProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTftpProgress, CDialog)
	//{{AFX_MSG_MAP(CTftpProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTftpProgress message handlers

BOOL CTftpProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctlProgress.SetStep(1);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.SetPos(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTftpProgress::Close()
{
	DestroyWindow();
}

int CTftpProgress::TFTPput(char *host, char *file, char *passwd)
{
	int i1, len;
    char *bp;
    unsigned long delay, inidelay;
#if NTRACE >= 1
	DWORD l1;
#endif
    DWORD l2;
    short seq;
    short ackbuf[40];
	char *mp;
    FILE *ffp;
    SOCKET sd;
	struct hostent FAR	  *hp;	
    struct in_addr        addr;
	struct sockaddr_in    serv_addr, temp_addr;
	int nAddrSize = sizeof(SOCKADDR);
	char szFile[_MAX_PATH]; //, *pin_addr;
	DWORD lSize, lStepSize;
	UINT  i, nCurStep = 0;
    int    numsd;
    fd_set fdset;
    struct timeval tv;

	// assume the local and remote share the same filename.
	lstrcpy(szFile, file);
	//GetLocalFileName(szFile);
    if((ffp = fopen(szFile, "rb")) == NULL) {
		ErrorMsg(IDS_ERR_OPENFILE, szFile);
		return -1;
	}
	fseek(ffp, 0L, SEEK_END);
	lSize = ftell(ffp);
//	lStepSize = (DWORD) lSize / (100 / PROGRESS_STEP);
	lStepSize = (DWORD) lSize / 100;

	fseek(ffp, 0L, SEEK_SET);

	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
	   ErrorMsg(IDS_ERR_SOCK_OPENFAIL);
	   return -1;
	}

	if((addr.s_addr=inet_addr(host))==INADDR_NONE) {                  
	   if((hp = gethostbyname(host)) == NULL) {               
		  ErrorMsg(IDS_ERR_WINSOCK);
		  goto err1;
	   } 
	   memcpy((char *)&addr, (char *)hp->h_addr, hp->h_length);
	}
	
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family    = AF_INET;
    serv_addr.sin_port      = TFTP_PORT;
    serv_addr.sin_addr      = addr;
	
    ackbuf[0] = T_WRQ;
    bp = "octet";       // another chocie is "netascii" 
    len = wsprintf((char *)ackbuf+2, "%s%c%s%c%s", file, 0, bp, 0, passwd); // "file" is the server-side file.
    
	//pin_addr = inet_ntoa(addr);
	//inidelay = (100 + sum_ip(pin_addr))/64;
	// the value of inidelay will affect the tranfer efficiency, so we use the minimum delay time.
	// the transfer bytes per second will be 55788 bytes/sec
	inidelay = 50; 
    for (delay=inidelay; delay<MAXDELAY/2; delay<<=1) {
        if (sendto(sd, (const char FAR*)ackbuf, len+3, 0, (LPSOCKADDR)&serv_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			ErrorMsg(IDS_ERR_SEND);
			goto err2;
		}
		//SleepEx(delay, TRUE);
		tv.tv_sec  = delay / 1000;
		tv.tv_usec = (delay % 1000) * 1000;
		FD_ZERO(&fdset);
		FD_SET(sd, &fdset);
		numsd = select(1, &fdset, 0, 0, &tv);
		if(numsd == 0)
		   continue;
		else if(numsd < 0) {
			ErrorMsg(IDS_ERR_RECV);
			goto err2;
		}
		i1 = recvfrom(sd, (char *)ackbuf, sizeof(ackbuf), 0, (LPSOCKADDR)&temp_addr, &nAddrSize) != SOCKET_ERROR;
		if (i1) {
			if(ackbuf[0] == T_ACK)
			   goto lab3;
			if(ackbuf[0] == T_ERROR) {
               if(ackbuf[1] == E_FILENAME)
				  ErrorMsg(IDS_ERR_FILENAME);
			   else if(ackbuf[1] == E_PASSWORD)
				  ErrorMsg(IDS_ERR_PASSWORD);
			   goto err1;
			}
		}
    }
    goto err1;
lab3:
#if NTRACE >= 1
    l1 = GetTickCount();
#endif

    if ((mp = (char *)malloc(516)) == 0)
		goto err1;
    bp = (char *)mp;
    *(short *)bp = T_DATA;

    seq = 1;
    for (l2=0; ; ) {
        if((len = fread(bp+4, 1, 512, ffp)) < 0)
            break;
        *(short *)(bp+2) = htons(seq);
        l2 += len;		

        for (delay=inidelay; delay<MAXDELAY; delay<<=1) {
			if(sendto(sd, (char*)bp, len+4, 0, (LPSOCKADDR)&serv_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			   ErrorMsg(IDS_ERR_SEND);
			   break;
			}
			//SleepEx(delay, TRUE);
			tv.tv_sec  = delay / 1000;
			tv.tv_usec = (delay % 1000) * 1000;
			FD_ZERO(&fdset);
			FD_SET(sd, &fdset);
			numsd = select(1, &fdset, 0, 0, &tv);
			if(numsd == 0)
			   continue;
			else if(numsd < 0) {
				ErrorMsg(IDS_ERR_RECV);
				goto err2;
			}
			memset((void *)ackbuf, 0, sizeof(ackbuf));
			i1 = recvfrom(sd, (char *)ackbuf, sizeof(ackbuf), 0, (LPSOCKADDR)&temp_addr, &nAddrSize) != SOCKET_ERROR;
            if (i1) {
			   if(ackbuf[0] == T_ACK && ntohs(ackbuf[1]) == seq)
				  goto lab2;
			   if(ackbuf[0] == T_ERROR && ackbuf[1] == E_UPDATEDATA) {
				  ErrorMsg(IDS_ERR_UPGRADE);
				  break;
			}
		}
	}
  	free(mp);
	goto err1;


lab2: seq++;
	  for(i=nCurStep; i<l2/lStepSize; i++) {
		m_ctlProgress.StepIt();
		nCurStep++;
	  }
	  if (len < 512) {
		 m_ctlProgress.SetPos(100);
         break;
	  }
    }
    free(mp);
#if NTRACE >= 1
    l1 = GetTickCount() - l1;
    if (l1 && l2) {
       wsprintf(szFile, "TX %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1, (l2*1000)/l1);
	   AfxMessageBox(szFile);	
	}
#endif

	closesocket(sd);
    fclose(ffp);
    return 0;

err1:
	closesocket(sd);
err2:
    fclose(ffp);
    return -1;
}

void CTftpProgress::PostNcDestroy() 
{
	delete this;	
}

