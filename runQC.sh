#!/bin/sh

export PATH=/bin:/usr/bin:/usr/local/bin:${PATH}

fileRunList="ZDCQAlist.txt"
while IFS=fileRunList read line; do
        echo "submitting macro"
        /Users/chiara/alice/sw/osx_arm64/ROOT/v6-30-01-alice5-local3/bin/root -b -q qcAnalysis.C\+\($line\)
done <"$fileRunList"