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
#include "ofTypes.h"


namespace of {
namespace Sketch {


/// \brief A class representing and openFrameworks addon.
class Addon
{
public:
    /// \brief Create an addon with the given path.
    /// \param path The path to the addon.
    Addon(const Poco::Path& path);

    /// \brief Create an addon with the given parameters.
    /// \param path The path to the addon.
    /// \param name The addon name.
    /// \param description The addon description.
    /// \param author The addon author.
    /// \param tags The addon tags.
    /// \param URL the addon URL.
    Addon(const Poco::Path& path,
          const std::string& name,
          const std::string& description,
          const std::string& author,
          const std::vector<std::string> tags,
          const std::string& URL);

    virtual ~Addon();

    /// \returns the absolute path to the addon.
    const Poco::Path& getPath() const;

    /// \returns the name of the addon.
    const std::string& getName() const;

    /// \returns the description of the addon.
    const std::string& getDesription() const;

    /// \returns the author of the addon.
    const std::string& getAuthor() const;

    /// \returns the tags of the addon.
    const std::vector<std::string> getTags() const;

    /// \returns the URL of the addon.
    const std::string& getURL() const;

//    /// \returns true iff this is a core addon.
//    bool isCoreAddon() const;

private:
    Poco::Path _path; ///< The absolute path to the addon.

    std::string _name; ///< The name of the addon.

    std::string _description; ///< An addon description.

    std::string _author; ///< The addon author.

    std::vector<std::string> _tags; ///< The addon tags.

    std::string _URL; ///< The addon URL.


};


} } // namespace of::Sketch
