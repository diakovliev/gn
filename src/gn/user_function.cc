// Copyright 2019 Zodiac Interactive LLC. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Author: Dmytro Iakovliev <dmytro.iakovliev@dev.zodiac.tv>

#include "user_function.h"

#include <utility>
#include <algorithm>

#include "err.h"
#include "functions.h"
#include "settings.h"
#include "parse_tree.h"
#include "scope.h"
#include "scope_per_file_provider.h"
#include "value.h"
#include "variables.h"
#include "build_settings.h"

UserFunction::UserFunction(const Scope* scope, const FunctionCallNode* def, const std::list<std::string>& parameters)
    : Template(scope, def)
    , parameters_(parameters.begin(), parameters.end()) {
}

UserFunction::UserFunction(std::unique_ptr<Scope> scope, const FunctionCallNode* def, const std::list<std::string>& parameters)
    : Template(std::move(scope), def)
    , parameters_(parameters.begin(), parameters.end()) {
}

UserFunction::~UserFunction() {
}

Value UserFunction::Invoke(Scope* scope,
                       const FunctionCallNode* invocation,
                       const std::string& template_name,
                       const std::vector<Value>& args,
                       BlockNode* block,
                       Err* err) const {
  Scope::ItemVector collector;

  auto call_scope = scope->MakeClosure();

  call_scope->set_item_collector(&collector);
  call_scope->set_source_dir(scope->GetSourceDir());

  size_t end_index = std::min(args.size(), parameters_.size());

  for(size_t i = 0; i < end_index; ++i) {
    call_scope->SetValue(parameters_[i], args[i], invocation);
  }

  definition()->block()->Execute(call_scope.get(), err);

  if (err->has_error())
    return Value();

  Value *res = call_scope->GetMutableValue("return", Scope::SEARCH_CURRENT, false);

  for (auto& item : collector)
    call_scope->settings()->build_settings()->ItemDefined(std::move(item));

  if (res)
      return Value(*res);
  else
      return Value();
}

