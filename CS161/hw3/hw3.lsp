;
; CS161 Hw3: Sokoban
; Rosemary He
; 204-753-981
; *********************
;    READ THIS FIRST
; ********************* 
;
; All functions that you need to modify are marked with 'EXERCISE' in their header comments.
; Do not modify a-star.lsp.
; This file also contains many helper functions. You may call any of them in your functions.
;
; *Warning*: The provided A* code only supports the maximum cost of 4999 for any node.
; That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your heuristic functions.
; Do not make them return anything too large.
;
; For Allegro Common Lisp users: The free version of Allegro puts a limit on memory.
; So, it may crash on some hard sokoban problems and there is no easy fix (unless you buy 
; Allegro). 
; Of course, other versions of Lisp may also crash if the problem is too hard, but the amount
; of memory available will be relatively more relaxed.
; Improving the quality of the heuristic will mitigate this problem, as it will allow A* to
; solve hard problems with fewer node expansions.
; 
; In either case, this limitation should not significantly affect your grade.
; 
; Remember that most functions are not graded on efficiency (only correctness).
; Efficiency can only influence your heuristic performance in the competition (which will
; affect your score).
;  
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
; They are not necessary for this homework.
; Use/modify them for your own convenience.
;

;
; For reloading modified code.
; I found this easier than typing (load "filename") every time. 
;
(defun reload()
  (load "hw3.lsp")
  )

;
; For loading a-star.lsp.
;
(defun load-a-star()
  (load "a-star.lsp"))

;
; Reloads hw3.lsp and a-star.lsp
;
(defun reload-all()
  (reload)
  (load-a-star)
  )

;
; A shortcut function.
; goal-test and next-states stay the same throughout the assignment.
; So, you can just call (sokoban <init-state> #'<heuristic-name>).
; 
;
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end general utility functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We now begin actual Sokoban code
;

; Define some global variables
(setq blank 0)
(setq wall 1)
(setq box 2)
(setq keeper 3)
(setq star 4)
(setq boxstar 5)
(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

;
; Helper function of getKeeperPosition
;
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
    (t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
           col
         (getKeeperColumn (cdr r) (+ col 1))
         );end if
       );end t
    );end cond
  )

;
; getKeeperPosition (s firstRow)
; Returns a list indicating the position of the keeper (c r).
; 
; Assumes that the keeper is in row >= firstRow.
; The top row is the zeroth row.
; The first (right) column is the zeroth column.
;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
    (t (let ((x (getKeeperColumn (car s) 0)))
         (if x
             ;keeper is in this row
             (list x row)
             ;otherwise move on
             (getKeeperPosition (cdr s) (+ row 1))
             );end if
           );end let
     );end t
    );end cond
  );end defun

;
; cleanUpList (l)
; returns l with any NIL element removed.
; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
;
(defun cleanUpList (L)
  (cond ((null L) nil)
    (t (let ((cur (car L))
         (res (cleanUpList (cdr L)))
         )
         (if cur 
         (cons cur res)
          res
         )
         );end let
       );end t
    );end cond
  );end 

; EXERCISE: Modify this function to return true (t)
; if and only if s is a goal state of a Sokoban game.
; (no box is on a non-goal square)
;
; Currently, it always returns NIL. If A* is called with
; this function as the goal testing function, A* will never
; terminate until the whole search space is exhausted.
;
(defun goal-test (s)
    (cond ((null s) t) ;no squares left to check, true
        ((atom s)
          (cond ((isBox s) nil) ;if box, false
            (t t) ;otherwise, true
          ))
        (t (and (goal-test (car s)) (goal-test (cdr s)))) ;else check the rest of the list
    )
);end defun

