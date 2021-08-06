#!/bin/sh
PAYLOAD_NAME="payload.asm"

nasm -f bin -o payload payloads/$PAYLOAD_NAME