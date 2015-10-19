#!/bin/bash

htlatex mammult_doc.tex "html,index=2,3,4,5,next" "" -d../html/
rm *.html

pdflatex mammult_doc.tex