; EXERCISE: Modify this function to return the list of 
; sucessor states of s.
;
; This is the top-level next-states (successor) function.
; Some skeleton code is provided below.
; You may delete them totally, depending on your approach.
; 
; If you want to use it, you will need to set 'result' to be 
; the set of states after moving the keeper in each of the 4 directions.
; A pseudo-code for this is:
; 
; ...
; (result (list (try-move s UP) (try-move s DOWN) (try-move s LEFT) (try-move s RIGHT)))
; ...
; 
; You will need to define the function try-move and decide how to represent UP,DOWN,LEFT,RIGHT.
; Any NIL result returned from try-move can be removed by cleanUpList.
; 
;

; helper functions
; takes in State s and row number r
; return the row
(defun get-row (s r)
  (cond ((or (null s) (< r 0)) '()) ;outside range, return empty
    ((= r 0) (car s)) ;return this row
    (t (get-row (cdr s) (- r 1))) ;else iterate through
  )
)

; takes in a row s and clm number c
; return the cth element of the row
(defun get-clm (s c)
  (cond ((or (null s) (< c 0)) 1) ;outside range, return wall=1
      ((= c 0) (car s)) ;return this element
      (t (get-clm (cdr s) (- c 1))) ;else iterate through
  )
)

; takes in State s, row number r and clm number c
; return content of square(r,c)
; if outside range, return wall=1
(defun get-square (s r c)
  (get-clm (get-row s r) c)
)

; takes in row s, clm number c and value v
; return the updated row
(defun set-row (s c v)
  (cond ((or (null s) (< c 0)) '()) ;null clm or out of range
    ((= c 0) (cons v (cdr s))) ;change this element
    (t (cons (car s) (set-row (cdr s) (- c 1) v))) ;else keep going
  )
)

; takes in State s, row number r, clm number c and square int v
; return new updated State s'
(defun set-square (s r c v)
  (cond ((or (null s) (< r 0)) '()) ;null row or out of range
    ((= r 0) (cons (set-row (car s) c v) (cdr s))) ;change this row
    (t (cons (car s) (set-square (cdr s) (- r 1) c v))) ;else keep going
  )
)

; takes in State s, move direction d, the keeper coordinates (x,y)
; return updated state of moving in that dir
; basic idea: only these scenarios do we move:
; 1. keeper in pos, then 1. oneaway is blank or star, just move; 2. if one away is box/boxstar AND twoaway is blank or star, then move
; 2. keeperstar in pos, then same logic as above
; all else are invalid moves
(defun try-move (s x y dir)
  (cond 
    ((equal dir 'up)
      (let* ((now (get-square s x y))
        (oneup (get-square s (- x 1) y))
        (twoup (get-square s (- x 2) y)))
          (cond
            ((isKeeper now)
                (cond
                    ((isBlank oneup) (set-square (set-square s (- x 1) y keeper) x y blank))
                    ((isStar oneup) (set-square (set-square s (- x 1) y keeperstar) x y blank))
                    ((and (isBox oneup) (isBlank twoup)) (set-square (set-square (set-square s (- x 2) y box) (- x 1) y keeper) x y blank))
                    ((and (isBox oneup) (isStar twoup)) (set-square (set-square (set-square s (- x 2) y boxstar) (- x 1) y keeper) x y blank))
                    ((and (isBoxStar oneup) (isBlank twoup)) (set-square (set-square (set-square s (- x 2) y box) (- x 1) y keeperstar) x y blank))
                    ((and (isBoxStar oneup) (isStar twoup)) (set-square (set-square (set-square s (- x 2) y boxstar) (- x 1) y keeperstar) x y blank))
                    (t '()) ;any other situation is illegal to move
                ))
            ((isKeeperStar now)
                (cond
                    ((isBlank oneup) (set-square (set-square s (- x 1) y keeper) x y star))
                    ((isStar oneup) (set-square (set-square s (- x 1) y keeperstar) x y star))
                    ((and (isBox oneup) (isBlank twoup)) (set-square (set-square (set-square s (- x 2) y box) (- x 1) y keeper) x y star))
                    ((and (isBox oneup) (isStar twoup)) (set-square (set-square (set-square s (- x 2) y boxstar) (- x 1) y keeper) x y star))
                    ((and (isBoxStar oneup) (isBlank twoup)) (set-square (set-square (set-square s (- x 2) y box) (- x 1) y keeperstar) x y star))
                    ((and (isBoxStar oneup) (isStar twoup)) (set-square (set-square (set-square s (- x 2) y boxstar) (- x 1) y keeperstar) x y star))
                    (t '()) ;any other situation is illegal to move
                ))
        )))

    ((equal dir 'down)
      (let* ((now (get-square s x y))
        (onedown (get-square s (+ x 1) y))
        (twodown (get-square s (+ x 2) y)))
          (cond
            ((isKeeper now)
                (cond
                    ((isBlank onedown) (set-square (set-square s (+ x 1) y keeper) x y blank))
                    ((isStar onedown) (set-square (set-square s (+ x 1) y keeperstar) x y blank))
                    ((and (isBox onedown) (isBlank twodown)) (set-square (set-square (set-square s (+ x 2) y box) (+ x 1) y keeper) x y blank))
                    ((and (isBox onedown) (isStar twodown)) (set-square (set-square (set-square s (+ x 2) y boxstar) (+ x 1) y keeper) x y blank))
                    ((and (isBoxStar onedown) (isBlank twodown)) (set-square (set-square (set-square s (+ x 2) y box) (+ x 1) y keeperstar) x y blank))
                    ((and (isBoxStar onedown) (isStar twodown)) (set-square (set-square (set-square s (+ x 2) y boxstar) (+ x 1) y keeperstar) x y blank))
                    (t '()) ;any other situation is illegal to move
                ))
            ((isKeeperStar now)
                (cond
                    ((isBlank onedown) (set-square (set-square s (+ x 1) y keeper) x y star))
                    ((isStar onedown) (set-square (set-square s (+ x 1) y keeperstar) x y star))
                    ((and (isBox onedown) (isBlank twodown)) (set-square (set-square (set-square s (+ x 2) y box) (+ x 1) y keeper) x y star))
                    ((and (isBox onedown) (isStar twodown)) (set-square (set-square (set-square s (+ x 2) y boxstar) (+ x 1) y keeper) x y star))
                    ((and (isBoxStar onedown) (isBlank twodown)) (set-square (set-square (set-square s (+ x 2) y box) (+ x 1) y keeperstar) x y star))
                    ((and (isBoxStar onedown) (isStar twodown)) (set-square (set-square (set-square s (+ x 2) y boxstar) (+ x 1) y keeperstar) x y star))
                    (t '()) ;any other situation is illegal to move
                ))
        )))

    ((equal dir 'left)
      (let* ((now (get-square s x y))
        (oneleft (get-square s x (- y 1)))
        (twoleft (get-square s x (- y 2))))
          (cond
            ((isKeeper now)
                (cond
                    ((isBlank oneleft) (set-square (set-square s x (- y 1) keeper) x y blank))
                    ((isStar oneleft) (set-square (set-square s x (- y 1) keeperstar) x y blank))
                    ((and (isBox oneleft) (isBlank twoleft)) (set-square (set-square (set-square s x (- y 2) box) x (- y 1) keeper) x y blank))
                    ((and (isBox oneleft) (isStar twoleft)) (set-square (set-square (set-square s x (- y 2) boxstar) x (- y 1) keeper) x y blank))
                    ((and (isBoxStar oneleft) (isBlank twoleft)) (set-square (set-square (set-square s x (- y 2) box) x (- y 1) keeperstar) x y blank))
                    ((and (isBoxStar oneleft) (isStar twoleft)) (set-square (set-square (set-square s x (- y 2) boxstar) x (- y 1) keeperstar) x y blank))
                    (t '()) ;any other situation is illegal to move
                ))
            ((isKeeperStar now)
                (cond
                    ((isBlank oneleft) (set-square (set-square s x (- y 1) keeper) x y star))
                    ((isStar oneleft) (set-square (set-square s x (- y 1) keeperstar) x y star))
                    ((and (isBox oneleft) (isBlank twoleft)) (set-square (set-square (set-square s x (- y 2) box) x (- y 1) keeper) x y star))
                    ((and (isBox oneleft) (isStar twoleft)) (set-square (set-square (set-square s x (- y 2) boxstar) x (- y 1) keeper) x y star))
                    ((and (isBoxStar oneleft) (isBlank twoleft)) (set-square (set-square (set-square s x (- y 2) box) x (- y 1) keeperstar) x y star))
                    ((and (isBoxStar oneleft) (isStar twoleft)) (set-square (set-square (set-square s x (- y 2) boxstar) x (- y 1) keeperstar) x y star))
                    (t '()) ;any other situation is illegal to move
                ))
        )))

    ((equal dir 'right)
      (let* ((now (get-square s x y))
        (oneright (get-square s x (+ y 1)))
        (tworight (get-square s x (+ y 2))))
          (cond
            ((isKeeper now)
                (cond
                    ((isBlank oneright) (set-square (set-square s x (+ y 1) keeper) x y blank))
                    ((isStar oneright) (set-square (set-square s x (+ y 1) keeperstar) x y blank))
                    ((and (isBox oneright) (isBlank tworight)) (set-square (set-square (set-square s x (+ y 2) box) x (+ y 1) keeper) x y blank))
                    ((and (isBox oneright) (isStar tworight)) (set-square (set-square (set-square s x (+ y 2) boxstar) x (+ y 1) keeper) x y blank))
                    ((and (isBoxStar oneright) (isBlank tworight)) (set-square (set-square (set-square s x (+ y 2) box) x (+ y 1) keeperstar) x y blank))
                    ((and (isBoxStar oneright) (isStar tworight)) (set-square (set-square (set-square s x (+ y 2) boxstar) x (+ y 1) keeperstar) x y blank))
                    (t '()) ;any other situation is illegal to move
                ))
            ((isKeeperStar now)
                (cond
                    ((isBlank oneright) (set-square (set-square s x (+ y 1) keeper) x y star))
                    ((isStar oneright) (set-square (set-square s x (+ y 1) keeperstar) x y star))
                    ((and (isBox oneright) (isBlank tworight)) (set-square (set-square (set-square s x (+ y 2) box) x (+ y 1) keeper) x y star))
                    ((and (isBox oneright) (isStar tworight)) (set-square (set-square (set-square s x (+ y 2) boxstar) x (+ y 1) keeper) x y star))
                    ((and (isBoxStar oneright) (isBlank tworight)) (set-square (set-square (set-square s x (+ y 2) box) x (+ y 1) keeperstar) x y star))
                    ((and (isBoxStar oneright) (isStar tworight)) (set-square (set-square (set-square s x (+ y 2) boxstar) x (+ y 1) keeperstar) x y star))
                    (t '()) ;any other situation is illegal to move
                ))
        )))
  )
)

; Actual function
; try moves in all direction and return valid states
(defun next-states (s)
  (let* ((pos (getKeeperPosition s 0))
     (x (car pos))
     (y (cadr pos)))
     ;x and y are now the coordinate of the keeper in s.
     (cleanUpList (list (try-move s y x 'up) (try-move s y x 'right) (try-move s y x 'down) (try-move s y x 'left)))
   );end let
);

; EXERCISE: Modify this function to compute the trivial 
; admissible heuristic.
;
; trivially admissable: just returns 0
(defun h0 (s)
  0
)

; EXERCISE: Modify this function to compute the 
; number of misplaced boxes in s.
; 
; admissable since we count each move as 1, then for 
; any misplaced boxes it costs at least 1 to get to
; goal state, therefore h(n) < f(n) for all n
(defun h1 (s)
  (cond ((null s) 0) ;no boxes left, return 0
    ((atom s) (cond ((isBox s) 1) ;return 1
        (t 0)))
    (t (+ (h1 (car s)) (h1 (cdr s))))
  )
)

; helper function for find_all_stars in the row number r
; returns a tuple of all positions if star
(defun get-star (s row c r)
    (cond ((null row) '())
        ((or (isStar (car row)) (isKeeperStar (car row))) (cons (list r c) (get-star s (cdr row) (+ c 1) r)))
        (t (get-star s (cdr row) (+ c 1) r))
    )
)

; helper for heuristic func
; find all emtpy stars on board above and beyond the row number r
; returns a list of tuples of each coordinate of star
(defun find_all_stars (s r)
    (let* ((row (get-row s r)))
        (cond ((or (null s) (null row)) '())
            (t (cleanUpList (append (get-star s row 0 r) (find_all_stars s (+ r 1)))))
        )
    )
)

; helper function for find_all_boxes in the row number r
; returns a tuple of all positions if box
(defun get-box (s row c r)
    (cond ((null row) '())
        ((isBox (car row)) (cons (list r c) (get-box s (cdr row) (+ c 1) r)))
        (t (get-box s (cdr row) (+ c 1) r))
    )
)

; helper for heuristic func
; find all emtpy boxes on board above and beyond the row number r
; returns a list of tuples of each coordinate of box
(defun find_all_boxes (s r)
    (let* ((row (get-row s r)))
        (cond ((or (null s) (null row)) '())
            (t (cleanUpList (append (get-box s row 0 r) (find_all_boxes s (+ r 1)))))
        )
    )
)

; helper func for find-closest-star
; takes in tuples of box and star coordinates
; return their distance
(defun calc_dist (box star)
    (cond ((or (null box) (null star)) 0)
        (t (let* 
            ((box_x (first box)) (box_y (second box)) (star_x (first star)) (star_y (second star)))
            (+ (abs (- box_x star_x)) (abs (- box_y star_y)))
        ))
    )
)

; helper for find_dist func
; takes in the coordinates of the box and list of empty stars coordinates
; finds the distance the closest star
(defun find_closest_star (s box star)
    (cond ((null star) 1000)
        (t (let* (
                    (cur (calc_dist box (car star)))
                    (next (find_closest_star s box (cdr star)))
                )
            (cond ((< next cur) next)
                (t cur))
        ))
    )
)

; helper func
; takes in list of stars, list of boxes in tuples
; return the total distance to get each box to closest star
(defun find_dist (s box star)
    (cond ((or (null star) (null box)) 0)
        ((= (length box) 1) (find_closest_star s (car box) star))
        (t (+ (find_closest_star s (car box) star) (find_dist s (cdr box) star)))
    )
)

; EXERCISE: Change the name of this function to h<UID> where
; <UID> is your actual student ID number. Then, modify this 
; function to compute an admissible heuristic value of s. 
; 
; This function will be entered in the competition.
; Objective: make A* solve problems as fast as possible.
; The Lisp 'time' function can be used to measure the 
; running time of a function call.
;
; will use the calculation of number of steps from box to closest star
; admissable because for each box not in star, it needs at least the num
; of steps to the closest star to get to any star on the map
(defun h204753981 (s)
    (let* ((starlist (find_all_stars s 0)) (boxlist (find_all_boxes s 0)))
        (cond ((or (null starlist) (null boxlist)) 0)
            (t (find_dist s boxlist starlist))
        )
    )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Some predefined problems.
 | Each problem can be visualized by calling (printstate <problem>). For example, (printstate p1).
 | Problems are roughly ordered by their difficulties.
 | For most problems, we also privide 2 additional number per problem:
 |    1) # of nodes expanded by A* using our next-states and h0 heuristic.
 |    2) the depth of the optimal solution.
 | These numbers are located at the comments of the problems. For example, the first problem below 
 | was solved by 80 nodes expansion of A* and its optimal solution depth is 7.
 | 
 | Your implementation may not result in the same number of nodes expanded, but it should probably
 | give something in the same ballpark. As for the solution depth, any admissible heuristic must 
 | make A* return an optimal solution. So, the depths of the optimal solutions provided could be used
 | for checking whether your heuristic is admissible.
 |
 | Warning: some problems toward the end are quite hard and could be impossible to solve without a good heuristic!
 | 
 |#

;(80,7)
(setq p1 '((1 1 1 1 1 1)
       (1 0 3 0 0 1)
       (1 0 2 0 0 1)
       (1 1 0 1 1 1)
       (1 0 0 0 0 1)
       (1 0 0 0 4 1)
       (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
       (1 0 0 0 0 0 1) 
       (1 0 0 0 0 0 1) 
       (1 0 0 2 1 4 1) 
       (1 3 0 0 1 0 1)
       (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
       (1 0 0 0 1 0 0 0 1)
       (1 0 0 0 2 0 3 4 1)
       (1 0 0 0 1 0 0 0 1)
       (1 0 0 0 1 0 0 0 1)
       (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
       (0 0 0 0 0 1 4)
       (0 0 0 0 0 0 0)
       (0 0 1 1 1 0 0)
       (0 0 1 0 0 0 0)
       (0 2 1 0 0 0 0)
       (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
       (1 1 0 0 1 1)
       (1 0 0 0 0 1)
       (1 4 2 2 4 1)
       (1 0 0 0 0 1)
       (1 1 3 1 1 1)
       (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
       (1 0 0 0 0 0 4 1)
       (1 0 0 0 2 2 3 1)
       (1 0 0 1 0 0 4 1)
       (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
       (0 0 1 1 1 1 0 0 0 3)
       (0 0 0 0 0 1 0 0 0 0)
       (0 0 0 0 0 1 0 0 1 0)
       (0 0 1 0 0 1 0 0 1 0)
       (0 2 1 0 0 0 0 0 1 0)
       (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
       (1 4 0 0 4 1)
       (1 0 2 2 0 1)
       (1 2 0 1 0 1)
       (1 3 0 0 4 1)
       (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
       (1 1 1 0 0 1 1 1 1) 
       (1 0 0 0 0 0 2 0 1) 
       (1 0 1 0 0 1 2 0 1) 
       (1 0 4 0 4 1 3 0 1) 
       (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
        (1 0 0 0 1 1 0)
        (1 3 2 0 0 1 1)
        (1 1 0 2 0 0 1)
        (0 1 1 0 2 0 1)
        (0 0 1 1 0 0 1)
        (0 0 0 1 1 4 1)
        (0 0 0 0 1 4 1)
        (0 0 0 0 1 4 1)
        (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
        (1 4 0 0 0 4 1)
        (1 0 2 2 1 0 1)
        (1 0 2 0 1 3 1)
        (1 1 2 0 1 0 1)
        (1 4 0 0 4 0 1)
        (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
        (1 1 1 1 1 0 0 0 1 1 1 1)
        (1 0 0 0 2 0 0 0 0 0 0 1)
        (1 3 0 0 0 0 0 0 0 0 0 1)
        (1 0 0 0 2 1 1 1 0 0 0 1)
        (1 0 0 0 0 1 0 1 4 0 4 1)
        (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
        (1 4 0 0 0 0 0 2 0 1)
        (1 0 2 0 0 0 0 0 4 1)
        (1 0 3 0 0 0 0 0 2 1)
        (1 0 0 0 0 0 0 0 0 1)
        (1 0 0 0 0 0 0 0 4 1)
        (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
        (0 2 1 4 0 0 0)
        (0 2 0 4 0 0 0)    
        (3 2 1 1 1 0 0)
        (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
        (1 0 0 0 0 0 1)
        (1 0 0 2 2 0 1)
        (1 0 2 0 2 3 1)
        (1 4 4 1 1 1 1)
        (1 4 4 1 0 0 0)
        (1 1 1 1 0 0 0)
        ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
        (1 0 0 0 1 0 0 0)
        (1 2 1 0 1 1 1 1)
        (1 4 0 0 0 0 0 1)
        (1 0 0 5 0 5 0 1)
        (1 0 5 0 1 0 1 1)
        (1 1 1 0 3 0 1 0)
        (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
        (1 3 0 0 1 0 0 0 4 1)
        (1 0 2 0 2 0 0 4 4 1)
        (1 0 2 2 2 1 1 4 4 1)
        (1 0 0 0 0 1 1 4 4 1)
        (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
        (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
        (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
        (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
        (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
        (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
        (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
        (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
        (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
        (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
        (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
        (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
        (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)       
        (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
        ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
        (0 0 0 1 0 0 0 0 1 0 0 0)
        (0 0 0 1 0 0 0 0 1 0 0 0)
        (1 1 1 1 0 0 0 0 1 1 1 1)
        (0 0 0 0 1 0 0 1 0 0 0 0)
        (0 0 0 0 0 0 3 0 0 0 2 0)
        (0 0 0 0 1 0 0 1 0 0 0 4)
        (1 1 1 1 0 0 0 0 1 1 1 1)
        (0 0 0 1 0 0 0 0 1 0 0 0)
        (0 0 0 1 0 0 0 0 1 0 0 0)
        (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
        (1 1 1 1 0 0 1 1 0)
        (1 0 0 0 2 0 0 1 0)
        (1 0 0 5 5 5 0 1 0)
        (1 0 0 4 0 4 0 1 1)
        (1 1 0 5 0 5 0 0 1)
        (0 1 1 5 5 5 0 0 1)
        (0 0 1 0 2 0 1 1 1)
        (0 0 1 0 3 0 1 0 0)
        (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
        (1 1 1 0 0 1 1 1 1 0)
        (1 0 0 2 0 0 0 1 1 0)
        (1 3 2 0 2 0 0 0 1 0)
        (1 1 0 2 0 2 0 0 1 0)
        (0 1 1 0 2 0 2 0 1 0)
        (0 0 1 1 0 2 0 0 1 0)
        (0 0 0 1 1 1 1 0 1 0)
        (0 0 0 0 1 4 1 0 0 1)
        (0 0 0 0 1 4 4 4 0 1)
        (0 0 0 0 1 0 1 4 0 1)
        (0 0 0 0 1 4 4 4 0 1)
        (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
        (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
        (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
        (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
        (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
        (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
        (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
        (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
        (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
        (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
        (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

;
; Helper function of prettyMoves
; from s1 --> s2
;
(defun detectDiff (s1 s2)
  (let* ((k1 (getKeeperPosition s1 0))
     (k2 (getKeeperPosition s2 0))
     (deltaX (- (car k2) (car k1)))
     (deltaY (- (cadr k2) (cadr k1)))
     )
    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
      (t (if (> deltaX 0) 'RIGHT 'LEFT))
      );end cond
    );end let
  );end defun

;
; Translates a list of states into a list of moves.
; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
;
(defun prettyMoves (m)
  (cond ((null m) nil)
    ((= 1 (length m)) (list 'END))
    (t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
    );end cond
  );

;
; Print the content of the square to stdout.
;
(defun printSquare (s)
  (cond ((= s blank) (format t " "))
    ((= s wall) (format t "#"))
    ((= s box) (format t "$"))
    ((= s keeper) (format t "@"))
    ((= s star) (format t "."))
    ((= s boxstar) (format t "*"))
    ((= s keeperstar) (format t "+"))
    (t (format t "|"))
    );end cond
  )

;
; Print a row
;
(defun printRow (r)
  (dolist (cur r)
    (printSquare cur)    
    )
  );

;
; Print a state
;
(defun printState (s)
  (progn    
    (dolist (cur s)
      (printRow cur)
      (format t "~%")
      )
    );end progn
  )

;
; Print a list of states with delay.
;
(defun printStates (sl delay)
  (dolist (cur sl)
    (printState cur)
    (sleep delay)
    );end dolist
  );end defun
