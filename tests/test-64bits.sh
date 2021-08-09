_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n${_YELLOW}$0 :${_END}\n"


# Test with two simples binary
gcc -no-pie ./tests/test1.c -o test1_no_pie
echo "\n${_GREEN}Creating and Executing woody with no_pie binary...${_END}\n"
./woody_woodpacker test1_no_pie
./woody
rm woody

gcc ./tests/test1.c -o test1
echo "\n${_GREEN}Creating and Executing woody with normal binary...${_END}\n"
./woody_woodpacker test1
./woody
rm woody

# Test with bin/ls binary
echo "\n${_GREEN}Creating and Executing woody with /bin/ls binary...${_END}\n"
./woody_woodpacker /bin/ls
./woody
rm woody

# Test with bin/echo binary
echo "\n${_GREEN}Creating and Executing woody with /bin/echo binary...${_END}\n"
./woody_woodpacker /bin/echo
./woody "This is echo binaryyy"

rm woody
rm test1
rm test1_no_pie