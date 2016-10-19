/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once

#include "afxwin.h"


#define Game_Timer 1                    // 游戏计时器标号.
#define Back_Color RGB(250, 200, 140)   // 窗体背景色.


class CSweepMineDlg : public CDialog
{
public:
    enum { IDD = IDD_SWEEPMINE_DIALOG };
    CSweepMineDlg(CWnd* pParent = NULL);


protected:
    virtual void DoDataExchange(CDataExchange* pDX);


protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();          // 初始化窗口.
    virtual void OnOK();                  // 屏蔽当按下Enter时窗口自动关闭的情况.
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
    enum Game_Params                // 枚举游戏参数.
    {
        ISMine= -1,                 // 雷.
        No_Mine,                    // 无雷.
        One_Mine,                   // 一颗雷.
        Two_Mines,                  // 两颗雷.
        Three_Mines,                //...以下依次类推.
        Four_Mines,
        Five_Mines,
        Six_Mines,
        Seven_Mines,
        Eight_Mines
    };


    struct SCell                    // 代表每一个格子.
    {
    public:
        SCell()
        {
            m_iStatus  = No_Mine;   // 起初全部初始化为0,表示没有地雷.
            m_bClicked = FALSE;     // 表示还没有单击.
            m_bChecked = FALSE;
        }
        ~SCell()
        {
        }

        BOOL m_bClicked;            // 是否已经被单击.
        BOOL m_bChecked;            // 是否已经标记.
        int  m_iStatus;             // 一小格的状态.(周围的雷数或者是否为地雷.为地雷时为-1)
    };

    const UINT m_cuxOffset;         // x位置的偏移.
    const UINT m_cuyOffset;         // y位置的偏移.


private:
    void NewGame(
        BOOL bCenterWnd = FALSE
        );                          // 开始游戏.
    void EndGame();                 // 结束游戏.

    void DrawScance();              // 绘制游戏场景.
    void ComputeCellSpread();       // 计算地雷分布.

    void DrawTimer();               // 绘制计时器.
    void DrawCkeckedCount();        // 更新旗标数据绘制.

    BOOL PlayerWin();               // 检测玩家是否赢得游戏.
    void HeroRegiste();             // 英雄信息写入注册表.

    void ExtendBlank(               // 将某一位置周围的空白扩展完.
        const int& uxPos,
        const int& uyPos
        );

    void MoveWindowCenter(          // 将窗口移到中间.
        const UINT& uWidth,
        const UINT& uHeight,
        BOOL bCenter = FALSE
        );


private:
    SCell** m_ppCells;              // 指向所有格子二维数组的指针.

    int     m_uWideCellCount;       // 横向的格子数.
    int     m_uHeighCellCount;      // 纵向的格子数.
    UINT    m_uMineCount;           // 雷的总数.

    UINT    m_uCellWidth;           // 一小格的宽度.
    UINT    m_uCellHeight;          // 一小格的高度.

    UINT    m_uClickedCount;        // 鼠标单击过的个数.
    UINT    m_uCheckedCount;        // 标记过的个数.

    BOOL    m_bGamePaused;          // 游戏是否暂停.
    BOOL    m_bPlayerFailed;        // 玩家是否失败.
    BOOL    m_bSoundPlay;           // 是否开启音效.

    BOOL    m_bFirstClicked;        // 第一次是否已经单击.
    DWORD   m_dwUsedTime;           // 玩家游戏用时.
    UINT    m_uPlayerLevel;         // 玩家当前的关卡.

    CPoint  m_FailedPoint;          // 玩家失败爆炸的位置索引.
    CBitmapButton m_NewGameBtn;     // 位图按钮.
};
