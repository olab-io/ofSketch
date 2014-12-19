// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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


/// \brief A specialized ofx::TaskQueue for handling task notifications.
class ProcessTaskQueue: public ofx::TaskQueue
{
public:
    typedef ofx::TaskDataEventArgs_<Poco::UUID, std::string> EventArgs;

    ProcessTaskQueue(int maximumTasks, Poco::ThreadPool& threadPool);

    virtual ~ProcessTaskQueue();

    ofEvent<const EventArgs> onTaskData;

    template<typename ListenerClass>
    void registerAllEvents(ListenerClass* listener);

    template<typename ListenerClass>
    void unregisterAllEvents(ListenerClass* listener);

protected:
    virtual void handleTaskCustomNotification(const Poco::UUID& taskID,
                                              TaskNotificationPtr pNotification);

};


template<typename ListenerClass>
void ProcessTaskQueue::registerAllEvents(ListenerClass* listener)
{
    registerTaskProgressEvents(listener);
    ofAddListener(onTaskData, listener, &ListenerClass::onTaskData);
}


template<typename ListenerClass>
void ProcessTaskQueue::unregisterAllEvents(ListenerClass* listener)
{
    unregisterTaskProgressEvents(listener);
    ofRemoveListener(onTaskData, listener, &ListenerClass::onTaskData);
}


} } // namespace of::Sketch
