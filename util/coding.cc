// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/coding.h"

namespace leveldb {

// dst追加32bit的value,将value完整地存入dst中
void PutFixed32(std::string* dst, uint32_t value) {
  char buf[sizeof(value)];
  EncodeFixed32(buf, value);
  dst->append(buf, sizeof(buf));
}

// dst追加64bit的value,将value完整地存入dst中
void PutFixed64(std::string* dst, uint64_t value) {
  char buf[sizeof(value)];
  EncodeFixed64(buf, value);
  dst->append(buf, sizeof(buf));
}

// 将32位无符号整数v以Varint的形式存储在dst中
char* EncodeVarint32(char* dst, uint32_t v) {
  // Operate on characters as unsigneds
  uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
  static const int B = 128;
  if (v < (1 << 7)) {   // 如果v可以用1 byte存储
    *(ptr++) = v;
  } else if (v < (1 << 14)) { // 如果v可以用2 byte存储
    *(ptr++) = v | B; // 第一个byte最高位为1表示后面还有字节用于存储v
    *(ptr++) = v >> 7;
  } else if (v < (1 << 21)) { // 如上
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = v >> 14;
  } else if (v < (1 << 28)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = v >> 21;
  } else { // 最多需要5个byte用于存储v,第5个字节中最多使用4 bits
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = (v >> 21) | B;
    *(ptr++) = v >> 28;
  }
  return reinterpret_cast<char*>(ptr);  // 返回unused部分的字节
}

// 将v以varint的形式append到dst中
void PutVarint32(std::string* dst, uint32_t v) {
  char buf[5];  // uint32_t最多需要5 byte
  char* ptr = EncodeVarint32(buf, v); // ptr为未被使用的部分
  dst->append(buf, ptr - buf);
}

// 将32位无符号整数v以Varint的形式存储在dst中
char* EncodeVarint64(char* dst, uint64_t v) {
  static const int B = 128;
  uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
  // 从高位到低位
  while (v >= B) {
    *(ptr++) = v | B;
    v >>= 7;
  }
  // 处理最后一个字节
  *(ptr++) = static_cast<uint8_t>(v);
  return reinterpret_cast<char*>(ptr);
}

// 将uint64_t v以varint的形式append到dst中
void PutVarint64(std::string* dst, uint64_t v) {
  char buf[10]; // uint64_t最多需要10个字节,需要最多位的情况下第8个字节还需要8位,第9个字节只能存7位, 7*9=63<64
  char* ptr = EncodeVarint64(buf, v); // ptr为未被使用的部分
  dst->append(buf, ptr - buf); // size = ptr - buf
}

// 存储value.size,再对value进行存储
void PutLengthPrefixedSlice(std::string* dst, const Slice& value) {
  PutVarint32(dst, value.size());
  dst->append(value.data(), value.size());
}

// 计算用Varint的格式存储数字v需要多少个字节
int VarintLength(uint64_t v) {
  int len = 1;
  while (v >= 128) {
    v >>= 7;
    len++;
  }
  return len;
}

// 获取uint32_t值，将存储在value中,返回剩下待处理的数据
const char* GetVarint32PtrFallback(const char* p, const char* limit,
                                   uint32_t* value) {
  uint32_t result = 0;

  for (uint32_t shift = 0; shift <= 28 && p < limit; shift += 7) {
    uint32_t byte = *(reinterpret_cast<const uint8_t*>(p));
    p++;
    if (byte & 128) { // 还有后续的byte未处理
      // More bytes are present
      result |= ((byte & 127) << shift); // 提取低7位
    } else {    // 要处理的最后一个字节
      result |= (byte << shift);
      *value = result;  // 将结果存储在value中
      return reinterpret_cast<const char*>(p); // 返回未处理数据的首地址
    }
  }
  return nullptr; // 要获取的值存储超过5 byte
}

// 解析input,获取value的值,返回的值为是否获取成功
bool GetVarint32(Slice* input, uint32_t* value) {
  const char* p = input->data();
  const char* limit = p + input->size(); // limit为input的结束边界
  const char* q = GetVarint32Ptr(p, limit, value);
  if (q == nullptr) {   // 处理时出现错误
    return false;
  } else {
    *input = Slice(q, limit - q);
    return true;
  }
}

// 获取uint64_t值，将存储在value中,返回剩下待处理的数据
const char* GetVarint64Ptr(const char* p, const char* limit, uint64_t* value) {
  uint64_t result = 0;
  for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
    uint64_t byte = *(reinterpret_cast<const uint8_t*>(p));
    p++;
    if (byte & 128) { // 还有后续的byte未处理
      // More bytes are present
      result |= ((byte & 127) << shift); // 提取低7位
    } else { // 要处理的最后一个字节
      result |= (byte << shift);
      *value = result;
      return reinterpret_cast<const char*>(p); // 返回未处理数据的首地址
    }
  }
  return nullptr;
}

// 解析input,获取value的值,返回的值为是否获取成功
bool GetVarint64(Slice* input, uint64_t* value) {
  const char* p = input->data();
  const char* limit = p + input->size();
  const char* q = GetVarint64Ptr(p, limit, value);
  if (q == nullptr) {
    return false;
  } else {
    *input = Slice(q, limit - q);
    return true;
  }
}

// p中存储的格式为先用varint把data的长度存储，再存储data
// 在这个函数中先获取data的size,再通过size获取data，最后返回未处理的部分
const char* GetLengthPrefixedSlice(const char* p, const char* limit,
                                   Slice* result) {
  uint32_t len;
  p = GetVarint32Ptr(p, limit, &len); // 获取data的长度
  if (p == nullptr) return nullptr;
  if (p + len > limit) return nullptr; // data的长度超过p的边界
  *result = Slice(p, len); // 获取data
  return p + len;  // 返回为处理的部分
}

// 从input中获取data,返回值为函数是否成功
bool GetLengthPrefixedSlice(Slice* input, Slice* result) {
  uint32_t len;
  // 注意GetVarint32后input指向data的起始地址
  if (GetVarint32(input, &len) && input->size() >= len) {
    *result = Slice(input->data(), len);
    input->remove_prefix(len);
    return true;
  } else {
    return false;
  }
}

}  // namespace leveldb
