#include "stdafx.h"
#include "SweepMine.h"
#include "HowPlayGameDialog.h"


IMPLEMENT_DYNAMIC(CHowPlayGameDialog, CDialog)

CHowPlayGameDialog::CHowPlayGameDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CHowPlayGameDialog::IDD, pParent)
{
}

CHowPlayGameDialog::~CHowPlayGameDialog()
{
}

void CHowPlayGameDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHowPlayGameDialog, CDialog)
    ON_BN_CLICKED(IDC_I_KNOW, &CHowPlayGameDialog::OnBnClickedIKnow)
END_MESSAGE_MAP()


void CHowPlayGameDialog::OnBnClickedIKnow()
{
    CDialog::EndDialog(0);
}
