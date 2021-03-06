/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting user game record
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <WinInet.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <cstdlib>
#include "GenshinAPIGetGameRecord.h"
#include "GenshinBasic.h"
#include "GenshinAPIBase.h"
#include "md5.h"
#include "yyjson.h"

BOOL AvatarsJsonAnalysis(yyjson_val *nodeAvatars, ATL::CAtlArray<GENSHIN_AVATAR_DATA> &AvatarData)
{
    if (!nodeAvatars)
    {
        return FALSE;
    }

    int iAvatarCount = yyjson_arr_size(nodeAvatars);

    yyjson_val *AvatarEnum =  yyjson_arr_get_first(nodeAvatars);

    AvatarData.SetCount(iAvatarCount);

    for (int iAvatarIndex = 0; iAvatarIndex < iAvatarCount; iAvatarIndex++)
    {
        yyjson_val *nodeID = yyjson_obj_get(AvatarEnum, "id");
        yyjson_val *nodeElement = yyjson_obj_get(AvatarEnum, "element");
        yyjson_val *nodeFetter = yyjson_obj_get(AvatarEnum, "fetter");
        yyjson_val *nodeLevel = yyjson_obj_get(AvatarEnum, "level");

        AvatarData[iAvatarIndex].AvatarID = yyjson_get_int(nodeID);
        AvatarData[iAvatarIndex].Element = ElementTextToEnum(yyjson_get_str(nodeElement));
        AvatarData[iAvatarIndex].Fetter = yyjson_get_int(nodeFetter);
        AvatarData[iAvatarIndex].Level = yyjson_get_int(nodeLevel);

        AvatarEnum = unsafe_yyjson_get_next(AvatarEnum);
    }

    return TRUE;
}

BOOL StatsJsonAnalysis(yyjson_val *nodeStats, GENSHIN_STATS_DATA &StatsData)
{
    if (!nodeStats)
    {
        return FALSE;
    }

    yyjson_val *nodeActiveDayNumber = yyjson_obj_get(nodeStats, "active_day_number");
    yyjson_val *nodeAchievementNumber = yyjson_obj_get(nodeStats, "achievement_number");
    yyjson_val *nodeAnemoculusNumber = yyjson_obj_get(nodeStats, "anemoculus_number");
    yyjson_val *nodeGeoculusNumber = yyjson_obj_get(nodeStats, "geoculus_number");
    yyjson_val *nodeAvatarNumber = yyjson_obj_get(nodeStats, "avatar_number");
    yyjson_val *nodeWayPointNumber = yyjson_obj_get(nodeStats, "way_point_number");
    yyjson_val *nodeDomainNumber = yyjson_obj_get(nodeStats, "domain_number");
    yyjson_val *nodeSpiralAbyss = yyjson_obj_get(nodeStats, "spiral_abyss");
    yyjson_val *nodePreciousChestNumber = yyjson_obj_get(nodeStats, "precious_chest_number");
    yyjson_val *nodeLuxuriousChestNumber = yyjson_obj_get(nodeStats, "luxurious_chest_number");
    yyjson_val *nodeExquistieChestNumber = yyjson_obj_get(nodeStats, "exquisite_chest_number");
    yyjson_val *nodeCommonChestNumber = yyjson_obj_get(nodeStats, "common_chest_number");


    StatsData.ActiveDayNumber = yyjson_get_int(nodeActiveDayNumber);
    StatsData.AchievementNumber = yyjson_get_int(nodeAchievementNumber);
    StatsData.AnemoculusNumber = yyjson_get_int(nodeAnemoculusNumber);
    StatsData.GeoculusNumber = yyjson_get_int(nodeGeoculusNumber);
    StatsData.AvatarNumber = yyjson_get_int(nodeAvatarNumber);
    StatsData.WayPointNumber = yyjson_get_int(nodeWayPointNumber);
    StatsData.DomainNumber = yyjson_get_int(nodeDomainNumber);
    MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeSpiralAbyss), -1, StatsData.SpiralAbyss, _countof(StatsData.SpiralAbyss));
    StatsData.PreciousChestNumber = yyjson_get_int(nodePreciousChestNumber);
    StatsData.LuxuriousChestNumber = yyjson_get_int(nodeLuxuriousChestNumber);
    StatsData.ExquistieChestNumber = yyjson_get_int(nodeExquistieChestNumber);
    StatsData.CommonChestNumber = yyjson_get_int(nodeCommonChestNumber);

    return TRUE;
}

