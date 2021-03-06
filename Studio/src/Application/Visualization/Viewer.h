#pragma once

#include <QSharedPointer>
#include <Visualization/ColorSchemes.h>
#include <array>
#include <Application/Data/Shape.h>

class vtkRenderer;
class vtkLookupTable;
class vtkRenderWindowInteractor;
class vtkImageData;
class vtkCamera;
class vtkGlyph3D;
class vtkSphereSource;
class vtkImageActor;
class vtkColorTransferFunction;
class vtkArrowSource;
class vtkTransformPolyDataFilter;

class DisplayObject;

class Viewer;

class StudioInteractorStyle;

typedef QSharedPointer< Viewer > ViewerHandle;
typedef QVector< ViewerHandle > ViewerList;

//! 3D Viewer
/*!
 * The Viewer class encapsulates all the functionality for visualizing a single DisplayObject
 *
 */
class Viewer
{

public:

  Viewer();
  ~Viewer();

  void set_renderer(vtkSmartPointer<vtkRenderer> renderer);
  vtkSmartPointer<vtkRenderer> get_renderer();

  void display_object(QSharedPointer<DisplayObject> object);

  void clear_viewer();
  void reset_camera(std::array<double, 3> c);

  void set_glyph_size_and_quality(double size, double quality);
  void set_show_glyphs(bool show);
  void set_show_surface(bool show);

  void update_points();
  void update_glyph_properties();

  int handle_pick(int* click_pos);

  void set_selected_point(int id);

  void set_lut(vtkSmartPointer<vtkLookupTable> lut);

  void set_loading_screen(vtkSmartPointer<vtkImageData> loading_screen);

  void set_color_scheme(int i);

private:

  void display_vector_field();

  void compute_point_differences(const std::vector<Point> &vecs,
                                 vtkSmartPointer<vtkFloatArray> magnitudes,
                                 vtkSmartPointer<vtkFloatArray> vectors);

  void compute_surface_differences(vtkSmartPointer<vtkFloatArray> magnitudes,
                                   vtkSmartPointer<vtkFloatArray> vectors);

  void draw_exclusion_spheres(QSharedPointer<DisplayObject> object);

  void updateDifferenceLUT(float r0, float r1);

  bool visible_;

  QSharedPointer<DisplayObject> object_;

  void update_actors();

  bool show_glyphs_;
  bool show_surface_;

  double glyph_size_;
  double glyph_quality_;

  vtkSmartPointer<vtkRenderer>             renderer_;

  vtkSmartPointer<vtkSphereSource>         sphere_source_;

  vtkSmartPointer<vtkPoints>               glyph_points_;
  vtkSmartPointer<vtkPolyData>             glyph_point_set_;
  vtkSmartPointer<vtkGlyph3D>              glyphs_;
  vtkSmartPointer<vtkPolyDataMapper>       glyph_mapper_;
  vtkSmartPointer<vtkActor>                glyph_actor_;

  vtkSmartPointer<vtkPoints>               exclusion_sphere_points_;
  vtkSmartPointer<vtkPolyData>             exclusion_sphere_point_set_;
  vtkSmartPointer<vtkGlyph3D>              exclusion_sphere_glyph_;
  vtkSmartPointer<vtkPolyDataMapper>       exclusion_sphere_mapper_;
  vtkSmartPointer<vtkActor>                exclusion_sphere_actor_;

  vtkSmartPointer<vtkPolyDataMapper>       surface_mapper_;
  vtkSmartPointer<vtkActor>                surface_actor_;

  vtkSmartPointer<vtkLookupTable>          lut_;

  vtkSmartPointer<StudioInteractorStyle>   style_;

  vtkSmartPointer<vtkImageActor>           image_actor_;

  vtkSmartPointer<vtkColorTransferFunction>   difference_lut_;
  vtkSmartPointer<vtkArrowSource>             arrow_source_;
  vtkSmartPointer<vtkTransformPolyDataFilter> arrow_flip_filter_;
  vtkSmartPointer<vtkGlyph3D>                 arrow_glyphs_;
  vtkSmartPointer<vtkPolyDataMapper>          arrow_glyph_mapper_;
  vtkSmartPointer<vtkActor>                   arrow_glyph_actor_;
  vtkSmartPointer<vtkTransform>               transform_180_;

  bool arrows_visible_ = false;

  ColorSchemes color_schemes_;
  int scheme_;
};
