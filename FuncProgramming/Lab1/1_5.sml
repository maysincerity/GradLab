(* double : int -> int *)
(* REQUIRES: n >= 0 *)
(* ENSURES: double n evaluates to 2 * n.*)

fun double (0 : int) : int = 0
    | double n = 2 + double (n - 1);


fun square (0 : int) : int = 0
	| square n = double n - 1 +square(n-1);
