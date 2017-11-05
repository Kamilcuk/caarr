#!/bin/bash
set -euo pipefail

f=$(cat $1)

iterators=$(echo "$f" | sed 's/[\ ,.<>()]/\n/g' \
	| grep Iterator | sort -u)

cat <<EOF

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define TYPE int

TYPE *std__copy(TYPE*,TYPE*,TYPE*);
TYPE *std__lower_bound(TYPE*,TYPE*,const TYPE);
TYPE *std__upper_bound(TYPE*,TYPE*,const TYPE);
#define std__tie(a,b) a
TYPE * std__mismatch(TYPE*,TYPE*,TYPE*);
TYPE * std__find(TYPE *,TYPE *, TYPE);
size_t std__count(TYPE *, TYPE *, TYPE);
#define std__distance(a,b) b - a
#define std__advance(a,b) a+=b
typedef void (*Function)(TYPE);

struct TYPE_TYPE_pair_s {
	TYPE * first;
	TYPE * last;
};

EOF

f=$(echo "$f" | sed \
	-e 's/&amp;/\&/g' \
	-e 's/T&/TYPE/g' \
	-e 's/Size /size_t /g' \
	-e 's;typename;/* typename */;g' \
	-e 's/pair[[:space:]]*<\([a-zA-Z1-9_]*\)[[:space:]]*,[[:space:]]*\([a-zA-Z1-9_]*\)>/pair_\1_\2/' \
	-e 's/iterator_traits<\([0-9A-Za-z_]*\)>::difference_type/iterator_traits_\1___difference_type/g'
)

iterator_traits=$(echo "$f" |  sed 's/[\ ,.<>()]/\n/g' | grep iterator_traits | sort -u)
pair_iterators=$(echo "$f" | sed 's/[\ ,.<>()]/\n/g' | grep pair | grep Iterator | sort -u)
predicates=$(echo "$f" | sed 's/[\ ,.<>()]/\n/g' | grep -i predicate | sort -u)

echoforeach() {
	while read line; do eval "echo \"$1\""; done <<<"${!2}" | column -s'|' -t
}

echoforeach '#define $line|TYPE *' "iterators"
echo
echoforeach '#define $line|struct TYPE_TYPE_pair_s' "pair_iterators"
echo
echoforeach '#define $line|size_t' "iterator_traits"
echo
cat <<EOF
typedef bool (*UnaryPredicate)(TYPE);
EOF

echo "$f" | sed \
	-e 's;^template;// template;' \
	-e 's/std::make_pair[[:space:]]*(\([^)]*\))/((struct TYPE_TYPE_pair_s){\1})/g' \
	-e 's/std::/std__/g' \

echo

for i in iterators pair_iterators iterator_traits; do
	echoforeach '#undef  $line' "$i"
done
cat <<EOF

int main() {
return 0;
}
	
EOF
