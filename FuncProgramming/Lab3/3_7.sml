datatype 'a tree = Empty
  	| Node of ('a tree * 'a * 'a tree)

(* treeFilter : (‘a -> bool) -> ‘a tree -> ‘a option tree *)
(* REQUIRES: *)
(* ENSURES: *)
fun treeFilter (p : 'a -> bool) (Empty : 'a tree) : 'a option tree = Empty 
  	| treeFilter (p : 'a -> bool) (Node(Lc, x, Rc) : 'a tree) : 'a option  tree = 
    	if p x
    		then Node(treeFilter p Lc, SOME x, treeFilter p Rc)
      	else Node(treeFilter p Lc, NONE, treeFilter p Rc)

val test = treeFilter (fn x => x = 1) (Node(Empty, 1, Node(Empty, 2, Empty)))
