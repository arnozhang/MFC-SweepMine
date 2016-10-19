/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once

#include "afxwin.h"


#define Game_Timer 1                    // ��Ϸ��ʱ�����.
#define Back_Color RGB(250, 200, 140)   // ���屳��ɫ.


class CSweepMineDlg : public CDialog
{
public:
    enum { IDD = IDD_SWEEPMINE_DIALOG };
    CSweepMineDlg(CWnd* pParent = NULL);


protected:
    virtual void DoDataExchange(CDataExchange* pDX);


protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();          // ��ʼ������.
    virtual void OnOK();                  // ���ε�����Enterʱ�����Զ��رյ����.
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

    afx_msg void OnAboutSoft();
    afx_msg void OnNewGame();
    afx_msg void OnPauseGame();
    afx_msg void OnPrimary();
    afx_msg void OnMiddle();
    afx_msg void OnHeigh();
    afx_msg void OnUserDefine();
    afx_msg void OnQuitGame();
    afx_msg void OnSound();
    afx_msg void OnHowSweepMine();
    afx_msg void OnAuthorInfo();
    afx_msg void OnViewHero();
    afx_msg void OnClose();

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void   OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()


private:
    enum Game_Params                // ö����Ϸ����.
    {
        ISMine= -1,                 // ��.
        No_Mine,                    // ����.
        One_Mine,                   // һ����.
        Two_Mines,                  // ������.
        Three_Mines,                //...������������.
        Four_Mines,
        Five_Mines,
        Six_Mines,
        Seven_Mines,
        Eight_Mines
    };


    struct SCell                    // ����ÿһ������.
    {
    public:
        SCell()
        {
            m_iStatus  = No_Mine;   // ���ȫ����ʼ��Ϊ0,��ʾû�е���.
            m_bClicked = FALSE;     // ��ʾ��û�е���.
            m_bChecked = FALSE;
        }
        ~SCell()
        {
        }

        BOOL m_bClicked;            // �Ƿ��Ѿ�������.
        BOOL m_bChecked;            // �Ƿ��Ѿ����.
        int  m_iStatus;             // һС���״̬.(��Χ�����������Ƿ�Ϊ����.Ϊ����ʱΪ-1)
    };

    const UINT m_cuxOffset;         // xλ�õ�ƫ��.
    const UINT m_cuyOffset;         // yλ�õ�ƫ��.


private:
    void NewGame(
        BOOL bCenterWnd = FALSE
        );                          // ��ʼ��Ϸ.
    void EndGame();                 // ������Ϸ.

    void DrawScance();              // ������Ϸ����.
    void ComputeCellSpread();       // ������׷ֲ�.

    void DrawTimer();               // ���Ƽ�ʱ��.
    void DrawCkeckedCount();        // ����������ݻ���.

    BOOL PlayerWin();               // �������Ƿ�Ӯ����Ϸ.
    void HeroRegiste();             // Ӣ����Ϣд��ע���.

    void ExtendBlank(               // ��ĳһλ����Χ�Ŀհ���չ��.
        const int& uxPos,
        const int& uyPos
        );

    void MoveWindowCenter(          // �������Ƶ��м�.
        const UINT& uWidth,
        const UINT& uHeight,
        BOOL bCenter = FALSE
        );


private:
    SCell** m_ppCells;              // ָ�����и��Ӷ�ά�����ָ��.

    int     m_uWideCellCount;       // ����ĸ�����.
    int     m_uHeighCellCount;      // ����ĸ�����.
    UINT    m_uMineCount;           // �׵�����.

    UINT    m_uCellWidth;           // һС��Ŀ��.
    UINT    m_uCellHeight;          // һС��ĸ߶�.

    UINT    m_uClickedCount;        // ��굥�����ĸ���.
    UINT    m_uCheckedCount;        // ��ǹ��ĸ���.

    BOOL    m_bGamePaused;          // ��Ϸ�Ƿ���ͣ.
    BOOL    m_bPlayerFailed;        // ����Ƿ�ʧ��.
    BOOL    m_bSoundPlay;           // �Ƿ�����Ч.

    BOOL    m_bFirstClicked;        // ��һ���Ƿ��Ѿ�����.
    DWORD   m_dwUsedTime;           // �����Ϸ��ʱ.
    UINT    m_uPlayerLevel;         // ��ҵ�ǰ�Ĺؿ�.

    CPoint  m_FailedPoint;          // ���ʧ�ܱ�ը��λ������.
    CBitmapButton m_NewGameBtn;     // λͼ��ť.
};
