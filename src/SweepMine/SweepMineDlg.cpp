#include "stdafx.h"
#include "SweepMine.h"
#include "WinedPlayer.h"
#include "SweepMineDlg.h"
#include "AuthorDialog.h"
#include "SettingDialog.h"
#include "HeroPageDialog.h"
#include "HowPlayGameDialog.h"

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialog
{
public:
    enum { IDD = IDD_ABOUTBOX };
    CAboutDlg();


protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



CSweepMineDlg::CSweepMineDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSweepMineDlg::IDD, pParent), m_cuxOffset(40), m_cuyOffset(80)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_ppCells         = 0;

    //
    // 默认为第一关的设置.
    //
    m_uWideCellCount  = 10;        // 横向的格子数.
    m_uHeighCellCount = 10;        // 纵向的格子数.
    m_uMineCount      = 10;        // 雷的数目.
    m_uPlayerLevel    = 1;         // 玩家在第一关.

    //
    // 游戏状态参数.
    //
    m_bSoundPlay    = TRUE;
    m_bGamePaused   = TRUE;
    m_bPlayerFailed = FALSE;
    m_bFirstClicked = FALSE;

    //
    // 获取每一格子的大小.
    //
    CBitmap Bitmap;
    BITMAP  BitmapInfo;

    Bitmap.LoadBitmap(IDB_CELL);
    Bitmap.GetBitmap(&BitmapInfo);

    m_uCellWidth  = BitmapInfo.bmWidth;
    m_uCellHeight = BitmapInfo.bmHeight;
}

void CSweepMineDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NEW_GAME_BTN, m_NewGameBtn);
}

void CSweepMineDlg::OnOK()
{
    //
    // 屏蔽当按下Enter时窗口自动关闭的情况.
    // 我们用新游戏代替.
    //
    NewGame();
}


BEGIN_MESSAGE_MAP(CSweepMineDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_WM_ACTIVATE()
    ON_COMMAND(ID_HEIGH,            &CSweepMineDlg::OnHeigh)
    ON_COMMAND(ID_SOUND,            &CSweepMineDlg::OnSound)
    ON_COMMAND(ID_MIDDLE,           &CSweepMineDlg::OnMiddle)
    ON_COMMAND(ID_PRIMARY,          &CSweepMineDlg::OnPrimary)
    ON_COMMAND(ID_NEW_GAME,         &CSweepMineDlg::OnNewGame)
    ON_COMMAND(ID_QUIT_GAME,        &CSweepMineDlg::OnQuitGame)
    ON_COMMAND(ID_VIEW_HERO,        &CSweepMineDlg::OnViewHero)
    ON_COMMAND(ID_PAUSE_GAME,       &CSweepMineDlg::OnPauseGame)
    ON_COMMAND(ID_ABOUT_SOFT,       &CSweepMineDlg::OnAboutSoft)
    ON_COMMAND(ID_USER_DEFINE,      &CSweepMineDlg::OnUserDefine)
    ON_COMMAND(ID_AUTHOR_INFO,      &CSweepMineDlg::OnAuthorInfo)
    ON_COMMAND(ID_HOW_SWEEP_MINE,   &CSweepMineDlg::OnHowSweepMine)
    ON_BN_CLICKED(IDC_NEW_GAME_BTN, &CSweepMineDlg::OnNewGame)
END_MESSAGE_MAP()



BOOL CSweepMineDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    //
    // 将窗口移居中心.
    //
    MoveWindowCenter(
        m_uCellWidth * m_uWideCellCount,
        m_uCellHeight * m_uHeighCellCount,
        TRUE
        );

    //
    // 先初始化英雄榜注册表.
    //
    HKEY hRegKey     = NULL;
    HKEY hPrimaryKey = NULL;
    HKEY hMiddleKey  = NULL;
    HKEY hHeighKey   = NULL;

    HKEY hPrimaryName, hPrimaryTime;
    HKEY hMiddleName, hMiddleTime;
    HKEY hHeighName, hHeighTime;

    //创建软件信息中的"SweepMine1.0"键.若该键已经存在,则打开之.
    RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\SweepMine1.0"), &hRegKey);

    //分别创建(打开)三个子键.
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
    RegCloseKey(hHeighKey);
    RegCloseKey(hMiddleKey);
    RegCloseKey(hPrimaryTime);
    RegCloseKey(hRegKey);

    NewGame();

    return TRUE;
}


void CSweepMineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


void CSweepMineDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }

    //
    // 绘制游戏场景.
    //
    DrawScance();
}


HCURSOR CSweepMineDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CSweepMineDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return ::CreateSolidBrush(Back_Color);
}


