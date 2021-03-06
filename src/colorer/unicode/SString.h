#ifndef _COLORER_SSTRING_H_
#define _COLORER_SSTRING_H_

#include <colorer/unicode/String.h>
#include <memory>
class DString;

/**
 * Unicode string.
 * @ingroup unicode
*/
class SString : public String
{
public:
  /**
  * Empty string constructor
  */
  SString();
  /**
   * String constructor from String source
   * @param cstring source string, can't be null.
   */
  SString(const String* cstring, size_t s = 0, size_t l = npos);

  /**
   * String constructor from String source
   * @param cstring source string, can't be null.
   */
  SString(const String &cstring, size_t s = 0, size_t l = npos);
  SString(const SString &cstring);

  /**
   * String constructor from char stream
   * @param str source string, can't be null.
   */
  SString(const char* string, size_t s = 0, size_t l = npos);
  SString(const wchar* string, size_t s = 0, size_t l = npos);
  SString(char* str, int enc = -1);
  SString(wchar* str);

  /**
   * String constructor from integer number
   */
  SString(int no);
  SString(size_t no);

  ~SString();

  /** Changes the length of this StringBuffer */
  void setLength(size_t newLength);

  wchar operator[](size_t i) const override;
  size_t length() const override;

  /** Appends to this string buffer @c string */
  SString &append(const String &string);
  /** Appends to this string buffer @c string */
  SString &append(const String* string);

  /** Appends to this string buffer @c string */
  SString &append(wchar c);

  /** Appends to this string buffer @c string.
      C++ operator+ form.
      You can write: yourcall(StringBuffer("first")+"second"+third);
  */
  SString &operator+(const String &string);
  /** Appends to this string buffer @c string. C++ operator+ form. */
  SString &operator+(const String* string);
  /** Appends to this string buffer @c string. C++ operator+ form. */
  SString &operator+(const char* string);
  /** Appends to this string buffer @c string. C++ operator+= form. */
  SString &operator+=(const String &string);
  /** Appends to this string buffer @c string. C++ operator+= form. */
  SString &operator+=(const char* string);

  SString &operator=(SString const &cstring);
  SString* replace(const String &pattern, const String &newstring) const;
  int compareTo(const SString& str) const;
  int compareTo(const DString& str) const;
  SString(SString &&cstring);
  SString &operator=(SString &&cstring);
protected:

  void construct(const String* cstring, size_t s, size_t l);

  std::unique_ptr<wchar[]> wstr;
  size_t len;
  size_t alloc;
};

inline SString::SString(SString &&cstring): wstr(std::move(cstring.wstr)),
  len(cstring.len),
  alloc(cstring.alloc)
{
  cstring.wstr = nullptr;
  cstring.alloc = 0;
  cstring.len = 0;
}

inline SString &SString::operator=(SString &&cstring)
{
  if (this != &cstring) {
    wstr = std::move(cstring.wstr);
    alloc = cstring.alloc;
    len = cstring.len;
    cstring.wstr = nullptr;
    cstring.alloc = 0;
    cstring.len = 0;
  }
  return *this;
}

inline size_t SString::length() const
{
  return len;
}

inline wchar SString::operator[](size_t i) const
{
  return wstr[i];
}

#include <unordered_map>
namespace std
{
  // Specializations for unordered containers

  template <> struct hash<SString> {
    size_t operator()(const SString &value) const
    {
      return value.hashCode();
    }
  };
  template <> struct equal_to<SString> {
    bool operator()(const SString &u1, const SString &u2) const
    {
      return u1.compareTo(u2) == 0;
    }
  };

} // namespace std

#endif

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

