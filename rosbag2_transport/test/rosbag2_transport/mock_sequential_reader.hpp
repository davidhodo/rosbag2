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

#ifndef ROSBAG2_TRANSPORT__MOCK_SEQUENTIAL_READER_HPP_
#define ROSBAG2_TRANSPORT__MOCK_SEQUENTIAL_READER_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "rosbag2_cpp/reader_interfaces/base_reader_interface.hpp"

class MockSequentialReader : public rosbag2_cpp::reader_interfaces::BaseReaderInterface
{
public:
  void open(
    const rosbag2_cpp::StorageOptions & storage_options,
    const rosbag2_cpp::ConverterOptions & converter_options) override
  {
    (void) storage_options;
    (void) converter_options;
  }

  void reset() override {}

  bool has_next() override
  {
    return num_read_ < messages_.size();
  }

  std::shared_ptr<rosbag2_storage::SerializedBagMessage> read_next() override
  {
    return messages_[num_read_++];
  }

  std::vector<rosbag2_storage::TopicMetadata> get_all_topics_and_types() override
  {
    return topics_;
  }

  void prepare(
    std::vector<std::shared_ptr<rosbag2_storage::SerializedBagMessage>> messages,
    std::vector<rosbag2_storage::TopicMetadata> topics)
  {
    messages_ = std::move(messages);
    topics_ = std::move(topics);
  }

private:
  std::vector<std::shared_ptr<rosbag2_storage::SerializedBagMessage>> messages_;
  std::vector<rosbag2_storage::TopicMetadata> topics_;
  size_t num_read_;
};

#endif  // ROSBAG2_TRANSPORT__MOCK_SEQUENTIAL_READER_HPP_
