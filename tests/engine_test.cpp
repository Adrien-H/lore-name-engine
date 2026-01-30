#undef NDEBUG
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "../lib/engine.cpp"

using namespace LoreNameEngine;
using namespace std;

void test_bounds() {
	Snake snake;
	cout << "[TEST] Check bounds… ";

	for (int i = 0; i < 1000; ++i) {
		string name = generate(snake);

		assert(!name.empty() && "Name must not be empty.");
		assert(name.length() >= (size_t) snake.min_len() && "Name too short.");
		assert (name.length() <= (size_t) snake.max_len() + 5 && "Name too long.");
	}

	cout << "✔️" << endl;
}

void test_batch() {
	Snake snake;

	cout << "[TEST] Check batch generation (1000 iterations) ";

	static const std::vector<const Model*> models = {&snake};

	std::vector<std::string> names = generate(models, 1000, -1, -1);

	cout << "✔️" << endl;
}

int main() {
	cout << "TESTING LORE NAME ENGINE" << endl;

	test_bounds();
	test_batch();

	cout << "✅ ALL TESTS ARE OK ✅" << endl;

	return 0;
}
