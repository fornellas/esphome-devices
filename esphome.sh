#!/bin/sh
cd "$(dirname "$0")"
exec docker run --rm --privileged --net=host -v $PWD:/config -it ghcr.io/esphome/esphome:2026.3.2 -- "$@"
