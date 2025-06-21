#!/bin/bash

find src -name '*.cpp' | entr ../build.sh --target=web