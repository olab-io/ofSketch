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


#include <string>
#include "ofFileUtils.h"


namespace of {
namespace Sketch {


class Project;


/// \brief Represents a SourceFile.
class SourceFile
{
public:
	enum class Type
	{
		/// \brief A .sketch ofSketch file.
		SKETCH,
		/// \brief A typical *.h file.
		HEADER,
		/// \brief A typical implementation file such as *.cpp.
		SOURCE,
		/// \brief An unknown SourceFile type.
		UNKNOWN
	};

	/// \brief Create a SourceFile with the given parameters.
	/// \param project The parent Project.
	/// param file The SourceFile's file.
	SourceFile(const Project& project, const std::string& path);
	
	/// \brief Destroy the SourceFile.
	virtual ~SourceFile();

	/// \brief Get a reference to the SourceFile's parent Project.
	/// \returns the SourceFile's parent Project.
	const Project& project() const;

	/// \brief Get the SourceFile's file.
	/// \returns the SourceFile's file.
	const ofFile& file() const;

	/// \brief Set the contents of the SourceFile.
	/// \param contents The contents of the SourceFile to set.
	void setContents(const ofBuffer& contents) const;

	/// \brief Get the contents of the SourceFile.
	/// \returns the contents of the SourceFile.
	const ofBuffer& getContents() const;

	/// \brief Save the SourceFile using the current contents of the buffer.
	/// \returns true if the buffer was saved successfully.
	bool save() const;

	/// \brief Set the SourceFile Type.
	/// \param type The SourceFile Type to set.
	void setType(Type type);

	/// \brief Get the SourceFile Type.
	/// \returns the SourceFile Type.
	Type getType() const;

	/// \brief Get the SourceFile Type for a given file extension.
	/// \param fileExtension The file extension to test.
	/// \returns the SourceFile Type or UNKNOWN if it cannot be determined.
	static Type typeForFileExtension(const std::string& fileExtension);

	/// \brief A map of the SourceFile::Type and their associated file extensions.
	static const std::map<SourceFile::Type, std::vector<std::string>> SOURCE_FILE_TYPE_MAP;

private:
	/// \brief A reference to the parent Project;
	const Project& _project;

	/// \brief The SourceFile's ofFile object.
	ofFile _file;

	/// \brief The SourceFile's Type.
	Type _type;

	/// \brief The contents of the SourceFile.
	mutable ofBuffer _contents;

	/// \brief True if the contents of the sourcefile been loaded
	mutable bool _isLoaded;

};


} } // namespace of::Sketch
