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


#include "ofTypes.h"


namespace of {
namespace Sketch {


/// \brief A class representing and openFrameworks addon.
class Addon
{
public:
    /// \brief Create an addon with the given path.
    /// \param path The path to the addon.
	Addon(const std::string& path);

    /// \brief Create an addon with the given parameters.
    /// \param path The path to the addon.
    /// \param name The addon name.
    /// \param description The addon description.
    /// \param author The addon author.
    /// \param tags The addon tags.
    /// \param URL the addon URL.
    Addon(const std::string& path,
          const std::string& name,
          const std::string& description,
          const std::string& author,
          const std::vector<std::string> tags,
          const std::string& URL);

    virtual ~Addon();

    /// \returns the absolute path to the addon.
    const std::string& path() const;

    /// \returns the name of the addon.
    const std::string& name() const;

    /// \returns the description of the addon.
    const std::string& desription() const;

    /// \returns the author of the addon.
    const std::string& author() const;

    /// \returns the tags of the addon.
    const std::vector<std::string> tags() const;

    /// \returns the URL of the addon.
    const std::string& url() const;

//    /// \returns true iff this is a core addon.
//    bool isCoreAddon() const;

private:
	/// \brief The absolute path to the Addon.
    std::string _path;

	/// \brief The name of the Addon.
    std::string _name;

	/// \brief An Addon description.
	std::string _description;

	/// \brief The Addon author.
    std::string _author;

	/// \brief The Addon tags.
    std::vector<std::string> _tags;

	/// \brief The Addon URL.
    std::string _url;

};


} } // namespace of::Sketch
