#ifndef STUDIO_DATA_SHAPE_H
#define STUDIO_DATA_SHAPE_H

#include <QSharedPointer>
#include <QString>

#include <Data/Mesh.h>

class Shape;
typedef QSharedPointer< Shape > ShapeHandle;
typedef QVector< ShapeHandle > ShapeList;

//! Representation of a single shape/patient.
class Shape
{

public:
  Shape();
  ~Shape();

  /// Import the original raw image file
  void import_original_image( QString filename, float iso_value );

  /// Retrieve the original mesh
  QSharedPointer<Mesh> get_original_mesh();

  /// Import the groomed raw image file
  void import_groomed_image( QString filename );

  /// Retrieve the groomed mesh
  QSharedPointer<Mesh> get_groomed_mesh();

  /// Import correspondence point file
  bool import_point_file( QString filename );

  /// Retrieve the reconstructed mesh
  QSharedPointer<Mesh> get_reconstructed_mesh();

  /// Get the correspondence points
  vnl_vector<double> get_correspondence_points();

  /// Get the id of this shape
  int get_id();

  /// Set the id of this shape
  void set_id( int id );

  QString get_original_filename();
  QString get_original_filename_with_path();

  QString get_groomed_filename();
  QString get_groomed_filename_with_path();

  QString get_point_filename();
  QString get_point_filename_with_path();

private:

  int id_;

  QSharedPointer<Mesh> original_mesh_;
  QSharedPointer<Mesh> groomed_mesh_;
  QSharedPointer<Mesh> reconstructed_mesh_;

  QString original_mesh_filename_;
  QString groomed_mesh_filename_;
  QString point_filename_;

  vnl_vector<double> correspondence_points_;
};

#endif /* STUDIO_DATA_SHAPE_H */
