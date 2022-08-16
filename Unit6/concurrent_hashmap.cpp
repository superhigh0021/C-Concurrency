#include <algorithm>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <thread>
#include <vector>
using namespace std;

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table {
private:
    class bucket_type {
    private:
        // friend class threadsafe_lookup_table;
        friend std::map<Key, Value> threadsafe_lookup_table<Key, Value, Hash>::get_map() const;

        using bucket_value = std::pair<Key, Value>;
        using bucket_data = std::list<bucket_value>;
        using bucket_iterator = typename bucket_data::iterator;
        using bucket_const_iterator = typename bucket_data::const_iterator;

        bucket_data data;
        mutable std::shared_mutex mutex;

        bucket_const_iterator find_entry_for(const Key& key) const {
            return std::find_if(data.begin(), data.end(),
                                [&](const bucket_value& item) { return item.first == key; });
        }

        bucket_iterator find_entry_for(const Key& key) {
            return std::find_if(data.begin(), data.end(),
                                [&](const bucket_value& item) { return item.first == key; });
        }

    public:
        Value value_for(const Key& key, const Value& default_value) const {
            std::shared_lock<std::shared_mutex> lock(mutex);
            const bucket_const_iterator found_entry = find_entry_for(key);
            return (found_entry == data.end()) ? default_value : found_entry->second;
        }

        void add_or_update_mapping(const Key& key, const Value& value) {
            std::unique_lock<std::shared_mutex> lock(mutex);
            const bucket_iterator found_entry = find_entry_for(key);
            if (found_entry == data.end())
                data.push_back(bucket_value(key, value));
            else
                found_entry->second = value;
        }

        void remove_mapping(const Key& key) {
            std::unique_lock<std::shared_mutex> lock(mutex);
            const bucket_iterator found_entry = find_entry_for(key);
            if (found_entry != data.end())
                data.erase(found_entry);
        }
    };

    std::vector<std::unique_ptr<bucket_type>> buckets;
    Hash hasher;
    bucket_type& get_bucket(const Key& key) const {
        const size_t bucket_index = hasher(key) % buckets.size();
        return *buckets[bucket_index];
    }

public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef Hash hash_type;

    threadsafe_lookup_table(unsigned num_buckets = 19,
                            const Hash hasher_ = Hash()) : buckets(num_buckets),
                                                           hasher(hasher_) {
        for (unsigned i = 0; i < num_buckets; ++i)
            buckets[i].reset(new bucket_type);
    }

    threadsafe_lookup_table(const threadsafe_lookup_table&) = delete;

    threadsafe_lookup_table* operator=(const threadsafe_lookup_table&) = delete;

    Value value_for(const Key& key, const Value& default_value = Value()) const {
        return get_bucket(key).value_for(key, default_value);
    }

    void add_or_update_mapping(const Key& key, const Value& value) {
        get_bucket(key).add_or_update_mapping(key, value);
    }

    void remove_mapping(const Key& key) {
        get_bucket(key).remove_mapping(key);
    }

    std::map<Key, Value> get_map() const {
        std::vector<std::unique_lock<std::shared_mutex>> locks;
        for (unsigned i = 0; i < buckets.size(); ++i)
            locks.push_back(std::unique_lock<std::shared_mutex>((*buckets[i]).mutex));
        std::map<Key, Value> res{};
        for (unsigned i = 0; i < buckets.size(); ++i)
            for (typename bucket_type::bucket_iterator it = (*buckets[i]).data.begin();
                 it != (*buckets[i]).data.end();
                 ++it)
                res.insert(*it);
        return res;
    }
};

auto main() -> int {
    threadsafe_lookup_table<int, int> mp{};

    std::thread t1(&threadsafe_lookup_table<int, int>::add_or_update_mapping, &mp, 1, 100);
    std::thread t2(&threadsafe_lookup_table<int, int>::add_or_update_mapping, &mp, 2, 200);
    t1.join();
    t2.join();

    std::future<int> f1 = std::async(&threadsafe_lookup_table<int, int>::value_for, &mp, 1, 10);
    std::future<int> f2 = std::async(&threadsafe_lookup_table<int, int>::value_for, &mp, 1, 10);

    std::cout << f1.get() << std::endl;
    std::cout << f2.get() << std::endl;

    auto stdMap = mp.get_map();
    for (auto i = stdMap.begin(); i != stdMap.end(); ++i)
        std::cout << "i->first = " << i->first << ' ' << "i->second = "
                  << i->second << std::endl;

    return 0;
}