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

    //创建软件信息中的"SweepMine1.0"键.若该键已经存在,则打开之.
    //
    RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\SweepMine1.0"), &hRegKey);

    //分别创建(打开)三个子键.
    //
    RegCreateKey(hRegKey, Primary, &hPrimaryKey);
    RegCreateKey(hRegKey, Middle, &hMiddleKey);
    RegCreateKey(hRegKey, Heigh, &hHeighKey);

    //
    // 分别为每个子键创建姓名、时间键.
    //
    RegCreateKey(hPrimaryKey, Name, &hPrimaryName);
    RegCreateKey(hPrimaryKey, Used_Times, &hPrimaryTime);

    RegCreateKey(hMiddleKey, Name, &hMiddleName);
    RegCreateKey(hMiddleKey, Used_Times, &hMiddleTime);

    RegCreateKey(hHeighKey, Name, &hHeighName);
    RegCreateKey(hHeighKey, Used_Times, &hHeighTime);

    //
    // 关闭姓名、时间键.
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
    // 获取初级英雄的信息.
    //
    m_szHeroNames[0] = GetRegisterValue(hPrimaryKey, Name);
    m_dwHeroUsedTimes[0] = GetRegisterValue(hPrimaryKey, Used_Times);

    //
    // 获取中级英雄的信息.
    //
    m_szHeroNames[1] = GetRegisterValue(hMiddleKey, Name);
    m_dwHeroUsedTimes[1] = GetRegisterValue(hMiddleKey, Used_Times);

    //
    // 获取高级英雄的信息.
    //
    m_szHeroNames[2] = GetRegisterValue(hHeighKey, Name);
    m_dwHeroUsedTimes[2] = GetRegisterValue(hHeighKey, Used_Times);

    //
    // 关闭注册表键.
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

    RegQueryValue(hKey, lpSubKeyName, NULL, &lValueLength);        //获得其数据的长度.

    TCHAR* pKeyValue = new TCHAR[lValueLength];
    RegQueryValue(hKey, lpSubKeyName, pKeyValue, &lValueLength);   //再获取数据.

    CString ReStr(pKeyValue);
    delete [] pKeyValue;

    return ReStr;
}
