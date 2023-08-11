#!/bin/sh

FILE="server/irc.hpp"
TO_FIND="define IP"
NEW_LINE="#define IP std::string(\"$(ip a | grep 'inet 10' | awk '{print $2}' | rev | cut -c 4- | rev)\")"
LINES=$(cat $FILE | wc -l)
IS_THERE=$(cat $FILE | grep "$TO_FIND" | wc -l)

i=1
while read line; do
	i=$((i + 1))
	if [ $IS_THERE -gt 0 ] && [ $(echo $line | grep "$TO_FIND" | wc -l) -gt 0 ]; then
		if [ "$line" = "$NEW_LINE" ]; then
			return 0
		fi
		break
	elif [ $IS_THERE -eq 0 ] && [ "$line" = "" ]; then
		break
	fi
done < "$FILE"

if [ $i -eq $LINES ]; then
	return 0
fi

sed -i $i"s/.*/$NEW_LINE/" $FILE

