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
    // Ĭ��Ϊ��һ�ص�����.
    //
    m_uWideCellCount  = 10;        // ����ĸ�����.
    m_uHeighCellCount = 10;        // ����ĸ�����.
    m_uMineCount      = 10;        // �׵���Ŀ.
    m_uPlayerLevel    = 1;         // ����ڵ�һ��.

    //
    // ��Ϸ״̬����.
    //
    m_bSoundPlay    = TRUE;
    m_bGamePaused   = TRUE;
    m_bPlayerFailed = FALSE;
    m_bFirstClicked = FALSE;

    //
    // ��ȡÿһ���ӵĴ�С.
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
    // ���ε�����Enterʱ�����Զ��رյ����.
    // ����������Ϸ����.
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
    // �������ƾ�����.
    //
    MoveWindowCenter(
        m_uCellWidth * m_uWideCellCount,
        m_uCellHeight * m_uHeighCellCount,
        TRUE
        );

    //
    // �ȳ�ʼ��Ӣ�۰�ע���.
    //
    HKEY hRegKey     = NULL;
    HKEY hPrimaryKey = NULL;
    HKEY hMiddleKey  = NULL;
    HKEY hHeighKey   = NULL;

    HKEY hPrimaryName, hPrimaryTime;
    HKEY hMiddleName, hMiddleTime;
    HKEY hHeighName, hHeighTime;

    //���������Ϣ�е�"SweepMine1.0"��.���ü��Ѿ�����,���֮.
    RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\SweepMine1.0"), &hRegKey);

    //�ֱ𴴽�(��)�����Ӽ�.
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

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }

    //
    // ������Ϸ����.
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


// ��ʾ���ɨ�׵İ�����Ϣ.
void CSweepMineDlg::OnHowSweepMine()
{
    CHowPlayGameDialog HowPlayGame;
    HowPlayGame.DoModal();
}


// ��ʾ������Ϣ.
void CSweepMineDlg::OnAuthorInfo()
{
    CAuthorDialog AuthorDialog;
    AuthorDialog.DoModal();
}


// ��ʾ�����Ϣ.
void CSweepMineDlg::OnAboutSoft()
{
    CAboutDlg AboutSoftDialog;
    AboutSoftDialog.DoModal();
}


// ��ʾӢ�۰�.
void CSweepMineDlg::OnViewHero()
{
    CHeroPageDialog HeroPageDialog;
    HeroPageDialog.DoModal();
}


// �˳���Ϸ.
void CSweepMineDlg::OnQuitGame()
{
    PostMessage(WM_CLOSE);
}


void CSweepMineDlg::OnClose()
{
    if (IDYES == MessageBox(_T("�����Ҫ�˳���?"), _T("�˳�"), MB_YESNO | MB_ICONWARNING))
    {
        EndGame();
        CDialog::OnClose();
    }
}


// �˵���ʼ����Ϸ.
void CSweepMineDlg::OnNewGame()
{
    NewGame();
}


// �˵���ͣ��Ϸ.
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
    m_uPlayerLevel    = 1;            // ����ڵ�һ��.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_CHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_UNCHECKED);

    NewGame(TRUE);
}


void CSweepMineDlg::OnMiddle()        // �м�.
{
    EndGame();                        // ���ͷ��Ѿ�������ڴ�.

    m_uWideCellCount  = 15;
    m_uHeighCellCount = 15;
    m_uMineCount      = 30;
    m_uPlayerLevel    = 2;            // ����ڵڶ���.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_CHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_UNCHECKED);

    NewGame(TRUE);
}


void CSweepMineDlg::OnHeigh()         // �߼�.
{
    EndGame();                        // ���ͷ��Ѿ�������ڴ�.

    m_uWideCellCount  = 20;
    m_uHeighCellCount = 16;
    m_uMineCount      = 50;
    m_uPlayerLevel    = 3;            // ����ڵ�����.

    CMenu* pMenu = GetMenu()->GetSubMenu(0);
    pMenu->CheckMenuItem(ID_PRIMARY, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_MIDDLE, MF_BYCOMMAND | MF_UNCHECKED);
    pMenu->CheckMenuItem(ID_HEIGH, MF_BYCOMMAND | MF_CHECKED);

    NewGame(TRUE);
}


