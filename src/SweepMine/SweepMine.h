/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"


class CSweepMineApp : public CWinApp
{
public:
    CSweepMineApp();


public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};


extern CSweepMineApp theApp;
