#!/bin/bash

mkdir -p build
cd build
cmake ../cpp/
make
cp alien_dog ../
cd ..
rm build -rf
