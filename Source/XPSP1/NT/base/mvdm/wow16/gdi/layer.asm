	title LAYER.ASM - Parameter validation layer

.xlist

LAYER_INCLUDE=1

include layer.inc
include gpfix.inc
;include gditype.inc
ilObjType   = 2
;include gdiobj.inc

createSeg _DATA,DATA,WORD,PUBLIC,DATA,DGROUP

.list

_GPFIX SEGMENT WORD PUBLIC 'CODE'
_GPFIX ENDS

.lall

LAYER_START

include gdi.api

LAYER_END

;LAYER_EXPAND	INIT
LAYER_EXPAND	TEXT
;LAYER_EXPAND	PALETTE
;LAYER_EXPAND	ESC
;LAYER_EXPAND	DCSTUFF
;LAYER_EXPAND	LOGMISC
;LAYER_EXPAND	FONTLOAD
;LAYER_EXPAND	FONTRES
;LAYER_EXPAND	FONTINQ
;LAYER_EXPAND	FONTSIMS
;LAYER_EXPAND	METAREC
;LAYER_EXPAND	METAPLAY
;LAYER_EXPAND	ARCDDA
;LAYER_EXPAND	OUTMAN
;LAYER_EXPAND	POLYGON
;LAYER_EXPAND	RGOUT
;LAYER_EXPAND	MISC
;LAYER_EXPAND	PIXDDA
;LAYER_EXPAND	DIBITMAP
;LAYER_EXPAND	FLOODFILL
;LAYER_EXPAND	 RECT

end
