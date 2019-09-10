/*
 * Shapeworks license
 */

/**
 * @file SurfaceReconstructor.h
 * @brief Surface Reconstruction Layer
 *
 * The SurfaceReconstructor wraps the surface reconstruction method
 */

#pragma once

#include <vector>
#include <string>

#include <itkImageFileReader.h>

#include <Libs/SurfaceReconstruction/Reconstruction.h>

class SurfaceReconstructor
{

public:

  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;

  typedef double CoordinateRepType;
  typedef itk::CompactlySupportedRBFSparseKernelTransform < CoordinateRepType,
                                                            3> RBFTransformType;
  typedef itk::ThinPlateSplineKernelTransform2< CoordinateRepType, 3> ThinPlateSplineType;

  typedef itk::LinearInterpolateImageFunction<ImageType, double > InterpolatorType;

  typedef Reconstruction < itk::CompactlySupportedRBFSparseKernelTransform,
                           itk::LinearInterpolateImageFunction,
                           CoordinateRepType, PixelType, ImageType> ReconstructionType;

  typedef typename ReconstructionType::PointType PointType;
  typedef typename ReconstructionType::PointArrayType PointArrayType;

  SurfaceReconstructor();

  void set_filenames(std::vector< std::string > distance_transform_filenames,
                     std::vector< std::string > local_point_filenames,
                     std::vector< std::string > world_point_filenames);

  void generate_mean_dense();

  bool get_surface_reconstruction_avaiable();

  vtkSmartPointer<vtkPolyData> build_mesh(const vnl_vector<double>& shape);

private:
  ReconstructionType reconstructor_;

  bool surface_reconstruction_available_ = false;

  std::vector< std::string > distance_transform_filenames_;
  std::vector< std::string > world_point_filenames_;
  std::vector< std::string > local_point_filenames_;
};
