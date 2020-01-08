(* split: int list -> int list * int * int list *)
(* REQUIRES: *)
(* ENSURES: *)
fun split([] : int list) : (int list * int * int list) = raise Fail "Empty list"
    | split (L : int list) =
      	let
        	val midLength = (length L) div 2
        	val L1 = (List.take(L,midLength))
        	val x :: L2 = (List.drop(L,midLength))  
      	in
        	(L1, x, L2)
      	end

datatype tree = Empty
  	| Node of (tree * int * tree)

(* listToTree: int list -> tree *)
(* REQUIRES: *)
(* ENSURES: convert a list to a tree *)
fun listToTree ([] : int list) : tree = Empty
  	| listToTree (L : int list) : tree = 
    	let val (L1, x, L2) = split L
    	in Node(listToTree L1, x, listToTree L2)
   		end

val test1 = Node (Empty,1,Empty) = listToTree [1]
val test2 = Node(Node(Node(Empty,5,Empty),1,Empty),6,Node(Node(Empty,2,Empty),3,Empty)) = listToTree [5,1,6,2,3]
