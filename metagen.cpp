/*
 * METAGEN
 * Made with beer in hand
 * Oleg Mingalev <oleg@mingalev.net>, 2018
 */

#include "testlib.h"
#include<string>
#include<unordered_map>
#include<iostream>
#include<memory>
#include <cstddef>

#include <string>
#include <unordered_map>
#include <cstddef>

namespace metagen {

class Cmd {
	public:
		virtual ~Cmd() {}
		virtual std::string operator() (char** &pos) = 0;
};

class Creator {
	public:
		Creator(const std::string& classname);
		virtual ~Creator() {}
		virtual Cmd* create() = 0;
};

template <class T>
class CreatorImpl : public Creator {
	public:
		CreatorImpl<T>(const std::string& classname) : Creator(classname) {}
		virtual ~CreatorImpl<T>() {}
		virtual Cmd* create() { return new T; }
};

class Factory {
	public:
		static Cmd* create(const std::string& classname);
		static void registerit(const std::string& classname, Creator* creator);
	private:
		static std::unordered_map<std::string, Creator*>& get_table();
};

#define REGISTER(classname) \
	private: \
	static const CreatorImpl<classname> creator;

#define REGISTERIMPL(classname,repr) \
	const CreatorImpl<classname> classname::creator = CreatorImpl<classname>(#repr);

Creator::Creator(const std::string& classname) {
	Factory::registerit(classname, this);
}

void Factory::registerit(const std::string& classname, Creator* creator) {
	get_table()[classname] = creator;
}

Cmd* Factory::create(const std::string& classname) {
	std::unordered_map<std::string, Creator*>::iterator i;
	i = get_table().find(classname);

	if(i != get_table().end())
		return i->second->create();
	else
		return (Cmd*)NULL;
}

std::unordered_map<std::string, Creator*>& Factory::get_table() {
	static std::unordered_map<std::string, Creator*> table;
	return table;
}

#define GLUE2(x,y) x##y
#define GLUE(x,y) GLUE2(x,y)

#define CMD2(name,repr) class name : public metagen::Cmd { \
	REGISTER(name); \
	public: \
		virtual ~name() {} \
		virtual string operator() (char** &pos);\
}; \
REGISTERIMPL(name,repr); \
string name::operator() (char** &pos)

#define CMD(name) CMD2(GLUE(mg_,name),name)

namespace utils {

std::string repeat(std::string str, const std::size_t n)
{
    if (n == 0) {
        str.clear();
        str.shrink_to_fit();
        return str;
    } else if (n == 1 || str.empty()) {
        return str;
    }
    const auto period = str.size();
    if (period == 1) {
        str.append(n - 1, str.front());
        return str;
    }
    str.reserve(period * n);
    std::size_t m {2};
    for (; m < n; m *= 2) str += str;
    str.append(str.c_str(), (n - (m / 2)) * period);
    return str;
}

inline int bitcnt(long long n) {
	int ret = 0;
	while (n) {
		ret += n & 1;
		n >>= 1;
	}
	return ret;
}

inline int mod(int n, int x)
{
     return ((n%x) + x) % x;
}

} // namespace metagen::utils

}; // namespace metagen

using namespace std;

string proceed(char** &cur, char stopword[]);

string next_token(char** &cur) {
	string ret(*cur++);
	bool only_op = true;
	for (int i = 0; i < ret.size(); ++i) {
		only_op &= ret[i] == '[';
	}
	if (!ret.size() || !only_op) {
		return ret;
	}
	char endword[] = "]";
	if (ret.size() == 1) {
		return proceed(cur, endword);
	}
	ret.resize(ret.size() - 1);
	return ret;
}

#define INTARG atoi(next_token(pos).c_str())
#define FLOATARG atof(next_token(pos).c_str())
#define STRARG next_token(pos)

string proceed(char** &cur, char stopword[] = nullptr) {
	string ret;
	while (*cur && (!stopword || strcmp(*cur, stopword))) {
		string token = next_token(cur);
		unique_ptr<metagen::Cmd> cmd(metagen::Factory::create(token));
		if (!cmd) {
			cerr << "Unknown command <" << token << ">" << endl;
			exit(1);
		}
		ret.append((*cmd)(cur));
	}
	if (stopword) {
		if (*cur) {
			++cur;
		} else {
			cerr << "Expected stopword <" << stopword << "> not found" << endl;
			exit(1);
		}
	}
	return ret;
}

