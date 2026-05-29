#!/bin/bash

# Script to run clang-tidy on changed C/C++ files
# Usage: ./run-clang-tidy.sh [build_type]

BUILD_TYPE="${1:-Debug}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
REPORT_FILE="${PROJECT_ROOT}/clang-tidy.txt"

cd "${PROJECT_ROOT}"  || exit

echo "=== Identifying Committed & Modified Files ==="

# 1. Clean up old report and run clang-tidy on the file list
rm -f "${REPORT_FILE}"
touch "${REPORT_FILE}"

# 2. Get the target tracking branch if inside a PR/Merge request, otherwise default to HEAD~1
TARGET_REF="HEAD~1"
if [ -n "${CHANGE_TARGET}" ]; then
    echo "Pull Request detected. Fetching target branch: ${CHANGE_TARGET}"
    git fetch --no-tags --depth=1 origin "${CHANGE_TARGET}"
    TARGET_REF="origin/${CHANGE_TARGET}"
fi

# 3. Extract changed file paths between the target ref and current HEAD
# Filter specifically for your requested C/C++ extensions located under 'src/'
CHANGED_FILES=$(git diff --name-only "${TARGET_REF}" HEAD || true)

if [ -z "${CHANGED_FILES}" ]; then
    echo "No C/C++ source or header files were modified in this commit. Skipping analysis."
    exit 0
fi

echo "Changed Files of the revision :"
echo "${CHANGED_FILES}"
echo "----------------------------------------"

for FILE in ${CHANGED_FILES}; do
    # Double check that the file actually exists on disk (handles deleted files gracefully)
    if [ -f "${FILE}" ]; then
        case "${FILE}" in
        src/*.cc|src/*.cpp|src/*.cxx|src/*.c++|src/*.hh|src/*.hpp|src/*.hxx|src/*.h++|src/*.ipp|src/*.c|src/*.h)
            echo "Clang-Tidy Analyzing file: ${FILE}"

            clang-tidy-18 -p="${PROJECT_ROOT}/build/${BUILD_TYPE}" --config-file=Configs/.clang-tidy "${FILE}" \
                -- -Wno-unknown-argument -Wno-unknown-warning-option >> "${REPORT_FILE}" 2>&1 || true
            ;;
        *)
            # Ignore files that don't match our path or extensions
            ;;
        esac
    fi
done
