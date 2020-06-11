// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gn/group_target_generator.h"

#include "gn/target.h"
#include "gn/variables.h"

GroupTargetGenerator::GroupTargetGenerator(
    Target* target,
    Scope* scope,
    const FunctionCallNode* function_call,
    Err* err)
    : TargetGenerator(target, scope, function_call, err) {}

GroupTargetGenerator::~GroupTargetGenerator() = default;

void GroupTargetGenerator::DoRun() {
  if (!FillDyndeps())
    return;

  target_->set_output_type(Target::GROUP);
  // Groups only have the default types filled in by the target generator
  // base class.
}
