#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
    java --module-path /usr/local/openjfx/lib --add-modules=javafx.controls,javafx.fxml,javafx.base,javafx.media,javafx.web,javafx.swing -jar 'UnifiedHost-1.17.0.jar'    
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    [ ! -d "/usr/share/openjfx/lib" ] && sudo apt install openjfx
    java --module-path /usr/share/openjfx/lib --add-modules=javafx.controls,javafx.fxml,javafx.base,javafx.media,javafx.web,javafx.swing -jar 'UnifiedHost-1.17.0.jar'
fi
