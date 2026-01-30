#include "engine.hpp"
#include "models/snake.cpp"
#include <random>
#include <iostream>

namespace LoreNameEngine {

	string generate(
		const Model& model,
		const int override_min_len = -1,
		const int override_max_len = -1
	) {
		int min_len = (override_min_len != -1) ? override_min_len : model.min_len();
		int max_len = (override_max_len != -1) ? override_max_len : model.max_len();

		const Chain& chain = model.chain();

		string current = START;
		string result = "";

		static random_device rd;
		static mt19937 rng(rd());

		while (true) {
			auto item = chain.find(current);
			if (item == chain.end() || item->second.empty()) {
				break;
			}

			const auto& candidates = item->second;

			// DYNAMIC WEIGHTING
			// If we approach max length, we artificially increase the weight
			// of the __END__ option
			vector<Phoneme> weighted_candidates;
			int total_weight = 0;

			for (const auto& candidate : candidates) {
				int weight = candidate.second;

				if (candidate.first == END) {
					int current_len = static_cast<int>(result.length());

					if (current_len < min_len) {
						weight = 0;
					} else if (current_len >= max_len - 3) {
						int urgency = (current_len - (max_len - 3)) + 1;
						weight *= (20 * urgency);
					}
				}

				if (weight > 0) {
					weighted_candidates.push_back({candidate.first, weight});
					total_weight += weight;
				}
			}

			// Fallback: in dead end, we enforce end
			if (total_weight == 0) {
				break;
			}

			uniform_int_distribution<int> dist(0, total_weight - 1);
			int target = dist(rng);
			int current_sum = 0;

			// Default to __END__ for security
			string next = END;
			for (const auto& candidate : weighted_candidates) {
				current_sum += candidate.second;
				if (target < current_sum) {
					next = candidate.first;
					break;
				}
			}

			if (next == END) {
				break;
			}

			// Security hard limit
			int potential_len = static_cast<int>(result.length()) + next.length();
			if (potential_len > max_len + 5) {
				break;
			}

			result += next;
			current = next;
		}

		if (!result.empty()) {
			result[0] = toupper(result[0]);
		}

		return result;
	}

	string generate(
		const vector<const Model*>& models,
		const int override_min_len = -1,
		const int override_max_len = -1
	) {
		if (models.empty()) {
			return "Error: no models provided";
		}

		static random_device rd;
		static mt19937 rng(rd());

		uniform_int_distribution<int> dist(0, models.size() - 1);
		size_t index = dist(rng);

		return generate(*models[index], override_min_len, override_max_len);
	}

	vector<string> generate(
		const Model& model,
		const unsigned int iterations,
		const int override_min_len = -1,
		const int override_max_len = -1
	) {
		vector<string> results;

		for (unsigned int i = 0; i < iterations; ++i) {
			results.push_back(generate(model, override_min_len, override_max_len));
		}

		return results;
	}

	vector<string> generate(
		const vector<const Model*>& models,
		const unsigned int iterations,
		const int override_min_len = -1,
		const int override_max_len = -1
	) {
		vector<string> results;

		for (unsigned int i = 0; i < iterations; ++i) {
			results.push_back(generate(models, override_min_len, override_max_len));
		}

		return results;
	}
}

int main() {
	LoreNameEngine::Snake snake;

	static const std::vector<const LoreNameEngine::Model*> models = {&snake};

	std::vector<std::string> names = LoreNameEngine::generate(models, 100, -1, -1);

	for (const auto& name : names) {
		std::cout << name << std::endl;
	}

	return 0;
}
