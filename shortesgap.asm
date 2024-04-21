;-----------------------------------------------------------------------		
;
; ShortesGap
; (c) 2023 GienekP
;
;-----------------------------------------------------------------------

TMP     = $00

;-----------------------------------------------------------------------

DMACTLS = $022F
IRQEN   = $D20E
IRQST   = $D20E
PORTB   = $D301
DMACTL  = $D400
NMIEN   = $D40E

;-----------------------------------------------------------------------		
		ORG $0600
;-----------------------------------------------------------------------		
INIT	lda TMP
		pha
		lda TMP+1
		pha
;-----------------------------------------------------------------------		
; IRQ DISABLE
IRQDIS	sei	
		lda #$00
		sta DMACTL
		sta NMIEN
		sta IRQEN
		sta IRQST
;-----------------------------------------------------------------------		
; COPY ROM TO RAM
ROM2RAM	lda PORTB
		pha
		lda #$C0
		sta TMP+1
		ldy #$00
		sty TMP
		ldx #$FF			
CPOS	stx PORTB
		lda (TMP),Y
		dex
		stx PORTB
		sta (TMP),Y
		inx
		iny
		bne CPOS
		inc TMP+1
		lda TMP+1
		cmp #$D0
		bne OSOK
		lda #$D8
		sta TMP+1
OSOK	cmp #$00
		bne CPOS
		pla
		and #$FE
		sta PORTB
;-----------------------------------------------------------------------		
; IRQ ENABLE
IRQENB	lda #$40
		sta NMIEN
		lda #$F7
		sta IRQST
		lda DMACTLS
		sta DMACTL
		cli			
;-----------------------------------------------------------------------		
; RESTORE TMP
		pla
		sta TMP+1
		pla
		sta TMP
;-----------------------------------------------------------------------		
; UPGRADE
UPGRADE	ldx #$04	; $04
		stx $BFD4	; Dolarnik
		dex			; $03
		dex			; $02
		dex			; $01
		stx $EE17 	; Short Gap for NTSC
		stx $EE18 	; Short Gap for PAL		
		dex			; $00
		stx $BC42	; No cursor
		rts		
;-----------------------------------------------------------------------		
		nop
		nop
;-----------------------------------------------------------------------		
		INI INIT
;-----------------------------------------------------------------------		
