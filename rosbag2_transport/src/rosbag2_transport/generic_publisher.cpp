// Copyright 2018, Bosch Software Innovations GmbH.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "generic_publisher.hpp"

#include <memory>
#include <string>

namespace rosbag2_transport {

rcl_publisher_options_t rcl_publisher_get_unreliable_options() {
  // !!! MAKE SURE THAT CHANGES TO THESE DEFAULTS ARE REFLECTED IN THE HEADER
  // DOC STRING
  static rcl_publisher_options_t default_options; /* = {
     .ignore_local_publications = false,
   };*/
  // Must set the allocator and qos after because they are not a compile time
  // constant.
  default_options.qos = rmw_qos_profile_default;
  default_options.qos.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
  default_options.qos.durability = RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
  default_options.allocator = rcl_get_default_allocator();
  return default_options;
}

GenericPublisher::GenericPublisher(
    rclcpp::node_interfaces::NodeBaseInterface *node_base,
    const std::string &topic, const rosidl_message_type_support_t &type_support)
    : rclcpp::PublisherBase(node_base, topic, type_support,
                            rcl_publisher_get_unreliable_options()) {}

void GenericPublisher::publish(
    std::shared_ptr<rmw_serialized_message_t> message) {
  auto return_code = rcl_publish_serialized_message(get_publisher_handle(),
                                                    message.get(), NULL);

  if (return_code != RCL_RET_OK) {
    rclcpp::exceptions::throw_from_rcl_error(
        return_code, "failed to publish serialized message");
  }
}

} // namespace rosbag2_transport
