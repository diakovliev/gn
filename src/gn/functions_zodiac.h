// Copyright 2019 Zodiac Interactive LLC. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Author: Dmytro Iakovliev <dmytro.iakovliev@dev.zodiac.tv>

#pragma once

#include <vector>

class Scope;
class FunctionCallNode;
class Value;
class ListNode;
class Err;

namespace functions {

extern const char kUserFunction[];
extern const char kUserFunction_HelpShort[];
extern const char kUserFunction_Help[];
Value RunUserFunction(Scope *scope,
                   const FunctionCallNode* function,
                   const ListNode* args_list,
                   Err* err);

} // namespace functions
