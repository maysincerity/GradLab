(* revT: tree -> tree *)
(* REQUIRES: *)
(* ENSURES: *)
fun revT (Empty : tree) : tree = Empty
  	| revT (Node(L1, x, L2) : tree) : tree = Node(revT L2, x, revT L1)

val test = Node(Node(Empty, 1, Empty), 2, Node(Empty, 3, Empty)) = revT((Node(Node(Empty, 3, Empty), 2, Node(Empty, 1, Empty))))