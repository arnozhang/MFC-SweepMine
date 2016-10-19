#include "stdafx.h"
#include "SweepMine.h"
#include "HeroPageDialog.h"


IMPLEMENT_DYNAMIC(CHeroPageDialog, CDialog)

CHeroPageDialog::CHeroPageDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CHeroPageDialog::IDD, pParent)
{
}

CHeroPageDialog::~CHeroPageDialog()
{
}

void CHeroPageDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHeroPageDialog, CDialog)
END_MESSAGE_MAP()


BOOL CHeroPageDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    HKEY hRegKey     = NULL;
    HKEY hPrimaryKey = NULL;
    HKEY hMiddleKey  = NULL;
    HKEY hHeighKey   = NULL;

    HKEY hPrimaryName, hPrimaryTime;
    HKEY hMiddleName, hMiddleTime;
    HKEY hHeighName, hHeighTime;

    //���������Ϣ�е�"SweepMine1.0"��.���ü��Ѿ�����,���֮.
    //
    RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\SweepMine1.0"), &hRegKey);

    //�ֱ𴴽�(��)�����Ӽ�.
    //
    RegCreateKey(hRegKey, Primary, &hPrimaryKey);
    RegCreateKey(hRegKey, Middle, &hMiddleKey);
    RegCreateKey(hRegKey, Heigh, &hHeighKey);

    //
    // �ֱ�Ϊÿ���Ӽ�����������ʱ���.
    //
    RegCreateKey(hPrimaryKey, Name, &hPrimaryName);
    RegCreateKey(hPrimaryKey, Used_Times, &hPrimaryTime);

    RegCreateKey(hMiddleKey, Name, &hMiddleName);
    RegCreateKey(hMiddleKey, Used_Times, &hMiddleTime);

    RegCreateKey(hHeighKey, Name, &hHeighName);
    RegCreateKey(hHeighKey, Used_Times, &hHeighTime);

    //
    // �ر�������ʱ���.
    //
    RegCloseKey(hPrimaryName);
    RegCloseKey(hPrimaryTime);
    RegCloseKey(hMiddleName);
    RegCloseKey(hMiddleTime);
    RegCloseKey(hHeighName);
    RegCloseKey(hHeighTime);


    LONG lValue(0);
    DWORD dwType(0);
    DWORD dwValue(0);

    //
    // ��ȡ����Ӣ�۵���Ϣ.
    //
    m_szHeroNames[0] = GetRegisterValue(hPrimaryKey, Name);
    m_dwHeroUsedTimes[0] = GetRegisterValue(hPrimaryKey, Used_Times);

    //
    // ��ȡ�м�Ӣ�۵���Ϣ.
    //
    m_szHeroNames[1] = GetRegisterValue(hMiddleKey, Name);
    m_dwHeroUsedTimes[1] = GetRegisterValue(hMiddleKey, Used_Times);

    //
    // ��ȡ�߼�Ӣ�۵���Ϣ.
    //
    m_szHeroNames[2] = GetRegisterValue(hHeighKey, Name);
    m_dwHeroUsedTimes[2] = GetRegisterValue(hHeighKey, Used_Times);

    //
    // �ر�ע����.
    //
    RegCloseKey(hHeighKey);
    RegCloseKey(hMiddleKey);
    RegCloseKey(hPrimaryKey);
    RegCloseKey(hRegKey);

    GetDlgItem(IDC_PRIMARY_NAME)->SetWindowText(m_szHeroNames[0]);
    GetDlgItem(IDC_PRIMARY_TIME)->SetWindowText( m_dwHeroUsedTimes[0]);

    GetDlgItem(IDC_MIDDLE_NAME)->SetWindowText(m_szHeroNames[1]);
    GetDlgItem(IDC_MIDDLE_TIME)->SetWindowText( m_dwHeroUsedTimes[1]);

    GetDlgItem(IDC_HEIGH_NAME)->SetWindowText(m_szHeroNames[2]);
    GetDlgItem(IDC_HEIGH_TIME)->SetWindowText( m_dwHeroUsedTimes[2]);

    return TRUE;
}


CString CHeroPageDialog::GetRegisterValue(HKEY hKey, LPCTSTR lpSubKeyName)
{
    LONG lValueLength(0);

    RegQueryValue(hKey, lpSubKeyName, NULL, &lValueLength);        //��������ݵĳ���.

    TCHAR* pKeyValue = new TCHAR[lValueLength];
    RegQueryValue(hKey, lpSubKeyName, pKeyValue, &lValueLength);   //�ٻ�ȡ����.

    CString ReStr(pKeyValue);
    delete [] pKeyValue;

    return ReStr;
}
