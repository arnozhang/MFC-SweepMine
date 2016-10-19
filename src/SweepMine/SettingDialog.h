/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


class CSettingDialog : public CDialog
{
    DECLARE_DYNAMIC(CSettingDialog)

public:
    enum { IDD = IDD_PLAYER_DEFINE };
    CSettingDialog(CWnd* pParent = NULL);
    virtual ~CSettingDialog();


protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    DECLARE_MESSAGE_MAP()


public:
    int m_uWidthCount;      // 横向的格子数
    int m_uHeighCount;      // 纵向的格子数
    int m_uMineCount;       // 地雷数目
};
