(* thenAddOne: ((int ->int) * int) -> int *)
(* REQUIRES: *)
(* ENSURES: *)
fun thenAddOne ((f:int -> int), (x:int)) : int = f(x) + 1

val test1 = 15 = thenAddOne(fn x => x*2, 7)