// 显示如何扫雷的帮助信息.
void CSweepMineDlg::OnHowSweepMine()
{
    CHowPlayGameDialog HowPlayGame;
    HowPlayGame.DoModal();
}


// 显示作者信息.
void CSweepMineDlg::OnAuthorInfo()
{
    CAuthorDialog AuthorDialog;
    AuthorDialog.DoModal();
}


// 显示软件信息.
void CSweepMineDlg::OnAboutSoft()
{
    CAboutDlg AboutSoftDialog;
    AboutSoftDialog.DoModal();
}


// 显示英雄榜.
void CSweepMineDlg::OnViewHero()
{
    CHeroPageDialog HeroPageDialog;
    HeroPageDialog.DoModal();
}


// 退出游戏.
void CSweepMineDlg::OnQuitGame()
{
    PostMessage(WM_CLOSE);
}


void CSweepMineDlg::OnClose()
{
    if (IDYES == MessageBox(_T("你真的要退出吗?"), _T("退出"), MB_YESNO | MB_ICONWARNING))
    {
        EndGame();
        CDialog::OnClose();
    }
}


// 菜单开始新游戏.
void CSweepMineDlg::OnNewGame()
{
    NewGame();
}


// 菜单暂停游戏.
void CSweepMineDlg::OnPauseGame()
{
    if (m_bGamePaused)
    {
        m_bGamePaused = FALSE;
        GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_PAUSE_GAME, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else
    {
        m_bGamePaused = TRUE;
        GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_PAUSE_GAME, MF_BYCOMMAND | MF_CHECKED);
    }
}


void CSweepMineDlg::OnPrimary()
{
    EndGame();

    m_uWideCellCount  = 10;
    m_uHeighCellCount = 10;
    m_uMineCount      = 10;
    m_uPlayerLevel    = 1;            // 玩家在第一关.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_CHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_UNCHECKED);

    NewGame(TRUE);
}


void CSweepMineDlg::OnMiddle()        // 中级.
{
    EndGame();                        // 先释放已经分配的内存.

    m_uWideCellCount  = 15;
    m_uHeighCellCount = 15;
    m_uMineCount      = 30;
    m_uPlayerLevel    = 2;            // 玩家在第二关.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_CHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_UNCHECKED);

    NewGame(TRUE);
}


void CSweepMineDlg::OnHeigh()         // 高级.
{
    EndGame();                        // 先释放已经分配的内存.

    m_uWideCellCount  = 20;
    m_uHeighCellCount = 16;
    m_uMineCount      = 50;
    m_uPlayerLevel    = 3;            // 玩家在第三关.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_CHECKED);

    NewGame(TRUE);
}


// 自定义游戏.
void CSweepMineDlg::OnUserDefine()
{
    CSettingDialog UserSetDialog;

    if (IDOK == UserSetDialog.DoModal())
    {
        m_uWideCellCount  = UserSetDialog.m_uWidthCount;
        m_uHeighCellCount = UserSetDialog.m_uHeighCount;
        m_uMineCount      = UserSetDialog.m_uMineCount;
        m_uPlayerLevel    = 0;        //玩家不再预设的任何一关.

        NewGame(TRUE);                //重新获取数据后即可开始新游戏.
    }
}


// 是否在游戏同时播放声音.
void CSweepMineDlg::OnSound()
{
    if (m_bSoundPlay)
    {
        GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_SOUND, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else
    {
        GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_SOUND, MF_BYCOMMAND | MF_CHECKED);
    }

    m_bSoundPlay = !m_bSoundPlay;
}


void CSweepMineDlg::MoveWindowCenter(const UINT& uWidth, const UINT& uHeight, BOOL bCenter /*=FALSE*/)
{
    //
    // 将窗口改变大小,并将之移到中央.
    //
    UINT x = ::GetSystemMetrics(SM_CXSCREEN);
    UINT y = ::GetSystemMetrics(SM_CYSCREEN);
    UINT uWindowWidth  = uWidth + 2 * m_cuxOffset + 2 * ::GetSystemMetrics(SM_CXBORDER) + 20;
    UINT uWindowHeight = uHeight + m_cuyOffset
        + ::GetSystemMetrics(SM_CYCAPTION)
        + ::GetSystemMetrics(SM_CYMENU) + m_cuxOffset + 10;

    if (bCenter)
    {
        // 将窗口移居中心.
        SetWindowPos(
            NULL,
            (x - uWindowWidth) / 2,
            (y - uWindowHeight) / 2,
            uWindowWidth,
            uWindowHeight,
            SWP_NOZORDER
            );
    }
    else
    {
        // 不改变窗口的位置.
        SetWindowPos(
            NULL,
            0,
            0,
            uWindowWidth,
            uWindowHeight,
            SWP_NOZORDER | SWP_NOMOVE
            );
    }
}


