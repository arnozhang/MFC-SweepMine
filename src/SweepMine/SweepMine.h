/**
 * Copyright (C) 2010. All Rights Reserved.
 *
 * @author  Arno Zhang
 * @email   zyfgood12@163.com
 * @date    2010/05/12
 */

#pragma once


#ifndef __AFXWIN_H__
    #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
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
