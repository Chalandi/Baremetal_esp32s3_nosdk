#!/bin/bash

start=$(date +%s)

make BUILD_STAGE_1 && make BUILD_STAGE_2 -k -j $(getconf _NPROCESSORS_ONLN) && make BUILD_STAGE_3

end=$(date +%s)
execution_time=$((end - start))

echo "Total execution time: $execution_time seconds"
