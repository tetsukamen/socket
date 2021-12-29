0x51
01010001
Version=01 type=01 TokenLength=0001

0x1
00000001
Code=00000001

0 0x1
00000000|00000001
messageId=00000000|00001111

0xf
00001111
token=00001111

0x11 0x1
00010001|00000001
option(delta=0001 length=0001 value=00000001)

0xff
11111111
marker


## 仕様
### ticket option
チケットを要求するとき　value=0x1
チケットを贈るとき value=xxxx
それ以外の時 value=0