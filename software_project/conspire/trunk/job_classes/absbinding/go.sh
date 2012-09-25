#!/bin/bash

echo "instance $1 of $2"
echo "Printing so much woo!"

sleep 20

echo "After sleep $1 of $2" >> ./results_$1.txt
