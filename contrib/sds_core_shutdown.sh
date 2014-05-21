#!/bin/bash

if [[ "$1" = "force" ]]; then
  pkill -9 xmssed
else
  pkill xmssed
fi

