#!/bin/bash
export port_num=5002
(flask run -p ${port_num} &
echo "L")&& sudo kill $(lsof -t -i:${port_num})