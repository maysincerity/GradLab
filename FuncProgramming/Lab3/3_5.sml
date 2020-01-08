(* subsetSumOption: int list * int -> int list option *)
(* REQUIRES: *)
(* ENSURES: *)
fun subsetSumOption (l : int list, 0 : int) : int list option = SOME []
  	| subsetSumOption ([] : int list, s : int) : int list option = NONE
  	| subsetSumOption (x::L, s) =
    	case subsetSumOption (L, s-x) of
      		SOME l => SOME (x::l)
    		| NONE => subsetSumOption (L, s)

val test1 = subsetSumOption([1,2,3],0)
val test2 = subsetSumOption([],5)
val test3 = subsetSumOption([1,2,3],5)
val test4 = subsetSumOption([1,2,3],7)
