//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "CacheHandler.h"

#include <memory>
#include <cstdlib>
#include <functional>
#include <map>
#include <utility>
#include <ctime>

#include "OCApi.h"

CacheHandler::CacheHandler(
            ServiceResource & pResource,
            CacheCB func,
            REPORT_FREQUENCY rf,
            long repeatTime
            ):sResource(pResource)
{
    subscriberList = new SubscriberInfo();
    data = new(CacheData());

    state = CACHE_STATE::READY_YET;
    updateTime = 0l;

    pObserveCB = (ObserveCB)(std::bind(&CacheHandler::onObserve, this,
            std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4));
    pGetCB = (GetCB)(std::bind(&CacheHandler::onGet, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    if(pResource.isObservable())
    {
        pResource.requestObserve(pObserveCB);
    }
    else
    {
        // TODO set timer
    }
}

CacheHandler::~CacheHandler()
{
    // TODO Auto-generated destructor stub
    data.reset();

    // TODO delete all node!!
    subscriberList->clear();
    delete subscriberList;
}

CacheID CacheHandler::addSubscriber(CacheCB func, REPORT_FREQUENCY rf, long repeatTime)
{
    Report_Info newItem;
    newItem.rf = rf;
    newItem.latestReportTime = 0l;
    newItem.repeatTime = repeatTime;

    srand(time(NULL));
    newItem.reportID = rand();

    while(1)
    {
        if(findSubscriber(newItem.reportID) != nullptr || newItem.reportID == 0)
        {
            newItem.reportID = rand();
        }
        else
        {
            break;
        }
    }

    subscriberList->insert(SubscriberInfoPair(newItem, func));

    return newItem.reportID;
}

CacheID CacheHandler::deleteSubscriber(CacheID id)
{
    CacheID ret = 0;

    SubscriberInfoPair pair = findSubscriber(id);
    if(pair != nullptr)
    {
        ret = pair.first.reportID;
        subscriberList->erase(pair.first);
    }

    return ret;
}

SubscriberInfoPair CacheHandler::findSubscriber(CacheID id)
{
    SubscriberInfoPair ret = nullptr;

    for(auto i : subscriberList)
    {
        if(i->first.reportID == id)
        {
            ret = i;
        }
    }

    return ret;
}

std::shared_ptr<CacheData> CacheHandler::getCachedData()
{
    if(state != CACHE_STATE::READY)
    {
        return NULL;
    }
    return data;
}

ServiceResource * CacheHandler::getServiceResource()
{
    ServiceResource ret = NULL;
    if(sResource)
    {
        ret = sResource;
    }

    return ret;
}

void CacheHandler::onObserve(
        const HeaderOptions& ho, const ResponseStatement& _rep, int _result, int _seq)
{

    if(_result != OC_STACK_OK)
    {
        // TODO handle error
        return;
    }
//    if(_rep.empty() || _rep.emptyData())
    if(_rep.getAttributes().isEmpty())
    {
        return;
    }

    ResourceAttributes att = _rep.getAttributes();

    // set data
    data->clear();
    OC::OCRepresentation::iterator it = att.begin();
    for(; att.end(); ++it)
    {
        std::string key = it->attrname();
        // TODO change template or variant
        std::string val = it->getValueToString();

        data[key] = val;
    }

    // notify!!
    std::map::iterator mapiter = subscriberList->begin();
    for(; subscriberList->end(); ++mapiter)
    {
        if(mapiter->first().rf == REPORT_FREQUENCY::UPTODATE)
        {
            // TODO update report time
//            mapiter->first().latestReportTime = now;
            mapiter->second()(this->sResource, data);
        }
    }
}
