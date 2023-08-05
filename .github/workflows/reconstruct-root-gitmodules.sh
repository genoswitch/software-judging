#!/bin/bash

GITMODULES_FILENAME=.gitmodules

if [ -f $GITMODULES_FILENAME ]; then
    echo "Found existing $GITMODULES_FILENAME! Removing..."
    rm $GITMODULES_FILENAME
fi

for d in */* ; do
    [ -L "${d%/}" ] && continue

    echo "Searching directory '$d' for .gitmodules file..."

    if [ -f $d/$GITMODULES_FILENAME ]; then
        # Remove trailing slash
        d=${d%/}

        echo "Found $GITMODULES_FILENAME in '$d'."

        ESCAPED_FILENAME=$(echo $d | sed "s|/|_|")

        echo "Using escaped filenname '$ESCAPED_FILENAME'."

        cp $d/$GITMODULES_FILENAME /tmp/$ESCAPED_FILENAME.$GITMODULES_FILENAME

        sed -i "s|path\ = |path = $d/|" /tmp/$ESCAPED_FILENAME.$GITMODULES_FILENAME

        if [ ! -f $GITMODULES_FILENAME ]; then
            echo "$GITMODULES_FILENAME not found in root dir! Creating..."
            touch $GITMODULES_FILENAME
        fi 

        cat /tmp/$ESCAPED_FILENAME.$GITMODULES_FILENAME | tee -a $GITMODULES_FILENAME > /dev/null

        rm /tmp/$ESCAPED_FILENAME.$GITMODULES_FILENAME
        
    fi
done