void CSweepMineDlg::NewGame(BOOL bCenterWnd /*= FALSE*/)
{
    //
    // 关闭计时器--如果它还没被关闭的话.
    //
    KillTimer(Game_Timer);

    //
    // 改变窗口大小,并将之移到中心.
    //
    MoveWindowCenter(m_uCellWidth * m_uWideCellCount, m_uCellHeight * m_uHeighCellCount, bCenterWnd);

    //
    // 使按钮可见.并使之移到中央.
    //
    m_NewGameBtn.LoadBitmaps(IDB_READY, IDB_WILL_RESULT);
    m_NewGameBtn.ShowWindow(SW_SHOW);
    m_NewGameBtn.SetWindowPos(
        NULL,
        (m_uWideCellCount * m_uCellWidth + 2 * m_cuxOffset - m_uCellWidth) / 2,
        15,
        0,
        0,
        SWP_NOZORDER | SWP_NOSIZE
        );

    //
    // 分配游戏内存.
    //
    try
    {
        m_ppCells = new SCell*[m_uWideCellCount];

        for (int i = 0; i != m_uWideCellCount; ++i)
        {
            m_ppCells[i] = new SCell[m_uHeighCellCount];
        }
    }
    catch(...)
    {
        MessageBox(_T("系统内存分配失败,程序即将退出!"), _T("错误"), MB_OK | MB_ICONWARNING);
        EndGame();
        PostQuitMessage(0);           // 退出程序.
    }

    //
    // 游戏状态参数设置.
    //
    m_bGamePaused = FALSE;            // 游戏没有暂停.
    m_bPlayerFailed = FALSE;          // 玩家没有失败.
    m_bFirstClicked = FALSE;          // 第一次单击还没有落下.

    m_uClickedCount = 0;              // 已经单击过的个数置为0.
    m_uCheckedCount = 0;              // 已经标记的个数置为0.
    m_dwUsedTime = 0;                 // 游戏计时置为0.

    m_FailedPoint.x = -1;             // 失败位置初始化.
    m_FailedPoint.y = -1;

    //将暂停菜单标记为可用.
    GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, FALSE);

    Invalidate();                     // 刷新窗口使之重绘.
}


void CSweepMineDlg::EndGame()
{
    if (m_ppCells)
    {
        for (int i = 0; i != m_uWideCellCount; ++i)
        {
            delete [] m_ppCells[i];
        }

        delete [] m_ppCells;
        m_ppCells = 0;
    }
}


