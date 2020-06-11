// Copyright 2018 The dmytro.iakovliev(daemondzk@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gn/dyndeps.h"

const char kDyndeps_Help[] =
    R"(Dynamic dependencies resolver setup

 TODO: Write reference.

)";

DepsIteratorRange Dyndeps::GetDeps() const {
  return DepsIteratorRange(
      DepsIterator(&deps_, nullptr, nullptr));
}
