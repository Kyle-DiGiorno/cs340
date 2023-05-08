#!/bin/bash
export port_num=5015
(flask run -p ${port_num} --host=0.0.0.0 &
echo "L")&& sudo kill $(lsof -t -i:${port_num})
