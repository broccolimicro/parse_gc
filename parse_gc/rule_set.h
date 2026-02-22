#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "rule.h"

namespace parse_gc
{
struct rule_set : parse::syntax
{
	rule_set();
	rule_set(tokenizer &tokens, void *data = NULL);
	~rule_set();

	vector<rule_set> regions;
	vector<rule> rules;
	string region;

	vector<string> assume;
	vector<string> require;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};
}

