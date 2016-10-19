#include "stdafx.h"
#include "SweepMine.h"
#include "SettingDialog.h"


IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CSettingDialog::IDD, pParent)
    , m_uWidthCount(0)
    , m_uHeighCount(0)
    , m_uMineCount(0)
{
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
    ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CSettingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL CSettingDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    GetDlgItem(IDC_WIDE_COUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_HEIGH_COUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_MINE_COUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_WIDE_COUNT)->SetFocus();

    return TRUE;
}

void CSettingDialog::OnBnClickedOk()
{
    UpdateData(TRUE);
    m_uWidthCount = GetDlgItemInt(IDC_WIDE_COUNT);
    m_uHeighCount = GetDlgItemInt(IDC_HEIGH_COUNT);
    m_uMineCount  = GetDlgItemInt(IDC_MINE_COUNT);

    if (m_uWidthCount > 25 || m_uWidthCount < 6)
    {
        MessageBox(_T("�뽫���򷽸�������Ϊ 6-25 ��!"), _T("��������"), MB_OK | MB_ICONWARNING);
        GetDlgItem(IDC_WIDE_COUNT)->SetWindowText(_T(""));
        GetDlgItem(IDC_WIDE_COUNT)->SetFocus();
        return ;
    }

    if (m_uHeighCount > 18 || m_uHeighCount < 2)
    {
        MessageBox(_T("�뽫���򷽸�������Ϊ 2-18 ��!"), _T("��������"), MB_OK | MB_ICONWARNING);
        GetDlgItem(IDC_HEIGH_COUNT)->SetWindowText(_T(""));
        GetDlgItem(IDC_HEIGH_COUNT)->SetFocus();
        return ;
    }

    if (m_uMineCount > m_uWidthCount * m_uHeighCount)
    {
        MessageBox(_T("���������ܴ��ڷ�����!"), _T("����������"), MB_OK | MB_ICONINFORMATION);
        GetDlgItem(IDC_MINE_COUNT)->SetWindowText(_T(""));
        GetDlgItem(IDC_MINE_COUNT)->SetFocus();
    }
    else if (m_uMineCount <= 0)
    {
        MessageBox(_T("����������Ϊ����0!"), _T("����������"), MB_OK | MB_ICONINFORMATION);
        GetDlgItem(IDC_MINE_COUNT)->SetWindowText(_T(""));
        GetDlgItem(IDC_MINE_COUNT)->SetFocus();
    }
    else
    {
        CDialog::EndDialog(IDOK);
    }
}


void CSettingDialog::OnBnClickedCancel()
{
    CDialog::EndDialog(IDCANCEL);
}
