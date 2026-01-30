# Try to locate Erlang
ERLANG_PATH := $(shell erl -eval 'io:format("~s", [code:root_dir()])' -s init stop -noshell)
ERL_INCLUDE := -I$(ERLANG_PATH)/usr/include

.PHONY: help dev prod

help: ## Display this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	sort | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

dev: ## Compile dev build
	@mkdir -p _build/dev
	g++ -std=c++23 \
		-g3 \
		-Og \
		-Wall -Wextra \
		-Wno-unused-parameter \
		lib/engine.cpp -o _build/dev/lore_name_engine

run: dev ## Run dev build
	./_build/dev/lore_name_engine

test: ## Run tests
	@mkdir -p _build/test
	g++ -std=c++23 \
    		-O3 \
    		-march=native \
    		-flto \
    		-DNDEBUG \
    		-s \
    		tests/engine_test.cpp -o _build/test/lore_name_engine
	@./_build/test/lore_name_engine

release-elixir-nif: ## Compile NIF shared library
	@mkdir -p _build/nif
	g++ -std=c++23 \
		-O3 \
		-march=native \
		-fPIC \
		-shared \
		$(ERL_INCLUDE) \
		adapters/elixir_nif.cpp -o _build/nif/lore_name_engine_elixir_nif.so
#
#prod: ## Compile prod build
#	@mkdir -p _build/prod
#	g++ -std=c++23 \
#		-O3 \
#		-march=native \
#		-flto \
#		-DNDEBUG \
#		-s \
#		lib/engine.cpp -o _build/prod/lore_name_engine

clean: ## Delete _build folder
	rm -rf _build
