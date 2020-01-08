(* divisibleByThree : int -> bool 	*)
(* REQUIRES: true				*)
(* ENSURES: divisibleByThree n evaluates to true if n is a multiple of 3 and to false otherwise *)

fun divisibleByThree (0 : int) : bool = true
	| divisibleByThree (1 : int) : bool = false
	| divisibleByThree (2 : int) : bool = false
    | divisibleByThree n = divisibleByThree (n - 3);