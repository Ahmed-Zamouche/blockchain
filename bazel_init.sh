#!/usr/bin/env bash
bazel version
generate_bash_completion.sh --bazel=/usr/local/bin/bazel --output=/etc/bash_completion.d/bazel-complete-list.bash