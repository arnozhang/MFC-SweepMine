/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


class CWinedPlayer : public CDialog
{
    DECLARE_DYNAMIC(CWinedPlayer)

public:
    enum { IDD = IDD_WINED_PLAYER_INFO };
    CWinedPlayer(CWnd* pParent = NULL);
    virtual ~CWinedPlayer();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnBnClickedOk();
    DECLARE_MESSAGE_MAP()


public:
    const CString& GetPlayerName() const
    {
        return m_szWinedPlayerName;
    }

private:
    CString m_szWinedPlayerName;    // 玩家输入的姓名.
};