// 绘制游戏场景.
void CSweepMineDlg::DrawScance()
{
    DrawTimer();                      // 先绘制计时器.
    DrawCkeckedCount();               // 再绘制旗标数目.

    CClientDC dc(this);               // 绘制场景DC.
    CDC       CompatiDC;              // 兼容DC.

    POINT Point;                      // 更改视口原点.
    Point.x = m_cuxOffset;
    Point.y = m_cuyOffset;
    dc.SetViewportOrg(Point);

    CompatiDC.CreateCompatibleDC(&dc);

    for (int i = 0; i != m_uWideCellCount; ++i)
    {
        for (int j = 0; j != m_uHeighCellCount; ++j)
        {
            CBitmap Bitmap;           // 位图对象.

            if (!m_bPlayerFailed)
            {
                if (m_ppCells[i][j].m_bChecked)
                {
                    // 此位置已经标记,则显示"旗子"标记图像.
                    Bitmap.LoadBitmap(IDB_CHECKED_FLAG);
                    CompatiDC.SelectObject(Bitmap);

                    dc.BitBlt(
                        i * m_uCellWidth,
                        j * m_uCellHeight,
                        m_uCellWidth,
                        m_uCellHeight,
                        &CompatiDC,
                        0,
                        0,
                        SRCCOPY
                        );

                    continue;
                }

                if (!m_ppCells[i][j].m_bClicked)
                {
                    Bitmap.LoadBitmap(IDB_NO_CLICKED);
                    CompatiDC.SelectObject(Bitmap);

                    dc.BitBlt(
                        i * m_uCellWidth,
                        j * m_uCellHeight,
                        m_uCellWidth,
                        m_uCellHeight,
                        &CompatiDC,
                        0,
                        0,
                        SRCCOPY
                        );

                    continue;
                }
            }
            else
            {
                if (!m_ppCells[i][j].m_bClicked && m_ppCells[i][j].m_iStatus != ISMine)
                {
                    if (!m_ppCells[i][j].m_bChecked)
                    {
                        Bitmap.LoadBitmap(IDB_NO_CLICKED);
                    }
                    else
                    {
                        Bitmap.LoadBitmap(IDB_FAILED_FLAG);
                    }

                    CompatiDC.SelectObject(Bitmap);
                    dc.BitBlt(
                        i * m_uCellWidth,
                        j * m_uCellHeight,
                        m_uCellWidth,
                        m_uCellHeight,
                        &CompatiDC,
                        0,
                        0,
                        SRCCOPY
                        );

                    continue;
                }
            }

            switch (m_ppCells[i][j].m_iStatus)
            {
            case ISMine:             // 本位置是雷.
                if (m_bPlayerFailed)
                {
                    if (i == m_FailedPoint.x && j == m_FailedPoint.y)
                    {
                        Bitmap.LoadBitmap(IDB_EXPLOSION);     // 加载爆炸地雷位图.
                    }
                    else
                    {
                        Bitmap.LoadBitmap(IDB_CELL);          // 加载普通地雷位图.
                    }
                }
                else
                {
                    Bitmap.LoadBitmap(IDB_BLANK);
                }
                break;

            case No_Mine:            // 本位置周围无雷.
                Bitmap.LoadBitmap(IDB_BLANK);
                break;

            case One_Mine:           // 本位置周围有1颗类.
                Bitmap.LoadBitmap(IDB_ONE_CELL);
                break;

            case Two_Mines:          // 本位置周围有2颗类.
                Bitmap.LoadBitmap(IDB_TWO_CELL);
                break;

            case Three_Mines:        // 本位置周围有3颗类.
                Bitmap.LoadBitmap(IDB_THREE_CELL);
                break;

            case Four_Mines:         // 本位置周围有4颗类.
                Bitmap.LoadBitmap(IDB_FOUR_CELL);
                break;

            case Five_Mines:         // 本位置周围有5颗类.
                Bitmap.LoadBitmap(IDB_FIVE_CELL);
                break;

            case Six_Mines:          // 本位置周围有6颗类.
                Bitmap.LoadBitmap(IDB_SIX_CELL);
                break;

            case Seven_Mines:        // 本位置周围有7颗类.
                Bitmap.LoadBitmap(IDB_SEVEN_CELL);
                break;

            case Eight_Mines:        // 本位置周围有8颗类.
                Bitmap.LoadBitmap(IDB_EIGHT_CELL);
                break;
            }

            CompatiDC.SelectObject(Bitmap);
            dc.BitBlt(
                i * m_uCellWidth,
                j * m_uCellHeight,
                m_uCellWidth,
                m_uCellHeight,
                &CompatiDC,
                0,
                0,
                SRCCOPY
                );
        }
    }
}


void CSweepMineDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bPlayerFailed)
    {
        int i = (point.x - m_uCellWidth) / m_uCellWidth;
        int j = (point.y - m_uCellHeight) / m_uCellHeight - 1;

        if (i < 0 || i >= m_uWideCellCount || j < 0 || j >= m_uHeighCellCount)
            return ;

        //
        // 单击位置如果未标记,则标记之.
        // 否则取消标记.
        //
        if (!m_ppCells[i][j].m_bClicked)
        {
            if (m_bSoundPlay)
            {
                PlaySound(
                    MAKEINTRESOURCE(IDR_CHECK_FLAG),
                    AfxGetResourceHandle(),
                    SND_ASYNC | SND_RESOURCE
                    );
            }

            if (m_ppCells[i][j].m_bChecked)          // 如果已经标记.
            {
                --m_uCheckedCount;                   // 将已标记的数目减1.
                m_ppCells[i][j].m_bChecked = FALSE;  // 将此位置设置为未标记.
            }
            else
            {
                ++m_uCheckedCount;                   // 将已标记数目加1.
                m_ppCells[i][j].m_bChecked = TRUE;
            }

            DrawCkeckedCount();
        }
    }

    CDialog::OnRButtonDown(nFlags, point);
}


void CSweepMineDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    DrawScance();

    CDialog::OnRButtonUp(nFlags, point);
}


void CSweepMineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bPlayerFailed)
    {
        //
        // 更改按钮小头像的显示.
        //
        m_NewGameBtn.LoadBitmaps(IDB_WILL_RESULT);
        m_NewGameBtn.Invalidate();

        int i = (point.x - m_uCellWidth) / m_uCellWidth;
        int j = (point.y - m_uCellHeight) / m_uCellHeight - 1;

        if (i < 0 || i >= m_uWideCellCount || j < 0 || j >= m_uHeighCellCount)
            return ;

        m_bGamePaused = FALSE;                // 标记为游戏未暂停.
        GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_PAUSE_GAME, MF_BYCOMMAND | MF_UNCHECKED);

        CBitmap   Bitmap;
        CClientDC dc(this);
        CDC       CompatiDC;

        point.x = m_cuxOffset;
        point.y = m_cuyOffset;
        dc.SetViewportOrg(point);             // 重新设置视口原点.

        CompatiDC.CreateCompatibleDC(&dc);    // 场创建贴图兼容DC.

        if (!m_bFirstClicked)                  // 这是第一次单击,开始布雷,要确保不死.
        {
            // 设置每秒一次的计时器.开始计时.
            SetTimer(Game_Timer, 1000, NULL);

            //
            // 在除了本位置之外的地方随机放置地雷.
            //
            srand((unsigned int)time(NULL));  // 初始化随机数生成器.
            for (int MineIndex=0; MineIndex != m_uMineCount; ++MineIndex)
            {
                while(TRUE)
                {
                    int xPos = rand() % m_uWideCellCount;
                    int yPos = rand() % m_uHeighCellCount;

                    // 如果随机生成的位置还没有雷,则放置一颗雷.
                    if (m_ppCells[xPos][yPos].m_iStatus != ISMine && xPos != i && yPos != j)
                    {
                        m_ppCells[xPos][yPos].m_iStatus = ISMine;
                        break;
                    }
                }
            }

            ComputeCellSpread();            // 布雷并计算地雷分布.
            m_bFirstClicked = TRUE;         // 第一次单击已经完成.
        }

        if (m_ppCells[i][j].m_bChecked)     // 如果此位置已经设置标记,则显示"单击标记"图像.
        {
            if (m_bSoundPlay)
            {
                MessageBeep(MB_OK);         // 播放无法操作声音.
            }

            Bitmap.LoadBitmap(IDB_CLICK_CHECKED_FLAG);
            CompatiDC.SelectObject(Bitmap);

            dc.BitBlt(
                i * m_uCellWidth,
                j * m_uCellHeight,
                m_uCellWidth,
                m_uCellHeight,
                &CompatiDC,
                0,
                0,
                SRCCOPY
                );
            return ;
        }

        //
        // 如果此位置没被单击或者被单击过且周围无雷,则加载"单击空白"图像.
        //
        if (!m_ppCells[i][j].m_bClicked ||
            (m_ppCells[i][j].m_bClicked &&
            m_ppCells[i][j].m_iStatus == No_Mine))
        {
            Bitmap.LoadBitmap(IDB_CLICK_BLANK);
            CompatiDC.SelectObject(Bitmap);
            dc.BitBlt(
                i * m_uCellWidth,
                j * m_uCellHeight,
                m_uCellWidth,
                m_uCellHeight,
                &CompatiDC,
                0,
                0,
                SRCCOPY
                );
        }

        //
        // 如果此位置周围有雷且已经被单击,则将周围的范围标出来.
        //
        if (m_ppCells[i][j].m_bClicked && m_ppCells[i][j].m_iStatus != No_Mine)
        {
            int iCheckedRightCount(0);                   // 周围标记对的计数器.

            for (int xPos=i-1; xPos<i+2; ++xPos)
            {
                if (xPos >= 0 && xPos < m_uWideCellCount)
                {
                    for (int yPos=j-1; yPos<j+2; ++yPos)
                    {
                        if (yPos >=0 && yPos < m_uHeighCellCount)
                        {
                            CBitmap TipBitmap;
                            TipBitmap.LoadBitmap(IDB_TRANS);
                            CompatiDC.SelectObject(TipBitmap);
                            dc.TransparentBlt(
                                xPos * m_uCellWidth,
                                yPos * m_uCellHeight,
                                m_uCellWidth,
                                m_uCellHeight,
                                &CompatiDC,
                                0,
                                0,
                                m_uCellWidth,
                                m_uCellHeight,
                                RGB(255, 0, 255)
                                );

                            if (m_ppCells[xPos][yPos].m_bChecked && m_ppCells[xPos][yPos].m_iStatus == ISMine)
                            {
                                ++iCheckedRightCount;    // 如果周围有标记对的雷,则将计数器加1.
                            }
                        }
                    }
                }
            } // xPos For end here.

            //
            // 检验此位置周围标记的雷的数目是否正确.
            // 如果正确,则将其周围的扩展开来.
            //
            if (m_ppCells[i][j].m_iStatus == iCheckedRightCount)
            {
                for (int xPos=i-1; xPos<i+2; ++xPos)
                {
                    if (xPos >= 0 && xPos < m_uWideCellCount)
                    {
                        for (int yPos=j-1; yPos<j+2; ++yPos)
                        {
                            if (yPos >=0 && yPos < m_uHeighCellCount)
                            {
                                if (!m_ppCells[xPos][yPos].m_bChecked && !m_ppCells[xPos][yPos].m_bClicked)
                                {
                                    m_ppCells[xPos][yPos].m_bClicked = TRUE;
                                    ++m_uClickedCount;   // 将单击过的数目加1.
                                }
                            }
                        }
                    }
                } // xPos For end here.

                if (PlayerWin())                          // 检测玩家是否赢得游戏.
                {
                    return ;
                }

            } // If End Here.
        }
    } // If Statement End Here.

    CDialog::OnLButtonDown(nFlags, point);
}


void CSweepMineDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (!m_bPlayerFailed)
    {
        //
        // 更改按钮小头像的显示.
        //
        m_NewGameBtn.LoadBitmaps(IDB_READY);
        m_NewGameBtn.Invalidate();

        int i = (point.x - m_uCellWidth) / m_uCellWidth;
        int j = (point.y - m_uCellHeight) / m_uCellHeight - 1;

        if (i < 0 || i >= m_uWideCellCount || j < 0 || j >= m_uHeighCellCount)
            return ;

        //
        // 如果此地是地雷且未标记失败,则显示所有地雷,结束游戏.
        //
        if (m_ppCells[i][j].m_iStatus == ISMine && !m_ppCells[i][j].m_bChecked)
        {
            // 为按钮加载玩家失败图片.
            m_NewGameBtn.LoadBitmaps(IDB_FAILED);
            m_NewGameBtn.Invalidate();

            if (m_bSoundPlay)
            {
                PlaySound(
                    MAKEINTRESOURCE(IDR_EXPLOSION),
                    AfxGetResourceHandle(),
                    SND_ASYNC | SND_RESOURCE
                    );
            }

            m_bPlayerFailed = TRUE;                // 标记为玩家失败.
            m_FailedPoint.x = i;
            m_FailedPoint.y = j;

            KillTimer(Game_Timer);                 // 关闭计时器.
            DrawScance();

            // 将暂停菜单标记为不可用.
            GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, TRUE);

            return ;
        }

        //
        // 若此位置还未被单击且此位置没有被标记,则更新之.
        //
        if (!m_ppCells[i][j].m_bClicked && !m_ppCells[i][j].m_bChecked)
        {
            if (m_bSoundPlay && m_ppCells[i][j].m_iStatus != ISMine)
            {
                PlaySound(
                    MAKEINTRESOURCE(IDR_PLAYER_STEP),
                    AfxGetResourceHandle(),
                    SND_ASYNC | SND_RESOURCE
                    );
            }

            // 标记为已单击.
            m_ppCells[i][j].m_bClicked = TRUE;
            ++m_uClickedCount;

            if (PlayerWin())
            {
                // 检测玩家是否赢得游戏.
                return ;
            }

            if (m_ppCells[i][j].m_iStatus == No_Mine)
            {
                // 将周围的空白格扩展开来.
                ExtendBlank(i, j);
            }

        } //If End Here.

        DrawScance();
    }

    CDialog::OnLButtonUp(nFlags, point);
}


