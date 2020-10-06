
const IR_AC = {
	encoding = "NEC",
	masks = {
		enable: 0x08,
		
	}

}

export default IR_AC


/*
0010 0101 0000 0000 0000 0000 1101 0000 1001

[0,1,2] Mode
001 - Cold
010 - Wet
011 - Fan
100 - Warm 
000 - STOP? also requires set temp to 001

[3] Enable AC
1 - enable
0 - disable

[4-5] Fan speed
00 - Auto
01 - 1 Speed
10 - 2 Speed
11 - 3 Speed

[6] Swing
1 - enable
0 - disable

[7] Sleep 
1 - enable
0 - disable

[8-10] Temperature
000 - 16
 -
110 - 30

[11]
1

[12-18 19?] Timer
0000 0000 - Off
0011 1000 - 3	h
0100 1000 - 4	h
0100 1001 - 4.5	h
0101 1000 - 5	h
0101 1001 - 5.5	h
0101 1000 - 6	h
0101 1001 - 6.5	h
0111 1000 - 7	h
0111 1001 - 7.5	h
 -
0000 1010 - 10		h
0000 1011 - 10.5	h
0001 1010 - 11		h
0000 1100 - 20		h
0000 1101 - 20.5	h
0001 1100 - 21		h
0001 1100 - 21.5	h

*/