// �Զ�����Ϸ.
void CSweepMineDlg::OnUserDefine()
{
    CSettingDialog UserSetDialog;

    if (IDOK == UserSetDialog.DoModal())
    {
        m_uWideCellCount  = UserSetDialog.m_uWidthCount;
        m_uHeighCellCount = UserSetDialog.m_uHeighCount;
        m_uMineCount      = UserSetDialog.m_uMineCount;
        m_uPlayerLevel    = 0;        //��Ҳ���Ԥ����κ�һ��.

        NewGame(TRUE);                //���»�ȡ���ݺ󼴿ɿ�ʼ����Ϸ.
    }
}


// �Ƿ�����Ϸͬʱ��������.
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
    // �����ڸı��С,����֮�Ƶ�����.
    //
    UINT x = ::GetSystemMetrics(SM_CXSCREEN);
    UINT y = ::GetSystemMetrics(SM_CYSCREEN);
    UINT uWindowWidth  = uWidth + 2 * m_cuxOffset + 2 * ::GetSystemMetrics(SM_CXBORDER) + 20;
    UINT uWindowHeight = uHeight + m_cuyOffset
        + ::GetSystemMetrics(SM_CYCAPTION)
        + ::GetSystemMetrics(SM_CYMENU) + m_cuxOffset + 10;

    if (bCenter)
    {
        // �������ƾ�����.
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
        // ���ı䴰�ڵ�λ��.
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
    // �رռ�ʱ��--�������û���رյĻ�.
    //
    KillTimer(Game_Timer);

    //
    // �ı䴰�ڴ�С,����֮�Ƶ�����.
    //
    MoveWindowCenter(m_uCellWidth * m_uWideCellCount, m_uCellHeight * m_uHeighCellCount, bCenterWnd);

    //
    // ʹ��ť�ɼ�.��ʹ֮�Ƶ�����.
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
    // ������Ϸ�ڴ�.
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
        MessageBox(_T("ϵͳ�ڴ����ʧ��,���򼴽��˳�!"), _T("����"), MB_OK | MB_ICONWARNING);
        EndGame();
        PostQuitMessage(0);           // �˳�����.
    }

    //
    // ��Ϸ״̬��������.
    //
    m_bGamePaused = FALSE;            // ��Ϸû����ͣ.
    m_bPlayerFailed = FALSE;          // ���û��ʧ��.
    m_bFirstClicked = FALSE;          // ��һ�ε�����û������.

    m_uClickedCount = 0;              // �Ѿ��������ĸ�����Ϊ0.
    m_uCheckedCount = 0;              // �Ѿ���ǵĸ�����Ϊ0.
    m_dwUsedTime = 0;                 // ��Ϸ��ʱ��Ϊ0.

    m_FailedPoint.x = -1;             // ʧ��λ�ó�ʼ��.
    m_FailedPoint.y = -1;

    //����ͣ�˵����Ϊ����.
    GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, FALSE);

    Invalidate();                     // ˢ�´���ʹ֮�ػ�.
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


