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


#include "SourceFile.h"
#include "Project.h"


namespace of {
namespace Sketch {



	const std::map<SourceFile::Type, std::vector<std::string>> SourceFile::SOURCE_FILE_TYPE_MAP =
													 {
														 { Type::SKETCH, { "sketch" } },
														 { Type::HEADER, { "h", "hpp" } },
														 { Type::SOURCE, { "cpp", "c", "m", "mm" } }
													 };


SourceFile::SourceFile(const Project& project, const Poco::Path& path):
	_project(project),
	_file(path),
	_type(typeForFileExtension(path.getExtension())),
	_isLoaded(false)
{
}


SourceFile::~SourceFile()
{
}


const Project& SourceFile::project() const
{
	return _project;
}

const Poco::File& SourceFile::file() const
{
	return _file;
}

void SourceFile::setContents(const ofBuffer& contents) const
{
	_contents = contents;
	_isLoaded = true;
}

const ofBuffer& SourceFile::getContents() const
{
	if (!_isLoaded)
	{
		_contents = ofBufferFromFile(_file.path());
	}

	return _contents;
}

bool SourceFile::save() const
{
	return ofBufferToFile(_file.path(), _contents);
}

void SourceFile::setType(Type type)
{
	_type = type;
}

SourceFile::Type SourceFile::getType()
{
	return _type;
}


SourceFile::Type SourceFile::typeForFileExtension(const std::string& fileExtension)
{
	auto iter = SOURCE_FILE_TYPE_MAP.begin();

	while (iter != SOURCE_FILE_TYPE_MAP.end())
	{
		auto findIter = std::find_if(iter->second.begin(),
									 iter->second.end(),
									 [fileExtension](const std::string& validExtension) {
										 return 0 == Poco::UTF8::icompare(fileExtension, validExtension);
									 });

		if (findIter != iter->second.end())
		{
			return iter->first;;
		}

		++iter;
	}

	return Type::UNKNOWN;
}


} } // namespace of::Sketch
