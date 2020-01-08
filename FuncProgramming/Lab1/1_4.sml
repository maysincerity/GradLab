(* mult’ : int list * int -> int 			*)
(* REQUIRES: true				*)
(* ENSURES: mult’(L, a) evaluates to a multiply the product of the integers in L 	*)

 fun mult’ ([ ], a) = a
	  | mult’ (x :: L, a) = mult’ (L, x * a);


fun Mult’ ( [ ], a) = 	a
    | Mult’ (r::R, a) = 	Mult’ (R, (mult’ (r, a)));
