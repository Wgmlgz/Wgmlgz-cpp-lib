#include <vector>

struct VCache {
  virtual void clear() {}
};
struct CacheHandler {
  std::vector<VCache*> caches;
  void add(VCache* new_cache) { caches.push_back(new_cache); }
  void clear() {
    for (const auto& i : caches) i->clear();
  }
};
CacheHandler cache_handler;
template <typename R, typename... A>
class Cache : public VCache {
  std::function<R(A...)> f_;
  std::map<std::tuple<A...>, R> map_;

 public:
  Cache(std::function<R(A...)> f) : f_(f) { cache_handler.add(this); }
  R operator()(A... a) {
    std::tuple<A...> key(a...);
    auto search = map_.find(key);
    if (search != map_.end()) return search->second;
    auto result = f_(a...);
    map_[key] = result;
    return result;
  }
  void clear() { map_.clear(); }
  void operator!() { clear(); }
};