#!/bin/sh
set -e
curl https://www.dropbox.com/s/6bbtntpywxm0wz0/example_binaries.tar.gz?dl=1 -o examples.tar.gz -J -L -k
gzip -d examples.tar.gz
tar -xvf examples.tar
rm examples.tar
