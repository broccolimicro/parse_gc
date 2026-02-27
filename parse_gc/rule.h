#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "expression.h"

namespace parse_gc {

struct rule : parse::syntax {
	rule();
	rule(tokenizer &tokens, void *data = NULL);
	~rule();

	expression assume;
	expression implicant;
	simple_composition action;
	bool weak;
	bool force;
	bool pass;
	bool keep;
	uint64_t after;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

