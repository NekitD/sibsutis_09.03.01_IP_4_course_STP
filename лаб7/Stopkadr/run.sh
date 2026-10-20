#!/usr/bin/env bash
set -e
export EQDB_NAME=postgres          
export EQDB_USER=RuslandUberAlles     
export EQDB_PASS=stopkadr          
export EQDB_HOST=localhost
cd "$(dirname "$0")"
./build/stopkadr