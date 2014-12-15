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


#include "Toolchain.h"


namespace of {
namespace Sketch {


Toolchain::Toolchain()
{
}


Toolchain::Toolchain(const std::string& name,
                     const std::vector<std::string>& targetPlatforms):
    _name(name),
    _targetPlatforms(targetPlatforms)
{
}

    
Toolchain::~Toolchain()
{
}

//#ifdef TARGET_WIN32
//    // Set up toolchain path information for Windows.
//    std::string pathVar = Poco::Environment::get("PATH","");
//
//    std::string pathToolChain0 = ofToDataPath("Toolchains/ofMinGW/MinGW/msys/1.0/bin", true);
//    std::string pathToolChain1 = ofToDataPath("Toolchains/ofMinGW/MinGW/bin", true);
//
//    std::stringstream pathSS;
//    pathSS << pathToolChain0 << ";" << pathToolChain1 << ";" << pathVar;
//    Poco::Environment::set("PATH", pathSS.str());
//#endif


const std::string Toolchain::getName() const
{
    return _name;
}


const std::vector<std::string>& Toolchain::getTargetPlatforms() const
{
    return _targetPlatforms;
}


} } // namespace of::Sketch
