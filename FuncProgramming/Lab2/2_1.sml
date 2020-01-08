(* reverse: int list -> int list *)
(* REQUIRES:  *)
(* ENSURES: reverse L (all elements) *)
fun reverse ([] : int list) : int list = []
	| reverse (x::L : int list) : int list = reverse(L) @ [x]


fun helpReverse ([] : int list, L1 : int list) : int list = L1
  	| helpReverse(x::L : int list, L1 : int list) : int list = helpReverse(L, x::L1)


(* reverse' : int list -> int list *)
(* REQUIRES:  *)
(* ENSURES: reverse L (all elements) in O(n) *)
fun reverse' ([] : int list) : int list = []
  	| reverse' (L : int list) : int list = helpReverse(L, [])


val test1 = [1,2,3,4] = reverse [4,3,2,1]
val test2 = [1,2,3,4] = reverse' [4,3,2,1]
