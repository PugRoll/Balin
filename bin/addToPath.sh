#!/bin/bash

if [[ -e ~/.zshrc ]]; then
    echo "#Add balin to path via zshrc file" >> ~/.zshrc ;
    echo "export PATH=$PWD:\$PATH" >> ~/.zshrc ;
fi
if [[ -e ~/.bashrc ]]; then
    echo "#Add balin to path via bashrc file" >> ~/.bashrc ;
    echo "export PATH=$PWD:\$PATH" >> ~/.bashrc ;
fi