BOOL WorldExplorationsJsonAnalysis(yyjson_val *nodeWorldExplorations, ATL::CAtlArray<GENSHIN_EXPLORATION_DATA> &ExplorationData)
{
    if (!nodeWorldExplorations)
    {
        return FALSE;
    }

    int iRegionCount = yyjson_arr_size(nodeWorldExplorations);

    yyjson_val *RegionEnum = yyjson_arr_get_first(nodeWorldExplorations);

    ExplorationData.SetCount(iRegionCount);

    for (int iRegionIndex = 0; iRegionIndex < iRegionCount; iRegionIndex++)
    {
        yyjson_val *nodeLevel = yyjson_obj_get(RegionEnum, "level");
        yyjson_val *nodeExplorationPercentage = yyjson_obj_get(RegionEnum, "exploration_percentage");
        yyjson_val *nodeName = yyjson_obj_get(RegionEnum, "name");

        ExplorationData[iRegionIndex].Level = yyjson_get_int(nodeLevel);
        ExplorationData[iRegionIndex].ExplorationPercentage = yyjson_get_int(nodeExplorationPercentage);
        MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeName), -1, ExplorationData[iRegionIndex].Name, _countof(ExplorationData[iRegionIndex].Name));

        RegionEnum = unsafe_yyjson_get_next(RegionEnum);
    }

    return TRUE;
}

BOOL UserGameRecordJsonAnalysis(GENSHIN_USER_GAME_RECORD_RESULT &Result)
{
    yyjson_val *nodeData = Result.nodeData;
    if (!nodeData)
    {
        return FALSE;
    }

    yyjson_val *nodeAvatars = yyjson_obj_get(nodeData, "avatars");
    yyjson_val *nodeStats = yyjson_obj_get(nodeData, "stats");
    yyjson_val *nodeWorldExplorations = yyjson_obj_get(nodeData, "world_explorations");

    if (nodeAvatars)
    {
        AvatarsJsonAnalysis(nodeAvatars, Result.AvatarData);
    }

    if (nodeStats)
    {
        StatsJsonAnalysis(nodeStats, Result.StatsData);
    }

    if (nodeWorldExplorations)
    {
        WorldExplorationsJsonAnalysis(nodeWorldExplorations, Result.ExploationData);
    }

    return TRUE;
}

extern "C" BOOL GenshinAPIGetUserGameRecord(const WCHAR UID[], GENSHIN_USER_GAME_RECORD_RESULT &Result)
{
    if (!UID)
    {
        return FALSE;
    }

    // construct request URL first
    ATL::CStringW RequestURL;
    if (lstrlenW(UID) != 9)
    {
        // Wrong UID
        return FALSE;
    }

    const WCHAR *ServerName = GetServerNameByUID(UID);
    if (!ServerName)
    {
        // server not supported, or wrong UID
        return FALSE;
    }
    RequestURL.Format(L"/game_record/genshin/api/index?server=%ls&role_id=%ls", ServerName, UID);
    
    const WCHAR *rgpszAcceptTypes[] = { L"*/*", NULL };

    HINTERNET hRequest = HttpOpenRequestW(GetmiHoYoServerConnect(), L"GET", RequestURL,
        NULL, NULL, rgpszAcceptTypes, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE, NULL);

    if (!hRequest)
    {
        return FALSE;
    }
    
    if (!GenshinAPISendRequest(hRequest, Result, NULL, NULL))
    {
        InternetCloseHandle(hRequest);
        return FALSE;
    }
    
    BOOL bAnalysisResult = UserGameRecordJsonAnalysis(Result);

    InternetCloseHandle(hRequest);

    return bAnalysisResult;
}