///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020, University of Edinburgh
// Copyright (C) 2025, Updated for ROS 2
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "whole_body_state_rviz_plugin/PinocchioLinkUpdater.hpp"
#include <rviz_common/frame_manager_iface.hpp>
#include <rviz_common/properties/status_property.hpp>

#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <pinocchio/algorithm/frames.hpp>

namespace whole_body_state_rviz_plugin {

PinocchioLinkUpdater::PinocchioLinkUpdater(pinocchio::Model &model, pinocchio::Data &data,
                                           const Eigen::Ref<const Eigen::VectorXd> &q, const StatusCallback &status_cb)
    : model_(model), data_(data), status_callback_(status_cb) {
  pinocchio::framesForwardKinematics(model_, data_, q);
}

bool PinocchioLinkUpdater::getLinkTransforms(const std::string &link_name, Ogre::Vector3 &visual_position,
                                             Ogre::Quaternion &visual_orientation, Ogre::Vector3 &collision_position,
                                             Ogre::Quaternion &collision_orientation) const {
  if (model_.existFrame(link_name)) {
    pinocchio::FrameIndex frameId = model_.getFrameId(link_name);
    const Eigen::Vector3d &translation = data_.oMf[frameId].translation();
    Eigen::Quaterniond quaternion(data_.oMf[frameId].rotation());
    Ogre::Vector3 position(translation[0], translation[1], translation[2]);
    Ogre::Quaternion orientation(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());

    // Collision/visual transforms are the same in this case
    visual_position = position;
    visual_orientation = orientation;
    collision_position = position;
    collision_orientation = orientation;
    setLinkStatus(rviz_common::properties::StatusProperty::Ok, link_name, "Frame OK");
  } else {
    std::stringstream ss;
    ss << "Do not exist frame [" << link_name << "]";
    setLinkStatus(rviz_common::properties::StatusProperty::Error, link_name, ss.str());
    return false;
  }
  return true;
}

void PinocchioLinkUpdater::setLinkStatus(rviz_common::properties::StatusLevel level, const std::string &link_name,
                                         const std::string &text) const {
  if (status_callback_) {
    status_callback_(level, link_name, text);
  }
}

}  // namespace whole_body_state_rviz_plugin