;;;;;;;;;;;;;;
; Homework 2 ;
;Rosemary He
;204-753-981
;;;;;;;;;;;;;;

;;;;;;;;;;;;;;
; Question 1 ;
;;;;;;;;;;;;;;

; TODO: comment code
;BFS returns the order of which the nodes are searched in a left to right breadth-first search
;@param FRINGE list that represented the root of the tree
(defun BFS (FRINGE)
    (cond ((not FRINGE) '()) ;empty list, return nothing
        ((atom (car FRINGE)) ;first item is atom, searched first
            (append (list (car FRINGE)) (BFS (cdr FRINGE))))
        (t (BFS (append (cdr FRINGE) (car FRINGE))))
    )
)

;;;;;;;;;;;;;;
; Question 2 ;
;;;;;;;;;;;;;;

;;my implementation of DFS to solve the homer problem.

; These functions implement a depth-first solver for the homer-baby-dog-poison
; problem. In this implementation, a state is represented by a single list
; (homer baby dog poison), where each variable is T if the respective entity is
; on the west side of the river, and NIL if it is on the east side.
; Thus, the initial state for this problem is (NIL NIL NIL NIL) (everybody 
; is on the east side) and the goal state is (T T T T).

; The main entry point for this solver is the function DFS, which is called
; with (a) the state to search from and (b) the path to this state. It returns
; the complete path from the initial state to the goal state: this path is a
; list of intermediate problem states. The first element of the path is the
; initial state and the last element is the goal state. Each intermediate state
; is the state that results from applying the appropriate operator to the
; preceding state. If there is no solution, DFS returns NIL.
; To call DFS to solve the original problem, one would call 
; (DFS '(NIL NIL NIL NIL) NIL) 
; However, it should be possible to call DFS with a different initial
; state or with an initial path.

; First, we define the helper functions of DFS.

; FINAL-STATE takes a single argument S, the current state, and returns T if it
; is the goal state (T T T T) and NIL otherwise.
(defun FINAL-STATE (S)
    (equal S '(T T T T))
)

; NEXT-STATE returns the state that results from applying an operator to the
; current state. It takes three arguments: the current state (S), and which entity
; to move (A, equal to h for homer only, b for homer with baby, d for homer 
; with dog, and p for homer with poison). 
; It returns a list containing the state that results from that move.
; If applying this operator results in an invalid state (because the dog and baby,
; or poisoin and baby are left unsupervised on one side of the river), or when the
; action is impossible (homer is not on the same side as the entity) it returns NIL.
; NOTE that next-state returns a list containing the successor state (which is
; itself a list); the return should look something like ((NIL NIL T T)).
(defun NEXT-STATE (S A)
    ;first check if action is impossible if homer not on same side
    ;check if result state is invalid
    ;else perform action

        ;just homer
    (cond ((equal A 'H)
            ;if baby will be alone with dog or poison, return empty
            (cond ((and (equal (first S) (second S)) 
                    (or (equal (second S) (third S)) (equal (second S) (fourth S)))) '())
                (t (list (cons (not (car S)) (cdr S))))
            )
          )
        ;homer and baby
        ((equal A 'B)
            ;if homer not with baby
            (cond ((not (equal (first S) (second S))) '())
                (t (list (list (not (first S)) (not (second S)) (third S) (fourth S))))
            )
        )
        ;homer and dog
        ((equal A 'D)
            ;if homer not with dog
            (cond ((not (equal (first S) (third S))) '())
                ;if baby will be with poison
                ((equal (second S) (fourth S)) '())
                (t (list (list (not (first S)) (second S) (not (third S)) (fourth S))))
            )
        )
        ;homer and poison
        ((equal A 'P)
            ;if homer not with poison
            (cond ((not (equal (first S) (fourth S))) '())
                ;if baby will be with dog
                ((equal (second S) (third S)) '())
                (t (list (list (not (first S)) (second S) (third S) (not (fourth S)))))
            )
        )
    )
)

; SUCC-FN returns all of the possible legal successor states to the current
; state. It takes a single argument (s), which encodes the current state, and
; returns a list of each state that can be reached by applying legal operators
; to the current state.
(defun SUCC-FN (S)
    (append (NEXT-STATE S 'h) (NEXT-STATE S 'b) (NEXT-STATE S 'd) (NEXT-STATE S 'p))
)

; ON-PATH checks whether the current state is on the stack of states visited by
; this depth-first search. It takes two arguments: the current state (S) and the
; stack of states visited by DFS (STATES). It returns T if s is a member of
; states and NIL otherwise.
(defun ON-PATH (S STATES)
    ;if null then nil, else recurse
    (cond ((not STATES) '())
        ((equal S (car STATES)) T)
        (t (ON-PATH S (cdr STATES)))
    )
)

; MULT-DFS is a helper function for DFS. It takes two arguments: a list of
; states from the initial state to the current state (PATH), and the legal
; successor states to the last, current state in the PATH (STATES). PATH is a
; first-in first-out list of states; that is, the first element is the initial
; state for the current search and the last element is the most recent state
; explored. MULT-DFS does a depth-first search on each element of STATES in
; turn. If any of those searches reaches the final state, MULT-DFS returns the
; complete path from the initial state to the goal state. Otherwise, it returns
; NIL.
(defun MULT-DFS (STATES PATH)
    (cond ((not STATES) '())
        ;keep track of current state
        (t (let ((current (DFS (car STATES) PATH)))
            ;if not in path then search
            (cond ((not current) (MULT-DFS (cdr STATES) PATH))
                (t current)
            ))
        )
    )
)

; DFS does a depth first search from a given state to the goal state. It
; takes two arguments: a state (S) and the path from the initial state to S
; (PATH). If S is the initial state in our search, PATH is set to NIL. DFS
; performs a depth-first search starting at the given state. It returns the path
; from the initial state to the goal state, if any, or NIL otherwise. DFS is
; responsible for checking if S is already the goal state, as well as for
; ensuring that the depth-first search does not revisit a node already on the
; search path.
(defun DFS (S PATH)
    ;if S already goal
    (cond ((FINAL-STATE S) (append PATH (list S)))
        ;if state already visited, don't explore again
        ((ON-PATH S PATH) '())
        ;perform DFS
        (t (MULT-DFS (SUCC-FN S) (append PATH (list S))))
    )
)