#!/bin/bash
# Regenera sumidero_wifi/pagina.h a partir de index.html
cd "$(dirname "$0")"
{ echo 'const char PAGINA[] PROGMEM = R"rawliteral('; cat index.html; echo ')rawliteral";'; } > sumidero_wifi/pagina.h
