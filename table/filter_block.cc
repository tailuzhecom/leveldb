// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/filter_block.h"

#include "leveldb/filter_policy.h"
#include "util/coding.h"

namespace leveldb {

// See doc/table_format.md for an explanation of the filter block format.

// Generate new filter every 2KB of data
static const size_t kFilterBaseLg = 11;
static const size_t kFilterBase = 1 << kFilterBaseLg; // 每2KB创建一个新的filter

FilterBlockBuilder::FilterBlockBuilder(const FilterPolicy* policy)
    : policy_(policy) {}

// 每写完一个data block时调用
void FilterBlockBuilder::StartBlock(uint64_t block_offset) {
  uint64_t filter_index = (block_offset / kFilterBase);
  assert(filter_index >= filter_offsets_.size());
  while (filter_index > filter_offsets_.size()) {
    GenerateFilter();
  }
}

// 将key记录到filter中
void FilterBlockBuilder::AddKey(const Slice& key) {
  Slice k = key;
  // 记录key在keys_中的起始地址
  start_.push_back(keys_.size());
  // 将key记录到keys_中
  keys_.append(k.data(), k.size());
}

// 将filter data和filter offset记录到filter block中
// 在写完整个sstable时调用
Slice FilterBlockBuilder::Finish() {
  if (!start_.empty()) {  // 如果还有filter data未生成
    GenerateFilter();
  }

  // Append array of per-filter offsets
  const uint32_t array_offset = result_.size(); // result_对应整个filter block
  // 添加filter data index
  for (size_t i = 0; i < filter_offsets_.size(); i++) {
    PutFixed32(&result_, filter_offsets_[i]);
  }

  PutFixed32(&result_, array_offset);  // 添加filter offset's offset,即filter data index的起始地址
  result_.push_back(kFilterBaseLg);  // Save encoding parameter in result
  return Slice(result_); // 以Slice的形式返回filter block
}

// 生成对应的filter
void FilterBlockBuilder::GenerateFilter() {
  const size_t num_keys = start_.size();  // key的数量
  if (num_keys == 0) {
    // Fast path if there are no keys for this filter
    filter_offsets_.push_back(result_.size());
    return;
  }

  // Make list of keys from flattened key structure
  start_.push_back(keys_.size());  // Simplify length computation
  tmp_keys_.resize(num_keys); // 以slice的形式记录key,将所有的key记录到tmp_keys_中
  for (size_t i = 0; i < num_keys; i++) {
    const char* base = keys_.data() + start_[i];
    size_t length = start_[i + 1] - start_[i];
    tmp_keys_[i] = Slice(base, length);
  }

  // Generate filter for current set of keys and append to result_.
  filter_offsets_.push_back(result_.size()); // 记录filter data的偏移量到filter offset index中
  policy_->CreateFilter(&tmp_keys_[0], static_cast<int>(num_keys), &result_); // 根据keys_生成filter data

  tmp_keys_.clear();
  keys_.clear();
  start_.clear();
}

// content为filter block
FilterBlockReader::FilterBlockReader(const FilterPolicy* policy,
                                     const Slice& contents)
    : policy_(policy), data_(nullptr), offset_(nullptr), num_(0), base_lg_(0) {
  size_t n = contents.size();  // filter block的大小
  if (n < 5) return;  // 1 byte for base_lg_ and 4 for start of offset array
  base_lg_ = contents[n - 1]; // 获取base_lg 1 byte
  uint32_t last_word = DecodeFixed32(contents.data() + n - 5); // 获取filter offset array的起始地址
  if (last_word > n - 5) return; // 如果filter offset array起始地址超出范围
  data_ = contents.data();
  offset_ = data_ + last_word;  // filter data offset起始地址
  num_ = (n - 5 - last_word) / 4; // filter array size / 4 : filter data的数量
}

// 看key是否有可能存在这个block中
bool FilterBlockReader::KeyMayMatch(uint64_t block_offset, const Slice& key) {
  uint64_t index = block_offset >> base_lg_;  // 获取filter data offset
  if (index < num_) {
    uint32_t start = DecodeFixed32(offset_ + index * 4); // 对应filter的起始地址
    uint32_t limit = DecodeFixed32(offset_ + index * 4 + 4); // 对应filter的结束地址
    if (start <= limit && limit <= static_cast<size_t>(offset_ - data_)) {
      Slice filter = Slice(data_ + start, limit - start);
      return policy_->KeyMayMatch(key, filter);
    } else if (start == limit) {
      // Empty filters do not match any keys
      return false;
    }
  }
  return true;  // Errors are treated as potential matches
}

}  // namespace leveldb
