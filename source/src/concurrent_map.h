#pragma once

#include <mutex>
#include <map>
#include <vector>
#include <string>

// class ConcurrentMap - functions - declaration - implementation

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
	private:

		struct Bucket {
			std::mutex mutex;
			std::map<Key, Value> map;
		};

		std::vector<Bucket> buckets_;

	public:

		static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

		explicit ConcurrentMap(size_t bucket_count)
			: buckets_(bucket_count)
		{}

		std::map<Key, Value> BuildOrdinaryMap() {
			std::map<Key, Value> result;
			for (auto& [mutex, map] : buckets_) {
				std::lock_guard guard(mutex);
				result.insert(map.begin(), map.end());
			}
			return result;
		}

		auto Erase(const Key& key) {
			uint64_t bucket_num = static_cast<uint64_t>(key) % buckets_.size();
			std::lock_guard guard(buckets_[bucket_num].mutex);
			return buckets_[bucket_num].map.erase(key);
		}

		struct Access {
			std::lock_guard <std::mutex> guard;
			Value& ref_to_value;

			Access(const Key& key, Bucket& bucket)
				: guard(bucket.mutex)
				, ref_to_value(bucket.map[key])
			{
			}
		};

		Access operator[](const Key& key) {
			auto& bucket = buckets_[key % buckets_.size()];
			return { key, bucket };
		} 
};