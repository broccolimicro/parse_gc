#include "factory.h"

#include "rule_set.h"

namespace parse_gc {

parse::syntax *produce(tokenizer &tokens, void *data) {
	return new rule_set(tokens, data);
}

void expect(tokenizer &tokens) {
	tokens.expect<rule_set>();
}

void register_syntax(tokenizer &tokens) {
	rule_set::register_syntax(tokens);
}

}

