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


#include "SSLSettings.h"
#include "ofSSLManager.h"


namespace of {
namespace Sketch {


const Poco::Net::Context::VerificationMode SSLSettings::DEFAULT_VERIFICATION_MODE = Poco::Net::Context::VERIFY_RELAXED;
const bool SSLSettings::DEFAULT_LOAD_CA = false;
const std::string SSLSettings::DEFAULT_CIPHER_LIST = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";


SSLSettings::SSLSettings():
    _privateKeyPath(ofSSLManager::DEFAULT_PRIVATE_KEY_FILE),
    _certificatePath(ofSSLManager::DEFAULT_CERTIFICATE_FILE),
    _CACertPath(ofSSLManager::DEFAULT_CA_LOCATION),
    _verificationMode(DEFAULT_VERIFICATION_MODE),
    _verificationDepth(DEFAULT_VERIFICATION_DEPTH),
    _loadDefaultCAs(DEFAULT_LOAD_CA),
    _cipherList(DEFAULT_CIPHER_LIST)
{
}


SSLSettings::SSLSettings(const std::string& sslPrivateKeyPath,
                         const std::string& sslCertificatePath,
                         const std::string& sslCACertPath,
                         Poco::Net::Context::VerificationMode verificationMode,
                         int sslVerificationDepth,
                         bool loadDefaultCAs,
                         const std::string sslCipherList):
    _privateKeyPath(sslPrivateKeyPath),
    _certificatePath(sslCertificatePath),
    _CACertPath(sslCACertPath),
    _verificationMode(verificationMode),
    _verificationDepth(sslVerificationDepth),
    _loadDefaultCAs(loadDefaultCAs),
    _cipherList(sslCipherList)
{
}


std::string SSLSettings::getPrivateKeyPath() const
{
    return _privateKeyPath;
}


std::string SSLSettings::getCertificatePath() const
{
    return _certificatePath;
}


std::string SSLSettings::getCACertPath() const
{
    return _CACertPath;
}


Poco::Net::Context::VerificationMode SSLSettings::getVerificationMode() const
{
    return _verificationMode;
}


int SSLSettings::getVerificationDepth() const
{
    return _verificationDepth;
}


bool SSLSettings::getLoadDefaultCAs() const
{
    return _loadDefaultCAs;
}


std::string SSLSettings::getCipherList() const
{
    return _cipherList;
}


} } // namespace of::Sketch
