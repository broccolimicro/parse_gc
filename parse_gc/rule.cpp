#include "rule.h"
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>

namespace parse_gc
{

rule::rule() {
	debug_name = "gc_rule";
	weak = false;
	force = false;
	pass = false;
	keep = false;
	after = std::numeric_limits<uint64_t>::max();
}

rule::rule(tokenizer &tokens, void *data)
{
	debug_name = "gc_rule";
	parse(tokens, data);
}

rule::~rule()
{

}

void rule::parse(tokenizer &tokens, void *data)
{
	weak = false;
	force = false;
	pass = false;
	keep = false;
	after = std::numeric_limits<uint64_t>::max();

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<parse::new_line>();

	tokens.increment(false);
	tokens.expect("[");

	tokens.increment(false);
	tokens.expect("{");

	tokens.increment(true);
	tokens.expect<simple_composition>();

	bool shortcut = true;
	tokens.push();
	while (shortcut) {
		string token = tokens.next();
		if (token == "\n") {
			break;
		} else if (token == "->") {
			shortcut = false;
		}
	}
	tokens.pop();

	if (not shortcut) {
		tokens.increment(true);
		tokens.expect("->");

		tokens.increment(true);
		tokens.expect<expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			implicant.parse(tokens, data);
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		action.parse(tokens, data);
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect("}");

		tokens.increment(true);
		tokens.expect<expression>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			assume.parse(tokens, data);
		}

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();

		tokens.increment(true);
		tokens.expect("]");

		bool first = true;
		do {
			if (first) {
				first = false;
			} else {
				tokens.next();
			}

			tokens.increment(false);
			tokens.expect(",");

			tokens.increment(true);
			tokens.expect("keep");
			tokens.expect("weak");
			tokens.expect("force");
			tokens.expect("pass");
			tokens.expect("after");

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				string value = tokens.next();
				if (value == "keep") {
					keep = true;
				}	else if (value == "weak") {
					weak = true;
				}	else if (value == "force") {
					force = true;
				} else if (value == "pass") {
					pass = true;
				} else if (value == "after") {
					tokens.increment(true);
					tokens.expect<parse::number>();
					tokens.increment(true);
					tokens.expect("=");
					if (tokens.decrement(__FILE__, __LINE__, data)) {
						tokens.next();
					}
					if (tokens.decrement(__FILE__, __LINE__, data)) {
						after = stoull(tokens.next());
					}
				}
			}
		} while (tokens.decrement(__FILE__, __LINE__, data));

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			tokens.next();
		}
	}

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
	}

	tokens.syntax_end(this);
}

bool rule::is_next(tokenizer &tokens, int i, void *data) {
	return expression::is_next(tokens, i, data) or simple_composition::is_next(tokens, i, data);
}

void rule::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<rule>()) {
		setup_expressions();
		tokens.register_syntax<rule>();
		tokens.register_token<parse::symbol>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>(true);
		expression::register_syntax(tokens);
		simple_composition::register_syntax(tokens);
	}
}

string rule::to_string(string tab) const {
	string result;
	if (implicant.valid) {
		result += implicant.to_string(tab) + "->";
	}
	if (action.valid) {
		result += action.to_string(tab);
	} else {
		result += "skip";
	}
	if (assume.valid) {
		result += " {" + assume.to_string(tab) + "}";
	}

	if (keep or weak or force or pass or after != std::numeric_limits<uint64_t>::max()) {
		bool comma=true;
		result += " [";
		if (keep) {
			result += "keep";
			comma=false;
		}
		if (weak) {
			if (not comma) {
				result += ",";
			}
			result += "weak";
			comma=false;
		}
		if (force) {
			if (not comma) {
				result += ",";
			}
			result += "force";
			comma=false;
		}
		if (pass) {
			if (not comma) {
				result += ",";
			}
			result += "pass";
			comma=false;
		}
		if (after != std::numeric_limits<uint64_t>::max()) {
			if (not comma) {
				result += ",";
			}
			result += "after=";
			result += ::to_string(after);
			comma=false;
		}

		result += "]";
	}

	return result;
}

parse::syntax *rule::clone() const {
	return new rule(*this);
}

}
