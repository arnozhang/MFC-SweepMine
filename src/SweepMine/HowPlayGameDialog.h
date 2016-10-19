/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


class CHowPlayGameDialog : public CDialog
{
    DECLARE_DYNAMIC(CHowPlayGameDialog)

public:
    enum { IDD = IDD_HOW_PLAY_GAME };
    CHowPlayGameDialog(CWnd* pParent = NULL);
    virtual ~CHowPlayGameDialog();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedIKnow();
};
