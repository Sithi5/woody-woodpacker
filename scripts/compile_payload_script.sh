#!/bin/sh
PAYLOAD_NAME="test.asm"

nasm -f bin -o payload payloads/$PAYLOAD_NAME

echo "Payload created..."