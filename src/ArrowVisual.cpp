///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020, University of Edinburgh, Istituto Italiano di Tecnologia
///////////////////////////////////////////////////////////////////////////////

#include <rcutils/logging_macros.h>  // for RCUTILS_LOG_WARN
#include <rviz_rendering/objects/arrow.hpp>
#include <whole_body_state_rviz_plugin/ArrowVisual.hpp>

namespace whole_body_state_rviz_plugin {

ArrowVisual::ArrowVisual(Ogre::SceneManager *scene_manager, Ogre::SceneNode *parent_node) {
  scene_manager_ = scene_manager;
  frame_node_ = parent_node->createChildSceneNode();
  arrow_ = new rviz_rendering::Arrow(scene_manager_, frame_node_);
}

ArrowVisual::~ArrowVisual() {
  delete arrow_;
  scene_manager_->destroySceneNode(frame_node_);
}

void ArrowVisual::setArrow(const Ogre::Vector3 &position, const Ogre::Quaternion &orientation) {
  arrow_->setPosition(position);
  arrow_->setOrientation(orientation);
}

void ArrowVisual::setFramePosition(const Ogre::Vector3 &position) {
  frame_node_->setPosition(position);
}

void ArrowVisual::setFrameOrientation(const Ogre::Quaternion &orientation) {
  frame_node_->setOrientation(orientation);
}

void ArrowVisual::setColor(float r, float g, float b, float a) {
  arrow_->setColor(r, g, b, a);
}

void ArrowVisual::setProperties(float shaft_length, float shaft_diameter, float head_length, float head_diameter) {
  if (!std::isfinite(shaft_length)) {
    RCUTILS_LOG_WARN("Shaft length is not finite: %f", shaft_length);
    return;
  }
  if (!std::isfinite(shaft_diameter)) {
    RCUTILS_LOG_WARN("Shaft diameter is not finite: %f", shaft_diameter);
    return;
  }
  if (!std::isfinite(head_length)) {
    RCUTILS_LOG_WARN("Head length is not finite: %f", head_length);
    return;
  }
  if (!std::isfinite(head_diameter)) {
    RCUTILS_LOG_WARN("Head diameter is not finite: %f", head_diameter);
    return;
  }

  arrow_->set(shaft_length, shaft_diameter, head_length, head_diameter);
}

}  // namespace whole_body_state_rviz_plugin
