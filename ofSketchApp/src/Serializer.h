// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
// Copyright (c) 2014 Brannon Dorsey <http://brannondorsey.com>
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


#include "json/json.h"


namespace of {
namespace Sketch {


class Settings;
class ClientSettings;
class Paths;
class SSLSettings;
class ServerSettings;
class Addon;


class Serializer
{
public:
    static bool fromFile(const std::string& path, Json::Value& value);
    static bool toFile(const std::string& path, const Json::Value& value);
    static bool fromString(const std::string& jsonString, Json::Value& value);

    /// \brief Load ofSketch settings from the disk.
    /// \returns The default settings from disk, or default settings.
    static Settings loadSettings();

    /// \brief Save settings to the default user settings location.
    /// \param settings The settings to save.
    /// \returns true if the settings were saved successfully.
    static bool saveSettings(const Settings& settings);

    // This is a utility method for quickly converting a json value to a string.
    static std::string toString(const Json::Value& json);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, Settings& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Settings& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, ClientSettings& object);
    
    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const ClientSettings& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, Paths& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Paths& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, ServerSettings& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const ServerSettings& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, SSLSettings& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const SSLSettings& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Addon& object);

};


} } // namespace of::Sketch
