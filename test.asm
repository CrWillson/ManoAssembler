     ORG 100
     LDA ADS
     STA PTR
     LDA NBR
     STA CTR
     CLA
LOP, ADD PTR I
     ISZ PTR
     ISZ CTR
     BUN LOP
     STA SUM
     HLT
ADS, HEX 150
PTR, HEX 0
NBR, DEC -10
CTR, HEX 0
SUM, HEX 0
     ORG 150
     DEC 25
     DEC 50
     DEC 75
     DEC 100
     DEC 25
     DEC 50
     DEC 75
     DEC 100
     DEC 25
     DEC 50
     END
