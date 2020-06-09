// Copyright 2019 Zodiac Interactive LLC. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Author: Dmytro Iakovliev <dmytro.iakovliev@dev.zodiac.tv>

#include <string>
#include <map>
#include <algorithm>
#include <list>

#include "parse_tree.h"
#include "err.h"
#include "functions.h"
#include "parse_node_value_adapter.h"
#include "scope.h"
#include "value.h"
#include "item.h"
#include "settings.h"
#include "user_function.h"

namespace functions {

// function --------------------------------------------------------------------
const char kUserFunction[] = "function";
const char kUserFunction_HelpShort[] =
    "function: Define a user function.";
const char kUserFunction_Help[] =
    R"(function: Define a template rule.

  Zodiac extension!

  A function defines a custom name what acts as classic function.

  The function() function is used to declare a user function. To invoke
  function, just use the name of declared function.

  A functions and templates have the same namespace, so if you will define
  the function with the name of existing template gn will fail.

Example of defining a user function

  # The function() expects identifiers as a parameters! First identifier is
  # the user function name, rest - variables what will be accessible in
  # function scope.
  function(my_function, param_name_1, param_name_2, ... param_name_N) {
    print("param_name_1: " + param_name_1)
    print("param_name_2: " + param_name_2)
    ...
    print("param_name_N: " + param_name_N)

    # To return the value just assign value into 'return' variable
    return = 999
  }

  # Use the user function in any place where visible.
  print("my_function result: " + my_function(value_1, value_2, ... value_N))

)";

Value RunUserFunction(Scope *scope,
                      const FunctionCallNode* function,
                      const ListNode* args_list,
                      Err* err) {
  NonNestableBlock non_nestable(scope, function, "function");
  if (!non_nestable.Enter(err))
    return Value();

  const auto& args_vector = args_list->contents();
  if (args_vector.empty()) {
    *err = Err(function, "Wrong definition.",
               "At least function identifier expected.");
    return Value();
  }

  const IdentifierNode* function_identifier = args_vector[0]->AsIdentifier();
  if (!function_identifier) {
    *err = Err(function, "Wrong definition.",
               "Identifiers instead values expected.");
    return Value();
  }
  std::string function_name = std::string(function_identifier->value().value());

  std::list<std::string> parameters;
  for (size_t i = 1; i < args_vector.size(); ++i) {
    if (args_vector.size() > 1) {
      const IdentifierNode* param_identifier = args_vector[i]->AsIdentifier();
      if (!param_identifier) {
        *err = Err(function, "Wrong definition.",
                   "Identifiers instead values expected.");
        return Value();
      }

      parameters.push_back(std::string(param_identifier->value().value()));
    }
  }

  const Template* existing_template = scope->GetTemplate(function_name);
  if (existing_template) {
    *err = Err(function, "Duplicate template/user_function definition.",
               "A template with this name was already defined.");
    err->AppendSubErr(Err(existing_template->GetDefinitionRange(),
                          "Previous definition."));
    return Value();
  }

  scope->AddTemplate(function_name, new UserFunction(scope, function, parameters));
  scope->MarkAllUsed();

  return Value();
}

} // namespace functions
