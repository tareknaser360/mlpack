/**
 * @file tests/main_tests/preprocess_imputer_test.cpp
 * @author Manish Kumar
 *
 * Test RUN_BINDING() of preprocess_imputer_main.cpp.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#define BINDING_TYPE BINDING_TYPE_TEST

#include <mlpack/core.hpp>
#include <mlpack/methods/preprocess/preprocess_imputer_main.cpp>
#include <mlpack/core/util/mlpack_main.hpp>

#include "main_test_fixture.hpp"

#include "../test_catch_tools.hpp"
#include "../catch.hpp"

using namespace mlpack;

BINDING_TEST_FIXTURE(PreprocessImputerTestFixture);

/**
 * Check that input and output have same dimensions
 * except for listwise_deletion strategy.
 */
TEST_CASE_METHOD(
    PreprocessImputerTestFixture, "PreprocessImputerDimensionTest",
    "[PreprocessImputerMainTest][BindingTests]")
{
  // Load synthetic dataset.
  arma::mat inputData;
  DatasetMapper<mlpack::data::MissingPolicy> info;
  if (!data::Load("preprocess_imputer_test.csv", inputData, info))
      FAIL("Cannot load preprocess_imputer_test.csv");

  arma::mat outputData;

  // Store size of input dataset.
  size_t inputSize = inputData.n_cols;

  // Input custom data points and labels.
  SetInputParam("input",  std::make_tuple(info, inputData));
  SetInputParam("missing_value", (std::string) "nan");

  // Check for mean strategy.
  SetInputParam("strategy", (std::string) "mean");


  RUN_BINDING();

  // Now check that the output has desired dimensions.
  outputData = params.Get<arma::mat>("output");
  REQUIRE(outputData.n_cols == inputSize);
  REQUIRE(outputData.n_rows == 3); // Input Dimension.

  // Reset passed strategy.
  ResetSettings();

  // Check for median strategy.
  SetInputParam("input",  std::make_tuple(info, inputData));
  SetInputParam("missing_value", (std::string) "nan");
  SetInputParam("strategy", (std::string) "median");

  RUN_BINDING();

  // Now check that the output has desired dimensions.
  outputData = params.Get<arma::mat>("output");
  REQUIRE(outputData.n_cols == inputSize);
  REQUIRE(outputData.n_rows == 3); // Input Dimension.

  // Reset passed strategy.
  ResetSettings();

  // Check for custom strategy.
  SetInputParam("input",  std::make_tuple(info, inputData));
  SetInputParam("missing_value", (std::string) "nan");
  SetInputParam("strategy", (std::string) "custom");
  SetInputParam("custom_value", (double) 75.12);

  RUN_BINDING();

  // Now check that the output has desired dimensions.
  outputData = params.Get<arma::mat>("output");
  REQUIRE(outputData.n_cols == inputSize);
  REQUIRE(outputData.n_rows == 3); // Input Dimension.
}

/**
 * Check that output has fewer points in case of listwise_deletion strategy.
 */
TEST_CASE_METHOD(
    PreprocessImputerTestFixture, "PreprocessImputerListwiseDimensionTest",
    "[PreprocessImputerMainTest][BindingTests]")
{
  // Load synthetic dataset.
  arma::mat inputData;
  DatasetMapper<mlpack::data::MissingPolicy> info;
  if (!data::Load("preprocess_imputer_test.csv", inputData, info))
      FAIL("Cannot load preprocess_imputer_test.csv!");

  // Store size of input dataset.
  size_t inputSize  = inputData.n_cols;
  size_t countNaN = 0;

  // Count number of unavailable entries in all dimensions.
  for (size_t i = 0; i < inputSize; ++i)
  {
    if (std::to_string(inputData(0, i)) == "nan" ||
        std::to_string(inputData(1, i)) == "nan" ||
        std::to_string(inputData(2, i)) == "nan")
    {
      countNaN++;
    }
  }

  // Input custom data points and labels.
  SetInputParam("input",  std::make_tuple(info, inputData));
  SetInputParam("missing_value", (std::string) "nan");
  SetInputParam("strategy", (std::string) "listwise_deletion");

  RUN_BINDING();

  // Now check that the output has desired dimensions.
  arma::mat outputData = params.Get<arma::mat>("output");
  REQUIRE(outputData.n_cols + countNaN == inputSize);
  REQUIRE(outputData.n_rows == 3); // Input Dimension.
}

/**
 * Check that invalid strategy can't be specified.
 */
TEST_CASE_METHOD(
    PreprocessImputerTestFixture, "PreprocessImputerStrategyTest",
    "[PreprocessImputerMainTest][BindingTests]")
{
  // Load synthetic dataset.
  arma::mat inputData;
  DatasetMapper<mlpack::data::MissingPolicy> info;
  if (!data::Load("preprocess_imputer_test.csv", inputData, info))
      FAIL("Cannot load preprocess_imputer_test.csv");


  // Input custom data points and labels.
  SetInputParam("input",  std::make_tuple(info, inputData));
  SetInputParam("missing_value", (std::string) "nan");
  SetInputParam("strategy", (std::string) "notmean"); // Invalid.

  REQUIRE_THROWS_AS(RUN_BINDING(), std::runtime_error);
}
