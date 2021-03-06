#include <colorer/io/HTTPInputSource.h>

#if COLORER_FEATURE_HTTPINPUTSOURCE
#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#endif
#endif

#ifndef DWORD_PTR
#define DWORD_PTR DWORD
#endif

HTTPInputSource::HTTPInputSource(const String* basePath, HTTPInputSource* base)
{
  if (isRelative(basePath) && base != nullptr) {
    baseLocation = getAbsolutePath(base->getLocation(), basePath);
  } else {
    baseLocation = new SString(basePath);
  }
  stream = nullptr;
}

HTTPInputSource::~HTTPInputSource()
{
  delete baseLocation;
  delete[] stream;
}

colorer::InputSource* HTTPInputSource::createRelative(const String* relPath)
{
  return new HTTPInputSource(relPath, this);
}

const String* HTTPInputSource::getLocation() const
{
  return baseLocation;
}

const byte* HTTPInputSource::openStream()
{
  if (stream != nullptr) {
    throw InputSourceException(SString("openStream(): source stream already opened: '") + baseLocation + "'");
  }

#if COLORER_FEATURE_HTTPINPUTSOURCE
#ifdef _WIN32
  const int blockSize = 0x1000;
  std::vector<byte*> streamVector;
  std::vector<int> streamSizeVector;

  HINTERNET ihandle = InternetOpen(L"Colorer-take5 library connector", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (ihandle == NULL) {
    throw InputSourceException(StringBuffer("Can't create internet connection"));
  }

  DWORD dw;
  HINTERNET iurl = InternetOpenUrl(ihandle, baseLocation->getTChars(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_UI, (DWORD_PTR)(size_t)&dw);
  if (iurl == NULL) {
    throw InputSourceException(StringBuffer("Can't access URL '") + baseLocation + "'");
  }

  len = 0;
  DWORD bread;
  BOOL ok = TRUE;
  while (true) {
    byte* block = new byte[blockSize];

    ok = InternetReadFile(iurl, block, blockSize, &bread);

    if (ok == FALSE || bread == 0) {
      delete[] block;
      break;
    }
    streamVector.push_back(block);
    streamSizeVector.push_back(bread);
    len += bread;
  }
  if (ok == FALSE) {
    throw InputSourceException(StringBuffer("Can't read http stream: ") + baseLocation);
  } else {
    stream = new byte[len];
    int csize = 0;
    for (int idx = 0; idx < streamVector.size(); idx++) {
      int thisSize = streamSizeVector.at(idx);
      memmove(stream + csize, streamVector.at(idx), thisSize);
      csize += thisSize;
    }
  }
  streamVector.clear();

  InternetCloseHandle(iurl);
  InternetCloseHandle(ihandle);
#endif
#endif
  return stream;
}

void HTTPInputSource::closeStream()
{
  if (stream == nullptr) {
    throw InputSourceException(SString("closeStream(): source stream is not yet opened"));
  }
  delete[] stream;
  stream = nullptr;
}

int HTTPInputSource::length() const
{
  if (stream == nullptr) {
    throw InputSourceException(CString("length(): stream is not yet opened"));
  }
  return len;
}

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library.
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <irusskih at gmail dot com>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2009
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): see file CONTRIBUTORS
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
