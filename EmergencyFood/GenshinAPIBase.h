/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic function and tools for calling Genshin API
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include <WinINet.h>
#include <atlcoll.h>
#include "yyjson.h"


class GENSHIN_API_RESULT
{
private:
    yyjson_doc *JsonDoc;

public:
    GENSHIN_API_RESULT()
    {
        JsonDoc = NULL;
    }

    ~GENSHIN_API_RESULT()
    {
        if (JsonDoc)
        {
            yyjson_doc_free(JsonDoc);
        }
    }

    // information retrieved from json
    int RetCode;
    yyjson_val *nodeData;
    
    friend BOOL GenshinAPISendRequest(HINTERNET hRequest, GENSHIN_API_RESULT &GenshinAPIResult);
};

BOOL GenshinAddAPIHttpHeader(HINTERNET hRequest);

BOOL GenshinAPISendRequest(HINTERNET hRequest, GENSHIN_API_RESULT &GenshinAPIResult);
