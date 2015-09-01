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


#include "Poco/Path.h"


namespace of {
namespace Sketch {


/// \brief A collection of important ofSketch paths.
class Paths
{
public:
    /// \brief Create a default Paths instance.
    Paths();

    /// \brief Destroy the Paths instance.
    virtual ~Paths();

    /// \brief Get the projects path.
    /// \returns The absolute projects path.
    Poco::Path getProjectsPath() const;

    /// \brief Set the projects path.
    /// \projectsPath The absolute projects path.
    void setProjectsPath(const Poco::Path& projectsPath);

    /// \brief Get the openFrameworks core path.
    ///
    /// This is the location of the openFrameworks core folder.
    ///
    /// \returns The absolute path to the openFrameworks core folder.
    Poco::Path getOpenFrameworksPath() const;

    /// \brief Get the core addons path.
    ///
    /// This is the addons/ path within the openFrameworks path.
    ///
    /// \returns The absolute openFrameworks/addons path.
    Poco::Path getCoreAddonsPath() const;

    /// \brief Get the examples path.
    ///
    /// This is the path containing the default ofSketch examples.  This path
    /// is within the application's data folder.
    ///
    /// \returns The absolute examples path.
    Poco::Path getExamplesPath() const;

    /// \brief Get the resources path.
    ///
    /// This is the path containing the ofSketch resources.  This path
    /// is within the application's data folder.
    ///
    /// \returns The absolute resources path.
    Poco::Path getResourcesPath() const;

    /// \brief Get the templates path.
    ///
    /// This is the path containing the ofSketch templates.  This path
    /// is within the application's data folder.
    ///
    /// \returns The absolute templates path.
    Poco::Path getTemplatesPath() const;

    /// \brief Get the toolchains path.
    ///
    /// This is the path containing the ofSketch toolchains.  This path
    /// is within the application's data folder.
    ///
    /// \returns The absolute toolchains path.
    Poco::Path getToolchainsPath() const;

    /// \brief Get the addons path.
    ///
    /// This is the path containing the user's addons.  This path is always
    /// within the user's projects path.
    ///
    /// \returns The absolute path to the user's addons.
    Poco::Path getAddonsPath() const;

    /// \brief Get the settings path.
    ///
    /// This is the path containing the ofSketch settings.  This is found within
    /// the user's home directory in a hidden folder.
    ///
    /// \returns The absolute settings path.
    static Poco::Path getSettingsPath();

private:
    /// \brief The absolute projects path.
    Poco::Path _projectsPath;

};


} } // namespace of::Sketch
