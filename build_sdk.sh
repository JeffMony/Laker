
#!/bin/bash

CUR_DIR=$(pwd)

if [ ! -d ${CUR_DIR}/packages/$1 ]; then
  mkdir ${CUR_DIR}/packages/$1
fi

./gradlew :videorender:deploy -Dbackdoor=true

cp -rf ${CUR_DIR}/videorender/build/outputs/. ${CUR_DIR}/packages/$1/outputs
cp -rf ${CUR_DIR}/videorender/build/intermediates/cmake/. ${CUR_DIR}/packages/$1/cmake
cp -rf ${CUR_DIR}/videorender/build/intermediates/stripped_native_libs/release/out/lib/. ${CUR_DIR}/packages/$1/libs