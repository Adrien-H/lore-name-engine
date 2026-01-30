#include "erl_nif.h"

#include "../lib/engine.hpp"
#include "../lib/models/snake.cpp"

#include <vector>
#include <string>
#include <cstring>

// Global data
static LoreNameEngine::Snake snake;
static const std::vector<const LoreNameEngine::Model*> models = {&snake};

// Helper
ERL_NIF_TERM string_to_binary(ErlNifEnv* env, const std::string& str) {
	ErlNifBinary bin;
	if (!enif_alloc_binary(str.size(), &bin)) {
		return enif_make_badarg(env);
	}
	memcpy(bin.data, str.data(), str.size());
	return enif_make_binary(env, &bin);
}

// Principal function
// Expected signature: generate(ModelAtom, Count, MinLen, MaxLen)
static ERL_NIF_TERM generate_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 4) {
		return enif_make_badarg(env);
	}

	char model_atom[32];
	int count, min_len, max_len;

	if (!enif_get_atom(env, argv[0], model_atom, sizeof(model_atom), ERL_NIF_LATIN1)
		|| !enif_get_int(env, argv[1], &count)
		|| !enif_get_int(env, argv[2], &min_len)
		|| !enif_get_int(env, argv[3], &max_len)) {
		return enif_make_badarg(env);
	}

	const std::vector<const LoreNameEngine::Model*>* selected_models = nullptr;

	if (strcmp(model_atom, "snake") == 0) {
		selected_models = &models; // Todo: Allow to pass multiple models
	}

	if (selected_models == nullptr) {
		return enif_make_tuple2(
			env,
			enif_make_atom(env, "error"),
			enif_make_string(env, "Unknown_model", ERL_NIF_LATIN1)
		);
	}

	std::vector<std::string> results = LoreNameEngine::generate(
		*selected_models,
		(unsigned int) count,
		min_len,
		max_len
	);

	std::vector<ERL_NIF_TERM> results_terms;
	results_terms.reserve(results.size());

	for (const auto& name : results) {
		results_terms.push_back(string_to_binary(env, name));
	}

	return enif_make_list_from_array(env, results_terms.data(), results_terms.size());
}

static ErlNifFunc nif_funcs[] = {
	{"generate", 4, generate_nif}
};

extern "C" {
	ErlNifEntry* nif_init(void) {
		static ErlNifEntry entry = {
			ERL_NIF_MAJOR_VERSION,
			ERL_NIF_MINOR_VERSION,
			"Elixir.LoreNameEngine.Native", // Adapt to real Elixir.ModuleName.Native
			sizeof(nif_funcs) / sizeof(ErlNifFunc),
			nif_funcs,
			NULL, NULL, NULL, NULL, // load, reload, upgrade, unload
			"beam.vanilla", // BEAM variant
			0, // Options
			sizeof(ErlNifEntry)
		};

		return &entry;
	}
}
