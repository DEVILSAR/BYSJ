#include <bitset>
#include <string>
using std::bitset;
using std::string;

struct BKDRHash {
	size_t operator()(const string& s) {
		size_t value = 0;
		for (auto ch : s) {
			value *= 131;
			value += ch;
		}
		return value;
	}
};

struct APHash {
	size_t operator()(const string& s) {
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++) {
			if ((i & 1) == 0) {
				hash ^= ((hash << 7) ^ s[i] ^ (hash >> 3));
			}
			else {
				hash ^= (~((hash << 11) ^ s[i] ^ (hash >> 5)));
			}
		}
		return hash;
	}
};

struct DJBHash {
	size_t operator()(const string& s) {
		size_t hash = 5381;
		for (auto ch : s) {
			hash += (hash << 5) + ch;
		}
		return hash;
	}
};

template<size_t N,	//数据范围
	size_t X = 5,    //每个元素最多消耗的比特位的位数
	class K = string,    //数据类型
	class HashFunc1 = BKDRHash,   //哈希函数
	class HashFunc2 = APHash,
	class HashFunc3 = DJBHash>
	class BloomFilter {
	public:
		void set(const K& key) {
			size_t hash1 = HashFunc1()(key) % (N * X);
			size_t hash2 = HashFunc2()(key) % (N * X);
			size_t hash3 = HashFunc3()(key) % (N * X);

			//将三个位置的比特位都置1才表示插入key
			_bs.set(hash1);
			_bs.set(hash2);
			_bs.set(hash3);
		}

		bool test(const K& key) {
			size_t hash1 = HashFunc1()(key) % (N * X);
			size_t hash2 = HashFunc2()(key) % (N * X);
			size_t hash3 = HashFunc3()(key) % (N * X);

			//key映射的三个下标位置都为真才表示key可能存在
			//也可能不存在，此时映射的位置全部冲突，从而发生误判
			if (_bs.test(hash1) && _bs.test(hash2) && _bs.test(hash3)) {
				return true;
			}

			return false;
		}
	private:
		bitset<N* X> _bs;
};
