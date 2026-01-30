#pragma once
#include <string>
#include <vector>
#include <map>

namespace LoreNameEngine {
	using namespace std;

	using Phoneme = pair<string, double>;
	using Chain = map<string, vector<Phoneme>>;

	struct Model {
		virtual ~Model() = default;

		virtual const Chain& chain() const = 0;
		virtual const int min_len() const = 0;
		virtual const int max_len() const = 0	;
	};

	static const string START = "__START__";
	static const string END = "__END__";

	string generate(const Model&, const int, const int);
	string generate(const vector<const Model*>&, const int, const int);
	vector<string> generate(const Model&, const unsigned int, const int, const int);
	vector<string> generate(const vector<const Model*>&, const unsigned int, const int, const int);
}
