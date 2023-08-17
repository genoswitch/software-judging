#!/bin/bash
SRC_LATEST_COMMIT=$(git log -n 1 --pretty=format:"%H" source/$SRC_BRANCH)

ENV_VALUE="(merge/$DST_PATH): Merge changes from $SRC_BRANCH (genoswitch/$SRC_REPO@$SRC_LATEST_COMMIT)"

echo "ACTION_COMMIT_MSG="$ENV_VALUE"" >> $GITHUB_ENV