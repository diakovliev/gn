// Copyright 2019 Zodiac Interactive LLC. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Author: Dmytro Iakovliev <dmytro.iakovliev@dev.zodiac.tv>

#pragma once

#include "template.h"

#include <memory>
#include <string>
#include <vector>
#include <list>

class BlockNode;
class Err;
class FunctionCallNode;
class LocationRange;
class Scope;
class Value;

class UserFunction : public Template {
 public:
  // Makes a new closure based on the given scope.
  UserFunction(const Scope* scope, const FunctionCallNode* def, const std::list<std::string>& parameters);

  // Takes ownership of a previously-constructed closure.
  UserFunction(std::unique_ptr<Scope> closure, const FunctionCallNode* def, const std::list<std::string>& parameters);

  // Invoke the user defined function. The values correspond to the state of the code
  // invoking the user function. The user function name needs to be supplied since the
  // UserFunction object itself doesn't know what name the calling code is using
  // to refer to it (this is used to set defaults).
  virtual Value Invoke(Scope* scope,
               const FunctionCallNode* invocation,
               const std::string& template_name,
               const std::vector<Value>& args,
               BlockNode* block,
               Err* err) const;

  virtual ~UserFunction();

 private:
  friend class base::RefCountedThreadSafe<UserFunction>;

  UserFunction();

  std::vector<std::string> parameters_;
};
