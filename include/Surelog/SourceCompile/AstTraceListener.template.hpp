// This file is auto-generated by generate_ast_listener.py
// DO NOT EDIT

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
 *
 * File:   AstTraceListener.h
 * Author: hs
 *
 */

#ifndef SURELOG_ASTTRACELISTENER_H
#define SURELOG_ASTTRACELISTENER_H
#pragma once

#include <Surelog/SourceCompile/AstListener.h>

#include <filesystem>
#include <ostream>

// clang-format off
#define TRACE_INIT_CONTEXT                \
  int32_t sl = 0, sc = 0, el = 0, ec = 0; \
  std::string name;                       \
  getNodeLocation(node, sl, sc, el, ec);  \
  getNodeName(node, name)

#define TRACE_PRINT_CONTEXT \
  "[" << sl << "," << sc <<  ":" << el << "," << ec <<  "], " << name

#define TRACE_ENTER TRACE_INIT_CONTEXT;       \
  m_strm << std::string(m_indent++ * 2, ' ')  \
  << __func__ << ": " << TRACE_PRINT_CONTEXT  \
  << std::endl
#define TRACE_LEAVE TRACE_INIT_CONTEXT;       \
  m_strm << std::string(2 * --m_indent, ' ')  \
  << __func__ << ": " << TRACE_PRINT_CONTEXT  \
  << std::endl
// clang-format on

namespace SURELOG {
class AstTraceListener final : public AstListener {
 public:
  AstTraceListener(std::ostream& strm) : m_strm(strm), m_indent(0) {}
  ~AstTraceListener() final = default;

  void enterSourceFile(SURELOG::PathId fileId) final {
    m_strm << std::string(m_indent++ * 2, ' ') << __func__ << ": "
           << SURELOG::PathIdPP(fileId) << std::endl;
  }
  void leaveSourceFile(SURELOG::PathId fileId) final {
    m_strm << std::string(2 * --m_indent, ' ') << __func__ << ": "
           << SURELOG::PathIdPP(fileId) << std::endl;
  }

  // clang-format off
<PUBLIC_ENTER_LEAVE_DECLARATIONS>
  // clang-format on

 private:
   std::ostream& m_strm;
   size_t m_indent = 0;
};
}  // namespace SURELOG

#endif  // SURELOG_ASTTRACELISTENER_H
