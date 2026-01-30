#include "engine.hpp"
#include "models/snake.cpp"
#include <iostream>

namespace LoreNameEngine {
	string generate(const Model& model) {
		const Chain& chain = model.chain();

		if (chain.empty()) {
			return "Error: empty chain";
		}

		return chain.at(START).begin()->first;
	}
}

int main() {
	LoreNameEngine::Snake snake_model;

	std::cout << LoreNameEngine::generate(snake_model) << std::endl;

	return 0;
}
