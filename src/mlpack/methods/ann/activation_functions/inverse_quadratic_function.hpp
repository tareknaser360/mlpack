/**
 * @file methods/ann/activation_functions/inverse_quadratic_function.hpp
 * @author Himanshu Pathak
 *
 * Definition and implementation of Inverse Quadratic function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_ACTIVATION_FUNCTIONS_INVERSE_QUAD_FUNCTION_HPP
#define MLPACK_METHODS_ANN_ACTIVATION_FUNCTIONS_INVERSE_QUAD_FUNCTION_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {

/**
 * The Inverse Quadratic function, defined by
 *
 * @f{eqnarray*}{
 * f(x) = 1 / (1 + x^2) \\
 * f'(x) = -2 * x / (1 + x^2)^2 \\
 * @f}
 */
class InvQuadFunction
{
 public:
  /**
   * Computes the Inverse Quadratic function.
   *
   * @param x Input data.
   * @return f(x).
   */
  static double Fn(const double x)
  {
    return 1 / ( 1 + x * x);
  }

  /**
   * Computes the Inverse Quadratic function.
   *
   * @param x Input data.
   * @param y The resulting output activation.
   */
  template<typename InputVecType, typename OutputVecType>
  static void Fn(const InputVecType& x, OutputVecType& y)
  {
    y = 1 / (1 + arma::pow(x, 2));
  }

  /**
   * Computes the first derivative of the Inverse Quadratic function.
   *
   * @param y Input data.
   * @return f'(x)
   */
  static double Deriv(const double y)
  {
    return  - 2 * y / std::pow(1 + std::pow(y, 2), 2);
  }

  /**
   * Computes the first derivatives of the Inverse Quadratic function.
   *
   * @param y Input data.
   * @param x The resulting derivatives.
   */
  template<typename InputVecType, typename OutputVecType>
  static void Deriv(const InputVecType& x, OutputVecType& y)
  {
    y = - 2 * x / arma::pow(1 + arma::pow(x, 2), 2);
  }
}; // class InvQuadFunction

} // namespace mlpack

#endif
