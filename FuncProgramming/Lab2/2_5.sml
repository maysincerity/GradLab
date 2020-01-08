(* binarySearch: tree * int -> bool *)
(* REQUIRES: *)
(* ENSURES: *)
fun binarySearch (Empty : tree, x : int) : bool = false
  	| binarySearch (Node(L1, y, L2) : tree , x : int) = 
    	case Int.compare(y, x) of
        	GREATER => binarySearch (L1, x)
      		| EQUAL => true
      		| LESS => binarySearch(L2, x)


val test1 = binarySearch (Node(Node(Empty,1,Empty),2,Node(Empty,3,Empty)), 1)
val test2 = binarySearch (Node(Node(Empty,1,Empty),2,Node(Empty,3,Empty)), 4)
