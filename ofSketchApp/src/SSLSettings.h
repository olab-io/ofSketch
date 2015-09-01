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


#include "Poco/Net/Context.h"
#include "ofx/Net/IPAddressRange.h"


namespace of {
namespace Sketch {


class SSLSettings
{
public:
    SSLSettings();
    SSLSettings(const std::string& privateKeyPath,
                const std::string& certificatePath,
                const std::string& CACertPath,
                Poco::Net::Context::VerificationMode verificationMode,
                int verificationDepth,
                bool loadDefaultCAs,
                const std::string cipherList);

    std::string getPrivateKeyPath() const;
    std::string getCertificatePath() const;
    std::string getCACertPath() const;
    Poco::Net::Context::VerificationMode getVerificationMode() const;
    int getVerificationDepth() const;
    bool getLoadDefaultCAs() const;
    std::string getCipherList() const;

    static const Poco::Net::Context::VerificationMode DEFAULT_VERIFICATION_MODE;
    static const bool DEFAULT_LOAD_CA;
    static const std::string DEFAULT_CIPHER_LIST;

    enum
    {
        DEFAULT_VERIFICATION_DEPTH = 9
    };

private:
    std::string _privateKeyPath;
    std::string _certificatePath;
    std::string _CACertPath;
    Poco::Net::Context::VerificationMode _verificationMode;
    int _verificationDepth;
    bool _loadDefaultCAs;
    std::string _cipherList;

};


} } // namespace of::Sketch
