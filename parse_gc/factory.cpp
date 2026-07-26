#include "factory.h"

#include "rule_set.h"

namespace parse_gc {

const parse::factory factory(parse::schema::from<rule_set>());

}

