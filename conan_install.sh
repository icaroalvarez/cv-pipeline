#!/bin/bash

base_dir=$(pwd)
CONAN_FILE=conanfile.txt
modules=$(find -path './modules/*' -prune -type d)
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

install () {
  conan_file_path=$1"/"$CONAN_FILE
  echo "installing conan dependencies defined in:" $conan_file_path"..."

  if test -f $conan_file_path; then
    cd $1
    if test -d ".conan"; then
      echo "${ORANGE}.conan folder already existing in: "$1 \
            ", remove the .conan folder first if you want to reinstall the dependencies"${NC}
    else
      mkdir .conan && cd .conan
      conan install ..
      echo ${GREEN}"conan dependencies sucessfully installed in: "$1${NC}
    fi
    cd $2
  else
    echo "${ORANGE}Conan file not found: "$conan_file_path${NC}
  fi
}

install $base_dir $base_dir
for path in $modules
  do
    install $path $base_dir
  done

