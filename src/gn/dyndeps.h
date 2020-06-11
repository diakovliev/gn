// Copyright 2020 The dmytro.iakovliev(daemondzk@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "gn/build_settings.h"
#include "gn/scope.h"
#include "gn/source_dir.h"
#include "gn/label_ptr.h"
#include "gn/deps_iterator.h"

extern const char kDyndeps_Help[];

class Dyndeps {
 public:
  Dyndeps() = default;

  const ParseNode* origin() const { return origin_; }
  void set_origin(const ParseNode* origin) { origin_ = origin; }

  // The relative source directory to use when rebasing.
  const SourceDir& source_dir() const { return source_dir_; }
  SourceDir& source_dir() { return source_dir_; }
  void set_source_dir(const SourceDir& d) { source_dir_ = d; }

  const std::string& script() const { return script_; }
  void set_script(const std::string& new_script) { script_ = new_script; }

  const std::vector<std::string>& args() const { return args_; }
  std::vector<std::string>& args() { return args_; }

  // Linked public dependencies.
  const LabelTargetVector& deps() const { return deps_; }
  LabelTargetVector& deps() { return deps_; }

  bool empty() const { return script_.empty(); }

  DepsIteratorRange GetDeps() const;

 private:
  const ParseNode* origin_ = nullptr;
  SourceDir source_dir_;

  std::string script_;
  std::vector<std::string> args_;
  LabelTargetVector deps_;

  DISALLOW_COPY_AND_ASSIGN(Dyndeps);
};
