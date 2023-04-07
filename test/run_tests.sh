#!/bin/bash
# Run unit tests


for t in bin/test/*; do
    ./$t || rc=$?
    if [[ rc -ne 0 ]] ; then
        exit $rc
    fi
done
