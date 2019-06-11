/**
 * @file weight_norm.hpp
 * @author Toshal Agrawal
 *
 * Definition of the Weight Normalization layer class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_WEIGHTNORM_HPP
#define MLPACK_METHODS_ANN_LAYER_WEIGHTNORM_HPP

#include <mlpack/prereqs.hpp>
#include "layer_types.hpp"

#include "../visitor/delete_visitor.hpp"
#include "../visitor/delta_visitor.hpp"
#include "../visitor/output_parameter_visitor.hpp"
#include "../visitor/reset_visitor.hpp"
#include "../visitor/weight_size_visitor.hpp"
#include "../visitor/weight_set_visitor.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * Declaration of the Weight Normalization layer class. The layer reparametrize
 * the weight vectors in a neural network decoupling the length of those weight
 * vectors from their direction. This reparametrization does not introduce any
 * dependencies between the examples in a minibatch.
 *
 * This class will be a wrapper around existing layers. It will just modify the
 * calculation and updation of weights of the layer.
 *
 * For more information, refer to the following paper,
 *
 * @code
 * @inproceedings{Salimans2016WeightNorm,
 *   title = {Weight Normalization: A Simple Reparameterization to Accelerate
 *            Training of Deep Neural Networks},
 *   author = {Tim Salimans, Diederik P. Kingma},
 *   booktitle = {Neural Information Processing Systems 2016},
 *   year = {2016}
 * }
 * @endcode
 *
 * @tparam InputDataType Type of the input data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 * @tparam OutputDataType Type of the output data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 * @tparam CustomLayers Additional custom layers that can be added.
 */
template <
  typename InputDataType = arma::mat,
  typename OutputDataType = arma::mat,
  typename... CustomLayers
>
class WeightNorm
{
 public:
  //! Create the WeightNorm layer object.
  WeightNorm();

  //! Destructor to release allocated memory.
  ~WeightNorm();

  /**
   * Reset the layer parameters.
   */
  void Reset();

  /**
   * Forward pass of the Weight Normalization layer. Calculates the weights of
   * the wrapped layer from the parameter vector v and the scalar parameter g.
   * It then calulates the output of the wrapped layer from the calculated
   * weights.
   *
   * @param input Input data for the layer.
   * @param output Resulting output activations.
   */
  template<typename eT>
  void Forward(arma::Mat<eT>&& input, arma::Mat<eT>&& output);

  /**
   * Backward pass through the layer. This function will call the
   * Backward() function of the wrapped layer.
   *
   * @param input The input activations.
   * @param gy The backpropagated error.
   * @param g The calculated gradient.
   */
  template<typename eT>
  void Backward(const arma::Mat<eT>&& input,
                arma::Mat<eT>&& gy,
                arma::Mat<eT>&& g);

  /**
   * Calculate the gradient using the output delta and the input activations and
   * weights of the wrapped layer.
   *
   * @param input The input activations.
   * @param error The calculated error.
   * @param gradient The calculated gradient.
   */
  template<typename eT>
  void Gradient(arma::Mat<eT>&& input,
                arma::Mat<eT>&& error,
                arma::Mat<eT>&& gradient);

  //! Get the delta.
  OutputDataType const& Delta() const { return delta; }
  //! Modify the delta.
  OutputDataType& Delta() { return delta; }

  //! Get the gradient.
  OutputDataType const& Gradient() const { return gradient; }
  //! Modify the gradient.
  OutputDataType& Gradient() { return gradient; }

  /*//! Get the input parameter.
  InputDataType const& InputParameter() const { return inputParameter; }
  //! Modify the input parameter.
  InputDataType& InputParameter() { return inputParameter; }*/

  //! Return the model modules.
  std::vector<LayerTypes<CustomLayers...> >& Model()
  {
    if (model)
    {
      return network;
    }

    return empty;
  }

  //! Get the output parameter.
  OutputDataType const& OutputParameter() const { return outputParameter; }
  //! Modify the output parameter.
  OutputDataType& OutputParameter() { return outputParameter; }

  //! Get the parameters.
  OutputDataType const& Parameters() const { return weights; }
  //! Modify the parameters.
  OutputDataType& Parameters() { return weights; }

  /*
   * Add a new module to the model.
   *
   * @param args The layer parameter.
   */
  template <class LayerType, class... Args>
  void Add(Args... args);

  /*
   * Add a new module to the model.
   *
   * @param layer The Layer to be added to the model.
   */
  void Add(LayerTypes<CustomLayers...> layer);

  /**
   * Serialize the layer.
   */
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);

 private:
  //! Locally-stored delete visitor module object.
  DeleteVisitor deleteVisitor;

  //! Locally-stored delta object.
  OutputDataType delta;

  //! Locally-stored delta visitor module object.
  DeltaVisitor deltaVisitor;

  //! Locally-stored empty list of modules.
  std::vector<LayerTypes<CustomLayers...> > empty;

  //! Locally-stored gradient object.
  OutputDataType gradient;

  //! Locally-stored input parameter object.
  InputDataType inputParameter;

  //! Parameter which indicates if the modules should be exposed.
  bool model;

  //! Locally-stored network modules.
  std::vector<LayerTypes<CustomLayers...> > network;

  //! Locally stored number of elements in the weights of wrapped layer.
  size_t networkWeightSize;

  //! Locally-stored output parameter object.
  OutputDataType outputParameter;

  //! Locally-stored output parameter visitor module object.
  OutputParameterVisitor outputParameterVisitor;

  // Reset the gradient for all modules that implement the Gradient function.
  void ResetGradients(arma::mat& gradient);

  //! Locally-stored reset visitor.
  ResetVisitor resetVisitor;

  //! Locally-stored scalar parameter.
  OutputDataType scalarParameter;

  //! Locally-stored parameter vector.
  OutputDataType vectorParameter;

  //! Locally-stored parameters.
  OutputDataType weights;

  //! Locally-stored weight size visitor.
  WeightSizeVisitor weightSizeVisitor;
}; // class BatchNorm

} // namespace ann
} // namespace mlpack

// Include the implementation.
#include "weight_norm_impl.hpp"

#endif
