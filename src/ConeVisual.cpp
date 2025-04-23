// BSD 3-Clause License
//
// Copyright (C) 2021, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.

#include <rclcpp/rclcpp.hpp>
#include <rviz_common/properties/float_property.hpp>
#include <rviz_rendering/objects/shape.hpp>
#include <whole_body_state_rviz_plugin/ConeVisual.hpp>


namespace whole_body_state_rviz_plugin {

ConeVisual::ConeVisual(Ogre::SceneManager *scene_manager, Ogre::SceneNode *parent_node) {
  scene_manager_ = scene_manager;

  // Ogre::SceneNode s form a tree, with each node storing the transform
  // (position and orientation) of itself relative to its parent. Ogre does
  // the math of combining those transforms when it is time to render.
  // Here we create a node to store the pose of the Point's header frame
  // relative to the RViz fixed frame.
  frame_node_ = parent_node->createChildSceneNode();

  // We create the cone object within the frame node so that we can set its
  // position and direction relative to its header frame.
  cone_ = new rviz_rendering::Shape(rviz_rendering::Shape::Cone, scene_manager_, frame_node_);
}

ConeVisual::~ConeVisual() {
  // Delete the cone to make it disappear.
  delete cone_;

  // Destroy the frame node since we don't need it anymore.
  scene_manager_->destroySceneNode(frame_node_);
}

void ConeVisual::setCone(const Ogre::Vector3 &position, const Ogre::Quaternion &orientation) {
  cone_->setPosition(position);
  cone_->setOrientation(orientation);
}

void ConeVisual::setFramePosition(const Ogre::Vector3 &position) { frame_node_->setPosition(position); }

void ConeVisual::setFrameOrientation(const Ogre::Quaternion &orientation) { frame_node_->setOrientation(orientation); }

void ConeVisual::setColor(float r, float g, float b, float a) { cone_->setColor(r, g, b, a); }

void ConeVisual::setProperties(float width, float length) {
  if (!std::isfinite(width)) {
    RCLCPP_WARN(rclcpp::get_logger("cone_visual"), "Cone width is not finite: %f", width);
    return;
  }
  if (!std::isfinite(length)) {
    RCLCPP_WARN(rclcpp::get_logger("cone_visual"), "Cone length is not finite: %f", length);
    return;
  }
  cone_->setScale(Ogre::Vector3(width, length, width));
  cone_->setOffset(Ogre::Vector3(0., -0.5, 0.));
}

}