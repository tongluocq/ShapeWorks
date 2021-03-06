#include <gtest/gtest.h>

#include <Libs/Image/Image.h>

#include "TestConfiguration.h"

using namespace shapeworks;

//---------------------------------------------------------------------------
TEST(ImageTests, resample_binary_isotropic_test) {

  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/resample/");

  Image image(test_location + "binary-isotropic-input.nrrd");
  image.antialias();
  image.isoresample();
  image.binarize();
  image.recenter();
  Image ground_truth(test_location + "binary-isotropic-resampled.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

TEST(ImageTests, resample_binary_anisotropic_test) {

  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/resample/");

  Image image(test_location + "binary-anisotropic-input.nrrd");
  image.antialias();
  image.isoresample();
  image.binarize();
  image.recenter();
  Image ground_truth(test_location + "binary-anisotropic-resampled.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

TEST(ImageTests, resample_smooth_isotropic_test) {

  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/resample/");

  Image image(test_location + "smooth-isotropic-input.nrrd");
  image.isoresample();
  image.recenter();
  Image ground_truth(test_location + "smooth-isotropic-resampled.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

TEST(ImageTests, resample_smooth_anisotropic_test) {

  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/resample/");

  Image image(test_location + "smooth-anisotropic-input.nrrd");
  image.isoresample();
  image.recenter();
  Image ground_truth(test_location + "smooth-anisotropic-resampled.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

TEST(ImageTests, padimage_test) {
  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/padimage/");

  Image image(test_location + "1x2x2.nrrd");
  image.pad(30, 0.0);
 
  Image ground_truth(test_location + "padimage_baseline.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

TEST(ImageTests, padimage_identity_test) {
  std::string test_location = std::string(TEST_DATA_DIR) + std::string("/padimage/");

  Image image(test_location + "1x2x2.nrrd");
  image.pad(0, 0.0);
  Image ground_truth(test_location + "1x2x2.nrrd");

  ASSERT_TRUE(image.compare_equal(ground_truth));
}

// TEST(ImageTests, binarize_test) {

// std::string test_location = std::string(TEST_DATA_DIR) + std::string("/binarize/");

// Image image(test_location + "1x2x2.nrrd");
// image.binarize();
// Image ground_truth(test_location + "binarize_baseline.nrrd");

// ASSERT_TRUE(image.compare_equal(ground_truth));

// image.binarize();

// ASSERT_TRUE(image.compare_equal(ground_truth));

// }

//TEST(ImageTests, blah_test) {

// ...

// }
