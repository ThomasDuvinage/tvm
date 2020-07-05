﻿/*
 * Copyright 2017-2020 CNRS-AIST JRL and CNRS-UM LIRMM
 */
#pragma once

#include <tvm/internal/math.h>

namespace tvm::manifold::internal
{

  /** sinus cardinal: sin(x)/x
 * Code adapted from boost::math::detail::sinc
 */
  template<typename T>
  T sinc(const T x)
  {
    constexpr T taylor_0_bound = std::numeric_limits<double>::epsilon();
    constexpr T taylor_2_bound = tvm::internal::sqrt(taylor_0_bound);
    constexpr T taylor_n_bound = tvm::internal::sqrt(taylor_2_bound);

    if (std::abs(x) >= taylor_n_bound)
    {
      return (std::sin(x) / x);
    }
    else
    {
      // approximation by taylor series in x at 0 up to order 0
      T result = static_cast<T>(1);

      if (std::abs(x) >= taylor_0_bound)
      {
        T x2 = x * x;

        // approximation by taylor series in x at 0 up to order 2
        result -= x2 / static_cast<T>(6);

        if (std::abs(x) >= taylor_2_bound)
        {
          // approximation by taylor series in x at 0 up to order 4
          result += (x2 * x2) / static_cast<T>(120);
        }
      }

      return (result);
    }
  }

  /**
   * Compute 1/sinc(x).
   * This code is inspired by boost/math/special_functions/sinc.hpp.
   */
  template<typename T>
  T sinc_inv(const T x)
  {
    constexpr T taylor_0_bound = std::numeric_limits<T>::epsilon();
    constexpr T taylor_2_bound = tvm::internal::sqrt(taylor_0_bound);
    constexpr T taylor_n_bound = tvm::internal::sqrt(taylor_2_bound);

    // We use the 4th order taylor series around 0 of x/sin(x) to compute
    // this function:
    //      2      4
    //     x    7⋅x     ⎛ 6⎞
    // 1 + ── + ──── + O⎝x ⎠
    //     6    360
    // this approximation is valid around 0.
    // if x is far from 0, our approximation is not valid
    // since x^6 becomes non negligable we use the normal computation of the function
    // (i.e. taylor_2_bound^6 + taylor_0_bound == taylor_0_bound but
    //       taylor_n_bound^6 + taylor_0_bound != taylor_0).

    if (std::abs(x) >= taylor_n_bound)
    {
      return (x / std::sin(x));
    }
    else
    {
      // x is bellow taylor_n_bound so we don't care of the 6th order term of
      // the taylor series.
      // We set the 0 order term.
      T result = static_cast<T>(1);

      if (std::abs(x) >= taylor_0_bound)
      {
        // x is above the machine epsilon so x^2 is meaningful.
        T x2 = x * x;
        result += x2 / static_cast<T>(6);

        if (std::abs(x) >= taylor_2_bound)
        {
          // x is upper the machine sqrt(epsilon) so x^4 is meaningful.
          result += static_cast<T>(7) * (x2 * x2) / static_cast<T>(360);
        }
      }

      return (result);
    }
  }
}