#!/bin/sh

gulp

ssh debian rm -rf public_html/ir_remote_fe

ssh debian mkdir -p public_html/ir_remote_fe
scp dest/index.html debian:~/public_html/ir_remote_fe/
