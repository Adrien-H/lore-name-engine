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
		engine.cpp -o _build/dev/lore_name_engine

run: dev ## Run dev build
	./_build/dev/lore_name_engine

prod: ## Compile prod build
	@mkdir -p _build/prod
	g++ -std=c++23 \
		-O3 \
		-march=native \
		-flto \
		-DNDEBUG \
		-s \
		engine.cpp -o _build/prod/lore_name_engine

clean: ## Delete _build folder
	rm -rf _build