// ������Ϸ����.
void CSweepMineDlg::DrawScance()
{
    DrawTimer();                      // �Ȼ��Ƽ�ʱ��.
    DrawCkeckedCount();               // �ٻ��������Ŀ.

    CClientDC dc(this);               // ���Ƴ���DC.
    CDC       CompatiDC;              // ����DC.

    POINT Point;                      // �����ӿ�ԭ��.
    Point.x = m_cuxOffset;
    Point.y = m_cuyOffset;
    dc.SetViewportOrg(Point);

    CompatiDC.CreateCompatibleDC(&dc);

    for (int i = 0; i != m_uWideCellCount; ++i)
    {
        for (int j = 0; j != m_uHeighCellCount; ++j)
        {
            CBitmap Bitmap;           // λͼ����.

            if (!m_bPlayerFailed)
            {
                if (m_ppCells[i][j].m_bChecked)
                {
                    // ��λ���Ѿ����,����ʾ"����"���ͼ��.
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
            case ISMine:             // ��λ������.
                if (m_bPlayerFailed)
                {
                    if (i == m_FailedPoint.x && j == m_FailedPoint.y)
                    {
                        Bitmap.LoadBitmap(IDB_EXPLOSION);     // ���ر�ը����λͼ.
                    }
                    else
                    {
                        Bitmap.LoadBitmap(IDB_CELL);          // ������ͨ����λͼ.
                    }
                }
                else
                {
                    Bitmap.LoadBitmap(IDB_BLANK);
                }
                break;

            case No_Mine:            // ��λ����Χ����.
                Bitmap.LoadBitmap(IDB_BLANK);
                break;

            case One_Mine:           // ��λ����Χ��1����.
                Bitmap.LoadBitmap(IDB_ONE_CELL);
                break;

            case Two_Mines:          // ��λ����Χ��2����.
                Bitmap.LoadBitmap(IDB_TWO_CELL);
                break;

            case Three_Mines:        // ��λ����Χ��3����.
                Bitmap.LoadBitmap(IDB_THREE_CELL);
                break;

            case Four_Mines:         // ��λ����Χ��4����.
                Bitmap.LoadBitmap(IDB_FOUR_CELL);
                break;

            case Five_Mines:         // ��λ����Χ��5����.
                Bitmap.LoadBitmap(IDB_FIVE_CELL);
                break;

            case Six_Mines:          // ��λ����Χ��6����.
                Bitmap.LoadBitmap(IDB_SIX_CELL);
                break;

            case Seven_Mines:        // ��λ����Χ��7����.
                Bitmap.LoadBitmap(IDB_SEVEN_CELL);
                break;

            case Eight_Mines:        // ��λ����Χ��8����.
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
        // ����λ�����δ���,����֮.
        // ����ȡ�����.
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

            if (m_ppCells[i][j].m_bChecked)          // ����Ѿ����.
            {
                --m_uCheckedCount;                   // ���ѱ�ǵ���Ŀ��1.
                m_ppCells[i][j].m_bChecked = FALSE;  // ����λ������Ϊδ���.
            }
            else
            {
                ++m_uCheckedCount;                   // ���ѱ����Ŀ��1.
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
        // ���İ�ťСͷ�����ʾ.
        //
        m_NewGameBtn.LoadBitmaps(IDB_WILL_RESULT);
        m_NewGameBtn.Invalidate();

        int i = (point.x - m_uCellWidth) / m_uCellWidth;
        int j = (point.y - m_uCellHeight) / m_uCellHeight - 1;

        if (i < 0 || i >= m_uWideCellCount || j < 0 || j >= m_uHeighCellCount)
            return ;

        m_bGamePaused = FALSE;                // ���Ϊ��Ϸδ��ͣ.
        GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_PAUSE_GAME, MF_BYCOMMAND | MF_UNCHECKED);

        CBitmap   Bitmap;
        CClientDC dc(this);
        CDC       CompatiDC;

        point.x = m_cuxOffset;
        point.y = m_cuyOffset;
        dc.SetViewportOrg(point);             // ���������ӿ�ԭ��.

        CompatiDC.CreateCompatibleDC(&dc);    // ��������ͼ����DC.

        if (!m_bFirstClicked)                  // ���ǵ�һ�ε���,��ʼ����,Ҫȷ������.
        {
            // ����ÿ��һ�εļ�ʱ��.��ʼ��ʱ.
            SetTimer(Game_Timer, 1000, NULL);

            //
            // �ڳ��˱�λ��֮��ĵط�������õ���.
            //
            srand((unsigned int)time(NULL));  // ��ʼ�������������.
            for (int MineIndex=0; MineIndex != m_uMineCount; ++MineIndex)
            {
                while(TRUE)
                {
                    int xPos = rand() % m_uWideCellCount;
                    int yPos = rand() % m_uHeighCellCount;

                    // ���������ɵ�λ�û�û����,�����һ����.
                    if (m_ppCells[xPos][yPos].m_iStatus != ISMine && xPos != i && yPos != j)
                    {
                        m_ppCells[xPos][yPos].m_iStatus = ISMine;
                        break;
                    }
                }
            }

            ComputeCellSpread();            // ���ײ�������׷ֲ�.
            m_bFirstClicked = TRUE;         // ��һ�ε����Ѿ����.
        }

        if (m_ppCells[i][j].m_bChecked)     // �����λ���Ѿ����ñ��,����ʾ"�������"ͼ��.
        {
            if (m_bSoundPlay)
            {
                MessageBeep(MB_OK);         // �����޷���������.
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
        // �����λ��û���������߱�����������Χ����,�����"�����հ�"ͼ��.
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
        // �����λ����Χ�������Ѿ�������,����Χ�ķ�Χ�����.
        //
        if (m_ppCells[i][j].m_bClicked && m_ppCells[i][j].m_iStatus != No_Mine)
        {
            int iCheckedRightCount(0);                   // ��Χ��ǶԵļ�����.

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
                                ++iCheckedRightCount;    // �����Χ�б�ǶԵ���,�򽫼�������1.
                            }
                        }
                    }
                }
            } // xPos For end here.

            //
            // �����λ����Χ��ǵ��׵���Ŀ�Ƿ���ȷ.
            // �����ȷ,������Χ����չ����.
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
                                    ++m_uClickedCount;   // ������������Ŀ��1.
                                }
                            }
                        }
                    }
                } // xPos For end here.

                if (PlayerWin())                          // �������Ƿ�Ӯ����Ϸ.
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
        // ���İ�ťСͷ�����ʾ.
        //
        m_NewGameBtn.LoadBitmaps(IDB_READY);
        m_NewGameBtn.Invalidate();

        int i = (point.x - m_uCellWidth) / m_uCellWidth;
        int j = (point.y - m_uCellHeight) / m_uCellHeight - 1;

        if (i < 0 || i >= m_uWideCellCount || j < 0 || j >= m_uHeighCellCount)
            return ;

        //
        // ����˵��ǵ�����δ���ʧ��,����ʾ���е���,������Ϸ.
        //
        if (m_ppCells[i][j].m_iStatus == ISMine && !m_ppCells[i][j].m_bChecked)
        {
            // Ϊ��ť�������ʧ��ͼƬ.
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

            m_bPlayerFailed = TRUE;                // ���Ϊ���ʧ��.
            m_FailedPoint.x = i;
            m_FailedPoint.y = j;

            KillTimer(Game_Timer);                 // �رռ�ʱ��.
            DrawScance();

            // ����ͣ�˵����Ϊ������.
            GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, TRUE);

            return ;
        }

        //
        // ����λ�û�δ�������Ҵ�λ��û�б����,�����֮.
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

            // ���Ϊ�ѵ���.
            m_ppCells[i][j].m_bClicked = TRUE;
            ++m_uClickedCount;

            if (PlayerWin())
            {
                // �������Ƿ�Ӯ����Ϸ.
                return ;
            }

            if (m_ppCells[i][j].m_iStatus == No_Mine)
            {
                // ����Χ�Ŀհ׸���չ����.
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
    // �ٽ���Χ��һСƬ�����׵������ǳ���.
    //
    for (int i=uxPos-1; i<uxPos+2; ++i)
    {
        if (i >= 0 && i < m_uWideCellCount)
        {
            // ȷ�������ܳ���.
            for (int j=uyPos-1; j<uyPos+2; ++j)
            {
                if (j >=0 && j< m_uHeighCellCount)
                {
                    // ȷ�������ܳ���.
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
        // �������Ƿ�Ӯ����Ϸ.
        return ;
    }
}


BOOL CSweepMineDlg::PlayerWin()
{
    //
    // �������Ƿ�Ӯ����Ϸ.
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

        KillTimer(Game_Timer);               // �رռ�ʱ��.
        m_uCheckedCount = m_uMineCount;      // ���������Ϊ�׵���Ŀ.
        m_bPlayerFailed = TRUE;              // ��Ȼ����,�����ʵ����Ӯ��.

        DrawScance();
        m_NewGameBtn.LoadBitmaps(IDB_READY, IDB_WILL_RESULT);

        // ����ͣ�˵����Ϊ������.
        GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_PAUSE_GAME, TRUE);

        // ��Ӣ����Ϣд��ע���.
        HeroRegiste();
        return TRUE;
    }

    return FALSE;
}


void CSweepMineDlg::HeroRegiste()
{
    HKEY hRegKey = NULL;
    CString szKeyInfo = _T("Software\\SweepMine1.0");

    // ���������Ϣ�е� "SweepMine1.0" ��.���ü��Ѿ�����,���֮.
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
        // ��Ȼ����,�����ʵ����Ӯ��.
        m_bPlayerFailed = TRUE;
        MessageBox(_T("���!��Ӯ��!!!"), _T("��Ӯ��"), MB_OK | MB_ICONINFORMATION);
        return ;
    }

    RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo, &hRegKey);
    CString szTime = CHeroPageDialog::GetRegisterValue(hRegKey, Used_Times);

    DWORD dwBestUsedTime =  wcstoul(szTime, '\0', 10);     // ��õ���Ϸ��¼.
    if (szTime == "" || m_dwUsedTime < dwBestUsedTime)     // ��Ҵ����˼�¼.
    {
        CWinedPlayer PlayerWinDialog;
        if (IDOK == PlayerWinDialog.DoModal())
        {
            HKEY hHeroNameKey      = NULL;
            HKEY hHeroUsedTimesKey = NULL;

            // ����������ʱע���.
            RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo + _T("\\Name"), &hHeroNameKey);
            RegCreateKey(HKEY_LOCAL_MACHINE, szKeyInfo + _T("\\UsedTimes"), &hHeroUsedTimesKey);

            // ������д��ע���.
            RegSetValue(
                hHeroNameKey,
                NULL,
                REG_SZ,
                PlayerWinDialog.GetPlayerName(),
                PlayerWinDialog.GetPlayerName().GetLength()
                );

            // ����ʱд��ע���.
            szTime.Format(_T("%d"), m_dwUsedTime);
            RegSetValue(
                hHeroUsedTimesKey,
                NULL,
                REG_SZ,
                szTime,
                szTime.GetLength()
                );

            // �ر���������ʱע���.
            RegCloseKey(hHeroNameKey);
            RegCloseKey(hHeroUsedTimesKey);
        }
    }
    else if (m_dwUsedTime == dwBestUsedTime)
    {
        // ���ƽ����Ѽ�¼.
        MessageBox(
            _T("������,���Ѿ�ƽ����Ѽ�¼!����Ŭ��!!"),
            _T("Good Job!"),
            MB_OK | MB_ICONINFORMATION
            );
    }
    else
    {
        // ���û�д��Ƽ�¼,����ʾ��ʾ��Ϣ.
        CString Str;
        Str.Format(
            _T("����Ŭ��,�㻹�� %d ��ʹ����˼�¼!"),
            m_dwUsedTime - dwBestUsedTime
            );
        MessageBox(Str, _T("��Ӯ��!"), MB_OK | MB_ICONINFORMATION);
    }

    if (hRegKey)
    {
        RegCloseKey(hRegKey);
    }
}


