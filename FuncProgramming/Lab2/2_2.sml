(* interleave: int list * int list -> int list *)
(* REQUIRES: *)
(* ENSURES: *)
fun interleave(A : int list, [] : int list) : int list = A
	|interleave([] : int list, B : int list) : int list = B
	|interleave(x::A, y::B) = x::y::interleave(A, B)


val test1 = [2,4] = interleave([2],[4])
val test2 = [2,4,3,5] = interleave([2,3],[4,5])
val test3 = [2,4,3,5,6,7,8,9] = interleave([2,3],[4,5,6,7,8,9]) 
val test4 = [2,3] = interleave([2,3],[ ]) 
