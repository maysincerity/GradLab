(* evenP : int -> bool 		*)
(* REQUIRES: n >= 0 		*)
(* ENSURES: evenP n evaluates to true iff n is even. *)
fun evenP (0 : int) : bool = true
	| evenP 1 = false
	| evenP n = evenP (n - 2);

fun oddP (0 : int) : bool =false
	| oddP 1 = true
	| oddP n = oddP (n - 2);
