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


#include "Addon.h"


namespace of {
namespace Sketch {


Addon::Addon(const Poco::Path& path): _path(path)
{
}


Addon::Addon(const Poco::Path& path,
             const std::string& name,
             const std::string& description,
             const std::string& author,
             const std::vector<std::string> tags,
             const std::string& URL):
    _path(path),
    _name(name),
    _description(description),
    _author(author),
    _tags(tags),
    _URL(URL)
{
}


Addon::~Addon()
{
}


const Poco::Path& Addon::getPath() const
{
    return _path;
}


const std::string& Addon::getName() const
{
    return _name;
}


const std::string& Addon::getDesription() const
{
    return _description;
}


const std::string& Addon::getAuthor() const
{
    return _author;
}


const std::vector<std::string> Addon::getTags() const
{
    return _tags;
}


const std::string& Addon::getURL() const
{
    return _URL;
}


//bool Addon::isCoreAddon() const
//{
//    returns false;
//}


} } // namespace of::Sketch
