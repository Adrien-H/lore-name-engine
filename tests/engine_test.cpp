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
	cout << "[TEST] Check bounds (1000 iterations) ";

	for (int i = 0; i < 1000; ++i) {
		string name = generate(snake);

		assert(!name.empty() && "Name must not be empty.");
		assert(name.length() >= (size_t) snake.min_len() && "Name too short.");
		assert (name.length() <= (size_t) snake.max_len() + 5 && "Name too long.");
	}

	cout << "✔️" << endl;
}

int main() {
	cout << "TESTING LORE NAME ENGINE" << endl;

	test_bounds();

	cout << "✅ ALL TESTS ARE OK ✅" << endl;
}
