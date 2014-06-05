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


#include "ProcessTaskQueue.h"


namespace of {
namespace Sketch {


ProcessTaskQueue::ProcessTaskQueue(int maximumTasks,
                                   Poco::ThreadPool& threadPool):
    ofx::TaskQueue_<std::string>(maximumTasks, threadPool)
{
    registerTaskEvents(this);
}


ProcessTaskQueue::~ProcessTaskQueue()
{
    unregisterTaskEvents(this);
}


void ProcessTaskQueue::handleUserNotification(Poco::AutoPtr<Poco::TaskNotification> task,
                                              const Poco::UUID& taskId,
                                              Poco::Notification::Ptr pNotification)
{
    // TODO: add additional custom types in addition to string.
    ofx::TaskQueue_<std::string>::handleUserNotification(task, taskId, pNotification);
}


bool ProcessTaskQueue::onTaskStarted(const ofx::TaskStartedEventArgs& args)
{
    // Define a task handle.
    TaskProgress task;
    task.name = args.getTaskName();
    task.uuid = args.getTaskId();

    // Add the task.
    tasks[task.uuid] = task;
}


bool ProcessTaskQueue::onTaskCancelled(const ofx::TaskCancelledEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = 0;
    }
    else
    {
        ofLogFatalError("ofApp::onTaskCancelled") << "Unknown UUID.";
    }

    // We did not consume the event, just made note of it.
    return false;
}


bool ProcessTaskQueue::onTaskFinished(const ofx::TaskFinishedEventArgs& args)
{
    std::map<Poco::UUID, TaskProgress>::iterator iter = tasks.find(args.getTaskId());

    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks.erase(iter);
    }
    else
    {
        ofLogFatalError("ofApp::onTaskFinished") << "Unknown UUID.";
    }

    // We did not consume the event, just made note of it.
    return false;
}


bool ProcessTaskQueue::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = 0;
        tasks[args.getTaskId()].message = args.getException().displayText();
    }
    else
    {
        ofLogFatalError("ofApp::onTaskFailed") << "Unknown UUID.";
    }

    // We did not consume the event, just made note of it.
    return false;
}


bool ProcessTaskQueue::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = args.getProgress();
    }
    else
    {
        ofLogFatalError("ofApp::onTaskProgress") << "Unknown UUID.";
    }

    // We did not consume the event, just made note of it.
    return false;
}


bool ProcessTaskQueue::onTaskData(const ofx::TaskDataEventArgs<std::string>& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].message = args.getData();
    }
    else
    {
        ofLogFatalError("ofApp::onTaskData") << "Unknown UUID.";
    }

    // We did not consume the event, just made note of it.
    return false;
}


Json::Value ProcessTaskQueue::toJson() const
{
    /// TODO: This method may need to be syncrhonized (which means that
    /// all of our ProcessTaskQueue may need to be synchronized depending on
    /// how our websocket route is synchronized.

    Json::Value json;

    std::map<Poco::UUID, TaskProgress>::const_iterator iter = tasks.begin();

    while (iter != tasks.end())
    {
        // Add each of the current tasks to the json array.
        json.append(iter->second.toJson());
        ++iter;
    }

    return json;
}


} } // namespace of::Sketch
