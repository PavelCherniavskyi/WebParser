/**
 * This file implements LibCurl library initialisation
 * @file LibCurlConfig.cpp
 * @copyright (C) 2015 Harman Becker Automotive Systems GmbH
 *
 * @author Aleksandr Stankov <astankov@luxoft.com>
 */

#include "curl/curl.h"
#include "LibCurlConfig.h"
#include "../../../SmartLogger/src/SmartLogger.h"

bool LibCurlConfig::initialysed = false;

LibCurlConfig::LibCurlConfig()
{
}

LibCurlConfig::~LibCurlConfig()
{
    if (true == initialysed) {
        curl_global_cleanup();
        initialysed = false;
    }
}

void LibCurlConfig::init()
{
    if (!initialysed) {
        INFO() << "Initialise libCurl";

        if (CURLE_OK == curl_global_init(CURL_GLOBAL_ALL)) {
            INFO() << "Initialisation done. LibCurl version: " << curl_version();
            initialysed = true;
        } else {
            WARN() << "Initialisation failed";
        }
    } else {
        WARN() << "Already initialised";
    }
}
