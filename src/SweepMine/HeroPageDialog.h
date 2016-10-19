/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


#define Name            _T("Name")
#define Used_Times      _T("UsedTimes")
#define Primary         _T("Primary")
#define Middle          _T("Middle")
#define Heigh           _T("Heigh")


class CHeroPageDialog : public CDialog
{
    DECLARE_DYNAMIC(CHeroPageDialog)

public:
    enum { IDD = IDD_HERO_PAGE };
    CHeroPageDialog(CWnd* pParent = NULL);
    virtual ~CHeroPageDialog();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();


public:
    static CString GetRegisterValue(HKEY hKey, LPCTSTR lpSubKeyName);


private:
    CString m_szHeroNames[3];            // 对应三个等级的英雄姓名.
    CString m_dwHeroUsedTimes[3];        // 对应的三个等级的英雄所对应的用时.
};
