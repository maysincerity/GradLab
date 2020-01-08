(* mapList': (‘a -> ‘b) -> (‘a list -> ‘b list) *)
(* REQUIRES: *)
(* ENSURES: *)
fun mapList' (f : 'a -> 'b) ([] : 'a list) : 'b list = []
	| mapList' (f : 'a -> 'b) (x::L : 'a list) : 'b list = (f x)::(mapList' f L)

val test1 = [2,4,6] = mapList' (fn x => x*2) [1,2,3]
