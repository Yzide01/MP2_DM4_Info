type formule =
	| Var of string
	| Top
	| Bot
	| And of formule * formule
	| Or of formule * formule
	| Not of formule
type valuation=(string *bool)list 
type sat_result =valuation option 

let implique (f1, f2) = Or(Not f1, f2)
let equivalence (f1, f2) = And(implique (f1, f2), implique (f2, f1))

(*** PARSER ***)


exception Erreur_syntaxe
exception Fichier_invalide

(* Symboles:
	'T' -> true
	'F' -> false
	'&' -> And
	'|' -> Or
	'~' -> Not
	'>' -> implication
	'=' -> equivalence
 *)

(* Détermine si c correspond à un opérateur binaire logique *)
let is_binop (c: char) : bool = match c with 
	| '&' |  '|' |  '>' |  '='  -> true
	| _ -> false 

(* Priorité de l'opérateur c. Permet de déterminer
	comment interpréter une formule sans parenthèses.
	Par exemple, "x&y|z" sera interprété comme "(x&y)|z"
	car & est plus prioritaire que | *)
let priority (c: char) : int = match c with
	| '&' -> 4
	| '|' -> 3
	| '=' -> 2
	| '>' -> 1
	| _ -> raise Erreur_syntaxe (* c n'est pas un opérateur *)

(* indice de l'opérateur le moins prioritaire parmis ceux
   qui ne sont pas entre parenthèses entre s.[i] et s.[j] 
   inclus *)
 let find_op_surface (s: string) (i: int) (j: int) : int =
 	(* 
 	   Renvoie l'indice de l'opérateur le moins prioritaire entre
 	   i et j, sachant que res est l'indice du meilleur opérateur
 	   entre i et k-1.
 	   paren_lvl: niveau d'imbrication actuel des parenthèses *)
 	let rec find_op_paren (k:int) (res:int) (paren_lvl: int) : int  =
 		if k=j+1 then res else
 		if s.[k] = '(' then find_op_paren (k+1) res (paren_lvl+1)
 		else if s.[k] = ')' then find_op_paren (k+1) res (paren_lvl-1) 

 		(* Le caractère lu est pris si l'on est hors des parenthèses,
 		   que le caractère est bien un opérateur, et qu'il est moins
 		   prioritaire que le meilleur résultat jusqu'ici *)
 		else if paren_lvl = 0 
 			 && is_binop s.[k] 
 			 && (res = -1 || priority s.[k] < priority s.[res]) 
 			 then find_op_paren (k+1) k (paren_lvl)
 		else find_op_paren (k+1) res (paren_lvl)
 	in find_op_paren i (-1) 0;;

(* Renvoie une formule construite à partir de la chaîne s.
   Lève une exception Erreur_syntaxe si la chaîne ne représente pas une formule valide. *)
let parse (s: string) : formule =
	let n = String.length s in
	(* construit une formule à partir de s[i..j] *)
	let rec parse_aux (i: int) (j:int) =
		assert (0 <= i && i < n && 0 <= j && j < n && i <= j );
		if s.[i] = ' ' then parse_aux (i+1) j
		else if s.[j] = ' ' then parse_aux i (j-1)
		else let k = find_op_surface s i j in 
		if k = -1 then
			if s.[i] = '~' then 
				Not (parse_aux (i+1) j)
			else if s.[i] = '(' then
				begin 
					if (s.[j] != ')') then (print_int j; failwith "mauvais parenthésage") else
					parse_aux (i+1) (j-1)
				end
			else if (i = j && s.[i] = 'T') then Top
			else if (i = j && s.[i] = 'F') then Bot
			else Var(String.sub s i (j-i+1))

		else match s.[k] with
			| '&' -> And(parse_aux i (k-1), parse_aux (k+1) j)
			| '|' -> Or(parse_aux i (k-1), parse_aux (k+1) j)
			| '=' -> equivalence(parse_aux i (k-1), parse_aux (k+1) j)
			| '>' -> implique(parse_aux i (k-1), parse_aux (k+1) j)
			| _ -> raise Erreur_syntaxe
	in parse_aux 0 (String.length s -1)

(* Renvoie une formule construire à partir du contenu du fichier fn.
   Lève une exception Erreur_syntaxe si le contenu du fichier n'est pas une formule valide.
   Lève une exception Sys_error(message_erreur) si le nom du fichier n'est pas valide. *)
let from_file (filename: string) : formule = 
	(* concatène toutes les lignes de f en une seule chaîne *)
	let rec read_lines f = 
		try 
			let next_line = input_line f in
			let s = read_lines f in
			next_line ^ s
		with 
			| End_of_file -> ""
	in
	let f = open_in filename in 
	let s = read_lines f in
	parse s

(*Entrée: formule valide 
  Sortie  :entier représentant le nombre de or , and et not *)
let compte_ops(f:formule):int=
   let rec is_op(f:formule):int=
     match f with
     |Var x->0 
	  |Top->0
	  |Bot->0
	  |And (x,y) ->1+is_op x +is_op y
	  |Or  (x,y)->1+is_op x +is_op y
	  |Not x->1 +is_op x
	  
      in is_op f

let rec liste_stric_croit(liste :'a list ):bool=
   match liste with 
   |[]->true
   |x::[]->true
   |x::y::q->if x<y then liste_stric_croit(y::q) else false




let rec union (l1:'a list)(l2 :'a list):'a list=
  if liste_stric_croit(l1) && liste_stric_croit(l2) then 
   match l1,l2 with
	 |[],[]->[]
	 |[],_->l2
	 |_,[]->l1
   |x::q,y::q2->if x<y then x::union q l2 else  if x>y then y::union l1 q2 else x::union q q2
	else failwith "Il y a une liste qui n'est pas strictement croissante"
let read_file (fn:string):string=
  let ic=open_in fn in
  let lecture= try input_line ic  with 
  |End_of_file ->failwith "Fichier vide"
in close_in ic;lecture


(*Renvoie la liste des variables contenues dans la formule*)
let rec calcul_var_formule(f:formule):'a list=
  match f with 
	|Var x->x::[]
	|Top->[]
	|Bot->[]
	|And (x,y) ->union (calcul_var_formule x) (calcul_var_formule y) 
	|Or  (x,y)->union (calcul_var_formule x) (calcul_var_formule y)
	|Not x->calcul_var_formule x

let rec calcul_valuation(environnement : valuation )(f:formule):bool=
  match f with 
	|Var x->List.assoc x environnement 
	|Top->true
	|Bot->false
	|And (x,y) ->calcul_valuation environnement x && calcul_valuation environnement y
	|Or  (x,y)->calcul_valuation environnement x || calcul_valuation environnement y
	|Not x->not (calcul_valuation environnement x)

(*Entrée: liste booléen représentant un entier en binaire
  Sortie: même entier en binaire incrémenté de 1
	!! Ne change pas la taille de la liste => ex: 15= 1111 et add_one [1;1;1;1]=[1;1;1;1]  *)
let rec add_one(l:bool list):bool list=
  match l with 
	|[]->[](*la retenue ne part pas quand c'est vide*)
	|x::q->if x =false then true::q else false::add_one q 



let valuation_next(environnement:valuation ):valuation option=
  let rec check_val_max(environnement:valuation ):bool=
	   match environnement with 
		 |[]->true  (*peut-être à changer dans le futur*)
		 |(x,valu)::q->if valu=true then check_val_max q else false 
	in if (check_val_max environnement =true )then None  
	else let (liste_var,liste_bool)=List.split environnement in let liste_bool_final=add_one liste_bool in Some (List.combine liste_var liste_bool_final)


let valuation_init(l:string list):valuation =
  let f x =(x,false) in List.map f l 

let satsolver_naif(f:formule):sat_result=
    let list_var=calcul_var_formule f in let environnement =valuation_init list_var in
		let rec test_val(environnement:valuation )(f:formule):sat_result=
		    if (calcul_valuation environnement f =true) then Some environnement else let next_environnement=valuation_next environnement in
				match  next_environnement with
				|None->None
				|Some env->test_val env f 
		in test_val environnement f 

let rec simpl_step (f: formule): formule*bool=
	match f with
	|And(Top, x) | And(x,Top) -> (x,true)
	|And(Bot, x) | And(x,Bot) -> (Bot,true)
	|Or(Bot, x)  | Or(x,Bot) -> (x,true)
	|Or(Top, x)  | Or(x,Top) -> (Top,true)
	|Not(Not (x)) -> (x,true)
	|Not(Top) -> (Bot,true)
	|Not(Bot) -> (Top,true) 
	|And(x,y) -> let (f1,b1),(f2,b2)=simpl_step x, simpl_step y in (And(f1,f2), b1||b2)
	|Or(x,y) -> let (f1,b1),(f2,b2)=simpl_step x, simpl_step y in (Or(f1,f2), b1||b2)
	|Not(x) -> let (f1,b1)=simpl_step x in (Not(f1), b1)
	|x -> (x,false)


let rec simpl_full(f:formule):formule=
  let (f_simplifie,bool_simplification) =simpl_step f in if bool_simplification=true then simpl_full f_simplifie else f_simplifie

(*Q21*)
let rec subst(f:formule)(remplace:string)(remplacement: formule):formule=
  match f with 
  |Var x->if x=remplace then remplacement else Var x
  |And(x,y)->And(subst x  remplace remplacement,subst y remplace remplacement)
	|Or(x,y)->Or(subst x  remplace remplacement,subst y remplace remplacement)
	|Not(x)->Not(subst x  remplace remplacement)
	|Top->Top
	|Bot->Bot

(*Q22*)
let  quine(f:formule):sat_result=
  let liste_variables = calcul_var_formule f in
	(*f_sauvegarde est très importante si on a remplacé X par top mais que ça nous donne bot après simplification de la nouvelle formule, alors on révalue la formule avant le changement qui est f_sauvegarde en remplacement X par Bot *)
  let rec remplacement_quine(f :formule)(f_sauvegarde:formule)(valuation_actu:valuation)(liste_var_a_test:'a list):sat_result=
	  match f with 
	  |Top->Some valuation_actu
		|Bot->begin
		      match valuation_actu with
		        |x::q->begin
						    match x with 
						      |(variable,true)->remplacement_quine f_sauvegarde f_sauvegarde ((variable,false)::q)(liste_var_a_test) 
									|(variable,false)->None   (*non sat*)
							end
						|[]->None
			end
		|_->match liste_var_a_test with(*Dans le cas où on ne peut encore déterminer si c'est sat*)
		      |x::q->let post_chg=(subst f x Top )in remplacement_quine (simpl_full post_chg) f ((x,true)::valuation_actu) q  
					|[]->failwith "Erreur ,var a test= [] , la fonction ne marche pas. CODE A MODIFIER " 
  in remplacement_quine f f [] liste_variables 
(*Q23*)
	
let rec print_true(environnement:valuation):unit=
   match environnement with
	 |(variable,true)::q->print_string(variable);print_true q
	 |[]->print_string "\n"





(*------------------------Partie Tests des fonctiosn -------------------------------------*)
let test_parse () =
	assert (parse "a | (b & ~c)" = Or(Var "a", And(Var "b", Not (Var "c"))));
	assert(parse "a|b"=Or(Var "a",Var "b"));
   assert(parse "(a&b)|(~c & ~d)"=Or(And(Var "a",Var"b"),And(Not(Var"c"),Not(Var "d"))));
   assert(from_file "./tests/test.txt"=Or(And(Var "a",Var"b"),And(Not(Var"c"),Not(Var "d"))));
	 assert (parse "F > T" = implique(Bot,Top));
	assert (parse "T = d" = equivalence(Top,Var "d"));
   print_string "Tests OK\n";;

   

let test_compte_ops()=
  assert(compte_ops(parse " a|b")=1);
	assert(compte_ops(parse "~(a|(~b& (~c | b) )   & (~d | a)  & c)  ")=10)

let test_satsolv_naif()=
   assert(satsolver_naif(parse("a|b"))=Some[("a",true);("b",false)])
	
let test()=	
   assert(1=1);
	 test_parse();
	 test_compte_ops();
	 test_satsolv_naif();
   print_string "Tous les tests ont réussi\n"
	 
	 
let main()=
   print_int (Array.length Sys.argv ); print_string "\n\n";
   if (Array.length Sys.argv)=1 then failwith "No argument "
   else
      if Sys.argv.(1) ="test" then test()
      else
         let fichier =read_file Sys.argv.(1) in
         (**print_string fichier; 
         print_string "\n\n";
         print_string Sys.argv.(0); print_string "\n\n";
         print_string Sys.argv.(1);print_string "\n\n"
**)
  let formule=from_file fichier in
	let reponse=quine formule in 
	match reponse with
	|None->print_string"Cette formule n'est pas satisfiable!\n"
	|Some environnement->if environnement =[]then print_string "Il faut que toutes les variables soient évalués en 0 \n"
	else print_string "La formule est satisfiable en assignant 1 aux variables suivantes et 0 aux autres:\n" ;print_true environnement     
    
      
let _=main()

