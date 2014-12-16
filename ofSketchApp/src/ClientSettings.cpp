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


#include "ClientSettings.h"
#include "ofLog.h"


namespace of {
namespace Sketch {


const std::string ClientSettings::DEFAULT_EDITOR_SETTINGS = "{\
        \"setDisplayIndentGuides\": true,   \
        \"setDragDelay\": true,             \
        \"setFadeFoldWidgets\": false,      \
        \"setFontSize\": 12,                \
        \"setHighlightActiveLine\": false,  \
        \"setHighlightGutterLine\": true,   \
        \"setHighlightSelectedWord\": false,\
        \"setMode\": \"ace/mode/c_cpp\",    \
        \"setNewLineMode\": \"auto\",       \
        \"setOverwrite\": false,            \
        \"setPrintMarginColumn\": \"80\",   \
        \"setReadOnly\": false,             \
        \"setScrollSpeed\": 2,              \
        \"setShowFoldWidgets\": true,       \
        \"setShowInvisibles\": false,       \
        \"setShowPrintMargin\": true,       \
        \"setTabSize\": 4,                  \
        \"setTheme\": \"ace/theme/twilight\",\
        \"setUseSoftTabs\": true,           \
        \"setUseWrapMode\": false,          \
        \"setWrapLimit\": 80                \
    }";


ClientSettings::ClientSettings():
    _isAutosaveEnabled(true),
    _autosaveFrequency(DEFAULT_AUTOSAVE_FREQUENCY)
{
    Json::Reader reader;

    if (!reader.parse(DEFAULT_EDITOR_SETTINGS, _editorSettings))
    {
        std::stringstream ss;
        ss <<  "Unable to parse " << DEFAULT_EDITOR_SETTINGS;
        ss << ": " << reader.getFormattedErrorMessages();
        ofLogError("ClientSettings::ClientSettings") << ss;
    }
}

    
ClientSettings::~ClientSettings()
{
}

    
bool ClientSettings::isAutosaveEnabled() const
{
    return _isAutosaveEnabled;
}

    
void ClientSettings::setAutosaveEnabled(bool isAutosaveEnabled)
{
    _isAutosaveEnabled = isAutosaveEnabled;
}

    
unsigned long long ClientSettings::getAutosaveFrequency() const
{
    return _autosaveFrequency;
}


void ClientSettings::setAutosaveFrequency(unsigned long long autosaveFrequency)
{
    _autosaveFrequency = autosaveFrequency;
}


const Json::Value& ClientSettings::getEditorSettings() const
{
    return _editorSettings;
}


void ClientSettings::setEditorSettings(const Json::Value& editorSettings)
{
    _editorSettings = editorSettings;
}


} } // namespace of::Sketch
