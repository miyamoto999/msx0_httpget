#!/bin/bash
pushd msxlib/src
make "$@"
popd

make -f Makefile.dos1 "$@"
cp -p httpget1.com ./webserver/webdata/

# pushd ./webserver/webdata/
# ish httpget1.com -s
# unix2dos httpget1.ish
# popd
