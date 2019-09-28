//
// Created by young on 19-9-28.
//


#include <leveldb/filter_policy.h>
#include <iostream>
#include <leveldb/table.h>
#include <leveldb/table_builder.h>

using namespace std;

int main() {
    const leveldb::FilterPolicy *bloom_filter = leveldb::NewBloomFilterPolicy(10);
    leveldb::FilterBlockBuilder filter_block_builder(bloom_filter);

    filter_block_builder.StartBlock(10);
    filter_block_builder.AddKey("hello");
    filter_block_builder.AddKey("world");

    filter_block_builder.StartBlock(3000);
    filter_block_builder.AddKey("Go");
    filter_block_builder.AddKey("Programmer");
    filter_block_builder.StartBlock(20000);

    filter_block_builder.AddKey("a");
    filter_block_builder.AddKey("b");
    filter_block_builder.AddKey("c");

    leveldb::Slice result = filter_block_builder.Finish();
    leveldb::FilterBlockReader filter_block_reader(bloom_filter, result);
    cout << filter_block_reader.KeyMayMatch(0, "hello") << endl;
    cout << filter_block_reader.KeyMayMatch(0, "world") << endl;
    cout << filter_block_reader.KeyMayMatch(0, "Go") << endl;
    cout << filter_block_reader.KeyMayMatch(3000, "Go") << endl;
    cout << filter_block_reader.KeyMayMatch(20000, "b") << endl;
    cout << filter_block_reader.KeyMayMatch(20000, "d") << endl;
}
