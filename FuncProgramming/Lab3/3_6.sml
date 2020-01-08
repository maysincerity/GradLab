(* exists : ('a -> bool) -> 'a list -> bool *)
(* REQUIRES: *)
(* ENSURES: *)
fun exists (p : 'a -> bool) ([] : 'a list) : bool = false
	| exists (p : 'a -> bool) (x::l : 'a list) : bool = p(x) orelse exists p l


(* forall : ('a -> bool) -> 'a list -> bool *)
(* REQUIRES: *)
(* ENSURES: *)
fun forall (p : 'a -> bool) ([] : 'a list) : bool = true
	| forall (p : 'a -> bool) (x::l : 'a list) : bool = p(x) andalso forall p l


val test1 = exists (fn x => x = 1) [2,3]
val test2 = exists (fn x => x = 1) [1,2,3]
val test3 = forall (fn x => x = 1) [1,1]
val test4 = forall (fn x => x = 1) [1,1,2]
