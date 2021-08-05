#!/bin/sh
PAYLOAD_NAME="payload.asm"

nasm -f bin -o payload tests/$PAYLOAD_NAME