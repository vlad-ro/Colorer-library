#ifndef _COLORER_PARSERFACTORY_H_
#define _COLORER_PARSERFACTORY_H_

#include <colorer/TextParser.h>
#include <colorer/HRCParser.h>
#include <colorer/parsers/HRDNode.h>
#include <colorer/handlers/StyledHRDMapper.h>
#include <colorer/handlers/TextHRDMapper.h>

/**
 * Maintains catalog of HRC and HRD references.
 * This class searches and loads <code>catalog.xml</code> file
 * and creates HRCParser, StyledHRDMapper, TextHRDMapper and TextParser instances
 * with information, loaded from specified sources.
 *
 * If no path were passed to it's constructor,
 * it uses the next search order to find 'catalog.xml' file:
 *
 * - win32 systems:
 *   - image_start_dir, image_start_dir\..\
 *   - %COLORER5CATALOG%
 *   - %HOMEDRIVE%%HOMEPATH%\.colorer5catalog
 *
 * - unix/macos systems:
 *   - $COLORER5CATALOG
 *   - $HOME/.colorer5catalog
 *   - $HOMEPATH/.colorer5catalog
 *   - /usr/share/colorer/catalog.xml
 *   - /usr/local/share/colorer/catalog.xml
 *
 * @note
 *   - \%NAME%, \$NAME - Environment variable of the current process.
 *   - image_start_dir - Directory, where current image was started.
 *
 * @ingroup colorer
 */
class ParserFactory
{
public:

  /**
   * ParserFactory Constructor.
   * Searches for catalog.xml in the set of predefined locations
   * @throw ParserFactoryException If can't find catalog at any of standard locations.
   */
  ParserFactory();

  virtual ~ParserFactory();

  static const char* getVersion();

  /**
   * Enumerates all declared hrd classes
   */
  std::vector<SString> enumHRDClasses();

  /**
   * Enumerates all declared hrd instances of specified class
   */
  std::vector<const HRDNode*> enumHRDInstances(const String &classID);

  const HRDNode* getHRDNode(const String &classID, const String &nameID);
  /**
   * Creates and loads HRCParser instance from catalog.xml file.
   * This method can detect directory entries, and sequentally load their
   * contents into created HRCParser instance.
   * In other cases it uses InputSource#newInstance() method to
   * create input data stream.
   * Only one HRCParser instance is created for each ParserFactory instance.
   */
  HRCParser*  getHRCParser() const;

  /**
   * Creates TextParser instance
   */
  TextParser* createTextParser();

  /**
   * Creates RegionMapper instance and loads specified hrd files into it.
   * @param classID Class identifier of loaded hrd instance.
   * @param nameID  Name identifier of loaded hrd instances.
   * @throw ParserFactoryException If method can't find specified pair of
   *         class and name IDs in catalog.xml file
   */
  StyledHRDMapper* createStyledMapper(const String* classID, const String* nameID);
  /**
   * Creates RegionMapper instance and loads specified hrd files into it.
   * It uses 'text' class by default.
   * @param nameID  Name identifier of loaded hrd instances.
   * @throw ParserFactoryException If method can't find specified pair of
   *         class and name IDs in catalog.xml file
   */
  TextHRDMapper* createTextMapper(const String* nameID);

  size_t countHRD(const String &classID);

  /**
  * @param catalog_path Path to catalog.xml file. If null,
  *        standard search method is used.
  * @throw ParserFactoryException If can't load specified catalog.
  */
  void loadCatalog(const String* catalog_path);
private:

  SString searchCatalog() const;
  void getPossibleCatalogPaths(std::vector<SString> &paths) const;

  void parseCatalog(const SString &catalog_path);

  void loadHrc(const String* hrc_path, const String* base_path) const;

  SString base_catalog_path;
  std::vector<SString> hrc_locations;
  std::unordered_map<SString, std::unique_ptr<std::vector<std::unique_ptr<HRDNode>>>> hrd_nodes;

  HRCParser* hrc_parser;

  ParserFactory(const ParserFactory &) = delete;
  void operator=(const ParserFactory &) = delete;
};

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

