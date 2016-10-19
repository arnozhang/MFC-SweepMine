/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


class CAuthorDialog : public CDialog
{
    DECLARE_DYNAMIC(CAuthorDialog)

public:
    enum { IDD = IDD_AUTHOR_DIALOG };
    CAuthorDialog(CWnd* pParent = NULL);
    virtual ~CAuthorDialog();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};
