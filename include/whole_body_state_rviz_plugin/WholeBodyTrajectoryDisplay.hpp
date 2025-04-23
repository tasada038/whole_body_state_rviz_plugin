///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020-2021, University of Edinburgh, Istituto Italiano di
// Tecnologia Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef WHOLE_BODY_STATE_RVIZ_PLUGIN_WHOLE_BODY_TRAJECTORY_DISPLAY_H
#define WHOLE_BODY_STATE_RVIZ_PLUGIN_WHOLE_BODY_TRAJECTORY_DISPLAY_H

#include "whole_body_state_rviz_plugin/ArrowVisual.hpp"
#include "whole_body_state_rviz_plugin/PointVisual.hpp"
#include <pinocchio/multibody/data.hpp>
#include <pinocchio/multibody/model.hpp>
#include <rviz_common/message_filter_display.hpp>

#include <rviz_common/properties/color_property.hpp>
#include <rviz_common/properties/enum_property.hpp>
#include <rviz_common/properties/float_property.hpp>
#include <rviz_common/properties/int_property.hpp>
#include <rviz_common/properties/string_property.hpp>
#include <rviz_common/properties/bool_property.hpp>
#include <rviz_common/properties/status_property.hpp>
#include <rviz_common/properties/property.hpp>

#include <rviz_default_plugins/robot/robot.hpp>
#include <rviz_rendering/objects/axes.hpp>
#include <rviz_rendering/objects/billboard_line.hpp>
#include "whole_body_state_msgs/msg/whole_body_trajectory.hpp"

#include <memory>

#include <rclcpp/rclcpp.hpp>

namespace whole_body_state_rviz_plugin {
using rviz_common::properties::BoolProperty;
using rviz_common::properties::ColorProperty;
using rviz_common::properties::FloatProperty;
using rviz_common::properties::StringProperty;
using rviz_common::properties::EnumProperty;
using rviz_common::properties::StatusProperty;
/**
 * @class WholeBodyTrajectoryDisplay
 * @brief Displays a whole_body_state_msgs::WholeBodyTrajectory message
 */
class WholeBodyTrajectoryDisplay : public rviz_common::MessageFilterDisplay<whole_body_state_msgs::msg::WholeBodyTrajectory> {
  Q_OBJECT
public:
  /** @brief Constructor function */
  WholeBodyTrajectoryDisplay();

  /** @brief Destructor function */
  ~WholeBodyTrajectoryDisplay();

  /** @brief Clear the visuals by deleting their objects */
  void reset() override;

  /** @brief Initialization procedure of the plugin. */
  void onInitialize() override;

  /** @brief Enable procedure of the plugin. */
  void onEnable() override;

  /** @brief Disable procedure of the plugin. */
  void onDisable() override;

  /** @brief Called when the fixed frame changed */
  void fixedFrameChanged() override;

  /**
   * @brief Function to handle an incoming ROS message
   * This is our callback to handle an incoming message
   * @param const whole_body_state_msgs::WholeBodyTrajectory::ConstSharedPtr&
   * Whole-body trajectory msg
   */
  void processMessage(whole_body_state_msgs::msg::WholeBodyTrajectory::ConstSharedPtr msg) override;

  /** @brief render callback */
  void update(float wall_dt, float ros_dt) override;

private Q_SLOTS:
  /**@{*/
  /** Helper functions to apply color and alpha to all visuals.
   * Set the current color and alpha values for each visual */
  void updateTargetEnable();
  void updateRobotDescription();
  void updateRobotVisualVisible();
  void updateRobotCollisionVisible();
  void updateRobotAlpha();
  void updateForceColorAndAlpha();
  void updateForceArrowGeometry();
  void updateCoMEnable();
  void updateCoMStyle();
  void updateCoMLineProperties();
  void updateContactEnable();
  void updateContactStyle();
  void updateContactLineProperties();
  void pushBackCoMAxes(const Ogre::Vector3 &axes_position, const Ogre::Quaternion &axes_orientation);
  void pushBackContactAxes(const Ogre::Vector3 &axes_position, const Ogre::Quaternion &axes_orientation);
  /**@}*/

private:
  /**@{*/
  /** Process the trajectories */
  void processTargetPosture();
  void processCoMTrajectory();
  void processContactTrajectory();
  /**@}*/