//
// Ϊÿһλ�ü�����׷ֲ�����.
//
void CSweepMineDlg::ComputeCellSpread()
{
    for (int i = 0; i != m_uWideCellCount; ++i)
    {
        for (int j = 0; j != m_uHeighCellCount; ++j)
        {
            if (m_ppCells[i][j].m_iStatus == ISMine)
            {
                // �����λ���ǵ���,��Ϊ����Χ�ĸ��Ӽ�����׷ֲ�.
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
                                    // ������ܲ��ǵ���,��֮��������1.
                                    ++m_ppCells[m][n].m_iStatus;
                                }
                            }
                        }
                    }
                } // ��Χ�ֲ��������.
            }
        } // ���е��׷������.
    } //For End Here.
}


void CSweepMineDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case Game_Timer:            // ��Ϸ��ʱ��.ÿ1s��һ��.
        if (!m_bGamePaused)
        {
            ++m_dwUsedTime;     // �����Ϸ��δ��ͣ,����Ϸ��ʱ��1.
            DrawTimer();        // ���¼�ʱ��.
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
    // ���Ƽ�ʱ������. ��ʱ����ʽ: ��:��,����"��"ռ��λ,"��"ռ��λ.
    //
    UINT uTimeFormat[4];
    uTimeFormat[0] = m_dwUsedTime / 600;      // ��ȡ������.
    uTimeFormat[1] = m_dwUsedTime / 60 - 10 * uTimeFormat[0];

    uTimeFormat[2] = (m_dwUsedTime - uTimeFormat[0] * 600 - uTimeFormat[1] * 60) / 10;     // ��ȡ����.
    uTimeFormat[3] = (m_dwUsedTime - uTimeFormat[0] * 600 - uTimeFormat[1] * 60) - uTimeFormat[2] * 10;

    //
    // ִ�м�ʱ������.
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
            // ����ʱ�ӷָ���.
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
    // ���±����Ŀ�Ļ���.
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
        m_bGamePaused = TRUE;          // ʧȥ����,��ͣ��Ϸ.
    }
    else if (nState == WA_CLICKACTIVE || nState == WA_ACTIVE)
    {
        m_bGamePaused = FALSE;         // ��ý���,������Ϸ.
    }
}


BOOL CSweepMineDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            if (IDYES == MessageBox(
                    _T("�����Ҫ�˳���?"),
                    _T("�˳�"),
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
