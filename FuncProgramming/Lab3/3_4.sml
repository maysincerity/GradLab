(* findOdd: int list -> int option *)
(* REQUIRES: *)
(* ENSURES: *)
fun findOdd ([] : int list) : int option = NONE
  	| findOdd (x::L : int list) : int option = 
    	if x mod 2 = 1
      		then SOME x
      	else findOdd L

val test1 = findOdd []
val test2 = findOdd [2,4,6]
val test3 = findOdd [2,4,7,9]
