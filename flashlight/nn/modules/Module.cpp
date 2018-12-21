/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include "Module.h"

#include <cereal/cereal.hpp>

#include <flashlight/common/Exception.h>
#include <flashlight/nn/Init.h>

namespace fl {

Module::Module() = default;

Module::Module(const std::vector<Variable>& params)
    : params_(params.begin(), params.end()) {}

Variable Module::param(int position) const {
  AFML_ASSERT(
      position >= 0 && position < params_.size(),
      "[Module] Invalid position",
      AF_ERR_ARG);
  return params_[position];
}

void Module::setParams(const Variable& var, int position) {
  AFML_ASSERT(
      position >= 0 && position < params_.size(),
      "[Module] Invalid position",
      AF_ERR_ARG);
  params_[position] = var;
}

void Module::train() {
  train_ = true;
  for (auto& param : params_) {
    param.setCalcGrad(true);
  }
}

void Module::zeroGrad() {
  for (auto& param : params_) {
    param.zeroGrad();
  }
}

void Module::eval() {
  train_ = false;
  for (auto& param : params_) {
    param.setCalcGrad(false);
  }
}

std::vector<Variable> Module::params() const {
  return params_;
}

Variable Module::operator()(const Variable& input) {
  return this->forward(input);
}

} // namespace fl