  /** @brief Load the robot model */
  void loadRobotModel();

  /** @brief Clear the robot model */
  void clearRobotModel();

  /** @brief Destroy all the objects for visualization */
  void destroyObjects();

  /** @brief Whole-body trajectory message */
  whole_body_state_msgs::msg::WholeBodyTrajectory::ConstSharedPtr msg_;

  /** @brief ROS node for parameter access */
  rclcpp::Node::SharedPtr ros_node_;

  bool has_new_msg_;  ///< Callback sets this to tell our update function
                      ///< it needs to update the model

  /**@{*/
  /** Properties to show on side panel */
  Property *target_category_;
  Property *com_category_;
  Property *contact_category_;
  /**@}*/

  /**@{*/
  /** Object for visualization of the data */
  std::shared_ptr<rviz_default_plugins::robot::Robot> robot_;
  std::shared_ptr<Ogre::ManualObject> com_manual_object_;
  std::shared_ptr<rviz_rendering::BillboardLine> com_billboard_line_;
  std::vector<std::shared_ptr<PointVisual>> com_points_;
  std::vector<std::shared_ptr<rviz_rendering::Axes>> com_axes_;
  std::vector<std::shared_ptr<Ogre::ManualObject>> contact_manual_object_;
  std::vector<std::shared_ptr<rviz_rendering::BillboardLine>> contact_billboard_line_;
  std::vector<std::vector<std::shared_ptr<PointVisual>>> contact_points_;
  std::vector<std::shared_ptr<rviz_rendering::Axes>> contact_axes_;
  std::vector<std::shared_ptr<ArrowVisual>> force_visual_;
  /**@}*/

  /**@{*/
  /** Property objects for user-editable properties */
  BoolProperty *target_enable_property_;
  StringProperty *robot_description_property_;
  Property *robot_visual_enabled_property_;
  Property *robot_collision_enabled_property_;
  FloatProperty *robot_alpha_property_;
  BoolProperty *force_enable_property_;
  ColorProperty *force_color_property_;
  FloatProperty *force_alpha_property_;
  FloatProperty *force_head_radius_property_;
  FloatProperty *force_head_length_property_;
  FloatProperty *force_shaft_radius_property_;
  FloatProperty *force_shaft_length_property_;
  BoolProperty *com_enable_property_;
  EnumProperty *com_style_property_;
  ColorProperty *com_color_property_;
  FloatProperty *com_alpha_property_;
  FloatProperty *com_line_width_property_;
  FloatProperty *com_scale_property_;
  BoolProperty *contact_enable_property_;
  EnumProperty *contact_style_property_;
  ColorProperty *contact_color_property_;
  FloatProperty *contact_alpha_property_;
  FloatProperty *contact_line_width_property_;
  FloatProperty *contact_scale_property_;
  /**@}*/

  /**@{*/
  /** Robot variables */
  std::string robot_description_;
  pinocchio::Model model_;
  pinocchio::Data data_;
  double weight_;
  /**@}*/

  Ogre::Vector3 last_point_position_;
  enum LineStyle { BILLBOARDS, LINES, POINTS };

  /**@{*/
  /** Flag that indicates if the category are enable */
  bool target_enable_;
  bool com_enable_;
  bool com_axes_enable_;
  bool contact_enable_;
  bool contact_axes_enable_;
  /**@}*/
};

}  // namespace whole_body_state_rviz_plugin

#endif  // WHOLE_BODY_STATE_RVIZ_PLUGIN_WHOLE_BODY_TRAJECTORY_DISPLAY_H