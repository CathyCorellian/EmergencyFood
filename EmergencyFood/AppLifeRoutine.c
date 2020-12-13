﻿/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Life routine of this app
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <wchar.h>
#include "CQAPITransfer.h"
#include "AppLifeRoutine.h"
#include "GenshinGetGameRecord.h"

//生命周期

int AppInitialize()//初始化时会被执行
{
    
    return 0;
}

int AppFinialize()//结束时会被执行
{
    return 0;
}

int AppEnabled()//启用时执行（如果初始化时是启用的，会在AppInitialize后执行一次）
{
    return 0;
}

int AppDisabled()//禁用时执行（如果结束时是启用的，会在AppFinialize前执行一次，这点和cq原生函数不一样）
{
    return 0;
}

int OnRecvMessage(int msgId, MESSAGE_SOURCE MessageSource, LPCWSTR szMsg, int font)
{
    if (lstrcmpW(szMsg, L"应急食品") == 0)
    {
        SendBackMessage(MessageSource, L"我！才！不！是！应！急！食！品！！");
    }
    else
    {
        long long UID;
        if (swscanf_s(szMsg, L"原神查询%lld", &UID) == 1)
        {
            if (UID >= 1000000000 || UID < 100000000)
            {
                SendBackMessage(MessageSource, L"UID长度错误，请检查输入的UID是否正确");
            }
            else
            {
                WCHAR szUID[10];
                swprintf(szUID, _countof(szUID), L"%lld", UID);
                USER_GAME_RECORD_RESULT GameRecordResult;
                BOOL bSuccess = GenshinGetUserGameRecord(szUID, &GameRecordResult);

                if (bSuccess)
                {
                    SendBackMessage(MessageSource, L"查询成功");
                }
                else
                {
                    SendBackMessage(MessageSource, L"查询失败QAQ");
                }
            }
        }
    }
    
    return 0;
}

