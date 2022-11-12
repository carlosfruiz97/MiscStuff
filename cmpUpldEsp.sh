#!/bin/bash
# chmod u+x cmpUpldEsp.sh
# cp cmpUpldEsp.sh ~/local/bin/

port=/dev/ttyUSB0
while getopts p:f: option
do
    case "${option}"
        in
        p)port=${OPTARG};;
        f)fileName=${OPTARG};;
    esac
done

echo "port : $port"
echo "FileName : $fileName"

arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 $fileName && arduino-cli upload -p $port --fqbn esp8266:esp8266:nodemcuv2 $fileName && arduino-cli monitor -p $port
