#!/bin/sh
##############################################################################
# queries sizes of your enlightenment cache                                  #
#
# Copyright (C) 1999 Carsten Haitzler, Geoff Harrison and various contributors
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies of the Software, its documentation and marketing & publicity
# materials, and acknowledgment shall be given in the documentation, materials
# and software packages that this Software was used.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##############################################################################

PATH=$PATH:$EBIN:$EROOT/scripts
export PATH

if [ $# -lt 1 ]; then
  echo "usage:"
  echo "    "$0" [ all | pager | bgsel | config ]"
  exit
fi

if [ $1 = "all" ]; then
  VAL=`du -s $ECACHEDIR/cached/pager $ECACHEDIR/cached/bgsel $ECACHEDIR/cached/cfg | awk '{printf("%s ", $1);}'`
  TOT=0
  for I in $VAL; do
    TOT=$[ $TOT + $I ];
  done
  eesh -e "dialog_ok Twoje cache u�ywaj� $TOT Kb przestrzeni dyskowej."
  exit
fi

if [ $1 = "pager" ]; then
  VAL=`du -s $ECACHEDIR/cached/pager | awk '{printf("%s", $1);}'`
  eesh -e "dialog_ok Tw�j cache Podgl�du Pulpitu u�ywa $VAL Kb przestrzeni dyskowej."
  exit
fi

if [ $1 = "config" ]; then
  VAL=`du -s $ECACHEDIR/cached/cfg | awk '{printf("%s", $1);}'`
  eesh -e "dialog_ok Tw�j cache plik�w konfiguracyjnych u�ywa $VAL Kb przestrzeni dyskowej."
  exit
fi

if [ $1 = "bgsel" ]; then
  VAL=`du -s $ECACHEDIR/cached/bgsel | awk '{printf("%s", $1);}'`
  eesh -e "dialog_ok Tw�j cache z T�ami Pulpitu uzywa $VAL Kb przestrzeni dyskowej."
  exit
fi
