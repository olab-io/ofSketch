// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include <vector>
#include <json/json.h>
#include "ofx/TaskQueue.h"


namespace of {
namespace Sketch {


class TaskProgress
{
public:
    TaskProgress(): progress(0)
    {
    }

    Json::Value toJson() const
    {
        Json::Value progressJson;

        progressJson["name"] = name;
        progressJson["uuid"] = uuid.toString();
        progressJson["progress"] = progress;
        progressJson["message"] = message;

        return progressJson;
    }

    std::string name;
    Poco::UUID uuid;
    float progress;
    std::string message;


};


class ProcessTaskQueue: public ofx::TaskQueue_<std::string>
{
public:
    ProcessTaskQueue(int maximumTasks, Poco::ThreadPool& threadPool);

    virtual ~ProcessTaskQueue();

    bool onTaskStarted(const ofx::TaskStartedEventArgs& args);
    bool onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    bool onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    bool onTaskFailed(const ofx::TaskFailedEventArgs& args);
    bool onTaskProgress(const ofx::TaskProgressEventArgs& args);
    bool onTaskData(const ofx::TaskDataEventArgs<std::string>& args);

    Json::Value toJson() const;

protected:
    virtual void handleUserNotification(Poco::AutoPtr<Poco::TaskNotification> task,
                                        const Poco::UUID& taskId,
                                        Poco::Notification::Ptr pNotification);

private:
    std::map<Poco::UUID, TaskProgress> tasks;

};


} } // namespace of::Sketch
