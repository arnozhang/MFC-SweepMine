#include "stdafx.h"
#include "SweepMine.h"
#include "WinedPlayer.h"


IMPLEMENT_DYNAMIC(CWinedPlayer, CDialog)

CWinedPlayer::CWinedPlayer(CWnd* pParent /*=NULL*/)
    : CDialog(CWinedPlayer::IDD, pParent)
{
}

CWinedPlayer::~CWinedPlayer()
{
}

void CWinedPlayer::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWinedPlayer, CDialog)
    ON_BN_CLICKED(IDOK, &CWinedPlayer::OnBnClickedOk)
END_MESSAGE_MAP()


void CWinedPlayer::OnBnClickedOk()
{
    GetDlgItem(IDC_WINED_PLAYER_NAME)->GetWindowText(m_szWinedPlayerName);
    if (m_szWinedPlayerName == _T(""))
    {
        MessageBox(_T("ÇëÊäÈëÃû×Ö!"), _T("´íÎó"), MB_OK | MB_ICONWARNING);
        GetDlgItem(IDC_WINED_PLAYER_NAME)->SetFocus();
        return ;
    }

    CDialog::EndDialog(IDOK);
}
