/*
  Copyright 2019 Alain Dargelas

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
 * File:   Cache.h
 * Author: alain
 *
 * Created on April 28, 2017, 9:32 PM
 */

#ifndef SURELOG_CACHE_H
#define SURELOG_CACHE_H
#pragma once

#include <Surelog/Cache/header_generated.h>
#include <Surelog/Common/PathId.h>
#include <flatbuffers/flatbuffers.h>

#include <string_view>
#include <vector>

namespace SURELOG {

class ErrorContainer;
class FileContent;
class SymbolTable;
class VObject;

// A cache class used as a base for various other cashes persisting
// things in flatbuffers.
// All methods are protected as they are ment for derived classes to use.
//
// The cache is storing a symbol table on disk, which we call "cacheSymbols";
// these typically only contain all the symbols that are exported.
// The "localSymbols" in the process will differ from "cacheSymbols" as it
// typically contains more.
class Cache {
 public:
  static constexpr uint64_t Capacity = 0x000000000FFFFFFF;

 protected:
  using VectorOffsetError =
      flatbuffers::Vector<flatbuffers::Offset<SURELOG::CACHE::Error>>;
  using VectorOffsetString =
      flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>;

  Cache() = default;

  std::string_view getExecutableTimeStamp() const;

  // Open file and read contents into a buffer.
  bool openFlatBuffers(PathId cacheFileId, std::vector<char>& content) const;

  bool saveFlatbuffers(const flatbuffers::FlatBufferBuilder& builder,
                       PathId cacheFileId, SymbolTable* symbolTable);

  bool checkIfCacheIsValid(const SURELOG::CACHE::Header* header,
                           std::string_view schemaVersion, PathId cacheFileId,
                           PathId sourceFileId) const;

  flatbuffers::Offset<SURELOG::CACHE::Header> createHeader(
      flatbuffers::FlatBufferBuilder& builder, std::string_view schemaVersion);

  // Store errors in cache. Canonicalize strings and store in "cacheSymbols".
  flatbuffers::Offset<VectorOffsetError> cacheErrors(
      flatbuffers::FlatBufferBuilder& builder, SymbolTable* cacheSymbols,
      const ErrorContainer* errorContainer, const SymbolTable& localSymbols,
      PathId subjectId);

  void restoreSymbols(const VectorOffsetString* symbolBuf,
                      SymbolTable* cacheSymbols);

  // Restores errors and the cache symbol table (to be used in other restore
  // operations).
  // References in the error container to local symbols will be entered into
  // the local symbol table.
  void restoreErrors(const VectorOffsetError* errorsBuf,
                     SymbolTable* cacheSymbols, ErrorContainer* errorContainer,
                     SymbolTable* localSymbols);

  // Convert vobjects from "fcontent" into cachable VObjects.
  // Uses "localSymbols" and "cacheSymbols" to map symbols found in "fcontent"
  // to IDs used on the cache on disk.
  // Updates "cacheSymbols" if new IDs are needed.
  std::vector<CACHE::VObject> cacheVObjects(const FileContent* fcontent,
                                            SymbolTable* cacheSymbols,
                                            const SymbolTable& localSymbols,
                                            PathId fileId);

  // Restore objects coming from the flatbuffer cache and with the corresponding
  // "cacheSymbols" into "fileContent", with IDs relevant in the local
  // symbol table "localSymbols" (which is updated).
  void restoreVObjects(
      const flatbuffers::Vector<const SURELOG::CACHE::VObject*>* objects,
      const SymbolTable& cacheSymbols, SymbolTable* localSymbols, PathId fileId,
      FileContent* fileContent);

  void restoreVObjects(
      const flatbuffers::Vector<const SURELOG::CACHE::VObject*>* objects,
      const SymbolTable& cacheSymbols, SymbolTable* localSymbols, PathId fileId,
      std::vector<SURELOG::VObject>* result);

 private:
  Cache(const Cache& orig) = delete;
};

}  // namespace SURELOG

#endif /* SURELOG_CACHE_H */