void CSweepMineDlg::ExtendBlank(const int& uxPos, const int& uyPos)
{
    //
    // 再将周围的一小片不是雷的区域标记出来.
    //
    for (int i=uxPos-1; i<uxPos+2; ++i)
    {
        if (i >= 0 && i < m_uWideCellCount)
        {
            // 确保横向不能出界.
            for (int j=uyPos-1; j<uyPos+2; ++j)
            {
                if (j >=0 && j< m_uHeighCellCount)
                {
                    // 确保纵向不能出界.
                    if (m_ppCells[i][j].m_iStatus != ISMine
                        && !m_ppCells[i][j].m_bClicked && !m_ppCells[i][j].m_bChecked)
                    {
                        m_ppCells[i][j].m_bClicked = TRUE;
                        ++m_uClickedCount;
                        if (m_ppCells[i][j].m_iStatus == No_Mine)
                        {
                            ExtendBlank(i, j);
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
            } //j -- For End Here.
        }
    } //i -- For End Here.

    if (PlayerWin())
    {
        // 检测玩家是否赢得游戏.
        return ;
    }
}


BOOL CSweepMineDlg::PlayerWin()
{
    //
    // 检查玩家是否赢得游戏.
    //
    if (m_uWideCellCount * m_uHeighCellCount - m_uClickedCount == m_uMineCount)
    {
        if (m_bSoundPlay)
        {
            PlaySound(
                MAKEINTRESOURCE(IDR_PLAYER_WIN),
                AfxGetResourceHandle(),
                SND_ASYNC | SND_RESOURCE
                );
        }

        KillTimer(Game_Timer);               // 关闭计时器.
        m_uCheckedCount = m_uMineCount;      // 将标记数设为雷的数目.
        m_bPlayerFailed = TRUE;              // 虽然这样,但玩家实际上赢了.

        DrawScance();
        m_NewGameBtn.LoadBitmaps(IDB_READY, IDB_WILL_RESULT);

        // 将暂停菜单标记为不可用.
        GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, TRUE);

        // 将英雄信息写入注册表.
        HeroRegiste();
        return TRUE;
    }

    return FALSE;
}


void CSweepMineDlg::HeroRegiste()
{
    HKEY hRegKey = NULL;
    CString szKeyInfo = _T("Software\\SweepMine1.0");

    // 创建软件信息中的 "SweepMine1.0" 键.若该键已经存在,则打开之.
    if (m_uPlayerLevel == 1)
    {
        szKeyInfo += _T("\\Primary");
    }
    else if (m_uPlayerLevel == 2)
    {
        szKeyInfo += _T("\\Middle");
    }
    else if (m_uPlayerLevel == 3)
    {
        szKeyInfo += _T("\\Heigh");
    }
    else
    {
        // 虽然这样,但玩家实际上赢了.
        m_bPlayerFailed = TRUE;
        MessageBox(_T("真棒!你赢了!!!"), _T("你赢了"), MB_OK | MB_ICONINFORMATION);
        return ;
    }

    RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo, &hRegKey);
    CString szTime = CHeroPageDialog::GetRegisterValue(hRegKey, Used_Times);

    DWORD dwBestUsedTime =  wcstoul(szTime, '\0', 10);     // 最好的游戏记录.
    if (szTime == "" || m_dwUsedTime < dwBestUsedTime)     // 玩家打破了记录.
    {
        CWinedPlayer PlayerWinDialog;
        if (IDOK == PlayerWinDialog.DoModal())
        {
            HKEY hHeroNameKey      = NULL;
            HKEY hHeroUsedTimesKey = NULL;

            // 打开姓名和用时注册表.
            RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo + _T("\\Name"), &hHeroNameKey);
            RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo + _T("\\UsedTimes"), &hHeroUsedTimesKey);

            // 将姓名写入注册表.
            RegSetValue(
                hHeroNameKey,
                NULL,
                REG_SZ,
                PlayerWinDialog.GetPlayerName(),
                PlayerWinDialog.GetPlayerName().GetLength()
                );

            // 将用时写入注册表.
            szTime.Format(_T("%d"), m_dwUsedTime);
            RegSetValue(
                hHeroUsedTimesKey,
                NULL,
                REG_SZ,
                szTime,
                szTime.GetLength()
                );

            // 关闭姓名和用时注册表.
            RegCloseKey(hHeroNameKey);
            RegCloseKey(hHeroUsedTimesKey);
        }
    }
    else if (m_dwUsedTime == dwBestUsedTime)
    {
        // 玩家平了最佳记录.
        MessageBox(
            _T("好样的,你已经平了最佳记录!继续努力!!"),
            _T("Good Job!"),
            MB_OK | MB_ICONINFORMATION
            );
    }
    else
    {
        // 玩家没有打破记录,则显示提示信息.
        CString Str;
        Str.Format(
            _T("继续努力,你还有 %d 秒就打破了记录!"),
            m_dwUsedTime - dwBestUsedTime
            );
        MessageBox(Str, _T("你赢了!"), MB_OK | MB_ICONINFORMATION);
    }

    if (hRegKey)
    {
        RegCloseKey(hRegKey);
    }
}


//
// 为每一位置计算地雷分布个数.
//
void CSweepMineDlg::ComputeCellSpread()
{
    for (int i = 0; i != m_uWideCellCount; ++i)
    {
        for (int j = 0; j != m_uHeighCellCount; ++j)
        {
            if (m_ppCells[i][j].m_iStatus == ISMine)
            {
                // 如果本位置是地雷,则为它周围的格子计算地雷分布.
                for (int m=i-1; m<i+2; ++m)
                {
                    if (m>=0 && m<m_uWideCellCount)
                    {
                        for (int n=j-1; n<j+2; ++n)
                        {
                            if (n>=0 && n<m_uHeighCellCount)
                            {
                                if (m_ppCells[m][n].m_iStatus != ISMine)
                                {
                                    // 如果四周不是地雷,则将之地雷数加1.
                                    ++m_ppCells[m][n].m_iStatus;
                                }
                            }
                        }
                    }
                } // 周围分布情况结束.
            }
        } // 所有地雷分析完成.
    } //For End Here.
}


void CSweepMineDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case Game_Timer:            // 游戏计时器.每1s记一次.
        if (!m_bGamePaused)
        {
            ++m_dwUsedTime;     // 如果游戏并未暂停,则将游戏计时加1.
            DrawTimer();        // 更新计时器.
        }

        break;

    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}


void CSweepMineDlg::DrawTimer()
{
    //
    // 绘制计时器部分. 计时器格式: 分:秒,其中"分"占两位,"秒"占两位.
    //
    UINT uTimeFormat[4];
    uTimeFormat[0] = m_dwUsedTime / 600;      // 获取分钟数.
    uTimeFormat[1] = m_dwUsedTime / 60 - 10 * uTimeFormat[0];

    uTimeFormat[2] = (m_dwUsedTime - uTimeFormat[0] * 600 - uTimeFormat[1] * 60) / 10;     // 获取秒数.
    uTimeFormat[3] = (m_dwUsedTime - uTimeFormat[0] * 600 - uTimeFormat[1] * 60) - uTimeFormat[2] * 10;

    //
    // 执行计时器绘制.
    //
    CBitmap   Bitmap;
    BITMAP    BitmapInfo;
    CClientDC dc(this);
    CDC       CompatiDC;

    Bitmap.LoadBitmap(IDB_TIME);
    Bitmap.GetBitmap(&BitmapInfo);
    CompatiDC.CreateCompatibleDC(&dc);
    CompatiDC.SelectObject(Bitmap);

    UINT uBltxPos = m_cuxOffset + (m_uCellWidth * m_uWideCellCount) / 2 + 60;
    UINT uBltyPos = 25;

    for (int i = 0; i != 4; ++i)
    {
        uBltxPos += BitmapInfo.bmWidth;
        dc.StretchBlt(
            uBltxPos,
            uBltyPos,
            BitmapInfo.bmWidth,
            BitmapInfo.bmHeight / 11,
            &CompatiDC,
            0,
            uTimeFormat[i] * (BitmapInfo.bmHeight / 11),
            BitmapInfo.bmWidth,
            BitmapInfo.bmHeight / 11,
            SRCCOPY
            );

        if (i == 1)
        {
            // 贴出时钟分隔符.
            uBltxPos += BitmapInfo.bmWidth;
            dc.StretchBlt(
                uBltxPos,
                uBltyPos,
                BitmapInfo.bmWidth,
                BitmapInfo.bmHeight / 11,
                &CompatiDC,
                0,
                10 * (BitmapInfo.bmHeight / 11),
                BitmapInfo.bmWidth,
                BitmapInfo.bmHeight / 11,
                SRCCOPY
                );
        }
    }
}


void CSweepMineDlg::DrawCkeckedCount()
{
    //
    // 更新标记数目的绘制.
    //
    UINT uTextxPos = m_cuxOffset + (m_uCellWidth * m_uWideCellCount) / 2 - 140;
    UINT uTextyPos = 20;

    CClientDC dc(this);
    CString   Str;
    Str.Format(_T("%d / %d"), m_uCheckedCount, m_uMineCount);

    CFont Font;
    Font.CreatePointFont(180, _T("Microsoft YaHei"));

    dc.SelectObject(Font);
    dc.SetBkColor(Back_Color);
    dc.SetTextColor(RGB(255, 0, 0));
    dc.TextOut(uTextxPos, uTextyPos, Str);
}


void CSweepMineDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CDialog::OnActivate(nState, pWndOther, bMinimized);

    if (nState == WA_INACTIVE)
    {
        m_bGamePaused = TRUE;          // 失去焦点,暂停游戏.
    }
    else if (nState == WA_CLICKACTIVE || nState == WA_ACTIVE)
    {
        m_bGamePaused = FALSE;         // 获得焦点,继续游戏.
    }
}


BOOL CSweepMineDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            if (IDYES == MessageBox(
                    _T("你真的要退出吗?"),
                    _T("退出"),
                    MB_YESNO | MB_ICONWARNING
                    )
                )
            {
                EndGame();
                CDialog::OnClose();
            }
            else
            {
                return TRUE;
            }
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}
