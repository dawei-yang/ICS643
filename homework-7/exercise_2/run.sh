#!/bin/bash

for v in {4..60..4}
do
    python3 pyspark-v2.py pyspark-input-non-identical.txt $v  >> results2.txt
done

