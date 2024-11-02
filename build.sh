#!/bin/bash

pushd msxlib/src
make "$@"
popd

make "$@"
cp -p httpget.com ./webserver/webdata/

#pushd ./webserver/webdata/
#ish httpget.com -s &> /dev/null
#unix2dos httpget.ish &> /dev/null
#popd
