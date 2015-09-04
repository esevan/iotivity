/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
  * @file   simulator_manager.h
  *
  * @brief   This file contains the declaration of SimulatorManager class which has the methods
  *              for configuring the platform and creation/deletion of resources.
  */

#ifndef SIMULATOR_MANAGER_H_
#define SIMULATOR_MANAGER_H_

#include <vector>
#include "simulator_resource_server.h"
#include "simulator_remote_resource.h"
#include "simulator_error_codes.h"
#include "simulator_logger.h"

/**
 * @class   SimulatorManager
 *
 * @brief   This class provides a set of methods for platform configuration,
 *              and creation/deletion of resources.
 *
 */
class SimulatorManager
{
    public:
        static SimulatorManager *getInstance();

        /**
         * This method is called for creating a single resource from RAML configuration file.
         *
         * @param configPath - RAML configuration file path.
         * @param callback - Callback method for receive notifications when resource model changes.
         *
         * @return SimulatorResourceServerPtr - Shared pointer of SimulatorResourceServer on success, otherwise NULL.
         */
        SimulatorResourceServerPtr createResource(const std::string &configPath,
                SimulatorResourceServer::ResourceModelChangedCB callback);

        /**
         * This method is called for creating a collection of resources from RAML configuration file.
         *
         * @param configPath - RAML configuration file path.
         * @param count - Number of resource to be created.
         * @param callback - Callback method for receive notifications when resource model changes.
         *
         * @return SimulatorResourceServerPtr - A vector of Shared pointers of SimulatorResourceServer Objects.
         */
        std::vector<SimulatorResourceServerPtr> createResource(const std::string &configPath,
                const int count,
                SimulatorResourceServer::ResourceModelChangedCB callback);

        /**
         * This method is called for obtaining a list of created resources.
         *
         * @return SimulatorResourceServerPtr - A vector of Shared pointers of SimulatorResourceServer Objects.
         */
        std::vector<SimulatorResourceServerPtr> getResources(const std::string &resourceType = "");

        /**
          * This method is called for deleting a single resource.
          *
          * @param resource - Shared pointer of the SimulatorResourceServer to be deleted.
          *
          * @return SimulatorResult
          */
        SimulatorResult deleteResource(SimulatorResourceServerPtr &resource);

        /**
          * This method is called for deleting multiple resources.
          * If this method is called without any parameter, then all resources will be deleted.
          * If thie method is called with a specific resourcetype as a parameter, then all the resources
          * of that particular type will be deleted.
          *
          * @param resourceType - Resource type of the resource
          *
          * @return SimulatorResult
          */
        SimulatorResult deleteResources(const std::string &resourceType = "");

        /**
         * API for discovering resources of a particular resource type.
         * Callback is called when a resource is found.
         *
         * @param resourceType - required resource type
         * @param callback - Returns SimulatorRemoteResource.
         *
         * @return SimulatorResult - return value of this API.
         *                         It returns SIMULATOR_SUCCESS if success.
         *
         * NOTE: SimulatorResult is defined in simulator_error_codes.h.
         */
        SimulatorResult findResource(const std::string &resourceType, ResourceFindCallback callback);

        /**
         * API for getting list of already found resources.
         *
         * @param resourceType - resource type
         *
         * @return List of SimulatorRemoteResource
         *
         */
        std::vector<SimulatorRemoteResourcePtr> getFoundResources(
            const std::string resourceType = "");

        /**
         * API for setting logger target for receiving the log messages.
         *
         * @param logger - ILogger interface for handling the log messages.
         *
         */
        void setLogger(std::shared_ptr<ILogger> logger);

        /**
         * API for setting console as logger target.
         *
         * @return true if console set as logger target,
         *         otherwise false.
         *
         */
        bool setDefaultConsoleLogger();

        /**
         * API for setting file as logger target.
         *
         * @param path - File to which log messages to be saved.
         *
         * @return true if console set as logger target,
         *         otherwise false.
         *
         */
        bool setDefaultFileLogger(std::string &path);

    private:
        SimulatorManager();
        ~SimulatorManager() = default;
};

#endif