namespace metagen::cmds::core {
	CMD(repeat) {
		int cnt = INTARG;
		char endword[] = "end";
		string ret;
		char** curpos = pos;
		for (int i = 0; i < cnt; ++i) {
			pos = curpos;
			ret.append(proceed(pos, endword));
		}
		return ret;
	}

	CMD(comment) {
		while (*pos) ++pos;
		return "";
	}
}

namespace metagen::cmds::testlib {
	CMD(next) {
		return rnd.next(STRARG);
	}

	CMD(inext) {
		int r = INTARG;
		return to_string(rnd.next(r));
	}
	
	CMD(inext2) {
		int l = INTARG;
		int r = INTARG;
		return to_string(rnd.next(l, r));
	}

	CMD(fnext) {
		float r = FLOATARG;
		return to_string(rnd.next(r));
	}
	
	CMD(wnext) {
		int l = INTARG;
		int t = INTARG;
		return to_string(rnd.wnext(l, t));
	}
}

namespace metagen::cmds::variables {
	unordered_map<string, string> vars;
	CMD(inc) {
		string name = STRARG;
		int n = atoi(vars[name].c_str());
		++n;
		vars[name] = to_string(n);
		return "";
	}

	CMD(var) {
		return vars[STRARG];
	}

	CMD(set) {
		string name = STRARG;
		string val = STRARG;
		vars[name] = val;
		return "";
	}
}

namespace metagen::cmds::strings {
	CMD(thuemorse) {
		int n = INTARG;
		string a = STRARG;
		string b = STRARG;
		string ret;
		for (int i = 0; i < n; ++i) {
			ret.append((metagen::utils::bitcnt(i) % 2) ? b : a);
		}
		return ret;
	}

	CMD(rotate) {
		int cnt = INTARG;
		string ret = STRARG;
		std::rotate(
			ret.begin(),
			ret.begin() + metagen::utils::mod(cnt, ret.size()),
			ret.end()
		);
		return ret;
	}

	CMD(shuffle) {
		string ret = STRARG;
		shuffle(ret.begin(), ret.end());
		return ret;
	}

	CMD(reverse) {
		string ret = STRARG;
		reverse(ret.begin(), ret.end());
		return ret;
	}

	CMD(print) {
		return STRARG;
	}

	CMD(lowercase) {
		static string ret;
		if (!ret.size()) {
			for (int i = 0; i < 26; ++i) {
				ret.push_back('a'+i);
			}
		}
		return ret;
	}

	CMD(uppercase) {
		static string ret;
		if (!ret.size()) {
			for (int i = 0; i < 26; ++i) {
				ret.push_back('A'+i);
			}
		}
		return ret;
	}

	CMD(digits) {
		static string ret;
		if (!ret.size()) {
			for (int i = 0; i < 10; ++i) {
				ret.push_back('0'+i);
			}
		}
		return ret;
	}

	CMD(cycle) {
		int cnt = INTARG;
		string s = STRARG;
		string ret;
		ret.reserve(cnt);
		for (int i = 0; i < cnt; ++i) {
			ret.push_back(s[i % s.size()]);
		}
		return ret;
	}

	CMD(times) {
		int cnt = INTARG;
		string s = STRARG;
		return metagen::utils::repeat(s, cnt);
	}

	CMD(shift) {
		int cnt = INTARG;
		string alphabet = STRARG;
		string s = STRARG;
		unordered_map<char, int> rev;
		for (int i = 0; i < alphabet.size(); ++i) {
			rev[alphabet[i]] = i;
		}
		std::rotate(
			alphabet.begin(),
			alphabet.begin() + metagen::utils::mod(cnt, alphabet.size()),
			alphabet.end()
		);
		for (int i = 0; i < s.size(); ++i) {
			unordered_map<char, int>::iterator it = rev.find(s[i]);
			if (it != rev.end()) {
				s[i] = alphabet[it->second];
			}
		}
		return s;
	}

	CMD(leftpad) {
		int cnt = INTARG;
		string ch = STRARG;
		if (ch.size() != 1) {
			cerr << "Invalid 2nd argument on leftpad: <" << ch << "> (must be char)" << endl;
			exit(1);
		}
		string s = STRARG;
		cnt -= s.size();
		string ret = metagen::utils::repeat(ch, max(0, cnt));
		ret.append(s);
		return ret;
	}

	CMD2(space,_) {
		return " ";
	}
	
	CMD(endl) {
		return "\n";
	}
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	char** cur = argv;
	++cur;
	cout << proceed(cur) << endl;
}
