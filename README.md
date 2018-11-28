# TADA-SWIFT
[![Build Status](https://semaphoreci.com/api/v1/ahmad88me/tada-swift/branches/master/badge.svg)](https://semaphoreci.com/ahmad88me/tada-swift)
[![codecov](https://codecov.io/gh/ahmad88me/tada-swift/branch/master/graph/badge.svg)](https://codecov.io/gh/ahmad88me/tada-swift)

It performs semantic annotations for numerical values based on features computed from a knowledge graph. It is based on a previous python implementation. It only exposes command line interface as it aims for medium to large scale experiments.


# Feature files

format: `tsv` Tab Separated Values

| CLASS_URI | PROPERTY_URI | #NUMERIC_VALS | MEAN | MEDIAN | STD |
| :-------: | :----------: | :-----------: | :--: | :----: | :-: |

# Run
`make run ARGS="golfplayers.tsv aaagolfplayers.csv 10 11"`

