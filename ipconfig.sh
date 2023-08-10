#!/bin/sh

FILE="server/irc.hpp"
TO_FIND="define IP"
LINES=100
i=1

while [ $i -lt $LINES ]; do
	if [ $(cat $FILE | head -$i | grep "$TO_FIND" | wc -l) -eq 1 ]; then
		break
	fi
	i=$((i + 1))
done

if [ $i -eq $LINES ]; then
	return 1
fi

sed -i $i"s/.*/#define IP std::string(\"$(ip a | grep "inet 10" | awk '{print $2}' | rev | cut -c 4- | rev)\")/" $FILE

