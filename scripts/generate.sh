#!/bin/bash
set -euo pipefail

cd $(dirname $(readlink -f $0))

n=$1
src=../src
VPARRc=$src/VPARR.c
VPARRh=$src/VPARR.h
namespace=AARR

filter() { 
	sed \
	-e 's/ VPARR_/ TYPE ## _/' \
        -e 's/(VPARR[ ]\?\*[ ]\?arr,[ ]\?/(TYPE *arr, size_t size, /g' \
        -e 's/void[ ]\?\*/TYPE \*/g' \
	-e 's/^{/{\n\tassert(arr);\n\tif ( size == 0 ) return NULL;/' \
        -e 's/VPARR_ITERATE(i, base, arr)/for(size_t i = size; i--;)/' \
        -e '0,/base/s/base/arr++/' \
        -e 's/base/arr/g' \
	-e 's/if ( arr->nmemb == 1 ) {/if ( size == 1 ) {/' \
	| sed \
        -e 's/$/ \\/' \
        -e '$s/ \\//'
}

echo "#define ${namespace}_FORWARD_DECLARE_TYPE_${n}(TYPE) \\"
cat ${VPARRh} | grep VPARR_$n -A100 | grep -B100 -m1 ";" | filter
echo
echo "#define AARR_DECLARE_TYPE_$n(TYPE) \\"
cat ${VPARRc} | grep VPARR_$n -A100 | grep -B100 -m1 "^}" | filter



