// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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
#include "json/json.h"


namespace of {
namespace Sketch {


/// \brief A collection of ofSketch clients settings.
class ClientSettings
{
public:
    // \brief Create a default ClientSettings instance.
    ClientSettings();

    /// \brief Destroy the instance of ClientSettings.
    virtual ~ClientSettings();

    /// \brief Get autosave status.
    /// \returns true iff autosave is enabled.
    bool isAutosaveEnabled() const;

    /// \brief Set autosave status.
    /// \param isAutosaveEnabled Set true to enable autosaving.
    void setAutosaveEnabled(bool isAutosaveEnabled);

    /// \brief Get the autosave frequency.
    /// \returns the autosave frequency in milliseconds.
    uint64_t getAutosaveFrequency() const;

    /// \brief Set the autosave frequency.
    /// \param autosaveFrequency The autosave frequency in milliseconds.
    void setAutosaveFrequency(uint64_t autosaveFrequency);

    /// \brief Get the ACE editor settings.
    /// \warning These settings are not validated on the server side.
    /// \returns the ACE editor settings as JSON.
    const Json::Value& getEditorSettings() const;

    /// \brief Set the ace editor settings.
    /// \warning These settings are not validated on the server side.
    /// \param editorSettings The ACE editor settings.
    void setEditorSettings(const Json::Value& editorSettings);

    enum
    {
        /// \brief The default autosave frequency in milliseconds.
        DEFAULT_AUTOSAVE_FREQUENCY = 60000
    };

    /// \brief The default text editor settings.
    static const std::string DEFAULT_EDITOR_SETTINGS;

private:
	/// \brief True if autosave is enabled.
	bool _isAutosaveEnabled;

	/// \brief Autosave frequency in milliseconds.
	uint64_t _autosaveFrequency;

	/// \brief The ACE editor settings as JSON.
	Json::Value _editorSettings;
    
};


} } // namespace of::Sketch
