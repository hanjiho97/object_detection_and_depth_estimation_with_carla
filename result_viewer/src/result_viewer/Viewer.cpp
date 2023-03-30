#include "result_viewer/Viewer.h"

Viewer::Viewer() 
{
  background_ = cv::imread("../resource/background.png");
  generate_color_map();
}

Viewer::~Viewer() {}

void Viewer::generate_color_map()
{
  std::mt19937 e2(1000);
  cv::Scalar color;
  uint8_t R, G, B;
  for (uint8_t index=0; index < COLOR_MAP_SIZE; ++index)
  {
    R = e2() % 256;
    G = e2() % 256;
    B = e2() % 256;
    color = cv::Scalar(R, G, B);
    color_map_.push_back(color);
  }
}

void Viewer::draw_3d_bbox(
  const std::vector<cv::Point>& bbox_points,
  bool showing_head
  )
{
  uint16_t size = bbox_points.size();
  if (showing_head == false)
  {
    size -= 4;
  }
  for (uint8_t index = 0; index < size; index += 2)
  {
    cv::line(
      image_,
      bbox_points[index],
      bbox_points[index + 1],
      color_map_[id_%100],
      2);
  }
}

void Viewer::draw_id(const cv::Point& centor_point)
{
  cv::putText(
    image_, 
    std::to_string(id_), 
    centor_point, 
    cv::FONT_HERSHEY_DUPLEX, 
    0.7, 
    color_map_[id_%100], 
    1, 
    4);
}

void Viewer::draw_topview(const std::vector<cv::Point>& topview_bbox_points)
{
  for (uint8_t index=0; index<topview_bbox_points.size()-1; ++index)
  {
    cv::line(
      background_,
      topview_bbox_points[index],
      topview_bbox_points[index + 1],
      color_map_[id_%100],
      2,
      cv::LINE_AA);
  }
  cv::Point id_point;
  cv::Point xyz_point;
  if (topview_bbox_points[1].y > topview_bbox_points[2].y)
  {
    id_point = topview_bbox_points[2];
    id_point.y -= 5;
    xyz_point = (topview_bbox_points[3]);
    xyz_point.x += 10;
    xyz_point.y += 15; 
  }
  else
  {
    id_point = topview_bbox_points[3];
    id_point.y += 20;
    xyz_point = (topview_bbox_points[1]);
    xyz_point.x += 10;
    xyz_point.y += 15;  
  }
  draw_topview_id(id_point);
  draw_topview_position(xyz_point);
}

void Viewer::draw_topview_id(const cv::Point& centor_point)
{
  cv::putText(
  background_, 
  "id:"+std::to_string(id_), 
  centor_point, 
  cv::FONT_HERSHEY_DUPLEX, 
  0.5, 
  color_map_[id_%100], 
  1, 
  cv::LINE_AA);
}

void Viewer::draw_topview_position(cv::Point& centor_point)
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << attributes_.loc_x;
  cv::putText(
  background_, 
  "x:"+ss.str()+"m",
  centor_point,
  cv::FONT_HERSHEY_DUPLEX, 
  0.4, 
  color_map_[id_%100], 
  1, 
  cv::LINE_AA);

  ss.str("");
  ss.clear();
  ss << std::fixed << std::setprecision(2) << attributes_.loc_z;
  centor_point.y += 12;
  cv::putText(
  background_, 
  "y:"+ss.str()+"m",
  centor_point, 
  cv::FONT_HERSHEY_DUPLEX, 
  0.4, 
  color_map_[id_%100], 
  1, 
  cv::LINE_AA); 

  ss.str("");
  ss.clear();
  ss << std::fixed << std::setprecision(2) << attributes_.loc_y;
  centor_point.y += 12;
  cv::putText(
  background_, 
  "z:"+ss.str()+"m",
  centor_point, 
  cv::FONT_HERSHEY_DUPLEX, 
  0.4, 
  color_map_[id_%100], 
  1, 
  cv::LINE_AA);
}

void Viewer::draw(
  bool show_bbox_3D,
  bool showing_head, 
  bool showing_id,
  bool showing_topview)
{
  std::vector<cv::Point> bbox_points;
  std::vector<cv::Point> top_view_bbox_points;
  cv::Point centor_point;
  bbox_points = projection_.get_2D_corners(attributes_, P2_);
  top_view_bbox_points = projection_.get_topview_conrers();
  centor_point = (bbox_points[24] + bbox_points[27]) / 2;
  centor_point.y -= 10;
  if (show_bbox_3D == true)
  {
    draw_3d_bbox(bbox_points, showing_head);
  }
  if (showing_id == true)
  {
    draw_id(centor_point);
  }
  if (showing_topview == true)
  {
    draw_topview(top_view_bbox_points);
  }
}

void Viewer::read_P2_matrix(const Eigen::Matrix<double, 3, 4>& P2)
{
  P2_ = P2;
}

void Viewer::add_image(const cv::Mat& image)
{
  image_ = image;
  background_ = cv::imread("../car/background.png");
}

void Viewer::add_3d_bbox(
  uint16_t id,
  const Attributes& attributes)
{
  id_ = id;
  attributes_ = attributes;
}

void Viewer::show_result()
{
  cv::imshow("top_view", background_);
  cv::imshow("result_image", image_);
  cv::waitKey(75);
}