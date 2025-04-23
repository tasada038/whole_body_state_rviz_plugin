///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020, University of Edinburgh
// Copyright (C) 2025, Updated for ROS 2
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef WHOLE_BODY_STATE_RVIZ_PLUGIN_PINOCCHIO_LINK_UPDATER_HPP
#define WHOLE_BODY_STATE_RVIZ_PLUGIN_PINOCCHIO_LINK_UPDATER_HPP

#include <functional>  // ROS 2では boost::function の代わりに std::function を使用
#include <pinocchio/multibody/data.hpp>
#include <pinocchio/multibody/model.hpp>
#include <rviz_default_plugins/robot/link_updater.hpp>
#include <string>

namespace whole_body_state_rviz_plugin {

class PinocchioLinkUpdater : public rviz_default_plugins::robot::LinkUpdater {
 public:
  // ROS 2ではboost::functionの代わりにstd::functionを使用
  // また、StatusLevelの名前空間も変更
  typedef std::function<void(rviz_common::properties::StatusLevel, const std::string &, const std::string &)> StatusCallback;

  PinocchioLinkUpdater(pinocchio::Model &model, pinocchio::Data &data, const Eigen::Ref<const Eigen::VectorXd> &q,
                       const StatusCallback &status_cb = StatusCallback());

  bool getLinkTransforms(const std::string &link_name, Ogre::Vector3 &visual_position,
                         Ogre::Quaternion &visual_orientation, Ogre::Vector3 &collision_position,
                         Ogre::Quaternion &collision_orientation) const override;

  void setLinkStatus(rviz_common::properties::StatusLevel level, const std::string &link_name, const std::string &text) const override;

 private:
  pinocchio::Model &model_;
  pinocchio::Data &data_;
  StatusCallback status_callback_;
};

}  // namespace whole_body_state_rviz_plugin

#endif  // WHOLE_BODY_STATE_RVIZ_PLUGIN_PINOCCHIO_LINK_UPDATER_HPP