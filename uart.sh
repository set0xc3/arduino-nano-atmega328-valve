#!/bin/sh

while true; do
    if [[ -e /dev/ttyUSB0 ]]; then
        cat /dev/ttyUSB0 || true
    else
        sleep 1
    fi
done
