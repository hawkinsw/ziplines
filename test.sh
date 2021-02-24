#!/bin/env bash

tempfile=`tempfile`;

if [ $? -ne 0 ]; then
	echo "Could not create a temporary file.";
	exit 1;
fi

./ziplines , tests/file1 tests/file2 tests/file3 tests/file4 > $tempfile
diff $tempfile tests/expected > /dev/null 2>&1
test_result_status=$?

rm -f $tempfile

if [ $test_result_status -ne 0 ]; then
	echo "FAIL"
else
	echo "PASS"
fi
exit $test_result_status;
