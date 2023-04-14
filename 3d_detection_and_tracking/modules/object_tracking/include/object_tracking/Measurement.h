#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Eigen/Dense"

#include "3d_detection_and_tracking/Type.h"
#include "object_tracking/Dataloader.h"

class Measurement
{
public:
  Measurement(
    uint16_t frame_count,
    uint16_t detection_count,
    const kitti::Data &kitti_data);
  virtual ~Measurement();

  Eigen::VectorXd get_z() const;
  Eigen::MatrixXd get_R() const;
  Eigen::VectorXd get_hx(const Eigen::VectorXd &x) const;
  Eigen::MatrixXd get_H(const Eigen::VectorXd &x) const;
  Eigen::MatrixXd get_cam_to_veh() const;
  Eigen::MatrixXd get_veh_to_cam() const;
  double get_t() const;
  Attributes get_attributes() const;
  static void kitti_to_measurement_list(uint frame_count, const kitti::Data& kitti_data, std::vector<Measurement>& meas_list);
  void print() const;

private:
  Eigen::VectorXd z_;
  Eigen::MatrixXd cam_to_veh_;
  Eigen::MatrixXd veh_to_cam_;
  Eigen::MatrixXd R_;

  double t_;

  std::string type_;
  double width_;
  double length_;
  double height_;
  double rot_y_;
};



#endif  // MEASUREMENT_H_