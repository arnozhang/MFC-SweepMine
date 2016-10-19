#include "stdafx.h"
#include "SweepMine.h"
#include "AuthorDialog.h"


IMPLEMENT_DYNAMIC(CAuthorDialog, CDialog)

CAuthorDialog::CAuthorDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CAuthorDialog::IDD, pParent)
{
}

CAuthorDialog::~CAuthorDialog()
{
}

void CAuthorDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAuthorDialog, CDialog)
END_MESSAGE_MAP()


